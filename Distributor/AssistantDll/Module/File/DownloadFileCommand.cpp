#include "stdafx.h"

#include "DownloadFileCommand.h"

#include "Core/DeviceData.h"
#include "Core/DeviceManager.h"
#include "Device/AdbDownloadFileCommand.h"
#include "Device/IosWiFiFileSystem.h"
#include "Device/AFCAppFileSystem.h"
#include "Device/AndroidTcpIO.h"
#include "Common/IosPngConver.h"

const  int TRANSFER_ACTION_PULL_FILE  = 2;

DownloadFileCommand::DownloadFileCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
	_DownResult = Transfer_Unknown;
	_pAdbDown = NULL;

	// Android
	_nAndroidChannelID = FILETRANSFER;
	_nAndroidCommandID = TRANSFER_ACTION_PULL_FILE;    
}

DownloadFileCommand::~DownloadFileCommand()
{
	SAFE_DELETE(_pAdbDown);
}

void DownloadFileCommand::SetFileNames( const wstring& filetoPc, const wstring& filefromPhone )
{
	_FilefromPhone = filefromPhone;
    _FileToPc = filetoPc;

	CStrOperation::ReplaceW(_FilefromPhone,L"\\", L"/");
	CStrOperation::ReplaceW(_FileToPc, L"/", L"\\");

	if (_FileToPc[_FileToPc.length()-1] == L'\\')
	{
		_FileToPc = _FileToPc + CFileOperation::GetFileName(_FilefromPhone);
	}

	if (_FileToPc.length() < 1 || _FilefromPhone.length() < 1 
		|| _FileToPc.length() >= 260 || _FilefromPhone.length() >= 260)
	{

		THROW_ASSISTANT_EXCEPTION(INVALID_FILE_NAME);
	}

}

void ND91Assistant::DownloadFileCommand::SetAndroidReturnData(std::string data)
{

}

std::string DownloadFileCommand::GetAndroidBodyData()
{
	//if(_pDevice->GetDevInfo()->_connectType == WIFI)
	{
		//string pathutf8 = CCodeOperation::UnicodeToUTF_8(_FilefromPhone);
		BinaryBufferWriter  writer;
		//writer.WriteBuffer(_pHead,4);
		//writer.WriteInt((int)_nFlag);
		//writer.WriteInt(pathutf8.length()+12);
		writer.WriteInt(18);
		writer.WriteInt(2);
		writer.WriteStringAsUTF8(_FilefromPhone);
		//writer.WriteInt(pathutf8.length());
		//writer.WriteBuffer(pathutf8.c_str(), pathutf8.length());

		return writer.GetBuffer();
	}
	//return  "";
}

void ND91Assistant::DownloadFileCommand::Execute()
{
    _DownResult = Transfer_Unknown;
	_success = false;

    RETURN_IF( _FilefromPhone.empty() || _FileToPc.empty() );

    // 文件已存在的处理
    RETURN_IF( ! CheckFileExist() );

	LOG->WriteDebug(_T("文件下载 测试A"));
	DeviceData* pTmp = DEVICE_MANAGER->FindDevice(_pDevice->GetId());
	if (!pTmp || !pTmp->GetDevInfo())
		return ;

	LOG->WriteDebug(_T("文件下载 测试B"));

	CFileOperation::MakeLocalDir(CFileOperation::GetFileDir(_FileToPc));

	if ( _pDevice->GetDevInfo()->_deviceType == Ios)
	{
		if ( _pDevice->GetDevInfo()->_connectType == WIFI)
		{
            IosWiFiFileSystem ioswifiFile(_pDevice->GetDevInfo()->_ios_wifi_handle);
			
			_DownResult = ioswifiFile.Ios_DownFromPhone(_FilefromPhone, _FileToPc, *this);
		}
		else if (_pDevice->GetDevInfo()->_connectType == USB)
		{
            if (!_pDevice->GetShareAppIdentifier().empty())
            {
                AFCFileSysemBase* pFileStream =  AFCAppFileSystem::StartAPPHouseArrestService(_pDevice->GetShareAppIdentifier(), (DeviceInfo*)_pDevice->GetDevInfo());
                _DownResult = pFileStream->Ios_DownFromPhone(_FilefromPhone, _FileToPc, *this);
                SAFE_DELETE(pFileStream);
            }
            else
            {
                AFCFileSysemBase iosUsbFile(_pDevice->GetDevInfo());
                _DownResult = iosUsbFile.Ios_DownFromPhone(_FilefromPhone, _FileToPc, *this);
            }
		}
#ifdef WIN32
		//转换PNG图片为可用格式
		if (_DownResult == Transfer_Success && CStrOperation::endsWithW(CStrOperation::toLowerW(_FilefromPhone), L".png"))
			IosPngConver::ConverApplePngToNormal(_FileToPc.c_str(), _FileToPc.c_str());
#endif
	}
	else if (_pDevice->GetDevInfo()->_deviceType == Android)
	{
		if(_pDevice->GetDevInfo()->_connectType == WIFI)
			WifiExecute();
		else
		{
			if(!_pAdbDown)
				_pAdbDown = new AdbDownloadFileCommand(_pDevice->GetDevInfo()->_strSerialNumber,_pDevice->GetDevInfo()->_ios_deviceIdentifier, this);
			_pAdbDown->SetParam(_FileToPc, _FilefromPhone);
			_pAdbDown->Execute();
			_DownResult = _pAdbDown->GetDownloadResult();
		}
	}	
	_success = true;
}

void ND91Assistant::DownloadFileCommand::WifiExecute()
{
	string result = "";

    AndroidTcpIO* pAndroidIO = dynamic_cast<AndroidTcpIO*>(GetIO());
    if ( ! pAndroidIO )
    {
        THROW_ASSISTANT_EXCEPTION(INVALID_IO_OBJECT);
    }

	pAndroidIO->SendAndroidDatagram(GetAndroidDatagram(), false);
	//需接收多次
	//第一次接收16位头
	result = pAndroidIO->Receive(16);
	BinaryBufferReader bufReader(result.c_str(), result.length());
	bufReader.ReadBuffer(_pReturnHead, 4);
	bufReader.ReadShort(_nFlag);
	bufReader.ReadShort(_nReturnLongConnectSign);
	bufReader.ReadInt32(_nReturnBodySize);
	bufReader.ReadInt32(_nReturnChannelID);

	/*__int64*/int64_t totallenth = 0;
	bool first = true;
	ofstream  ofs;
	while(true)
	{
	// 接收8位
	result = pAndroidIO->Receive(4);
	BinaryBufferReader readnextHeaderflag(result.c_str(), result.length());
	char* pbuffer = new char[5];
	int  bodylength = 0;
	readnextHeaderflag.ReadBuffer(pbuffer, 4);

	if(!strncmp(pbuffer, "SEND", 4))
	{
		_DownResult = Transfer_Success;
		// 接收错误信息
		//result = pAndroidIO->Receive(0);
		break;
	}
	else if( !strncmp(pbuffer, "SYNC", 4))
	{
		result = pAndroidIO->Receive(4);
		BinaryBufferReader buflength(result.c_str(), result.length());
		buflength.ReadInt32(bodylength);
		int Datalength = bodylength-8;
		if(Datalength == 13 && first)
		{
			result = pAndroidIO->Receive(13);
			BinaryBufferReader readeraction(result.c_str(), result.length());
			int action;
			readeraction.ReadInt32(action);
			if(action == 2)
			{
				bool suc = false;
				readeraction.ReadBool(suc);
				if(suc)
				{
					readeraction.ReadInt64(totallenth);
				}
			}
		}
		if(first)
		{
			int index = (int)_FileToPc.find_last_of(L"\\");
			CFileOperation::MakeLocalDir(_FileToPc.substr(0, index));
			//ofs.open(_FileToPc.c_str(), ios::binary|ios::out|ios::app);
            ofs.open(CCodeOperation::UnicodeToGB2312(_FileToPc).c_str(), ios::binary|ios::out|ios::app);

			SetCurrentTaskSum((int)totallenth>0?(int)totallenth:Datalength);
			SetCurrentTaskName(_FileToPc);
			SendProgress();
			first = false;
			if (totallenth>0)
				continue;
		}
		int  receviedLength = 0;
		int  AvailableLen = 10240;
        while(receviedLength < Datalength)
		{
            ProgressObserver* pObserver = NULL;
            if (!_observers.empty())
                pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

            IF_CANCEL(pObserver)
		    {
			    _DownResult = Transfer_Cancel;
			    break;
		    }

			int len = Datalength - receviedLength;
			len = len > AvailableLen?AvailableLen:len;

			// 接收len 长度
			result = pAndroidIO->Receive(len);
			ofs.write(result.c_str(), result.length());
			receviedLength += result.length();

            AddTaskProgress(result.length());
			SendProgress();
		}
		if(Datalength == receviedLength || Datalength == 0)
		{
			_DownResult = Transfer_Success;
		}
	}
	SAFE_DELETE_ARRAY(pbuffer);
	}
	ofs.close();
}

bool ND91Assistant::DownloadFileCommand::CheckFileExist()
{
    if ( CFileOperation::IsFileExist(_FileToPc.c_str()) )
    {
        switch( AskUser(_FileToPc) )
        {
        case OVERWRITE:
        case OVERWRITE_ALL:
			{
				DeviceData* pTmp = DEVICE_MANAGER->FindDevice(_pDevice->GetId());
				if (!pTmp || !pTmp->GetDevInfo())
					return false;
				CFileOperation::DeleteFile(_FileToPc);
			}
            break;

        case RENAME:
        case RENAME_ALL:
            _FileToPc = CFileOperation::RenameFile(_FileToPc);
            break;

        case CANCEL:
            _DownResult = Transfer_Cancel; 
            return false;

        default:
            THROW_ASSISTANT_EXCEPTION(PROGRAM_ERROR);
            break;
        }
    }
    return true;
}
