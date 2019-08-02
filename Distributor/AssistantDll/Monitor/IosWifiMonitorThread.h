///////////////////////////////////////////////////////////
//  IosWifiMonitorThread.h
//  Implementation of the Class IosWifiMonitorThread
//  Created on:      01-����-2011 9:42:59
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_2A8EE341_4036_47ad_AF21_D3557997C667__INCLUDED_)
#define EA_2A8EE341_4036_47ad_AF21_D3557997C667__INCLUDED_

#include "IosMonitorThread.h"

namespace ND91Assistant
{

    class IosWifiMonitorThread : public IosMonitorThread
    {

    public:
        IosWifiMonitorThread(const DeviceInfo* pDeviceInfo);
        virtual ~IosWifiMonitorThread();

        // ���������߳�
        // ����ֵ: Tcp ���ӱ��ض˿� 
        virtual int StartThread() { return 0; }

        // ֹͣ�����߳�
        virtual void StopThread() {}
    };
}

#endif // !defined(EA_2A8EE341_4036_47ad_AF21_D3557997C667__INCLUDED_)
