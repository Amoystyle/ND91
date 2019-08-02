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

		// ����׼��
		virtual bool PreActivate(){return true;};

		// ����
		virtual bool Activate(){return true;};

        // ����
        virtual bool Connect();

        // �Ͽ�
        virtual void Disconnect();

        // �������ӳɹ����õ�Ӳ����Ϣ
        virtual const DeviceInfo* GetDeviceInfo();

		//�������״̬
		//���캯���п�ȫ���ÿ�
		//�����Ƿ����ӣ������ӳɹ���ȡ�õ��豸�汾
		bool CheckConnState(int nHandle, wstring &strPhoneVer);

        // ���ûص�����
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

