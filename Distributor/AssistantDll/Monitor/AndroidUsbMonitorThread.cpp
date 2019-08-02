///////////////////////////////////////////////////////////
//  AndroidUsbMonitorThread.cpp
//  Implementation of the Class AndroidUsbMonitorThread
//  Created on:      01-����-2011 9:42:48
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

    //�ȷ������Ӳ��ȴ����յ���һ�����ݰ��󷵻أ��Ա�֤���������Ӳ���ʱ�Ϸ���
    int nLongConnectSign = getPackage();
	if (nLongConnectSign == 0)
	{
		THROW_ASSISTANT_EXCEPTION(ADB_WAKE_DEMON_ERROE);
	}
    //���������̣߳����ֽ���
	_hThread = CreateThread(NULL, 0, AndroidMonitorThread::run,(LPVOID)this, 0, &_nThreadId);
    return nLongConnectSign;
}

void ND91Assistant::AndroidUsbMonitorThread::StopThread()
{

}

