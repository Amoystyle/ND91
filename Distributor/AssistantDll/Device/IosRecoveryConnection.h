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

		//�����ֻ�������ģʽ
		void RebootToNormal();

        // ��ǰ�豸�Ƿ��ֹ����װ,Ҳ�������豸���������濪�������Ƴ���װ.wifi���ӵ��Ƿ�����жϲ�?
        virtual bool IsProhibitAppInstall() { return true; };

	private:
        std::wstring GetDeviceValueAsString(void* cfstring);
		std::wstring GetDevTypeString(int type);

	private:		
		DeviceInfo* m_devInfo;
	};
}
