#pragma once

#include "IConnection.h"
#include "Core/DeviceInfo.h"
#include <string>
using namespace std;

namespace ND91Assistant
{
    class IosRecoveryConnection : public IConnection
	{
	public:
		IosRecoveryConnection(DeviceInfo* devInfo);
		~IosRecoveryConnection(void);

		virtual bool Connect();

		virtual void Disconnect();

        virtual DeviceInfo* GetDeviceInfo();

		//重启手机至正常模式
		void RebootToNormal();

        // 当前设备是否禁止程序安装,也就是在设备的设置里面开启了限制程序安装.wifi连接的是否可以判断不?
        virtual bool IsProhibitAppInstall() { return true; };

	private:
        std::wstring GetDeviceValueAsString(void* cfstring);
		std::wstring GetDevTypeString(int type);

	private:		
		DeviceInfo* m_devInfo;
	};
}
