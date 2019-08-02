#include "stdafx.h"
#include "IosUsbConnection.h"

#include "Device/ITunesMobileDevice.h"
#include "Device/CFConverter.h"
#include "Device/IosPublicFunc.h"
#include "Device/AFCFileSysemBase.h"
#include "Device/IosDaemon.h"

#include "Common/NDBase64.h"
#include "Common/CSLock.h"
#include "Activator.h"
IosUsbConnection::IosUsbConnection(DeviceInfo* pDevInfo)
{
	_pDeviceInfo       = pDevInfo;
    _sessionStarted    = false;
	_bDeviceLock       = false;
    _mapDeviceInfo.clear();
	InitializeCriticalSection(&_lock);
}

ND91Assistant::IosUsbConnection::~IosUsbConnection()
{
	DeleteCriticalSection(&_lock);
}

void IosUsbConnection::Disconnect()
{
	SetCSLock(_lock);
	if (_pDeviceInfo)
	{
		//越狱则释放AFC服务句柄，否则句柄与_ios_afcHandle及_ios_afcSocket一致
		if (_pDeviceInfo->_ios_bJailBreaked)
		{
			if (_pDeviceInfo->_ios_afcHandleApp)
			{
				CITunesMobileDevice::iTunes_AFCConnectionClose(_pDeviceInfo->_ios_afcHandleApp);
				_pDeviceInfo->_ios_afcHandleApp = NULL;
			}

			if (_pDeviceInfo->_ios_afcSocketApp)
			{
				CITunesMobileDevice::CloseSocket(_pDeviceInfo->_ios_afcSocketApp);
				_pDeviceInfo->_ios_afcSocketApp = NULL;
			}
		}

		if (_pDeviceInfo->_ios_afcHandle)
		{
			CITunesMobileDevice::iTunes_AFCConnectionClose(_pDeviceInfo->_ios_afcHandle);
			_pDeviceInfo->_ios_afcHandle = NULL;
		}/*else  if (_pDeviceInfo->_ios_afc2Handle)
		{
			CITunesMobileDevice::iTunes_AFCConnectionClose(_pDeviceInfo->_ios_afc2Handle);
			_pDeviceInfo->_ios_afc2Handle = NULL;
		} else  if (_pDeviceInfo->_ios_afc1Handle)
		{
			CITunesMobileDevice::iTunes_AFCConnectionClose(_pDeviceInfo->_ios_afc1Handle);
			_pDeviceInfo->_ios_afc1Handle = NULL;
		}*/

		if (_pDeviceInfo->_ios_afcSocket)
		{
			CITunesMobileDevice::CloseSocket(_pDeviceInfo->_ios_afcSocket);
			_pDeviceInfo->_ios_afcSocket = NULL;
		}

       

        if (_pDeviceInfo->_ios_afc1Socket)
        {
            CITunesMobileDevice::CloseSocket(_pDeviceInfo->_ios_afc1Socket);
            _pDeviceInfo->_ios_afc1Socket = NULL;
        }

       

        if (_pDeviceInfo->_ios_afc2Socket)
        {
            CITunesMobileDevice::CloseSocket(_pDeviceInfo->_ios_afc2Socket);
            _pDeviceInfo->_ios_afc2Socket = NULL;
        }

		if (_pDeviceInfo->_ios_devptr)
		{
			CITunesMobileDevice::iTunes_AMDeviceRelease(_pDeviceInfo->_ios_devptr);
            _pDeviceInfo->_ios_devptr = NULL;
		}

		if (_pDeviceInfo->_ios_house_arrest_service_handle)
		{
			CITunesMobileDevice::CloseSocket(_pDeviceInfo->_ios_house_arrest_service_handle);
			_pDeviceInfo->_ios_house_arrest_service_handle = NULL;
		}

		if (_pDeviceInfo->_ios_house_arrest_connect_handle)
		{
			CCFConverter::NDCFRelease(_pDeviceInfo->_ios_house_arrest_connect_handle);
			_pDeviceInfo->_ios_house_arrest_connect_handle = NULL;
		}

        if (_pDeviceInfo->_ios_pInstallationProxySocket)
        {
            CITunesMobileDevice::CloseSocket(_pDeviceInfo->_ios_pInstallationProxySocket);
            _pDeviceInfo->_ios_pInstallationProxySocket = NULL;
        }
	}
}

bool IosUsbConnection::Connect()
{
	SetCSLock(_lock);
	if (!_pDeviceInfo || !_pDeviceInfo->_ios_devptr)
		return false;
LOG->WriteDebug(L"Begin IosUsbConnection Connect ..");
	CITunesMobileDevice::iTunes_AMDeviceRetain(_pDeviceInfo->_ios_devptr);

	if (iTunesCommand(Device_Connect) != 0)
		return false;
	if(iTunesCommand(Device_is_Pair)==0)
	{
		if(iTunesCommand(Device_Validate_Pairing)!=0)
		{
			if (iTunesCommand(Device_Pair) != 0)
			{
				GetDevInfo();
				iTunesCommand(Device_Disconnect);
				return false;
			}
		}
	}
	iTunesCommand(Device_Disconnect);
	LOG->WriteDebug(L"Begin IosUsbConnection OpenConnection ..");
    OpenConnection();

	if (!_pDeviceInfo->_ios_afcSocket || !_pDeviceInfo->_ios_afcHandle)
		return false;
	LOG->WriteDebug(L"Begin IosUsbConnection GetDevInfo ..");
	GetDevInfo();

	int nIosDeamonMode = 0;
	OPTION->Get(GlobalOption::INT_IOS_DEAMON_MODE, nIosDeamonMode);
	LOG->WriteDebug(L"Begin IosUsbConnection GetDevInfo ok!");
	if (_pDeviceInfo->_ios_bJailBreaked && nIosDeamonMode == 1)
		_pDeviceInfo->_ios_daemonInstalled = PhoneDaemonIsStillRunning();	
	LOG->WriteDebug(L"Begin IosUsbConnection connect ok!");
	return true;
}

void IosUsbConnection::OpenConnection()
{
    void* afcSocket = NULL;
    void* afcHandle = NULL;

	void* afc2Socket = NULL;
	void* afc2Handle = NULL;

	_pDeviceInfo->_ios_bJailBreaked = false;
	//先启动afc，安装应用程序时需用afc服务，否则有很大可能无法安装
    string temp = "com.apple.afc";
	if (0 == StartIOSService((char*)temp.c_str(), &afcSocket))
	{
		if (0 == CITunesMobileDevice::iTunes_AFCConnectionOpen(afcSocket, 0, &afcHandle))
		{
			_pDeviceInfo->_ios_afcSocket = afcSocket;
			_pDeviceInfo->_ios_afcHandle = afcHandle;
            _pDeviceInfo->_ios_afc1Socket = afcSocket;
            _pDeviceInfo->_ios_afc1Handle = afcHandle;
			_pDeviceInfo->_ios_bAFCConn = true;
			_pDeviceInfo->_ios_afcSocketApp = afcSocket;
			_pDeviceInfo->_ios_afcHandleApp = afcHandle;
		}
		else
		{
			CITunesMobileDevice::CloseSocket(afcSocket);
			afcSocket = NULL;
		}
	}

	//启动afc2
    temp = "com.apple.afc2";
    int nRec = StartIOSService((char*)temp.c_str(), &afc2Socket);
	if (0 == nRec)
	{
        nRec = CITunesMobileDevice::iTunes_AFCConnectionOpen(afc2Socket, 0, &afc2Handle);
        if (0 != nRec)
        {   // 增加一次重试
            Sleep(500);
            nRec = CITunesMobileDevice::iTunes_AFCConnectionOpen(afc2Socket, 0, &afc2Handle);
        }
		if (0 == nRec)
		{
			_pDeviceInfo->_ios_afcSocket = afc2Socket;
			_pDeviceInfo->_ios_afcHandle = afc2Handle;
            _pDeviceInfo->_ios_afc2Socket = afc2Socket;
            _pDeviceInfo->_ios_afc2Handle = afc2Handle;
			_pDeviceInfo->_ios_bAFCConn = true;
			_pDeviceInfo->_ios_bJailBreaked = true;
		}
		else
		{
			CITunesMobileDevice::CloseSocket(afc2Socket);
			afcSocket = NULL;
		}
	}   	
}

bool IosUsbConnection::GetDiskMemoInfo(IosDiskMemoInfo& diskInfo)
{
	if (!_pDeviceInfo || !_pDeviceInfo->_ios_devptr)
		return false;

	void* pDiskMemoInfo = NULL;
SetCSLock(_pDeviceInfo->_csIOSSessionLock);
	if (iTunesCommand(Device_Connect) == 0)
	{
		if (iTunesCommand(Device_Start_Session) == 0)
		{
			try
			{
				pDiskMemoInfo = CITunesMobileDevice::iTunes_AMDeviceCopyValue(_pDeviceInfo->_ios_devptr, 
					CCFConverter::StringtoCFString("com.apple.disk_usage"), NULL);
			}
			catch (...)
			{
				LOG->WriteError(L"GetDiskMemoInfo was not successful.");
			}
			iTunesCommand(Device_Stop_Session);
		}
		iTunesCommand(Device_Disconnect);
	}

	if (pDiskMemoInfo)
	{
		std::map<string, void*> mapDiskMemoInfo;
		CCFConverter::GetIosData(pDiskMemoInfo, mapDiskMemoInfo);
		std::map<string, void*>::iterator it=mapDiskMemoInfo.begin();
		for (; it!=mapDiskMemoInfo.end(); it++)
		{
			UINT64 nTemp = 0;
			void* pTemp = it->second;
			if (CCFConverter::GetIosData(pTemp, nTemp))
			{
				string strKey = it->first;
				if (strKey.compare("TotalDiskCapacity") == 0)
					diskInfo.nTotalDiskCapacity = nTemp;
				else if(strKey.compare("TotalSystemAvailable") == 0)
					diskInfo.nTotalSystemAvailable = nTemp;
				else if(strKey.compare("TotalDataCapacity") == 0)
					diskInfo.nTotalDataCapacity = nTemp;
				else if(strKey.compare("TotalDataAvailable") == 0)
					diskInfo.nTotalDataAvailable = nTemp;
				else if(strKey.compare("AmountDataReserved") == 0)
					diskInfo.nAmountDataReserved = nTemp;
				else if(strKey.compare("MobileApplicationUsage") == 0)
					diskInfo.nMobileApplicationUsage = nTemp;
				else if(strKey.compare("TotalSystemCapacity") == 0)
					diskInfo.nTotalSystemCapacity = nTemp;
				else if(strKey.compare("AmountDataAvailable") == 0)
					diskInfo.nAmountDataAvailable = nTemp;
			}			
		}		
		CCFConverter::ClearMap(mapDiskMemoInfo);
		return true;
	}

	return false;
}

bool IosUsbConnection::SetDeviceName(wstring strName)
{
	if (!_pDeviceInfo || !_pDeviceInfo->_ios_devptr)
		return false;

	bool bRet = false;
	SetCSLock(_pDeviceInfo->_csIOSSessionLock);
	if (iTunesCommand(Device_Connect) == 0)
	{
		if (iTunesCommand(Device_Start_Session) == 0)
		{
			try
			{
				bRet = (0 == CITunesMobileDevice::iTunes_AMDeviceSetValue(_pDeviceInfo->_ios_devptr, NULL, 
					CCFConverter::StringtoCFString("DeviceName"), 
					CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(strName))));
			}
			catch (...)
			{
				LOG->WriteError(L"SetDeviceName was not successful.");
			}
			iTunesCommand(Device_Stop_Session);
		}
		iTunesCommand(Device_Disconnect);
	}

	return bRet;
}

int IosUsbConnection::GetBetteryState()
{
	if (!_pDeviceInfo || !_pDeviceInfo->_ios_devptr)
		return 0;

	int nBettery = 0;
	void* pBetteryInfo = NULL;
//SetCSLock(_pDeviceInfo->_csIOSSessionLock);
	  EnterCriticalSection(&_pDeviceInfo->_csIOSSessionLock);
	if (iTunesCommand(Device_Connect) == 0)
	{
		if (iTunesCommand(Device_Start_Session) == 0)
		{
			try
			{
				pBetteryInfo = CITunesMobileDevice::iTunes_AMDeviceCopyValue(_pDeviceInfo->_ios_devptr, 
					CCFConverter::StringtoCFString("com.apple.mobile.battery"), NULL);
			}
			catch (...)
			{
				LOG->WriteError(L"GetBetteryState was not successful.");
			}
			iTunesCommand(Device_Stop_Session);
		}
		iTunesCommand(Device_Disconnect);
	}

	LeaveCriticalSection(&_pDeviceInfo->_csIOSSessionLock);
	if (pBetteryInfo)
	{
		std::map<string, void*> mapBetteryInfo;
		CCFConverter::GetIosData(pBetteryInfo, mapBetteryInfo);
		std::map<string, void*>::iterator find;
		find = mapBetteryInfo.find("BatteryCurrentCapacity");
		if (find!=mapBetteryInfo.end())
		{
			if (!CCFConverter::GetIosData(find->second, nBettery))
				nBettery = 0;
		}
		CCFConverter::ClearMap(mapBetteryInfo);
	}

	return nBettery;
}

bool IosUsbConnection::IsProhibitAppInstall()
{
    if (!_pDeviceInfo || !_pDeviceInfo->_ios_devptr)
        return false;

    void* pProhibitAppInstall = NULL;
    bool bProhibitAppInstall = false;
	SetCSLock(_pDeviceInfo->_csIOSSessionLock);
    if (iTunesCommand(Device_Connect) == 0)
    {
        if (iTunesCommand(Device_Start_Session) == 0)
        {
            try
            {
                pProhibitAppInstall = CITunesMobileDevice::iTunes_AMDeviceCopyValue(_pDeviceInfo->_ios_devptr, 
                    CCFConverter::StringtoCFString("com.apple.mobile.restriction"), CCFConverter::StringtoCFString("ProhibitAppInstall"));
            }
            catch (...)
            {
                LOG->WriteError(L"GetProhibitAppInstall was not successful.");
            }
            iTunesCommand(Device_Stop_Session);
        }
        iTunesCommand(Device_Disconnect);
    }

    if (pProhibitAppInstall)
        CCFConverter::GetIosData(pProhibitAppInstall, bProhibitAppInstall);

    return bProhibitAppInstall;//获取不到默认可以安装
}

bool IosUsbConnection::StartSpringBoardService(void** ServiceSocket)
{
	//启动屏幕操作服务，只针对3.1及以上版本的机器
	if (IosPublicFunc::GetVerNum(_pDeviceInfo->_deviceProductVersion) >= 310)
	{
		void* pSpringBoardSocket = NULL;
        string temp = "com.apple.springboardservices";
		return (0 == StartIOSService((char*)temp.c_str(), ServiceSocket));
	}	
	return false;
}

bool IosUsbConnection::StartInstallationProxyService()
{
    string temp = "com.apple.mobile.installation_proxy";
    RETURN_TRUE_IF(NULL != _pDeviceInfo->_ios_pInstallationProxySocket);

    int errorCode = StartIOSService((char*)temp.c_str(), &_pDeviceInfo->_ios_pInstallationProxySocket);
    if (0 != errorCode)
    {
        LOG->WriteError(L"StartInstallationProxyService error:"
            +CCodeOperation::UTF_8ToUnicode(CITunesMobileDevice::iTunes_GetDeviceErrorCodeString(errorCode)));
        Sleep(500);
        errorCode = StartIOSService((char*)temp.c_str(), &_pDeviceInfo->_ios_pInstallationProxySocket);
    }
    return (errorCode == 0);
}

//mobile_installation 服务是支持重用的,因此不需要每次用完后就关闭
//现在只当设备固件比较旧的时候才每次使用完关闭,以免因为设备性能限制
void IosUsbConnection::StopInstallationProxyService()
{
    RETURN_IF(_pDeviceInfo->_ios_deviceProductVersion >= 500);
    StopIOSService(&_pDeviceInfo->_ios_pInstallationProxySocket);
}

#define UNDEFINED_ERROR -402653183   // Error.kAMDUndefinedError
#define INVALID_HOST_ID -402653156   // Error.kAMDInvalidHostIDError

int IosUsbConnection::iTunesCommand(DEVICE_COMMAND_TYPE command)
{
    int nErr = UNDEFINED_ERROR;
    try
    {
        switch(command)
        {
        case Device_Connect:
            nErr = CITunesMobileDevice::iTunes_AMDeviceConnect(_pDeviceInfo->_ios_devptr);
            break;

        case Device_Disconnect:
            nErr = CITunesMobileDevice::iTunes_AMDeviceDisconnect(_pDeviceInfo->_ios_devptr);
            break;

        case Device_Pair:
			{
				_bDeviceLock = false;
				nErr = CITunesMobileDevice::iTunes_AMDevicePair(_pDeviceInfo->_ios_devptr);
				if (nErr == PASSWORD_PROTECTED_CODE) _bDeviceLock = true;
			}
            break;

        case Device_UnPair:
            nErr = CITunesMobileDevice::iTunes_AMDeviceUnpair(_pDeviceInfo->_ios_devptr);
            break;

        case Device_Validate_Pairing:
            nErr = CITunesMobileDevice::iTunes_AMDeviceValidatePairing(_pDeviceInfo->_ios_devptr);
            break;

		case Device_Stop_Session:
            nErr = CITunesMobileDevice::iTunes_AMDeviceStopSession(_pDeviceInfo->_ios_devptr);
            break;

		case Device_Start_Session:
            nErr = CITunesMobileDevice::iTunes_AMDeviceStartSession(_pDeviceInfo->_ios_devptr);
            break;

		case Device_is_Pair:
				nErr = CITunesMobileDevice::iTunes_AMDeviceIsPaired(_pDeviceInfo->_ios_devptr);
				break;
        default:
#ifdef _DEBUG
            throw "增加了定义了命令类型，但此功能未实现";
#endif
            break;
        }
    }
    catch (...)
    {
        wostringstream woss;
        woss << "iTunes device command throw exception, command type = " << command ;
        LOG->WriteError(woss.str());
    }

    if ( nErr )
    {
        wostringstream woss;
        woss << "iTunes device command return error, command type = " << command << ", error no = " << nErr;
        LOG->WriteError(woss.str());
    }
    return nErr;
}

int IosUsbConnection::StartSession()
{
    int nErr = iTunesCommand(Device_Start_Session);

    if ( 0 == nErr )
    {
        _sessionStarted = true;
    }
    else if ( nErr == INVALID_HOST_ID)
    {
        LOG->WriteError(L" got an invalid host error while attaching to the device.  will try to unpair and re-pair.");
        if ( iTunesCommand(Device_UnPair) || iTunesCommand(Device_Pair) )
        {
            LOG->WriteError(L"Re-pairing with the device was not successful.");
        }
        // TODO: 这里重新配对以后，不需要再次尝试 start session 吗 ?
    }

	return nErr;
}

int IosUsbConnection::StopSession()
{
	if (_sessionStarted == false)
		return 0;

    int err = iTunesCommand(Device_Stop_Session);

    if ( 0 == err )
    {
        _sessionStarted = false;
    }

	return err;
}
int IosUsbConnection::EnterRecoveryMode()
{
	int err = UNDEFINED_ERROR;
	SetCSLock(_pDeviceInfo->_csIOSSessionLock);
	if ( err = iTunesCommand(Device_Connect))
		return err;

	if ( err = StartSession())
		return err;

	try
	{
		err = CITunesMobileDevice::iTunes_AMDeviceEnterRecover(_pDeviceInfo->_ios_devptr);
	}
	catch(...)
	{
		LOG->WriteError(L"Exception: AMDeviceStartService");
	}

	StopSession();
	iTunesCommand(Device_Disconnect);
	return err;
}

int IosUsbConnection::StartIOSService(char* service, void **ServiceSocket)
{
	int err = UNDEFINED_ERROR;
	SetCSLock(_pDeviceInfo->_csIOSSessionLock);
    if ( err = iTunesCommand(Device_Connect))
        return err;

    if ( err = StartSession())
        return err;

	try
	{
		err = CITunesMobileDevice::iTunes_AMDeviceStartService(_pDeviceInfo->_ios_devptr,
            CCFConverter::StringtoCFString(service), ServiceSocket, NULL);
	}
	catch(...)
	{
		LOG->WriteError(L"Exception: AMDeviceStartService");
	}

	StopSession();
	iTunesCommand(Device_Disconnect);
	return err;
}

void IosUsbConnection::StopIOSService(void** pSocket)
{
    if (*pSocket == NULL)
        return ;

    CITunesMobileDevice::CloseSocket(*pSocket);
    *pSocket = NULL;
}

bool IosUsbConnection::IsDataClassDisableTetheredSync(string dataClass, bool tryEnableIfDisabled)
{
    bool isDisableSync = false;
	SetCSLock(_pDeviceInfo->_csIOSSessionLock);
    if (iTunesCommand(Device_Connect))
        return isDisableSync;

    if (StartSession())
    {
        iTunesCommand(Device_Disconnect);
        return isDisableSync;
    }

    map<string, string> *tetheredSyncDict = GetDeviceValueAsDict("com.apple.mobile.tethered_sync",dataClass.c_str());
    if (tetheredSyncDict->empty())
    {
        StopSession();
        iTunesCommand(Device_Disconnect);
        return isDisableSync;
    }

    isDisableSync = IsDisableTethered(*tetheredSyncDict);
    if (isDisableSync && tryEnableIfDisabled)
    {
        //*tetheredSyncDict["DisableTethered"] = "false";
        int ret = CITunesMobileDevice::iTunes_AMDeviceSetValue(_pDeviceInfo->_ios_devptr,
            NULL,//CCFConverter::StringtoCFString("com.apple.mobile.tethered_sync"),
            CCFConverter::StringtoCFString(dataClass), 
            CCFConverter::DictionarytoCFDictionary(*tetheredSyncDict));
        if (ret == 0)
        {
            tetheredSyncDict = GetDeviceValueAsDict("com.apple.mobile.tethered_sync",dataClass.c_str());
            if (!tetheredSyncDict->empty())
                isDisableSync = IsDisableTethered(*tetheredSyncDict);
        }
    }

    //书签,再判断下是否开启iCloud,开启则表示不让同步.
    if (!isDisableSync && dataClass == "Bookmarks")
        isDisableSync = IsDataSyncContainiCloudSources(dataClass);

    StopSession();
    iTunesCommand(Device_Disconnect);
    return isDisableSync;
}

bool IosUsbConnection::IsDataSyncContainiCloudSources(string dataClass)
{
    map<string, string> *dataSyncDic = GetDeviceValueAsDict("com.apple.mobile.data_sync",dataClass.c_str());
    if (!dataSyncDic->empty())
    {
        map<string, string>::iterator iter = dataSyncDic->find("Sources");
        if (iter != dataSyncDic->end())
        {
            string source = iter->second;
            if (!source.empty())
                return true;
        }
    }

    return false;
}

bool IosUsbConnection::IsDataClassEnableiCloud(string dataClass)
{
    bool result = false;
	SetCSLock(_pDeviceInfo->_csIOSSessionLock);
    if (iTunesCommand(Device_Connect))
        return result;

    if (StartSession() == 0)
    {
        result = IsDataSyncContainiCloudSources(dataClass);
        StopSession();
    }

    iTunesCommand(Device_Disconnect);
    return result;
}

bool IosUsbConnection::IsDisableTethered(map<string, string> mapDataClass)
{
    map<string, string>::iterator iter = mapDataClass.find("DisableTethered");
    if (iter != mapDataClass.end())
    {
        string disableTethered = iter->second;
        if (disableTethered == "1" || CStrOperation::toLower(disableTethered) == "true")
            return true;
    }

    return false;
}

map<string, string>* IosUsbConnection::GetDeviceValueAsDict(const char* pDomain, const char* pKey)
{
    CCFConverter::GetIosData(_pDeviceInfo->_ios_devptr, pDomain, pKey, _mapDeviceInfo);
    return &_mapDeviceInfo;
}

wstring IosUsbConnection::GetDeviceValueAsString(const char* pDomain, const char* pKey)
{
    string s = "";
    CCFConverter::GetIosData(_pDeviceInfo->_ios_devptr, pDomain, pKey, s);
	return CCodeOperation::GB2312ToUnicode(s);
}

INT64 IosUsbConnection::GetDeivceValueAsInt64(const char* pDomain, const char* pKey)
{
    UINT64 i = 0;
    CCFConverter::GetIosData(_pDeviceInfo->_ios_devptr, pDomain, pKey, i);
    return i;
}

wstring IosUsbConnection::GetDeivceValueAsBase64( const char* pDomainKey, const char* pKey )
{
    wstring ret;
	vector<char> buf;
	CCFConverter::GetIosData(_pDeviceInfo->_ios_devptr, pDomainKey, pKey, buf);
    if (buf.empty())
        return ret;

    int newLen = CNDBase64::Base64encode_len(buf.size());
    char* pBase64 = new char[newLen];
    memset(pBase64, 0, newLen);

    CNDBase64::Base64encode(pBase64, (const char*)&buf[0], buf.size());
	ret = CCodeOperation::StringToWstring(pBase64);

    delete[] pBase64;
    return ret;
}

wstring IosUsbConnection::GetDeviceIdentifier()
{
    string ret = "";
	try
	{
		void* outRef = CITunesMobileDevice::iTunes_AMDeviceCopyDeviceIdentifier(_pDeviceInfo->_ios_devptr);
		if (outRef)
		{
			if ( ! CCFConverter::GetIosData(outRef, ret) )
                ret = "";

			CCFConverter::NDCFRelease(outRef);
		}
	}
	catch (...)
	{
		LOG->WriteError(L"Exception: GetDeviceIdentifier");
	}	
	return CCodeOperation::GB2312ToUnicode(ret);
}

void IosUsbConnection::GetDevInfo()
{
	SetCSLock(_pDeviceInfo->_csIOSSessionLock);
    if (iTunesCommand(Device_Connect))
        return;

    if (StartSession())
        return;

	_pDeviceInfo->_deviceName               = GetDeviceValueAsString(NULL, "DeviceName");
    _pDeviceInfo->_ios_activationState      = GetDeviceValueAsString(NULL, "ActivationState");
    _pDeviceInfo->_ios_deviceActivated      = _pDeviceInfo->_ios_activationState != L"Unactivated";
    _pDeviceInfo->_strSerialNumber 	        = GetDeviceValueAsString(NULL, "SerialNumber");
    _pDeviceInfo->_ios_deviceIdentifier	    = GetDeviceIdentifier   ();
	_pDeviceInfo->_deviceHWModel		    = GetDeviceValueAsString(NULL, "HardwareModel");
    _pDeviceInfo->_deviceProductType		= GetDeviceValueAsString(NULL, "ProductType");
	if ( (int)_pDeviceInfo->_deviceProductType.find( L"com.apple.mobile.wireless_lockdown-supportswifisyncing" ) > 0 )
		_pDeviceInfo->_deviceProductType	= GetDeviceTypeByHardwareModel(_pDeviceInfo->_deviceHWModel);	
    _pDeviceInfo->_ios_deviceClass		    = GetDeviceValueAsString(NULL, "DeviceClass");
    _pDeviceInfo->_ios_deviceFWVersion		= GetDeviceValueAsString(NULL, "FirmwareVersion");
    _pDeviceInfo->_deviceProductVersion     = GetDeviceValueAsString(NULL, "ProductVersion");
    _pDeviceInfo->_ios_deviceProductVersion = IosPublicFunc::GetVerNum(_pDeviceInfo->_deviceProductVersion);
    _pDeviceInfo->_ios_deviceBuildVersion   = GetDeviceValueAsString(NULL, "BuildVersion");
    _pDeviceInfo->_deviceIMEI		        = GetDeviceValueAsString(NULL, "InternationalMobileEquipmentIdentity");
    _pDeviceInfo->_deviceIMSI				= GetDeviceValueAsString(NULL, "InternationalMobileSubscriberIdentity");
    _pDeviceInfo->_ios_deviceICCID		    = GetDeviceValueAsString(NULL, "IntegratedCircuitCardIdentity");
    _pDeviceInfo->_ios_devicePhoneNumber	= GetDeviceValueAsString(NULL, "PhoneNumber");
    _pDeviceInfo->_ios_deviceBTMac		    = GetDeviceValueAsString(NULL, "BluetoothAddress");
    _pDeviceInfo->_deviceWiFiMac		    = GetDeviceValueAsString(NULL, "WiFiAddress");
    _pDeviceInfo->_ios_deviceECID		    = GetDeivceValueAsInt64 (NULL, "UniqueChipID");
    _pDeviceInfo->_ios_deviceCapablity		= GetDeivceValueAsInt64 ("com.apple.disk_usage", "TotalDataCapacity");
    _pDeviceInfo->_ios_deviceModelNumber	= GetDeviceValueAsString(NULL, "ModelNumber");
    _pDeviceInfo->_ios_deviceRegionInfo	    = GetDeviceValueAsString(NULL, "RegionInfo");
    _pDeviceInfo->_deviceBaseVersion		= GetDeviceValueAsString(NULL, "BasebandVersion");
    _pDeviceInfo->_ios_deviceBBSerialNumber = GetDeivceValueAsBase64(NULL, "BasebandSerialNumber");
	_pDeviceInfo->_ios_deviceBBNonce		= GetDeivceValueAsBase64(NULL, "BasebandNonce");
	_pDeviceInfo->_ios_deviceBBGoldCerID	= GetDeivceValueAsInt64(NULL, "CertID");
    if (_pDeviceInfo->_ios_deviceBBGoldCerID == 0)
		_pDeviceInfo->_ios_deviceBBGoldCerID= GetDeivceValueAsInt64 (NULL, "BasebandGoldCertId");
    _pDeviceInfo->_deviceCPUArch			= GetDeviceValueAsString(NULL, "CPUArchitecture");
    _pDeviceInfo->_ios_deviceColor		    = GetDeviceValueAsString(NULL, "DeviceColor");
    wstring timeZone                        = GetDeviceValueAsString(NULL, "TimeZone");
    if (!timeZone.empty())
        _pDeviceInfo->_ios_timeZone = timeZone;
    INT64 timeZoneOff                       = GetDeivceValueAsInt64(NULL, "TimeZoneOffsetFromUTC");
    if (timeZoneOff != 0)
        _pDeviceInfo->_ios_timeZoneOffsetFromUTC = CDealTime::ToDATE(timeZoneOff);

    //kCTSIMSupportSIMStatusNotReady 有卡,开飞行模式是这个值
    //todo判定是否 kCTSIMSupportSIMStatusNotInserted
    _pDeviceInfo->_ios_simStatus = GetDeviceValueAsString(NULL, "SIMStatus");
    if (!_pDeviceInfo->_ios_simStatus.empty() && _pDeviceInfo->_ios_simStatus != L"kCTSIMSupportSIMStatusNotInserted")
        _pDeviceInfo->_ios_isSimInserted = true;

    StopSession();
    iTunesCommand(Device_Disconnect);
}

wstring ND91Assistant::IosUsbConnection::GetDeviceTypeByHardwareModel( wstring strHardware )
{
	wstring strDeviceType = L"";
	strHardware = CStrOperation::toLowerW(strHardware);

	//iphone1
	if (strHardware == L"m68ap")
		strDeviceType = L"iPhone1,1";
	//iphone2g
	else if (strHardware == L"n82ap")
		strDeviceType = L"iPhone1,2";
	//iphone3gs
	else if (strHardware == L"n88ap")
		strDeviceType = L"iPhone2,1";
	//iphone4
	else if (strHardware == L"n90ap")
		strDeviceType = L"iPhone3,1";
	else if (strHardware == L"n92ap")
		strDeviceType = L"iPhone3,3";
	//iphone4s
	else if (strHardware == L"n94ap")
		strDeviceType = L"iPhone4,1";
	//iPod touch 1g
	else if (strHardware == L"n45ap")
		strDeviceType = L"iPod1,1";
	//iPod touch 2g
	else if (strHardware == L"n72ap")
		strDeviceType = L"iPod2,1";
	//iPod touch 3g
	else if (strHardware == L"n18ap")
		strDeviceType = L"iPod3,1";
	//iPod touch 4g
	else if (strHardware == L"n81ap")
		strDeviceType = L"iPod4,1";
	//iPad1
	else if (strHardware == L"k48ap")
		strDeviceType = L"iPad1,1";
	//iPad2
	else if (strHardware == L"k93ap")
		strDeviceType = L"iPad2,1";
	else if (strHardware == L"k94ap")
		strDeviceType = L"iPad2,2";
	else if (strHardware == L"k95ap")
		strDeviceType = L"iPad2,3";
	else if (strHardware == L"k93aap")
		strDeviceType = L"iPad2,4";		//wifi r2 版本
	//iPad3
	else if (strHardware == L"j1ap")
		strDeviceType = L"iPad3,1";
	else if (strHardware == L"j2ap")
		strDeviceType = L"iPad3,2";
	else if (strHardware == L"j1aap")
		strDeviceType = L"iPad3,3";
	//AppleTV
	else if (strHardware == L"k66ap")
		strDeviceType = L"AppleTV2,1";
	else if (strHardware == L"j33ap")
		strDeviceType = L"AppleTV3,1";
	//iso6
	else if (strHardware == L"n41ap")
		strDeviceType = L"iPhone5,1";
	else if (strHardware == L"n42ap")
		strDeviceType = L"iPhone5,2";
	else if (strHardware == L"n78ap")
		strDeviceType = L"iPod5,1";
	else if (strHardware == L"p101ap")
		strDeviceType = L"iPad3,4";
	else if (strHardware == L"p105ap")
		strDeviceType = L"iPad2,5";
	else
		strDeviceType = L"";

	return strDeviceType;
}


bool IosUsbConnection::PhoneDaemonIsStillRunning()
{
	AFCFileSysemBase fileStream(_pDeviceInfo);
	IosDaemon daemon(&fileStream, _pDeviceInfo->_ios_deviceIdentifier);
	bool bRet = daemon.PhoneDaemonIsStillRunning();
	_pDeviceInfo->_ios_devicebTurnCypher = daemon.GetIsTrunCypher();
	_pDeviceInfo->_ios_deviceszDaemonCypherBytes = daemon.GetDaemonCypherBytes();
	_pDeviceInfo->_daemonVer = daemon.GetDaemonVer();

	return bRet;
}

const DeviceInfo* ND91Assistant::IosUsbConnection::GetDeviceInfo()
{
    return _pDeviceInfo;
}

bool IosUsbConnection::Connect2AppSharingDirectory(wstring strIdentifier)
{
	if (IosPublicFunc::GetVerNum(_pDeviceInfo->_deviceProductVersion) < 300)
		return false;

	if (!_pDeviceInfo || _pDeviceInfo->_connectType != USB || strIdentifier.length() <= 0)
		return false;

	if (_pDeviceInfo->_ios_house_arrest_service_handle)
	{
		CITunesMobileDevice::CloseSocket(_pDeviceInfo->_ios_house_arrest_service_handle);
		_pDeviceInfo->_ios_house_arrest_service_handle = NULL;
	}

	if (_pDeviceInfo->_ios_house_arrest_connect_handle)
	{
		CCFConverter::NDCFRelease(_pDeviceInfo->_ios_house_arrest_connect_handle);
		_pDeviceInfo->_ios_house_arrest_connect_handle = NULL;
	}

	void* serviceSocket = NULL;
	void* hrHandle = NULL;

	bool bRet = false;
    string temp = "com.apple.mobile.house_arrest";
	if (0 == StartIOSService((char*)temp.c_str(), &serviceSocket))
	{
		string strIdentifierTemp = CCodeOperation::WstringToString(strIdentifier);
		map<string, string> mapMsg;
		mapMsg.insert(pair<string, string>("Command", "VendContainer"));
		mapMsg.insert(pair<string, string>("Identifier", strIdentifierTemp));
		if (SendMessageToService(serviceSocket, mapMsg) && 
			ReadMssageFromService(serviceSocket, mapMsg))
		{
			map<string, string>::iterator it = mapMsg.find("Status");
			if (it != mapMsg.end() && it->second == "Complete" &&
				0 == CITunesMobileDevice::iTunes_AFCConnectionOpen(serviceSocket, 0, &hrHandle))
			{
				_pDeviceInfo->_ios_house_arrest_service_handle = serviceSocket;
				_pDeviceInfo->_ios_house_arrest_connect_handle = hrHandle;
				bRet = true;
			}
		}		
	}

	return bRet;
}

bool IosUsbConnection::SendMessageToService(void* pSocket, map<string, string> mapMsg)
{
	if (pSocket == NULL || mapMsg.size() <= 0)
		return false;

	void* pMsgDict = CCFConverter::DictionarytoCFDictionary(mapMsg);
	if (!pMsgDict)
		return false;

	bool bRet = IosPublicFunc::SendMessageToService(pSocket, pMsgDict);

	CCFConverter::NDCFRelease(pMsgDict);

	return bRet;
}

bool IosUsbConnection::SendMessageToService(void* pSocket, vector<string> vecMsg)
{
    if (pSocket == NULL || vecMsg.size() <= 0)
        return false;

    void* pMsgVec = CCFConverter::ConvertStringVector(vecMsg);
    if (!pMsgVec)
        return false;

    bool bRet = IosPublicFunc::SendMessageToService(pSocket, pMsgVec);
    CCFConverter::NDCFRelease(pMsgVec);

    return bRet;
}

bool IosUsbConnection::SendMessageToService(void* pSocket, vector<void*> vecMsg)
{
    if (pSocket == NULL || vecMsg.size() <= 0)
        return false;

    void* pMsgVec = CCFConverter::ArrayToCFArray(vecMsg);
    if (!pMsgVec)
        return false;

    bool bRet = IosPublicFunc::SendMessageToService(pSocket, pMsgVec);
    CCFConverter::NDCFRelease(pMsgVec);

    return bRet;
}

bool IosUsbConnection::ReadMssageFromService(void* pSocket, map<std::string, std::string> &mapMsg)
{
	mapMsg.clear();
	if (pSocket == NULL)
		return false;

	bool bRet = false;
	void* pReplay = IosPublicFunc::ReadMssageFromService(pSocket);
	if (pReplay)
	{
		bRet = CCFConverter::GetIosData(pReplay, mapMsg);
		CCFConverter::NDCFRelease(pReplay);
	}

	return bRet;
}

bool IosUsbConnection::ReadMssageFromService(void* pSocket, vector<string> &vecMsg)
{
    vecMsg.clear();
    if (pSocket == NULL)
        return false;

    bool bRet = false;
    void* pReplay = IosPublicFunc::ReadMssageFromService(pSocket);
    if (pReplay)
    {
        bRet = CCFConverter::GetIosData(pReplay, vecMsg);
        CCFConverter::NDCFRelease(pReplay);
    }

    return bRet;
}

bool IosUsbConnection::ReadMssageFromService(void* pSocket, vector<void*> &vecMsg)
{
	vecMsg.clear();
	if (pSocket == NULL)
		return false;

	bool bRet = false;
	void* pReplay = IosPublicFunc::ReadMssageFromService(pSocket);
	if (pReplay)
	{
		bRet = CCFConverter::GetIosData(pReplay, vecMsg);
		CCFConverter::NDCFRelease(pReplay);
	}

	return bRet;
}


bool IosUsbConnection::ReadMssageFromService(void* pSocket, vector<string> &vecMsg, map< string, map<string,void*> > &mapEntities)
{
    if (pSocket == NULL)
        return false;

    bool bRet = false;
    void* pReplay = IosPublicFunc::ReadMssageFromService(pSocket);
    if (pReplay)
    {
        bRet = CCFConverter::GetIosEntitiesData(pReplay, vecMsg, mapEntities, _pDeviceInfo->_ios_devptr);
        CCFConverter::NDCFRelease(pReplay);
    }

    return bRet;
}

bool IosUsbConnection::ReadMssageFromService( void* pSocket, vector<string> &vecMsg, map<string, string>& mapIds, map<string,string> &mapEntities )
{
	if (pSocket == NULL)
		return false;

	bool bRet = false;
	void* pReplay = IosPublicFunc::ReadMssageFromService(pSocket);
	if (pReplay)
	{
		bRet = CCFConverter::GetIosEntitiesData(pReplay, vecMsg, mapIds, mapEntities);
		CCFConverter::NDCFRelease(pReplay);
	}

	return bRet;
}

bool IosUsbConnection::ReadMssageFromService(void* pSocket, vector<string> &vecMsg, map<string, string>& mapIds)
{
    if (pSocket == NULL)
        return false;

    bool bRet = false;
    void* pReplay = IosPublicFunc::ReadMssageFromService(pSocket);
    if (pReplay)
    {
        bRet = CCFConverter::GetIosEntitiesData(pReplay, vecMsg, mapIds);
        CCFConverter::NDCFRelease(pReplay);
    }

    return bRet;
}

const char* RELAY_TYPE[] = 
{
    "AppleSupport",
    "Network",
    "VPN",
    "WiFi",
    "UserDatabases",
    "CrashReporter",
    "tmp",
    "SystemConfiguration"
};

int MyCloseSocket(void* afcSocket)
{
#ifdef WIN32
		return closesocket((SOCKET)afcSocket);
#else
		//return close((int)&afcSocket);
#endif	
}

int MySocketRecv(void* afcSocket, char* buf, int len, int flags)
{
#ifdef WIN32
		return recv((SOCKET)afcSocket, buf, len, flags);
#else
		//return recvfrom((int)&afcSocket, (void*)buf, len, flags, NULL, NULL);
#endif	
}

bool ND91Assistant::IosUsbConnection::FileRelay( RelayType eType, wstring strSavePath )
{
    // 启动服务
    void* afcSocket = NULL;
    string temp = "com.apple.mobile.file_relay";
    RETURN_FALSE_IF(StartIOSService((char*)temp.c_str(), &afcSocket));
    RETURN_FALSE_IF ( ! afcSocket );

    // 生成参数
    vector<string> list;
    list.push_back(RELAY_TYPE[eType]);
    map< string, vector<string> > dict;
    dict["Sources"] = list;

    // 转换参数格式
    void* pMsgDict = CCFConverter::ConvertStringVectorMap(dict);
    RETURN_FALSE_IF (!pMsgDict);

    // 发送请求
    bool bRet = IosPublicFunc::SendMessageToService(afcSocket, pMsgDict);
    CCFConverter::NDCFRelease(pMsgDict);
    RETURN_FALSE_IF( ! bRet );

    // 读取返回结果
    map<string, string> returnData;
    if ( ! ReadMssageFromService(afcSocket, returnData))
    { 	
        MyCloseSocket(afcSocket);
        return false;
    }

    // 检查结果
    if( returnData.find("Error") != returnData.end() ||
        returnData.find("Status") == returnData.end()) 
    {
        MyCloseSocket(afcSocket);
        return false;
    }

    // 接收返回数据, 写入到文件
    char* buf = new char[4096];
    int iRecv = 0;
    ofstream ofs(CCodeOperation::WstringToString(strSavePath).c_str(), ios_base::out |ios::binary);
    while ( (iRecv = MySocketRecv(afcSocket, buf, 4096, 0)) > 0)
    {
        ofs.write(buf, iRecv);
    }
    ofs.close();

    // 释放资源, 返回
    delete[] buf;
    MyCloseSocket(afcSocket);
    return true;
}

bool ND91Assistant::IosUsbConnection::ReadMssageFromService( void* pSocket, vector<string> &vecMsg, wstring path)
{
	if (pSocket == NULL)
		return false;

	bool bRet = false;
	void* pReplay = IosPublicFunc::ReadMssageFromService(pSocket);
	if (pReplay)
	{
		bRet = CCFConverter::GetIosScreenSnapEntitiesData(pReplay, vecMsg, path);
		CCFConverter::NDCFRelease(pReplay);
	}

	return bRet;
}

bool ND91Assistant::IosUsbConnection::SendMessageToService( void* pSocket, map<string, void*> mapMsg )
{
	if (pSocket == NULL || mapMsg.size() <= 0)
		return false;

	void* pMsgDict = CCFConverter::DictionarytoCFDictionary(mapMsg);
	if (!pMsgDict)
		return false;

	bool bRet = IosPublicFunc::SendMessageToService(pSocket, pMsgDict);

	CCFConverter::NDCFRelease(pMsgDict);

	return bRet;
}
int ND91Assistant::IosUsbConnection::IsJailBreak()
{
	//启动afc2服务能启动说明现在越狱了
	void* afc2Socket = NULL;
	void* afc2Handle = NULL;

	string  temp = "com.apple.afc2";
	int nRec = StartIOSService((char*)temp.c_str(), &afc2Socket);
	if (0 == nRec)
	{
		nRec = CITunesMobileDevice::iTunes_AFCConnectionOpen(afc2Socket, 0, &afc2Handle);
		if (0 != nRec)
		{   // 增加一次重试
			Sleep(500);
			nRec = CITunesMobileDevice::iTunes_AFCConnectionOpen(afc2Socket, 0, &afc2Handle);
		}
		if(0 != nRec)
			CITunesMobileDevice::CloseSocket(afc2Socket);
		else
		{
			CITunesMobileDevice::CloseSocket(_pDeviceInfo->_ios_afcSocket);
			_pDeviceInfo->_ios_afcSocket = afc2Socket;
			_pDeviceInfo->_ios_afcHandle = afc2Handle;
			_pDeviceInfo->_ios_bAFCConn = true;
			_pDeviceInfo->_ios_bJailBreaked = true;
		}

	}   
	return nRec;
}
bool ND91Assistant::IosUsbConnection::RebootWithDiagnosticsRelay()
{
	void* ServiceSocket=NULL;
	string temp = "com.apple.mobile.diagnostics_relay";
	if(0 != StartIOSService((char*)temp.c_str(), &ServiceSocket))
	{
		temp = "com.apple.iosdiagnostics.relay";
		if(0 != StartIOSService((char*)temp.c_str(), &ServiceSocket))
		{
			return false;
		}
	}
	map<string, string> mapMsg;
	mapMsg.insert(pair<string, string>("Request", "Restart"));
	return SendMessageToService(ServiceSocket, mapMsg);
}

bool ND91Assistant::IosUsbConnection::ShutDownWithDiagnosticsRelay()
{
	void* ServiceSocket=NULL;
	string temp = "com.apple.mobile.diagnostics_relay";
	if(0 != StartIOSService((char*)temp.c_str(), &ServiceSocket))
	{
		temp = "com.apple.iosdiagnostics.relay";
		if(0 != StartIOSService((char*)temp.c_str(), &ServiceSocket))
		{
			return false;
		}
	}
	map<string, string> mapMsg;
	mapMsg.insert(pair<string, string>("Request", "Shutdown"));
	return SendMessageToService(ServiceSocket, mapMsg);
}

bool ND91Assistant::IosUsbConnection::Activate()
{
 	if(_pDeviceInfo->_ios_deviceActivated)
 		return true;

	bool ret=false;
		CActivator Activator(_pDeviceInfo);
		ret = Activator.Activate();	
	return ret;
}