#include "pch_module.h"

#include "Module/Application/AppManagerEx.h"

#ifdef MODULE_APP



AppManagerEx::AppManagerEx(DeviceData* pDevice)
:_AppManager(pDevice)
{
    _pDevice = pDevice;
    InitializeCriticalSection(&_csLock);
}
AppManagerEx::~AppManagerEx()
{
	LeaveCriticalSection(&_csLock);
}

CommandExecuteResult AppManagerEx::Load()
{
    SetCSLock(_csLock);
	return _AppManager.Load();
}

CommandExecuteResult AppManagerEx::InstallApp(wstring strAppFullName, wstring& errorMsg, wstring &AppId, ProgressObserver* pObs, bool bLock, bool bRecover,
								bool bOnSDCard, bool isMove,App2ExeInfo* AppInExeInfo)
{
	AppData* appdata = NULL;	
	CommandExecuteResult ret =  _AppManager.InstallApp(strAppFullName,errorMsg,appdata,pObs,bLock,bRecover,bOnSDCard,isMove,AppInExeInfo);
	AppId = appdata->_packageName;
	return ret;
}
bool ND91Assistant::AppManagerEx::GetData( wstring AppdataID, AppData& appData )
{
	SetCSLock(_csLock);
	AppData* pAppData = _AppManager.FindAppInfoByID(AppdataID);

	if (pAppData)
	{
		appData = *pAppData;
		return true;
	}
	return false;
}

CommandExecuteResult AppManagerEx::UnInstallApp(wstring AppId, ProgressObserver* pObs)
{
	AppData* appdata = (AppData*)GetData(AppId);
	if(!appdata)
		return COMMAND_EXECUTE_FAIL;
	return _AppManager.UnInstallApp(appdata,pObs);
}
CommandExecuteResult AppManagerEx::ArchiveApp(wstring AppId, wstring strFileOnPC, ProgressObserver* pObs)
{

	AppData* appdata =  (AppData*)GetData(AppId);	
	if(!appdata)
		return COMMAND_EXECUTE_FAIL;
	CommandExecuteResult ret = _AppManager.ArchiveApp(appdata,strFileOnPC,pObs);

	SAFE_DELETE(appdata);
	return ret;
}
int  AppManagerEx::GetAppCount(APP_OWNER appOwner)
{
    SetCSLock(_csLock);
	return _AppManager.GetAppCount(appOwner);
}

int ND91Assistant::AppManagerEx::GetAppCount( bool bOnSDCard )
{
    SetCSLock(_csLock);
    return _AppManager.GetAppCount(bOnSDCard);
}
void AppManagerEx::ReleaseData()
{
	_AppManager.ReleaseData();
}
const AppData* AppManagerEx::GetData(wstring AppdataID)
{
    SetCSLock(_csLock);
	return _AppManager.FindAppInfoByID(AppdataID);
}

// AppData* ND91Assistant::AppManagerEx::GetAppDataCopyByID( wstring AppdataID )
// {
//     SetCSLock(_csLock);
//     AppData* pAppData = _AppManager.FindAppInfoByID(AppdataID);
// 
//     if (pAppData)
//     {
//        AppData* appDataRet = new AppData(pAppData);
//         return appDataRet;
//     }
//     return NULL;
// }

const vector<wstring> AppManagerEx::GetAppIds()
{
    SetCSLock(_csLock);
	vector<wstring> ids;
	const vector<AppData*>* apps = _AppManager.GetDatas();
	for(size_t i=0;i<apps->size();i++)
	{
		AppData* appdata =apps->at(i);
		ids.push_back(appdata->_packageName);
	}
	return ids;
}
CommandExecuteResult AppManagerEx::RepairIcon()
{
	return _AppManager.RepairIcon();
}
wstring AppManagerEx::PxlToIpa(wstring strPxlFullName)
{
	return _AppManager.PxlToIpa(strPxlFullName);
}
CommandExecuteResult AppManagerEx::KillDeamonProcess(wstring strProcessName)
{
	return _AppManager.KillDeamonProcess(strProcessName);
}
CommandExecuteResult AppManagerEx::InstallDaemon(wstring strDaemonFullName, wstring daemonVersionOnService)
{
	return _AppManager.InstallDaemon(strDaemonFullName, daemonVersionOnService);
}
CommandExecuteResult AppManagerEx::UnInstallDaemon()
{
	return _AppManager.UnInstallDaemon();
}
CommandExecuteResult AppManagerEx::MoveToApp(wstring AppId, wstring& errorMsg, AppData* &pRetAppData, ProgressObserver* pObs)
{
    SetCSLock(_csLock);
    AppData* appdata =  (AppData*)GetData(AppId);	
	if(!appdata)
		return COMMAND_EXECUTE_FAIL;
	CommandExecuteResult ret = _AppManager.MoveToApp(appdata,errorMsg,pRetAppData,pObs);
	return ret;
}
AppData* AppManagerEx::GetAppInfoByFile(wstring filepath, wstring tempdir,  bool icon)
{
	return AppManager::GetAppInfoByFile(filepath,tempdir,icon);
}
bool AppManagerEx::IsInstallApp(wstring packageName)
{
	return _AppManager.IsInstallApp(packageName);
}
int AppManagerEx::FixFlashExit()
{
	return _AppManager.FixFlashExit();
}
bool AppManagerEx::GetAppDiskUsage()
{
	return _AppManager.GetAppDiskUsage();
}

bool ND91Assistant::AppManagerEx::GetAppIcon( wstring packageName, wstring strIconPath )
{
    SetCSLock(_csLock);
    const vector<AppData*>* apps = _AppManager.GetDatas();
    for(size_t i=0;i<apps->size();i++)
    {
        AppData* appdata =apps->at(i);
        if(appdata && appdata->_packageName == packageName)
            return appdata->LoadAppIcon(_pDevice, strIconPath);
    }
    return false;
}

bool ND91Assistant::AppManagerEx::GetAppPermissionList( wstring packageName, vector<wstring>& permissionList )
{
    SetCSLock(_csLock);
    const vector<AppData*>* apps = _AppManager.GetDatas();
    for(size_t i=0;i<apps->size();i++)
    {
        AppData* appdata =apps->at(i);
        if(appdata && appdata->_packageName == packageName)
        {            
            if (appdata->LoadPermissions(_pDevice))
            {
                appdata->GetPermissions(permissionList);
                return true;
            }
        }
    }
    return false;
}

#endif

