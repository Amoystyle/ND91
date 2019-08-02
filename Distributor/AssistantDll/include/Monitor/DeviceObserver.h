///////////////////////////////////////////////////////////
//  DeviceObserver.h
//  Implementation of the Class DeviceObserver
//  Created on:      01-����-2011 9:42:53
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

        // �ػ�ͨ��wifi�����̷߳��͵���Ϣ
        //   int nNotify: 0 ������                         
        //                1 �ֻ����ֻ����ֵ�'�Ͽ�wifi����' 
        //                2 ϵͳ����������                 
        //                3 ϵͳ��������                 
        //                4 �ֻ�����                       
        //                5 ͨ����Դ��ť�ػ�     
        virtual void WifiTickNotify(int nNotify, wstring sn) {};
        // ��⵽�豸����OffLine״̬
        virtual void UsbDeviceOffLine( wstring sn ) {};
		//��һ̨�����������豸����
		virtual void IosDevicePasswordLockConnect(wstring devicename) {};
    };
}

#endif // !defined(EA_253D8621_B866_4e51_AEE4_E49E0C26B006__INCLUDED_)
