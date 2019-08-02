#include "stdafx.h"
#include "IosMonitorThread.h"


IosMonitorThread::IosMonitorThread(const DeviceInfo* pDeviceInfo, const std::wstring id)
:MonitorThread(pDeviceInfo, id)
{
    _pWiFiConn = NULL;
    _daemon = NULL;
    _pFileStream = NULL;
}

IosMonitorThread::~IosMonitorThread()
{
    StopThread();
}

#ifdef WIN32
	DWORD WINAPI IosMonitorThread::IosMonitorThreadRun(LPVOID lpParameter)
#else
	void* IosMonitorThreadRun(void* lpParameter)
#endif
{
	IosMonitorThread* pThread = (IosMonitorThread*)lpParameter;

	// 监测时间间隔
	int nTimeout = 0;
	if( ! OPTION->Get(GlobalOption::INT_IOS_CHECK_DEAMON_RUNNING, nTimeout) )
		nTimeout = IOS_DEFAULT_CHECK_DEAMON;

	while (! pThread->_bThreadQuitSign)
	{
		Sleep(IOS_DEFAULT_POLL_INTERVAL);  // 轮询时间间隔
		pThread->_nSilenceTime += IOS_DEFAULT_POLL_INTERVAL;

		if (pThread->_nSilenceTime >= nTimeout)
		{
			//监测WIFI连接状态(仅对wifi)
			if (pThread->_pWiFiConn)
            {
                wstring strVerTemp = L"";
                if( ! pThread->_pWiFiConn->CheckConnState(pThread->_pDeviceInfo->_ios_wifi_handle, strVerTemp) )
			    {
				    pThread->onDisConnectDevice();
				    break;
			    }
            }

			//监测守护状态(对USB/wifi都检查守护状态)
			if (! pThread->_daemon->PhoneDaemonIsStillRunning())
			{
				pThread->onDaemonDisable();
				break;
			}

            // 监测成功
            
			pThread->_nSilenceTime = 0;
		}
	}

    pThread->_bThreadQuitSign = true;
	return 0L;
}

bool IosMonitorThread::Init()
{
    if (_pDeviceInfo->_connectType == USB)
    {
        _pFileStream = new AFCFileSysemBase(_pDeviceInfo);
        if ( _pFileStream )
            return false;
    }
    else if (_pDeviceInfo->_connectType == WIFI)
    {
        _pFileStream = new IosWiFiFileSystem(_pDeviceInfo->_ios_wifi_handle);
        if ( _pFileStream )
            return false;

        _pWiFiConn = new IosWifiConnection(L"", L"");
        if (! _pWiFiConn )
            return false;
    }

    _daemon = new IosDaemon(_pFileStream, _pDeviceInfo->_ios_deviceIdentifier);
    if ( ! _daemon )
        return false;

    return true;
}

int IosMonitorThread::StartThread()
{
    if ( ! _bThreadQuitSign )
    {
        return true;
    }

    if ( ! Init() )
    {
        Release();
        return false;
    }

	_bThreadQuitSign = false;

	// 启动监听线程，保持接收
	DWORD	nThreadId;
#ifdef WIN32
	_hThread = CreateThread(NULL, 0, IosMonitorThread::IosMonitorThreadRun, (LPVOID)this, 0, &nThreadId);
#else
	pthread_create(&_hThread, NULL, IosMonitorThreadRun, (LPVOID)this);
#endif

	if (_hThread)
		return true;

	return false;
}

void IosMonitorThread::Release()
{
    if (_pFileStream)
    {
        delete _pFileStream;
        _pFileStream = NULL;
    }

    if (_daemon)
    {
        delete _daemon;
        _daemon = NULL;
    }

    if (_pWiFiConn)
    {
        delete _pWiFiConn;
        _pWiFiConn = NULL;
    }
}

void IosMonitorThread::StopThread()
{
    if (_hThread &&  ! _bThreadQuitSign )
    {
	    _bThreadQuitSign = true;
	    WaitForSingleObject(_hThread,1000); 
    }
    Release();
}
