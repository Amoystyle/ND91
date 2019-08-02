///////////////////////////////////////////////////////////
//  IosUsbMonitorThread.h
//  Implementation of the Class IosUsbMonitorThread
//  Created on:      01-����-2011 9:42:59
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_B6D2F110_0B08_4961_AC26_A6FC17419923__INCLUDED_)
#define EA_B6D2F110_0B08_4961_AC26_A6FC17419923__INCLUDED_

#include "IosMonitorThread.h"

namespace ND91Assistant
{

    class IosUsbMonitorThread : public IosMonitorThread
    {

    public:
        IosUsbMonitorThread(const DeviceInfo* pDeviceInfo);
        virtual ~IosUsbMonitorThread();

        // ���������߳�
        // ����ֵ: Tcp ���ӱ��ض˿� 
        virtual int StartThread() { return 0; }

        // ֹͣ�����߳�
        virtual void StopThread() {}

    };
}

#endif // !defined(EA_B6D2F110_0B08_4961_AC26_A6FC17419923__INCLUDED_)
