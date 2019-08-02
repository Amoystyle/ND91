#include "stdafx.h"
#include "AndroidMonitorThread.h"

#include "Module/System/CheckNeedConnectConfirmCommand.h"
#include "Module/System/CreateLongConnectionCommand.h"
#ifdef MODULE_MESSAGE
#include "Module/Message/MessageData.h"
#include "Module/Message/MessageReceiptData.h"
#endif
#include "Module/Theme/PandaThemeActionReturn.h"
#include "Core/DeviceManager.h"

AndroidMonitorThread::AndroidMonitorThread(const DeviceInfo* pDeviceInfo, const std::wstring id)
: MonitorThread(pDeviceInfo, id)
{
    InitializeCriticalSection(&_csLock);
    _pBaseSocket = NULL;
	_bDeamonSockConnect =false;
}

AndroidMonitorThread::~AndroidMonitorThread()
{
    StopThread();

		SetCSLock(_csLock);
    //�˳�����
    if (_pBaseSocket)
    {
        _pBaseSocket->close();
        delete _pBaseSocket;
        _pBaseSocket = NULL;
    }
}

#ifdef WIN32
	DWORD WINAPI AndroidMonitorThread::run(LPVOID lpParameter)
#else
	void* AndroidMonitorThreadRun(void* lpParameter)
#endif
{
    LOG->WriteDebug(L"AndroidMonitorThread Begin"); 
    AndroidMonitorThread* pThread = (AndroidMonitorThread*)lpParameter;

	// ��ʱ����Ϊ�豸�Ͽ�
	//int silenceLimit = 0;
	//if( ! OPTION->Get(GlobalOption::INT_DEAMON_SILENCE_LIMIT, silenceLimit) )

	//��ʱǿ������Ϊ2��
	int	silenceLimit = DEFAULT_SILENCE_LIMIT;

recivepackage:
    while (!pThread->_bThreadQuitSign)
    {
        if (pThread->getPackage())
        {
            LOG->WriteDebug(pThread->_id + L"���յ�������"); 
			pThread->_bDeamonSockConnect=true;
            pThread->_nSilenceTime = 0;
			pThread->OnConnectionOK();
        }
        else
        {
            pThread->_nSilenceTime += DEFAULT_POLL_INTERVAL;
			pThread->_bDeamonSockConnect=false;
            if (pThread->_nSilenceTime > silenceLimit)
            {
				LOG->WriteDebug(pThread->_id + L"���ػ�ʧȥ��Ӧ"); 
                pThread->_bThreadQuitSign = true;
                pThread->onDisConnectDevice();				
                 goto THREAD_END; 
				
			}
			pThread->OnConnectionRetry();
			Sleep(DEFAULT_POLL_INTERVAL);  // ��ѯʱ����
            
        }
    }
THREAD_END:
	
    pThread->_bThreadQuitSign = true;
    LOG->WriteDebug(L"AndroidMonitorThread End"); 
    return 0L;
}

int AndroidMonitorThread::StartThread()
{
	StopThread();
    SetCSLock(_csLock);
    LOG->WriteDebug(L"AndroidMonitorThread StartThread");
    //�˳�����
    if (_pBaseSocket)
    {
        _pBaseSocket->close();
        delete _pBaseSocket; 
        _pBaseSocket = NULL;
    }
    //��������
    _pBaseSocket = new CBaseSocket();
	_pBaseSocket->create(CCodeOperation::UnicodeToGB2312(_pDeviceInfo->_strHost).c_str(), _pDeviceInfo->_nPort,true,5000);
    int nLongConnectSign = _pBaseSocket->getLocalPort();

    if ( nLongConnectSign == 0 )
    {
        THROW_ASSISTANT_EXCEPTION(ADB_WAKE_DEMON_ERROE);
    }

    // ��������
    ActivateDeamon(nLongConnectSign);

    // ���յ�һ�����ݰ�
    if ( ! getPackage() )
    {
		//�������쳣��������̻߳��и������ػ���ѭ��
        THROW_ASSISTANT_EXCEPTION(ADB_WAKE_DEMON_ERROE);
    }

    _bThreadQuitSign = false;

    // ���������̣߳����ֽ���
    DWORD	nThreadId;
#ifdef WIN32
	_hThread = CreateThread(NULL, 0, AndroidMonitorThread::run, (LPVOID)this, 0, &nThreadId);
#else
	pthread_create(&_hThread, NULL, AndroidMonitorThreadRun, (LPVOID)this);
#endif
    // �������ӳɹ� ��������������


    return nLongConnectSign;
}

void AndroidMonitorThread::StopThread()
{
    LOG->WriteDebug(L"AndroidMonitorThread StopThread");

    SetCSLock(_csLock);

    if (_hThread && !_bThreadQuitSign)
    {
        _bThreadQuitSign = true;
        WaitForSingleObject(_hThread,30000); 
    }
            
    //�˳�����
    if (_pBaseSocket)
    {
        _pBaseSocket->close();
        delete _pBaseSocket;
        _pBaseSocket = NULL;
    }
    _hThread = NULL;
}

void AndroidMonitorThread::ActivateDeamon(int nLongConnectSign)
{
    if (_pBaseSocket && _pBaseSocket->getLastError() == 0)
    {
		 LOG->WriteDebug(L"AndroidMonitorThread ActivateDeamon");
        wstring pcName = L"";
        bool bNeed = CheckNeedConnectConfirmCommand::IsNeedConnectConfirm(_pDeviceInfo->_strHost, _pDeviceInfo->_nPort);
        if (bNeed)
        {
            OPTION->Get(GlobalOption::STR_PCNAME, pcName);
            if ( pcName.empty() )   // Ĭ��ֵ��ȡ��ǰ��¼�û���
            {
                DWORD len = 256;
                TCHAR buf[256];
                if ( GetUserName(buf, &len) )
                    pcName.assign(buf);
            }
        }

        // ���������ػ��������ݰ�
        string datagram = CreateLongConnectionCommand::GetAndroidDatagram(nLongConnectSign, pcName);

		LOG->WriteDebug(L"AndroidMonitorThread ActivateDeamon :" + pcName);
        // �������ݰ�
        if (_pBaseSocket)
            _pBaseSocket->sendData(datagram.c_str(), datagram.size());
    }
}

bool AndroidMonitorThread::getPackage()
{
    // ��ȡ���ݰ�
    char	pPackageHead[DEMON_PACKAGE_HEAD_SIZE];
    if ( ! _pBaseSocket->receiveDataLength(pPackageHead, DEMON_PACKAGE_HEAD_SIZE) )
    {
        return false;
    }

    // �������ݰ�
    string datagram(pPackageHead, DEMON_PACKAGE_HEAD_SIZE);
    CreateLongConnectionCommand cmd;
    cmd.ParseturnDatagram(datagram);

    // ����ʽ���󣬿������ݻ��ң�������������
    if ( ! cmd.VerifyPackageHead() )
    {        
        string elseData = _pBaseSocket->receiveData();
        LOG_DEBUG(L"UNKNOWN DATA FORMAT" + cmd.ToString());
        LOG_DEBUG(CCodeOperation::GB2312ToUnicode(elseData));
        return false;
    }

    if (cmd.GetDataLength() == 4) // ��������
        return true;

    // ������������������
    LOG->WriteDebug(cmd.ToString());
    int nDataSize = cmd.GetDataLength() - 4;
    string data = _pBaseSocket->receiveData(nDataSize);
    if (data.length() > 4)
    {
        if( 2 == cmd.GetCommandId())    // �����������������ͣ���Ӧ����һ��enum
        {
            BinaryBufferReader reader(data.c_str(), data.length());
            int nAction;    //����Ϣ����
            reader.ReadInt32(nAction);
#ifdef MODULE_MESSAGE
            if (nAction == 2) //����Ϣ
            {
				MessageData* pMessageData = MessageData::FromDeamonNotifyBuffer(&reader);
                NewMessage(pMessageData);
				SAFE_DELETE(pMessageData);
            }
            else if (nAction == 15) //����Ϣ���ͽ��
            {
				MessageReceiptData* pMessageReceiptData = MessageReceiptData::FromDeamonNotifyBuffer(&reader);
                MessageReceipt(pMessageReceiptData);
				SAFE_DELETE(pMessageReceiptData);
            }
#endif
        }
        else if(15 == cmd.GetCommandId() || 21 == cmd.GetCommandId())
        {
#ifdef MODULE_THEME
            BinaryBufferReader reader(data.c_str(), data.length());
            PandaThemeActionReturn* pPandaThemeActionReturn = PandaThemeActionReturn::FromDeamonNotifyBuffer(&reader);
            PandaThemeAction(pPandaThemeActionReturn);
            SAFE_DELETE(pPandaThemeActionReturn);
#endif
        }
        else
        {
            // δ֪�����壬Log����
            //LOG->WriteDebug(CCodeOperation::GB2312ToUnicode(CStrOperation::BufferToHex(data.c_str(), data.length())));
		//	LOG->WriteDebug(L"AndroidMonitorThread �յ�δ֪�����壡");
		//	return false;
        }
    }
    return true;
}


void ND91Assistant::AndroidMonitorThread::OnConnectionOK()
{
	DeviceData* pDeviceData = DEVICE_MANAGER->FindDevice(_id);
	if (pDeviceData)
	{
		pDeviceData->SetConnectRetry(false);
	}
	
}

void ND91Assistant::AndroidMonitorThread::OnConnectionRetry()
{
	DeviceData* pDeviceData = DEVICE_MANAGER->FindDevice(_id);
	if (pDeviceData)
	{
		pDeviceData->SetConnectRetry(true);

	}
}
