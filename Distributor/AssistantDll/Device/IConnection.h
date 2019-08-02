#pragma once

#include "Core/DeviceInfo.h"

namespace ND91Assistant
{
    // 连接设备的接口
    class IConnection
    {
    public:
        // 基类必须有虚析构函数
        virtual ~IConnection() {}

        // 激活准备
        virtual bool PreActivate(){return false;};

        // 激活
        virtual bool Activate(){return false;};

        // 连接
        virtual bool Connect() = 0;

        // 断开
        virtual void Disconnect() = 0;

        // 返回连接成功后获得的硬件信息
        virtual const DeviceInfo* GetDeviceInfo() = 0;
        
        // 当前IOS设备是否禁止程序安装,也就是在设备的设置里面开启了限制程序安装.wifi连接的是否可以判断不?
        virtual bool IsProhibitAppInstall() { return false; };

		//初始化设备数据
		virtual void InitDeviceData(){};


        void SetConfirmConfig(int nConfig) {_nConfirmConfig = nConfig;};
    protected:
        int _nConfirmConfig;    // 连接确认配置
    };
}
