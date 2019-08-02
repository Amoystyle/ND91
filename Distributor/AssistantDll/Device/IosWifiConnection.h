#pragma once

#include "IConnection.h"
#include "Device/IosWiFiMobileDevice.h"
using namespace std;

namespace ND91Assistant
{

	typedef enum eNDWiFiError
	{
		ND_ERROR_SUCCESS,
		ND_ERROR_CANNOT_CONNECT,
		ND_ERROR_PSW_ERROR,
		ND_ERROR_OTHER,
	};

    class IosWifiConnection : public IConnection
    {

    public:
        IosWifiConnection(wstring strIP, wstring strPSW, int nWiFiHandle = 0);
        virtual ~IosWifiConnection();

		// 激活准备
		virtual bool PreActivate(){return true;};

		// 激活
		virtual bool Activate(){return true;};

        // 连接
        virtual bool Connect();

        // 断开
        virtual void Disconnect();

        // 返回连接成功后获得的硬件信息
        virtual const DeviceInfo* GetDeviceInfo();

		//检查连接状态
		//构造函数中可全部置空
		//返回是否连接，和连接成功后取得的设备版本
		bool CheckConnState(int nHandle, wstring &strPhoneVer);

        // 设置回调函数
        void SetTickCallBack(TICKCALLBACK pCallBack, void* pVoid);
	private:
		bool PhoneDaemonIsStillRunning();
		wstring GetSerial();
		wstring GetDevName();
		wstring GetProductVersion();

		wstring GetTypeValue(wstring strFileOnPhone, wstring strType, bool bNeedChangeToXML = false);

	private:		
		wstring _strIP;
		wstring _strPSW;
		int _nWiFiHandle;
		bool _bConn;
		wstring _strVersion;
		wstring _strIdentifier;
		bool _bTurnCypher;
		wstring _strDaemonCypherBytes;
		wstring _strDevName;
		wstring _strProductVersion;
		wstring _strSerialNumber;
		wstring _strDaemonVer;

        DeviceInfo _info;
    };
}

