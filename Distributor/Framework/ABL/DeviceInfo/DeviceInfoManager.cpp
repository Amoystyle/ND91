#include "stdafx.h"
#include "DeviceInfoManager.h"
#include "LoadDeviceInfoThread.h"
#include "LoadDeviceSpaceThread.h"
#include "LoadDevicePowerInfoThread.h"
#include "LoadDeviceImageThread.h"
#include "RebootDeviceThread.h"
#include "ClearMemoryThread.h"
#include "RecoveryDeviceThread.h"
#include "FixFlashExitThread.h"
#include "ChangeInstallPositionThread.h"


DeviceInfoManager::DeviceInfoManager()
{
}

DeviceInfoManager::~DeviceInfoManager()
{
    AutoLock;
    for (map<wstring, AblDeviceInfo*>::iterator it = _infoMap.begin(); it != _infoMap.end(); it++)
    {
        SAFE_DELETE(it->second);
    }
    _infoMap.clear();
}

bool DeviceInfoManager::Handle( RequestDeviceTypeMsg* pMsg )
{
    DeviceData* pDevice = DEVICE_MANAGER->FindDevice(pMsg->deviceID);
    if (!pDevice)
    {
		pMsg->deviceType = ePhoneOS_None;
    }
	else
	{
		pMsg->deviceType = pDevice->GetDevInfo()->_deviceType == Android? ePhoneOS_Android : ePhoneOS_Ios;  
	} 

    return true;
}

bool DeviceInfoManager::Handle( const RequestDeviceInfoMsg* pMsg )
{
	int nID = Singleton::GetThreadManager()->GetGUID();
	string threadID = "RequestDeviceInfoThread-" + CCodeOperation::UnicodeToGB2312(pMsg->deviceID);

	LoadDeviceInfoThread* pThread = new LoadDeviceInfoThread(threadID, nID, pMsg->deviceID);
	pThread->SetDeviceInfo(GetDeviceInfo(pMsg->deviceID));
	pThread->SetIsRefresh(pMsg->bRefresh);
	pThread->Run();

    return true;
}

bool DeviceInfoManager::Handle( const RequestDeviceSpaceMsg* pMsg )
{
	int nID = Singleton::GetThreadManager()->GetGUID();
	string threadID = "RequestDeviceSpaceThread-" + CCodeOperation::UnicodeToGB2312(pMsg->deviceID);

	LoadDeviceSpaceThread* pThread = new LoadDeviceSpaceThread(threadID, nID, pMsg->deviceID);
	pThread->SetDeviceInfo(GetDeviceInfo(pMsg->deviceID));
	pThread->Run();

	return true;
}

bool DeviceInfoManager::Handle( const DeviceConnectMsg* pMsg )
{
    AblDeviceInfo* pInfo = GetDeviceInfo(pMsg->deviceID);
    if (pInfo)
    {
        pInfo->bIsJailBreak = pMsg->bIsJailBreak;
        pInfo->deviceName = pMsg->deviceName;
    }
    return true;
}

bool DeviceInfoManager::Handle( const RequestDevicePowerInfoMsg* pMsg )
{
	int nID = Singleton::GetThreadManager()->GetGUID();
	string threadID = "RequestDevicePowerInfoThread-" + CCodeOperation::UnicodeToGB2312(pMsg->deviceID);

	Singleton::GetThreadManager()->ExitThread(threadID);

	LoadDevicePowerInfoThread* pThread = new LoadDevicePowerInfoThread(threadID, nID, pMsg->deviceID);
	pThread->SetDeviceInfo(GetDeviceInfo(pMsg->deviceID));
	pThread->Run();

	return true;
}

bool DeviceInfoManager::Handle( const RequestDeviceImageMsg* pMsg )
{
	string threadID = "RequestDeviceImageThread-" + CCodeOperation::UnicodeToGB2312(pMsg->deviceID);

	Singleton::GetThreadManager()->ExitThread(threadID);

	LoadDeviceImageThread* pThread = new LoadDeviceImageThread(threadID, pMsg->deviceID, pMsg->bRefresh);
	pThread->Run();

	return true;
}

bool DeviceInfoManager::Handle( const RequestRebootDeviceMsg* pMsg )
{
	string threadID = "RequestRebootDeviceThread-" + CCodeOperation::UnicodeToGB2312(pMsg->deviceID);

	Singleton::GetThreadManager()->ExitThread(threadID);

	RebootDeviceThread* pThread = new RebootDeviceThread(threadID, pMsg->deviceID);
	pThread->Run();
	
 	return true;
}

bool DeviceInfoManager::Handle( const RequestRecoveryDeviceMsg* pMsg )
{
	string threadID = "RequestRecoveryDeviceThread-" + CCodeOperation::UnicodeToGB2312(pMsg->deviceID);

	Singleton::GetThreadManager()->ExitThread(threadID);

	RecoveryDeviceThread* pThread = new RecoveryDeviceThread(threadID, pMsg->deviceID);
	pThread->Run();

	return true;
}

bool DeviceInfoManager::Handle( const RequestClearDeviceMemoryMsg* pMsg )
{
	string threadID = "RequestClearDeviceMemoryThread-" + CCodeOperation::UnicodeToGB2312(pMsg->deviceID);

	Singleton::GetThreadManager()->ExitThread(threadID);

	ClearMemoryThread* pThread = new ClearMemoryThread(threadID, pMsg->deviceID);
	pThread->Run();

	return true;
}

bool DeviceInfoManager::Handle( const RequestRepairDeviceMsg* pMsg )
{
	string threadID = "RequestRepairDeviceThread-" + CCodeOperation::UnicodeToGB2312(pMsg->deviceID);

	Singleton::GetThreadManager()->ExitThread(threadID);

	FixFlashExitThread* pThread = new FixFlashExitThread(threadID, pMsg->deviceID);
	pThread->Run();

	return true;
}

bool DeviceInfoManager::Handle( const RequestChangeInstallPositionMsg* pMsg )
{
	AblDeviceInfo* pInfo = GetDeviceInfo(pMsg->deviceID);
	pInfo->bInstallOnPhone = pMsg->bInstallOnPhone;
	return true;
}

bool DeviceInfoManager::Handle( RequestInstallPositionMsg* pMsg )
{
	AblDeviceInfo* pInfo = GetDeviceInfo(pMsg->deviceID);	
	pMsg->bInstallOnPhone = pInfo->bInstallOnPhone;
	return true;
}

bool DeviceInfoManager::Handle( const RequestExitRecoveryDeviceMsg* pMsg )
{
	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(pMsg->deviceID);
	if (!pDevice) 
		return false;

	if(pDevice->GetDevInfo()->_deviceType != Ios )
		return false;

	pDevice->GetSystemManager()->RebootToNormal();

	return true;
}

bool DeviceInfoManager::Handle( RequestDeviceStateMsg* pMsg )
{
	DeviceData* pDeviceData = DEVICE_MANAGER->FindDevice(pMsg->deviceID);
	
	pMsg->bResult = pDeviceData != NULL;
	if(!pMsg->bResult)
		return true;

	pMsg->nOsType = pDeviceData->GetDevInfo()->_deviceType == Ios? ePhoneOS_Ios : ePhoneOS_Android;
	if(pMsg->nOsType == ePhoneOS_Ios)
	{
		pMsg->bIsActive = pDeviceData->GetDevInfo()->_ios_deviceActivated;
		pMsg->bIsJailBreak = pDeviceData->GetDevInfo()->_ios_bJailBreaked;
		wstring strVal = pDeviceData->GetId().empty()? pDeviceData->GetDevInfo()->_deviceIMEI : pDeviceData->GetId();
		pMsg->strTrueImei = CCodeOperation::UnicodeToGB2312(strVal);
	}
	else
	{
		pMsg->bIsActive = pDeviceData->GetDevInfo()->_ios_deviceActivated;
		pMsg->bIsRoot = pDeviceData->GetDevInfo()->_bRoot;

		wstring strVal  = pDeviceData->GetDevInfo()->_deviceIMEI.empty() ? pDeviceData->GetDevInfo()->_deviceWiFiMac : pDeviceData->GetDevInfo()->_deviceIMEI;

		if (strVal.empty())
		{
			strVal = pDeviceData->GetDevInfo()->_strSerialNumber;
		}
		pMsg->strTrueImei = CCodeOperation::UnicodeToGB2312(strVal);
	}
	pMsg->bIsPad = pDeviceData->GetDevInfo()->IsPad();
	pMsg->strDeviceIMSI = pDeviceData->GetDevInfo()->_deviceIMSI;
	pMsg->deviceProductType = pDeviceData->GetDevInfo()->_deviceProductType;
	pMsg->deviceProductVersion = pDeviceData->GetDevInfo()->_deviceProductVersion;
	
	return true;
}


bool DeviceInfoManager::Handle( GetAblDeviceInfoMsg* pMsg )
{
	AblDeviceInfo* pInfo = GetDeviceInfo(pMsg->deviceID);
	if(pInfo->baseInfo.deviceIMEI.empty() || pInfo->baseInfo.deviceIMEI == L"0")
	{
		DeviceData* pData = DEVICE_MANAGER->FindDevice(pMsg->deviceID);
		if(pData != NULL)
		{
			pInfo->baseInfo.deviceIMEI				= pData->GetDevInfo()->_deviceIMEI;	
			pInfo->baseInfo.serialNumber			= pData->GetDevInfo()->_strSerialNumber;
			pInfo->strMacAddress					= pData->GetDevInfo()->_deviceWiFiMac;
			pInfo->baseInfo.deviceIMSI				= pData->GetDevInfo()->_deviceIMSI;
			pInfo->bIsJailBreak						= pData->GetDevInfo()->_ios_bJailBreaked;
			pInfo->bIsRoot							= pData->GetDevInfo()->_bRoot;
			pInfo->baseInfo.nOsType					= pData->GetDevInfo()->_deviceType == Ios ? ePhoneOS_Ios : ePhoneOS_Android;
			pInfo->connectType						= pData->GetDevInfo()->_connectType == USB ? USB_TYPE : WIFI_TYPE;
			pInfo->baseInfo.deviceProductVersion	= pData->GetDevInfo()->_deviceProductVersion;
			pInfo->baseInfo.deviceModelName			= pData->GetDevInfo()->GetDevModelName();

			if ( pData->GetDevInfo()->_deviceType == Ios )
			{
				if ( pData->GetDevInfo()->IsPod() )
				{
					pInfo->baseInfo.nPlatForm = ePhonePlatForm_iPod;
				}
				else if ( pData->GetDevInfo()->IsPad() )
				{
					pInfo->baseInfo.nPlatForm = ePhonePlatForm_iPad;
				}
				else
				{
					pInfo->baseInfo.nPlatForm = ePhonePlatForm_iPhone;
				}	 	
			}
			else
			{
				pInfo->baseInfo.nPlatForm = ePhonePlatForm_Android;
			}
		}
	}
	pMsg->info = new AblDeviceInfo(pInfo);
	return true;
}

AblDeviceInfo* DeviceInfoManager::GetDeviceInfo(wstring deviceKey)
{
    AutoLock;
    map<wstring, AblDeviceInfo*>::iterator it = _infoMap.find(deviceKey);
    if (it != _infoMap.end())
        return it->second;

    AblDeviceInfo* pDeviceInfo = new AblDeviceInfo();
    _infoMap[deviceKey] = pDeviceInfo;
    return pDeviceInfo;
}
