#include "stdafx.h"
#include "LoadDeviceInfoThread.h"

LoadDeviceInfoThread::LoadDeviceInfoThread( string threadID, int nRequestID, wstring deviceID )
						: DeviceBaseThread(threadID, deviceID)
{
    _autoDelete = true;    
	_nRequestID = nRequestID;
	_bRefresh = false;
	_pInfo = NULL;
}

LoadDeviceInfoThread::~LoadDeviceInfoThread()
{
}

unsigned long LoadDeviceInfoThread::Main()
{
    this->LoadInfo();

	return 0;
}

void LoadDeviceInfoThread::LoadInfo()
{
    try
    {
		this->GetBaseInfo();

		RETURN_IF(CheckExit());

		this->GetDeviceName();

		RETURN_IF(CheckExit());

		this->GetDeviceState();

		RETURN_IF(CheckExit());
    }
    catch (...)
    {
    }

    NotifyLoadDeviceInfoFinishMsg msg;
    msg.deviceID = _deviceID;
    msg.SendToUI();
}

void LoadDeviceInfoThread::SetDeviceInfo( AblDeviceInfo* pInfo )
{
	_pInfo = pInfo;
}

void LoadDeviceInfoThread::SetIsRefresh( bool bRefresh )
{
	_bRefresh = bRefresh;
}

void LoadDeviceInfoThread::GetBaseInfo()
{
	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_deviceID);
	if (!pDevice) 
		return;

	RETURN_IF(CheckExit());

	bool bRec = true;  
	
	if (pDevice->GetDevInfo()->_deviceType == Android && _bRefresh)
	{
		pDevice->AndroidReadDeviceInfo();
	}

	RETURN_IF(CheckExit());

	ResponseDeviceBaseInfoMsg msg;
	msg.deviceID = _deviceID;

	if(_pInfo != NULL && _pInfo->bLoaded && !_bRefresh)
	{
		msg.nOsType = _pInfo->baseInfo.nOsType;
		msg.serialNumber = _pInfo->baseInfo.serialNumber;
		msg.deviceIMEI = _pInfo->baseInfo.deviceIMEI;
		msg.deviceProductVersion = _pInfo->baseInfo.deviceProductVersion;
		msg.deviceCoreVersion = _pInfo->baseInfo.deviceCoreVersion;
		msg.deviceBaseVersion = _pInfo->baseInfo.deviceBaseVersion;

		RETURN_IF(CheckExit());

		msg.uuid = _pInfo->baseInfo.uuid;
		msg.manufacturer = _pInfo->baseInfo.manufacturer;
		msg.deviceHWModel = _pInfo->baseInfo.deviceHWModel;
		msg.deviceProductType = _pInfo->baseInfo.deviceProductType;
		msg.macAddress = _pInfo->strMacAddress;

		RETURN_IF(CheckExit());
		msg.SendToUI();

		msg.deviceProductTypeAlias = _pInfo->baseInfo.deviceProductTypeAlias;
		msg.strSellArea = _pInfo->baseInfo.strSellArea;

		RETURN_IF(CheckExit());

		msg.deviceInsideVersion = _pInfo->baseInfo.deviceInsideVersion;
		msg.iccid = _pInfo->baseInfo.iccid;
		msg.mobileNumber = _pInfo->baseInfo.mobileNumber;
		msg.deviceIMEI = _pInfo->baseInfo.deviceIMEI;
		msg.macAddress = _pInfo->strMacAddress;
		msg.strRomBuildTeam = L"";

		RETURN_IF(CheckExit());

		if (pDevice->GetDevInfo()->_deviceType == Ios)
		{
			msg.deviceColor = _pInfo->baseInfo.deviceColor;   
			msg.bIsActived = _pInfo->bIsActived;
			msg.bIsJailBreak = _pInfo->bIsJailBreak;
			msg.bIsPad		= _pInfo->baseInfo.nPlatForm == ePhonePlatForm_iPad;
		}
		else
		{
			msg.bIsRoot = _pInfo->bIsRoot;
		}
	}
	else
	{
		msg.nOsType = pDevice->GetDevInfo()->_deviceType == Ios? ePhoneOS_Ios:ePhoneOS_Android;
		msg.serialNumber = pDevice->GetDevInfo()->_strSerialNumber;
		msg.deviceIMEI = pDevice->GetDevInfo()->_deviceIMEI;
		msg.deviceProductVersion = pDevice->GetDevInfo()->_deviceProductVersion;
		msg.deviceCoreVersion = pDevice->GetDevInfo()->_deviceCoreVersion;
		msg.deviceBaseVersion = pDevice->GetDevInfo()->_deviceBaseVersion;

		RETURN_IF(CheckExit());

		msg.uuid = pDevice->GetDevInfo()->_ios_deviceIdentifier;
		msg.manufacturer = pDevice->GetDevInfo()->_manufacturer;
		msg.deviceHWModel = pDevice->GetDevInfo()->GetDevModelName();
		msg.deviceProductType = pDevice->GetDevInfo()->_deviceProductType;
		msg.macAddress = pDevice->GetDevInfo()->_deviceWiFiMac;

		RETURN_IF(CheckExit());
		msg.SendToUI();

		if((!pDevice->IsActivate() && pDevice->GetDevInfo()->_deviceType == Android))
			return;

		if (pDevice->GetDevInfo()->_deviceType == Android && _bRefresh)
		{
			pDevice->AndroidReadDeviceOtherInfo();  
		}

		RETURN_IF(CheckExit());

		msg.deviceProductTypeAlias = pDevice->GetDevInfo()->GetDevTypeText();
		msg.strSellArea = GetIosDeviceRegion(pDevice->GetDevInfo()->_ios_deviceRegionInfo).c_str();

		RETURN_IF(CheckExit());

		msg.deviceInsideVersion = pDevice->GetDevInfo()->_deviceInsdeVersion;
		msg.iccid = pDevice->GetDevInfo()->_ios_deviceICCID;
		msg.mobileNumber = pDevice->GetDevInfo()->_ios_devicePhoneNumber;
		msg.deviceIMEI = pDevice->GetDevInfo()->_deviceIMEI;
		msg.macAddress = pDevice->GetDevInfo()->_deviceWiFiMac;
		msg.strRomBuildTeam = L"";

		RETURN_IF(CheckExit());

		if (pDevice->GetDevInfo()->_deviceType == Ios)
		{
			msg.deviceColor = pDevice->GetDevInfo()->_ios_deviceColor;   
			msg.bIsActived = pDevice->GetDevInfo()->_ios_deviceActivated;
			msg.bIsJailBreak = pDevice->GetDevInfo()->_ios_bJailBreaked;
			msg.bIsPad		= pDevice->GetDevInfo()->IsPad();
		}
		else
		{
			msg.bIsRoot = pDevice->GetDevInfo()->_bRoot;
		}
	}

	RETURN_IF(CheckExit());

	msg.SendToUI();

	if (_pInfo)
	{        
		_pInfo->baseInfo.nOsType = pDevice->GetDevInfo()->_deviceType == Android ? ePhoneOS_Android : ePhoneOS_Ios;
		if (_pInfo->baseInfo.nOsType == ePhoneOS_Android)
		{
			_pInfo->baseInfo.nPlatForm = ePhonePlatForm_Android;
		}
		else
		{
			if (pDevice->GetDevInfo()->IsPad())
				_pInfo->baseInfo.nPlatForm = ePhonePlatForm_iPad;
			else if(pDevice->GetDevInfo()->IsPod())
				_pInfo->baseInfo.nPlatForm = ePhonePlatForm_iPod;
			else
				_pInfo->baseInfo.nPlatForm = ePhonePlatForm_iPhone;
		}

		RETURN_IF(CheckExit());

		_pInfo->strMacAddress = msg.macAddress;
		_pInfo->baseInfo.serialNumber = msg.serialNumber;
		_pInfo->baseInfo.deviceIMEI = msg.deviceIMEI;		       
		_pInfo->baseInfo.manufacturer = msg.manufacturer;          
		_pInfo->baseInfo.deviceHWModel = msg.deviceHWModel;         
		_pInfo->baseInfo.deviceProductVersion = msg.deviceProductVersion;  
		_pInfo->baseInfo.deviceProductType = msg.deviceProductType;     
		_pInfo->baseInfo.deviceProductTypeAlias = msg.deviceProductTypeAlias;
		_pInfo->baseInfo.language = L"";           

		RETURN_IF(CheckExit());

		_pInfo->baseInfo.deviceCoreVersion = msg.deviceCoreVersion;		
		_pInfo->baseInfo.deviceBaseVersion = msg.deviceBaseVersion;     
		_pInfo->baseInfo.deviceInsideVersion = msg.deviceInsideVersion;   
		_pInfo->baseInfo.mobileNumber = msg.mobileNumber;          
		_pInfo->baseInfo.iccid = msg.iccid;                 
		_pInfo->baseInfo.uuid = msg.uuid;                  
		_pInfo->baseInfo.deviceColor = msg.deviceColor;   
		_pInfo->baseInfo.strRomBuildTeam = msg.strRomBuildTeam;
		_pInfo->baseInfo.strSellArea =  msg.strSellArea;
		_pInfo->bIsRoot = msg.bIsRoot;
		_pInfo->bIsJailBreak = msg.bIsJailBreak;
		_pInfo->bIsActived = msg.bIsActived;

		_pInfo->bLoaded = true;
	}
}

void LoadDeviceInfoThread::GetDeviceName( )
{
	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_deviceID);
	if (!pDevice || (!pDevice->IsActivate() && pDevice->GetDevInfo()->_deviceType == Android)) 
		return;

	RETURN_IF(CheckExit());

	ResponseDeviceNameMsg msg;
	msg.deviceID = _deviceID;
	msg.nOsType = pDevice->GetDevInfo()->_deviceType == Ios? ePhoneOS_Ios:ePhoneOS_Android;
	msg.deviceName = pDevice->GetDevInfo()->GetDeviceName();

	RETURN_IF(CheckExit());

	if(msg.deviceName.empty())
		msg.deviceName = pDevice->GetDevInfo()->_deviceHWModel;
	if(msg.deviceName.empty())
		msg.deviceName = pDevice->GetDevInfo()->_strSerialNumberForShow;
	if(msg.deviceName.empty())
		msg.deviceName = pDevice->GetDevInfo()->_strSerialNumber;

	if (_pInfo)
		_pInfo->deviceName = msg.deviceName;

	RETURN_IF(CheckExit());

	msg.SendToUI();
}

void LoadDeviceInfoThread::GetDeviceState( )
{
	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_deviceID);
	if (!pDevice || (!pDevice->IsActivate() && pDevice->GetDevInfo()->_deviceType == Android)) 
		return;

	RETURN_IF(CheckExit());

	ResponseDeviceStateMsg msg;
	msg.deviceID = _deviceID;
	if(pDevice->GetDevInfo()->_deviceType == Android)
	{
		msg.bIsRoot = pDevice->GetDevInfo()->_bRoot;
		msg.nOsType = ePhoneOS_Android;
	}
	else
	{
		msg.bIsActive = pDevice->GetDevInfo()->_ios_deviceActivated;
		msg.nOsType = ePhoneOS_Ios;
	}

	RETURN_IF(CheckExit());

	msg.SendToUI();
}

wstring LoadDeviceInfoThread::GetIosDeviceRegion(wstring ios_deviceRegionInfo)
{
	wstring strRegion = CStrOperation::toUpperW(ios_deviceRegionInfo);
	strRegion = CStrOperation::ReplaceW(strRegion, L"/A", L"");
	if (strRegion == L"AB" )
		return L"中东地区";
	if (strRegion == L"B" )
		return L"英国/爱尔兰";
	if (strRegion == L"C" )
		return L"加拿大";
	if (strRegion == L"CH" )
		return L"中国";
	if (strRegion == L"CZ" )
		return L"捷克";
	if (strRegion == L"DN" )
		return L"德国";
	if (strRegion == L"HN" )
		return L"印度";
	if (strRegion == L"J" )
		return L"日本";
	if (strRegion == L"KH" )
		return L"韩国";
	if (strRegion == L"LL" )
		return L"美国";
	if (strRegion == L"RS" )
		return L"俄罗斯";
	if (strRegion == L"T" )
		return L"意大利";
	if (strRegion == L"TA" )
		return L"台湾";
	if (strRegion == L"X" )
		return L"澳大利亚/新西兰";
	if (strRegion == L"Y" )
		return L"西班牙";
	if (strRegion == L"ZA" )
		return L"新加坡";
	if (strRegion == L"ZP" )
		return L"香港/澳门";
	return L"未知";
}