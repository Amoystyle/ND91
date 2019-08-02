///////////////////////////////////////////////////////////
//  AndroidUsbMonitorThread.h
//  Implementation of the Class AndroidUsbMonitorThread
//  Created on:      01-六月-2011 9:42:48
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_60EAB56D_B142_48cb_8A26_73DEC8C435D7__INCLUDED_)
#define EA_60EAB56D_B142_48cb_8A26_73DEC8C435D7__INCLUDED_

#include "AndroidMonitorThread.h"

namespace ND91Assistant
{

    class AndroidUsbMonitorThread : public AndroidMonitorThread
    {

    public:
        AndroidUsbMonitorThread(const DeviceInfo* pDeviceInfo);
        virtual ~AndroidUsbMonitorThread();

	public:
		//启动线程
		virtual int StartThread();

		//停止线程
		virtual void StopThread();
    };
}

#endif // !defined(EA_60EAB56D_B142_48cb_8A26_73DEC8C435D7__INCLUDED_)
