///////////////////////////////////////////////////////////
//  DeviceObserver.h
//  Implementation of the Class DeviceObserver
//  Created on:      01-六月-2011 9:42:53
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_253D8621_B866_4e51_AEE4_E49E0C26B006__INCLUDED_)
#define EA_253D8621_B866_4e51_AEE4_E49E0C26B006__INCLUDED_

#include "IObserver.h"
#include <string>

using namespace std;

namespace ND91Assistant
{

    class CLASSINDLL_CLASS_DECL DeviceObserver : public IObserver
    {

    public:
        virtual ~DeviceObserver() {}

        virtual void WifiDeviceConnect(wstring sn) = 0;
        virtual void WifiDeviceDisconnect(wstring sn) = 0;

        virtual void UsbDeviceConnect(wstring sn) = 0;
        virtual void UsbDeviceDisconnect(wstring sn) = 0;

		virtual void DaemonDisable(wstring sn) = 0;

        virtual void DFUConnect(wstring sn) {};
        virtual void DFUDisConnect(wstring sn) {};

        virtual void RecoveryConnect(wstring sn) {};
        virtual void RecoveryDisConnect(wstring sn) {};

		 virtual void OnIosDevJailBreak(wstring sn) {};

        // 守护通过wifi心跳线程发送的消息
        //   int nNotify: 0 心跳包                         
        //                1 手机端手机助手点'断开wifi连接' 
        //                2 系统将进入休眠                 
        //                3 系统进入休眠                 
        //                4 手机锁屏                       
        //                5 通过电源按钮关机     
        virtual void WifiTickNotify(int nNotify, wstring sn) {};
        // 检测到设备处于OffLine状态
        virtual void UsbDeviceOffLine( wstring sn ) {};
		//有一台处于屏锁的设备接入
		virtual void IosDevicePasswordLockConnect(wstring devicename) {};
    };
}

#endif // !defined(EA_253D8621_B866_4e51_AEE4_E49E0C26B006__INCLUDED_)
