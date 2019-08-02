#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "IosPXLAppOperation.h"
#include "Common/Path.h"
#include "Device/NoDeamonInterface/NoDeamonInterface.h"
#include "Device/ITunesMobileDevice.h"
#include "Module/Application/AppManager.h"

IosPXLAppOperation::IosPXLAppOperation(const DeviceData* pDevice, ProgressSubject& subject)
:IIosAppOperation(pDevice, subject)
{
}

IosPXLAppOperation::~IosPXLAppOperation(void)
{
}

int IosPXLAppOperation::GetAppCount(APP_OWNER appOwner)
{
	if (!_pDevInfo->_ios_bJailBreaked)
		return 0;

	// 判断pxl类型程序文件所在的目录是否存在
	if (!_pFileStream->Ios_Exists(PXL_APP_INFO_FOLDER))
		return 0;

	DirData pxlDir(_pDevData);

	pxlDir._name = PXL_APP_INFO_FOLDER;
	// 获取该目录下的所有文件
	if ( ! _pFileStream->Ios_GetFileInfos(&pxlDir, eFileGetType_Files))
		return 0;

	int nCount = 0;
	deque<NodeData*> childs = pxlDir.GetChildrens();
	for(size_t i = 0; i < childs.size(); i++)
	{
		NodeData* pNode = childs.at(i);

		// 以.plist结尾的文件下载到本地并解析
		if (CStrOperation::endsWithW(pNode->_name, L".plist"))
		{
			nCount++;
		}
	}

	return nCount;
}

bool IosPXLAppOperation::GetAppInfos(vector<AppData*> &vtInfos)
{
	if (!_pDevInfo->_ios_bJailBreaked)
		return false;

    // 文件存放在本地的全路径
    wstring destinationOnComputer = _pDevData->GetPath()->GetLocalAppPath();

    // pxl程序文件所在IOS设备的目录
	wstring PxlOnIosFolder = PXL_APP_INFO_FOLDER;   

	// 判断pxl类型程序文件所在的目录是否存在
	if (!_pFileStream->Ios_Exists(PxlOnIosFolder))
		return false;
	
	RETURN_FALSE_IF(! _pDevData);
	DirData pxlDir(_pDevData);

	pxlDir._name = PxlOnIosFolder;
	// 获取该目录下的所有文件
	if ( ! _pFileStream->Ios_GetFileInfos(&pxlDir, eFileGetType_Files))
		return false;

	bool bRet = false;

	deque<NodeData*> childs = pxlDir.GetChildrens();
	for(size_t i = 0; i < childs.size(); i++)
	{
		NodeData* pNode = childs.at(i);
		wstring strLocalFile = destinationOnComputer + pNode->_name;

		// 以.plist结尾的文件下载到本地并解析
		if (CStrOperation::endsWithW(pNode->_name, L".plist") &&
			_pFileStream->Ios_DownFromPhone(pNode->GetFullPath(), strLocalFile) == Transfer_Success)
		{
			IosAppXmlHelper::PrasePxlAppInfo(strLocalFile, vtInfos, _pFileStream);

            CFileOperation::DeleteFile(strLocalFile);
			bRet = true;
		}
	}

	return bRet;
}

CommandExecuteResult IosPXLAppOperation::InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo)
{
	if (_pDevData && _pDevData->GetIosDeamonMode() == 0)
		return COMMAND_EXECUTE_FAIL;
//     if (!_pDevData)
//         return COMMAND_EXECUTE_FAIL;

	if (!_pDevInfo || !_pProgressSubject || 
		!_pDevInfo->_ios_bJailBreaked || !_pDevInfo->_ios_daemonInstalled)
		return COMMAND_EXECUTE_FAIL;
//     if (!_pDevInfo || !_pProgressSubject || 
//         !_pDevInfo->_ios_bJailBreaked || (!_pDevInfo->_ios_daemonInstalled && _pDevInfo->_connectType == WIFI))
//         return COMMAND_EXECUTE_FAIL;	

	//检测固件是否符合要求
	wstring strIdentifier;
	wstring strUnZipPath;
	if(!exeInfo)
	{
		if ( !CheckInstallPkg(strAppFullName, strIdentifier, strUnZipPath) || strIdentifier.length() <= 0 )
            return COMMAND_EXECUTE_FAIL;
	}
	//卸载
	//UnInstallApp(strIdentifier);

	_pProgressSubject->SetTaskNumberSum(3);
	_pProgressSubject->SetAllTaskSum(300);

	//上传
	_pProgressSubject->SetCurrentTaskSum(100);
	_pProgressSubject->SendPrompt(MESSAGE_INSTALL_UPLOAD);
	bool bUpLoadSuccess;
	if(exeInfo)
		bUpLoadSuccess = UpLoadNormalApp(strAppFullName,exeInfo->OffsetAppInExe,exeInfo->len);
	else
		bUpLoadSuccess = UpLoadNormalApp(strAppFullName);
	CFileOperation::DeleteDirectory(strUnZipPath);
    if (!bUpLoadSuccess)
        return COMMAND_EXECUTE_FAIL;

	_pProgressSubject->SetCurrentTaskSum(100);
	_pProgressSubject->IncreaseTaskNumberComplete();
    _pProgressSubject->SendProgress();

	//安装
	_pProgressSubject->SendPrompt(MESSAGE_INSTALL_INSTALL);
    bool bInstallSuccess = false;
//     if (_pDevInfo->_connectType == USB && _pDevInfo->_ios_bJailBreaked)
//     {
//         _pProgressSubject->AddTaskProgress(100);
//         _pProgressSubject->SendProgress();
// 		int iErr;
// 
// 	EnterCriticalSection(&((DeviceInfo*)_pDevInfo)->_csIOSSessionLock);
//	try{
// 			iErr = NoDeamonInterface::PxlInstall(_pDevInfo->_ios_devptr, PXL_NORMAL_PATH);
//	}
//	catch(...)
//	{
//	}
// 	LeaveCriticalSection(&((DeviceInfo*)_pDevInfo)->_csIOSSessionLock);	
// 
//         errorMsg = CITunesMobileDevice::iTunes_GetDeviceErrorCodeString(iErr);
//         bInstallSuccess = (iErr == 0);
//         _pFileStream->Ios_DeleteFile(PXL_NORMAL_PATH);
//     }
//     else
    {
        bInstallSuccess = InstallAppService(APP_TYPE_PXL);
        _pProgressSubject->AddTaskProgress(100);
        _pProgressSubject->SendProgress();
    }
    if(!bInstallSuccess)
        return COMMAND_EXECUTE_FAIL;

	_pProgressSubject->SetCurrentTaskSum(100);
	_pProgressSubject->IncreaseTaskNumberComplete();
    _pProgressSubject->SendProgress();

	//修复并等待图标生成
	_pProgressSubject->SendPrompt(MESSAGE_INSTALL_NOTIFYSB);
	RepairAppIcon();
	Sleep(1000);
	_pProgressSubject->AddTaskProgress(100);
	_pProgressSubject->IncreaseTaskNumberComplete();
    _pProgressSubject->SendProgress();

	return COMMAND_EXECUTE_SUCCESS;
}

bool IosPXLAppOperation::UpLoadNormalApp(wstring strAppFullName,__int64 AppOffsetInExe,unsigned __int64 AppSize)
{
    wstring pxlPath = CStrOperation::trimRightW(PXL_NORMAL_PATH, L"ndapp.pxl");
    if (!_pFileStream->Ios_Exists(pxlPath))
        _pFileStream->Ios_CreateDirectory(pxlPath);
    if (_pFileStream->Ios_Exists(PXL_NORMAL_PATH))
        _pFileStream->Ios_DeleteFile(PXL_NORMAL_PATH);

	//上传安装包
	UploadFileCommand uploadCmd(_pDevData);
	uploadCmd.setfilename(strAppFullName, PXL_NORMAL_PATH);
	// 添加进度转换
	//AppProgressObserver pObserver((ProgressSubject*)(_pProgressSubject));
    ProgressObserver* pObserver = NULL;
    if (!_pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_pProgressSubject->_observers[0]);
	uploadCmd.Attach(pObserver);
	uploadCmd.SetParam(AppOffsetInExe,AppSize);
	uploadCmd.SetTaskNumberSum(1);
	uploadCmd.SetAllTaskSum(CFileOperation::GetFileSizeW(strAppFullName));
	uploadCmd.Execute();
	return (uploadCmd.GetUploadResult() == Transfer_Success);
}

bool IosPXLAppOperation::UnInstallApp(wstring strIdentifier)
{
	if (_pDevData && _pDevData->GetIosDeamonMode() == 0)
		return false;
//    RETURN_FALSE_IF(!_pDevData);

	if (!_pDevInfo || 
		!_pDevInfo->_ios_bJailBreaked || 
		!_pDevInfo->_ios_daemonInstalled)
		return false;
//     if (!_pDevInfo ||
//         !_pDevInfo->_ios_bJailBreaked || 
//         (!_pDevInfo->_ios_daemonInstalled && _pDevInfo->_connectType == WIFI))
//         return false;

	if (strIdentifier.length() <= 0 || 
		strIdentifier == DAEMON_APP_ID_ONE || 
		strIdentifier == DAEMON_APP_ID_TWO)
		return false;

	bool bRet = false;
	SetCSLock(((DeviceInfo*)_pDevInfo)->_csIOSSessionLock);
//     if (_pDevInfo->_connectType == USB && _pDevInfo->_ios_bJailBreaked)
//         bRet = (NoDeamonInterface::PxlUninstall(_pDevInfo->_ios_devptr, (wchar_t*)strIdentifier.c_str()) == 0);
//     else
        bRet = UnInstallAppService(APP_TYPE_PXL, strIdentifier);

    if (bRet)
    {
        RepairAppIcon();
        Sleep(1000);
        bRet = true;
    }

	return bRet;
}

CommandExecuteResult IosPXLAppOperation::ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, wstring strVersion, eArchiveMode eMode, wstring strFileOnPc)
{
	if (strIdentifier.length() <= 0 ||
		strAppPhonePath.length() <= 0 ||
		strFileOnPc.length() <=0)
	{
		return COMMAND_EXECUTE_FAIL;
	}

	wstring strLocalRootPath = _pDevData->GetPath()->GetLocalAppPath() + CFileOperation::GetGuidW();

	wstring strAppPhonePathName = CFileOperation::GetFileName(strAppPhonePath);
	wstring strPostfightPath = PXL_APP_INFO_FOLDER + strIdentifier + L".Postflight";
	wstring strPreremovePath = PXL_APP_INFO_FOLDER + strIdentifier + L".Preremove";	
	wstring strLocalAppPath = strLocalRootPath + WSTRING_PATH_SEPARATER + CFileOperation::GetFileName(strAppPhonePath);
	wstring strLocalPkgScriptPath = strLocalRootPath + WSTRING_PATH_SEPARATER + L"PkgScript" + WSTRING_PATH_SEPARATER;
    wstring strLocalDocumentsPath = strLocalRootPath + WSTRING_PATH_SEPARATER + L"Documents" + WSTRING_PATH_SEPARATER;
	wstring strLocalInfoPlistPath = strLocalRootPath + WSTRING_PATH_SEPARATER + L"Info.plist";

	CFileOperation::DeleteDirectory(strLocalRootPath);
	//::CreateDirectory(strLocalRootPath.c_str(), NULL);
	CFileOperation::MakeLocalDir(strLocalRootPath);

	_pProgressSubject->ResetProgressData();
	_pProgressSubject->SetTaskNumberSum(3);
	_pProgressSubject->SetAllTaskSum(300);

	//下载数据
	_pProgressSubject->SetCurrentTaskSum(100);
	_pProgressSubject->SendPrompt(MESSAGE_ARCHIVE_DOWNLOAD);
	if (!DownLoadFolder(strAppPhonePath, strLocalAppPath))
		return COMMAND_EXECUTE_FAIL;
	_pProgressSubject->AddTaskProgress(100-_pProgressSubject->GetLastProgress()->_allTaskProgresss);
	_pProgressSubject->SendProgress();

	//::CreateDirectory(strLocalPkgScriptPath.c_str(), NULL);
	CFileOperation::MakeLocalDir(strLocalPkgScriptPath);
    CFileOperation::MakeLocalDir(strLocalDocumentsPath);

	//下载数据
	_pProgressSubject->SetCurrentTaskSum(100);
	_pProgressSubject->SendPrompt(MESSAGE_ARCHIVE_DOWNLOAD);
	DownLoadFolder(strPostfightPath, strLocalPkgScriptPath + L"Postflight");
	_pProgressSubject->AddTaskProgress(100-_pProgressSubject->GetLastProgress()->_allTaskProgresss);
	_pProgressSubject->SendProgress();

	//下载数据
	_pProgressSubject->SetCurrentTaskSum(100);
	_pProgressSubject->SendPrompt(MESSAGE_ARCHIVE_DOWNLOAD);
	DownLoadFolder(strPreremovePath, strLocalPkgScriptPath + L"Preremove");
	_pProgressSubject->AddTaskProgress(100-_pProgressSubject->GetLastProgress()->_allTaskProgresss);
    _pProgressSubject->IncreaseTaskNumberComplete();
	_pProgressSubject->SendProgress();

	//整理数据
	_pProgressSubject->SetCurrentTaskSum(100);	
	_pProgressSubject->SendPrompt(MESSAGE_ARCHIVE_ARRANGE);
	vector<wstring> vtFiles;
	CFileOperation::FindAllFileInDir(strLocalPkgScriptPath, vtFiles);
	if (vtFiles.size() > 0)
		IosAppXmlHelper::CreatePxlPlistFile(strLocalAppPath, strAppName, strIdentifier+L".Postflight", 
		strIdentifier+L".Preremove", strVersion);
	else
		IosAppXmlHelper::CreatePxlPlistFile(strLocalAppPath,strAppName, L"", L"", strVersion);
	_pProgressSubject->AddTaskProgress(100-_pProgressSubject->GetLastProgress()->_currentTaskProgress);
	_pProgressSubject->IncreaseTaskNumberComplete();
    _pProgressSubject->SendProgress();

	//打包数据
	_pProgressSubject->SetCurrentTaskSum(100);	
	_pProgressSubject->SendPrompt(MESSAGE_ARCHIVE_PACKAGE);
	bool ret = CFileOperation::ZipDirectory(strLocalRootPath, strFileOnPc);
	_pProgressSubject->AddTaskProgress(100-_pProgressSubject->GetLastProgress()->_currentTaskProgress);
	_pProgressSubject->IncreaseTaskNumberComplete();
    _pProgressSubject->SendProgress();

	CFileOperation::DeleteDirectory(strLocalRootPath);

    return ret ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
}

bool IosPXLAppOperation::CheckInstallPkg(wstring strAppFullName, wstring &strIdentifier, wstring &strUnZipPath)
{
	strUnZipPath = _pDevData->GetPath()->GetLocalAppPath() + CFileOperation::GetGuidW();
    CFileOperation::UnZipFile(strAppFullName, strUnZipPath, L"PxlPkg.plist", false);

	vector<wstring> vt;
	CFileOperation::GetMatchFileFullName(strUnZipPath, L"PxlPkg.plist", vt);
    RETURN_FALSE_IF(vt.empty());
	
	IosPlistHelper plistHelper(vt.at(0), eXmlType_UTF8);
	wstring strFirmwareRange = IosAppXmlHelper::PxlSearchKeyFromXML(&plistHelper, "RDPxlPackageFireware");
	strIdentifier = IosAppXmlHelper::PxlSearchKeyFromXML(&plistHelper, "CFBundleIdentifier");
	RETURN_TRUE_IF(strFirmwareRange.empty());

	bool bRet = false;
	int nProductVer = IosPublicFunc::GetVerNum(_pDevInfo->_deviceProductVersion);
	vector<wstring> vtFirmware = CStrOperation::parseStringsW(strFirmwareRange, L",");
	for (vector<wstring>::iterator it = vtFirmware.begin(); it != vtFirmware.end(); it++)
	{
		int nNeedVer = IosPublicFunc::GetVerNum(*it);
        if (nProductVer != nNeedVer)
            continue;

        bRet = true;
        break;
	}

    if (!bRet)
        THROW_ASSISTANT_EXCEPTION(PRODUCT_VERSION_NOT_AGREE);

	return bRet;
}
#endif