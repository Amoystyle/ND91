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
    //退出连接
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

	// 超时，认为设备断开
	//int silenceLimit = 0;
	//if( ! OPTION->Get(GlobalOption::INT_DEAMON_SILENCE_LIMIT, silenceLimit) )

	//超时强制设置为2秒
	int	silenceLimit = DEFAULT_SILENCE_LIMIT;

recivepackage:
    while (!pThread->_bThreadQuitSign)
    {
        if (pThread->getPackage())
        {
            LOG->WriteDebug(pThread->_id + L"：收到心跳包"); 
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
				LOG->WriteDebug(pThread->_id + L"：守护失去响应"); 
                pThread->_bThreadQuitSign = true;
                pThread->onDisConnectDevice();				
                 goto THREAD_END; 
				
			}
			pThread->OnConnectionRetry();
			Sleep(DEFAULT_POLL_INTERVAL);  // 轮询时间间隔
            
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
    //退出连接
    if (_pBaseSocket)
    {
        _pBaseSocket->close();
        delete _pBaseSocket; 
        _pBaseSocket = NULL;
    }
    //创建连接
    _pBaseSocket = new CBaseSocket();
	_pBaseSocket->create(CCodeOperation::UnicodeToGB2312(_pDeviceInfo->_strHost).c_str(), _pDeviceInfo->_nPort,true,5000);
    int nLongConnectSign = _pBaseSocket->getLocalPort();

    if ( nLongConnectSign == 0 )
    {
        THROW_ASSISTANT_EXCEPTION(ADB_WAKE_DEMON_ERROE);
    }

    // 发起连接
    ActivateDeamon(nLongConnectSign);

    // 接收第一个数据包
    if ( ! getPackage() )
    {
		//不用抛异常，后面的线程会有个启动守护的循环
        THROW_ASSISTANT_EXCEPTION(ADB_WAKE_DEMON_ERROE);
    }

    _bThreadQuitSign = false;

    // 启动监听线程，保持接收
    DWORD	nThreadId;
#ifdef WIN32
	_hThread = CreateThread(NULL, 0, AndroidMonitorThread::run, (LPVOID)this, 0, &nThreadId);
#else
	pthread_create(&_hThread, NULL, AndroidMonitorThreadRun, (LPVOID)this);
#endif
    // 发起连接成功 代表真正连接上


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
            
    //退出连接
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
            if ( pcName.empty() )   // 默认值：取当前登录用户名
            {
                DWORD len = 256;
                TCHAR buf[256];
                if ( GetUserName(buf, &len) )
                    pcName.assign(buf);
            }
        }

        // 创建唤醒守护命令数据包
        string datagram = CreateLongConnectionCommand::GetAndroidDatagram(nLongConnectSign, pcName);

		LOG->WriteDebug(L"AndroidMonitorThread ActivateDeamon :" + pcName);
        // 发送数据包
        if (_pBaseSocket)
            _pBaseSocket->sendData(datagram.c_str(), datagram.size());
    }
}

bool AndroidMonitorThread::getPackage()
{
    // 读取数据包
    char	pPackageHead[DEMON_PACKAGE_HEAD_SIZE];
    if ( ! _pBaseSocket->receiveDataLength(pPackageHead, DEMON_PACKAGE_HEAD_SIZE) )
    {
        return false;
    }

    // 解析数据包
    string datagram(pPackageHead, DEMON_PACKAGE_HEAD_SIZE);
    CreateLongConnectionCommand cmd;
    cmd.ParseturnDatagram(datagram);

    // 若格式错误，可能数据混乱，丢弃所有数据
    if ( ! cmd.VerifyPackageHead() )
    {        
        string elseData = _pBaseSocket->receiveData();
        LOG_DEBUG(L"UNKNOWN DATA FORMAT" + cmd.ToString());
        LOG_DEBUG(CCodeOperation::GB2312ToUnicode(elseData));
        return false;
    }

    if (cmd.GetDataLength() == 4) // 是心跳包
        return true;

    // 不是心跳包，带数据
    LOG->WriteDebug(cmd.ToString());
    int nDataSize = cmd.GetDataLength() - 4;
    string data = _pBaseSocket->receiveData(nDataSize);
    if (data.length() > 4)
    {
        if( 2 == cmd.GetCommandId())    // 若增加其他命令类型，就应该做一个enum
        {
            BinaryBufferReader reader(data.c_str(), data.length());
            int nAction;    //短消息类型
            reader.ReadInt32(nAction);
#ifdef MODULE_MESSAGE
            if (nAction == 2) //新消息
            {
				MessageData* pMessageData = MessageData::FromDeamonNotifyBuffer(&reader);
                NewMessage(pMessageData);
				SAFE_DELETE(pMessageData);
            }
            else if (nAction == 15) //短消息发送结果
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
            // 未知数据体，Log出来
            //LOG->WriteDebug(CCodeOperation::GB2312ToUnicode(CStrOperation::BufferToHex(data.c_str(), data.length())));
		//	LOG->WriteDebug(L"AndroidMonitorThread 收到未知数据体！");
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
