#include "stdafx.h"

#include "Core/DeviceManager.h"
#include "Module/System/SystemManager.h"
#include "Module/System/CheckNeedConnectConfirmCommand.h"

#include "Device/ITunesService.h"
#include "Device/IosWiFiMobileDevice.h"
#include "Device/NDCrEnLib.h"
#include "Device/AndroidAdbHelper.h"
#include "Device/AdbGetDeviceCommand.h"
#include "Device/IosWifiConnection.h"
#include "Device/NoDeamonInterface/NoDeamonInterface.h"
#include "Device/CFConverter.h"

#include "Common/BRMath.h"
#include "Common/Path.h"
#include "Monitor/IosjailBreakMonitorThread.h"
#pragma comment(lib,"winmm.lib")
// ��ʼ���������ľ�̬˽��ȫ�ֱ���
DeviceManager* DeviceManager::_pInstance = NULL;

DeviceManager::DeviceManager()
{
	_bIosInitSuccess = false;
	_IosjailBreakMonitorThread = NULL;
	_AdbScanTimer = 0;
	 InitializeCriticalSection(&_csADBSSessionLock);
	 InitializeCriticalSection(&_csIosInitLock);
	 
}

DeviceManager::~DeviceManager()
{
    CITunesService::UnRegisterIosDeviceCallback();
    RemoveAllDevice();
	ReleaseAllDisconnectDevice();
	RemoveAllPasswordLockIosDevice();
// commented by qiuchang 2011.9.16 : cause to test_dll crash when conenct android device
//     CITunesMobileDevice::MyFreeLibrary();
    CIosWiFiMobileDevice::MyFreeLibrary();
    CNDCrEnLib::MyFreeLibrary();
    NoDeamonInterface::Uninitialize();

    //ֹͣADB����
    AndroidAdbHelper aah;
    aah.stopServer();

	wostringstream woss;
	woss << CCodeOperation::StringToWstring("����DeviceManager��� ��") << DeviceManager::_pInstance;
	LOG->WriteDebug(woss.str());

	LOG->Release();
	OPTION->Release();

	SAFE_DELETE(_IosjailBreakMonitorThread);
	DeleteCriticalSection(&_csADBSSessionLock);
	DeleteCriticalSection(&_csIosInitLock);
}

void ND91Assistant::DeviceManager::Release()
{	
	wostringstream woss;
	woss << CCodeOperation::StringToWstring("��ʼ����DeviceManager ��") << DeviceManager::_pInstance;
	LOG->WriteDebug(woss.str());
//#ifndef ASSISTANT_LITE
    CFileOperation::DeleteDirectory(Path::GlobalTemp());

	// 	DRIVER_MANAGER->Release();
	
	//ֹͣAdb 
	//#endif
	AndroidAdbHelper aah;
	aah.stopServer();
	//SAFE_DELETE(DeviceManager::_pInstance);

}

void ND91Assistant::DeviceManager::StopAdb()
{
    AndroidAdbHelper aah;

    aah.stopServer();
}

bool ND91Assistant::DeviceManager::RestartADB()
{

		AndroidAdbHelper aah;

	if(!aah.stopServer())
	{
		return false;
	}
	return aah.startServer();
}

bool ND91Assistant::DeviceManager::isAdbWork()
{
	AndroidAdbHelper aah;
	return aah.isAdbWorking();
}

DeviceManager* DeviceManager::GetInstance()
{
    if (_pInstance)
        return _pInstance;

    _pInstance = new DeviceManager();
    _pInstance->Init();

	wostringstream woss;
	woss << CCodeOperation::StringToWstring("����DeviceManager ��") << DeviceManager::_pInstance;
	LOG->WriteAlert(woss.str());

    return _pInstance;
}

void DeviceManager::Init()
{
    LOG->WriteAlert(L"========  assistant.DLL initialize  ========");
#ifdef WIN32	

    // ����winsock
    WSADATA wsaData;
    if ( NO_ERROR != WSAStartup( MAKEWORD(2,2), &wsaData ))
    {
        wostringstream woss;
        woss << "WSAStartup fail, error code = " << WSAGetLastError();
        LOG->WriteAlert(woss.str());
        THROW_ASSISTANT_EXCEPTION(ADB_SOCKET_ERROR);
    }
#endif    
}


// ios��ʼ��������������ע��ص�����
bool DeviceManager::IosInit()
{
	if(_bIosInitSuccess)
		return true;

	bool bSuccess = _bIosInitSuccess;
	try
	{
		SetCSLock(_csIosInitLock);

		if(_bIosInitSuccess)
			return true;

		wstring path;
		OPTION->Get(GlobalOption::STR_DLLPATH, path);
		if ( !path.empty() )
		{
			SetDllDirectory(path.c_str());	
			LOG_DEBUG(path);
		}

#ifndef ASSISTANT_LITE
		CIosWiFiMobileDevice::MyLoadLibrary();
		CNDCrEnLib::MyLoadLibrary();
#endif

		if (CITunesService::InitIosEnviroment())
		{
#ifdef WIN32
			RETURN_FALSE_IF(!CITunesMobileDevice::MyLoadLibrary());
			RETURN_FALSE_IF(!NoDeamonInterface::Initialize());
			RETURN_FALSE_IF(!CCFConverter::LoadLib());
			bSuccess = CITunesService::RegisterIosDeviceCallback();
#endif
		}
		_IosjailBreakMonitorThread = new IosjailBreakMonitorThread();
		_IosjailBreakMonitorThread->StartThread();
	}
	catch (AssistantException e)
	{
		bSuccess = false;
	}
	catch (...)
	{
		bSuccess = false;
	}

	_bIosInitSuccess = bSuccess;
	return bSuccess;
}

void DeviceManager::RemoveDevice(std::wstring id)
{
    DEVICE_MAP::iterator it = _devices.find(id);

    if ( it != _devices.end() )
    {
		//it->second->Disconnect();

		// �ͷ�DeviceData����
		//delete it->second; 
		InsertDisconnectDevices(it->second);

        _devices.erase(it);
    }
}

void DeviceManager::OnLinkout(wstring id)
{
	SetCSLock(_csADBSSessionLock);

	RemoveDevice(id);

	UsbDeviceDisconnect(id);
}

bool DeviceManager::ScanUsbDevice()
{
	LOG->WriteDebug(L"DeviceManager::ScanUsbDevice Begin");

	//��ֹƵ������adbɨ��
	DWORD nowtime = timeGetTime(); 
	if(nowtime -_AdbScanTimer < 1500)
	{		
		LOG->WriteDebug(L"DeviceManager::ScanUsbDevice nowtime -_AdbScanTimer");
		return true;
	}

	_AdbScanTimer = nowtime;
	SetCSLock(_csADBSSessionLock);

    // ȷ��ADB.exe����
    std::wstring adbFilePath;
    if ( ! OPTION->Get(GlobalOption::STR_ADB_FILE_APTH, adbFilePath) || _waccess(adbFilePath.c_str(), 0) )
    {
        wostringstream woss;
        woss << "adb file path option not found or file not exist: value = " << adbFilePath;
		LOG->WriteAlert(woss.str());
		LOG->WriteDebug(L"DeviceManager::ScanUsbDevice THROW_ASSISTANT_EXCEPTION");
		THROW_ASSISTANT_EXCEPTION(ADB_EXE_NOT_FOUND);
    }

    bool bHasNewDevice = false;

     // ɨ��Android�豸
	string cmdretdata;
	const std::vector<std::wstring>* pDevices;
	 const map<wstring, wstring>* snMap;
	 AdbGetDeviceCommand getDeviceCmd("host:nddevices");
	 AdbGetDeviceCommand getDeviceCmd1("host:devices");
	try
	{
		
		getDeviceCmd.Execute();
		cmdretdata = getDeviceCmd.GetReturnData();
		int retrycount=0;
		while(cmdretdata.find("error:") != string::npos)
		{
			getDeviceCmd.Execute();
			cmdretdata = getDeviceCmd.GetReturnData();
			LOG->WriteDebug(CCodeOperation::s2ws(cmdretdata));
			retrycount++;
			if(retrycount>5)
			{
				LOG->WriteDebug(L"DeviceManager::ScanUsbDevice if(retrycount>5)");
				return false;
			}
		}
		pDevices = getDeviceCmd.GetSnList();
		snMap = getDeviceCmd.GetSnMap();
	}
	catch(AssistantException e)
	{
		//if(e.GetErrorCode() == AssistantException::ADB_COMMAND_NO_NDDEVICE)
		{
			
			getDeviceCmd1.Execute();
			cmdretdata = getDeviceCmd1.GetReturnData();
			int retrycount=0;
			while(cmdretdata.find("error:") != string::npos)
			{
				getDeviceCmd1.Execute();
				cmdretdata = getDeviceCmd1.GetReturnData();
				retrycount++;
				if(retrycount>5)
					cmdretdata = "error";
			}	
			pDevices = getDeviceCmd1.GetSnList();
			 snMap = getDeviceCmd1.GetSnMap();
		}		
	}
    
	
	
 	std::vector<std::wstring>::const_iterator it;
 	for( it = pDevices->begin(); it != pDevices->end(); ++it)
 	{
        wstring id = *it;
		map<wstring, wstring>::const_iterator snit = snMap->find(id);
		if(snit != snMap->end())
		{
			wstring state = snit->second;
			if(CStrOperation::CompareUnCase(state, L"offline") == 0)
			{
				RemoveDevice(id);
				UsbDeviceOffLine(id);
				LOG->WriteDebug(L"DeviceManager::ScanUsbDevice CStrOperation::CompareUnCase(state, offline)" + id);
				continue;
			}
		}

        if ( FindDevice(id) ) // �豸�Ѿ�����
		{
			LOG->WriteDebug(L"DeviceManager::ScanUsbDevice if ( FindDevice(id)) // �豸�Ѿ�����" + id);
            continue;            
        } 

        bHasNewDevice = true;

        DeviceInfo di;
        di._deviceType = Android;
        di._connectType = USB;
        di._strSerialNumber = id;
        di._strHost = L"127.0.0.1";
        di._nPort = 0;//Ҫ���ӵ�PC�˿ڣ���Ϊ0���ʾ��Activateʱ�ٷ���

        // �����µ��豸
        DeviceData* pDeviceData = AddDevice(id, &di);
		//����Ѿ����ڵ��豸�Ͳ�������
		if(pDeviceData)
		{
			bool bConnected = false;            
			try 
			{
				bConnected = pDeviceData->Connect();
			}
			catch (...)
			{
			}
			if (!bConnected)
			{
				RemoveDevice(id);
				LOG->WriteDebug(L"DeviceManager::ScanUsbDevice if (!bConnected) RemoveDevice" + id);
				continue;
			}
			LOG->WriteDebug(L"Begin UsbDeviceConnect" + id);			
			UsbDeviceConnect(id); 
			LOG->WriteDebug(L"End UsbDeviceConnect" + id);
		}
 	}

	// �����ѶϿ����豸
	bool bRemove = true; 
	while (bRemove)
	{
		bRemove = false;
		for (DEVICE_MAP::const_iterator itBefore = _devices.begin(); itBefore != _devices.end(); itBefore++)
		{
			if (itBefore->second->_DeviceInfo._deviceType != Android
				|| itBefore->second->_DeviceInfo._connectType != USB)
			{
				LOG->WriteDebug(L"DeviceManager::ScanUsbDevice itBefore->second->_DeviceInfo._deviceType != Android");
				continue;
			}
			bool bSign = false;
			for( it = pDevices->begin(); it != pDevices->end(); ++it)
			{
				if (itBefore->first == *it)
				{
					bSign = true;
					break;
				}
			}
			if (!bSign)
			{
				LOG->WriteDebug(L"DeviceManager::ScanUsbDevice bSign= true");
				itBefore->second->Disconnect();				
				wstring id = itBefore->second->_DeviceInfo._strSerialNumber;

				bool bOtherStatDEV=false;

				for(size_t i=0;i<pDevices->size();i++)
				{
					if(pDevices->at(i) == id)
					{
						bOtherStatDEV =true;
						break;
					}
				}	
				//����������״̬�豸�����ͶϿ�
				if(!bOtherStatDEV)
				{				
					// �Ƴ����ѶϿ����豸
					RemoveDevice(itBefore->second->_DeviceInfo._strSerialNumber);
					// ֪ͨ�����豸�ѶϿ�
					UsbDeviceDisconnect(id);
				}
				
				
				bRemove = true;
				break;
			}
			LOG->WriteDebug(L"DeviceManager::ScanUsbDevice bSign= false");
		}
	}

    // ���������豸
	if ( !snMap || snMap->empty() )
		return bHasNewDevice;
	
    for (map<wstring, wstring>::const_iterator it = snMap->begin(); it != snMap->end(); it++)
    {
        wstring id = it->first;
        wstring state = it->second;

		//�����ж��豸�Ƿ��device���offline
        if (_androidDeviceMap.find(id) != _androidDeviceMap.end()
			&& _androidDeviceMap[id] == it->second )
            continue;
        Sleep(200);
		//ֻ�������ŵı�offline���豸
        if (CStrOperation::CompareUnCase(state, L"offline") == 0 && FindDevice(id))// && !FindDevice(id))
        { // ��⵽δ�����豸, �������м�ֵ�����ﲻ�˷�����Ϣ
            UsbDeviceOffLine(id);
        }
        else if (CStrOperation::CompareUnCase(state, L"recovery") == 0)
        {
            RecoveryConnect(id);
        }
    }

    _androidDeviceMap.clear();
    for (map<wstring, wstring>::const_iterator it = snMap->begin(); it != snMap->end(); it++)
    {
        _androidDeviceMap[it->first] = it->second;
    }

	LOG->WriteDebug(L"DeviceManager::ScanUsbDevice End");
	return bHasNewDevice;
}

DeviceData* DeviceManager::FindDevice(std::wstring id) const
{
	 DeviceData* pResult = NULL;

	if (_devices.find(id)!=_devices.end())
	{
		DEVICE_MAP::const_iterator iter = _devices.find(id);
		if (iter!=_devices.end())
		{
			pResult = iter->second;
		}
	}
	else if (_devices.find(CStrOperation::toLowerW(id))!=_devices.end())
	{
		DEVICE_MAP::const_iterator iter = _devices.find(CStrOperation::toLowerW(id));
		if (iter!=_devices.end())
		{
			pResult = iter->second;
		}
	}

	return pResult;
   /* for (DEVICE_MAP::const_iterator it = _devices.begin(); it != _devices.end(); it++)
    {
        if (CStrOperation::CompareUnCase(id, it->first) == 0)
            return it->second;
    }
    return NULL;*/
}

DeviceData* DeviceManager::FindIosDevice(void* pDevice) const
{
    for(DEVICE_MAP::const_iterator it = _devices.begin(); it != _devices.end(); ++it )
    {
        if ( it->second->GetDevInfo()->_ios_devptr == pDevice )
            return it->second;
    }
    return NULL;
}

DeviceData* DeviceManager::FindIosDevice(wstring strIP) const
{
	for(DEVICE_MAP::const_iterator it = _devices.begin(); it != _devices.end(); ++it )
	{
		if ( it->second->GetDevInfo()->_ios_deviceIP == strIP )
			return it->second;
	}

	return NULL;
}

std::vector<std::wstring> DeviceManager::GetAllDevice() const
{
    std::vector<std::wstring> ids;

    for(DEVICE_MAP::const_iterator it = _devices.begin(); it != _devices.end(); ++it )
        ids.push_back(it->first);

    return ids;
}

std::string DeviceManager::GetIosDevice() const
{
	std::string devices;

	for(DEVICE_MAP::const_iterator it = _devices.begin(); it != _devices.end(); ++it )
	{
		if(it->second != NULL
           && it->second->GetDevInfo()->_deviceType == Ios)
		{
			devices += CCodeOperation::UnicodeToUTF_8(it->second->GetDevInfo()->_ios_deviceIdentifier);
			devices +="\tdevice\n\r";
		}
	}	

	return devices;
}


// ɾ�������豸����
void DeviceManager::RemoveAllDevice()
{
    for(DEVICE_MAP::iterator it = _devices.begin(); it != _devices.end(); ++it )
	{
		it->second->Disconnect();
        delete it->second;
	}

    _devices.clear();
}

void DeviceManager::RemoveAllIosUSBDevice()
{
	for(DEVICE_MAP::iterator it = _devices.begin(); it != _devices.end();)
	{
		if ( it->second->GetDevInfo()->_deviceType == Ios && 
			(it->second->GetDevInfo()->_connectType == USB ||
			it->second->GetDevInfo()->_connectType == RECOVERY))
		{
			wstring id = it->second->GetId();

			it->second->Disconnect();

			// �Ȳ��ͷţ�����ʧЧ�豸
			//delete it->second; // �ͷ�DeviceData����
			InsertDisconnectDevices(it->second);
			//it = _devices.erase(it);
			_devices.erase(it++);

			// �㲥��Ϣ
			DEVICE_MANAGER->UsbDeviceDisconnect(id);

			continue;
		}		 
		++it;
	}
}

DeviceData* DeviceManager::AddDevice(const std::wstring id, const DeviceInfo* pInfo)
{
    if ( _devices.find(id) != _devices.end() )
    {
        return NULL;    // ID�Ѵ���
    }

    DeviceData* pDevice = new DeviceData(id, pInfo);

    _devices[id] = pDevice;

    return pDevice;
}

bool ND91Assistant::DeviceManager::Wifi_Connect(wstring strPreIP, wstring strCheckCode)
{
	BRMath brMath(strCheckCode);

	wstring strIP;
	if(strCheckCode.length()<=4)
		strIP= strPreIP + CCodeOperation::StringToWstring(brMath._strIP);
	else
		strIP= CCodeOperation::StringToWstring(brMath._strIP);
	if (brMath._nType == Android)
		return Android_Wifi_Connect(strIP, strCheckCode);
	else if (brMath._nType == Ios)
    {
		return Ios_WiFi_Connect(strIP);
    }
	return false;
}

bool ND91Assistant::DeviceManager::Android_Wifi_Connect( wstring strHost)
{
	return Android_Wifi_Connect(strHost, L"");
}

bool ND91Assistant::DeviceManager::Android_Wifi_Connect( wstring strHost, wstring strCheckCode )
{
    int nDeamonPort;
    OPTION->Get(GlobalOption::INT_DEAMON_PORT, nDeamonPort);


    DeviceInfo di;
    di._deviceType = Android;
    di._connectType = WIFI;
    di._strHost = strHost;
    di._nPort = nDeamonPort;

    wstring id;
//  wstringstream ss;
//  ss << strHost << ":" << nDeamonPort; 
//  ss >> id;

	if (strCheckCode.length() > 0)
		id = strCheckCode;
	else
		id = strHost;

    // �����µ��豸
    DeviceData* pDeviceData = AddDevice(id, &di);
	if (!pDeviceData)
		pDeviceData = FindDevice(id);
	if (!pDeviceData)
		return false;
	try 
	{
		pDeviceData->Connect();
		pDeviceData->Activate();
	}
	catch (AssistantException e)
	{ // ����ʧ��
		RemoveDevice(id);
		return false;
	}

    WifiDeviceConnect(id);
 
    return true;
}

bool ND91Assistant::DeviceManager::Ios_WiFi_Connect(wstring strIP, wstring strPSW, int nConfig)
{
	if (FindIosDevice(strIP) != NULL)
		return true;

    if (nConfig == -1)
        nConfig = CIosWiFiMobileDevice::WiFiConnectConfig(strIP, strPSW, 5000);

	IosWifiConnection conn(strIP, strPSW);
    conn.SetConfirmConfig(nConfig);
	if (conn.Connect())
	{        
		const DeviceInfo* pInfo = conn.GetDeviceInfo();

		// ���ӵ��豸�б�: ios�豸��UUID��ΪID
		wstring id = CStrOperation::toLowerW(pInfo->_ios_deviceIdentifier);
		DeviceData* pNewDevice = AddDevice(id, pInfo);        

		//ͨ���ػ���ȡIOS�豸���ͣ���ʱ��д���⣬����ɸ�����Ҫ����λ�ã������Ҫ1���ִ��ʱ��
		if (pNewDevice)
		{
            conn.SetTickCallBack(DeviceManager::WifiTickCallBack, pNewDevice);
			wstring strDevType = pNewDevice->GetSystemManager()->GetIosDeviceType();
			pNewDevice->GetMutableDevInfo()->_deviceProductType = strDevType;
			//pNewDevice->Activate();
		}

		// �㲥��Ϣ
		if (pNewDevice)
			WifiDeviceConnect(pNewDevice->GetId());

		return true;
	}
	else
		conn.Disconnect();

	return false;
}

void ND91Assistant::DeviceManager::OnDeviceDisconnect( wstring id )
{	
	DeviceData* pDeviceData = FindDevice(id);
	if (!pDeviceData)
		return;		
	
	RemoveDevice(id);
	pDeviceData->OnDeviceDisconnect();

	if (USB == pDeviceData->GetDevInfo()->_connectType)
	{
		DEVICE_MANAGER->UsbDeviceDisconnect(id);
	}
	else if (WIFI == pDeviceData->GetDevInfo()->_connectType)
	{
		DEVICE_MANAGER->WifiDeviceDisconnect(id);
	}
}

void ND91Assistant::DeviceManager::ReleaseDisconnectDevice( wstring id )
{
	if (id.length() == 0)
		return;
	for (vector<DeviceData*>::iterator it = _disConnectDevices.begin(); it != _disConnectDevices.end(); it ++)
	{
		if ((*it)->GetId() == id)
		{
			SAFE_DELETE(*it);
			_disConnectDevices.erase(it);
			break;
		}
	}
}

void ND91Assistant::DeviceManager::ReleaseAllDisconnectDevice( )
{
	for (vector<DeviceData*>::iterator it = _disConnectDevices.begin(); it != _disConnectDevices.end(); it ++)
	{
		SAFE_DELETE(*it);
	}
	_disConnectDevices.clear();
}

std::wstring ND91Assistant::DeviceManager::GetITunesUrl()
{
	//return HttpDriver::GetITunesUrl();
    return L"";
}

void ND91Assistant::DeviceManager::InsertDisconnectDevices( DeviceData* pDeviceData )
{
	_disConnectDevices.push_back(pDeviceData);
}

RootData* ND91Assistant::DeviceManager::GetRootData( const DeviceData* pDeviceData )
{
    for (DEVICE_MAP::const_iterator it = _devices.begin(); it != _devices.end(); it++)
    {
        DeviceData* pDevice = (DeviceData*)(it->second);
        if (pDevice == pDeviceData)
            return pDevice->GetRootData();
    }
    return NULL;
}

//�������δ���������IOS�豸
void ND91Assistant::DeviceManager::AddPasswordLockIosDevice(void* pConnHandle)
{
	for (vector<void*>::iterator it=_iosPasswordLockDevices.begin(); it!=_iosPasswordLockDevices.end(); it++)
	{
		if (pConnHandle == (*it))
			return ;
	}

	_iosPasswordLockDevices.push_back(pConnHandle);
}
 void ND91Assistant::DeviceManager::PassWordLockDeviceConnect(wstring devicename)
 {	 
		 DEVICE_MANAGER->IosPasswordLockDeviceConnect(devicename);	
 }
//����������������δ���������IOS�豸
void ND91Assistant::DeviceManager::ReConnectAllPasswordLockIosDevice()
{
	vector<void*> vtTemp = _iosPasswordLockDevices;
	_iosPasswordLockDevices.clear();

	for (vector<void*>::iterator it=vtTemp.begin(); it!=vtTemp.end(); it++)
	{
		if (!(*it)) continue;

		CITunesService::ReConnPasswordLockDevice(*it);
	}	
}

//ɾ����������δ���������IOS�豸
void ND91Assistant::DeviceManager::RemoveAllPasswordLockIosDevice()
{
	for (vector<void*>::iterator it=_iosPasswordLockDevices.begin(); it!=_iosPasswordLockDevices.end(); it++)
	{
		if (!(*it)) continue;

		CITunesMobileDevice::iTunes_AMDeviceRelease(*it);
		*it = NULL;
	}
	_iosPasswordLockDevices.clear();
}

void ND91Assistant::DeviceManager::RemovePasswordLockIosDevice(void* pConnHandle, bool bReleaseConn)
{
	for (vector<void*>::iterator it=_iosPasswordLockDevices.begin(); it!=_iosPasswordLockDevices.end(); it++)
	{
		if (pConnHandle == (*it))
		{
			if (bReleaseConn && (*it)) 
				CITunesMobileDevice::iTunes_AMDeviceRelease(*it);
			*it = NULL;
			_iosPasswordLockDevices.erase(it);
			break;
		}
	}
}

int ND91Assistant::DeviceManager::GetDeviceCount()
{
    return _devices.size();    
}

int ND91Assistant::DeviceManager::GetDeviceCount(ND91Assistant::DEVICE_OS_TYPE nType)
{
    int nCount = 0;
    for(DEVICE_MAP::const_iterator it = _devices.begin(); it != _devices.end(); ++it )
    {
        if (it->second->GetDevInfo()->_deviceType == nType)
            nCount ++;
    }
    return nCount;
}

void ND91Assistant::DeviceManager::WifiTickCallBack( int nNotify, void* pVoid )
{    
    for (map<wstring, DeviceData*>::iterator it = DEVICE_MANAGER->_devices.begin(); it != DEVICE_MANAGER->_devices.end(); it++)
    {
        if ((void*)it->second == pVoid)
        {
            DeviceData* pDevice = (DeviceData*)pVoid;
            wstring id = pDevice->GetId();
            DEVICE_MANAGER->WifiTickNotify(nNotify, id);
        }
    }
}

int ND91Assistant::DeviceManager::Wifi_IsNeedConnectConfirm( wstring strPreIP, wstring strCheckCode )
{
    int nDeamonPort;
    OPTION->Get(GlobalOption::INT_DEAMON_PORT, nDeamonPort);
    BRMath brMath(strCheckCode);
    wstring strIP;
	if(strCheckCode.length()>4)
		strIP = CCodeOperation::StringToWstring(brMath._strIP);
	else
		strIP = strPreIP + CCodeOperation::StringToWstring(brMath._strIP);
    if (brMath._nType == Android)    
    {
        return (int)CheckNeedConnectConfirmCommand::IsNeedConnectConfirm(strIP, nDeamonPort);
    }
    else if (brMath._nType == Ios)
    {
        return CIosWiFiMobileDevice::WiFiConnectConfig(strIP, Wifi_Connect_Psw, 5000);
    }
    return false;
}

int ND91Assistant::DeviceManager::GetSuitPlateForm(DEVICE_OS_TYPE eType)
{
	int nDevice = 0;
	vector<wstring> idList = DEVICE_MANAGER->GetAllDevice();
	int size = idList.size();
	if(size > 0)
	{
		for (int i = 0; i < size; i++)
		{
			wstring sn = idList.at(i);
			DeviceData* pDevice = DEVICE_MANAGER->FindDevice(sn);
			if (NULL == pDevice)
			{
				continue;
			}

			if(pDevice->GetDevInfo()->_deviceType != eType)
				continue;

			nDevice++;
		}
	}

	return nDevice;
}