///////////////////////////////////////////////////////////
//  AndroidWifiMonitorThread.cpp
//  Implementation of the Class AndroidWifiMonitorThread
//  Created on:      01-����-2011 9:42:49
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
    //�ȷ������Ӳ��ȴ����յ���һ�����ݰ��󷵻أ��Ա�֤���������Ӳ���ʱ�Ϸ���
    int nLongConnectSign = getPackage();
    if (nLongConnectSign == 0)
    {
        return 0;
    }
    //���������̣߳����ֽ���
    _hThread = CreateThread(NULL, 0, AndroidMonitorThread::run,(LPVOID)this, 0, &_nThreadId);
    return nLongConnectSign;
}

void ND91Assistant::AndroidWifiMonitorThread::StopThread()
{

}