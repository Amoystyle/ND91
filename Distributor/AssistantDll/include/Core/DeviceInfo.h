#pragma once
#include <windows.h>
#include "AssistantDll.h"
#include <string>
#include "stdint.h"
using namespace std;
#pragma warning(disable : 4251)

namespace ND91Assistant
{
    enum DEVICE_OS_TYPE // 改名，因为与wininet中的名字重复
    {
        Android,
        Ios
    };

	// 平台类型 - 如果苹果有区分平台，也可以放到这里来
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

    // 所有硬件信息，以及连接信息
    class CLASSINDLL_CLASS_DECL DeviceInfo
    {
    public:
        DeviceInfo();
		~DeviceInfo();
#pragma region 共有信息

        DEVICE_OS_TYPE     _deviceType;    // 设备类型
        CONNECT_TYPE    _connectType;   // 连接类型
		DEVICE_PLATFORM _platform;		// 平台类型	

        std::wstring 	_strSerialNumber;		// 安卓设备硬件编号采用新编号
		std::wstring 	_strSerialNumberForShow;	// 由于phonecenter那边重新编码了序号，所以展示的时候需要一个真正的序列号
		std::wstring 	_strHost;				// 安卓当前连接的Host
		short	     	_nPort;					// 安卓当前连接的端口值

        std::wstring 	_deviceIMEI;            // IMEI 移动设备标识
		std::wstring	_deviceIMSI;			// IMSI	移动运营商用户标识
        std::wstring 	_manufacturer;          // 厂家
		std::wstring 	_deviceHWModel;         // 硬件型号, 如N90AP
        std::wstring    _deviceName;			// 设备名
		std::wstring 	_deviceProductType;		// 系统 类似iPad1,1
        std::wstring 	_deviceProductVersion;  // 版本 这个才是固件版本 IOS版本的数字表示
        std::wstring 	_language;              // 当前手机使用语言，考虑用于指导界面更换语言
        std::wstring    _deviceWiFiMac;			// WiFi的MAC地址
 
		std::wstring 	_daemonVer;				// 守护版本
		std::wstring    _deviceBaseVersion;		// 基带版本
		std::wstring    _deviceCoreVersion;		// 内核版本
		std::wstring    _deviceInsdeVersion;	// 内部版本
		std::wstring    _deviceCPUArch;			// CPU型号

		std::wstring    _deviceResolution;		// 分辨率
		std::wstring    _deviceBattery;			// 设备电量

#pragma endregion

#pragma region Android设备信息

		bool IsLenovo() const {return (_deviceIMEI.find(L"863664")==0 || _deviceIMEI.find(L"356727")==0);}

		std::wstring	_sdcardpath;

		short			_nLongConnectSign;		// 安卓用于当前通讯的长连接值
		bool			_bRoot;					// 安卓设备是否已经root
		std::wstring    _FirewareVersion;		// 防火墙版本号

#pragma endregion

#pragma region IOS设备信息

        bool IsPad() const { return _deviceType == Ios &&
            (_deviceProductType.substr(0, 4) == L"iPad" || _deviceProductType.substr(0,5) == L"iProd"); }

        bool IsPod() const { return _deviceType == Ios && _deviceProductType.substr(0, 4) == L"iPod"; }

        bool IsPhone() const { return _deviceType == Ios && _deviceProductType.substr(0, 6) == L"iPhone"; }

        bool Is2x() const {  return _deviceType == Ios && 
            ( _deviceProductType.substr(0, 7) == L"iPhone3" || _deviceProductType.substr(0, 5) == L"iPod4" ); }

		void * _ios_devptr;				// USB模式下连接/断开时候返回的设备句柄
		void * _ios_afcSocket;			// USB模式下的当前服务句柄
		void * _ios_afcHandle;			// USB模式下的当前连接句柄

        void * _ios_afc1Socket;         // USB模式下的"com.apple.afc"服务句柄
        void * _ios_afc1Handle;         // USB模式下的"com.apple.afc"连接句柄
        void * _ios_afc2Socket;         // USB模式下的"com.apple.afc2"服务句柄
        void * _ios_afc2Handle;         // USB模式下的"com.apple.afc2"连接句柄


		void * _ios_afcSocketApp; 		// USB模式下安装应用程序的服务句柄
		void * _ios_afcHandleApp; 		// USB模式下安装应用程序的连接句柄

		bool _ios_bAFCConn;				// 是否连接上
		bool _ios_bJailBreaked;			// 是否越狱
		bool _ios_daemonInstalled;		// 守护是否安装

//		std::string _ios_deviceSerialNumber;		// 序列号，Wifi下无法取到
		std::wstring _ios_deviceIdentifier; 		// 设备的唯一标识
		std::wstring _ios_deviceClass;
		std::wstring _ios_deviceFWVersion;			// iBoot的版本，并不是真的固件版本
		std::wstring _ios_deviceBuildVersion;		// 固件的内部编号
		std::wstring _ios_deviceICCID;              // SIM卡标识
		std::wstring _ios_devicePhoneNumber;		// 电话号码
		std::wstring _ios_deviceBTMac;				// 蓝牙MAC地址
		int64_t		 _ios_deviceECID;
		int64_t		 _ios_deviceCapablity;
        int          _ios_deviceProductVersion;     // IOS设备版本的数字表示

		std::wstring _ios_deviceModelNumber;
		std::wstring _ios_deviceRegionInfo;				// 发行地信息
		std::wstring _ios_deviceBBSerialNumber;
		int64_t       _ios_deviceBBGoldCerID;			// 基带
		std::wstring _ios_deviceBBNonce;				// Baseband Nonce
        wstring      _ios_activationState;              // 激活状态
		bool         _ios_deviceActivated;				// 是否已经激活
		std::wstring _ios_deviceColor;					// 设备颜色
		bool		 _ios_devicebTurnCypher;			// 守护是否开启交互
		std::wstring _ios_deviceszDaemonCypherBytes;	// 交互口令

		std::wstring _ios_deviceIP;						// 设备WIFI连接的IP地址
		std::wstring _ios_devicePSW;					// 设备WIFI连接的密码
		int			 _ios_wifi_handle;					// wifi连接句柄
        bool         _ios_isSimInserted;                // 是否有SIM卡
        wstring      _ios_simStatus;                    // SIM卡状态
        wstring      _ios_timeZone;                     // 设备时区,如果设备获取不到,默认'Asia/Shanghai'
        double       _ios_timeZoneOffsetFromUTC;        // 设备时区与UTC差,如果设备获取不到,默认'Asia/Shanghai'对应的28800.0

		void*		 _ios_house_arrest_service_handle;  // 应用程序共享服务句柄
		void*		 _ios_house_arrest_connect_handle;  // 应用程序共享服务通信句柄

        void*        _ios_pInstallationProxySocket;     // IOS安装,卸载,备份服务协议SOCKET，可重复使用但不可同时使用

		// 设备类型的短名称, 如"iPhone3,1"的短名称是"iPhone4"
		// 用于网络协议中的设备类型参数，如服务端需要通过具体设备类型提供应用程序列表等信息
		std::wstring GetDevModelName() const;

		// 设备类型的用户友好显示名称, 如"iPad2,3"的友好显示名称是"iPad2 WiFi + 3G(WCDMA)"
		// 提供给界面更直观的现实设备类型
		std::wstring GetDevTypeText() const;

        // 取得设备名称，如 XXXX 的iPhone
        std::wstring GetDeviceName() const;

		// 屏幕宽度高度像素
		int GetScreenWidth() const;
		int GetScreenHeight() const;

		 CRITICAL_SECTION _csIOSSessionLock;
         CRITICAL_SECTION _csAuthorizeLock;             // 正版授权锁
         CRITICAL_SECTION _csInstallationProxyLock;     // IOS安装,卸载,备份服务协议锁

#pragma endregion IOS设备信息
    };
}
