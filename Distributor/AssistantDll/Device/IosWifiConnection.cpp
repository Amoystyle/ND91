#include "stdafx.h"
#include "IosWifiConnection.h"

#include "Device/IosPublicFunc.h"
#include "Device/IosWiFiFileSystem.h"
#include "Device/IosDaemon.h"
#include "Common/CPing.h"

IosWifiConnection::IosWifiConnection(wstring strIP, wstring strPSW, int nWiFiHandle)
{
	_strIP = strIP;
	_strPSW = strPSW;
	_nWiFiHandle = nWiFiHandle;
	_bConn = false;
	_strVersion = L"";
	_strIdentifier = L"";
	_bTurnCypher = false;
	_strDaemonCypherBytes = L"";
	_strDevName = L"";
	_strProductVersion = L"";
	_strSerialNumber = L"";
	_strDaemonVer = L"";
}



IosWifiConnection::~IosWifiConnection()
{

}

bool IosWifiConnection::Connect()
{
	if (_strIP.length() < 0)
		return false;

	int nLastError = ND_ERROR_OTHER;

#ifdef WIN32
	CPing objPing;
	if (objPing.Ping(CCodeOperation::UnicodeToGB2312(_strIP).c_str(), 1500))
#else
	if (ping(CCodeOperation::WstringToString(_strIP), 1500))
#endif				
    {
        if (_nConfirmConfig == 1)
        {
            int nTimeOut = 5000;
            wstring showText = L"电信助手||电信助手请求连接，是否允许？||允许||拒绝";
            OPTION->Get(GlobalOption::STR_IOS_CONNECT_SHOWTEXT, showText);
            OPTION->Get(GlobalOption::INT_IOS_CONNECT_TIMEOUT, nTimeOut);
            _nWiFiHandle = CIosWiFiMobileDevice::WiFiRequestConnect(_strIP, _strPSW, showText, nTimeOut);
            if (!_nWiFiHandle) 
                nLastError = ND_ERROR_CANNOT_CONNECT;
        }
        else
		    _nWiFiHandle = CIosWiFiMobileDevice::WiFiConnect(_strIP, _strPSW, nLastError);
    }
	else
		nLastError = ND_ERROR_CANNOT_CONNECT;

	switch (nLastError)
	{
	case ND_ERROR_SUCCESS:
		{
			if (_nWiFiHandle <= 0)
			{
				THROW_ASSISTANT_EXCEPTION(IOS_WIFI_C0NN_ERROR);
				return false;
			}
		}
		break;
	case ND_ERROR_CANNOT_CONNECT:
		{
			THROW_ASSISTANT_EXCEPTION(IOS_WIFI_C0NN_ERROR);
			return false;
		}
		break;
	case ND_ERROR_PSW_ERROR:
		{
			THROW_ASSISTANT_EXCEPTION(IOS_WIFI_PSW_ERROR);
			return false;
		}
		break;
	}

	_bConn = CheckConnState(_nWiFiHandle, _strVersion);	

	if (!_bConn)
		return false;

	_strIdentifier = GetSerial();
	_strDevName = GetDevName(); 
	_strProductVersion = GetProductVersion();

	PhoneDaemonIsStillRunning();

	return true;
}

bool IosWifiConnection::CheckConnState(int nHandle, wstring &strPhoneVer)
{
	bool bConn = false;
	char aryIPBuff[255] = {0};
	char aryVersionBuff[255] = {0};
	CIosWiFiMobileDevice::WiFiConnectionState(nHandle, bConn, aryIPBuff, aryVersionBuff);

	string strVersionBuff = aryVersionBuff;
	strPhoneVer = CCodeOperation::UTF_8ToUnicode(strVersionBuff);

	return bConn;
}

void IosWifiConnection::Disconnect()
{
}

const DeviceInfo* IosWifiConnection::GetDeviceInfo()
{
	if (_bConn)
	{
		_info._deviceType = Ios;
		_info._connectType = WIFI;
        _info._ios_bAFCConn = true;
        _info._ios_bJailBreaked = true;
        _info._ios_daemonInstalled = true;//wifi能连接 守护必定在运行
        _info._ios_deviceActivated = true;
        _info._ios_deviceIdentifier = _strIdentifier;
		_info._deviceName = _strDevName;
        _info._deviceProductVersion = _strProductVersion;
        _info._ios_deviceProductVersion = IosPublicFunc::GetVerNum(_strProductVersion);
        _info._strSerialNumber = _strSerialNumber;

        _info._manufacturer = L"Apple";
        _info._deviceIMEI = L"";
        _info._deviceIMSI = L"";
        _info._ios_deviceICCID = L"";
        _info._ios_devicePhoneNumber = L"";
        _info._ios_deviceFWVersion = L"";
        _info._ios_deviceBTMac = L"";
        _info._deviceWiFiMac = L"";
        _info._deviceHWModel = L"";
        _info._deviceBaseVersion = L"";
        _info._ios_deviceBBGoldCerID = 0;
        _info._ios_deviceColor = L"";
        _info._deviceCPUArch = L"";
        _info._strSerialNumber = L"";
        _info._ios_devicebTurnCypher = _bTurnCypher;
        _info._ios_deviceszDaemonCypherBytes = _strDaemonCypherBytes;
        _info._ios_wifi_handle = _nWiFiHandle;
		_info._daemonVer = _strDaemonVer;
        _info._ios_isSimInserted = true;//wifi 当前还没有实现,默认返回true.
	}

	return &_info;
}


bool IosWifiConnection::PhoneDaemonIsStillRunning()
{
	IosWiFiFileSystem fileStream(_nWiFiHandle);
	IosDaemon daemon(&fileStream, _strIdentifier);
	bool bRet = daemon.PhoneDaemonIsStillRunning();
	_bTurnCypher = daemon.GetIsTrunCypher();
	_strDaemonCypherBytes = daemon.GetDaemonCypherBytes();
	_strDaemonVer = daemon.GetDaemonVer();

	return bRet;
}

wstring IosWifiConnection::GetSerial()
{ 
	return GetTypeValue(L"/private/var/root/Media/ndDaemon/Info.plist", L"Serial");
}

wstring IosWifiConnection::GetDevName()
{
	wstring strResult = GetTypeValue(L"/private/var/root/Library/Lockdown/data_ark.plist", L"-DeviceName", true);

	if (strResult.length() > 0)
		return strResult;
	else
		return L"Unnaming";
}

wstring IosWifiConnection::GetProductVersion()
{
	return GetTypeValue(L"/System/Library/CoreServices/SystemVersion.plist", L"ProductVersion");
}

wstring IosWifiConnection::GetTypeValue(wstring strFileOnPhone, wstring strType, bool bNeedChangeToXML)
{
	if (!_bConn || _nWiFiHandle <= 0)
		return L"";

	IosWiFiFileSystem fileStream(_nWiFiHandle);

	return IosPublicFunc::GetTypeValue(&fileStream, strFileOnPhone, strType, bNeedChangeToXML);
}

void ND91Assistant::IosWifiConnection::SetTickCallBack( TICKCALLBACK pCallBack, void* pVoid )
{
    if (_nWiFiHandle)
        CIosWiFiMobileDevice::WiFiSetTickCallBack(_nWiFiHandle, pCallBack, pVoid);
}
