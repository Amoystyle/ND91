///////////////////////////////////////////////////////////
//  AndroidWifiMonitorThread.h
//  Implementation of the Class AndroidWifiMonitorThread
//  Created on:      01-六月-2011 9:42:49
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_740F69EE_8EFE_4266_B3B0_2812D06053CE__INCLUDED_)
#define EA_740F69EE_8EFE_4266_B3B0_2812D06053CE__INCLUDED_

#include "AndroidMonitorThread.h"

namespace ND91Assistant
{

    class AndroidWifiMonitorThread : public AndroidMonitorThread
    {

    public:
        AndroidWifiMonitorThread(const DeviceInfo* pDeviceInfo);
        virtual ~AndroidWifiMonitorThread();

    public:
        //启动线程
        virtual int StartThread();

        //停止线程
        virtual void StopThread();
    };
}

#endif // !defined(EA_740F69EE_8EFE_4266_B3B0_2812D06053CE__INCLUDED_)
