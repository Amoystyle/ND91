#pragma once

#include "DeamonMessageSubject.h"
#include "Monitor/DeviceSubject.h"
#include "Core/DeviceInfo.h"
#include "DeamonMessageSubject.h"

#ifdef WIN32
	#include <windows.h>
#else
	#include "Common/winPort.h"
#endif

namespace ND91Assistant
{
    // 检测线程：执行心跳监测，接受守护发送信息（新短信通知、安卓的短信回执）
    class MonitorThread : public DeamonMessageSubject
    {
    public:
        MonitorThread(const DeviceInfo* pDeviceInfo, const std::wstring id);
        virtual ~MonitorThread();

        // 启动监听线程
        // 返回值: Tcp 连接本地端口 
        virtual int StartThread() = 0;

        // 停止监听线程
        virtual void StopThread() = 0;

    //protected:

        //设备连接断开
        void onDisConnectDevice();

        //守护停止运行
        void onDaemonDisable();

    //protected:
        const DeviceInfo*   _pDeviceInfo;   // 设备信息

        const std::wstring   _id;            // 设备ID

        HANDLE	_hThread;					// 开启的线程句柄

        bool	_bThreadQuitSign;			// 标志线程是否退出	

        int		_nSilenceTime;				// 间隔时间
    };
}
