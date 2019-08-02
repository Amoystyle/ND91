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

    //����Adb���񣬿�����Ҫ���ĵ��ø÷���ĵط�
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


// ִ������Ĺ�ͬ����
void AdbRemoteCommand::CommonExecute(string command)
{
	_strCommand = command;

    SendCommand(command);

    if ( ! _bReturnOK )   // û�з���ֵ
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
    // ���շ���ֵ
    if ( ! ReceiveData(_bSizeInBody) )
    {
#ifdef _DEBUG
        THROW_ASSISTANT_EXCEPTION(ADB_COMMAND_RETURN_ERROR);
#else
        return;
#endif
    }

    // ��������ֵ
    if (! ParseReturnData() )
    {
    }
}

// ��������
// ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
void AdbRemoteCommand::SendCommand(string command)
{
    string adbCommand = FormAdbRequest(command);

    if ( ! _pBaseSocket->sendData(adbCommand.c_str(), 0) )
	{

		DeviceData* pDeviceData = DEVICE_MANAGER->FindDevice(_id);
		RETURN_IF( ! pDeviceData);

		_pBaseSocket->close();
		delete _pBaseSocket;

		// ������������
		_pBaseSocket = new CBaseSocket();
		_pBaseSocket->create(ADB_COMMAND_HOST, ADB_COMMAND_PORT);
		if ( ! _pBaseSocket->sendData(adbCommand.c_str(), 0) )
		{
			LOG->WriteDebug(L" SendData fail ");
			THROW_ASSISTANT_EXCEPTION(ADB_SOCKET_ERROR);
		}
		
	}
}

// ͨ��Socket���ӵ��豸
// ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
bool AdbRemoteCommand::ConnectToDevice(string sn)
{
    // �����и���ʱ�ԵĴ���ִ�������豸����ʱ��һ�����ü�shell��
    // �����_isShellCommand�����ݴ�������ִ�������������û�ȥ
	bool b = _isShellCommand;
	_isShellCommand = false;
	SendCommand(ADB_CONNECT_TO_DEVICE + sn);
	_isShellCommand = b;

    if ( ! ReceiveOkFlag() )
        return false; //THROW_ASSISTANT_EXCEPTION(ADB_COMMAND_RETURN_ERROR);
	return true;
}

// ���շ��ص�OK��־
// ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
bool AdbRemoteCommand::ReceiveOkFlag()
{
    // �������ճɹ���־��buffer
    int bufferSize = sizeof(ADB_OK_FLAG) - 1; // sizeof�����Ͻ�β��'\0'
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

// ���շ��ص���������
// ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
bool AdbRemoteCommand::ReceiveData(bool bSizeInBody)
{
    // ���շ��أ��ַ������ȡ��ַ�������
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
		if ((int)_strCommand.find("pm install") >= 0)// ��װӦ��
			_returnData = _pBaseSocket->receiveData(0, 2);
        else if ((int)_strCommand.find("dd if=") >= 0)// �����ļ�(���Դ������ļ��Ĵ�С������������5000�Σ����ȴ�3*5000�룬��ɻ���������)
            _returnData = _pBaseSocket->receiveData(0, 5000);
		else
			_returnData = _pBaseSocket->receiveData();
	}

    //����ת��
    //_returnData = CCodeOperation::UTF_8ToGB2312(_returnData);

    return true;
}


// ����ADB�����ַ���
string AdbRemoteCommand::FormAdbRequest(string command)
{
    if ( _isShellCommand )
    {
        command = "shell:" + command;
    }

    // �����ת��Ϊ16�����ı���ʽ(��"002B")
    // �����������Ȳ�����65535���������
    string leng = CStrOperation::Int2Hex(command.length());

    // ���ȷ���ǰͷ
    return  leng + command;
}

void ND91Assistant::AdbRemoteCommand::Execute()
{
    if ( ! _pBaseSocket )
    {
        // ���ӵ�������ADB.EXE���������ӵ��ػ����ʲ�ʹ�����ӳ�
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
