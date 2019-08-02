#include "pch_module.h"

#ifdef MODULE_APP
#include "Module/Application/ApkData.h"
#include "Module/Application/NpkData.h"
#include "Module/Application/AppManager.h"
#include "Module/Application/GetAppListCommand.h"
#include "Module/Application/InstallCommand.h"
#include "Module/Application/UninstallCommand.h"
#include "Module/Application/UninstallSystemAppCommand.h"
#include "Module/Application/ArchiveAppCommand.h"

#include "Device/IosDaemonHelper.h"
#include "Device/AdbPMListCommand.h"
#include "Device/NoDeamonInterface/NoDeamonInterface.h"
#include "Module/System/KillProcessCommand.h"
#include "Module/Application/IosApp/PxlToIpaHelper.h"
#include "Common/Path.h"
#include "Module/Application/GetAppInfoCommand.h"
#include "Module/File/GetFileInfoCommand.h"
#include "Common/plist/PlistOperation.h"
#include  "Module/Common/AndroidXMLParser/AndroidXMLOperation.h"
#include "Module/Application/IosApp/IosDebAppOperation.h"
#include "Module/Common/IosPList/IosPlistConverter.h"
#include "Common/Path.h"
#include "Core/DeviceData.h"
#include "Core/DeviceManager.h"
#include "Device/AccountAuthorize/Authorizor.h"
#include "Module/Application/IosApp/IosIPAAppOperation.h"

#define RENAME_FILE(oldname,filename) MoveFile(oldname.c_str(),filename.c_str());while(!CFileOperation::IsFileExist(filename.c_str())){Sleep(50);}

AppManager::AppManager(DeviceData* pDevice)
{
    InitializeCriticalSection(&_cs);
    _bLoading = false;
    _pDevice = pDevice;
}

AppManager::~AppManager(void)
{
    DeleteCriticalSection(&_cs);
    ReleaseData();
    ReleaseGetDatas();
}

bool ND91Assistant::AppManager::GetAppDiskUsage()
{
    RETURN_FALSE_IF(NULL == _pDevice || _pDevice->GetDevInfo()->_deviceType == Android);

    map< wstring, vector<int64_t> > diskUsages;
    IosIPAAppOperation operation(_pDevice, NullProgressSubject::Instance());
    bool bRes = operation.GetAppDiskUsage(diskUsages);
    RETURN_FALSE_IF(!bRes);

    RETURN_TRUE_IF(_datas.empty());
    for (size_t i = 0; i < _datas.size(); i++)
    {
        AppData* pApp = _datas.at(i);

        map< wstring, vector<int64_t> >::iterator it = diskUsages.find(pApp->_packageName);
        if ( it == diskUsages.end() )
            continue;

        vector<int64_t> disk = it->second;
        if (disk.size() != 2)
            continue;

        pApp->_dynamicDiskUsage = disk.at(0);
        pApp->_staticDiskUsage = disk.at(1);
    }

    return bRes;
}

ND91Assistant::CommandExecuteResult ND91Assistant::AppManager::Load()
{
    if (_bLoading)
    {
        int nCount = 0;
        while (_bLoading && nCount < 20)
        {
            Sleep(500);
            nCount ++;
        }
        return COMMAND_EXECUTE_SUCCESS;
    }

    EnterCriticalSection(&_cs);
    _bLoading = true;

    LOG->WriteDebug(L"开始获得appdata...");
    GetAppListCommand cmd(_pDevice, APP_TYPE_ALL, APP_OWNER_ANY);
    cmd.Execute();
    cmd.SortData();

    ReleaseData();

    vector<AppData*> DataList = cmd.GetDataList();
    for(vector<AppData*>::iterator iter = DataList.begin(); iter != DataList.end(); iter++)
    {
        AppData* pAppData = *iter;
        _datas.push_back(new AppData(pAppData));
    }
	
	LOG->WriteDebug(L"获得appdata..."+CStrOperation::IntToWString(DataList.size()));
	LOG_DEBUG(cmd.Success()==COMMAND_EXECUTE_SUCCESS?L"加载程序列表成功":L"加载程序列表失败");

    _bLoading = false;
    LeaveCriticalSection(&_cs);
    return cmd.Success() ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
}

void ND91Assistant::AppManager::ReleaseData()
{
	EnterCriticalSection(&_cs);
    for(size_t i = 0; i < _datas.size(); ++i)
    {
        SAFE_DELETE(_datas[i]);
    }
    _datas.clear();
	LeaveCriticalSection(&_cs);
}
AppData* ND91Assistant::AppManager::FindAppInfoByID(wstring AppdataID)
{
	for(size_t i = 0; i < _datas.size(); ++i)
	{
		AppData* appdata =_datas.at(i);
		 		if(appdata->_packageName == AppdataID)
		 			return appdata;
	}
	return 0;
}
void ND91Assistant::AppManager::ReleaseGetDatas()
{
    for(size_t i = 0; i < _newDatas.size(); ++i)
    {
        SAFE_DELETE(_newDatas[i]);
    }
    _newDatas.clear();
}

bool ND91Assistant::AppManager::IsInstallApp(wstring packageName)
{
	if(_pDevice->GetDevInfo()->_deviceType==Android)
	{
		GetAppInfoCommand  cmd(_pDevice, packageName);
		const AppData* appdata = cmd.GetAppInfo(packageName);
		if(appdata && appdata->_packageName==packageName)
			return true;
		else
			return false;
	}
	for(size_t i = 0; i < _datas.size(); ++i)
	{
		  AppData* pAppData = _datas.at(i);
		  if(pAppData->_packageName == packageName)
			  return true;
	}
	return false;
}

const vector<AppData*>* ND91Assistant::AppManager::GetDatas()
{
    ReleaseGetDatas();
    for (size_t i = 0; i < _datas.size(); i++)
    {
        _newDatas.push_back(new AppData(_datas.at(i)));
    }
    return &_newDatas;
}

void ND91Assistant::AppManager::GetDatas( vector<AppData*>& vecData )
{
	for (size_t i = 0; i < _datas.size(); i++)
	{
		vecData.push_back(new AppData(_datas.at(i)));
	}
}

ND91Assistant::CommandExecuteResult ND91Assistant::AppManager::JudgeInstallAppResult(CommandExecuteResult installResult, AppData* pPackData, AppData* &pRetData)
{
	RETURN_VALUE_IF(!_pDevice, COMMAND_EXECUTE_FAIL);
    RETURN_VALUE_IF(installResult != COMMAND_EXECUTE_SUCCESS&&installResult!=COMMAND_EXECUTE_SUCCESS_AuthorizorFaile, installResult);
	LOG_DEBUG(L"JudgeInstallAppResult install success: "+pPackData->_AppName);

    if (_pDevice->GetDevInfo()->_deviceType == Ios)
    {/*
        Load();
        const vector<AppData*>* pApps = GetDatas();
        installResult = COMMAND_EXECUTE_FAIL;
        AppData* pApp = NULL;
        for (size_t i = 0; i < pApps->size(); i++)
        {
            pApp = pApps->at(i);
            if (pApp->_packageName != pPackData->_packageName
                || pApp->_strVersion != pPackData->_strVersion)
                continue;

            installResult = COMMAND_EXECUTE_SUCCESS;
            break;
        }
        installResult == COMMAND_EXECUTE_SUCCESS ? pRetData = pApp : pRetData = NULL;*/
        if(!pRetData)
			pRetData = new AppData(pPackData);
        _datas.push_back(new AppData(pPackData));
    }
    else
    {
        GetAppInfoCommand  cmd(_pDevice, pPackData->_packageName);
        AppData* data = (AppData*)cmd.GetAppInfo(pPackData->_packageName);
        if (data == NULL)
            installResult = COMMAND_EXECUTE_FAIL;
        //else if (data->_strVersion != pPackData->_strVersion)
        //    installResult = COMMAND_EXECUTE_FAIL;

		wstring result= (installResult == COMMAND_EXECUTE_FAIL) ? L" fail":L" sucess";
		LOG_DEBUG(L"JudgeInstallAppResult install end: "+pPackData->_AppName+result);

        if (installResult == COMMAND_EXECUTE_SUCCESS)
        {
            GetFileInfoCommand fileInfoCommand(_pDevice);
            fileInfoCommand.SetRemotePath(data->_strAppPhonePath);
            fileInfoCommand.Execute();
            data->_staticDiskUsage = fileInfoCommand.GetAndroidFileInfo()._size;
			//SAFE_DELETE(pRetData);
            pRetData = new AppData(data);
            _datas.push_back(new AppData(data));
        }
        //else
       //     pRetData = NULL;
    }

 
    return installResult;
}

ND91Assistant::CommandExecuteResult ND91Assistant::AppManager::InstallApp(wstring strAppFullName, wstring& errorMsg, AppData* &pRetData, ProgressObserver* pObs, bool bLock, bool bRecover, bool bOnSDCard, bool isMove, App2ExeInfo* AppInExeInfo)
{
	if(!CFileOperation::IsFileExist(strAppFullName.c_str()))
	{
		errorMsg =L"File not exist";
		return COMMAND_EXECUTE_FAIL;	
	}

	wstring oldname = strAppFullName;
	bool bRename = GetRightFileName(oldname,strAppFullName);

    wstring tempDir = Path::GlobalTemp()+CFileOperation::GetGuidW()+L"\\";
	LOG->WriteDebug(L"InstallApp app start:" + strAppFullName);
    AppData* pPackData = NULL;
	
	if(_pDevice->GetDevInfo()->_deviceType == Ios)
		pPackData=GetAppInfoByFile(strAppFullName, tempDir,true);  
	else
		pPackData=GetAppInfoByFile(strAppFullName, tempDir);  

	if(pPackData)
		LOG->WriteDebug(L"InstallApp GetInfo OK:" +  pPackData->_AppName);

	//app在exe包里时取不到app信息
	if(AppInExeInfo)
	{
		pPackData=new AppData();
		pPackData->_eAppType = APP_TYPE_EXE;
		pPackData->_packageName = strAppFullName;
		pPackData->_strAppPCPath = strAppFullName;
		pPackData->_strVersion = L"";
		pPackData->_ExtName = AppInExeInfo->AppInExeType;
		pPackData->_isLegalSoft = 1;
		NoDeamonInterface::SetPath(AppInExeInfo->backdir,AppInExeInfo->deamondir);
	}
	//注意，app2exe项目GetAppInfoByFile取回的指针肯定空，上一句就是处理这个状态，所以判断只能在这句下面
	RETURN_VALUE_IF(pPackData == NULL, COMMAND_EXECUTE_PARSEPKGERROR);
    RETURN_VALUE_IF(pPackData->_eAppType == APP_TYPE_UNKNOW, COMMAND_EXECUTE_PARSEPKGERROR);
    RETURN_VALUE_IF(pPackData->_packageName == L"com.nd.assistance", COMMAND_EXECUTE_UNSUPPORT);

	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_pDevice->GetId());
	if(pDevice == NULL)
	{
		return COMMAND_EXECUTE_CANCEL;
	}

	InstallCommand installCommand(_pDevice, pPackData);	
	installCommand.SetParam(AppInExeInfo);
    installCommand.SetLock(bLock);
    installCommand.SetRecover(bRecover);
    installCommand.SetOnSDCard(bOnSDCard);
    installCommand.SetIsMove(isMove);
    installCommand.Attach(pObs);
    installCommand.Execute();

	if(bRename)
	{
		RENAME_FILE(strAppFullName,oldname);		
	}

    errorMsg = installCommand.GetErrorMsg();
    //return installCommand.Result();
    // 判断应用是否安装成功
	 CommandExecuteResult ret=   JudgeInstallAppResult(installCommand.Result(), pPackData, pRetData);
	 //失败时，不能给pRetData复制，会导致崩溃
	 if(!pRetData && (ret == COMMAND_EXECUTE_SUCCESS || ret == COMMAND_EXECUTE_SUCCESS_AuthorizorFaile))
	 {
		 pRetData=new AppData(pPackData);
	 }

	 //SAFE_DELETE(pPackData);

	 return ret;
}

ND91Assistant::CommandExecuteResult ND91Assistant::AppManager::UnInstallApp( AppData* pAppData, ProgressObserver* pObs /*= NULL*/ )
{
    if (!pAppData)
        return COMMAND_EXECUTE_ARGUMENTERROR;
	
	CommandExecuteResult result;
	BaseCommand* basecmd = NULL;
	if ( _pDevice->GetDevInfo()->_deviceType == Android &&
		 APP_OWNER_SYS == pAppData->_eAppOwner )
	    basecmd = new UninstallSystemAppCommand(_pDevice, pAppData);
	else
		basecmd = new UninstallCommand(_pDevice, pAppData);
	
	basecmd->Attach(pObs);
	basecmd->Execute();
	result = basecmd->Result();
	SAFE_DELETE(basecmd);

    // 应用卸载成功后从列表中删除
    if (result == COMMAND_EXECUTE_SUCCESS)
        RemoveAppFromList(pAppData->_packageName);

	return result;
}

ND91Assistant::CommandExecuteResult ND91Assistant::AppManager::ArchiveApp( AppData* pAppData, wstring strFileOnPC, ProgressObserver* pObs /*= NULL*/ )
{
    if (!pAppData)
        return COMMAND_EXECUTE_ARGUMENTERROR;

    ArchiveAppCommand archiveCmd(_pDevice, pAppData, strFileOnPC);
    archiveCmd.Attach(pObs);
    archiveCmd.Execute();

    return archiveCmd.Result();
}

ND91Assistant::CommandExecuteResult ND91Assistant::AppManager::InstallDaemon(wstring strDaemonFullName, wstring daemonVersionOnService)
{
    RETURN_VALUE_IF( ! _pDevice || ! _pDevice->GetDevInfo(), COMMAND_EXECUTE_ARGUMENTERROR);

    IosDaemonHelper iosDaemon(_pDevice);
    bool bRec = iosDaemon.InstallDaemon(strDaemonFullName, daemonVersionOnService);
    if (bRec)
        _pDevice->SetIosDeamonInstalled(true);
    return  bRec ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
}

ND91Assistant::CommandExecuteResult ND91Assistant::AppManager::UnInstallDaemon()
{
    RETURN_VALUE_IF( ! _pDevice || ! _pDevice->GetDevInfo(), COMMAND_EXECUTE_ARGUMENTERROR);

    //停止守护运行
    KillDeamonProcess(L"TQServer");

    //删除守护相关文件
    IosDaemonHelper iosDaemon(_pDevice);
    return iosDaemon.UnInstallDaemon() ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
}

ND91Assistant::CommandExecuteResult ND91Assistant::AppManager::RepairIcon()
{
    if (_pDevice->GetDevInfo()->_connectType == USB 
        && _pDevice->GetDevInfo()->_ios_bJailBreaked)
    {
		SetCSLock(((DeviceInfo*)_pDevice->GetDevInfo())->_csIOSSessionLock);
        int ret = NoDeamonInterface::IconsRepair(_pDevice->GetDevInfo()->_ios_devptr);
        return ret == 0 ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
    }
    else
        return COMMAND_EXECUTE_UNSUPPORT;
}

std::wstring ND91Assistant::AppManager::PxlToIpa( wstring strPxlFullName )
{
    wstring strIpaFileName = L"";
    if (PxlToIpaHelper::ConverPxlToIpa(strPxlFullName, strIpaFileName,
        _pDevice->GetPath()->GetLocalAppPath()))
        return strIpaFileName;
    else
        return L"";
}

ND91Assistant::CommandExecuteResult ND91Assistant::AppManager::KillDeamonProcess( wstring strProcessName )
{
    KillProcessCommand kpCmd(_pDevice, strProcessName);
    kpCmd.Execute();
    return kpCmd.Result();
}

int ND91Assistant::AppManager::GetAppCount( APP_OWNER appOwner )
{
    int nCount = 0;
    if (_pDevice->GetDevInfo()->_deviceType == Android && _pDevice->GetDevInfo()->_connectType == USB)
    {
        AdbPMListCommand cmd(_pDevice->GetDevInfo()->_strSerialNumber);
        cmd.Execute();
        vector<AppData*> appList = cmd.getDataList();
        for (vector<AppData*>::iterator it = appList.begin(); it != appList.end(); it++)
            if ((*it)->_eAppOwner == appOwner || appOwner == APP_OWNER_ANY)
                nCount++;
    }
    else if (_pDevice->GetDevInfo()->_deviceType == Android && _pDevice->GetDevInfo()->_connectType == WIFI)
    {
        GetAppListCommand cmd(_pDevice, APP_TYPE_ALL, appOwner);
        cmd.Execute();
        nCount = cmd.GetDataList().size();
    }
    else
    {
        GetAppListCommand cmd(_pDevice, APP_TYPE_ALL, appOwner);
        nCount = cmd.GetIosAppCount();
    }
    return nCount;    
}

int ND91Assistant::AppManager::GetAppCount( bool bOnSDCard )
{
    int nCount = 0;
    for (vector<AppData*>::iterator it = _datas.begin(); it != _datas.end(); it++)
    {
        if ((*it)->_bOnSDCard == bOnSDCard)
            nCount ++;
    }
    return nCount;  
}

ND91Assistant::CommandExecuteResult ND91Assistant::AppManager::MoveToApp(AppData* pAppData, wstring& errorMsg, AppData* &pRetAppData, ProgressObserver* pObs)
{
    // 备份
    wstring strFileOnPC = _pDevice->GetPath()->GetLocalAppPath()+pAppData->_packageName+L".apk";
    if (COMMAND_EXECUTE_SUCCESS != ArchiveApp(pAppData, strFileOnPC/*, pObs*/))
	{
		errorMsg=L"设备不支持转移或程序已经不存在";
        return COMMAND_EXECUTE_FAIL;
	}

    // 安装
    CommandExecuteResult res = InstallApp(strFileOnPC, errorMsg, pRetAppData, pObs, false, true, !pAppData->_bOnSDCard, true, NULL);
	CFileOperation::DeleteFile(strFileOnPC);
    RETURN_VALUE_IF(res != COMMAND_EXECUTE_SUCCESS, res);

    // 判断是否转移成功
	//GetAppInfoCommand cmd(_pDevice, pAppData->_packageName);
	//const AppData* newapp = cmd.GetAppInfo(pAppData->_packageName);
	if(pRetAppData&&pRetAppData->_bOnSDCard ==pAppData->_bOnSDCard)
	{
		// 将要移动的应用从列表中删除
		RemoveAppFromList(pRetAppData);
		pRetAppData=NULL;
		return COMMAND_EXECUTE_FAIL;
	}

	RemoveAppFromList(pAppData->_packageName);
    // 将要移动的应用从列表中删除
    return res;
}

bool ND91Assistant::AppManager::GetRightFileName(wstring &oldname,wstring &filename)
{
	
	wstring name = CFileOperation::GetFileName(oldname);		
	if(WideCharToMultiByte(CP_ACP, 0, name.c_str(), name.length(), NULL, 0, NULL, NULL) >= 260 )
	{		
		wstring newname = CFileOperation::GetFileDir(oldname) + L"temp.";
		newname += CFileOperation::GetFileExt(oldname);
		filename = CFileOperation::RenameFile(newname);
		RENAME_FILE(oldname,filename);
	}
	else
		return false;

	return true;
}

 AppData* ND91Assistant::AppManager::GetAppInfoByFile( wstring filepath, wstring tempdir, bool icon)
 {
	 DeviceManager::GetInstance()->IosInit();

	wstring oldname = filepath;
	bool bRename = GetRightFileName(oldname,filepath);
	
	AppData* data=NULL;
	wstring ext=CFileOperation::GetFileExt(filepath);
	ext=CStrOperation::toLowerW(ext);
	//const DeviceData* device=(const DeviceData*)(_pDevice);
	wstring TempPath=tempdir+CFileOperation::GetGuidW()+L"\\";
	if(ext==L"apk")
	{
		ApkPackageInfo apkinfo = AndroidXMLOperation::GetPackageInfoFromApk(filepath);
		if(apkinfo.packageName.empty())
			return NULL;
		if(!data)data=new AppData();
		data->_AppName=AndroidXMLOperation::GetChineseAppName(&apkinfo);
		data->_packageName=apkinfo.packageName;
		data->_strVersion=apkinfo.versionName;
        data->_nVersionCode = _wtoi(apkinfo.versionCode.c_str());
		data->_eAppType=APP_TYPE_APK;
		data->_strAppPCPath=oldname;
		data->_strIconPhonePath=apkinfo.packageIcon;
        data->_sdkVer=apkinfo.minSdkVersion;
		if(apkinfo.packageIcon.empty())
			data->_strIconPhonePath=L"icon.png";

		//upzipIcon(filepath,apkinfo.packageIcon,data->_AppName,apkinfo.versionName);
	}
	else if(ext==L"npk")
	{
		wstring strXMLPath; //NPK的XML路径
		wstring strNPKPath;	//NPK
		//解压NPK
		wstring strTempPath=TempPath;
		strTempPath += CFileOperation::GetFileNameNoExt(filepath);    
		strTempPath += WSTRING_PATH_SEPARATER;
		strXMLPath = strTempPath + L"npk.xml";
		CFileOperation::UnZipFile(filepath, strTempPath);

		//载入xml	
		string str = CCodeOperation::UTF_8ToGB2312(CFileOperation::ReadFile(strXMLPath, false));
		NpkData npkData(str);
		for (vector<NPK_ITEM*>::iterator it = npkData._items.begin(); it != npkData._items.end(); it++)
		{
			NPK_ITEM* pItem = (NPK_ITEM*)(*it);
			//安装APK
			strNPKPath = strTempPath + pItem->Name + WSTRING_PATH_SEPARATER+pItem->apkFileName;
			data=GetAppInfoByFile(strNPKPath,tempdir,icon);
			if(data)
			{
				data->_eAppType=APP_TYPE_NPK;
				data->_strAppPCPath=oldname;
			}
			break;
		}
	}
	else if(ext==L"ipa")
	{
		data = PlistOperation::GetIpaAppInfo(TempPath, filepath);

		if(data && data->_strIconPhonePath.find(L"\\")==wstring::npos && data->_strIconPhonePath.find(L"/")==wstring::npos )
		{
			wstring iconpath = data->_strExecutable;
			iconpath+= L".app\\";
			data->_strIconPhonePath = iconpath + data->_strIconPhonePath;
		}

		if(data)
		{
			data->_strAppPCPath=oldname;
			data->_eAppType=APP_TYPE_IPA;
			if(data->_strExecutable.empty())
				data->_isLegalSoft = 0;
			else
			{
				wstring Executablename = data->_strExecutable + L".sinf";
				data->_isLegalSoft = CFileOperation::UnZipFile(filepath,TempPath,Executablename,false,L"SC_Info",true);
			}	

			wstring embeddedName = L"embedded.mobileprovision";
			data->_isEnterpriseApp = CFileOperation::UnZipFile(filepath,TempPath,embeddedName,false,data->_strExecutable + L".app",true);
		}
	}
	else if(ext==L"pxl")
	{
		wstring pxl_app_info_file_name =L"PxlPkg.plist";
		CFileOperation::UnZipFile(filepath, TempPath, pxl_app_info_file_name,false);
		vector<AppData*> vtInfos;
		wstring destinationOnComputer = TempPath+pxl_app_info_file_name;
		IosAppXmlHelper::PrasePxlAppInfo(destinationOnComputer, vtInfos, NULL);	
		CFileOperation::DeleteFile(destinationOnComputer);
		if(vtInfos.size()>0)
		{
            AppData* pData = PlistOperation::GetIpaAppInfo(TempPath, filepath);
			/*if(!pData)
			{
				LOG->WriteDebug(L"GetAppInfoFail:" + filepath);
				return NULL;
			}*/
			if(pData)
			{
				 vtInfos.at(0)->_DeviceFamily.insert(vtInfos.at(0)->_DeviceFamily.begin(), pData->_DeviceFamily.begin(), pData->_DeviceFamily.end());
				SAFE_DELETE(pData);
			}

			data=new AppData(vtInfos.at(0));
           data->_strAppPCPath=oldname;
			data->_eAppType=APP_TYPE_PXL;
			for (size_t i=0;i<vtInfos.size();i++)
			{
				SAFE_DELETE(vtInfos[i]);
			}
			vtInfos.clear();
		}
	}
	if(icon&&data&&ext!=L"npk")
		data->_strIconPCPath= AppManager::upzipIcon(filepath,TempPath,data->_strIconPhonePath,data->_AppName,data->_strVersion);
    if ( NULL != data )
        data->_eAppOwner = APP_OWNER_USER;

	if(bRename)
	{
		RENAME_FILE(filepath,oldname);		
	}
	return data;
}

 void ND91Assistant::AppManager::RemoveAppFromList( AppData* pAppData )
 {
	 // 将要移动的应用从列表中删除
	 vector<AppData*>::iterator it = _datas.begin();
	 while ( it != _datas.end() )
	 {
		 AppData* pApp = *it;
		 if (pApp == pAppData)
		 {
			 it = _datas.erase(it);
			 SAFE_DELETE(pApp);
			 break;
		 }
		 else
			 ++it;
	 }
 }

 void ND91Assistant::AppManager::RemoveAppFromList(wstring appID)
 {
     // 将要移动的应用从列表中删除
     vector<AppData*>::iterator it = _datas.begin();
     while ( it != _datas.end() )
     {
         AppData* pApp = *it;
         if (pApp->_packageName == appID)
         {
             it = _datas.erase(it);
             SAFE_DELETE(pApp);
             break;
         }
         else
             ++it;
     }
 }

wstring AppManager::upzipIcon(wstring zipFilePath,wstring tempicondir, wstring iconName,wstring appname,wstring appVersion)
{
	DeviceManager::GetInstance()->IosInit();

	wstring filename = CFileOperation::GetFileName(iconName);
	wstring sourcedir= CFileOperation::GetFileDir(iconName);
	if(sourcedir==filename)
		sourcedir=L"";
	if(!sourcedir.empty())sourcedir.erase(--sourcedir.end());
	//const DeviceData* device= dynamic_cast<const DeviceData*>(_pDevice);
	wstring tempDir = tempicondir ;
	filename=filename.empty()?L"icon.png":filename;

	if(! CFileOperation::UnZipFile(zipFilePath, tempDir, filename, false,sourcedir))
	{
		// 解压失败，换个文件试试
		filename = _wcsicmp(filename.c_str(), L"icon.png") ? L"icon.png" : L"icon.jpg";	
		if(! CFileOperation::UnZipFile(zipFilePath, tempDir, filename, false,sourcedir))
		{
			CFileOperation::DeleteDir(tempDir);
			return L"";
		}
	}

	wstring extractFilename = tempDir + filename;

	wchar_t ls[1];
	ls[0] = 13;
	//将图标文件改为新名字避免被覆盖
	wstring iconNewName = tempDir + L"icon\\";
	CFileOperation::MakeLocalDir(iconNewName);
	//iconNewName+=appname;
	//iconNewName+=appVersion;
	//iconNewName+=CFileOperation::GetFileExt(zipFilePath);
	iconNewName+=_T("icon.png");

	if( ! IosPngConver::ConverApplePngToNormal(extractFilename.c_str(), iconNewName.c_str()) )
	{
		if( ! CFileOperation::MoveFile(extractFilename, iconNewName))
		{
			CopyFile(extractFilename.c_str(), iconNewName.c_str(),false);
		}
	}
	return iconNewName;
}

int AppManager::FixFlashExit()
{
    Authorizor au(_pDevice);
    return au.FixFlashExit();
}

int AppManager::IsValideToAuthorize()
{
    return Authorizor::IsValideToAuthorize(_pDevice);
}

bool AppManager::InstallSpecialDebs(vector<wstring> remotePaths)
{
    int ires = NoDeamonInterface::Install91HelperService(_pDevice->GetDevInfo()->_ios_devptr);

    wstring program = L"dpkg --force-all -i ";
    for (size_t i = 0; i < remotePaths.size(); i++)
    {
        program += remotePaths.at(i) + L" ";
    }
    program = CStrOperation::trimRightW(program, L" ");
    bool res = NoDeamonInterface::SpecialRunProgram(_pDevice->GetDevInfo()->_ios_devptr, (wchar_t*)program.c_str(), 10) == 0;

    program =  L"dpkg --configure -a";
    res = NoDeamonInterface::SpecialRunProgram(_pDevice->GetDevInfo()->_ios_devptr, (wchar_t*)program.c_str(), 10) == 0;
    return res;
/*
    int ires = NoDeamonInterface::Install91HelperService(_pDevice->GetDevInfo()->_ios_devptr);

    wstring program = L"dpkg -i ";
    for (size_t i = 0; i < remotePaths.size(); i++)
    {
        program += remotePaths.at(i)+L" ";
    }
    program = CStrOperation::trimRightW(program, L" ");

    return NoDeamonInterface::SpecialRunProgram(_pDevice->GetDevInfo()->_ios_devptr, (wchar_t*)program.c_str(), 10) == 0;*/
}

bool AppManager::InstallAppsyncService()
{
    IosIPAAppOperation operation(_pDevice, NullProgressSubject::Instance());
    return operation.InstallAppService(APP_TYPE_APPSYNC);
}


#endif


