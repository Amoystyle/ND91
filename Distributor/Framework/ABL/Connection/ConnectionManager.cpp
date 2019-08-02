#include "stdafx.h"
#include "ConnectionManager.h"
#include "DDMManager.h"
#include "AblDeviceObserver.h"
#include "ActiveDeviceThread.h"
#include "ScanUsbDeviceThread.h"
#include "InitIOSThread.h"
#include "RetryConnectThread.h"
#include "WaitWindowsThread.h"

ConnectionManager::ConnectionManager()
{
	_pDDMManager = new DDMManager(this);
	_pDeviceObserver = new AblDeviceObserver(this);
	_pScanThread = NULL;
	_pInitIOSThread = NULL;
}

ConnectionManager::~ConnectionManager()
{
	DEVICE_MANAGER->StopAdb();
}

void ConnectionManager::Init()
{
	this->_pDeviceObserver->Init();
	this->_pDDMManager->Init();

	_pScanThread = new ScanUsbDeviceThread("ScanDeviceThread");
	_pScanThread->Run();

	_pInitIOSThread = new InitIOSThread("InitIOSThread");
	_pInitIOSThread->Run();

}

void ConnectionManager::OnDeviceLinkIn( void* pPhone, wstring sn, EnumPhoneType phoneType, wstring phoneKey)
{
	LOG->WriteDebug(L"OnDeviceLinkIn: sn->"+sn+L" pPhone->"+CStrOperation::IntToWString((UINT)pPhone));

	DDMDataItem* pItem = this->FindDDMItem(sn);
	if(pItem != NULL && pItem->pPhone == NULL)
	{
		//先判断Assistant.dll的没有提前扫描到的设备.
		//设备接入
		//设备连接
		LOG->WriteDebug(L"OnDeviceLinkIn Branch1: sn->"+sn+L" pPhone->"+CStrOperation::IntToWString((UINT)pPhone));

		pItem->pPhone = pPhone;
		pItem->phoneKey = phoneKey;

		DeviceLinkInMsg msg;
		msg.id = sn;
		msg.nPhoneOs = (phoneType == Android) ? ePhoneOS_Android : ePhoneOS_Ios;
		msg.pPhone = pPhone;
		msg.SendToUI();

		DeviceChangeMsg deviceChangeMsg;
		deviceChangeMsg._bLinkOut = false;
		deviceChangeMsg._st_sn = sn;
		deviceChangeMsg.phoneType = (phoneType == Android) ? 0 : 1;
		deviceChangeMsg.PostToUI();

		OnUsbDeviceConnect(sn, pItem->bIsRecovery);
	}
	else
	{
		//如果设备不存在、通知设备接入
		LOG->WriteDebug(L"OnDeviceLinkIn Branch2: sn->"+sn+L" pPhone->"+CStrOperation::IntToWString((UINT)pPhone));
		DDMDataItem* pItem = this->FindDDMItem(pPhone);
		if(pItem == NULL)
		{
			{
				//添加设备
				AutoLock;

				DDMDataItem* pNewItem = new DDMDataItem();
				pNewItem->pPhone = pPhone;
				pNewItem->phoneKey = phoneKey;
				pNewItem->type = phoneType;
				pNewItem->key = sn;
				this->_vecDDMItems.push_back(pNewItem);
			}

			DeviceLinkInMsg msg;
			msg.id = sn;
			msg.nPhoneOs = (phoneType == Android) ? ePhoneOS_Android : ePhoneOS_Ios;
			msg.pPhone = pPhone;
			msg.SendToUI();

			DeviceChangeMsg deviceChangeMsg;
			deviceChangeMsg._bLinkOut = false;
			deviceChangeMsg._st_sn = sn;
			deviceChangeMsg.phoneType = (phoneType == Android) ? 0 : 1;
			deviceChangeMsg.PostToUI();
		}
	}
}

void ConnectionManager::OnDeviceLinkOut( void* pPhone, wstring sn, EnumPhoneType phoneType )
{
	//停止设备相关线程
	Singleton::GetThreadManager()->ExitThreads(sn);

	LOG->WriteDebug(L"OnDeviceLinkOut: sn->"+sn+L" pPhone->"+CStrOperation::IntToWString((UINT)pPhone));
	{
		//移除设备
		AutoLock;
		for (vector<DDMDataItem*>::iterator it = _vecDDMItems.begin(); it != _vecDDMItems.end(); it++)
		{

			DDMDataItem* item = *it;
			if(item != NULL && (item->pPhone == pPhone || item->key == sn))
			{
				_vecDDMItems.erase(it);
				break;
			}
		}
	}

	DEVICE_MANAGER->OnLinkout(sn);

	//通知设备断开
	DeviceLinkOutMsg msg;
	msg.nDeviceCount = _vecDDMItems.size();
	msg.id = sn;
	msg.nPhoneOs = (phoneType == Android) ? ePhoneOS_Android : ePhoneOS_Ios;
	msg.pPhone = pPhone;
	msg.SendToUI();

	DeviceChangeMsg deviceChangeMsg;
	deviceChangeMsg._bLinkOut = true;
	deviceChangeMsg._st_sn = sn;
	deviceChangeMsg.phoneType = (phoneType == Android) ? 0 : 1;
	deviceChangeMsg.PostToUI();
}

void ConnectionManager::OnUsbDeviceConnect( wstring sn, bool bRecovery )
{
	LOG->WriteDebug(L"OnUsbDeviceConnect: sn->"+sn);
	DDMDataItem* pItem = this->FindDDMItem(sn);
	
	//当前设备已连接，但是PhoneCenter还未通知设备就绪时，先记录设备，在LinkIn时，再通知连接。
	if(pItem == NULL)
	{
		AutoLock;

		LOG->WriteDebug(L"OnUsbDeviceConnect Branch1: sn->"+sn);
		DeviceData* pDeviceDate = DEVICE_MANAGER->FindDevice(sn);
		if(pDeviceDate == NULL)
			return;

		DDMDataItem* pNewItem = new DDMDataItem();
		pNewItem->bIsRecovery = bRecovery;
		pNewItem->pPhone = NULL;
		pNewItem->type = pDeviceDate->GetDevInfo()->_deviceType == Android?EnumPhoneType_Android:EnumPhoneType_IPhone;
		pNewItem->key = sn;
		this->_vecDDMItems.push_back(pNewItem);

		return;
	}

	if(pItem->bWaitWindows)
	{
		pItem->bWaitWindows = false;
		string threadID = "WaitWindowThread-" + CCodeOperation::WstringToString(pItem->key);
		Singleton::GetThreadManager()->ExitThread(threadID);
	}

	LOG->WriteDebug(L"OnUsbDeviceConnect Branch2: sn->"+sn);

	pItem->bIsRecovery = bRecovery;

	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(pItem->key);
	if (pDevice == NULL || pDevice->GetDevInfo() == NULL) 
		return;

	//设备连接时，只加载基础信息。
	//其它信息，在设备激活时，时使用者主动请求。
	DeviceConnectMsg msg;
	msg.nPhoneOs = pDevice->GetDevInfo()->_deviceType == Android?ePhoneOS_Android:ePhoneOS_Ios;
	if (msg.nPhoneOs == ePhoneOS_Ios)
	{
		msg.bIsJailBreak = pDevice->GetDevInfo()->_ios_bJailBreaked;
	}
	else
	{
		pDevice->AndroidReadDeviceInfo();
	}	

	wstring deviceName = pDevice->GetDevInfo()->GetDeviceName();
	if(deviceName.empty())
		deviceName = pDevice->GetDevInfo()->GetDevModelName();
	if(deviceName.empty())
		deviceName = pDevice->GetDevInfo()->_strSerialNumberForShow;
	if(deviceName.empty())
		deviceName = pDevice->GetDevInfo()->_strSerialNumber;

	msg.deviceName = deviceName;
	msg.deviceID = pItem->key;	
	msg.bIsUsb = true;
	msg.bIsRecovery = bRecovery;
	msg.nPhoneOs = pDevice->GetDevInfo()->_deviceType == Android ? ePhoneOS_Android : ePhoneOS_Ios;

	msg.pPhone = pItem->pPhone;
	msg.SendToUI();

	pItem->nAdbErrorRetry = 0;

	//激活设备（与守护建立连接）
	int nID = Singleton::GetThreadManager()->GetGUID();
	string threadID = "RequestInstallAppThread-" + CStrOperation::Int2string(nID);
	ActiveDeviceThread* pThread = new ActiveDeviceThread(threadID, nID, sn, pItem->pPhone);
	pThread->Run();

}

void ConnectionManager::OnRecoveryConnect( wstring sn )
{
	this->OnUsbDeviceConnect(sn, true);
}

void ConnectionManager::OnIosDevJailBreak( wstring sn )
{
	DDMDataItem* pItem = this->FindDDMItem(sn);
	if(pItem == NULL)
		return;

	DeviceJailbreakMsg msg;
	msg.deviceID = sn;
	msg.bJailbreak = true;
	msg.SendToUI();
}

void ConnectionManager::OnIosDevicePasswordLockConnect( wstring sn )
{
	DDMDataItem* pItem = this->FindDDMItem(sn);
	if(pItem == NULL)
		return;

	DeviceConnectErrorMsg msg;
	msg.pPhone = pItem->pPhone;
	msg.nError = DeviceConnectError_ICR_NeedUnLock;
	msg.SendToUI();
}

void ConnectionManager::OnDevicePropertyChange( void* pPhone, EnumPhoneProperty nType, const char* szData )
{
	DDMDataItem* pItem = this->FindDDMItem(pPhone);
	if(pItem == NULL || pItem->bIsRecovery)
		return;

	if (nType == EnumPhoneProperty_ConnectState)
	{
		this->OnConnectStateChange(pPhone, nType, szData);
	}
	else if(nType == EnumPhoneProperty_ErrorCode)
	{
		int nValue = szData? atoi(szData) : 0;

		LOG->WriteDebug(L"EnumPhoneProperty_ErrorCode: pPhone->" + CStrOperation::IntToWString((UINT)pPhone) + L" ErrorCode->" + CStrOperation::IntToWString((UINT)nValue));

		DeviceConnectError nError = this->ParseError(nValue);
		if(nValue!=0 && nError != DeviceConnectError_Check_Success)
		{
			if(DeviceConnectError_Check_ADBError == nError && pItem->IsSamSungPhone())
			{
				int nID = Singleton::GetThreadManager()->GetGUID();
				string threadID = "RequestDeviceInfoThread-" + CStrOperation::Int2string(nID);
				RetryConnectThread* pThread = new RetryConnectThread(threadID, pPhone);
				pThread->Run();
			}
			else
			{
				DeviceConnectErrorMsg msg;
				msg.pPhone = pPhone;
				msg.nError = nError;
				msg.PostToUI();
			}
		}
	}
	else if(nType == EnumPhoneProperty_ProgressTitle)
	{
		int title = szData? atoi(szData) : 0;

		LOG->WriteDebug(L"EnumPhoneProperty_ProgressTitle: pPhone->" + CStrOperation::IntToWString((UINT)pPhone) + L" Title->" + CStrOperation::IntToWString((UINT)title));

		DeviceDriverInstallProgressMsg msg;
		msg.pPhone = pPhone;
		msg.nState = (title%2 == 0)? DriverInstallState_Downloading : DriverInstallState_Installing;
		msg.nStep = title/2;
		pItem->nState = msg.nState;
		pItem->nStep = msg.nStep;
		msg.SendToUI();
	}
	else if(nType == EnumPhoneProperty_Progress)
	{
		int progress = szData? atoi(szData) : 0;
		LOG->WriteDebug(L"EnumPhoneProperty_Progress: pPhone->" + CStrOperation::IntToWString((UINT)pPhone) + L" Progress->" + CStrOperation::IntToWString(progress));
		DeviceDriverInstallProgressMsg msg;
		msg.pPhone = pPhone;
		msg.nState = pItem->nState;
		msg.nStep = pItem->nStep;
		msg.nProgress = progress;
		msg.SendToUI();
	}
	else if(nType == EnumPhoneProperty_Key)
	{
		pItem->key = CCodeOperation::GB2312ToUnicode(szData);

		LOG->WriteDebug(L"EnumPhoneProperty_Key: pPhone->" + CStrOperation::IntToWString((UINT)pPhone) + L" Title->" + pItem->key);

		NotifyDeviceKeyChangeMsg msg;
		msg.newKey = pItem->key;
		msg.pPhone = pItem->pPhone;
		msg.SendToUI();
	}
}

void ConnectionManager::OnConnectStateChange( void* pPhone, EnumPhoneProperty nType, const char* szData )
{
	int nValue = szData ? atoi(szData) : 0;
	switch (nValue)
	{
/*
	case EnumConnectState_ConnectSuccess: //连接成功
		break;
	case EnumConnectState_ConnectActive:
		break;
	case EnumConnectState_UnConnect:
		break;	
*/
	case EnumConnectState_ConnectInit:
		{
			LOG->WriteDebug(L"EnumConnectState_ConnectInit: pPhone->" + CStrOperation::IntToWString((UINT)pPhone));
			DeviceConnectInitMsg msg;
			msg.pPhone = pPhone;
			msg.SendToUI();

			DDMDataItem* pItem = this->FindDDMItem(pPhone);
			if(pItem != NULL)
			{
				pItem->bWaitWindows = true;
				string threadID = "WaitWindowThread-" + CCodeOperation::WstringToString(pItem->key);
				Singleton::GetThreadManager()->ExitThread(threadID);

				WaitWindowsThread* pThread = new WaitWindowsThread(threadID, pItem->key, pItem->pPhone);
				pThread->Run();
			}
		}
		break;
	case EnumConnectState_InstallInit:
		{
			LOG->WriteDebug(L"EnumConnectState_InstallInit: pPhone->" + CStrOperation::IntToWString((UINT)pPhone));
			DeviceDriverInstallInitMsg msg;
			msg.pPhone = pPhone;
			msg.SendToUI();
		}
		break;
	case EnumConnectState_InstallEnd:
		{
			LOG->WriteDebug(L"EnumConnectState_InstallEnd: pPhone->" + CStrOperation::IntToWString((UINT)pPhone));
			DeviceDriverInstallEndMsg msg;
			msg.pPhone = pPhone;
			msg.SendToUI();
		}
		break;
	}	
}

DeviceConnectError ConnectionManager::ParseError( int nValue )
{
	if((nValue & 0xf000) == 0x1000)
		return this->ParseCheckErrorCode(nValue & 0x0fff);
/*
	if((nValue & 0x1000) == 0x1000)
		return this->ParseCheckErrorCode(nValue & 0x0fff);
	else if((nValue & 0x2000) == 0x2000)
		return this->ParseAndroidConnectResult(nValue & 0x0fff); 
	else if((nValue & 0x3000) == 0x3000)
		return this->ParseIphoneConnectResult(nValue & 0x0fff); 
	else if ((nValue & 0x4000) == 0x4000)
		return this->ParseAndroidConnectState(nValue & 0x0fff); 
	else if ((nValue & 0x4000) == 0x5000)
		return this->ParseOtherErrorCode(nValue & 0x0fff); 
	else if ((nValue & 0x4000) == 0x6000)
		return this->ParseIPhoneConnectState(nValue & 0x0fff); 
*/
	else
		return DeviceConnectError_UnKnow;
}

DeviceConnectError ConnectionManager::ParseCheckErrorCode( int nValue )
{
	switch(nValue)
	{
	case EnumCheckErrorCode_Success:
		return DeviceConnectError_Check_Success;
	case EnumCheckErrorCode_Exception:
		return DeviceConnectError_Check_Exception;
	case EnumCheckErrorCode_offline:
		return DeviceConnectError_Check_offline;
	case EnumCheckErrorCode_DriverError:
		return DeviceConnectError_Check_DriverError;
	case EnumCheckErrorCode_OffDebug:
		return DeviceConnectError_Check_OffDebug;
	case EnumCheckErrorCode_ConnectTypeError:
		return DeviceConnectError_Check_ConnectTypeError;
	case EnumCheckErrorCode_ADBError:
		return DeviceConnectError_Check_ADBError;
	case EnumCheckErrorCode_UnConnect:
		return DeviceConnectError_Check_UnConnect;
	case EnumCheckErrorCode_ITunesNoFind:
		return DeviceConnectError_Check_ITunesNoFind;
	case EnumCheckErrorCode_ItunesNeedReconnect:
		return DeviceConnectError_Check_ItunesNeedReconnect;
	case EnumCheckErrorCode_ItunesNeedReconnect2:
		return DeviceConnectError_Check_ItunesNeedReconnect2;
/*
	case EnumCheckErrorCode_DriverUnload:
		return DeviceConnectError_Check_DriverUnload;
	case EnumCheckErrorCode_ConnectModeError:
		return DeviceConnectError_Check_ConnectModeError;
*/
	default:
		return DeviceConnectError_UnKnow;
	}
}

DDMDataItem* ConnectionManager::FindDDMItem( void* pPhone )
{
	AutoLock;
	for (size_t i = 0; i < _vecDDMItems.size(); i++)
	{
		DDMDataItem* item = _vecDDMItems.at(i);
		if(item != NULL && item->pPhone == pPhone)
		{
			return item;
		}
	}
	return NULL;
}

DDMDataItem* ConnectionManager::FindDDMItem( wstring sn )
{
	AutoLock;
	for (size_t i = 0; i < _vecDDMItems.size(); i++)
	{
		DDMDataItem* item = _vecDDMItems.at(i);
		if(item != NULL && CStrOperation::toLowerW(item->key) == CStrOperation::toLowerW(sn))
		{
			return item;
		}
	}
	return NULL;
}

EnumPhoneType ConnectionManager::GetPhoneType( void* pPhone )
{
	DDMDataItem* pItem = this->FindDDMItem(pPhone);

	if(pItem)
		return pItem->type;
	else
		return EnumPhoneType_Android;
}

bool ConnectionManager::Handle( RequestIsRecoveryMode* pMsg )
{
	DDMDataItem* pItem = this->FindDDMItem(pMsg->deviceID);
	if(pItem != NULL)
		pMsg->bRecoverMode = pItem->bIsRecovery;
	else
		pMsg->bRecoverMode = false;
	return true;
}

bool ConnectionManager::Handle( const RequestConnectMsg* pMsg )
{
	DDMDataItem* pItem = this->FindDDMItem(pMsg->pPhone);
	if(pItem == NULL)
		return true;

	if(pMsg->bRetry)
	{
		pItem->nAdbErrorRetry++;
		if(pItem->nAdbErrorRetry < 3)
		{
			this->_pDDMManager->ReConnect(pMsg->pPhone);
		}
		else
		{
			DeviceConnectErrorMsg msg;
			msg.pPhone = pMsg->pPhone;
			msg.nError = DeviceConnectError_Check_ADBError;
			msg.PostToUI();
		}
	}
	else
	{
		pItem->nAdbErrorRetry = 0;
		this->_pDDMManager->ReConnect(pMsg->pPhone);
	}

	return true;
}

bool ConnectionManager::Handle( const RequestActiveDeviceMsg* pMsg )
{
	DDMDataItem* pItem = this->FindDDMItem(pMsg->deviceID);
	if(pItem == NULL)
		return true;

	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(pMsg->deviceID);
	if (!pDevice) 
		return true;

	if(pDevice->GetDevInfo()->_deviceType != Ios )
	{

	}
	else
	{
		int nID = Singleton::GetThreadManager()->GetGUID();
		string threadID = "RequestInstallAppThread-" + CStrOperation::Int2string(nID);
		ActiveDeviceThread* pThread = new ActiveDeviceThread(threadID, nID, pMsg->deviceID, pItem->pPhone);
		pThread->Run();
	}
	return true;
}

void ConnectionManager::NotifyScan()
{
	_pScanThread->ActiveScan();
}