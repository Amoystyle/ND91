#pragma once

#include "MonitorThread.h"
#include "Device/IosDaemon.h"
#include "Device/AFCFileSysemBase.h"
#include "Device/IosWiFiFileSystem.h"
#include "Device/IosWifiConnection.h"

// 默认守护心跳检测间隔时长(ms)，超过则认为设备断开
// 该时长可在Option中设置，option中找不到则使用此默认值
#define IOS_DEFAULT_CHECK_DEAMON   20000

// 轮询时间间隔：100ms
#define IOS_DEFAULT_POLL_INTERVAL   100

#ifndef WIN32
	void* IosMonitorThreadRun (void* lpParameter);
#endif

namespace ND91Assistant
{
	// ios 监听线程：守护心跳包、WIFI连接状态
    class IosMonitorThread : public MonitorThread
    {

    public:
        IosMonitorThread(const DeviceInfo* pDeviceInfo, const std::wstring id);
        virtual ~IosMonitorThread();

        // 启动监听线程
        // 返回值: 0: 失败 1: 成功 
        virtual int StartThread();

        // 停止监听线程
        virtual void StopThread();

	//private:
#ifdef WIN32		
		//线程
		static DWORD WINAPI IosMonitorThreadRun(LPVOID lpParameter);
#endif		

        // 初始化
        // 返回：true成功 false 失败
        bool Init();

        // 释放成员变量指针
        void Release();

    //private:
        IosDaemon*          _daemon;        // 守护访问类
        IIosFileSystem*     _pFileStream;   // 文件读写类
        IosWifiConnection*  _pWiFiConn;     // wifi连接类

    };
}
