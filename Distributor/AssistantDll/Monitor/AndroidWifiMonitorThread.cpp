///////////////////////////////////////////////////////////
//  AndroidWifiMonitorThread.cpp
//  Implementation of the Class AndroidWifiMonitorThread
//  Created on:      01-六月-2011 9:42:49
//  Original author: Administrator
///////////////////////////////////////////////////////////

#include "AndroidWifiMonitorThread.h"

using namespace ND91Assistant;

AndroidWifiMonitorThread::AndroidWifiMonitorThread(const DeviceInfo* pDeviceInfo)
:AndroidMonitorThread(pDeviceInfo)
{

}



AndroidWifiMonitorThread::~AndroidWifiMonitorThread()
{

}

int ND91Assistant::AndroidWifiMonitorThread::StartThread()
{
    //先发起连接并等待接收到第一个数据包后返回，以保证后续的连接操作时合法的
    int nLongConnectSign = getPackage();
    if (nLongConnectSign == 0)
    {
        return 0;
    }
    //启动心跳线程，保持接收
    _hThread = CreateThread(NULL, 0, AndroidMonitorThread::run,(LPVOID)this, 0, &_nThreadId);
    return nLongConnectSign;
}

void ND91Assistant::AndroidWifiMonitorThread::StopThread()
{

}