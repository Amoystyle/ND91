#include "NDMobileCommu.h"

#include "Data/RootData.h"
#include "Data/ContactAccount.h"
#include "Command/application/AppData.h"
#include "Data/FileHelper.h"
#include "Data/FileData.h"
#include "Data/NodeData.h"
#include "Data/PictureData.h"
#include "Data/PictureGroup.h"
#include "Data/IosPicturesImpl3.h"
#include "Data/IosPicturesImpl4.h"
#include "Data/IosPicturesImpl421.h"

#include "Core/DeviceManager.h"
#include "Core/DeviceData.h"
#include "Core/GlobalOption.h"
#include "Core/PictureManager.h"
#include "Core/IosPictureManager.h"

#include "Common/Log.h"
#include "Common/AssistantException.h"
#include "Common/FileOperation.h"
#include "Common/IosPicturePath.h"
#include "DefaultProgressObserver.h"
#include "Device/IosPublicFunc.h"

using namespace ND91Assistant;

#define RETURL_FALSE_CHECKSN(e) DeviceData* pDeviceData = DEVICE_MANAGER->FindDevice(CStrOperation::toLowerW(e));\
	if (!pDeviceData)\
	return /*FALSE*/false;\

#define RETURL_ZERO_CHECKSN(e) DeviceData* pDeviceData = DEVICE_MANAGER->FindDevice(CStrOperation::toLowerW(e));\
	if (!pDeviceData)\
	return 0;\

#define RETURL_ERROR_CHECKSN(e) DeviceData* pDeviceData = DEVICE_MANAGER->FindDevice(CStrOperation::toLowerW(e));\
	if (!pDeviceData)\
	return UNFIND_DEVICE_ERROR;\

//#define ENTER_CS_NDMOBILECOMM //EnterCriticalSection(&g_csNDMobileCommuEven);
//#define LEAVE_CS_NDMOBILECOMM //LeaveCriticalSection(&g_csNDMobileCommuEven);

#pragma region Srv
NDMobileSrvCommu::NDMobileSrvCommu()
{
	OPTION->Set(GlobalOption::STR_LOG_FILE_PATH,CFileOperation::GetCurrWorkingDir() + L"AssistantDllLog.txt");
	LOG->SetLevel(Log::LOG_DEBUG);
}
NDMobileSrvCommu::~NDMobileSrvCommu()
{

}

void ND91Assistant::NDMobileSrvCommu::ReleaseMemory()
{
	LOG_FUNTCION_BEGIN
	DEVICE_MANAGER->Release();
	LOG_FUNTCION_END
}

void ND91Assistant::NDMobileSrvCommu::DestroyInstanceSrvCommu()
{
	LOG_FUNTCION_BEGIN
	delete this;
	LOG_FUNTCION_END
}

/*BOOL*/bool __stdcall ND91Assistant::NDMobileSrvCommu::IosEnvironmentSuc(/*LPWSTR*/wchar_t* pUrl, size_t nSize)
{
	LOG_FUNTCION_BEGIN
	/*BOOL*/bool bRec = /*TRUE*/true;
	//wcscpy_s(pUrl, nSize, L"");
    wcscpy(pUrl, L"");
	bRec = DEVICE_MANAGER->IosInit();
	// 再尝试一次
	if (!bRec)
	{
        LOG->WriteDebug(L"NDMobileSrvCommu::IosEnvironmentSuc::IosInit 失败重试");
#ifdef WIN32
		Sleep(1000);
#else
        sleep(1);
#endif
		bRec = DEVICE_MANAGER->IosInit();
	}
#ifdef MODULE_DRIVER
	// 如果已安装iTunes，则再尝试一次
	if (!bRec && !DRIVER_MANAGER->IsNeedInstalliTurns())
	{
        LOG->WriteDebug(L"NDMobileSrvCommu::IosEnvironmentSuc::IosInit 失败重试2");
#ifdef WIN32
		Sleep(2000);
#else
        sleep(2);
#endif
		bRec = DEVICE_MANAGER->IosInit();
	}
#endif
	if (!bRec)
	{
		wstring strUrl = DEVICE_MANAGER->GetITunesUrl();
		LOG->WriteDebug(L"NDMobileSrvCommu::IosEnvironmentSuc::IosInit 失败！ 取得ItunesUrl : " + strUrl);
		//wcscpy_s(pUrl, nSize, strUrl.c_str());
        wcscpy(pUrl, strUrl.c_str());
		bRec =  /*FALSE*/false;
	}
	LOG->WriteDebug(L"NDMobileSrvCommu::IosEnvironmentSuc::IosInit" + bRec ? L"TRUE" : L"FALSE");
	LOG_FUNTCION_END
	return bRec;
}

#ifdef MODULE_DRIVER
/*BOOL*/bool __stdcall ND91Assistant::NDMobileSrvCommu::IsIosDevice( /*LPCWSTR*/const wchar_t* lpVID, /*LPCWSTR*/const wchar_t* lpPID )
{
    LOG_FUNTCION_BEGIN
    /*BOOL*/bool bRec = DRIVER_MANAGER->IsIosDevice(lpVID, lpPID);	
    wstringstream wss;
    wss << L"VID_" << lpVID << L"&PID_" <<lpPID << L":IsIosDevice = " << bRec;
    LOG->WriteDebug(wss.str());
    LOG_FUNTCION_END
        return bRec;
}

/*BOOL*/bool ND91Assistant::NDMobileSrvCommu::GetDriverInfo( PMOBILE_DEVICE_INFO lpDeviceInfo, /*LPCWSTR*/const wchar_t* lpVID, /*LPCWSTR*/const wchar_t* lpPID )
{
	LOG_FUNTCION_BEGIN
	/*BOOL*/bool bRec = /*FALSE*/false;
	Driver* pDriver = DRIVER_MANAGER->GetDriver(lpVID, lpPID);
	if (pDriver)
	{
		const HardwareInfo* pInfo = pDriver->getHardwareInfo();
		//wcscpy_s(lpDeviceInfo->szHardID, MOBILE_BASE_INFO_LEN, pInfo->_strHardID.c_str());
		//wcscpy_s(lpDeviceInfo->szProviderName, MOBILE_BASE_INFO_LEN, pInfo->_strProviderName.c_str());
		//wcscpy_s(lpDeviceInfo->szDriverDesc, MOBILE_BASE_INFO_LEN, pInfo->_strDriverDesc.c_str());
        wcscpy(lpDeviceInfo->szHardID, pInfo->_strHardID.c_str());
        wcscpy(lpDeviceInfo->szProviderName, pInfo->_strProviderName.c_str());
        wcscpy(lpDeviceInfo->szDriverDesc, pInfo->_strDriverDesc.c_str());

		lpDeviceInfo->_nState = pInfo->_nState;
		// 如果需要安装驱动，直接返回驱动地址
		if (pInfo->_nState != 0)
		{
			wstring strUrl = pDriver->GetDriverUrl();
			//wcscpy_s(lpDeviceInfo->szDriverDesc, MAX_PATH, strUrl.c_str());
            wcscpy(lpDeviceInfo->szDriverDesc, strUrl.c_str());
		}
		bRec = /*TRUE*/true;
	}
	LOG_FUNTCION_END
	return bRec;
}
#endif
NDASSISTANT_DEVICE_STATE __stdcall ND91Assistant::NDMobileSrvCommu::GetDeviceState(/*LPCWSTR*/const wchar_t* lpSn)
{
	LOG_FUNTCION_BEGIN
	NDASSISTANT_DEVICE_STATE nState = NDS_UNCONNECT;
	// 连接
	if (!ConnectDevice(30000, lpSn))
		nState = NDS_UNCONNECT;
	else
	{
		wstring strSn = CStrOperation::toLowerW(lpSn);
		DeviceData *pDeviceData = DEVICE_MANAGER->FindDevice(strSn);

		if (pDeviceData)
		{
			if (pDeviceData->GetDevInfo()->_connectType == USB)
				nState = NDS_NORMAL;
			else if (pDeviceData->GetDevInfo()->_connectType == RECOVERY)
				nState = NDS_RECOVERY;
		}	
	}	
	LOG->WriteDebug( L"GetDeviceState : " + CStrOperation::IntToWString(nState));
	LOG_FUNTCION_END
	return nState;
}

/*UINT*/unsigned int __stdcall ND91Assistant::NDMobileSrvCommu::GetBatteryLever( /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN
	/*UINT*/unsigned int nRec = 0;
	// 连接
	if (!ConnectDevice(30000, lpSn))
	{
		LOG->WriteDebug(L"! NDMobileSrvCommu::GetBatteryLever连接设备失败！");
		nRec = 0;
	}
	else
	{
		wstring strSn = CStrOperation::toLowerW(lpSn);
		DeviceData *pDeviceData = DEVICE_MANAGER->FindDevice(strSn);
		if (pDeviceData)
			nRec = pDeviceData->GetCommands()->GetBatteryLever(); 
	}
	LOG_FUNTCION_END
	return nRec;
}

/*BOOL*/bool ND91Assistant::NDMobileSrvCommu::ConnectDevice(int nWaitTime, /*LPCWSTR*/const wchar_t* lpSn)
{
	wstring strSn = CStrOperation::toLowerW(lpSn);
	int nCount = 0;
	DeviceData* pDeviceData = NULL;
	// 30秒超时
	/*BOOL*/bool bRec = /*FALSE*/false;
	LOG->WriteDebug(L"NDMobileSrvCommu::Srv开始连接....");
	while (nCount < nWaitTime)
	{
		pDeviceData = DEVICE_MANAGER->FindDevice(strSn);
		if (pDeviceData)
		{
			bRec = /*TRUE*/true;
			break;
		}
#ifdef WIN32
		Sleep(100);
#else
        sleep(0.1);
#endif
		nCount += 100;
	}

	if (bRec)
		LOG->WriteDebug(L"NDMobileSrvCommu::完成连接！" + strSn);
	else
		LOG->WriteDebug(L"NDMobileSrvCommu::连接超时！" + strSn);

	return bRec;
}

void ND91Assistant::NDMobileSrvCommu::SetLogPath(/*LPCWSTR*/const wchar_t* lpPath)
{
	OPTION->Set(GlobalOption::STR_LOG_FILE_PATH, lpPath);
	LOG->Restart();
}
#pragma endregion

/*****************************************************************************************************/
/****  Mgr *******************************************************************************************/
/*****************************************************************************************************/

NDMobileMgrCommu::NDMobileMgrCommu()
{
	OPTION->Set(GlobalOption::STR_LOG_FILE_PATH,CFileOperation::GetCurrWorkingDir() + L"AssistantDllLog.txt");
	LOG->SetLevel(Log::LOG_DEBUG);
}
NDMobileMgrCommu::~NDMobileMgrCommu()
{

}

void ND91Assistant::NDMobileMgrCommu::ReleaseMemory()
{
	DEVICE_MANAGER->Release();
}

void ND91Assistant::NDMobileMgrCommu::DestroyInstanceMgrCommu( )
{
	delete this;
}

/*BOOL*/bool __stdcall ND91Assistant::NDMobileMgrCommu::BeginConnect( /*LPCWSTR*/const wchar_t* lpSn )
{
	bool bRec = /*TRUE*/true;
	if (!lpSn || lpSn && wcslen(lpSn) == 0)
		bRec = /*FALSE*/false;
	
	// Ios 环境初始化
	if (bRec)
	{
		LOG->WriteDebug(L"NDMobileMgrCommu::Mgr开始环境初始化....");
		if (!DEVICE_MANAGER->IosInit())
			bRec = /*FALSE*/false;
	}	
	// 死等连接
	if (bRec)
	{
		wstring strSn = CStrOperation::toLowerW(lpSn);
		LOG->WriteDebug(L"NDMobileMgrCommu::完成环境初始化，开始连接:" + strSn);
		int nWaitTime = 30000;
		int nCount = 0;
		while (nCount < nWaitTime)
		{
			if (DEVICE_MANAGER->FindDevice(strSn))
            {
                LOG->WriteDebug(L"NDMobileMgrCommu::完成连接！" + strSn);
				break;
            }
#ifdef WIN32
			Sleep(100);
#else
            sleep(0.1);
#endif
			nCount += 100;
			if (nCount % 1000 == 0)
				LOG->WriteDebug(L"NDMobileMgrCommu::1秒后还没有连接...");
		}
        bRec = nCount < nWaitTime;
	}
	return bRec;
}

/*BOOL*/bool __stdcall ND91Assistant::NDMobileMgrCommu::GetBaseInfo( PMOBILE_BASE_INFO lpMobileInfo, /*LPCWSTR*/const wchar_t* lpSn ) 
{
	LOG_FUNTCION_BEGIN

	RETURL_FALSE_CHECKSN(lpSn);

	lpMobileInfo->uConnType = pDeviceData->GetDevInfo()->_connectType;		  // 连接方式 0 USB数据线 1 WIFI
	lpMobileInfo->uBattery = pDeviceData->GetCommands()->GetBatteryLever();  // 电池电量
	lpMobileInfo->bJailBreaked = pDeviceData->GetDevInfo()->_ios_bJailBreaked;

	SpaceInfo spaceInfo;
	if (pDeviceData->GetRootData()->GetSpaceInfo(RootData::PHONEDATA, &spaceInfo))
	{
		lpMobileInfo->dskDisk.i64Total = spaceInfo._totalsize;
		lpMobileInfo->dskDisk.i64Free = spaceInfo._Available;
	}

// 	wcscpy_s(lpMobileInfo->szCPU, MOBILE_BASE_INFO_LEN, pDeviceData->GetDevInfo()->_ios_deviceCPUArch.c_str());
// 	wcscpy_s(lpMobileInfo->szManu, MOBILE_BASE_INFO_LEN, pDeviceData->GetDevInfo()->_manufacturer.c_str());
// 	wcscpy_s(lpMobileInfo->szModel, MOBILE_BASE_INFO_LEN, pDeviceData->GetDevInfo()->_deviceHWModel.c_str());
// // 	wcscpy_s(lpMobileInfo->szSerial, MOBILE_BASE_INFO_LEN, pDeviceData->GetDevInfo()->_strSerialNumber.c_str());
// 	wcscpy_s(lpMobileInfo->szImei, MOBILE_BASE_INFO_LEN, pDeviceData->GetDevInfo()->_deviceIMEI.c_str());
// 	wcscpy_s(lpMobileInfo->szOSBuild, MOBILE_BASE_INFO_LEN, pDeviceData->GetDevInfo()->_deviceProductVersion.c_str()); 	
// 	wcscpy_s(lpMobileInfo->szDevName, MOBILE_BASE_INFO_LEN, pDeviceData->GetDevInfo()->_ios_deviceName.c_str());
    wcscpy(lpMobileInfo->szCPU, pDeviceData->GetDevInfo()->_ios_deviceCPUArch.c_str());
    wcscpy(lpMobileInfo->szManu, pDeviceData->GetDevInfo()->_manufacturer.c_str());
    wcscpy(lpMobileInfo->szModel, pDeviceData->GetDevInfo()->_deviceHWModel.c_str());
// 	wcscpy(lpMobileInfo->szSerial, pDeviceData->GetDevInfo()->_strSerialNumber.c_str());
    wcscpy(lpMobileInfo->szImei, pDeviceData->GetDevInfo()->_deviceIMEI.c_str());
    wcscpy(lpMobileInfo->szOSBuild, pDeviceData->GetDevInfo()->_deviceProductVersion.c_str()); 	
    wcscpy(lpMobileInfo->szDevName, pDeviceData->GetDevInfo()->_ios_deviceName.c_str());
	lpMobileInfo->uDeviceType = 1;                        // 设备类型: 1 ios 2 android
	if (pDeviceData->GetDevInfo()->_deviceType == Android)
		lpMobileInfo->uDeviceType = 2;

	LOG_FUNTCION_END

	return /*TRUE*/true;
}

/*BOOL*/bool __stdcall ND91Assistant::NDMobileMgrCommu::GetSpaceInfo( PDISK_INFO lpDiskInfo, /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	RETURL_FALSE_CHECKSN(lpSn);

	/*BOOL*/bool bRet = /*FALSE*/false;

	try {
		SpaceInfo spaceInfo;
		if (pDeviceData->GetRootData()->GetSpaceInfo(RootData::PHONEDATA, &spaceInfo))
		{
			lpDiskInfo->i64Total = spaceInfo._totalsize;
			lpDiskInfo->i64Free = spaceInfo._Available;
			bRet = /*TRUE*/true;
		}		
	}
	catch (...){bRet = /*FALSE*/true;}

	LOG_FUNTCION_END

	return bRet;
}

/*UINT*/unsigned int __stdcall ND91Assistant::NDMobileMgrCommu::GetBatteryLever( /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	RETURL_ZERO_CHECKSN(lpSn);

	/*UINT*/unsigned int nResult = 0;
	try {
		nResult = pDeviceData->GetCommands()->GetBatteryLever(); 
	}
	catch (...)
	{
		nResult = 0;
	}

	LOG_FUNTCION_END;

	return nResult;
}

/*INT*/int __stdcall ND91Assistant::NDMobileMgrCommu::GetAppCount( /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	RETURL_ZERO_CHECKSN(lpSn);

	/*INT*/int nResult = 0;

	try {
		nResult = pDeviceData->GetCommands()->GetAppCount(APP_OWNER_USER);
	}
	catch (...)
	{
		nResult = 0;
	}

	LOG_FUNTCION_END

	return nResult;
}

/*INT*/int __stdcall ND91Assistant::NDMobileMgrCommu::GetMusicCount( /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	RETURL_ZERO_CHECKSN(lpSn);

	LOG_FUNTCION_END

	return 0;//pDeviceData->GetCommands()->GetMusicCount();
}

/*INT*/int __stdcall ND91Assistant::NDMobileMgrCommu::GetImageCount(/*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	RETURL_ZERO_CHECKSN(lpSn);

	/*INT*/int nResult = 0;

	try {
		nResult = pDeviceData->GetCommands()->GetPhotoCount();
	}
	catch (...)
	{
		nResult = 0;
	}

	LOG_FUNTCION_END

	return nResult;
}

/*INT*/int __stdcall ND91Assistant::NDMobileMgrCommu::GetVideoCount( /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	RETURL_ZERO_CHECKSN(lpSn);

	LOG_FUNTCION_END

	return 0;
}

/*INT*/int __stdcall ND91Assistant::NDMobileMgrCommu::GeteBookCount(/*LPCWSTR*/const wchar_t* lpAppPath,  /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	RETURL_ZERO_CHECKSN(lpSn);

	int nCount = 0;
	try {
		DirData* pDirInfo = dynamic_cast<DirData*>(FileHelper::GetNode(pDeviceData->GetRootData()->GetDir(), lpAppPath));
		if (!pDirInfo)
			return 0;

		deque<NodeData*> childs = pDirInfo->GetChildrens();
		deque<NodeData*>::iterator it;
		for (it=childs.begin(); it!=childs.end(); it++)
		{
			NodeData* pTempData = (*it);
			if (pTempData && pTempData->_type == ND91Assistant::NodeData::FILE)
				nCount++;
		}
		childs.clear();
	}
	catch (...)
	{
		nCount = 0;
	}

	LOG_FUNTCION_END

	return nCount;
}

/*INT*/int __stdcall ND91Assistant::NDMobileMgrCommu::GetContactCount( /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	RETURL_ZERO_CHECKSN(lpSn);

	int nCount = 0;
	try {
		pDeviceData->GetContactAccounts()->Load();	
		std::deque<BaseData*> datas = pDeviceData->GetContactAccounts()->GetDatas<ContactAccount>();
		for (std::deque<BaseData*>::iterator it = datas.begin(); it != datas.end(); it++)
		{
			ContactAccount* pData = dynamic_cast<ContactAccount*>(*it);
			if (pData)
				nCount += pData->Count();
		}
	}
	catch (...)
	{
		nCount = 0;
	}

	LOG_FUNTCION_END

	return nCount;
}

/*INT*/int __stdcall ND91Assistant::NDMobileMgrCommu::GetMessageCount( /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	RETURL_ZERO_CHECKSN(lpSn);

	/*INT*/int nResult = 0;

	try {
		nResult = pDeviceData->GetCommands()->GetSMSCount();
		//pDeviceData->GetMessageDatas()->Load();
		//nResult = pDeviceData->GetMessageDatas()->Count<MessageData>();
	}
	catch (...)
	{
		nResult = 0;
	}

	LOG_FUNTCION_END

	return nResult;
}

/*BOOL*/bool __stdcall ND91Assistant::NDMobileMgrCommu::SetDevName( /*LPCWSTR*/const wchar_t* lpNewName, /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	RETURL_FALSE_CHECKSN(lpSn);

	/*BOOL*/bool bRet = /*FALSE*/false;

	try {
		bRet = pDeviceData->GetCommands()->SetDevName(lpNewName);
	}
	catch (...)
	{
		bRet = /*FALSE*/false;
	}

	LOG_FUNTCION_END

	return bRet;
}

/*UINT*/unsigned int __stdcall ND91Assistant::NDMobileMgrCommu::InstallApp( /*LPCWSTR*/const wchar_t* lpAppPath ,/*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	RETURL_ERROR_CHECKSN(lpSn);

	bool bRet = false;
	/*UINT*/unsigned int errorCode = UNKNOW_ERROR;
	DefaultProgressObserver observer(RENAME_ALL);
	try{
		bRet = pDeviceData->GetCommands()->InstallApp(lpAppPath, &observer, false, true, true);
	}
	catch(AssistantException e)
	{
		switch (e.GetErrorCode())
		{
		case AssistantException::PRODUCT_VERSION_NOT_AGREE:
			errorCode = PRODUCT_VERSION_NOT_AGREE_ERROR;
			break;
		case AssistantException::IOS_UNINSTALL_APPSYNC:
			errorCode = UNINSTALL_APPSYNC_ERROR;
			break;
		}
	}

	if (bRet)
		errorCode = 0;

	LOG_FUNTCION_END

	return errorCode;
}

/*UINT*/unsigned int __stdcall ND91Assistant::NDMobileMgrCommu::UnInstallApp( /*LPCWSTR*/const wchar_t* lpAppId, /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	RETURL_ERROR_CHECKSN(lpSn);

	std::deque<BaseData*> datas = pDeviceData->GetAppDatas()->GetDatas<AppData>();
	AppData* pAppData = NULL;
	std::deque<BaseData*>::iterator it;
	for (it = datas.begin(); it!=datas.end(); it++)
	{
		AppData* pTemp = dynamic_cast<AppData*>(*it);
		if (pTemp && pTemp->_packageName.compare(lpAppId) == 0)
		{
			pAppData = pTemp;
			break;
		}
	}
	if (!pAppData)
		return UNFIND_APP_ERROR;

	bool bRet = false;
	/*UINT*/unsigned int errorCode = UNKNOW_ERROR;
	try{
		bRet = pDeviceData->GetCommands()->UnInstallApp(pAppData);
	}
	catch(AssistantException e)
	{		
	}

	if (bRet)
		errorCode = 0;

	LOG_FUNTCION_END

	return errorCode;
}

/*BOOL*/bool __stdcall ND91Assistant::NDMobileMgrCommu::GetAllApps( MOBILE_APP_INFO** lpAppInfo, int* lpSize, /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	if (*lpAppInfo)
	{
		delete (*lpAppInfo);
		*lpAppInfo = NULL;
	}
	*lpSize = 0;

	RETURL_FALSE_CHECKSN(lpSn);

	try{
		pDeviceData->GetAppDatas()->Load();
	}
	catch(AssistantException e)
	{		
		LOG_FUNTCION_END

		return FALSE;
	}

	std::deque<BaseData*> datas = pDeviceData->GetAppDatas()->GetDatas<AppData>();
	*lpSize = datas.size();
	if (*lpSize > 0)
	{
		*lpAppInfo = new MOBILE_APP_INFO[*lpSize];
		memset(*lpAppInfo, 0, sizeof(MOBILE_APP_INFO)*(*lpSize));
		for (int i=0; i<(*lpSize); i++)
		{
			AppData* pTemp = dynamic_cast<AppData*>(datas.at(i));
			if (pTemp)
			{
				((*lpAppInfo)[i]).bShare = pTemp->_bShare;

				switch (pTemp->_eAppType)
				{
				case APP_TYPE_PXL:
					((*lpAppInfo)[i]).eAppType = 1;
					break;
				case APP_TYPE_IPA:
					((*lpAppInfo)[i]).eAppType = 2;
					break;
				case APP_TYPE_DEB:
					((*lpAppInfo)[i]).eAppType = 5;
					break;
				case APP_TYPE_INSTALLER:
					((*lpAppInfo)[i]).eAppType = 3;
					break;
				case APP_TYPE_PANDA:
					((*lpAppInfo)[i]).eAppType = 4;
					break;
				}

				((*lpAppInfo)[i]).nFlags = 0;
				if (pTemp->_eAppOwner == APP_OWNER_SYS)
					((*lpAppInfo)[i]).nFlags = 1;

// 				wcscpy_s(((*lpAppInfo)[i]).szAppPhonePath, MAX_PATH, pTemp->_strAppPhonePath.c_str());
// 				wcscpy_s(((*lpAppInfo)[i]).szDescription, URL_MAX_LEN, pTemp->_Description.c_str());
// 				wcscpy_s(((*lpAppInfo)[i]).szIconPhonePath, MAX_PATH, pTemp->_strIconPhonePath.c_str());
// 				wcscpy_s(((*lpAppInfo)[i]).szPurchaser, MOBILE_PACKAGE_NAME_LEN, pTemp->_strPurchaser.c_str());
// 				wcscpy_s(((*lpAppInfo)[i]).szShareFolder, MAX_PATH, L"/Documents");
// 				wcscpy_s(((*lpAppInfo)[i]).szUrl, URL_MAX_LEN, pTemp->_Url.c_str());
// 				wcscpy_s(((*lpAppInfo)[i]).wszAppName, MOBILE_PACKAGE_NAME_LEN, pTemp->_AppName.c_str());
// 				wcscpy_s(((*lpAppInfo)[i]).wszPackageName, MOBILE_PACKAGE_NAME_LEN, pTemp->_packageName.c_str());
// 				wcscpy_s(((*lpAppInfo)[i]).szIosVersion, MOBILE_PACKAGE_NAME_LEN, pTemp->_strVersion.c_str());
// 				wcscpy_s(((*lpAppInfo)[i]).wszVersionName, MOBILE_PACKAGE_NAME_LEN, pTemp->_strVersion.c_str());
                wcscpy(((*lpAppInfo)[i]).szAppPhonePath, pTemp->_strAppPhonePath.c_str());
                wcscpy(((*lpAppInfo)[i]).szDescription, pTemp->_Description.c_str());
                wcscpy(((*lpAppInfo)[i]).szIconPhonePath, pTemp->_strIconPhonePath.c_str());
                wcscpy(((*lpAppInfo)[i]).szPurchaser, pTemp->_strPurchaser.c_str());
                wcscpy(((*lpAppInfo)[i]).szShareFolder, L"/Documents");
                wcscpy(((*lpAppInfo)[i]).szUrl, pTemp->_Url.c_str());
                wcscpy(((*lpAppInfo)[i]).wszAppName, pTemp->_AppName.c_str());
                wcscpy(((*lpAppInfo)[i]).wszPackageName, pTemp->_packageName.c_str());
                wcscpy(((*lpAppInfo)[i]).szIosVersion, pTemp->_strVersion.c_str());
                wcscpy(((*lpAppInfo)[i]).wszVersionName, pTemp->_strVersion.c_str());
			}			
		}
	}	

	LOG_FUNTCION_END

	return TRUE;
}

/*BOOL*/bool __stdcall ND91Assistant::NDMobileMgrCommu::ReleaseAllApps(MOBILE_APP_INFO* lpAppInfo, /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	RETURL_FALSE_CHECKSN(lpSn);

	if (lpAppInfo)
	{
		delete lpAppInfo;
		lpAppInfo = NULL;
	}

	LOG_FUNTCION_END

	return TRUE;
}

/*BOOL*/bool __stdcall ND91Assistant::NDMobileMgrCommu::GetPackageIcon( /*LPCWSTR*/const wchar_t* lpAppId, /*LPCWSTR*/const wchar_t* lpSavePath, /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	RETURL_FALSE_CHECKSN(lpSn);

	std::deque<BaseData*> datas = pDeviceData->GetAppDatas()->GetDatas<AppData>();
	AppData* pAppData = NULL;
	std::deque<BaseData*>::iterator it;
	for (it = datas.begin(); it!=datas.end(); it++)
	{
		AppData* pTemp = dynamic_cast<AppData*>(*it);
		if (pTemp->_packageName.compare(lpAppId) == 0)
			pAppData = pTemp;
	}
	if (!pAppData)
	{
		LOG->WriteDebug(L"GetPackageIcon end！");
		return UNFIND_APP_ERROR;
	}

	/*BOOL*/bool bRet = pAppData->LoadAppIcon(pDeviceData, lpSavePath);

	LOG_FUNTCION_END

	return bRet;
}

/*BOOL*/bool __stdcall ND91Assistant::NDMobileMgrCommu::UploadFile( /*LPCWSTR*/const wchar_t* lpLocalPath, /*LPCWSTR*/const wchar_t* lpDevicePath, /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN;

    // 检查本地文件
    if ( ! CFileOperation::IsFileExist(lpLocalPath) )
    {
        LOG->WriteDebug(L"UploadFile : loca file not found ！");
        return /*FALSE*/false;
    }

	RETURL_FALSE_CHECKSN(lpSn);	

    // 创建手机上的目录
	if (!FileHelper::CreateDir(pDeviceData, lpDevicePath))
	{
        LOG->WriteDebug(L"UploadFile : create directory fail！");
		return /*FALSE*/false;
	}

    // 上传文件
	DefaultProgressObserver observer(RENAME_ALL);
    bool success = FileHelper::Upload(pDeviceData, lpLocalPath, lpDevicePath, &observer);

	LOG_FUNTCION_END

	return success;
}

/*BOOL*/bool __stdcall ND91Assistant::NDMobileMgrCommu::PxlToIpa( /*LPCWSTR*/const wchar_t* lpSrcPxlFullName, /*LPWSTR*/wchar_t* lpDesIpaFullName, int nMaxPath, /*LPCWSTR*/const wchar_t* lpSn)
{
	LOG_FUNTCION_BEGIN

	ZeroMemory(lpDesIpaFullName, nMaxPath);

	RETURL_FALSE_CHECKSN(lpSn);

	if (!CFileOperation::IsFileExist(lpSrcPxlFullName))
	{
		LOG_FUNTCION_END
		return /*FALSE*/false;
	}

	std::wstring strDesIpa = L"";
	try {
		strDesIpa = pDeviceData->GetCommands()->PxlToIpa(lpSrcPxlFullName).c_str();		
	}	
	catch (...)
	{
		LOG_FUNTCION_END

		return /*FALSE*/false;
	}

	if (CFileOperation::IsFileExist(strDesIpa.c_str()))
	{
		//wcscpy_s(lpDesIpaFullName, nMaxPath, strDesIpa.c_str());
        wcscpy(lpDesIpaFullName, nMaxPath, strDesIpa.c_str());

		LOG_FUNTCION_END

		return /*TRUE*/true;
	}

	LOG_FUNTCION_END

	return /*FALSE*/false;
}

/*BOOL*/bool __stdcall ND91Assistant::NDMobileMgrCommu::UploadWallpaper( /*LPCWSTR*/const wchar_t* lpLocalPath, /*LPCWSTR*/const wchar_t* lpSn )
{
	LOG_FUNTCION_BEGIN

	RETURL_ZERO_CHECKSN(lpSn);

    if ( ! pDeviceData->GetDevInfo()->_ios_bJailBreaked )
    {
        PictureManager* pManager = pDeviceData->GetPictureManager();
        RETURN_FALSE_IF( ! pManager->Load() );

        // 未越狱时将图片添加到第一个图库分类中
        vector<PictureGroup*> groups;
        pManager->GetPictureGroups(&groups);
        if ( groups.empty() )   // 目前只有iPad可能有这个情况, 其他至少有个"全部"分类
        {
            pManager->AddPhotoGroup(L"新增图片");
            pManager->GetPictureGroups(&groups);
            RETURN_FALSE_IF ( groups.empty() );
        }
        RETURN_FALSE_IF( ! pManager->AddPictureToGroup(lpLocalPath, groups[0]->GetId()) );
		
		if ( pDeviceData->GetDevInfo()->_deviceType == Ios )
		{
			dynamic_cast<IosPictureManager*>(pManager)->IosSync();
		}

        return true;
    }

    // 下面是越狱机的处理
	int version = IosPublicFunc::GetVerNum(pDeviceData->GetDevInfo()->_deviceProductVersion);

    // 临时文件夹
	if ( ! CFileOperation::IsDirExist(pDeviceData->GetIosPicturePath()->GetLocalWallPaperPath().c_str()))
	{
		RETURN_FALSE_IF (!CFileOperation::MakeLocalDir(pDeviceData->GetIosPicturePath()->GetLocalWallPaperPath()));
	}

    IosPicturesImpl* pIosImpl = NULL;
    wstring deviceFolder;
	if (version < 400 )
	{
		pIosImpl = new IosPicturesImpl3(pDeviceData);
	}
	else if (version >= 400 && version < 421)
	{
		pIosImpl = new IosPicturesImpl4(pDeviceData);
	}
	else
	{
		pIosImpl = new IosPicturesImpl421(pDeviceData);
	}

    // 取得下一个壁纸编号(从300开始)
    int number = 300;

    DirData dir(pDeviceData);
    dir._name = pDeviceData->GetIosPicturePath()->GetIosWallPaperPath();
    deque<NodeData*> childs = dir.GetChildrens();

    if ( ! childs.empty() )
    {
        // 将最后一个文件名的数字加1
        wstring lastFileName = (*childs.rbegin())->_name;
        wstring numberPart = lastFileName.substr(4, 4);
#ifdef WIN32
        int n = _wtoi(numberPart.c_str()) + 1;
#else
        int n = atoi((const char*)numberPart.c_str()) + 1;
#endif

        // 比较
        if ( n > number )
            number = n;
    }

    // 上传
    PictureData* pData = pIosImpl->AddSpecialPictures(lpLocalPath, PICTURE_TYPE_WALLPAPER, number);

    bool success = pData != NULL;
    SAFE_DELETE(pIosImpl);
	SAFE_DELETE(pData);

    LOG_FUNTCION_END
	return success;
}

void NDMobileMgrCommu::SetLogPath(/*LPCWSTR*/const wchar_t* lpPath)
{
	OPTION->Set(GlobalOption::STR_LOG_FILE_PATH, lpPath);
	LOG->Restart(); 
}