///////////////////////////////////////////////////////////
//  AndroidUsbMonitorThread.cpp
//  Implementation of the Class AndroidUsbMonitorThread
//  Created on:      01-六月-2011 9:42:48
//  Original author: Administrator
///////////////////////////////////////////////////////////

#include "AndroidUsbMonitorThread.h"
#include "Device/AndroidAdbHelper.h"
#include "Device/AdbStartDeamonCommand.h"
#include "Device/AdbForwardTcpCommand.h"
#include "Common/CodeOperation.h"
#include "Core/GlobalOption.h"

#include "Common/AssistantException.h"
#include "Common/Log.h"
#include <sstream>

using namespace ND91Assistant;

AndroidUsbMonitorThread::AndroidUsbMonitorThread(const DeviceInfo* pDeviceInfo)
:AndroidMonitorThread(pDeviceInfo)
{

}



AndroidUsbMonitorThread::~AndroidUsbMonitorThread()
{

}

int ND91Assistant::AndroidUsbMonitorThread::StartThread()
{

    //先发起连接并等待接收到第一个数据包后返回，以保证后续的连接操作时合法的
    int nLongConnectSign = getPackage();
	if (nLongConnectSign == 0)
	{
		THROW_ASSISTANT_EXCEPTION(ADB_WAKE_DEMON_ERROE);
	}
    //启动心跳线程，保持接收
	_hThread = CreateThread(NULL, 0, AndroidMonitorThread::run,(LPVOID)this, 0, &_nThreadId);
    return nLongConnectSign;
}

void ND91Assistant::AndroidUsbMonitorThread::StopThread()
{

}

