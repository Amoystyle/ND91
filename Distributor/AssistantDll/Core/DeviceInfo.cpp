#include "stdafx.h"

#include "Core/DeviceInfo.h"
#include "device/AdbGetPropertyCommand.h"

DeviceInfo::DeviceInfo()
{
	_platform = Platform_IOS;
	
	_nLongConnectSign = 0;

	_deviceIMEI= L"";            //IMEI
	_manufacturer= L"";          //厂家
	_deviceHWModel= L"";         //硬件型号, 如N90AP
	_deviceProductVersion= L"";  //这个才是固件版本 IOS版本的数字表示
	_language= L"";              //当前手机使用语言，考虑用于指导界面更换语言

	_ios_devptr = NULL;
	_ios_afcSocket = NULL;
	_ios_afcHandle = NULL;
    _ios_afc1Socket = NULL;
    _ios_afc1Handle = NULL;
    _ios_afc2Socket = NULL;
    _ios_afc2Handle = NULL;

	_ios_bAFCConn = false;
	_ios_bJailBreaked = false;
	_ios_daemonInstalled = false;
	_ios_devicebTurnCypher = false;
	_ios_deviceszDaemonCypherBytes = L"";
	_ios_wifi_handle = 0;

	_ios_house_arrest_service_handle = NULL;
	_ios_house_arrest_connect_handle = NULL;

	_ios_afcSocketApp = NULL;
	_ios_afcHandleApp = NULL;

    _ios_deviceECID = 0;
    _ios_deviceCapablity = 0;
    _ios_deviceBBGoldCerID = 0;
    _ios_deviceProductVersion = 0;
    _ios_deviceActivated = false;

    _ios_pInstallationProxySocket = NULL;

	_sdcardpath  = L"";
	_bRoot		 = false;
    _ios_isSimInserted = false;
    _ios_timeZone = L"Asia/Shanghai";
    _ios_timeZoneOffsetFromUTC = 28800.0;

	 InitializeCriticalSection(&_csIOSSessionLock);
     InitializeCriticalSection(&_csAuthorizeLock);
     InitializeCriticalSection(&_csInstallationProxyLock);
}
DeviceInfo::~DeviceInfo()
{
	DeleteCriticalSection(&_csIOSSessionLock);
    DeleteCriticalSection(&_csAuthorizeLock);
    DeleteCriticalSection(&_csInstallationProxyLock);
}
// 设备类型的短名称, 如"iPhone3,1"的短名称是"iPhone4"
// 用于网络协议中的设备类型参数，如服务端需要通过具体设备类型提供应用程序列表等信息
std::wstring DeviceInfo::GetDevModelName() const
{
	std::wstring strDevModelName = L"";

	if (_deviceType == Ios)
	{
		// apple TV
		if (_deviceProductType == L"AppleTV2,1")
			strDevModelName = L"AppleTV";
		if (_deviceProductType == L"AppleTV3,1")
			strDevModelName = L"AppleTV";
		if (_deviceProductType == L"iProd2,1")
			strDevModelName = L"AppleTV";
		// iPad3
		else if (_deviceProductType == L"iPad3,3")
			strDevModelName = L"The New iPad";
		else if (_deviceProductType == L"iPad3,2")
			strDevModelName = L"The New iPad";
		else if (_deviceProductType == L"iPad3,1")
			strDevModelName = L"The New iPad";
		// iPad2
		else if (_deviceProductType == L"iPad2,1")
			strDevModelName = L"iPad2";
		else if (_deviceProductType == L"iPad2,2")
			strDevModelName = L"iPad2";
		else if (_deviceProductType == L"iPad2,3")
			strDevModelName = L"iPad2";
		else if (_deviceProductType == L"iPad2,4")
			strDevModelName = L"iPad2";
		else if (_deviceProductType == L"iPad2,5")
			strDevModelName = L"iPad mini";
		// iPad
		else if (_deviceProductType == L"iPad1,1")
			strDevModelName = L"iPad";
		else if (_deviceProductType == L"iPad1,2")
			strDevModelName = L"iPad";
		//iPhone
		else if (_deviceProductType == L"iPhone1,1")
			strDevModelName = L"iPhone";
		else if (_deviceProductType == L"iPhone1,2")
			strDevModelName = L"iPhone3G";
		else if (_deviceProductType == L"iPhone2,1")
			strDevModelName = L"iPhone3GS";
		// iPhone4
		else if (_deviceProductType == L"iPhone3,1")
			strDevModelName = L"iPhone4";
		else if (_deviceProductType == L"iPhone3,3")
			strDevModelName = L"iPhone4";
		// iPhone4S
		else if (_deviceProductType == L"iPhone4,1")
			strDevModelName = L"iPhone4S";
		else if (_deviceProductType == L"iPhone4,2")
			strDevModelName = L"iPhone4S";
		// iPod
		else if (_deviceProductType == L"iPod1,1")
			strDevModelName = L"iPod";
		else if (_deviceProductType == L"iPod2,1")
			strDevModelName = L"iPod2G";
		else if (_deviceProductType == L"iPod2,2")
			strDevModelName = L"iPod2G";
		else if (_deviceProductType == L"iPod3,1")
			strDevModelName = L"iPod3G";
		else if (_deviceProductType == L"iPod4,1")
			strDevModelName = L"iPod4G";
		//ios6
		else if (_deviceProductType == L"iPhone5,1")
			strDevModelName = L"iPhone5";
		else if (_deviceProductType == L"iPhone5,2")
			strDevModelName = L"iPhone5";
		else if (_deviceProductType == L"iPod5,1")
			strDevModelName = L"iPod5";
		else if (_deviceProductType == L"iPad3,4")
			strDevModelName = L"iPad4";
		else if (_deviceProductType == L"iPad2,5")
			strDevModelName = L"iPad mini";
		else
			strDevModelName = L"iPhone";
	}
	else if (_deviceType == Android)
	{
		strDevModelName = _deviceHWModel;
        if (strDevModelName == L"")
		{
           // strDevModelName = _strSerialNumber;
			AdbGetPropertyCommand cmd(_strSerialNumber);
			cmd.Execute();
			const DeviceInfo *pInfo = cmd.GetDeviceInfo();   
			strDevModelName			= pInfo->_deviceHWModel;      
		}
	}


	return strDevModelName;
}

// 设备类型的用户友好显示名称, 如"iPad2,3"的友好显示名称是"iPad2 WiFi + 3G(WCDMA)"
// 提供给界面更直观的现实设备类型
std::wstring DeviceInfo::GetDevTypeText() const
{
	std::wstring strDevTypeText = L"";

	if (_deviceType == Ios)
	{
		// apple TV
		if (_deviceProductType == L"AppleTV2,1")
			strDevTypeText = L"AppleTV";
		if (_deviceProductType == L"AppleTV3,1")
			strDevTypeText = L"AppleTV";
		if (_deviceProductType == L"iProd2,1")
			strDevTypeText = L"AppleTV";
		// iPad3
		else if (_deviceProductType == L"iPad3,3")
			strDevTypeText = L"New iPad 4G(CDMA)";
		else if (_deviceProductType == L"iPad3,2")
			strDevTypeText = L"New iPad 4G";
		else if (_deviceProductType == L"iPad3,1")
			strDevTypeText = L"New iPad";
		// iPad2
		else if (_deviceProductType == L"iPad2,1")
			strDevTypeText = L"iPad2";
		else if (_deviceProductType == L"iPad2,2")
			strDevTypeText = L"iPad2 3G(WCDMA)";
		else if (_deviceProductType == L"iPad2,3")
			strDevTypeText = L"iPad2 3G(CDMA)";
		else if (_deviceProductType == L"iPad2,4")
			strDevTypeText = L"iPad2 R2";
		else if (_deviceProductType == L"iPad2,5")
			strDevTypeText = L"iPad mini";
		// iPad
		else if (_deviceProductType == L"iPad1,1")
			strDevTypeText = L"iPad";
		else if (_deviceProductType == L"iPad1,2")
			strDevTypeText = L"iPad + 3G(WCDMA)";
		//iPhone
		else if (_deviceProductType == L"iPhone1,1")
			strDevTypeText = L"iPhone GSM";
		else if (_deviceProductType == L"iPhone1,2")
			strDevTypeText = L"iPhone 3G";
		else if (_deviceProductType == L"iPhone2,1")
			strDevTypeText = L"iPhone 3GS";
		// iPhone4
		else if (_deviceProductType == L"iPhone3,1")
			strDevTypeText = L"iPhone 4";
		else if (_deviceProductType == L"iPhone3,3")
			strDevTypeText = L"iPhone 4 CDMA";
		// iPhone4S
		else if (_deviceProductType == L"iPhone4,1")
			strDevTypeText = L"iPhone 4S";
		else if (_deviceProductType == L"iPhone4,2")
			strDevTypeText = L"iPhone 4S (CDMA)";
		// iPod
		else if (_deviceProductType == L"iPod1,1")
			strDevTypeText = L"iPod Touch";
		else if (_deviceProductType == L"iPod2,1")
			strDevTypeText = L"iPod Touch 2";
		else if (_deviceProductType == L"iPod2,2")
			strDevTypeText = L"iPod Touch 2 Prototype";
		else if (_deviceProductType == L"iPod3,1")
			strDevTypeText = L"iPod Touch 3";
		else if (_deviceProductType == L"iPod4,1")
			strDevTypeText = L"iPod Touch 4";
		else if (_connectType == RECOVERY)
			strDevTypeText = L"Recovery mode devices";
		else
			strDevTypeText = _deviceProductType;
	}
	else if (_deviceType == Android)
	{
        strDevTypeText = L"Android " + _deviceProductVersion;
		//strDevTypeText = _manufacturer + L" " + _deviceHWModel;
	}

	return strDevTypeText;
}

std::wstring ND91Assistant::DeviceInfo::GetDeviceName() const
{
    if (_deviceType == Ios)    
        return _deviceName;
    else
    {
        if ( _deviceName.empty())
            return GetDevModelName();
        return _deviceName;
    }
}

int ND91Assistant::DeviceInfo::GetScreenWidth() const
{
	int iScreenWidth = 320;
	if (_deviceType == Ios)
	{
		wstring strLower = _deviceProductType;
		//transform(strLower.begin(), strLower.end(), strLower.begin(), tolower);
        strLower = CStrOperation::toLowerW(strLower);

		if (strLower.compare(0, 7, L"iphone3" ) == 0
			|| strLower.compare(0, 7, L"iphone4") == 0
			|| strLower.compare(0, 5, L"ipod4") == 0)
		{
			iScreenWidth = 640;
		}
		else if (strLower.compare(0, 5, L"ipad1") == 0
			|| strLower.compare(0, 5, L"ipad2") == 0)
		{
			iScreenWidth = 768;
		}
		else if (strLower.compare(0, 4, L"ipad") == 0)
		{
			iScreenWidth = 1536;
		}
	}
	return iScreenWidth;
}

int ND91Assistant::DeviceInfo::GetScreenHeight() const
{
	int iScreenHeight = 480;
	if (_deviceType == Ios)
	{
		wstring strLower = _deviceProductType;
		//transform(strLower.begin(), strLower.end(), strLower.begin(), tolower);
        strLower = CStrOperation::toLowerW(strLower);

		if (strLower.compare(0, 7, L"iphone3" ) == 0
			|| strLower.compare(0, 7, L"iphone4") == 0
			|| strLower.compare(0, 5, L"ipod4") == 0)
		{
			iScreenHeight = 960;
		}
		else if (strLower.compare(0, 5, L"ipad1") == 0
			|| strLower.compare(0, 5, L"ipad2") == 0)
		{
			iScreenHeight = 1024;
		}
		else if (strLower.compare(0, 4, L"ipad") == 0)
		{
			iScreenHeight = 2048;
		}
	}
	return iScreenHeight;
}

