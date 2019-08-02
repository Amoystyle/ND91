///////////////////////////////////////////////////////////
//  AndroidWifibConnection.h
//  Implementation of the Class AndroidWifibConnection
//  Created on:      01-六月-2011 9:42:48
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_C1637CC2_B865_4502_A25B_6E2AB1293B56__INCLUDED_)
#define EA_C1637CC2_B865_4502_A25B_6E2AB1293B56__INCLUDED_
#include "Common/BaseSocket.h"
#include "Core/DeviceData.h"
#include "IConnection.h"
namespace ND91Assistant
{
    // Android WIFI连接处理
    class AndroidWifiConnection : public IConnection
    {

    public:
        AndroidWifiConnection(DeviceData* pDeviceData);
        virtual ~AndroidWifiConnection();

        // 连接准备
        virtual bool PreActivate();

        // 激活
        virtual bool Activate();

        // 连接
        virtual bool Connect();

        // 断开
        virtual void Disconnect();

        // 返回连接成功后获得的硬件信息
        virtual const DeviceInfo* GetDeviceInfo();

        //获取设备属性
        void GetAndroidProperty();

    private:
        DeviceData* _pDeviceData;
   };
}

#endif // !defined(EA_C1637CC2_B865_4502_A25B_6E2AB1293B56__INCLUDED_)
