#pragma once

#include "AdbRemoteCommand.h"

#include "common/BaseSocket.h"
#include "Core/DeviceInfo.h"
#include "Common/SystemException.h"
#include "Common/AssistantException.h"
#include "Common/StrOperation.h"

#define ADB_COMMAND_HOST "127.0.0.1"
#define ADB_COMMAND_PORT 5037
#define ADB_OK_FLAG "OKAY"
#define ADB_CONNECT_TO_DEVICE "host:transport:"

namespace ND91Assistant
{
    // ADB�������
    // ͨ��Socketִ�з�Shell��Adb����
    // ÿ������ʵ��һ�����࣬��ʵ��Execute��ParseReturnData����
    class AdbRemoteCommand
    {
    public:
        // ���캯������Ҫ�ṩ�豸���к�
        AdbRemoteCommand(wstring id);
        virtual ~AdbRemoteCommand();

        // ִ������
		void Execute();

	protected:

        // ����ʵ��ʱ���������⴦��ֻ�����CommonExecute����
        virtual void InternalExecute() = 0;

        // ������������
        // ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
        virtual bool ParseReturnData() = 0;

        // ִ������Ĺ�ͬ����
        void CommonExecute(string command);

        // ���ӵ�ָ���豸
        bool ConnectToDevice(string sn);

        // ��������
        void SendCommand(string command);

        // ���շ��ص�OK��־
        // ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
        bool ReceiveOkFlag();

        // ���շ��ص���������
        // ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
        bool ReceiveData(bool bSizeInBody = true);

        // ����ADB�����ַ���
        string FormAdbRequest(string command);

    protected:

        CBaseSocket* _pBaseSocket;  // socket����ʵ��

		wstring _id;		//ʶ���
        string _sn;           // �豸��

		string _strCommand;			//���͵�����

        bool _isShellCommand;       // �Ƿ�Shell����

        string  _returnData;        // ִ������󷵻ص����ݣ�����OKAY��־��

        bool _bSizeInBody;    //bSizeInBody  ��ʶ���ص������������Ƿ����4���ֽڵĳ��ȱ�ʶ����"001F"
        bool _bReturnData;    //returnData   ��ʶ�Ƿ񷵻�������
        bool _bReturnOK;      //returnOK	 ��ʶ�Ƿ񷵻�"OKAY"
    };

}