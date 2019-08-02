///////////////////////////////////////////////////////////
//  DeviceSubject.h
//  Implementation of the Class DeviceSubject
//  Created on:      01-六月-2011 9:42:53
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_50E97224_8A4F_40c6_9368_E2B95A40C297__INCLUDED_)
#define EA_50E97224_8A4F_40c6_9368_E2B95A40C297__INCLUDED_

#include "AssistantDll.h"

#include "ISubject.h"
#include <string>

using namespace std;

namespace ND91Assistant
{

    class CLASSINDLL_CLASS_DECL DeviceSubject : public ISubject
    {
    public:
        virtual ~DeviceSubject() {};

        void WifiDeviceConnect(wstring sn);
        void WifiDeviceDisconnect(wstring sn);

        void UsbDeviceConnect(wstring sn);
        void UsbDeviceDisconnect(wstring sn);

		void IosPasswordLockDeviceConnect(wstring devicename);

		void DaemonDisable(wstring sn);

        void DFUConnect(wstring sn);
        void DFUDisConnect(wstring sn);

        void RecoveryConnect(wstring sn);
        void RecoveryDisConnect(wstring sn);  

        void WifiTickNotify(int nNotify, wstring sn);

        // 检测到设备处于OffLine状态
        virtual void UsbDeviceOffLine( wstring sn );

		//ios设备越狱
		void OnIosDevJailBreak(wstring sn);
    };
}

#endif // !defined(EA_50E97224_8A4F_40c6_9368_E2B95A40C297__INCLUDED_)
