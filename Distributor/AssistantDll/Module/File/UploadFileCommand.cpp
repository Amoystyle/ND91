#include "stdafx.h"

#include "UploadFileCommand.h"
#include "DirectoryOperationCommand.h"

#include "Core/DeviceData.h"
#include "Device/IIosFileSystem.h"
#include "Device/AdbUploadFileCommand.h"
#include "Device/AFCAppFileSystem.h"

const int FILE_COMMAND_LENGTH = 4;
const char* const FILE_COMMAND_SEND = "SEND";
const char* const FILE_COMMAND_DATA = "DATA";
const char* const FILE_COMMAND_DONE = "DONE";
const char* const FILE_COMMAND_MODE = ",01A4";
const char* const FILE_COMMAND_OKAY = "OKAY";
const char* const FILE_COMMAND_FAIL = "FAIL";

const int USB_BUFFER_SIZE = 0x10000;
const int ASYNC_BUFF_SIZE = 10240;


UploadFileCommand::UploadFileCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
    _uploadSuccess = Transfer_Unknown;
    _fileSize = 0;

	_bSetAttr = true;

	_nAndroidChannelID = PUSHFILE;

	_pAdbUpload     = NULL;
    _pDirCmd        = NULL;
    //_pSetAttrCmd    = NULL;
    _pIosFile       = NULL;
	_OffsetAppInExe =0;
}


UploadFileCommand::~UploadFileCommand()
{
	SAFE_DELETE(_pAdbUpload);
    SAFE_DELETE(_pDirCmd);
    //SAFE_DELETE(_pSetAttrCmd);
    SAFE_DELETE(_pIosFile);

}

void UploadFileCommand::SetParam(__int64 OffsetAppInExe,__int64 len)
{
	if(len>0)
		_fileSize = (int)len;

	_OffsetAppInExe = OffsetAppInExe;
}

void UploadFileCommand::setfilename(const wstring& srcfile, const wstring& destfile)
{
	if (!this->_pDevice || !this->_pDevice->GetDevInfo())
		return ;

	if ( this->_pDevice->GetDevInfo()->_deviceType == Ios)
	{
		_fileOnPc = srcfile;
		_fileonphone = destfile;
	}
	else if ( this->_pDevice->GetDevInfo()->_deviceType == Android )
	{
		if (! CFileOperation::IsFileExist(srcfile.c_str()))
		{
			THROW_ASSISTANT_EXCEPTION(FILE_NOT_FOUND);
		}
// 		if(_fileSize == 0)
// 			_fileSize = CFileOperation::GetFileSizeW(srcfile);
        if(_OffsetAppInExe == 0)
            _fileSize = CFileOperation::GetFileSizeW(srcfile);

		_fileOnPc = srcfile;
		_fileonphone = destfile;

		SetCurrentTaskSum(_fileSize);
	}
}

void UploadFileCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader  bufferReader(data.c_str(), data.length());
    if( data.length()<1 || ! parseAndroidPackageHead(&bufferReader))
	{
		 _uploadSuccess = Transfer_Fail;
        return;
	}
	_uploadSuccess = Transfer_Success;
}


bool UploadFileCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
{
	pReader->ReadBuffer(_pReturnHead, 4);
	pReader->ReadShort(_nReturnFlag);
	pReader->ReadShort(_nReturnLongConnectSign);
	pReader->ReadInt32(_nReturnBodySize);
	pReader->ReadInt32(_nReturnChannelID);
	//如果返回的数据包信息不符合本Command的信息，则跳过
	if (_nReturnChannelID != _nAndroidChannelID)
		return false;
	return true;
}

std::string UploadFileCommand::GetAndroidBodyData()
{
    // 文件上传是一串数据包，此方法无用，不实现
    return "";
}

std::string UploadFileCommand::GetWifiHeadData()
{

	string temp = CCodeOperation::UnicodeToUTF_8(_fileonphone);
    BinaryBufferWriter writer;

	writer.WriteBuffer(_pHead, 4);
	writer.WriteShort(_nFlag);
	writer.WriteShort(_pDevice->GetDevInfo()->_nLongConnectSign);
	writer.WriteInt(4+temp.length()+_fileSize);
    writer.WriteInt(PUSHFILE);

    writer.WriteInt(temp.length());
    writer.WriteBuffer(temp.c_str(), temp.length());
    return writer.GetBuffer();
}

void UploadFileCommand::Execute()
{
    _uploadSuccess = Transfer_Unknown;

    RETURN_IF( _fileOnPc.empty() || _fileonphone.empty() );

	// 存在同名文件检查
	//是否加个删除重新上传？？
    if( !CheckExistFile() )
	{
		_uploadSuccess = Transfer_Cancel;
		return;
	}

	if (!_pDevice || !_pDevice->GetDevInfo())
		return ;

	if ( _pDevice->GetDevInfo()->_deviceType == Ios)
	{
        if (!_pDevice->GetShareAppIdentifier().empty())
            SAFE_DELETE(_pIosFile);

		if(_OffsetAppInExe ==0)
			 _uploadSuccess = GetIosFile()->Ios_CopyToPhone(_fileOnPc, _fileonphone, *this);
		else
		  _uploadSuccess = GetIosFile()->Ios_CopyToPhoneEx(_fileOnPc, _fileonphone ,_OffsetAppInExe,_fileSize,*this);

        if (_uploadSuccess == Transfer_Success)
        {
			if (_bSetAttr)
			{
				/*GetSetAttrCmd()->SetPath(_fileonphone);
				GetSetAttrCmd()->Execute();*/
				GetDirCmd()->ChangeMode(_fileonphone, "644", L"501"); // 权限: rw-r--r--, 默认用户: mobile
			}
        }
	}
	else if (_pDevice->GetDevInfo()->_deviceType == Android)
	{

		if(_pDevice->GetDevInfo()->_connectType == WIFI)
		{
			SetAndroidReturnData(WifiExecute());
		}
		else
		{
            GetUploadCmd()->SetParam(_fileOnPc, _fileonphone,_OffsetAppInExe,_fileSize);
			GetUploadCmd()->Execute();
			_uploadSuccess = GetUploadCmd()->upLoadResult();
			GetDirCmd()->ChangeMode(_fileonphone, "644");
		}
	}

}

TransferResult UploadFileCommand::GetUploadResult()
{
    return _uploadSuccess;
}

bool UploadFileCommand::CheckExistFile()
{
    RETURN_TRUE_IF ( ! GetDirCmd()->Exist(_fileonphone) );

    switch(AskUser(_fileonphone))
    {
    case CANCEL:
        return false;

    case OVERWRITE:
    case OVERWRITE_ALL:
       // GetDirCmd()->DeleteFileLink(_fileonphone);
        return true;

    case RENAME:
    case RENAME_ALL:
        _fileonphone = DirData::PhoneFileRename(_fileonphone, GetDirCmd());
        return true;

    default:
        return false;
    }
}

string UploadFileCommand::WifiExecute()
{
	string result = "";
	ifstream  ifs;
	int lastOffset = 0;

    ifs.open(CCodeOperation::WstringToString(_fileOnPc).c_str(), ios::binary|ios::in);
	if ( !ifs.is_open() || !ifs.good())
	{
		THROW_ASSISTANT_EXCEPTION(OPEN_FILE_FAIL);
	}

	if(ifs.eof() || _fileSize <= 0)
	{
		result = GetIO()->SendAndroidDatagram(GetWifiHeadData());
	}
	else
	{
        SetCurrentTaskSum(_fileSize);
        SetCurrentTaskName(_fileonphone);

		GetIO()->SendAndroidDatagram(GetWifiHeadData(), false);

		char* pbuffer = new char[ASYNC_BUFF_SIZE + 1];
		//modify by wengcunmin 2012增加从文件中间某个数据段读取app包
		int  curOffset = (int)_OffsetAppInExe;
		while(curOffset <=_OffsetAppInExe + _fileSize)
		{
			size_t buffsize = 0;

            ProgressObserver* pObserver = NULL;
            if (!_observers.empty())
                pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

            IF_CANCEL(pObserver)
		    {
			    _uploadSuccess = Transfer_Cancel;
			    break;
		    }

			buffsize = (size_t)(_OffsetAppInExe + _fileSize-curOffset);
			buffsize = buffsize > ASYNC_BUFF_SIZE?ASYNC_BUFF_SIZE:buffsize;
			ifs.read(pbuffer, buffsize);
			curOffset += buffsize;
			ifs.seekg(curOffset, ios::beg);

			// 当进度变化超过百分之一时，才发送进度数据
			if( (curOffset - lastOffset)*100/ GetLastProgress()->_currentTaskSum >= 1)
			{
                AddTaskProgress(curOffset - lastOffset);
                SendProgress();

                lastOffset = curOffset;
			}

			string data(pbuffer,buffsize);
			if (buffsize > 0 && curOffset < _OffsetAppInExe+_fileSize)
			{
				GetIO()->SendAndroidDatagram(data,false);
			}
			else
			{
				result = GetIO()->SendAndroidDatagram(data);
				_uploadSuccess = Transfer_Success;
				Complete();     // 发送完成
				break;
			}
		}
		ifs.close();
		delete[] pbuffer;
	}

	return result;

}


DirectoryOperationCommand* ND91Assistant::UploadFileCommand::GetDirCmd()
{
    if ( ! _pDirCmd )
        _pDirCmd = new DirectoryOperationCommand(_pDevice);

    return _pDirCmd;
}

/*SetAttributeCommand* ND91Assistant::UploadFileCommand::GetSetAttrCmd()
{
    if ( ! _pSetAttrCmd )
    {
        _pSetAttrCmd = new SetAttributeCommand(_pDevice,
            L"",        // 先设为空, 执行命令前再设置
            L"644",      // 权限: rw-r--r--
            L"501");     // 默认用户: mobile
    }
    return _pSetAttrCmd;
}*/

AdbUploadFileCommand* ND91Assistant::UploadFileCommand::GetUploadCmd()
{
    if ( ! _pAdbUpload )
    {
        _pAdbUpload = new AdbUploadFileCommand(
            _pDevice->GetDevInfo()->_strSerialNumber, _pDevice->GetDevInfo()->_ios_deviceIdentifier,
            L"", L"", *this);
    }

    return _pAdbUpload;
}

IIosFileSystem* ND91Assistant::UploadFileCommand::GetIosFile()
{
	if (!this->_pDevice || !this->_pDevice->GetDevInfo())
		return NULL;

    ProgressObserver* pObserver = NULL;
    if (!_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

    if ( ! _pIosFile )
    {
        if ( _pDevice->GetDevInfo()->_connectType == WIFI)
        {
            _pIosFile =  new IosWiFiFileSystem(_pDevice->GetDevInfo()->_ios_wifi_handle);
        }
        else if (_pDevice->GetDevInfo()->_connectType == USB)
        {
            if (!_pDevice->GetShareAppIdentifier().empty())
                _pIosFile =  AFCAppFileSystem::StartAPPHouseArrestService(_pDevice->GetShareAppIdentifier(), 
                                                                          (DeviceInfo*)_pDevice->GetDevInfo());
            else
                _pIosFile =  new AFCFileSysemBase(_pDevice->GetDevInfo());
        }
        else
            THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
    }

    return _pIosFile;
}

std::wstring ND91Assistant::UploadFileCommand::GetDestFilePath()
{
    return _fileonphone;
}
