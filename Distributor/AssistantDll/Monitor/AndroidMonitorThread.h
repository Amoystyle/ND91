///////////////////////////////////////////////////////////
//  AndroidMonitorThread.h
//  Implementation of the Class AndroidMonitorThread
//  Created on:      01-六月-2011 9:42:47
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_909DF4C3_2DAA_4b9e_99DA_2DEDEAAF6F29__INCLUDED_)
#define EA_909DF4C3_2DAA_4b9e_99DA_2DEDEAAF6F29__INCLUDED_

#include "MonitorThread.h"
#include "Common/BaseSocket.h"
#include "Monitor/DeamonMessageObserver.h"

#define DEMON_PACKAGE_HEAD_SIZE 16

// 默认守护心跳检测间隔时长(ms)，超过则认为设备断开
// 该时长可在Option中设置，option中找不到则使用此默认值
#define DEFAULT_SILENCE_LIMIT   2000 //TODO: 改为40000 先定为3000，方便测试

// 轮询时间间隔：30ms
#define DEFAULT_POLL_INTERVAL   30

#ifndef WIN32
	void* AndroidMonitorThreadRun (void* lpParameter);
#endif

namespace ND91Assistant
{
	// Android 监听线程：守护心跳包、新短信通知、短信回执
    class AndroidMonitorThread : public MonitorThread 
    {
    public:
        AndroidMonitorThread(const DeviceInfo* pDeviceInfo, const std::wstring id);
        virtual ~AndroidMonitorThread();

        // 启动线程
        // 返回值: Tcp 连接本地端口 
        virtual int StartThread();

        // 停止线程
        virtual void StopThread();

		bool  isDeamonVaid() const { return (_pBaseSocket && _pBaseSocket->GetSockLastError()==0 && _bDeamonSockConnect==true); }

	private:

        // 发送唤醒包给守护
        // 返回本地连接端口，返回0表示失败
        void ActivateDeamon(int nLongConnectSign);

		// 接收守护发过来的数据包,数据包内容放在_pDemonData
		// 返回true表示成功，false表示失败
		bool getPackage();

		void OnConnectionOK();

		void OnConnectionRetry();
#ifdef WIN32
		//线程
		static DWORD WINAPI run(LPVOID lpParameter);
#endif		

    //private:

        CRITICAL_SECTION _csLock;        // 操作临界值
        CBaseSocket *_pBaseSocket;          // socket实例

		bool _bDeamonSockConnect;
		
		
	};
}

#endif // !defined(EA_909DF4C3_2DAA_4b9e_99DA_2DEDEAAF6F29__INCLUDED_)
