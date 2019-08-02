#include "stdafx.h"

#include "AdbUploadFileCommand.h"
#include "Common/BinaryBufferReader.h"
#include "Common/BinaryBufferWriter.h"
#include "include/Core/DeviceData.h"
#include "include/core/DeviceManager.h"

const int FILE_COMMAND_LENGTH = 4;
const char* const FILE_COMMAND_SEND = "SEND";
const char* const FILE_COMMAND_DATA = "DATA";
const char* const FILE_COMMAND_DONE = "DONE";
const char* const FILE_COMMAND_MODE = ",01A4";
const char* const FILE_COMMAND_OKAY = "OKAY";
const char* const FILE_COMMAND_FAIL = "FAIL";

const int USB_BUFFER_SIZE = 0x10000;
const int ASYNC_BUFF_SIZE = 10240;


AdbUploadFileCommand::AdbUploadFileCommand(wstring sn,wstring id, wstring strPCPath, wstring strPhonePath, ProgressSubject& subject)
: AdbRemoteCommand(id)
{
	_sn = CCodeOperation::UnicodeToUTF_8(sn);
	_pSubject = &subject; 
	_fileSize = 0;
	_OffsetAppInExe =0;
    SetParam(strPCPath, strPhonePath);
}

AdbUploadFileCommand::~AdbUploadFileCommand()
{
}

void AdbUploadFileCommand::InternalExecute()
{
	_success = Transfer_Unknown;

    if (!ConnectToDevice(_sn))
        return;

    ProgressObserver* pObserver = NULL;
    if (!_pSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_pSubject->_observers[0]);

    IF_CANCEL_RETURN(pObserver)

    string result = "";
    ifstream  ifs;
#ifdef WIN32
    ifs.open(_fileOnPc.c_str(),ios::in|ios::binary);
#else
    ifs.open(CCodeOperation::UnicodeToGB2312(_fileOnPc).c_str(),ios::in|ios::binary);
#endif
    if ( !ifs.is_open() || !ifs.good())
    {
        THROW_ASSISTANT_EXCEPTION(OPEN_FILE_FAIL);
    }

    CreateSyncReq();

    _pSubject->SetCurrentTaskSum(_fileSize);
    _pSubject->SetCurrentTaskName(_fileonphone);

    result = CreateSendFileReqPkg();
    _pBaseSocket->sendData(result.c_str(),result.length()); // 发送【文件上传开始】数据包

    if(ifs.eof())
    {
        ifs.close();
        result = CreateSendDonePkg();
        _pBaseSocket->sendData(result.c_str(),result.length());
        _returnData = _pBaseSocket->receiveData(4);
    }
    else
    {
		//2012 增加从文件其中一段读取数据
        size_t buffsize = 0;
        char* pbuffer = new char[USB_BUFFER_SIZE + 1];
        int  curOffset = (size_t)_OffsetAppInExe;
		ifs.seekg((long)_OffsetAppInExe,ios::cur);
        while(curOffset <= _OffsetAppInExe+_fileSize)
		{
			DeviceData* pDevice = DEVICE_MANAGER->FindDevice(CCodeOperation::s2ws(__super::_sn));
			if(pDevice == NULL)
			{
				_success = Transfer_Cancel;
				return;
			}
            IF_CANCEL_BREAK(pObserver)
            if(curOffset == _OffsetAppInExe+_fileSize)
            {
                result = CreateSendDonePkg();
                _pBaseSocket->sendData(result.c_str(),result.length());
                _returnData = _pBaseSocket->receiveData(4);  // 期待返回 "OKAY"
                break;
            }
            buffsize = (size_t)(_OffsetAppInExe+_fileSize-curOffset);
            buffsize = buffsize >USB_BUFFER_SIZE?USB_BUFFER_SIZE:buffsize;

            if ( 0 != buffsize )
            {
                ifs.read(pbuffer, buffsize);
                result = CreateSendDataPkg(pbuffer, buffsize);
				bool bReturn = _pBaseSocket->sendData(result.c_str(),result.length());
				wstring wsFlag = L"FALSE";
				if (bReturn)
				{
					wsFlag = L"TRUE";
				}
				LOG->WriteDebug(L"UpLoad File, Socket Return:"+wsFlag);
                if (!bReturn)
					break;

                // TODO: 是否需要减少消息发送频率？
                curOffset += buffsize;  // 发送进度
                ifs.seekg(curOffset, ios::beg);
				_pSubject->AddTaskProgress(buffsize);
				_pSubject->SendProgress();
            }
        }
        ifs.close();
        delete[] pbuffer;
    }
    ParseReturnData();
}

bool AdbUploadFileCommand::ParseReturnData()
{
    ProgressObserver* pObserver = NULL;
    if (!_pSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_pSubject->_observers[0]);

    IF_CANCEL(pObserver)
    {
        _success = Transfer_Cancel;
        return false;
    }

	BinaryBufferReader  bufferReader(_returnData.c_str(), _returnData.length());

	char ret[5];
	bufferReader.ReadBuffer(ret, 4);

	if (!strncmp(ret, "OKAY", 4) )
    {
        _success = Transfer_Success;
    }
	else if (!strncmp(ret, "FAIL", 4) )
	{
		_returnData = _pBaseSocket->receiveData(4);
		BinaryBufferReader  reader(_returnData.c_str(), _returnData.length());
		int lenth;  // 错误信息长度
		reader.ReadInt32(lenth);
		_returnData = _pBaseSocket->receiveData(lenth);
		BinaryBufferReader  Desreader(_returnData.c_str(), _returnData.length());
		string  failDes;    // 错误信息内容
		Desreader.ReadString(failDes);
        _success = Transfer_Fail;
	}
	else
	{
        _success = Transfer_Unknown;
	}
    return _success==Transfer_Success;
}

void ND91Assistant::AdbUploadFileCommand::CreateSyncReq()
{
	//需重新创建否则下面收不到结果
	_isShellCommand = false;
	SendCommand("sync:");
	if ( ! ReceiveOkFlag() )
		THROW_ASSISTANT_EXCEPTION(ADB_COMMAND_RETURN_ERROR);
}

std::string ND91Assistant::AdbUploadFileCommand::CreateSendFileReqPkg()
{
	BinaryBufferWriter writer;

	// 上传文件：在文件名后面增加",01A4"字符串
	// 原因未知, 参见USBTransfer.cs 之 DoPushFile方法
	string path = CCodeOperation::UnicodeToUTF_8(_fileonphone) + FILE_COMMAND_MODE;

	writer.WriteBuffer(FILE_COMMAND_SEND, FILE_COMMAND_LENGTH);
	writer.WriteInt(path.length());
	writer.WriteBuffer(path.c_str(), path.length());
	return writer.GetBuffer();
}

std::string ND91Assistant::AdbUploadFileCommand::CreateSendDataPkg( const char* pbuffer, size_t buffsize )
{
	if ( buffsize <= 0 )
	{
		return "";
	}

	BinaryBufferWriter writer;

	writer.WriteBuffer(FILE_COMMAND_DATA, FILE_COMMAND_LENGTH);
	writer.WriteInt(buffsize);
	writer.WriteBuffer(pbuffer, buffsize);
	return writer.GetBuffer();
}

std::string ND91Assistant::AdbUploadFileCommand::CreateSendDonePkg()
{
	BinaryBufferWriter writer;

	writer.WriteBuffer(FILE_COMMAND_DONE, FILE_COMMAND_LENGTH);

	//时间戳
	writer.WriteInt((UINT32)time(NULL));

	return writer.GetBuffer();
}

void ND91Assistant::AdbUploadFileCommand::SetParam( wstring strPCPath, wstring strPhonePath ,unsigned __int64 OffsetAppInExe,unsigned __int64 len)
{
	_fileOnPc = strPCPath;
	_fileonphone = strPhonePath;
	if(len ==0)
		_fileSize = CFileOperation::GetFileSizeW(_fileOnPc);
	else
		_fileSize = (int)len;
	_OffsetAppInExe = OffsetAppInExe;
}
