#pragma once
#include <windows.h>
#include "AssistantDll.h"
#include <string>
#include "stdint.h"
using namespace std;
#pragma warning(disable : 4251)

namespace ND91Assistant
{
    enum DEVICE_OS_TYPE // ��������Ϊ��wininet�е������ظ�
    {
        Android,
        Ios
    };

	// ƽ̨���� - ���ƻ��������ƽ̨��Ҳ���Էŵ�������
	enum DEVICE_PLATFORM
	{
		Platform_Android,
		Platform_OMS,
		Platform_IOS,
	};

    enum CONNECT_TYPE
    {
        USB,
        WIFI,
		RECOVERY,
        DFU,
    };

    // ����Ӳ����Ϣ���Լ�������Ϣ
    class CLASSINDLL_CLASS_DECL DeviceInfo
    {
    public:
        DeviceInfo();
		~DeviceInfo();
#pragma region ������Ϣ

        DEVICE_OS_TYPE     _deviceType;    // �豸����
        CONNECT_TYPE    _connectType;   // ��������
		DEVICE_PLATFORM _platform;		// ƽ̨����	

        std::wstring 	_strSerialNumber;		// ��׿�豸Ӳ����Ų����±��
		std::wstring 	_strSerialNumberForShow;	// ����phonecenter�Ǳ����±�������ţ�����չʾ��ʱ����Ҫһ�����������к�
		std::wstring 	_strHost;				// ��׿��ǰ���ӵ�Host
		short	     	_nPort;					// ��׿��ǰ���ӵĶ˿�ֵ

        std::wstring 	_deviceIMEI;            // IMEI �ƶ��豸��ʶ
		std::wstring	_deviceIMSI;			// IMSI	�ƶ���Ӫ���û���ʶ
        std::wstring 	_manufacturer;          // ����
		std::wstring 	_deviceHWModel;         // Ӳ���ͺ�, ��N90AP
        std::wstring    _deviceName;			// �豸��
		std::wstring 	_deviceProductType;		// ϵͳ ����iPad1,1
        std::wstring 	_deviceProductVersion;  // �汾 ������ǹ̼��汾 IOS�汾�����ֱ�ʾ
        std::wstring 	_language;              // ��ǰ�ֻ�ʹ�����ԣ���������ָ�������������
        std::wstring    _deviceWiFiMac;			// WiFi��MAC��ַ
 
		std::wstring 	_daemonVer;				// �ػ��汾
		std::wstring    _deviceBaseVersion;		// �����汾
		std::wstring    _deviceCoreVersion;		// �ں˰汾
		std::wstring    _deviceInsdeVersion;	// �ڲ��汾
		std::wstring    _deviceCPUArch;			// CPU�ͺ�

		std::wstring    _deviceResolution;		// �ֱ���
		std::wstring    _deviceBattery;			// �豸����

#pragma endregion

#pragma region Android�豸��Ϣ

		bool IsLenovo() const {return (_deviceIMEI.find(L"863664")==0 || _deviceIMEI.find(L"356727")==0);}

		std::wstring	_sdcardpath;

		short			_nLongConnectSign;		// ��׿���ڵ�ǰͨѶ�ĳ�����ֵ
		bool			_bRoot;					// ��׿�豸�Ƿ��Ѿ�root
		std::wstring    _FirewareVersion;		// ����ǽ�汾��

#pragma endregion

#pragma region IOS�豸��Ϣ

        bool IsPad() const { return _deviceType == Ios &&
            (_deviceProductType.substr(0, 4) == L"iPad" || _deviceProductType.substr(0,5) == L"iProd"); }

        bool IsPod() const { return _deviceType == Ios && _deviceProductType.substr(0, 4) == L"iPod"; }

        bool IsPhone() const { return _deviceType == Ios && _deviceProductType.substr(0, 6) == L"iPhone"; }

        bool Is2x() const {  return _deviceType == Ios && 
            ( _deviceProductType.substr(0, 7) == L"iPhone3" || _deviceProductType.substr(0, 5) == L"iPod4" ); }

		void * _ios_devptr;				// USBģʽ������/�Ͽ�ʱ�򷵻ص��豸���
		void * _ios_afcSocket;			// USBģʽ�µĵ�ǰ������
		void * _ios_afcHandle;			// USBģʽ�µĵ�ǰ���Ӿ��

        void * _ios_afc1Socket;         // USBģʽ�µ�"com.apple.afc"������
        void * _ios_afc1Handle;         // USBģʽ�µ�"com.apple.afc"���Ӿ��
        void * _ios_afc2Socket;         // USBģʽ�µ�"com.apple.afc2"������
        void * _ios_afc2Handle;         // USBģʽ�µ�"com.apple.afc2"���Ӿ��


		void * _ios_afcSocketApp; 		// USBģʽ�°�װӦ�ó���ķ�����
		void * _ios_afcHandleApp; 		// USBģʽ�°�װӦ�ó�������Ӿ��

		bool _ios_bAFCConn;				// �Ƿ�������
		bool _ios_bJailBreaked;			// �Ƿ�Խ��
		bool _ios_daemonInstalled;		// �ػ��Ƿ�װ

//		std::string _ios_deviceSerialNumber;		// ���кţ�Wifi���޷�ȡ��
		std::wstring _ios_deviceIdentifier; 		// �豸��Ψһ��ʶ
		std::wstring _ios_deviceClass;
		std::wstring _ios_deviceFWVersion;			// iBoot�İ汾����������Ĺ̼��汾
		std::wstring _ios_deviceBuildVersion;		// �̼����ڲ����
		std::wstring _ios_deviceICCID;              // SIM����ʶ
		std::wstring _ios_devicePhoneNumber;		// �绰����
		std::wstring _ios_deviceBTMac;				// ����MAC��ַ
		int64_t		 _ios_deviceECID;
		int64_t		 _ios_deviceCapablity;
        int          _ios_deviceProductVersion;     // IOS�豸�汾�����ֱ�ʾ

		std::wstring _ios_deviceModelNumber;
		std::wstring _ios_deviceRegionInfo;				// ���е���Ϣ
		std::wstring _ios_deviceBBSerialNumber;
		int64_t       _ios_deviceBBGoldCerID;			// ����
		std::wstring _ios_deviceBBNonce;				// Baseband Nonce
        wstring      _ios_activationState;              // ����״̬
		bool         _ios_deviceActivated;				// �Ƿ��Ѿ�����
		std::wstring _ios_deviceColor;					// �豸��ɫ
		bool		 _ios_devicebTurnCypher;			// �ػ��Ƿ�������
		std::wstring _ios_deviceszDaemonCypherBytes;	// ��������

		std::wstring _ios_deviceIP;						// �豸WIFI���ӵ�IP��ַ
		std::wstring _ios_devicePSW;					// �豸WIFI���ӵ�����
		int			 _ios_wifi_handle;					// wifi���Ӿ��
        bool         _ios_isSimInserted;                // �Ƿ���SIM��
        wstring      _ios_simStatus;                    // SIM��״̬
        wstring      _ios_timeZone;                     // �豸ʱ��,����豸��ȡ����,Ĭ��'Asia/Shanghai'
        double       _ios_timeZoneOffsetFromUTC;        // �豸ʱ����UTC��,����豸��ȡ����,Ĭ��'Asia/Shanghai'��Ӧ��28800.0

		void*		 _ios_house_arrest_service_handle;  // Ӧ�ó����������
		void*		 _ios_house_arrest_connect_handle;  // Ӧ�ó��������ͨ�ž��

        void*        _ios_pInstallationProxySocket;     // IOS��װ,ж��,���ݷ���Э��SOCKET�����ظ�ʹ�õ�����ͬʱʹ��

		// �豸���͵Ķ�����, ��"iPhone3,1"�Ķ�������"iPhone4"
		// ��������Э���е��豸���Ͳ�������������Ҫͨ�������豸�����ṩӦ�ó����б����Ϣ
		std::wstring GetDevModelName() const;

		// �豸���͵��û��Ѻ���ʾ����, ��"iPad2,3"���Ѻ���ʾ������"iPad2 WiFi + 3G(WCDMA)"
		// �ṩ�������ֱ�۵���ʵ�豸����
		std::wstring GetDevTypeText() const;

        // ȡ���豸���ƣ��� XXXX ��iPhone
        std::wstring GetDeviceName() const;

		// ��Ļ��ȸ߶�����
		int GetScreenWidth() const;
		int GetScreenHeight() const;

		 CRITICAL_SECTION _csIOSSessionLock;
         CRITICAL_SECTION _csAuthorizeLock;             // ������Ȩ��
         CRITICAL_SECTION _csInstallationProxyLock;     // IOS��װ,ж��,���ݷ���Э����

#pragma endregion IOS�豸��Ϣ
    };
}
