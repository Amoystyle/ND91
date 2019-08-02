#include "StdAfx.h"
#include "InstallCaseManagerEx.h"
#include "InstallCaseModelEx.h"
#include "InstallCaseInterface.h"
#include "Module/Server/GetAppUpdateInfo.h"
#include "Logic/WebResModel.h"
//#include "Logic/StatisticHelp.h"
#include "InstallsCaseThread/NoticeMsgThread.h"
#include "InstallsCaseThread/InstallsCaseThread.h"
#include "InstallsCaseThread/LoadWenCaseThread.h"
INSTALLCASE_BEGIN

#define HTTPUPDATEURL "http://pcx.sj.91.com/soft/all/" 

#define CHECK_OPERATING if(_bOperating){ Notice(L"正在处理数据请稍候...", 2000); return;}
#define CHECK_SEARCHING if(_bSearching){ Notice(L"正在处理数据请稍候...", 2000); return;}
#define CHECK_FRESHING if(_bRefreshing){ Notice(L"正在处理数据请稍候...", 2000); return;}
#define CONTINUE_IF(x)            if ( x ) { continue; }

bool g_bResearch=false;
InstallCaseModelEx::InstallCaseModelEx(InstallCaseInterface* pIntf)
{INTERFUN;
	CRITICAL_SECTION _csLock0;
	CRITICAL_SECTION _csLock1;
	CRITICAL_SECTION _csLock2;
	CRITICAL_SECTION _csLock3;
	InitializeCriticalSection(&_csLock0);
	InitializeCriticalSection(&_csLock1);
	InitializeCriticalSection(&_csLock2);
	InitializeCriticalSection(&_csLock3);
	_csLockList[0] = _csLock0;
	_csLockList[1] = _csLock1;
	_csLockList[2] = _csLock2;
	_csLockList[3] = _csLock3;
	_bExit = false;
	_bRefreshing = false;
	_pInterface = pIntf;
	_bOperating = false;
	_bSearching = false;
	_bFirstiOSUPSearchFinish = false;
	_bFirstAndUPSearchFinish = false;

	this->GetNetInstallsCaseDataList();
	InstallsCaseThread::SetInstallCaseModelEx(this);
}

InstallCaseModelEx::~InstallCaseModelEx(void)
{INTERFUN;
	_bExit = true;	
}

//----------------------------------------------------------------------- 公有方法 Begin -----------------------------------------------------------------------

void InstallCaseModelEx::Load()
{INTERFUN;

	InstallsCaseThread* pInstallsCaseThread = new InstallsCaseThread;
	pInstallsCaseThread->SetThreadInfo(Action_LoadCase);
	pInstallsCaseThread->Run();

	LoadWebCase(true);//获取网络装机方案
	InstallsCaseThread* pCheckIOSUpgrateThread = new InstallsCaseThread;
	pCheckIOSUpgrateThread->SetThreadInfo(Action_CheckIOSUpgrate);
	pCheckIOSUpgrateThread->Run();

	InstallsCaseThread* pCheckAndroidUpgrate = new InstallsCaseThread;
	pCheckAndroidUpgrate->SetThreadInfo(Action_CheckAndroidUpgrate);
	pCheckAndroidUpgrate->Run();

	InstallsCaseThread* pCheckIOSLegalUpgrade = new InstallsCaseThread;
	pCheckIOSLegalUpgrade->SetThreadInfo(Action_CheckIOSLegalUpgrade);
	pCheckIOSLegalUpgrade->Run();

}

void InstallCaseModelEx::LoadWebCase( bool bLoadDataBase )
{INTERFUN;
	InstallsCaseThread* pInstallsCaseThread = new InstallsCaseThread;
	pInstallsCaseThread->SetThreadInfo(Action_LoadWebCase, 0, bLoadDataBase);
	pInstallsCaseThread->Run();
}

//登陆重新获取装机方案
bool InstallCaseModelEx::Handle(const ResponseUserLoginMsg* pMsg)
{
	wstring wsUserId =  pMsg->userID;

	LoadWenCaseThread* ploadThread = new LoadWenCaseThread;
	ploadThread->SetLoadCaseInfo(this, false, _wtoi(wsUserId.c_str()), pMsg->nCpaUser == 1 ? true : false);
	ploadThread->Run();

	return true;
}
//退出重新获取装机方案
bool InstallCaseModelEx::Handle(const QuitMsg* pMsg)
{
	LoadWenCaseThread* ploadThread = new LoadWenCaseThread;
	ploadThread->SetLoadCaseInfo(this, false);
	ploadThread->Run();

	return true;
}

void InstallCaseModelEx::RefreshCase(bool isOnlyShowLastVer, int caseId )
{INTERFUN;	
	if(_bOperating || _bRefreshing)
	{
		InstallCaseMsg msg;
		msg.message = WM_CASE_FINISHREFRESH;
		msg.wParam  = (WPARAM)caseId;
		msg.lParam  = NULL;
		msg.SendToUI();
	}

	CHECK_OPERATING;
	CHECK_FRESHING;
	if(caseId==0 && !_bFirstiOSUPSearchFinish)
	{
		InstallCaseMsg msg;
		msg.message = WM_CASE_FINISHREFRESH;
		msg.wParam  = (WPARAM)caseId;
		msg.lParam  = NULL;
		msg.SendToUI();

		Notice(L"正在处理数据请稍候...", 2000);
		return;
	}
	if(caseId==1 && !_bFirstAndUPSearchFinish)
	{
		InstallCaseMsg msg;
		msg.message = WM_CASE_FINISHREFRESH;
		msg.wParam  = (WPARAM)caseId;
		msg.lParam  = NULL;
		msg.SendToUI();

		Notice(L"正在处理数据请稍候...", 2000);
		return;
	}

	InstallsCaseThread* pInstallsCaseThread = new InstallsCaseThread;
	pInstallsCaseThread->SetThreadInfo(Action_RefreshCase, caseId, isOnlyShowLastVer);
	pInstallsCaseThread->Run();
	
}

bool InstallCaseModelEx::Handle(const LoadAppListMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	this->LoadAppList(pMsg->caseId, pMsg->type);

	return true;
}

void InstallCaseModelEx::LoadAppList( int caseId, wstring type)
{INTERFUN;
	InstallsCaseThread* pInstallsCaseThread = new InstallsCaseThread;
	pInstallsCaseThread->SetThreadInfo(Action_LoadAppList, caseId, true, type);
	pInstallsCaseThread->Run();

}

void InstallCaseModelEx::SearchApp( AppSearchContextEx* context )
{INTERFUN;
	InstallsCaseThread* pInstallsCaseThread = new InstallsCaseThread;
	pInstallsCaseThread->SetThreadInfo(Action_SearchApp, 0, true, L"", context);
	pInstallsCaseThread->Run();

}

bool InstallCaseModelEx::Handle(const AddCaseMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	if (-1 == pMsg->caseType)
		this->AddCase((vector<wstring>*)pMsg->pPathList);
	else
		this->AddCase((_INSTALLS_CASE_DATA_TYPE)pMsg->caseType, (vector<wstring>*)pMsg->pPathList, pMsg->fromLib);

	return true;
}

void InstallCaseModelEx::AddCase( _INSTALLS_CASE_DATA_TYPE caseType, vector<wstring>* pathList, bool fromLib /*= true*/ )
{INTERFUN;
	CHECK_OPERATING;
	int count = InstallCaseManagerEx::GetInstance()->InstallCaseDataList.size();
	if(InstallCaseManagerEx::GetInstance()->InstallCaseDataList.size()>54)
	{
		Notice(CommonUIFunc::LoadStringW(L"InstallCase_Text_InstallCaseNumLimit"), 2000);
		return;
	}
	if(pathList == NULL || pathList->size() <= 0)
	{
		if(fromLib)
		{
			this->CreateCaseFromLib(caseType);
		}
		else
		{
			this->CreateCaseFromFolder(caseType);
		}
	}
	else
	{
		//将idList对应的软件列表创建为新方案;
		this->CreateCaseByAppList(caseType, pathList);
	}
}

void InstallCaseModelEx::AddCase( vector<wstring>* fileList )
{INTERFUN;
	CHECK_OPERATING;
	if(InstallCaseManagerEx::GetInstance()->InstallCaseDataList.size()>54)
	{
		Notice(CommonUIFunc::LoadStringW(L"InstallCase_Text_InstallCaseNumLimit"), 2000);
		return;
	}
	_INSTALLS_CASE_DATA_TYPE caseType = _INSTALLS_CASE_DATA_IOS;

	vector<wstring>* pathList = new vector<wstring>();
	this->GetCaseTypeAndPathList(caseType, fileList, pathList);


	if(caseType == _INSTALLS_CASE_DATA_IOS && CSystemInfo::IsNeedInstalliTunes())
	{
		_pInterface->IsNeedInstalliTunesConfirm();
		return;
	}

	wstring caseName = this->GetNewDefaultCaseName(caseType);
	_pInterface->GetCaseName(caseName);
	if(caseName.empty())
	{
		return;
	}

	InstallsCaseData* caseData = CreateCase(caseName, caseType);
	if(caseData == NULL)
		return;
	this->AddAppToCaseByPath(caseData->Id(), pathList);
}

void InstallCaseModelEx::EditCase( int caseId )
{INTERFUN;
	CHECK_OPERATING;

	InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->GetCaseById(caseId);
	if(caseData == NULL)
		return;

	wstring caseName = caseData->CaseName();
	vector<wstring>* pPathList = new vector<wstring>();

	_pInterface->GetEditCaseIdList(caseData, caseName, pPathList);
	if(caseName.empty())
	{
		return;
	}

	InstallsCaseThread* pInstallsCaseThread = new InstallsCaseThread;
	pInstallsCaseThread->SetThreadInfo(Action_EditCase, caseId, true, caseName, pPathList);
	pInstallsCaseThread->Run();

}

void InstallCaseModelEx::AddAppToCaseByPath( int caseId, vector<wstring>* pathList, bool showInfo, wstring path)
{INTERFUN;
	if(showInfo)
		CHECK_OPERATING;

	InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->GetCaseById(caseId);
	if(caseData == NULL)
		return;

	if(caseData->CaseType() == _INSTALLS_CASE_DATA_IOS && CSystemInfo::IsNeedInstalliTunes())
	{
		if(showInfo)
		{
			_pInterface->IsNeedInstalliTunesConfirm();
			return;
		}
	}

	vector<wstring>* pPathList = new vector<wstring>();
	if(pathList== NULL || pathList->size() <= 0 && showInfo)
	{
		_pInterface->GetAppPath(caseData->CaseType(), pPathList);
	}
	else
	{
		pPathList->insert(pPathList->end(), pathList->begin(), pathList->end());
	}
	if(pPathList->size() <= 0 && showInfo)
	{
		return;
	}

	vector<wstring>* pTempParam = NULL;
	if(!showInfo)
	{
		pTempParam = new vector<wstring>;
		wstring phoneCate = caseData->CaseType() == _INSTALLS_CASE_DATA_IOS? L"iOS" : L"Android";
		pTempParam->push_back(path);
		pTempParam->push_back(phoneCate);
	}


	InstallsCaseThread* pAddAppToCaseMsg = new InstallsCaseThread;
	pAddAppToCaseMsg->SetThreadInfo(Action_AddAppToCase, caseId, true, L"", pPathList, pTempParam);
	pAddAppToCaseMsg->Run();

}

void InstallCaseModelEx::AddAppToCase( int caseId, vector<wstring>* pathList)
{INTERFUN;
	CHECK_OPERATING;

	vector<wstring>* appList = new vector<wstring>();
	appList->insert(appList->end(), pathList->begin(), pathList->end());

	InstallsCaseThread* pAddAppToCaseMsg = new InstallsCaseThread;
	pAddAppToCaseMsg->SetThreadInfo(Action_AddAppToCase, caseId, false, L"", appList);
	pAddAppToCaseMsg->Run();
}

bool InstallCaseModelEx::Handle(const RemoveCaseByCaseIdMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	this->RemoveCase(pMsg->idList);
	return true;
}

void InstallCaseModelEx::RemoveCase( vector<int>* idList )
{INTERFUN;
	CHECK_OPERATING;

	if(!_pInterface->RemoveCaseConfirm())
		return;

	vector<int>* pIdList = new vector<int>();
	pIdList->insert(pIdList->end(), idList->begin(), idList->end());

	InstallsCaseThread* pInstallsCaseThread = new InstallsCaseThread;
	pInstallsCaseThread->SetThreadInfo(Action_RemoveCase, 0, true, L"", pIdList);
	pInstallsCaseThread->Run();

}

void InstallCaseModelEx::RemoveAppFromCase( int caseId, vector<wstring>* pathList, bool removeFile, bool isFolderRemove)
{INTERFUN;
	CHECK_OPERATING;

	InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->GetCaseById(caseId);

	if(isFolderRemove)
	{
		if(!_pInterface->RemoveFolderConfirm())
			return;
	}
	else
	{
		if(!_pInterface->RemoveAppConfirm(caseData->PlanType() == _INSTALL_PLAN_TYPE_SYS && removeFile))
			return;
	}

	vector<wstring>* pPathList = new vector<wstring>();
	pPathList->insert(pPathList->end(), pathList->begin(), pathList->end());

	InstallsCaseThread* pInstallsCaseThread = new InstallsCaseThread;
	pInstallsCaseThread->SetThreadInfo(Action_RemoveAppFromCase, caseId, removeFile, isFolderRemove?L"FolderRemove":L"", pPathList);
	pInstallsCaseThread->Run();

}

bool InstallCaseModelEx::Handle(const UpgrateAppMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}
	this->UpgrateApp(pMsg->caseId, pMsg->pPathList);

	return true;
}

void InstallCaseModelEx::UpgrateApp( int caseId, vector<wstring>* pathList )
{INTERFUN;
	CHECK_OPERATING;
	vector<wstring>* pPathList = new vector<wstring>();
	pPathList->insert(pPathList->end(), pathList->begin(), pathList->end());

	InstallsCaseThread* pInstallsCaseThread = new InstallsCaseThread;
	pInstallsCaseThread->SetThreadInfo(Action_UpgrateApp, caseId, true, L"", pPathList);
	pInstallsCaseThread->Run();

}

bool InstallCaseModelEx::Handle(const UpgradeAppToLegalMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	this->UpgradeAppToLegal(pMsg->caseId, pMsg->pPathList);

	return true;
}

void InstallCaseModelEx::UpgradeAppToLegal( int caseId, vector<wstring>* pathList )
{INTERFUN;
	CHECK_OPERATING;
	if (NULL == pathList)
	{
		return;
	}
	vector<wstring>* pPathList = new vector<wstring>();
	pPathList->insert(pPathList->end(), pathList->begin(), pathList->end());

	InstallsCaseThread* pInstallsCaseThread = new InstallsCaseThread;
	pInstallsCaseThread->SetThreadInfo(Action_UpgrateAppToLegal, caseId, true, L"", pPathList);
	pInstallsCaseThread->Run();

}

bool InstallCaseModelEx::Handle(const InstallAppByCaseIdMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	this->InstallApp(pMsg->sn, pMsg->caseId, pMsg->pathList);

	return true;
}

void InstallCaseModelEx::InstallApp( wstring sn, int caseId, vector<wstring>* pathList )
{INTERFUN;
	vector<wstring>* pPathList = new vector<wstring>();
	pPathList->insert(pPathList->end(), pathList->begin(), pathList->end());

	InstallsCaseThread* pInstallsCaseThread = new InstallsCaseThread;
	pInstallsCaseThread->SetThreadInfo(Action_InstallApp, caseId, true, sn, pPathList);
	pInstallsCaseThread->Run();

}

void InstallCaseModelEx::AddDownLoadFolder(wstring iosPath, wstring androidPath,wstring fromModular)
{INTERFUN;
	vector<wstring>* pathList = new vector<wstring>();	
	vector<wstring> subFiles;

	int ioscount = 0;
	int andcount = 0;
	CFileOperation::FindAllFileInDir(iosPath, subFiles, _T("ipa"));
	pathList->insert(pathList->end(),subFiles.begin(), subFiles.end());
	ioscount += subFiles.size();
	subFiles.clear();
	CFileOperation::FindAllFileInDir(iosPath, subFiles, _T("pxl"));
	pathList->insert(pathList->end(),subFiles.begin(), subFiles.end());
	subFiles.clear();
	ioscount += subFiles.size();

	CFileOperation::FindAllFileInDir(androidPath, subFiles, _T("apk"));
	pathList->insert(pathList->end(),subFiles.begin(), subFiles.end());
	andcount += subFiles.size();
	subFiles.clear();
	CFileOperation::FindAllFileInDir(androidPath, subFiles, _T("npk"));
	pathList->insert(pathList->end(),subFiles.begin(), subFiles.end());
	andcount += subFiles.size();

	vector<wstring>* pTemp = new vector<wstring>;
	pTemp->push_back(iosPath);
	pTemp->push_back(CStrOperation::IntToWString(ioscount));
	pTemp->push_back(androidPath);
	pTemp->push_back(CStrOperation::IntToWString(andcount));

	InstallsCaseThread* pAddAppToCaseMsg = new InstallsCaseThread;
	pAddAppToCaseMsg->SetThreadInfo(Action_AddAppToCase, -1, true, fromModular, pathList, pTemp);
	pAddAppToCaseMsg->Run();

}

void InstallCaseModelEx::AddLibFolder(_INSTALLS_CASE_DATA_TYPE type, wstring path, bool showInfo )
{INTERFUN;
	if(showInfo)
		CHECK_OPERATING;

	wstring folderPath = path;

	if(folderPath.empty())
		folderPath = _pInterface->GetFolder(type);

	if(folderPath.empty())
		return;

	vector<wstring> pathList;

	vector<wstring> files;
	wstring ext, ext2;
	if(type == _INSTALLS_CASE_DATA_IOS)
	{
		ext = L"ipa";
		ext2 = L"pxl";
	}
	else
	{
		ext = L"apk";
		ext2 = L"npk";
	}
	//IOS
	CFileOperation::FindAllFileInDir(folderPath, files, ext);
	if(files.size() > 0)
		pathList.insert(pathList.end(), files.begin(), files.end());
	files.clear();
	CFileOperation::FindAllFileInDir(folderPath, files, ext2);
	if(files.size() > 0)
		pathList.insert(pathList.end(), files.begin(), files.end());

	if(pathList.size() <= 0 && showInfo)
	{
		Notice(CommonUIFunc::LoadString(_T("InstallCase_Text_FileNoFound")), L"#CommonUi_icon_TipWarning", 2000);
		return;
	}

	InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->GetSystemCaseData(type);
	this->AddAppToCaseByPath(caseData->Id(), &pathList, showInfo, path);
}

void InstallCaseModelEx::AddFilesToLib(vector<wstring>* iosPathList, vector<wstring>* androidPathList)
{INTERFUN;
	if(iosPathList != NULL && iosPathList->size() > 0 && CSystemInfo::IsNeedInstalliTunes())
	{
		_pInterface->IsNeedInstalliTunesConfirm();
		return;
	}

	CHECK_OPERATING;

	vector<wstring>* pIOSPathList = new vector<wstring>();
	pIOSPathList->insert(pIOSPathList->end(), iosPathList->begin(), iosPathList->end());
	vector<wstring>* pAndroidPathList = new vector<wstring>();
	pAndroidPathList->insert(pAndroidPathList->end(), androidPathList->begin(), androidPathList->end());

	InstallsCaseThread* pInstallsCaseThread = new InstallsCaseThread;
	pInstallsCaseThread->SetThreadInfo(Action_AddFilesToLib, 0, true, L"", pIOSPathList, pAndroidPathList);
	pInstallsCaseThread->Run();

	Notice(CommonUIFunc::LoadStringW(_T("软件已添加到库！")), 2000);
}
InstallsAppInfo* InstallCaseModelEx::AddSingleAppFileToLib(wstring filePath, bool &isReshUI, int fid,  wstring oldFilePath)
{
	InstallsAppInfo* appInfo = AddSingleAppFileToLib(  filePath,  fid,  oldFilePath);
	if (appInfo != NULL)
	{
		InstallsCaseData* caseData = NULL;
		wstring ext= CFileOperation::GetFileExt(filePath);
		ext=CStrOperation::toLowerW(ext);

		//添加到库
		if(ext == L"ipa" || ext == L"pxl")
			caseData = InstallCaseManagerEx::GetInstance()->GetSystemCaseData(_INSTALLS_CASE_DATA_IOS);
		else if(ext == L"apk" || ext == L"npk")
			caseData = InstallCaseManagerEx::GetInstance()->GetSystemCaseData(_INSTALLS_CASE_DATA_ANDROID);

		if ( !oldFilePath.empty() )
		{
			InstallsAppInfo* appInfoOld = caseData->GetAppBySavePath(oldFilePath);
			if (appInfoOld != NULL && appInfoOld->m_isLegalSoft != appInfo->m_isLegalSoft)
			{
				isReshUI = true;
			}			
		}
	}
	return appInfo;
}

InstallsAppInfo* InstallCaseModelEx::AddSingleAppFileToLib( wstring filePath, int fid, wstring oldFilePath)
{INTERFUN;
	InstallsCaseData* caseData = NULL;
	wstring ext= CFileOperation::GetFileExt(filePath);
	ext=CStrOperation::toLowerW(ext);

	//添加到库
	if(ext == L"ipa" || ext == L"pxl")
		caseData = InstallCaseManagerEx::GetInstance()->GetSystemCaseData(_INSTALLS_CASE_DATA_IOS);
	else if(ext == L"apk" || ext == L"npk")
		caseData = InstallCaseManagerEx::GetInstance()->GetSystemCaseData(_INSTALLS_CASE_DATA_ANDROID);

	InstallsAppInfo* appInfo = InstallCaseManagerEx::GetInstance()->AddAppToCaseByPath(caseData, filePath, true, fid);


	if(appInfo == NULL)
		return NULL;
	
	//InstallCaseMsg msg;
	//msg.message = WM_CASE_APP_ADD;
	//msg.wParam  = (WPARAM)caseData->Id();
	//msg.lParam  = (LPARAM)appInfo;
	//msg.SendToUI();

	_pInterface->AddAppToView(caseData->Id(), appInfo);

	//msg.message = WM_CASE_SET_NUMBER;
	//msg.wParam  = (WPARAM)caseData->Id();
	//msg.lParam  = (LPARAM)caseData;
	//msg.SendToUI();

	_pInterface->UpdateCaseView(caseData);

	if(oldFilePath!=L"")//oldFilePath升级的时候不为空
	{
		//移除底版本软件
		this->RemoveDiffVerApp(appInfo, oldFilePath);//从Mode移除
	}
	else
	{
		vector<AppCountByFolder*> oldFolders;
		oldFolders.insert(oldFolders.end(), InstallCaseManagerEx::GetInstance()->AppFolderList.begin(), InstallCaseManagerEx::GetInstance()->AppFolderList.end());
		CheckFolderChanged(&oldFolders);
	}

	return appInfo;
}

void InstallCaseModelEx::AppUpdategradeCancel(CTaskItem* item)
{INTERFUN;
	InstallsCaseData* caseData = NULL;

	if(item->m_platform == L"Android")
		caseData = InstallCaseManagerEx::GetInstance()->GetCaseById((int)Sys_Case_Android_Upgrate_ID);
	else
		caseData = InstallCaseManagerEx::GetInstance()->GetCaseById((int)Sys_Case_IOS_Upgrate_ID);

	RETURN_IF(!caseData);

	//UpgrateCancel* pUpgrateCancel = new UpgrateCancel;
	//pUpgrateCancel->nUpgradetype  = item->m_taskupgradetype;
	//pUpgrateCancel->wstrAppId     = item->m_appid;
	//pUpgrateCancel->wstrOldappfullpath = item->m_oldappfullpath;

	//InstallCaseMsg msg;
	//msg.message = WM_CASE_APP_UPGRATECANCEL;
	//msg.wParam  = (WPARAM)caseData->Id();
	//msg.lParam  = (LPARAM)pUpgrateCancel;
	//msg.SendToUI();

	InstallsCaseData* casedata =  this->GetCaseById(caseData->Id());
	if(casedata)
	{
		_pInterface->CancelAppUpgrate(caseData->Id(), item->m_oldappfullpath);
	}

}

const vector<AppCountByFolder*> InstallCaseModelEx::GetAppFolderList()
{INTERFUN;
	return InstallCaseManagerEx::GetInstance()->AppFolderList;
}

void InstallCaseModelEx::RemoveAppFolder(AppCountByFolder* appFolder)
{INTERFUN;
	_INSTALLS_CASE_DATA_TYPE type = appFolder->Filetype;

	InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->GetSystemCaseData(type);
	if(appFolder->AppPathList->size() == 0)
	{
		this->RemoveAppFromCase(appFolder->m_id, appFolder->AppPathList, false, true);
	}
	else
		this->RemoveAppFromCase(caseData->Id(), appFolder->AppPathList, false, true);
}

void InstallCaseModelEx::OpenDir( wstring path, bool select )
{INTERFUN;
	if(select)
		BaseOperation::FindTarget(path.c_str());
	else
		::ShellExecute(0,_T("open"),path.c_str(),NULL,NULL,SW_SHOWNORMAL);
}

const vector<InstallsCaseData*> InstallCaseModelEx::GetCaseDataList()
{INTERFUN;
	return InstallCaseManagerEx::GetInstance()->InstallCaseDataList;
}

vector<InstallsCaseData *> InstallCaseModelEx::GetNetInstallsCaseDataList()
{INTERFUN;
	return InstallCaseManagerEx::GetInstance()->NetInstallPlans;
}


vector<InstallsCaseData *> InstallCaseModelEx::GetSortByUseTimeCaseList()
{INTERFUN;
	vector<InstallsCaseData*> caseList = GetCaseDataList();

	return caseList;
}

bool InstallCaseModelEx::Handle(GetCaseByIdMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	pMsg->pDataCase = this->GetCaseById(pMsg->CaseDataid);

	return true;
}

InstallsCaseData* InstallCaseModelEx::GetCaseById( int caseId )
{INTERFUN;
	return InstallCaseManagerEx::GetInstance()->GetCaseById(caseId);
}

InstallsCaseData* InstallCaseModelEx::GetSystemCaseData( _INSTALLS_CASE_DATA_TYPE caseType )
{INTERFUN;
	return InstallCaseManagerEx::GetInstance()->GetSystemCaseData(caseType);
}

//----------------------------------------------------------------------- 公有方法 End   -----------------------------------------------------------------------

void InstallCaseModelEx::LoadDataInternal()
{INTERFUN;
	//初始化
	InstallCaseManagerEx::GetInstance()->Load();

	//加载方案
	this->LoadCaseList();

	CheckFolderChanged(NULL);

	//第一次加载，扫描指定的文件夹
	if(CONFIG_HANDLE->_bFirstTimeRun)
	{
		this->LoadDefaultFolder();

		CONFIG_HANDLE->SetFirstTimeRun(true);                 
	}      
}

void InstallCaseModelEx::LoadWebCaseInternal( bool bLoadDataBase, int nUserId, bool bCpa )
{INTERFUN;
	InstallCaseManagerEx::GetInstance()->LoadWebCase(bLoadDataBase, nUserId, bCpa);

	// 发消息给主界面，获取网络装机方案完成
	//SendMessage(Singleton::GetMainWnd(),WM_NETCASE_LOADFINISHED,0,0);

	//zjf待制作
	LoadFinshedMsg msg;
	msg.SendToUI();
}

void InstallCaseModelEx::CheckIOSUpgrateInternal()
{INTERFUN;
	InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->GetSystemCaseData(_INSTALLS_CASE_DATA_IOS);//by chenxy 取数据源
	int count = InstallCaseManagerEx::GetInstance()->CheckPiracyUpgrateAppList(caseData);//by chenxy 网络请求是否有升级的
	
	InstallCaseMsg msg;
	msg.message = WM_CASE_IOSAPPCHECKED;
	msg.wParam  = (WPARAM)count;
	msg.lParam  = NULL;
	msg.SendToUI();

}

void InstallCaseModelEx::CheckAndroidUpgrateInternal()
{INTERFUN;
	InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->GetSystemCaseData(_INSTALLS_CASE_DATA_ANDROID);
	int count = InstallCaseManagerEx::GetInstance()->CheckPiracyUpgrateAppList(caseData);

	InstallCaseMsg msg;
	msg.message = WM_CASE_ANDAPPCHECKED;
	msg.wParam  = (WPARAM)count;
	msg.lParam  = NULL;
	msg.SendToUI();
}

void InstallCaseModelEx::CheckIOSLegalUpgradeInternal()
{INTERFUN;
	InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->GetSystemCaseData(_INSTALLS_CASE_DATA_IOS);
	int count = InstallCaseManagerEx::GetInstance()->CheckLegalUpgrateAppList(caseData);

	//SendMessage(Singleton::GetMainWnd(), WM_CASE_IOSLEGALAPPCHECKED, (WPARAM)count, NULL);//IOS正版升级数据完成消息
}

void InstallCaseModelEx::RefreshCaseInternal( int caseId, bool isOnlyShowLaserVer )
{INTERFUN;
	Notice(CommonUIFunc::LoadStringW(_T("InstallCase_Text_CaseRefreshing")), 2000);

	if(caseId == -1)
	{
		RefreshSimpleCase(GetCaseById(Sys_Case_IOS_ID),isOnlyShowLaserVer);
		RefreshSimpleCase(GetCaseById(Sys_Case_Android_ID),isOnlyShowLaserVer);
		this->RefreshUpgrateCase(GetCaseById(Sys_Case_IOS_ID),isOnlyShowLaserVer);
		this->RefreshUpgrateCase(GetCaseById(Sys_Case_Android_ID),isOnlyShowLaserVer);

		InstallCaseMsg msg;
		msg.message = WM_CASE_APP_ADD_FILTER;
		msg.wParam  = (WPARAM)(GetCaseById(Sys_Case_IOS_ID)->Id());
		msg.lParam  = NULL;
		msg.SendToUI();

		msg.wParam  = (WPARAM)(GetCaseById(Sys_Case_Android_ID)->Id());
		msg.lParam  = NULL;
		msg.SendToUI();

	}
	else
	{
		InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->LoadAppList(caseId);
		this->RefreshSimpleCase(caseData,isOnlyShowLaserVer);
		this->RefreshUpgrateCase(caseData,isOnlyShowLaserVer);
	}
	InstallCaseMsg msg;
	msg.message = WM_CASE_FINISHREFRESH;
	msg.wParam  = (WPARAM)caseId;
	msg.lParam  = NULL;
	msg.SendToUI();

	Notice(CommonUIFunc::LoadStringW(_T("InstallCase_Text_CaseRefreshCompleted")), 2000);
}

void InstallCaseModelEx::RefreshSimpleCase(InstallsCaseData* caseData,bool isOnlyShowLastVer)
{INTERFUN;
	InstallsAppList* appList = caseData->ListApp();

	//移除不存在的软件信息
	vector<wstring> pathList;
	for (InstallsAppList::iterator it = appList->begin(); it != appList->end(); it++)
	{
		InstallsAppInfo* appInfo = *it;
		if(!CFileOperation::IsFileExist(appInfo->m_strSavePath.c_str()))
			pathList.push_back(appInfo->m_strSavePath);
	}
	if(pathList.size() > 0)
	{
		this->RemoveAppFromCaseInternal(caseData->Id(), L"", true, (void*)&pathList);
	}

	//扫描目录添加不存在的软件
	if(caseData->PlanType() == _INSTALL_PLAN_TYPE_SYS)
	{
		vector<wstring> pathListResult;
		this->GetRefreshFileList(caseData, pathListResult);

		if(pathListResult.size() > 0)
		{
			vector<int>* pTemp = new vector<int>;
			if (isOnlyShowLastVer)
			{
				pTemp->push_back(1);
			}
			else
			{
				pTemp->push_back(0);
			}
			this->AddAppToCaseInternal(caseData->Id(), L"Refresh", true, (void*)&pathListResult, NULL, (void*)pTemp, false);
		}
	}
}
void InstallCaseModelEx::RefreshUpgrateCase(InstallsCaseData* caseData)
{
	RefreshUpgrateCase(caseData,false);
}
void InstallCaseModelEx::RefreshUpgrateCase(InstallsCaseData* caseData,bool isOnlyShowLastVer)
{INTERFUN;

	if(caseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
	{
		CheckIOSUpgrateInternal();
		CheckIOSLegalUpgradeInternal();
	}
	else
	{
		CheckAndroidUpgrateInternal();
	}

	InstallsAppList* appList = new InstallsAppList();
	if (isOnlyShowLastVer)
	{
		GetLastVerAppList(caseData, appList);//相同版本把旧版本过滤了
	}
	else
	{	
		appList = caseData->ListApp();
	}
	for (InstallsAppList::iterator ne = appList->begin(); ne != appList->end(); ne++)
	{
		InstallsAppInfo* newAppInfo = *ne;

		InstallCaseMsg msg;
		msg.message = WM_CASE_APP_ADD;
		msg.wParam  = (WPARAM)caseData->Id();
		msg.lParam  = (LPARAM)newAppInfo;
		msg.SendToUI();
	}
}

void InstallCaseModelEx::LoadAppListInternal(int caseId, wstring type)
{INTERFUN;
	InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->LoadAppList(caseId);

	//SendMessage(Singleton::GetMainWnd(), WM_CASE_APP_LOADED, (WPARAM)caseId, NULL);

	InstallsAppList* appList = caseData->ListApp();
	for (InstallsAppList::iterator it = appList->begin(); it != appList->end(); it++)
	{
		InstallsAppInfo* appInfo = *it;

		if ( type != SUPPORTDEVICE_ALL && !appInfo->m_SupportDevice.empty() && !appInfo->m_SDKVersion.empty() )
		{
			if ( (int)CStrOperation::toLowerW(appInfo->m_SupportDevice).find(CStrOperation::toLowerW(type)) < 0 )
			{
				continue;
			}
		}

		InstallCaseMsg msg;
		msg.message = WM_CASE_APP_ADD;
		msg.wParam  = (WPARAM)caseId;
		msg.lParam  = (LPARAM)appInfo;
		msg.SendToUI();
	}

	if(caseData->PlanType() == _INSTALL_PLAN_TYPE_NET)
	{
		NetInstallsCaseData* netCase = (NetInstallsCaseData*)caseData;
		for (InstallsAppList::iterator it = netCase->NetAppItems->begin(); it != netCase->NetAppItems->end(); it++)
		{
			InstallsAppInfo* appInfo = *it;

			if ( type != SUPPORTDEVICE_ALL && !appInfo->m_SupportDevice.empty() )
			{
				if ( (int)CStrOperation::toLowerW(appInfo->m_SupportDevice).find(CStrOperation::toLowerW(type)) < 0 )
				{
					continue;
				}
			}
			
			InstallCaseMsg msg;
			msg.message = WM_CASE_APP_ADD;
			msg.wParam  = (WPARAM)caseId;
			msg.lParam  = (LPARAM)appInfo;
			msg.SendToUI();
		}
	}

	InstallCaseMsg msg;
	msg.message = WM_CASE_APP_EMPTY;
	msg.wParam  = (WPARAM)caseId;
	msg.lParam  = NULL;
	msg.SendToUI();

	if(!_bFirstiOSUPSearchFinish && caseId==0)
		_bFirstiOSUPSearchFinish = true;
	if(!_bFirstAndUPSearchFinish && caseId==1)
		_bFirstAndUPSearchFinish = true;
}

void InstallCaseModelEx::SearchAppInternal( void* pParam )
{INTERFUN;
	
	AppSearchContextEx* context = (AppSearchContextEx*)pParam;
	if (NULL == context)
	{
		return;
	}
	InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->GetCaseById(context->CaseID);
	if (NULL == caseData)
	{
		return;
	}
	if(caseData->Id()<4)
	{
		g_bResearch=true;
		SetCSLock(_csLockList[caseData->Id()]);
		g_bResearch=false;
		SearchAppInternalEx(context, caseData);
	}
	else
	{
		SearchAppInternalEx(context, caseData);
	}
	
}

void InstallCaseModelEx::SearchAppInternalEx( AppSearchContextEx* context, InstallsCaseData* caseData )
{
	//如果当前Context正在搜索，等待上次搜索退出
	while (context->IsSearching)
	{
		//取消上次搜索
		context->ExistLastSearch = true;

		Sleep(10);
	}
	
	InstallCaseMsg msg;
	msg.message = WM_CASE_CLEARAPPIST;
	msg.wParam  = (WPARAM)context->CaseID;
	msg.lParam  = (LPARAM)context;
	msg.SendToUI();

	//@zjf,这样写内存泄露
	//InstallsAppList* appList = new InstallsAppList();
	InstallsAppList* appList;
	if(context->ShowLastVer)
	{
		appList = new InstallsAppList();
		GetLastVerAppList(caseData, appList);
	}
	else
		appList = caseData->ListApp();

	//恢复搜索
	context->ExistLastSearch = false;

	//当前Context正在搜索
	context->IsSearching = true;

	
	//@zjf,修改内存泄露
	//InstallsAppList* resultList = new InstallsAppList();
	for(InstallsAppList::iterator it=appList->begin(); it!=appList->end();it++)
	{
		if (g_bResearch)
		{
			InstallCaseMsg msg;
			msg.message = WM_CASE_CLEARAPPIST;
			msg.wParam  = (WPARAM)context->CaseID;
			msg.lParam  = (LPARAM)context;
			msg.SendToUI();

			g_bResearch=false;
			break;
		}
		InstallsAppInfo * appinfo = *it;

		wstring supportDev = L"";
		if(context->PhoneType == 1)
			supportDev = L"iphone";
		else if(context->PhoneType == 2)
			supportDev = L"ipad";

		//是否取消
		if(context->ExistLastSearch)
		{
			//结果搜索，以便下次搜索顺利进行
			context->IsSearching = false;
			break;
		}
		InstallCaseMsg msg;
		msg.message = WM_CASE_APP_ADD;
		msg.wParam  = (WPARAM)context->CaseID;
		msg.lParam  = (LPARAM)(*it);
		msg.SendToUI();
	}

	msg.message = WM_CASE_SET_NUMBER;
	msg.wParam  = (WPARAM)caseData->Id();
	msg.lParam  = (LPARAM)caseData;
	msg.SendToUI();

	context->IsSearching = false;
	if(!_bFirstiOSUPSearchFinish && context->CaseID==0)
		_bFirstiOSUPSearchFinish = true;
	if(!_bFirstAndUPSearchFinish && context->CaseID==1)
		_bFirstAndUPSearchFinish = true;
}

void InstallCaseModelEx::GetLastVerAppList(InstallsCaseData* caseData,InstallsAppList* newAppList)
{INTERFUN;
	for(InstallsAppList::iterator it = caseData->ListApp()->begin(); it!= caseData->ListApp()->end(); it++)
	{
		
		InstallsAppInfo *appInfo = *it;
		bool findSameAppIndex = false;
		InstallsAppInfo *newApp = NULL;
		InstallsAppList::iterator newListIt;
		for(newListIt = newAppList->begin(); newListIt != newAppList->end();newListIt++ )
		{
			
			newApp = *newListIt;
			if(!newApp || newApp->m_bRemoving ||!appInfo ||  appInfo->m_bRemoving)
				continue;
			if(newApp->m_strPackageID == appInfo->m_strPackageID)
			{
				findSameAppIndex = true;
				break;
			}
		}
		
		if(findSameAppIndex)
		{
			//找到的app版本要比保存的新
			if(UIStringFormat::IsNewVersion(newApp->m_strVersion.c_str(),appInfo->m_strVersion.c_str()))
			{
				*newListIt = appInfo;
			}
		}
		else
		{
			newAppList->m_list.push_back(*it);
		}
	}
}

void InstallCaseModelEx::EditCaseInternal( int caseId, wstring caseName, void* pList )
{INTERFUN;

	vector<wstring>* pathList = (vector<wstring>*)pList;

	if (NULL == pathList)
	{
		return;
	}
	//删除
	vector<wstring> removePathList;
	InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->GetCaseById(caseId);
	if(caseData->CaseName() != caseName)
	{
		InstallCaseManagerEx::GetInstance()->EditCaseName(caseData, caseName);
		caseData->CaseName(caseName);

		InstallCaseMsg msg;
		msg.message = WM_CASE_SET_NUMBER;
		msg.wParam  = (WPARAM)caseData->Id();
		msg.lParam  = (LPARAM)caseData;
		msg.SendToUI();
	}
	InstallsAppList* appList = caseData->ListApp();
	for (InstallsAppList::iterator it = appList->begin(); it != appList->end(); it++)
	{
		InstallsAppInfo* appInfo = *it;

		bool find = false;
		for(vector<wstring>::iterator sit = pathList->begin(); sit != pathList->end(); sit++)
		{
			
			if(CStrOperation::CompareUnCase(appInfo->m_strSavePath, *sit) == 0)
			{
				find = true;
				break;
			}
		}
		if(!find)
			removePathList.push_back(appInfo->m_strSavePath);
	}

	if(removePathList.size() > 0)
	{
		this->RemoveAppFromCaseInternal(caseId, caseName, true, (void*)&removePathList);
	}

	//添加
	vector<wstring> addPathList;
	for (vector<wstring>::iterator it = pathList->begin(); it != pathList->end(); it++)
	{
		wstring path = *it;
		bool find = false;
		for (InstallsAppList::iterator sit = appList->begin(); sit != appList->end(); sit++)
		{
			InstallsAppInfo* appInfo = *sit;
			if(CStrOperation::CompareUnCase(appInfo->m_strSavePath, path) == 0)
			{
				find = true;
				break;
			}
		}
		if(!find)
			addPathList.push_back(path);
	}

	if(addPathList.size() > 0)
	{
		this->AddAppToCaseInternal(caseId, caseName, false, (void*)&addPathList);
	}
}

void InstallCaseModelEx::AddAppToCaseInternal( int caseId, wstring isRefresh, bool isPathList, void* pVoidParam, \
											  void* pWstring, void* pInt,bool isShowNotify )
{INTERFUN;
	
	vector<wstring>* pvec_wstring = (vector<wstring>*)pWstring;
	vector<int>* pvec_int = (vector<int>*)pInt;

	//StatisticHelp stathelp;
	InstallsCaseData* caseData =  InstallCaseManagerEx::GetInstance()->GetCaseById(caseId);

	int  addAppCount = 0;
	//通过路径添加至方案
	if(isPathList)
	{
		vector<AppCountByFolder*> oldFolders;
		oldFolders.insert(oldFolders.end(), InstallCaseManagerEx::GetInstance()->AppFolderList.begin(), InstallCaseManagerEx::GetInstance()->AppFolderList.end());

		vector<wstring>* pathList = (vector<wstring>*)pVoidParam;
		if((pathList == NULL || pathList->size() <= 0) && NULL == pvec_wstring || (pvec_wstring && pvec_wstring <= 0))
			return;

		if( pvec_wstring && pvec_wstring->size() >0)
		{
			vector<AppCountByFolder*> newFolderList = InstallCaseManagerEx::GetInstance()->AppFolderList;
			for (vector<AppCountByFolder*>::iterator it = newFolderList.begin(); it != newFolderList.end(); it++)
			{
				AppCountByFolder* folder =  *it;
				folder->bHasChanged = true;
			}
			if(pvec_wstring && L"0" == pvec_wstring->at(1))
			{
				InstallCaseManagerEx::GetInstance()->AddFolder(pvec_wstring->at(0), _INSTALLS_CASE_DATA_IOS);
			}
			if(pvec_wstring && L"0" == pvec_wstring->at(3))
			{
				InstallCaseManagerEx::GetInstance()->AddFolder(pvec_wstring->at(2), _INSTALLS_CASE_DATA_ANDROID);
			}
		}

		bool isShowLaserVer = false;
		if (pvec_int && pvec_int->size() >0)
		{
			if (pvec_int->at(0) == 1)
			{
				isShowLaserVer = true;
			}
		}

		InstallsAppList* appList = new InstallsAppList();
		if (isShowLaserVer)
		{
			GetLastVerAppList(caseData, appList);
		}
		
		for (vector<wstring>::iterator it = pathList->begin(); it != pathList->end(); it++)
		{
			wstring path = *it;
			
			bool isAddView = false;
			if (isShowLaserVer)
			{
				for(InstallsAppList::iterator it2=appList->begin(); it2!=appList->end();it2++)
				{
					InstallsAppInfo * appinfo = *it2;
					if (appinfo->m_strSavePath.compare(path) == 0)
					{
						isAddView = true;
						break;
					}
				}
				if (0==appList->size())
				{
					isAddView = true;
				}
			}
			else
			{
				isAddView = true;
			}
			if (!isAddView&&IsAppExistInCase(caseData,path))
			{
				InstallsAppInfo* planAppInfo = caseData->GetAppBySavePath(path);
				
				if(planAppInfo)
				{
					InstallCaseMsg msg;
					msg.message = WM_CASE_APP_REMOVE;
					msg.wParam  = (WPARAM)caseData->Id();
					msg.lParam  = (LPARAM)planAppInfo->m_id;
					msg.SendToUI();
				}
			}

			if(caseId < 0)
			{
				wstring ext= CFileOperation::GetFileExt(path);
				ext=CStrOperation::toLowerW(ext);

				if(ext == L"ipa" || ext == L"pxl")
					caseData = InstallCaseManagerEx::GetInstance()->GetSystemCaseData(_INSTALLS_CASE_DATA_IOS);
				else if(ext == L"apk" || ext == L"npk")
					caseData = InstallCaseManagerEx::GetInstance()->GetSystemCaseData(_INSTALLS_CASE_DATA_ANDROID);
				else
					continue;
			}
			
			if(IsAppExistInCase(caseData, path))
			{
				continue;
			}
			InstallsAppInfo* appInfo = InstallCaseManagerEx::GetInstance()->AddAppToCaseByPath(caseData, path, isRefresh == L"Refresh");
			if(appInfo == NULL )
			{
				wstring info = L"文件" + path;
				info += L"解析失败！";
				Notice(info, 1000);
				continue;
			}

			if (isShowLaserVer)
			{
				for(InstallsAppList::iterator it2=appList->begin(); it2!=appList->end();it2++)
				{
					InstallsAppInfo * appinfoTmp = *it2;
					if (appinfoTmp->m_strPackageID == appInfo->m_strPackageID)
					{
						isAddView = false;
						break;
					}
				}
				if (0==appList->size())
				{
					isAddView = true;
				}
			}
			PostInstallPlanListMsg postInstallPlanListMsg;
			postInstallPlanListMsg.pAppInfo = (void *)appInfo;
			postInstallPlanListMsg.Send();
			//stathelp.PostInstallPlanList(appInfo);
			addAppCount++;
			if (isAddView)
			{
				InstallCaseMsg msg;
				msg.message = WM_CASE_APP_ADD;
				msg.wParam  = (WPARAM)(caseId > 0 ? caseId:caseData->Id());
				msg.lParam  = (LPARAM)appInfo;
				msg.SendToUI();
			}

			InstallCaseMsg msg;
			msg.message = WM_CASE_SET_NUMBER;
			msg.wParam  = (WPARAM)(caseId > 0 ? caseId:caseData->Id());
			msg.lParam  = (LPARAM)caseData;
			msg.SendToUI();

			//如果为向装机方案装添加数据，则需要主动触发向软件库添加数据的事件
			if(caseData->PlanType() != _INSTALL_PLAN_TYPE_SYS)
			{
				InstallsCaseData* sysCaseData = InstallCaseManagerEx::GetInstance()->GetSystemCaseData(caseData->CaseType());
				InstallsAppInfo* libAppInfo = sysCaseData->GetAppBySavePath(appInfo->m_strSavePath);
				if(!libAppInfo)
					continue;

				PostInstallPlanListMsg postInstallPlanListMsg2;
				postInstallPlanListMsg2.pAppInfo = (void *)libAppInfo;
				postInstallPlanListMsg2.Send();

				//stathelp.PostInstallPlanList(libAppInfo);

				InstallCaseMsg msg;
				msg.message = WM_CASE_APP_ADD;
				msg.wParam  = (WPARAM)sysCaseData->Id();
				msg.lParam  = (LPARAM)libAppInfo;
				msg.SendToUI();

				msg.message = WM_CASE_SET_NUMBER;
				msg.wParam  = (WPARAM)sysCaseData->Id();
				msg.lParam  = (LPARAM)sysCaseData;
				msg.SendToUI();
	
			}
		}
		if (isRefresh == _T("DownLoadSystemSetUI"))
			isShowNotify = false;

		if (isShowNotify)
		{
			InstallCaseMsg msg;
			msg.message = WM_CASE_APP_ADD_RESULT;
			msg.wParam  = (WPARAM)addAppCount;
			msg.lParam  = (LPARAM)NULL;
			msg.SendToUI();
		}
		
		if(caseData)
		{
			PostZJAppNumOfAnPlanMsg postZjAppNumOfAnPlanMsg;
			postZjAppNumOfAnPlanMsg.num = caseData->GetAppNum();
			postZjAppNumOfAnPlanMsg.Send();
			//stathelp.PostZJAppNumOfAnPlan(caseData->GetAppNum());

			PostZJInstallDurationMsg postZJInstallDurationMsg;
			postZJInstallDurationMsg.num = caseData->GetAppNum();
			postZJInstallDurationMsg.allSize = caseData->GetSumFileSize();
			postZJInstallDurationMsg.Send();
			//stathelp.PostZJInstallDuration(caseData->GetAppNum(), caseData->GetSumFileSize());

			PostInstallPlanMsg postInstallPlanMsg;
			postInstallPlanMsg.num =  caseData->GetAppNum();
			postInstallPlanMsg.size = caseData->GetSumFileSize();
			postInstallPlanMsg.Send();
			//stathelp.PostInstallPlan(caseData->GetSumFileSize(), caseData->GetAppNum());
		}
		CheckFolderChanged(&oldFolders);
	}
	//从软件库中选中软件添加至装机方案
	else
	{
		vector<wstring>* pathList = (vector<wstring>*)pVoidParam;
		if(pathList == NULL || pathList->size() <= 0)
			return;

		for (vector<wstring>::iterator it = pathList->begin(); it != pathList->end(); it++)
		{
			if(IsAppExistInCase(caseData, *it))
				continue;
			InstallsAppInfo* appInfo = InstallCaseManagerEx::GetInstance()->AddAppToCase(caseData, *it);
			if(appInfo == NULL)
				continue;

			//stathelp.PostInstallPlanList(appInfo);
			PostInstallPlanListMsg postInstallPlanListMsg3;
			postInstallPlanListMsg3.pAppInfo = (void *)appInfo;
			postInstallPlanListMsg3.Send();
			addAppCount++;
			
			InstallCaseMsg msg;
			msg.message = WM_CASE_APP_ADD;
			msg.wParam  = (WPARAM)caseId;
			msg.lParam  = (LPARAM)appInfo;
			msg.SendToUI();
			
			msg.message = WM_CASE_SET_NUMBER;
			msg.wParam  = (WPARAM)caseId;
			msg.lParam  = (LPARAM)caseData;
			msg.SendToUI();

		}	
		if (isShowNotify)
		{
			InstallCaseMsg msg;
			msg.message = WM_CASE_APP_ADD_RESULT;
			msg.wParam  = (WPARAM)addAppCount;
			msg.lParam  = (LPARAM)NULL;
			msg.SendToUI();
		}

		PostZJAppNumOfAnPlanMsg postZjAppNumOfAnPlanMsg2;
		postZjAppNumOfAnPlanMsg2.num = caseData->GetAppNum();
		postZjAppNumOfAnPlanMsg2.Send();
		//stathelp.PostZJAppNumOfAnPlan(caseData->GetAppNum());

		PostZJInstallDurationMsg postZJInstallDurationMsg2;
		postZJInstallDurationMsg2.num = caseData->GetAppNum();
		postZJInstallDurationMsg2.allSize = caseData->GetSumFileSize();
		postZJInstallDurationMsg2.Send();
		//stathelp.PostZJInstallDuration(caseData->GetAppNum(), caseData->GetSumFileSize());

		PostInstallPlanMsg postInstallPlanMsg2;
		postInstallPlanMsg2.num =  caseData->GetAppNum();
		postInstallPlanMsg2.size = caseData->GetSumFileSize();
		postInstallPlanMsg2.Send();
		//stathelp.PostInstallPlan(caseData->GetSumFileSize(), caseData->GetAppNum());
	}

}

void InstallCaseModelEx::CheckFolderChanged(vector<AppCountByFolder*>* folderList)
{INTERFUN;
	vector<AppCountByFolder*> newFolderList = InstallCaseManagerEx::GetInstance()->AppFolderList;
	if(folderList != NULL && folderList->size() > 0)
	{
		for (vector<AppCountByFolder*>::iterator it = folderList->begin(); it != folderList->end(); it++)
		{
			//folderList里不存在的列表已经移除
			vector<AppCountByFolder*>::iterator findIt = std::find(newFolderList.begin(), newFolderList.end(), *it);
			if(findIt == newFolderList.end())
			{
				InstallCaseMsg msg;
				msg.message = WM_CASE_FOLDER_REMOVE;
				msg.wParam  = (WPARAM)*it;
				msg.lParam  = NULL;
				msg.SendToUI();

			}
		}
	}
	for (vector<AppCountByFolder*>::iterator it = newFolderList.begin(); it != newFolderList.end(); it++)
	{
		AppCountByFolder* folder =  *it;
		
		InstallCaseMsg msg;
		msg.message = WM_CASE_FOLDER_ADD;
		msg.wParam  = (WPARAM)folder;
		msg.lParam  = NULL;
		msg.SendToUI();

		folder->bHasChanged = false;
	}
}

void InstallCaseModelEx::RemoveCaseInternal( void* pBaseParam )
{INTERFUN;
	vector<int>* idList = (vector<int>*)pBaseParam;
	if (NULL == idList)
	{
		return;
	}
	for (vector<int>::iterator it = idList->begin(); it != idList->end(); it++)
	{
		//暂时先删除后再删除界面， 不然会在首页装机方案有bug，之后首页装机方案那边重构一下，再改过来
		int caseId = *it;
		
		InstallCaseMsg msg;
		msg.message = WM_CASE_REMOVE;
		msg.wParam  = (WPARAM)caseId;
		msg.lParam  = NULL;
		msg.SendToUI();

		bool remove = InstallCaseManagerEx::GetInstance()->RemoveCase(caseId);
		if(!remove)
			continue;
		
		msg.message = WM_CASE_REMOVE;
		msg.wParam  = (WPARAM)caseId;
		msg.lParam  = NULL;
		msg.SendToUI();

	}
}

void InstallCaseModelEx::RemoveAppFromCaseInternal( int caseId, wstring isRefresh, bool removeFile, void* pBaseParam, bool isUpdate )
{INTERFUN;
	vector<wstring>* pathList = (vector<wstring>*)pBaseParam;
	InstallsCaseData* caseData =  InstallCaseManagerEx::GetInstance()->GetCaseById(caseId);

	if (NULL == pathList || NULL == caseData)
	{
		return;
	}
	vector<AppCountByFolder*> oldFolders;
	oldFolders.insert(oldFolders.end(), InstallCaseManagerEx::GetInstance()->AppFolderList.begin(), InstallCaseManagerEx::GetInstance()->AppFolderList.end());
	//移除空软件库文件夹单独处理
	if(pathList->size() == 0 && isRefresh == L"FolderRemove")
	{
		InstallCaseManagerEx::GetInstance()->RemoveFolder(caseId);
		CheckFolderChanged(&oldFolders);
	}

	for (vector<wstring>::iterator it = pathList->begin(); it != pathList->end(); it++)
	{
		wstring appPath = *it;
		InstallsAppInfo* appInfo = caseData->GetAppBySavePath(appPath);
		//zhengjx 升级时从软件库删除旧版本文件，有可能软件库文件已经被删除
		if(!appInfo && !isUpdate)
			continue;
		int appId = -1;
		if(appInfo)
			appId = appInfo->m_id;

		//如果是系统默认方案，删除本地文件并且移除其它方案中的该软件
		if(caseData->PlanType() == _INSTALL_PLAN_TYPE_SYS)
		{
			bool isFolderRemove = isRefresh == L"FolderRemove";
			if(removeFile && appInfo && CFileOperation::IsFileExist(appInfo->m_strSavePath.c_str()) && (!isUpdate || CONFIG(_bAutoDelUpdatedApp)))
			{
				CFileOperation::DeleteFileW(appInfo->m_strSavePath);
			}

			//如果不是删除文件夹，则要同步删除其它方案中的软件，否则保留
			if(!isFolderRemove)
			{
				vector<InstallsCaseData*> caseList =  InstallCaseManagerEx::GetInstance()->InstallCaseDataList;
				for (vector<InstallsCaseData*>::iterator caseit = caseList.begin(); caseit != caseList.end(); caseit++)
				{
					InstallsCaseData* tmp = *caseit;
					if( tmp->CaseType() != caseData->CaseType())
						continue;
					if( tmp->PlanType() == _INSTALL_PLAN_TYPE_SYS && !CONFIG(_bAutoDelUpdatedApp) && isUpdate)
						continue;
					InstallsAppInfo* planAppInfo = tmp->GetAppBySavePath(appPath);
					
					if(planAppInfo)
					{
						InstallCaseMsg msg;
						msg.message = WM_CASE_APP_REMOVE;
						msg.wParam  = (WPARAM)tmp->Id();
						msg.lParam  = (LPARAM)planAppInfo->m_id;
						msg.SendToUI();
					}
					
					bool remove = InstallCaseManagerEx::GetInstance()->RemoveAppFromCaseByPath(tmp, appPath);
					if(!remove)
						continue;
					
					InstallCaseMsg msg;
					msg.message = WM_CASE_SET_NUMBER;
					msg.wParam  = (WPARAM)tmp->Id();
					msg.lParam  = (LPARAM)tmp;
					msg.SendToUI();
				}
			}
			else
			{
				if(caseData->Id()<4)
				{
					SetCSLock(_csLockList[caseData->Id()]);
					
					InstallCaseMsg msg;
					msg.message = WM_CASE_APP_REMOVE;
					msg.wParam  = (WPARAM)caseData->Id();
					msg.lParam  = (LPARAM)appId;
					msg.SendToUI();
					
					bool remove = InstallCaseManagerEx::GetInstance()->RemoveAppFromCaseByPath(caseData, appPath);
					if(!remove)
						continue;
					
					msg.message = WM_CASE_SET_NUMBER;
					msg.wParam  = (WPARAM)caseData->Id();
					msg.lParam  = (LPARAM)caseData;
					msg.SendToUI();
				}
				else
				{
					InstallCaseMsg msg;
					msg.message = WM_CASE_APP_REMOVE;
					msg.wParam  = (WPARAM)caseData->Id();
					msg.lParam  = (LPARAM)appId;
					msg.SendToUI();
					
					bool remove = InstallCaseManagerEx::GetInstance()->RemoveAppFromCaseByPath(caseData, appPath);
					if(!remove)
						continue;
					
					msg.message = WM_CASE_SET_NUMBER;
					msg.wParam  = (WPARAM)caseData->Id();
					msg.lParam  = (LPARAM)caseData;
					msg.SendToUI();
				}
			}
			
			CheckFolderChanged(&oldFolders);
		}
		else
		{
			InstallCaseMsg msg;
			msg.message = WM_CASE_APP_REMOVE;
			msg.wParam  = (WPARAM)caseId;
			msg.lParam  = (LPARAM)appId;
			msg.SendToUI();

			bool remove = InstallCaseManagerEx::GetInstance()->RemoveAppFromCaseByPath(caseData, appPath);
			if(!remove)
				continue;
			
			msg.message = WM_CASE_SET_NUMBER;
			msg.wParam  = (WPARAM)caseId;
			msg.lParam  = (LPARAM)caseData;
			msg.SendToUI();
		}
	}
}

void InstallCaseModelEx::UpgrateAppInternal(int caseId, void* pBaseParam)
{INTERFUN;
	vector<wstring>* pathList = (vector<wstring>*)pBaseParam;

	InstallsCaseData* caseData =  InstallCaseManagerEx::GetInstance()->GetCaseById(caseId);
	if (NULL == pathList || NULL == caseData)
	{
		return;
	}
	vector<AppUpdateInfo*> updateapps;
	for (vector<wstring>::iterator it = pathList->begin(); it != pathList->end(); it++)
	{
		wstring path=  *it;
		InstallsAppInfo* appInfo = caseData->GetAppBySavePath(path);
		if(appInfo == NULL)
			continue;
		AppUpdateInfo* updateappinfo = new AppUpdateInfo();
		updateappinfo->_nfid = appInfo->m_fid;
		updateappinfo->_url = appInfo->m_strDownLoadURL;
		updateappinfo->_name = appInfo->m_strName;
		updateappinfo->_newVersion = appInfo->m_strNewVersion;
		updateappinfo->_localPath = path;
		updateapps.push_back(updateappinfo);
	}
	CGetAppUpdateInfo *getrealurls = new CGetAppUpdateInfo((int)caseData->CaseType());
	bool Geturlsresult = false;
	Geturlsresult = getrealurls->GetAppsRealUpdateUrls( &updateapps );
	if(Geturlsresult)
	{
		vector<int> hasChangeFids;
		for(vector<AppUpdateInfo*>::iterator it = updateapps.begin();it != updateapps.end();it++)
		{
			AppUpdateInfo* appInfo = *it;
			bool hasSameFid = false;
			for(vector<AppUpdateInfo*>::iterator itTemp = updateapps.begin();itTemp != updateapps.end();itTemp++)
			{
				AppUpdateInfo* appInfoTemp = *itTemp;
				if(appInfoTemp->_nfid == appInfo->_nfid && appInfoTemp->_url != appInfo->_url)
				{
					vector<int>::iterator tempfid = std::find(hasChangeFids.begin(), hasChangeFids.end(), appInfoTemp->_nfid);
					if (tempfid == hasChangeFids.end())
					{
						hasSameFid = true;
						appInfoTemp->_url = appInfo->_url;
					}
				}
			}
			if(hasSameFid)
				hasChangeFids.push_back(appInfo->_nfid);
			InstallsAppInfo* caseInfo= caseData->GetAppBySavePath(appInfo->_localPath);//
			if (caseInfo == NULL)
			{
				caseInfo = caseData->GetAppByName(appInfo->_name);
			}
			wstring fidstr;
			WebResModel::ParseDownUrl(appInfo->_url,caseInfo->m_strName,caseInfo->m_strNewVersion,fidstr);
			string response = CNetOperation::Get(CCodeOperation::WstringToString(appInfo->_url),false);
			int pos = response.find("<a href=");
			if(pos>-1)
			{
				string str1=response.substr(pos);
				pos=str1.find(">");
				appInfo->_url=CCodeOperation::StringToWstring(str1.substr(9,pos-10));
			}
			wstring platform = caseData->CaseType() ==_INSTALLS_CASE_DATA_IOS?L"iPhone":L"Android";
			wstring savepath = CFileOperation::GetFileDir(caseInfo->m_strSavePath);
			wstring appfullpath = caseInfo->m_strSavePath;
			TASK_UPGRADE_TYPE upgradetype = PIRACY2PIRACY;
			if(L"iPhone"==platform)
			{
				if(caseInfo->m_isLegalSoft)
					upgradetype=LEGAL2PIRACY;
			}
			DownloadTaskItemMsg DownloadMsg;
			DownloadMsg.url = appInfo->_url;
			DownloadMsg.savepath = savepath;
			DownloadMsg.appname = caseInfo->m_strName;
			DownloadMsg.version = caseInfo->m_strNewVersion;
			DownloadMsg.platform = platform;
			DownloadMsg.fid = _wtoi(fidstr.c_str());
			DownloadMsg.bUpdategrade = true;
			DownloadMsg.needInstall = false;
			DownloadMsg.oldappfullpath = appfullpath;
			DownloadMsg.isLegalSoft = false; 
			DownloadMsg.taskupgradetype = (int)upgradetype;
			DownloadMsg.appID = appInfo->_identifier;
			DownloadMsg.SendAndGetReturn();
		}
	}
}

void InstallCaseModelEx::UpgradeAppToLegalInternal(int caseId, void* pBaseParam)
{INTERFUN;
	vector<wstring>* pathList = (vector<wstring>*)pBaseParam;

	if (NULL == pathList)
	{
		return;
	}
	InstallsCaseData* caseData =  InstallCaseManagerEx::GetInstance()->GetCaseById(caseId);
	if (NULL == caseData)
	{
		return;
	}
	vector<AppUpdateInfo*> updateapps;
	for (vector<wstring>::iterator it = pathList->begin(); it != pathList->end(); it++)
	{
		wstring path=  *it;
		InstallsAppInfo* appInfo = caseData->GetAppBySavePath(path);
		if(appInfo == NULL)
			continue;
		AppUpdateInfo* updateappinfo = new AppUpdateInfo();
		//正版_nfid就是m_itemID(因为appInfo的m_fid在盗版升级的时候需要，故这里不能使用)
		updateappinfo->_nfid = appInfo->m_itemID;
		updateappinfo->_url = appInfo->m_strLegalDownLoadURL;
		updateappinfo->_name = appInfo->m_strName;
		updateappinfo->_newVersion = appInfo->m_strNewLegalVersion;
		updateappinfo->_localPath = path;
		updateapps.push_back(updateappinfo);
	}
	for(vector<AppUpdateInfo*>::iterator it = updateapps.begin();it != updateapps.end();it++)
	{
		AppUpdateInfo* appInfo = *it;
		InstallsAppInfo* caseInfo= caseData->GetAppBySavePath(appInfo->_localPath);
		int fidstr = appInfo->_nfid;
		wstring downurl = appInfo->_url;
		int legalindex = downurl.find(L"itunesapp2");
		if(legalindex>0)
		{
			string content=CNetOperation::Get(CCodeOperation::UnicodeToGB2312(downurl),false);
			int pos= content.find("href=");
			CONTINUE_IF(pos<0);
			content =content.substr(pos+6);
			int pos1=content.find("'");
			CONTINUE_IF(pos1<0);
			content =content.substr(0,pos1);
			content = CNetOperation::Get(content,false);
			pos = content.find("href=\"");
			CONTINUE_IF(pos<0);
			content =content.substr(pos+6);
			pos1=content.find("\"");
			CONTINUE_IF(pos1<0);
			content=content.substr(0,pos1);
			downurl = CCodeOperation::GB2312ToUnicode(content);
		}
		else
			continue;
		wstring platform = L"iPhone";
		wstring savepath = CFileOperation::GetFileDir(caseInfo->m_strSavePath);
		wstring appfullpath = caseInfo->m_strSavePath;
		TASK_UPGRADE_TYPE upgradetype = LEGAL2LEGAL;
		if(!appInfo->_bLegal)
			upgradetype=PIRACY2LEGAL;
		DownloadTaskItemMsg DownloadMsg;
		DownloadMsg.url = downurl;
		DownloadMsg.savepath = savepath;
		DownloadMsg.appname = appInfo->_name;
		DownloadMsg.version = appInfo->_newVersion;
		DownloadMsg.platform = platform;
		DownloadMsg.fid = fidstr;
		DownloadMsg.bUpdategrade = true;
		DownloadMsg.needInstall = false;
		DownloadMsg.oldappfullpath = appfullpath;
		DownloadMsg.isLegalSoft = true; 
		DownloadMsg.taskupgradetype = (int)upgradetype;
		DownloadMsg.appID = appInfo->_identifier;
		DownloadMsg.SendAndGetReturn();
	}
}


void InstallCaseModelEx::InstallAppInternal(int caseId, wstring deviceId, void* pBaseParam)
{INTERFUN;
	vector<wstring>* pathList = (vector<wstring>*)pBaseParam;

	if (NULL == pathList)
	{
		return;
	}

	RequestInstallPositionMsg requestInstallPositionMsg(deviceId);
	requestInstallPositionMsg.SendAndGetReturn();

	//wstring strInstall = MyFileOperation::GetIniFileValue(CONFIG(_strOptionIni), _T("Android"), _T("installonphone"));
	bool bOnSDCard = requestInstallPositionMsg.bInstallOnPhone;//(strInstall == L"0" ? true : false);

	InstallsCaseData* caseData =  InstallCaseManagerEx::GetInstance()->GetCaseById(caseId);

	if (NULL == caseData)
	{
		return;
	}
	if(caseData->GetAppNum() == pathList->size() && caseData->PlanType() == _INSTALL_PLAN_TYPE_USER)
	{
		int count = caseData->InstallCount();
		count++;
		caseData->InstallCount(count);
		time_t now;	
		time(&now);
		caseData->lastUseTime = now;
		InstallCaseManagerEx::GetInstance()->IncCaseInstallCount(caseData);
		
		InstallCaseMsg msg;
		msg.message = WM_CASE_TIMECHANGE;
		msg.wParam  = (WPARAM)caseData->Id();
		msg.lParam  = (LPARAM)caseData;
		msg.SendToUI();
	}

	for (vector<wstring>::iterator it = pathList->begin(); it != pathList->end(); it++)
	{
		wstring path = *it;
		InstallsAppInfo* appInfo = caseData->GetAppBySavePath(path);
		if(appInfo == NULL)
			continue;

		InstallTaskItemMsg Installmsg;
		Installmsg.appname = appInfo->m_strName;
		Installmsg.appfilepath = appInfo->m_strSavePath;
		Installmsg.version = appInfo->m_strVersion;
		Installmsg.filesize = appInfo->m_FileSize;
		Installmsg.deviceId = deviceId;
		Installmsg.onSDcard = bOnSDCard;
		Installmsg.plat = L"";
		Installmsg.fid  = appInfo->m_fid;
		Installmsg.casename = CStrOperation::IntToWString(caseData->Id());
		Installmsg.icon = appInfo->m_Iconpath;
		Installmsg.isLegalSoft = appInfo->m_isLegalSoft==1;
		Installmsg.SendAndGetReturn();
	}

	Notice(CommonUIFunc::LoadStringW(_T("InstallCase_Text_HasbeenAddToTaskList")), 2000);
}

//zhengjx
void InstallCaseModelEx::AddFilesToLibInternal(void* pParam, void* pSpevparam)
{INTERFUN;

	if(NULL != pParam)
	{
		InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->GetSystemCaseData(_INSTALLS_CASE_DATA_IOS);
		AddAppToCaseInternal(caseData->Id(), L"", true, pParam);
	}

	if(NULL != pSpevparam)
	{
		InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->GetSystemCaseData(_INSTALLS_CASE_DATA_ANDROID);
		AddAppToCaseInternal(caseData->Id(), L"", true, pSpevparam);	
	}
}

//----------------------------------------------------------------------- 线程处理 End   -----------------------------------------------------------------------


//----------------------------------------------------------------------- 私有方法 Begin -----------------------------------------------------------------------

void InstallCaseModelEx::LoadCaseList()
{INTERFUN;
	vector<InstallsCaseData*> caseList =  InstallCaseManagerEx::GetInstance()->InstallCaseDataList;
	for (vector<InstallsCaseData*>::iterator it = caseList.begin(); it != caseList.end(); it++)
	{
		InstallsCaseData* caseData = *it;
		
		InstallCaseMsg msg;
		msg.message = WM_CASE_ADD;
		msg.wParam  = (WPARAM)caseData;
		msg.lParam  = NULL;
		msg.SendToUI();
	}
}

InstallsCaseData*  InstallCaseModelEx::CreateCase( wstring caseName, _INSTALLS_CASE_DATA_TYPE caseType )
{INTERFUN;
	InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->CreateCase(caseName, caseType);
	InstallCaseMsg msg;
	msg.message = WM_CASE_ADD;
	msg.wParam  = (WPARAM)caseData;
	msg.lParam  = NULL;
	msg.SendToUI();

	return caseData;
}

void InstallCaseModelEx::CreateCaseFromLib( _INSTALLS_CASE_DATA_TYPE caseType )
{INTERFUN;
	//从库中选择创建方案
	wstring caseName = GetNewDefaultCaseName(caseType);
	vector<wstring>* pathList = new vector<wstring>();
	_pInterface->GetCaseInfoFromLib(caseName, caseType, pathList);
	if(caseName.empty())
	{
		return;
	}

	InstallsCaseData* caseData = CreateCase(caseName, caseType);
	if(caseData == NULL)
		return;

	Notice(CommonUIFunc::LoadStringW(_T("方案已创建！")), 2000);

	if(pathList != NULL && pathList->size() > 0)
	{
		this->AddAppToCase(caseData->Id(), pathList);
	}
}

void InstallCaseModelEx::CreateCaseFromFolder(_INSTALLS_CASE_DATA_TYPE caseType)
{INTERFUN;
	//从文件夹方创建方案
	wstring caseName = GetNewDefaultCaseName(caseType);
	wstring folder;

	_pInterface->GetCaseInfoFromFolder(caseName, caseType, folder);

	if(folder.empty())
		return;

	InstallsCaseData* caseData = CreateCase(caseName, caseType);
	if(caseData == NULL)
		return;

	Notice(CommonUIFunc::LoadStringW(_T("方案已创建！")), 2000);

	vector<wstring> pathList;
	if(caseType == _INSTALLS_CASE_DATA_IOS)
	{
		vector<wstring> subFiles;
		CFileOperation::FindAllFileInDir(folder, subFiles, L"ipa");
		pathList.insert(pathList.end(), subFiles.begin(), subFiles.end());

		subFiles.clear();
		CFileOperation::FindAllFileInDir(folder, subFiles, L"pxl");
		pathList.insert(pathList.end(), subFiles.begin(), subFiles.end());
	}
	else
	{
		vector<wstring> subFiles;
		CFileOperation::FindAllFileInDir(folder, subFiles, L"apk");
		pathList.insert(pathList.end(), subFiles.begin(), subFiles.end());

		subFiles.clear();
		CFileOperation::FindAllFileInDir(folder, subFiles, L"npk");
		pathList.insert(pathList.end(), subFiles.begin(), subFiles.end());
	}

	if(pathList.size() > 0)
		this->AddAppToCaseByPath(caseData->Id(), &pathList);
}

void InstallCaseModelEx::CreateCaseByAppList( _INSTALLS_CASE_DATA_TYPE caseType, vector<wstring>* pathList)
{INTERFUN;
	wstring caseName = this->GetNewDefaultCaseName(caseType);
	_pInterface->GetCaseName(caseName);
	if(caseName.empty())
	{
		return;
	}

	InstallsCaseData* caseData = CreateCase(caseName, caseType);
	if(caseData == NULL)
		return;

	Notice(CommonUIFunc::LoadStringW(_T("方案已创建！")), 2000);

	this->AddAppToCase(caseData->Id(), pathList);
}

void InstallCaseModelEx::LoadDefaultFolder()
{INTERFUN;
	wstring filePath = PublicFunc::GetDocmentDataPath();
	map<wstring,_INSTALLS_CASE_DATA_TYPE> folderList;
	folderList[filePath+L"\\91 Mobile\\iPhone\\DownloadSavePath\\SoftwarePath"]=_INSTALLS_CASE_DATA_IOS;
	folderList[filePath+L"\\91 PC Suite\\DownloadCenter\\Software\\IPAD"]=_INSTALLS_CASE_DATA_IOS;
	folderList[filePath+L"\\91 PC Suite\\DownloadCenter\\Software\\IPHONE"]=_INSTALLS_CASE_DATA_IOS;
	folderList[filePath+L"\\My Music\\iTunes\\iTunes Media\\Mobile Applications"]=_INSTALLS_CASE_DATA_IOS;
	folderList[L"C:\\Users\\Administrator\\Music\\iTunes\\iTunes Media\\Mobile Applications"]=_INSTALLS_CASE_DATA_IOS;
	folderList[filePath+L"\\Tongbu\\PublicStaging"]=_INSTALLS_CASE_DATA_IOS;
	folderList[filePath+L"\\Mobile\\Android\\Download Packages"]=_INSTALLS_CASE_DATA_ANDROID;
	folderList[filePath+L"\\91 PC Suite\\DownloadCenter\\Software\\ANDROID"]=_INSTALLS_CASE_DATA_ANDROID;
	folderList[filePath+L"\\Wandoujia\\Download\\App"]=_INSTALLS_CASE_DATA_ANDROID;
	folderList[filePath+L"\\iMolo for Android\\软件"]=_INSTALLS_CASE_DATA_ANDROID;
	folderList[L"D:\\downloads\\apps"]=_INSTALLS_CASE_DATA_IOS;
	folderList[filePath+L"\\Tencent\\QQPhoneManager\\Application"]=_INSTALLS_CASE_DATA_ANDROID;
	folderList[L"D:\\360Downloads\\Apk"]=_INSTALLS_CASE_DATA_ANDROID;
	folderList[L"D:\\360Downloads\\IPA"]=_INSTALLS_CASE_DATA_IOS;

	for(map<wstring,_INSTALLS_CASE_DATA_TYPE>::iterator it=folderList.begin();
		it!=folderList.end();it++)
	{
		int caseId;
		vector<wstring> pathList;
		if(it->second == _INSTALLS_CASE_DATA_IOS)
		{
			vector<wstring> files;
			vector<wstring> filesNext;
			CFileOperation::FindAllFileInDir(it->first,	files,	_T("ipa"));
			CFileOperation::FindAllFileInDir(it->first,	filesNext,	_T("pxl"));
			pathList.insert(pathList.end(), files.begin(), files.end());
			pathList.insert(pathList.end(), filesNext.begin(), filesNext.end());

			caseId = Sys_Case_IOS_ID;
		}
		else
		{
			vector<wstring> files;
			vector<wstring> filesNext;
			CFileOperation::FindAllFileInDir(it->first, files, _T("apk"));
			CFileOperation::FindAllFileInDir(it->first, filesNext, _T("npk"));
			pathList.insert(pathList.end(), files.begin(), files.end());
			pathList.insert(pathList.end(), filesNext.begin(), filesNext.end());

			caseId = Sys_Case_Android_ID;
		}

		this->AddAppToCaseInternal(caseId, L"", true, (void*)&pathList);
	}

	CheckIOSUpgrateInternal();
	CheckAndroidUpgrateInternal();
}

void InstallCaseModelEx::GetRefreshFolderList(InstallsCaseData* casedata, vector<AppCountByFolder*>* folders)
{INTERFUN;
	int id=0;
	if (!casedata) return;
	for (vector<AppCountByFolder*>::iterator it = InstallCaseManagerEx::GetInstance()->AppFolderList.begin(); 
		 it != InstallCaseManagerEx::GetInstance()->AppFolderList.end(); it++)
	{
		AppCountByFolder* appFolder = *it;
		if(casedata->CaseType() == appFolder->Filetype)
			folders->push_back(appFolder);
	}
}

void InstallCaseModelEx::GetCaseTypeAndPathList( _INSTALLS_CASE_DATA_TYPE& caseType, vector<wstring>* fileList, vector<wstring>* pathList)
{INTERFUN;
	vector<wstring> iosPathList;
	vector<wstring> androidPathList;
	for (vector<wstring>::iterator it = fileList->begin(); it != fileList->end(); it++)
	{
		wstring path = *it;
		if(CFileOperation::IsDirExist(path.c_str()))
		{
			vector<wstring> subFiles;
			CFileOperation::FindAllFileInDir(path, subFiles, L"ipa");
			if(subFiles.size() > 0)
				iosPathList.insert(iosPathList.end(), subFiles.begin(), subFiles.end());

			CFileOperation::FindAllFileInDir(path, subFiles, L"pxl");
			if(subFiles.size() > 0)
				iosPathList.insert(iosPathList.end(), subFiles.begin(), subFiles.end());

			CFileOperation::FindAllFileInDir(path, subFiles, L"apk");
			if(subFiles.size() > 0)
				androidPathList.insert(iosPathList.end(), subFiles.begin(), subFiles.end());

			CFileOperation::FindAllFileInDir(path, subFiles, L"npk");
			if(subFiles.size() > 0)
				androidPathList.insert(iosPathList.end(), subFiles.begin(), subFiles.end());
		}
		else
		{
			wstring ext= CFileOperation::GetFileExt(path);
			ext=CStrOperation::toLowerW(ext);
			if(ext == L"ipa" || ext == L"pxl")
			{
				iosPathList.push_back(path);
			}
			else if(ext == L"apk" || ext == L"npk")
			{
				androidPathList.push_back(path);
			}
		}
	}
	
	if(iosPathList.size() < androidPathList.size())
	{
		caseType = _INSTALLS_CASE_DATA_ANDROID;
		pathList->insert(pathList->end(), androidPathList.begin(), androidPathList.end());
	}
	else
	{
		caseType = _INSTALLS_CASE_DATA_IOS;
		pathList->insert(pathList->end(), iosPathList.begin(), iosPathList.end());
	}
}

void InstallCaseModelEx::GetRefreshFileList(InstallsCaseData* caseData, vector<wstring> &pathList )
{INTERFUN;
	vector<AppCountByFolder *> folders;
	GetRefreshFolderList(caseData, &folders);

	for(vector<AppCountByFolder*>::iterator it=folders.begin(); it!=folders.end(); it++)
	{
		AppCountByFolder* folder = *it;

		vector<wstring> subFiles;
		vector<wstring> subFiles1;
		wstring foldername = CStrOperation::trimRightW(folder->FolderName,L"\\");
		if(folder->Filetype == _INSTALLS_CASE_DATA_IOS)
		{
			CFileOperation::FindAllFileInDir(foldername, subFiles, L"ipa");
			CFileOperation::FindAllFileInDir(foldername, subFiles1, L"pxl");
			subFiles.insert(subFiles.end(), subFiles1.begin(), subFiles1.end());
		}
		else
		{
			CFileOperation::FindAllFileInDir(foldername, subFiles, L"apk");
			CFileOperation::FindAllFileInDir(foldername, subFiles1, L"npk");
			subFiles.insert(subFiles.end(), subFiles1.begin(), subFiles1.end());
		}

		InstallsAppList* appList = caseData->ListApp();

		for(size_t i=0;i<subFiles.size();i++)
		{
			InstallsAppInfo *appInfo = NULL;
			wstring filePath = subFiles.at(i);
			for(InstallsAppList::iterator it = appList->begin(); it != appList->end(); it++)
			{
				InstallsAppInfo* tmp = *it;
				if(filePath.compare(tmp->m_strSavePath) == 0)
				{
					appInfo = tmp;
					break;
				}
			}

			//图标是否正常
			bool isIconExist = appInfo != NULL && !appInfo->m_Iconpath.empty() && 
				CFileOperation::IsFileExist(appInfo->m_Iconpath.c_str()) &&
				CFileOperation::GetFileSizeW(appInfo->m_Iconpath.c_str()) > 0;

			if(appInfo == NULL || !isIconExist)
			{
				pathList.push_back(filePath);
			}
			appInfo = NULL;
		}
	}
}

void InstallCaseModelEx::RemoveDiffVerApp( InstallsAppInfo* appInfo, wstring oldFilepath )
{INTERFUN;
	InstallsCaseData* caseData = InstallCaseManagerEx::GetInstance()->GetSystemCaseData(appInfo);

	InstallsAppInfo* diffInfo = caseData->GetAppBySavePath(oldFilepath);

	bool refresh = !diffInfo;

	//将已升级的软件更新到方案
	vector<InstallsCaseData*> caseList = InstallCaseManagerEx::GetInstance()->InstallCaseDataList;
	for(vector<InstallsCaseData*>::iterator it = caseList.begin(); it != caseList.end(); it++)
	{
		InstallsCaseData* tmpCase = *it;
		if(tmpCase->PlanType() != _INSTALL_PLAN_TYPE_SYS && tmpCase->PlanType() != _INSTALL_PLAN_TYPE_UPGRATE)
		{
			InstallsAppInfo* tmpInfo = tmpCase->GetAppBySavePath(oldFilepath);
			if(tmpInfo != NULL && tmpCase->GetAppBySavePath(appInfo->m_strSavePath) == NULL )
			{
				InstallsAppInfo* info = InstallCaseManagerEx::GetInstance()->AddAppToCase(tmpCase, appInfo->m_strSavePath);
				
				InstallCaseMsg msg;
				msg.message = WM_CASE_APP_ADD;
				msg.wParam  = (WPARAM)tmpCase->Id();
				msg.lParam  = (LPARAM)info;
				msg.SendToUI();

				msg.message = WM_CASE_SET_NUMBER;
				msg.wParam  = (WPARAM)tmpCase->Id();
				msg.lParam  = (LPARAM)tmpCase;
				msg.SendToUI();
			}
		}
	}

	vector<wstring> pathList;
	set<wstring> dirs = GetSameAppList(caseData,appInfo,&pathList);
	wstring fileNameTmp = CFileOperation::GetFileNameNoExt(appInfo->m_strSavePath);
	wstring fileExt = CFileOperation::GetFileExt(appInfo->m_strSavePath);
	for(set<wstring>::iterator it = dirs.begin(); it != dirs.end(); it++)
	{
		wstring dirTmp = *it;
		if (CFileOperation::IsDirExist(dirTmp.c_str()))
		{
			wstring newFileTmp = dirTmp + fileNameTmp + _T(".") + fileExt;
			if (CFileOperation::IsFileExist(newFileTmp.c_str()))
			{
				for (int i = 0 ; i<1000 ; i ++)
				{
					wstringstream ssTmp;
					ssTmp<<i;
				    newFileTmp = dirTmp + fileNameTmp + ssTmp.str()  + _T(".") + fileExt;
					if (!CFileOperation::IsFileExist(newFileTmp.c_str()))
					{
						break;
					}
				}
			}
			
			CFileOperation::CopyFile(appInfo->m_strSavePath,newFileTmp,false);

			InstallsAppInfo* appInfoTmp = InstallCaseManagerEx::GetInstance()->AddAppToCaseByPath(caseData, newFileTmp, true, appInfo->m_id);

			if (appInfoTmp)
			{
				InstallCaseMsg msg;
				msg.message = WM_CASE_APP_ADD;
				msg.wParam  = (WPARAM)caseData->Id();
				msg.lParam  = (LPARAM)appInfoTmp;
				msg.SendToUI();

				msg.message = WM_CASE_SET_NUMBER;
				msg.wParam  = (WPARAM)caseData->Id();
				msg.lParam  = (LPARAM)caseData;
				msg.SendToUI();
			}
			
		}
	}
	//删除底版本软件

	RemoveAppFromCaseInternal(caseData->Id(), L"", true, (void*)&pathList, true);

	if(refresh)
	{
		vector<AppCountByFolder*> oldFolders;
		oldFolders.insert(oldFolders.end(), InstallCaseManagerEx::GetInstance()->AppFolderList.begin(), InstallCaseManagerEx::GetInstance()->AppFolderList.end());
		CheckFolderChanged(&oldFolders);
	}
}

wstring InstallCaseModelEx::GetNewDefaultCaseName(_INSTALLS_CASE_DATA_TYPE type)
{INTERFUN;
	wstring newname;
	if(type==_INSTALLS_CASE_DATA_IOS)
		newname = CommonUIFunc::LoadString(L"InstallCase_Text_CaseiOS");
	else
		newname = CommonUIFunc::LoadString(L"InstallCase_Text_CaseAndroid");

	int j=1;
	wstring seackName = newname + CStrOperation::IntToWString(j);
	bool continuefor = true;
	while(continuefor)
	{
		bool hassamename = false;
		for(size_t i =0;i< InstallCaseManagerEx::GetInstance()->InstallCaseDataList.size();i++)
		{			
			InstallsCaseData *casedata = InstallCaseManagerEx::GetInstance()->InstallCaseDataList.at(i);		
			if(casedata->CaseName() == seackName )
			{
				hassamename = true;
				j++;
				seackName = newname + CStrOperation::IntToWString(j);
				break;
			}
		}
		if(!hassamename)
			continuefor = false;
	}
	return seackName;
}

bool InstallCaseModelEx::IsAppExistInCase( InstallsCaseData* caseData, wstring appPath )
{
	InstallsAppList* appList = caseData->ListApp();
	for(InstallsAppList::iterator it = appList->begin(); it != appList->end(); it++)
	{
		InstallsAppInfo* tmp = *it;
		if(CStrOperation::CompareUnCase(tmp->m_strSavePath, appPath) == 0)
			return true;
	}
	return false;
}

void InstallCaseModelEx::AddAppToCaseByPathNotify( int count )
{
	if ( count > 0)
	{
		Notice(CommonUIFunc::LoadString(_T("软件已添加！")), 2000);
	}
	else
	{
		Notice(CommonUIFunc::LoadString(_T("此软件已存在于您选择的装机方案！")), 2000);
	}
}

set<wstring>  InstallCaseModelEx::GetSameAppList( InstallsCaseData* caseData,InstallsAppInfo* appInfo,vector<wstring>* pathList )
{
	INTERFUN;
	set<wstring> r;
	wstring appInfoDir = CFileOperation::GetFileDir(appInfo->m_strSavePath);
	for(InstallsAppList::iterator it = caseData->ListApp()->begin(); it!= caseData->ListApp()->end(); it++)
	{

		InstallsAppInfo *appInfoTmp = *it;
		if (appInfoTmp->m_strPackageID == appInfo->m_strPackageID && appInfo->m_strSavePath!= appInfoTmp->m_strSavePath)
		{
			if (appInfo->m_isLegalSoft == appInfoTmp->m_isLegalSoft)
			{
				pathList->push_back(appInfoTmp->m_strSavePath);
				wstring dir = CFileOperation::GetFileDir(appInfoTmp->m_strSavePath);
				if (r.find(dir)==r.end()&&appInfoDir!=dir)
				{
					r.insert(dir);
				}
			}
		}
	}
	return r;
}

bool InstallCaseModelEx::Handle(GetCaseDataMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	InstallsCaseData* caseData = GetSystemCaseData((_INSTALLS_CASE_DATA_TYPE)pMsg->nCaseType);

	if (NULL == caseData)
	{
		return false;
	}
	for(InstallsAppList::iterator it = caseData->ListApp()->begin(); it!= caseData->ListApp()->end(); it++)
	{
		InstallsAppInfo *appInfo = *it;
		if(!appInfo)
			continue;
		if(appInfo->m_strPackageID==pMsg->_packageName && appInfo->m_strVersion==pMsg->_strVersion 
			&& appInfo->m_isLegalSoft==pMsg->_isLegalSoft && CFileOperation::IsFileExist(appInfo->m_strSavePath.c_str()))
		{
			pMsg->installFilePaths.push_back(appInfo->m_strSavePath);
		}
	}

	return true;
}

void InstallCaseModelEx::Notice(wstring msg, wstring icon, int millisecond)
{
	NoticeMsgThread* pNoticeMsgThread = new NoticeMsgThread;
	pNoticeMsgThread->SetNoticeMsg(msg, icon, millisecond);
	pNoticeMsgThread->Run();
}

void InstallCaseModelEx::Notice(wstring msg, int millisecond)
{
	this->Notice(msg, L"", millisecond);
}

bool InstallCaseModelEx::Handle(GetNetInstallsCaseDataListMsg* pMsg)
{
	vector<InstallsCaseData*>* pCaseData = new vector<InstallsCaseData*>;
	*pCaseData = InstallCaseManagerEx::GetInstance()->NetInstallPlans;
	pMsg->pDataList = (void*)pCaseData;

	return true;
}

bool InstallCaseModelEx::Handle(GetSortByUseTimeCaseListMsg* pMsg)
{
	vector<InstallsCaseData*>* pCaseData = new vector<InstallsCaseData*>;
	*pCaseData = GetCaseDataList();
	pMsg->pDataList = (void*)pCaseData;

	return true;
}

bool InstallCaseModelEx::Handle(const AddDownLoadFolderMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}
	this->AddDownLoadFolder(pMsg->iosPath, pMsg->androidPath, pMsg->fromModular);

	return true;
}

bool InstallCaseModelEx::Handle(const AppUpdategradeCancelMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}
	this->AppUpdategradeCancel((CTaskItem*)pMsg->item);

	return true;
}

bool InstallCaseModelEx::Handle( AddSingleAppFileToLibMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}
	pMsg->result = this->AddSingleAppFileToLib(pMsg->filePath, pMsg->isReshUI, pMsg->fid, pMsg->oldFilePath);

	return true;
}

bool InstallCaseModelEx::Handle(const AddAppToCaseByPathMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}
	this->AddAppToCaseByPath(pMsg->nCaseId, pMsg->pPathes);

	return true;
}

bool InstallCaseModelEx::Handle( InstallCaseInfoMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}
	InstallsCaseData* pInstallsCaseData = this->GetCaseById(pMsg->nParam);
	if (pInstallsCaseData)
	{
		pMsg->bIsExist = true;
		pMsg->nCaseType = pInstallsCaseData->CaseType();
	}

	return true;
}

INSTALLCASE_END