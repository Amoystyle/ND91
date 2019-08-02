#include "stdafx.h"

#include "AdbDownloadFileCommand.h"
#include "Common/BinaryBufferReader.h"
#include "Common/BinaryBufferWriter.h"

const char* const SOCKET_COMMAND_RECV  = "RECV";
const char* const SOCKET_COMMAND_DATA  = "DATA";
const char* const SOCKET_COMMAND_DONE  = "DONE";
const char* const FILE_STATE_COMMAND_STAT = "STAT";

AdbDownloadFileCommand::AdbDownloadFileCommand( wstring sn,wstring id, ProgressSubject* pSubject)
: AdbRemoteCommand(id)
{
    _sn = CCodeOperation::UnicodeToUTF_8(sn);
    _success = Transfer_Unknown;

    if ( pSubject )
        _pSubject = pSubject;
    else
        _pSubject = &_nullSubject;
}

AdbDownloadFileCommand::~AdbDownloadFileCommand( void )
{

}

void ND91Assistant::AdbDownloadFileCommand::InternalExecute()
{
    _success = Transfer_Unknown;
    string result = "";

    // 链接到设备
    if (_sn.length() > 0)
        if (!ConnectToDevice(_sn))
            return;

    ProgressObserver* pObserver = NULL;
    if (!_pSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_pSubject->_observers[0]);

    IF_CANCEL_RETURN(pObserver)

    // 创建异步接收
    bool b = _isShellCommand;
    _isShellCommand = false;
    SendCommand("sync:");
    _isShellCommand = b;
    if ( ! ReceiveOkFlag() )
    {
        _success = Transfer_Fail;
        return;
    }

    // 获取文件属性
    string cmdstr = CreateFileStateReqPkg(CCodeOperation::UnicodeToUTF_8(_FilefromPhone));
    _pBaseSocket->sendData(cmdstr.c_str(), cmdstr.length());
    result = _pBaseSocket->receiveData(4);

    BinaryBufferReader  statreader(result.c_str(), result.length());
    string okay;
    statreader.ReadString(okay,4);
    TFileState state;
    if(okay != FILE_STATE_COMMAND_STAT)
	{
        //THROW_ASSISTANT_EXCEPTION(FILE_NOT_FOUND);
		_success = Transfer_Fail;
		return;
	}
    else
    {
        result = _pBaseSocket->receiveData(12);
        state = ParseFileStateResult(result);

        _pSubject->SetCurrentTaskName(_FileToPc);
        _pSubject->SetCurrentTaskSum(state._mSize);
    }

    // 不可修改不可读无法下载
    if(state._mModified <= 0 || state._mReadMode <= 0)
    {
		//文件不存在
        //THROW_ASSISTANT_EXCEPTION(FILE_NOT_FOUND);
		_success = Transfer_Fail;
		return;
    }

    int index = (int)_FileToPc.find_last_of(L"\\");
    CFileOperation::MakeLocalDir(_FileToPc.substr(0, index).c_str());
    ofstream ofs;
#ifdef WIN32
    ofs.open(_FileToPc.c_str(), ios::binary|ios::out|ios::app);
#else
    ofs.open(CCodeOperation::UnicodeToGB2312(_FileToPc).c_str(), ios::binary|ios::out|ios::app);
#endif
    if(state._mSize == 0)
    {
        _success = Transfer_Success;
        ofs.close();
    }
    else
    {
        cmdstr = CreateReceiveRegPkg();
        _pBaseSocket->sendData(cmdstr.c_str(), cmdstr.length());
        char* pretflag = new char[5];
        while (true)
        {
            IF_CANCEL_BREAK(pObserver)
            /*IF_CANCEL(pObserver)
            {
            _success = Transfer_Cancel;
            break;
            }*/
            //TODO: receive  8 byte  header= flag+datalenth;
            result = _pBaseSocket->receiveData(4);
            BinaryBufferReader  reader(result.c_str(), result.length());
            reader.ReadBuffer(pretflag, 4);
            if(!strncmp(pretflag, SOCKET_COMMAND_DONE, 4))
            {
                _success = Transfer_Success;
                break;
            }
            result = _pBaseSocket->receiveData(4);
            int dataLength = 0;
            BinaryBufferReader  Lengthreader(result.c_str(), result.length());
            Lengthreader.ReadInt32(dataLength);
            if(strncmp(pretflag, SOCKET_COMMAND_DATA, 4))
            {
                _success = Transfer_Fail;
                result = _pBaseSocket->receiveData(dataLength);
                break;
            }
            if(dataLength == 0)
            {
                _success = Transfer_Success;
                break;
            }
            if(dataLength > 0x10000 || dataLength < 0)
            {
                _success = Transfer_Fail;
                break;
            }
            // TODO: receive  Data content
            result = _pBaseSocket->receiveData(dataLength);
            ofs.write(result.c_str(), result.length());
            _pSubject->AddTaskProgress(result.length());
            //_pSubject->AddAllTaskProgress(result.length());
            _pSubject->SendProgress();

        }
        ofs.close();
        delete []pretflag;
    }
    IF_CANCEL(pObserver)

    CFileOperation::DeleteFile(_FileToPc);
}

bool ND91Assistant::AdbDownloadFileCommand::ParseReturnData()
{
    return _success== Transfer_Success;
}

string ND91Assistant::AdbDownloadFileCommand::CreateFileStateReqPkg( const string& afilepath )
{
    BinaryBufferWriter  aBufferWriter;
    aBufferWriter.WriteBuffer(FILE_STATE_COMMAND_STAT, 4);
    aBufferWriter.WriteInt(afilepath.length());
    aBufferWriter.WriteBuffer(afilepath.c_str(), afilepath.length());
    return aBufferWriter.GetBuffer();
}

string ND91Assistant::AdbDownloadFileCommand::CreateReceiveRegPkg()
{
    string pathutf8 = CCodeOperation::UnicodeToUTF_8(_FilefromPhone);
    BinaryBufferWriter  writer;
    writer.WriteBuffer(SOCKET_COMMAND_RECV, 4);
    writer.WriteInt(pathutf8.length());
    writer.WriteBuffer(pathutf8.c_str(),pathutf8.length());
    return writer.GetBuffer();
}

ND91Assistant::TFileState ND91Assistant::AdbDownloadFileCommand::ParseFileStateResult( const string& result )
{
    BinaryBufferReader  reader(result.c_str(),result.length());
    TFileState fileState;
    fileState._mReadMode = -1;
    fileState._mSize = -1;
    fileState._mModified = -1;
    reader.ReadInt32(fileState._mReadMode);
    reader.ReadInt32(fileState._mSize);
    reader.ReadInt32(fileState._mModified);
    return  fileState;
}

void ND91Assistant::AdbDownloadFileCommand::SetParam( wstring strPCPath, wstring strPhonePath )
{
    _FileToPc = strPCPath;			// PC端文件
    _FilefromPhone = strPhonePath;	// 手机端文件
}