#include "stdafx.h"

#include "AdbRemoteCommand.h"
#include "AndroidAdbHelper.h"
#include "Core/DeviceData.h"
#include "Core/DeviceManager.h"

AdbRemoteCommand::AdbRemoteCommand(wstring id)
: _pBaseSocket(NULL)
, _isShellCommand(false)
{
    _bSizeInBody = true;
    _bReturnData = true;
    _bReturnOK = true;

    //启动Adb服务，可能需要更改调用该服务的地方
    AndroidAdbHelper aah;
    aah.startServer();

    _isShellCommand = false;

	_pBaseSocket = NULL;

	_id = id;
}

AdbRemoteCommand::~AdbRemoteCommand()
{
    if ( _pBaseSocket )
    {
        _pBaseSocket->close();
        delete _pBaseSocket;
    }
}


// 执行命令的共同流程
void AdbRemoteCommand::CommonExecute(string command)
{
	_strCommand = command;

    SendCommand(command);

    if ( ! _bReturnOK )   // 没有返回值
        return;

    if ( ! ReceiveOkFlag() )
	{
		if(command == ADB_CMD_LIST_DEVICE)
			THROW_ASSISTANT_EXCEPTION(ADB_COMMAND_RETURN_ERROR);
		else if(command == "host:nddevices")
			THROW_ASSISTANT_EXCEPTION(ADB_COMMAND_NO_NDDEVICE);
		else
			return;
	}


    if ( ! _bReturnData )
	{
		LOG->WriteDebug(L" returnData is null");
        return;
	}
    // 接收返回值
    if ( ! ReceiveData(_bSizeInBody) )
    {
#ifdef _DEBUG
        THROW_ASSISTANT_EXCEPTION(ADB_COMMAND_RETURN_ERROR);
#else
        return;
#endif
    }

    // 解析返回值
    if (! ParseReturnData() )
    {
    }
}

// 发送命令
// 返回值：true 表示成功，false 表示失败
void AdbRemoteCommand::SendCommand(string command)
{
    string adbCommand = FormAdbRequest(command);

    if ( ! _pBaseSocket->sendData(adbCommand.c_str(), 0) )
	{

		DeviceData* pDeviceData = DEVICE_MANAGER->FindDevice(_id);
		RETURN_IF( ! pDeviceData);

		_pBaseSocket->close();
		delete _pBaseSocket;

		// 重新连接试试
		_pBaseSocket = new CBaseSocket();
		_pBaseSocket->create(ADB_COMMAND_HOST, ADB_COMMAND_PORT);
		if ( ! _pBaseSocket->sendData(adbCommand.c_str(), 0) )
		{
			LOG->WriteDebug(L" SendData fail ");
			THROW_ASSISTANT_EXCEPTION(ADB_SOCKET_ERROR);
		}
		
	}
}

// 通过Socket连接到设备
// 返回值：true 表示成功，false 表示失败
bool AdbRemoteCommand::ConnectToDevice(string sn)
{
    // 这里有个临时性的处理：执行连接设备命令时是一定不用加shell的
    // 这里把_isShellCommand属性暂存起来，执行完命令再设置回去
	bool b = _isShellCommand;
	_isShellCommand = false;
	SendCommand(ADB_CONNECT_TO_DEVICE + sn);
	_isShellCommand = b;

    if ( ! ReceiveOkFlag() )
        return false; //THROW_ASSISTANT_EXCEPTION(ADB_COMMAND_RETURN_ERROR);
	return true;
}

// 接收返回的OK标志
// 返回值：true 表示成功，false 表示失败
bool AdbRemoteCommand::ReceiveOkFlag()
{
    // 用来接收成功标志的buffer
    int bufferSize = sizeof(ADB_OK_FLAG) - 1; // sizeof会算上结尾的'\0'
    char* pBuffer = new char[bufferSize + 1];
    memset(pBuffer, 0, bufferSize + 1);

    if ( ! _pBaseSocket->receiveDataLength(pBuffer, bufferSize) )
    {
		LOG->WriteDebug(L" ReceiveBuffer is null");
        delete[] pBuffer;
        return false;
    }

    bool isOK = ! strncmp(pBuffer, ADB_OK_FLAG, bufferSize) ;

    delete[] pBuffer;
    return isOK;
}

// 接收返回的数据内容
// 返回值：true 表示成功，false 表示失败
bool AdbRemoteCommand::ReceiveData(bool bSizeInBody)
{
    // 接收返回：字符串长度、字符串内容
	if (bSizeInBody)
	{
		char pSize[5] = {0};
		if (! _pBaseSocket->receiveDataLength(pSize, 4))
			return false;

        char* p;
		unsigned long nSize = strtoul(pSize, &p, 16);
		if (nSize > 102400)
			return false;
        if (nSize == 0)
            return true;

		_returnData = _pBaseSocket->receiveData(nSize);
	}
	else
	{
		if ((int)_strCommand.find("pm install") >= 0)// 安装应用
			_returnData = _pBaseSocket->receiveData(0, 2);
        else if ((int)_strCommand.find("dd if=") >= 0)// 复制文件(重试次数由文件的大小决定，暂重试5000次，即等待3*5000秒，完成会立即返回)
            _returnData = _pBaseSocket->receiveData(0, 5000);
		else
			_returnData = _pBaseSocket->receiveData();
	}

    //编码转换
    //_returnData = CCodeOperation::UTF_8ToGB2312(_returnData);

    return true;
}


// 生成ADB命令字符串
string AdbRemoteCommand::FormAdbRequest(string command)
{
    if ( _isShellCommand )
    {
        command = "shell:" + command;
    }

    // 命令长度转换为16进制文本格式(如"002B")
    // 这里假设命令长度不超过65535，否则出错
    string leng = CStrOperation::Int2Hex(command.length());

    // 长度放在前头
    return  leng + command;
}

void ND91Assistant::AdbRemoteCommand::Execute()
{
    if ( ! _pBaseSocket )
    {
        // 连接到本机的ADB.EXE，并非连接到守护，故不使用连接池
        _pBaseSocket = new CBaseSocket();
        _pBaseSocket->create(ADB_COMMAND_HOST, ADB_COMMAND_PORT);
    }
	try{
		InternalExecute();
		if (_pBaseSocket)
			_pBaseSocket->close();
	}
	catch(...)
	{
		THROW_ASSISTANT_EXCEPTION(ADB_COMMAND_RETURN_ERROR);
	}
    SAFE_DELETE( _pBaseSocket );
}
