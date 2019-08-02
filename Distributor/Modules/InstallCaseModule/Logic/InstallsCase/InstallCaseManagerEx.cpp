#include "StdAfx.h"

#include   <sys\stat.h> 

#include "Module/Common/AndroidXMLParser/AndroidXMLOperation.h"
#include "Module/Common/IosPList/IosPlistConverter.h"
#include "Module/Application/AppManager.h"
#include "Module/Application/IosApp/PxlToIpaHelper.h"
#include "Module/Application/IosApp/IosDebAppOperation.h"
#include "Module/Application/IosApp/PxlToIpaHelper.h"
#include "Common/IosPngConver.h"
#include "Common/plist/PlistOperation.h"
#include "Common/MergeUserData.h"
#include "../NetInstallsCase/NetInstallsCaseDB.h"
//#include "InstallsCaseDBEx.h"
//#include "InstallCaseManagerEx.h"

INSTALLCASE_BEGIN
#define CHECK_LOADING while (!_bLoaded){::Sleep(50);}
#define CHECK_IOSUPGRATING while (_bIOSChecking){::Sleep(50);}
#define CHECK_ANDROIDUPGRATING while (_bAndroidChecking){::Sleep(50);}
#define CHECK_IOSLEGALUPGRADING while (_bIOSLegalChecking){::Sleep(50);}
#define CHECK_ANDROIDLEGALUPGRADING while (_bAndroidLegalChecking){::Sleep(50);}

InstallCaseManagerEx * InstallCaseManagerEx::_pInstallsCaseManager = NULL;

InstallCaseManagerEx::InstallCaseManagerEx(void)
{INTERFUN;
	_bLoaded = false;
	_bIOSChecking = false;
	_bAndroidChecking = false;
	_bIOSLegalChecking = false;
	_bAndroidLegalChecking = false;
	_pInstallsCaseDB = NULL;
	_pNetInstallsCaseDB = NULL;

	_mFolderId = 1;
}

InstallCaseManagerEx::~InstallCaseManagerEx(void)
{INTERFUN;
}

InstallCaseManagerEx* InstallCaseManagerEx::GetInstance()
{INTERFUN;
	if(!_pInstallsCaseManager)
		_pInstallsCaseManager = new InstallCaseManagerEx();

	return _pInstallsCaseManager;
}

void InstallCaseManagerEx::Load()
{INTERFUN;
	_bLoaded = false;

	//初始化数据库	
	if(_pInstallsCaseDB == NULL)
	{
		wstring iconPath = DataPath::GetDataDir() + L"\\icon";
		wstring path=CONFIG(_strIosDownPath);
		CFileOperation::MakeLocalDir(iconPath);
		CFileOperation::MakeLocalDir(path);
		path=CONFIG(_strAndroidDownPath);
		CFileOperation::MakeLocalDir(path);

		_pInstallsCaseDB = new InstallsCaseDBEx();

		_pNetInstallsCaseDB = new NetInstallsCaseDB();
	}

	AddFolder(NULL, _INSTALLS_CASE_DATA_IOS);
	AddFolder(NULL, _INSTALLS_CASE_DATA_ANDROID);

    // 合并数据
    wstring mergeData = MyFileOperation::GetIniFileValue(CONFIG(_strUserSettingIni), _T("Run"), _T("MergeData"));
    if (mergeData != L"1")
    {
        MergeUserData merge; 
        MyFileOperation::SetIniFileValue(CONFIG(_strUserSettingIni), _T("Run"), _T("MergeData"),L"1");
    }
	_pInstallsCaseDB->LoadData(this);

	_pUpgrateCaseDataIOS = new InstallsCaseData(0);
	_pUpgrateCaseDataIOS->CaseType(_INSTALLS_CASE_DATA_IOS);
	_pUpgrateCaseDataIOS->PlanType(_INSTALL_PLAN_TYPE_UPGRATE);
	this->InstallCaseDataList.push_back(_pUpgrateCaseDataIOS);

	_pUpgrateCaseDataAndroid = new InstallsCaseData(1);
	_pUpgrateCaseDataAndroid->CaseType(_INSTALLS_CASE_DATA_ANDROID);
	_pUpgrateCaseDataAndroid->PlanType(_INSTALL_PLAN_TYPE_UPGRATE);
	this->InstallCaseDataList.push_back(_pUpgrateCaseDataAndroid);

	_pUpgrateCaseDataIOSLegal = new InstallsCaseData(10000);
	_pUpgrateCaseDataIOSLegal->CaseType(_INSTALLS_CASE_DATA_IOS);
	_pUpgrateCaseDataIOSLegal->PlanType(_INSTALL_PLAN_TYPE_LEGALUPGRADE);
	this->InstallCaseDataList.push_back(_pUpgrateCaseDataIOSLegal);

	_bLoaded = true;

	if(_pInstallsCaseDB->needReAnalyzeApps)
	{
		ReAnalyzeAppForItemID();
	}
}

void InstallCaseManagerEx::LoadWebCase( bool bLoadDataBase, int nUserId, bool bCpa)
{INTERFUN;
	CHECK_LOADING;
	_pNetInstallsCaseDB->LoadData(bLoadDataBase, nUserId,  bCpa);
}

int InstallCaseManagerEx::CheckPiracyUpgrateAppList(InstallsCaseData* caseData)
{INTERFUN;
	CHECK_LOADING;

	if(caseData == NULL)
		return 0;

	InstallsCaseData* resultCase = NULL;//可升级应用获取过程
	if(caseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
	{
		_bIOSChecking = true;

		resultCase = this->_pUpgrateCaseDataIOS;//赋值
	}
	else
	{
		_bAndroidChecking = true;

		resultCase = this->_pUpgrateCaseDataAndroid;//赋值
	}

	resultCase->ListApp()->Empty();

	try
	{
		vector<AppUpdateInfo*> updateapps;

		vector<InstallsAppInfo*> LastVerAppList;
		vector<InstallsAppInfo*> SameVerAppList;
		GetLastVerAppList(caseData,&LastVerAppList,0,&SameVerAppList);

		for(vector<InstallsAppInfo*>::iterator it = SameVerAppList.begin();
			it != SameVerAppList.end();it++)
		{
			InstallsAppInfo * sameAppinfo = *it;
			for(vector<InstallsAppInfo*>::iterator itLastVer = LastVerAppList.begin();
				itLastVer != LastVerAppList.end();itLastVer++)
			{
				InstallsAppInfo * lastVerAppinfo = *itLastVer;
				if (lastVerAppinfo->m_strPackageID == sameAppinfo->m_strPackageID)
				{
					if(!UIStringFormat::IsNewVersion(lastVerAppinfo->m_strVersion.c_str(),sameAppinfo->m_strVersion.c_str()))
					{
						*itLastVer = *it;
						break;
					}
				}
			}
		}

		for(vector<InstallsAppInfo*>::iterator it = LastVerAppList.begin();
			it != LastVerAppList.end();it++)
		{
			InstallsAppInfo * appinfo = *it;
			AppUpdateInfo* updateappinfo = new AppUpdateInfo();
			updateappinfo->_identifier = appinfo->m_strPackageID;
			updateappinfo->_oldVersion = appinfo->m_strVersion;
			updateappinfo->_verCode =caseData->CaseType()==_INSTALLS_CASE_DATA_IOS?L"0":appinfo->m_VerCode;
			updateapps.push_back(updateappinfo);		
		}
		
		CGetAppUpdateInfo *getappinfo = new CGetAppUpdateInfo((int)caseData->CaseType());
		bool Getinforesult = false;
		Getinforesult = getappinfo->GetAppUpdateDataNew(&updateapps, false);
		if(Getinforesult == false)
			Getinforesult = getappinfo->GetAppUpdateDataNew(&updateapps, false);
		if(Getinforesult == true)
		{
			//by to do....
			SetAppsPiracyUpgrade(updateapps, caseData->CaseType());
			
			vector<InstallsAppInfo*>::iterator caseit = LastVerAppList.begin();
			for(size_t i=0;i<updateapps.size();i++)
			{
				InstallsAppInfo * appinfo = *caseit;
				caseit++;
				AppUpdateInfo *updateappinfo = updateapps.at(i);
				if(updateappinfo->_newVersion.length() > 0)
				{		
					//查看是否已经有这个版本的软件了
					bool bHasUpdate=false;
					for(vector<InstallsAppInfo*>::iterator it = LastVerAppList.begin();
						it != LastVerAppList.end();it++)
					{
						InstallsAppInfo * appinfofind = *it;	
						if(appinfofind->m_strVersion == updateappinfo->_newVersion &&
							appinfofind->m_strPackageID == appinfo->m_strPackageID)
						{
							bHasUpdate =true;
							break;
						}
					}
					if(!bHasUpdate)
					{					
						InstallsAppInfo* newappinfo = new InstallsAppInfo(appinfo->m_id);
						this->CopyTo(appinfo, newappinfo);
						newappinfo->m_strDownLoadURL = updateappinfo->_url;
						newappinfo->m_strNewVersion = updateappinfo->_newVersion;
						newappinfo->m_FileSize = updateappinfo->_nsize;
						newappinfo->m_fid = updateappinfo->_nfid;
						resultCase->AddAppToList(newappinfo);
					}
				}
				SAFE_DELETE(updateappinfo);
			}
		}
		SAFE_DELETE(getappinfo);	
		updateapps.clear();
	}
	catch (...)
	{
		LOG->WriteDebug(L"InstallCaseManagerEx->CheckPiracyUpgrateAppList: 出错");	
	}
	if(caseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
	{
		_bIOSChecking = false;
	}
	else
	{
		_bAndroidChecking = false;
	}
	return resultCase->GetAppNum();
}

void InstallCaseManagerEx::CheckPlanUpgrateAppList()
{INTERFUN;
	//CHECK_LOADING;

	//if(caseData == NULL)
	//return 0;

	//InstallsCaseData* resultCase = NULL;
	//if(caseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
	//{
	//	_bIOSChecking = true;

	//	resultCase = this->_pUpgrateCaseDataIOS;
	//}
	//else
	//{
	//	_bAndroidChecking = true;

	//	resultCase = this->_pUpgrateCaseDataAndroid;
	//}

	//resultCase->ListApp()->Empty();

	//try
	//{
	//	vector<AppUpdateInfo*> updateapps;

	//	vector<InstallsAppInfo*> LastVerAppList;
	//	GetLastVerAppList(caseData,&LastVerAppList);

	//	for(vector<InstallsAppInfo*>::iterator it = LastVerAppList.begin();
	//		it != LastVerAppList.end();it++)
	//	{
	//		InstallsAppInfo * appinfo = *it;
	//		AppUpdateInfo* updateappinfo = new AppUpdateInfo();
	//		updateappinfo->_identifier = appinfo->m_strPackageID;
	//		updateappinfo->_oldVersion = appinfo->m_strVersion;
	//		updateappinfo->_verCode =caseData->CaseType()==_INSTALLS_CASE_DATA_IOS?L"0":appinfo->m_VerCode;
	//		updateapps.push_back(updateappinfo);		
	//	}
	//	CGetAppUpdateInfo *getappinfo = new CGetAppUpdateInfo((int)caseData->CaseType());
	//	bool Getinforesult = false;
	//	Getinforesult = getappinfo->GetAppUpdateDataNew(&updateapps, false);
	//	if(Getinforesult == false)
	//		Getinforesult = getappinfo->GetAppUpdateDataNew(&updateapps, false);

	//	if(Getinforesult == true)
	//	{
	//		vector<InstallsAppInfo*>::iterator caseit = LastVerAppList.begin();
	//		for(size_t i=0;i<updateapps.size();i++)
	//		{
	//			InstallsAppInfo * appinfo = *caseit;
	//			caseit++;
	//			AppUpdateInfo *updateappinfo = updateapps.at(i);
	//			if(updateappinfo->_newVersion.length() > 0)
	//			{		
	//				//查看是否已经有这个版本的软件了
	//				bool bHasUpdate=false;
	//				for(vector<InstallsAppInfo*>::iterator it = LastVerAppList.begin();
	//					it != LastVerAppList.end();it++)
	//				{
	//					InstallsAppInfo * appinfofind = *it;	
	//					if(appinfofind->m_strVersion == updateappinfo->_newVersion &&
	//						appinfofind->m_strPackageID == appinfo->m_strPackageID)
	//					{
	//						bHasUpdate =true;
	//						break;
	//					}
	//				}
	//				if(!bHasUpdate)
	//				{					
	//					InstallsAppInfo* newappinfo = new InstallsAppInfo(appinfo->m_id);
	//					this->CopyTo(appinfo, newappinfo);
	//					newappinfo->m_strDownLoadURL = updateappinfo->_url;
	//					newappinfo->m_strNewVersion = updateappinfo->_newVersion;
	//					newappinfo->m_FileSize = updateappinfo->_nsize;
	//					newappinfo->m_fid = updateappinfo->_nfid;
	//					resultCase->AddAppToList(newappinfo);
	//				}
	//			}
	//			SAFE_DELETE(updateappinfo);
	//		}
	//	}

	//	SAFE_DELETE(getappinfo);	

	//	updateapps.clear();
	//}
	//catch (...)
	//{
	//	LOG->WriteDebug(L"InstallCaseManagerEx->CheckPiracyUpgrateAppList: 出错");
	//}
	//SetAppsPiracyUpgrade(resultCase);
	//if(caseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
	//{
	//	_bIOSChecking = false;
	//}
	//else
	//{
	//	_bAndroidChecking = false;
	//}
	//return resultCase->GetAppNum();
	//return 0;
}


int InstallCaseManagerEx::CheckLegalUpgrateAppList(InstallsCaseData* caseData)
{INTERFUN;
	CHECK_LOADING;

	if(caseData == NULL)
	return 0;

	InstallsCaseData* resultCase = this->_pUpgrateCaseDataIOSLegal;
	resultCase->ListApp()->Empty();

	try
	{
		vector<AppUpdateInfo*> updateapps;
		vector<AppUpdateInfo*> getitemidapps;

		vector<InstallsAppInfo*> LastVerAppList;
		//GetLastVerAppList(caseData,&LastVerAppList,1);
		//GetLastVerAppList(caseData,&LastVerAppList);
		vector<InstallsAppInfo*> SameVerAppList;
		GetLastVerAppList(caseData,&LastVerAppList,0,&SameVerAppList);

		for(vector<InstallsAppInfo*>::iterator it = SameVerAppList.begin();
			it != SameVerAppList.end();it++)
		{
			InstallsAppInfo * sameAppinfo = *it;
			for(vector<InstallsAppInfo*>::iterator itLastVer = LastVerAppList.begin();
				itLastVer != LastVerAppList.end();itLastVer++)
			{
				InstallsAppInfo * lastVerAppinfo = *itLastVer;
				if (lastVerAppinfo->m_strPackageID == sameAppinfo->m_strPackageID)
				{
					if(!UIStringFormat::IsNewVersion(lastVerAppinfo->m_strVersion.c_str(),sameAppinfo->m_strVersion.c_str()))
					{
						*itLastVer = *it;
						break;
					}
				}
			}
		}

		for(vector<InstallsAppInfo*>::iterator it = LastVerAppList.begin();
			it != LastVerAppList.end();it++)
		{
			InstallsAppInfo* appinfo = *it;
			if(appinfo->m_isLegalSoft==1)
			{
				//chenxy2013227 m_itemID可能在包里面不存在这个信息
				if(appinfo->m_itemID>1000000)
				{
					AppUpdateInfo* updateappinfo = new AppUpdateInfo();
					updateappinfo->_identifier = appinfo->m_strPackageID;
					updateappinfo->_oldVersion = appinfo->m_strVersion;
					updateappinfo->_nfid = appinfo->m_itemID;
					updateapps.push_back(updateappinfo);
				}
				else
				{
					AppUpdateInfo* getitemidappinfo = new AppUpdateInfo();
					getitemidappinfo->_identifier = appinfo->m_strPackageID;
					getitemidapps.push_back(getitemidappinfo);
				}
			}
		}
		CGetAppUpdateInfo *getappinfo = new CGetAppUpdateInfo((int)caseData->CaseType());
		if(getitemidapps.size()>0)
		{
			bool getitemidresult = false;
			//chenxy2013227 将getitemidapps中的ItemId补全
			getitemidresult = getappinfo->GetItunesidByIdentify(&getitemidapps);
			for(vector<AppUpdateInfo*>::iterator itupapp=getitemidapps.begin(); itupapp!=getitemidapps.end();itupapp++)
			{
				AppUpdateInfo* upinfo = *itupapp;
				if(upinfo->_itunesid>100000)
				{
					for(vector<InstallsAppInfo*>::iterator it = LastVerAppList.begin();it != LastVerAppList.end();it++)
					{
						InstallsAppInfo* appinfo = *it;
						if(appinfo->m_itemID<1000000 && CStrOperation::CompareUnCase(appinfo->m_strPackageID, upinfo->_identifier) ==0)
						{
							AppUpdateInfo* updateappinfo = new AppUpdateInfo();
							updateappinfo->_identifier = appinfo->m_strPackageID;
							updateappinfo->_oldVersion = appinfo->m_strVersion;
							updateappinfo->_nfid = upinfo->_itunesid;
							updateapps.push_back(updateappinfo);
							appinfo->m_itemID = upinfo->_itunesid;
							_pInstallsCaseDB->UpdateItemIDByPath(L"SoftLibFile", appinfo->m_strSavePath, appinfo->m_itemID);
							_pInstallsCaseDB->UpdateItemIDByPath(L"InstallPlanFile", appinfo->m_strSavePath, appinfo->m_itemID);
						}
					}
				}
			}
		}
		bool Getinforesult = false;
		Getinforesult = getappinfo->GetAppUpdateDataNew(&updateapps, true);
		if(Getinforesult == false)
			Getinforesult = getappinfo->GetAppUpdateDataNew(&updateapps, true);
		
		if(Getinforesult == true)
		{
			SetAppsLegalUpgrade(updateapps);
			vector<InstallsAppInfo*>::iterator caseit = LastVerAppList.begin();
			for(size_t i=0;i<updateapps.size();i++)
			{
				InstallsAppInfo * appinfo = *caseit;
				caseit++;
				AppUpdateInfo *updateappinfo = updateapps.at(i);
				if(updateappinfo->_newVersion.length() > 0)
				{		
					//查看是否已经有这个版本的软件了
					bool bHasUpdate=false;
					for(vector<InstallsAppInfo*>::iterator it = LastVerAppList.begin();
						it != LastVerAppList.end();it++)
					{
						InstallsAppInfo * appinfofind = *it;	
						if(appinfofind->m_strVersion == updateappinfo->_newVersion &&
							appinfofind->m_strPackageID == appinfo->m_strPackageID)
						{
							bHasUpdate =true;
							break;
						}
					}
					if(!bHasUpdate)
					{
						InstallsAppInfo* newappinfo = new InstallsAppInfo(appinfo->m_id);
						this->CopyTo(appinfo, newappinfo);
						newappinfo->m_strDownLoadURL = updateappinfo->_url;
						newappinfo->m_strVersion = updateappinfo->_newVersion;
						newappinfo->m_FileSize = updateappinfo->_nsize;
						newappinfo->m_fid = updateappinfo->_nfid;
						resultCase->AddAppToList(newappinfo);
					}
				}
				SAFE_DELETE(updateappinfo);
			}
		}

		SAFE_DELETE(getappinfo);	

		updateapps.clear();
	}
	catch (...)
	{
		LOG->WriteDebug(L"InstallCaseManagerEx->CheckPiracyUpgrateAppList: 出错");	
	}
	if(caseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
	{
		_bIOSLegalChecking = false;
	}
	else
	{
		_bAndroidLegalChecking = false;
	}
	return resultCase->GetAppNum();
}

InstallsCaseData* InstallCaseManagerEx::LoadAppList( int caseId )
{INTERFUN;
	CHECK_LOADING;

	//目前是在Load的时候一次性从数据库里加载出方案的所以信息
	//后继如果性能上有问题，就在这里逐个加载

	InstallsCaseData* caseData = this->GetCaseById(caseId);

	if(caseData->PlanType() == _INSTALL_PLAN_TYPE_UPGRATE)
	{
		if(caseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
		{
			CHECK_IOSUPGRATING;
		}
		else
		{
			CHECK_ANDROIDUPGRATING;
		}
	}

	if(caseData->PlanType() == _INSTALL_PLAN_TYPE_LEGALUPGRADE)
	{
		if(caseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
		{
			CHECK_IOSUPGRATING;
		}
		else
		{
			CHECK_ANDROIDUPGRATING;
		}
	}

	return caseData;
}

InstallsCaseData* InstallCaseManagerEx::CreateCase( wstring caseName,_INSTALLS_CASE_DATA_TYPE type )
{INTERFUN;
	CHECK_LOADING;

	int caseId = _pInstallsCaseDB->GetCaseMaxId();
	InstallsCaseData* caseData = new InstallsCaseData(caseId);
	caseData->CaseName(caseName);
	caseData->CaseType(type);
	caseData->PlanType(_INSTALL_PLAN_TYPE_USER);
	_pInstallsCaseDB->InsertNewCase(caseData);
	this->InstallCaseDataList.push_back(caseData);

	return caseData;
}

bool InstallCaseManagerEx::Handle(GetInstallPathByCaseIdMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	InstallsCaseData* caseData = NULL;
	for (vector<InstallsCaseData*>::iterator it = this->InstallCaseDataList.begin(); it != this->InstallCaseDataList.end(); it++)
	{
		InstallsCaseData* tmp = *it;
		if(tmp && tmp->Id() == pMsg->caseId)
		{
			for (InstallsAppList::iterator it = tmp->ListApp()->begin(); it != tmp->ListApp()->end(); it++)
			{
				pMsg->InstallpathList.push_back((*it)->m_strSavePath);
			}
			pMsg->nListSize = tmp->GetAppNum();
			return true;
		}
	}

	for(size_t i=0 ;i<this->NetInstallPlans.size();i++)
	{
		NetInstallsCaseData* tmp = dynamic_cast<NetInstallsCaseData*>(this->NetInstallPlans.at(i));
		if(tmp&&tmp->Id() == pMsg->caseId)
		{
			for (InstallsAppList::iterator it = tmp->ListApp()->begin(); it != tmp->ListApp()->end(); it++)
			{
				pMsg->InstallpathList.push_back((*it)->m_strSavePath);
			}
			pMsg->nListSize = tmp->GetAppNum();
			return true;
		}
	}

	return false;
}

InstallsCaseData* InstallCaseManagerEx::GetCaseById( int caseId )
{INTERFUN;
	CHECK_LOADING;

	InstallsCaseData* caseData = NULL;
	for (vector<InstallsCaseData*>::iterator it = this->InstallCaseDataList.begin(); it != this->InstallCaseDataList.end(); it++)
	{
		InstallsCaseData* tmp = *it;
		if(tmp->Id() == caseId)
		{
			return tmp;
		}
	}

	for(size_t i=0 ;i<this->NetInstallPlans.size();i++)
	{
		InstallsCaseData* tmp = this->NetInstallPlans.at(i);
		if(tmp->Id() == caseId)
			return tmp;
	}
	return NULL;
}

void InstallCaseManagerEx::DeleteCaseByPackId( int PackId )
{INTERFUN;

	for(vector<InstallsCaseData*>::iterator iter=NetInstallPlans.begin();iter!=NetInstallPlans.end();++iter)
	{
		NetInstallsCaseData* tmp = (NetInstallsCaseData*)(*iter);
		if(tmp&&tmp->PackId==PackId)
		{
			NetInstallPlans.erase(iter);
			break;
		}
	}
}

InstallsAppInfo* InstallCaseManagerEx::AddAppToCaseByPath( InstallsCaseData* caseData, wstring path, bool bRefresh, int fid )
{INTERFUN;
	CHECK_LOADING;

	InstallsAppInfo* appInfo = NULL;
	if(!bRefresh)
	{
		InstallsCaseData* sysCaseData = this->GetSystemCaseData(caseData->CaseType());
		appInfo = sysCaseData->GetAppBySavePath(path);
	}
	
	if(appInfo != NULL)
	{
		if(caseData->PlanType() != _INSTALL_PLAN_TYPE_SYS)
		{
			_INSTALLS_CASE_DATA_TYPE type = (appInfo->m_strFileType == L"ipa" || appInfo->m_strFileType == L"pxl")?_INSTALLS_CASE_DATA_IOS:_INSTALLS_CASE_DATA_ANDROID;
			if(type != caseData->CaseType())
				return NULL;

			InstallsAppInfo* newAppInfo = new InstallsAppInfo(appInfo->m_id);
			this->CopyTo(appInfo, newAppInfo);

			caseData->AddAppToList(newAppInfo);		
			_pInstallsCaseDB->InsertCaseFile(caseData, newAppInfo);
			appInfo = newAppInfo;
		}
		return appInfo;
	}
	else
	{
		appInfo = this->AnalyzeApp(path);
		if(appInfo)
			appInfo->m_fid=fid;
	}

	if(appInfo == NULL || appInfo->m_strPackageID.empty())
		return NULL;

	return this->AddApp(caseData, appInfo);
}

InstallsAppInfo* InstallCaseManagerEx::AddAppToCase( InstallsCaseData* caseData, wstring path )
{INTERFUN;
	CHECK_LOADING;

	if(caseData->PlanType() != _INSTALL_PLAN_TYPE_USER)
		return NULL;

	InstallsAppInfo* appInfo = caseData->GetAppBySavePath(path);
	if(appInfo != NULL)
		return appInfo;

	appInfo = this->GetSystemAppInfo(path);
	if(appInfo == NULL)
		return NULL;

	_INSTALLS_CASE_DATA_TYPE type = _INSTALLS_CASE_DATA_IOS;
	if(appInfo->m_strFileType == L"apk" || appInfo->m_strFileType == L"npk")
		type = _INSTALLS_CASE_DATA_ANDROID;

	if(caseData->CaseType() != type)
		return NULL;

	InstallsAppInfo* newAppInfo = new InstallsAppInfo(appInfo->m_id);
	this->CopyTo(appInfo, newAppInfo);
	appInfo = newAppInfo;

	caseData->AddAppToList(appInfo);
	_pInstallsCaseDB->InsertCaseFile(caseData, appInfo);
	return appInfo;;
}

bool InstallCaseManagerEx::RemoveCase( int caseId )
{INTERFUN;
	CHECK_LOADING;

	InstallsCaseData* caseData =  GetCaseById(caseId);

	if(caseData == NULL || caseData->PlanType() != _INSTALL_PLAN_TYPE_USER)
		return false;

	_pInstallsCaseDB->RemoveCase(caseData);
	caseData->ListApp()->Empty();
	for(size_t i =0 ;i<InstallCaseDataList.size();i++)
	{
		InstallsCaseData* tmp = InstallCaseDataList.at(i);
		if(tmp->Id() ==caseData->Id())
		{
			SAFE_DELETE(tmp);
			InstallCaseDataList.erase(InstallCaseDataList.begin()+i);
			return true;
		}
	}
	return false;
}

bool InstallCaseManagerEx::RemoveAppFromCaseByPath( InstallsCaseData* caseData, wstring path )
{INTERFUN;
	CHECK_LOADING;

	InstallsAppInfo* appInfo = caseData->GetAppBySavePath(path);
	if(appInfo == NULL)
		return false;


	if(caseData->PlanType() == _INSTALL_PLAN_TYPE_SYS)
	{		
		_pInstallsCaseDB->RemoveAppFromLib(caseData, appInfo);
		RemoveFolder(appInfo);
	}
	else
	{
		_pInstallsCaseDB->RemoveCaseFile(caseData, appInfo);
	}
	caseData->RemoveAppByid(appInfo->m_id);

	appInfo = NULL;
	return true;
}



InstallsCaseData* InstallCaseManagerEx::GetSystemCaseData( InstallsAppInfo* appInfo )
{INTERFUN;
	CHECK_LOADING;

	_INSTALLS_CASE_DATA_TYPE type = _INSTALLS_CASE_DATA_IOS;
	if(appInfo->m_strFileType == L"apk" || appInfo->m_strFileType == L"npk")
		type = _INSTALLS_CASE_DATA_ANDROID;

	InstallsCaseData* caseData = this->GetSystemCaseData(type);
	return caseData;	
}

InstallsCaseData* InstallCaseManagerEx::GetSystemCaseData( _INSTALLS_CASE_DATA_TYPE caseType )
{INTERFUN;
	CHECK_LOADING;

	InstallsCaseData* sysCaseData = this->InstallCaseDataList.at(0);
	if(sysCaseData->CaseType() != caseType)
		sysCaseData = this->InstallCaseDataList.at(1);

	return sysCaseData;
}

InstallsAppInfo* InstallCaseManagerEx::FindDiffVerApp( InstallsCaseData* caseData, InstallsAppInfo* appinfo )
{INTERFUN;
	if(!appinfo)
		return NULL;

	InstallsAppList *applist = caseData->ListApp();
	if(applist->size() == 0)
		return NULL;

	for(InstallsAppList::iterator it=applist->begin();it!=applist->end();it++ )
	{
		InstallsAppInfo * appinfoold=*it;
		if(appinfoold->m_strPackageID == appinfo->m_strPackageID &&
			appinfoold->m_strVersion != appinfo->m_strVersion)
		{
			return appinfoold;
		}
	}
	return NULL;
}

bool InstallCaseManagerEx::Handle( AnalyzeAppMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	pMsg->pAppInfo = this->AnalyzeApp(pMsg->filePath);

	return true;
}

InstallsAppInfo * InstallCaseManagerEx::AnalyzeApp( wstring filePath )
{INTERFUN;
	//包内容解压临时文件夹
	wstring strTempFilePath = DataPath::GetTempDir();
	CFileOperation::MakeLocalDir(strTempFilePath + L"\\");

	AppData *appdata = NULL;
	try
	{
		appdata = AppManager::GetAppInfoByFile(filePath,strTempFilePath+L"\\",true);
	}
	catch (...)
	{
		TRACE(_T("AppManager::GetAppInfoByFile Error!"));		
	}

	if(!appdata)
		return NULL;

	//新建app信息项
	InstallsAppInfo *newAppinfo = new InstallsAppInfo(0);
	wstring ext= CFileOperation::GetFileExt(filePath);
	ext=CStrOperation::toLowerW(ext);
	if(ext==L"ipa")
	{
		if ( * strTempFilePath.rbegin() != '\\' )
			strTempFilePath.push_back('\\');
		bool hasMetadata = CFileOperation::UnZipFile(filePath, strTempFilePath, L"iTunesMetadata.plist");
		if ( hasMetadata )
		{
			wstring metadata_plist =strTempFilePath + L"iTunesMetadata.plist";
			if (IosPlistConverter::GetXmlFromPlist(metadata_plist, metadata_plist))
			{
				appdata->_strPurchaser=IosAppXmlHelper::GetPurchaser(metadata_plist);
			}
			CFileOperation::DeleteFile(metadata_plist);
		}
	}

	struct stat statbuf;   
	if( stat( CCodeOperation::ws2s(filePath).c_str(), &statbuf ) != -1 )
	{
		newAppinfo->m_tDownloadTime = statbuf.st_mtime;
	}

	newAppinfo->m_strFileType=CFileOperation::GetFileExt(filePath);
	newAppinfo->m_FileSize = CFileOperation::GetFileSizeW(filePath);
	newAppinfo->m_strVersion = appdata->_strVersion;
	newAppinfo->m_strName = appdata->_AppName;
	if(newAppinfo->m_strName.size() == 0)
		newAppinfo->m_strName = CFileOperation::GetFileNameNoExt(filePath);
	newAppinfo->m_strSavePath = filePath;
	newAppinfo->m_strPackageID =appdata->_packageName;

	wstring iconPath = DataPath::GetDataDir() + L"\\icon\\";
	iconPath+=UIStringFormat::CheckDownLoadFileName(newAppinfo->m_strName);
	iconPath+=UIStringFormat::CheckDownLoadFileName(newAppinfo->m_strVersion);
	iconPath+=CFileOperation::GetFileExt(newAppinfo->m_strSavePath);
	iconPath+=_T(".png");
	::CopyFileW(appdata->_strIconPCPath.c_str(), iconPath.c_str(), true);
	newAppinfo->m_Iconpath = iconPath;//upzipIcon(filePath,appdata->_strIconPhonePath,newAppinfo->m_strName,newAppinfo->m_strVersion);

	if(appdata->_nVersionCode < 0)
		appdata->_nVersionCode = 0;

	newAppinfo->m_VerCode = CStrOperation::IntToWString(appdata->_nVersionCode);
	newAppinfo->m_Purchaser = appdata->_strPurchaser;
	newAppinfo->m_isLegalSoft = appdata->_isLegalSoft;
	if(appdata->_DeviceFamily.size()>1)
	{
		newAppinfo->m_SupportDevice= L"iphone/ipod/ipad";
	}
	else if(appdata->_DeviceFamily.size()==1)
	{
		wstring did=appdata->_DeviceFamily.at(0);
		if(did==L"1")
			newAppinfo->m_SupportDevice= L"iphone/ipod";
		else if(did==L"2")
			newAppinfo->m_SupportDevice= L"ipad";
	}
	if (ext== L"ipa")
	{
		newAppinfo->m_SDKVersion = appdata->_minimumOSVersion;
	}
	else
	{
		newAppinfo->m_SDKVersion=appdata->_sdkVer;
	}
	 
	newAppinfo->m_itemID=appdata->_itemId>0? appdata->_itemId:0;
	return newAppinfo;
}

InstallsAppInfo* InstallCaseManagerEx::AddApp( InstallsCaseData* caseData, InstallsAppInfo* appInfo  )
{INTERFUN;
	//InstallsAppInfo* oldApp = NULL;
	//for(InstallsAppList::iterator it = caseData->ListApp()->begin();
	//	it != caseData->ListApp()->end();it++)
	//{
	//	InstallsAppInfo* tmp = *it;
	//	if(tmp->m_strPackageID == appInfo->m_strPackageID 
	//		&& tmp->m_strVersion == appInfo->m_strVersion)
	//	{
	//		oldApp = tmp;
	//		break;
	//	}
	//}
	//if(oldApp != NULL)
	//{
	//	this->CopyTo(appInfo, oldApp);

	//	return oldApp;
	//}
	//else
	//{
	int appId = _pInstallsCaseDB->GetAppMaxId();
	appInfo->m_id = appId;

	_INSTALLS_CASE_DATA_TYPE type = (appInfo->m_strFileType == L"ipa" || appInfo->m_strFileType == L"pxl")?_INSTALLS_CASE_DATA_IOS:_INSTALLS_CASE_DATA_ANDROID;
	if(type != caseData->CaseType())
		return NULL;

	InstallsCaseData* sysCaseData = this->GetSystemCaseData(caseData->CaseType());
	sysCaseData->AddAppToList(appInfo);
	_pInstallsCaseDB->InsertAppToLib(sysCaseData, appInfo);

	InstallsAppInfo* tempAppInfo = NULL;
	if(caseData->PlanType() != _INSTALL_PLAN_TYPE_SYS)
	{
		int planappId = _pInstallsCaseDB->GetInstallPlanFileMaxId();
		tempAppInfo = new InstallsAppInfo(planappId);
		this->CopyTo(appInfo, tempAppInfo);
		caseData->AddAppToList(tempAppInfo);
		_pInstallsCaseDB->InsertCaseFile(caseData, tempAppInfo);
	}
	AddFolder(appInfo, caseData->CaseType());
	if(tempAppInfo)
		return tempAppInfo;
	else
		return appInfo;
	//}
}

void InstallCaseManagerEx::CopyTo(InstallsAppInfo* srcInfo, InstallsAppInfo* desInfo)
{INTERFUN;
	if(srcInfo == NULL || desInfo == NULL)
		return;

	desInfo->m_strName = srcInfo->m_strName.c_str();
	desInfo-> m_strVersion = srcInfo->m_strVersion.c_str();
	desInfo->m_FileSize = srcInfo->m_FileSize;
	desInfo->m_strFileType = srcInfo->m_strFileType.c_str();
	desInfo->m_tDownloadTime = srcInfo->m_tDownloadTime;
	desInfo->m_bUpdateable = srcInfo->m_bUpdateable;
	desInfo->m_bLegalUpdateable = srcInfo->m_bLegalUpdateable;
	desInfo->m_Iconpath = srcInfo->m_Iconpath.c_str();
	desInfo->m_bSelected = srcInfo->m_bSelected;
	desInfo->m_status = srcInfo->m_status;
	desInfo->m_strSavePath = srcInfo->m_strSavePath.c_str();
	desInfo->m_strPackageID = srcInfo->m_strPackageID.c_str();
	desInfo->m_strDownLoadURL = srcInfo->m_strDownLoadURL.c_str();
	desInfo->m_strLegalDownLoadURL = srcInfo->m_strLegalDownLoadURL.c_str();
	desInfo->m_VerCode = srcInfo->m_VerCode.c_str();
	desInfo->m_Purchaser = srcInfo->m_Purchaser.c_str();
	desInfo->m_isLegalSoft = srcInfo->m_isLegalSoft;
	desInfo->m_SupportDevice = srcInfo->m_SupportDevice.c_str();
	desInfo->m_SDKVersion = srcInfo->m_SDKVersion.c_str();
	desInfo->m_fid = srcInfo->m_fid;
	desInfo->m_itemID = srcInfo->m_itemID;
	desInfo->m_strNewVersion = srcInfo->m_strNewVersion.c_str();
	desInfo->m_strNewLegalVersion = srcInfo->m_strNewLegalVersion.c_str();
}

InstallsAppInfo* InstallCaseManagerEx::GetSystemAppInfo( wstring path )
{INTERFUN;
	InstallsAppInfo* appInfo = NULL;
	InstallsCaseData* sysCaseData = this->InstallCaseDataList.at(0);
	appInfo = sysCaseData->GetAppBySavePath(path);
	if(appInfo == NULL)
	{
		sysCaseData = this->InstallCaseDataList.at(1);
		appInfo = sysCaseData->GetAppBySavePath(path); 
	}
	return appInfo;
}

void InstallCaseManagerEx::GetLastVerAppList( InstallsCaseData* casedata,vector<InstallsAppInfo*>* appList, int isLegal,vector<InstallsAppInfo*>* pSameAppList)
{INTERFUN;
	for(InstallsAppList::iterator it = casedata->ListApp()->begin();
		it!= casedata->ListApp()->end();it++)
	{
		InstallsAppInfo *oldInfo = *it;

		//正版软件不升级，防止升级成盗版软件
		//if(oldInfo->m_isLegalSoft && casedata->CaseType() == _INSTALLS_CASE_DATA_IOS)
		//	continue;
		//正版升级去掉盗版软件，盗版升级去掉正版软件，没有正盗版限制（islegal==0）则不过滤
		if(casedata->CaseType() == _INSTALLS_CASE_DATA_IOS && (1==isLegal&&!oldInfo->m_isLegalSoft || -1==isLegal&&oldInfo->m_isLegalSoft))
			continue;

		bool findSameAppIndex = false;
		InstallsAppInfo *newAppInfo = NULL;
		vector<InstallsAppInfo*>::iterator nit;
		for(nit=appList->begin(); nit!=appList->end(); nit++ )
		{
			newAppInfo = *nit;
			if(newAppInfo->m_strPackageID == oldInfo->m_strPackageID)
			{
				findSameAppIndex = true;
				break;
			}
		}
		if(findSameAppIndex)
		{
			if (pSameAppList != NULL)
			{
				if (pSameAppList->size() == 0)
				{
					if(!UIStringFormat::IsNewVersion(newAppInfo->m_strVersion.c_str(),oldInfo->m_strVersion.c_str()))
					{
						pSameAppList->push_back(oldInfo);
					}
					else
					{
						pSameAppList->push_back(newAppInfo);
					}
				}
				else
				{
					bool isPushBackSameAppList = true;
					for (vector<InstallsAppInfo*>::iterator iterTmp = pSameAppList->begin(); iterTmp != pSameAppList->end(); ++iterTmp)
					{
						if ((*iterTmp)->m_strPackageID == oldInfo->m_strPackageID)
						{
							if(!UIStringFormat::IsNewVersion((*iterTmp)->m_strVersion.c_str(),oldInfo->m_strVersion.c_str()))
							{
								*iterTmp = oldInfo;
							}
							isPushBackSameAppList = false;
							break;
						}
					}
					if (isPushBackSameAppList)
					{
						if(!UIStringFormat::IsNewVersion(newAppInfo->m_strVersion.c_str(),oldInfo->m_strVersion.c_str()))
						{
							pSameAppList->push_back(oldInfo);
						}
						else
						{
							pSameAppList->push_back(newAppInfo);
						}
					}
				}
			}
			//找到的app版本要比保存的新
			if(UIStringFormat::IsNewVersion(newAppInfo->m_strVersion.c_str(),oldInfo->m_strVersion.c_str()))
			{
				*nit = oldInfo;
			}
		}
		else
		{
			appList->push_back(oldInfo);
		}
	}
}

void InstallCaseManagerEx::EditCaseName( InstallsCaseData* caseData, wstring caseName )
{INTERFUN;
	_pInstallsCaseDB->RenameCase(caseData, caseName);
}

void InstallCaseManagerEx::AddFolder( InstallsAppInfo* appInfo, _INSTALLS_CASE_DATA_TYPE type )
{INTERFUN;
	wstring folderPath;
	if(appInfo != NULL)
	{
		folderPath = CFileOperation::GetFileDir(appInfo->m_strSavePath);
		if (wstring::npos!=folderPath.find(L"cpa"))
		{
			return;
		}
		for(vector<AppCountByFolder*>::iterator it = AppFolderList.begin(); it != AppFolderList.end(); it++)
		{
			AppCountByFolder* folder = *it;
			if(folder->FolderName == folderPath && type == folder->Filetype)
			{
				folder->AppPathList->push_back(appInfo->m_strSavePath);
				folder->Filecount++;
				folder->TotalFileSize += appInfo->m_FileSize;
				folder->bHasChanged = true;
				return;
			}
		}

		AppCountByFolder* appcountbyFolder = new AppCountByFolder();
		appcountbyFolder->m_id = _mFolderId++;
		appcountbyFolder->FolderName = folderPath;
		appcountbyFolder->TotalFileSize = appInfo->m_FileSize;
		appcountbyFolder->Filecount	= 1;
		appcountbyFolder->Filetype = type;
		appcountbyFolder->bHasChanged = true;
		appcountbyFolder->AppPathList->push_back(appInfo->m_strSavePath);
		AppFolderList.push_back(appcountbyFolder);
	}
	else
	{
		AppCountByFolder* appcountbyFolder = new AppCountByFolder();
		appcountbyFolder->FolderName = type == _INSTALLS_CASE_DATA_IOS? CONFIG(_strIosDownPath) : CONFIG(_strAndroidDownPath);
		appcountbyFolder->TotalFileSize = 0;
		appcountbyFolder->Filecount	= 0;
		appcountbyFolder->Filetype = type;
		appcountbyFolder->m_id = _mFolderId++;
		AppFolderList.push_back(appcountbyFolder);
	}
}

void InstallCaseManagerEx::AddFolder( wstring folderPath, _INSTALLS_CASE_DATA_TYPE type )
{INTERFUN;
	bool hasSamePath = false;
	for(vector<AppCountByFolder*>::iterator it = AppFolderList.begin(); it != AppFolderList.end(); it++)
	{
		AppCountByFolder* folder = *it;
		//folder->bHasChanged = true;
		if(folder->FolderName == folderPath && folder->Filetype == type)
		{
			hasSamePath = true;
		}
	}
	if(!hasSamePath)
	{
		AppCountByFolder* folder = new AppCountByFolder();
		folder->m_id = _mFolderId++;	
		folder->FolderName = folderPath;
		folder->TotalFileSize = 0;
		folder->Filecount	= 0;
		folder->Filetype = type;
		folder->m_id = _mFolderId++;
		AppFolderList.push_back(folder);
	}
}

void InstallCaseManagerEx::RemoveFolder( InstallsAppInfo* appInfo)
{INTERFUN;
	if(appInfo == NULL)
		return;

	_INSTALLS_CASE_DATA_TYPE type = _INSTALLS_CASE_DATA_ANDROID;
	if(appInfo->m_strFileType == L"ipa" || appInfo->m_strFileType == L"pxl")
	{
		type = _INSTALLS_CASE_DATA_IOS;
	}
	wstring folderPath = CFileOperation::GetFileDir(appInfo->m_strSavePath);
	for(vector<AppCountByFolder*>::iterator it = AppFolderList.begin(); it != AppFolderList.end(); it++)
	{
		AppCountByFolder* folder = *it;
		if(folder->FolderName == folderPath && type == folder->Filetype)
		{
			folder->Filecount--;
			folder->TotalFileSize -=appInfo->m_FileSize;
			if( folder->TotalFileSize <= 0 )
				folder->TotalFileSize = 0;
			folder->bHasChanged = true;
			for(vector<wstring>::iterator ait = folder->AppPathList->begin(); ait != folder->AppPathList->end(); ait++)
			{
				wstring path = *ait;
				if(path == appInfo->m_strSavePath)
				{
					folder->AppPathList->erase(ait);
					break;
				}
			}
		}
		if(folder->Filecount <= 0)
		{
			if(_INSTALLS_CASE_DATA_IOS == folder->Filetype && folder->FolderName !=CONFIG(_strIosDownPath)
				|| _INSTALLS_CASE_DATA_ANDROID  == folder->Filetype && folder->FolderName !=CONFIG(_strAndroidDownPath))
			{
				AppFolderList.erase(it);
				break;
			}
		}
	}
}

void InstallCaseManagerEx::RemoveFolder( int appFolderID)
{INTERFUN;
	for(vector<AppCountByFolder*>::iterator it = AppFolderList.begin(); it != AppFolderList.end(); it++)
	{
		AppCountByFolder* folder = *it;
		if(folder->m_id == appFolderID)
		{
			AppFolderList.erase(it);
			break;
		}
	}
}

void InstallCaseManagerEx::IncCaseInstallCount( InstallsCaseData* caseData )
{INTERFUN;
	_pInstallsCaseDB->UpdateCase(caseData);
}

void InstallCaseManagerEx::SetAppsPiracyUpgrade( vector<AppUpdateInfo*> updateapps, _INSTALLS_CASE_DATA_TYPE dataType )
{INTERFUN;
	for(vector<InstallsCaseData*>::iterator it=InstallCaseDataList.begin();it!=InstallCaseDataList.end();it++)
	{
		InstallsCaseData* tempCase=*it;
		if( tempCase->PlanType()!=_INSTALL_PLAN_TYPE_USER &&  tempCase->PlanType()!= _INSTALL_PLAN_TYPE_SYS|| tempCase->CaseType()!=dataType)
			continue;
		InstallsAppList* tempapplist = tempCase->ListApp();

		for(vector<AppUpdateInfo*>::iterator itupapp=updateapps.begin(); itupapp!=updateapps.end();itupapp++)
		{
			AppUpdateInfo* upapp = *itupapp;
			if(upapp->_newVersion.length() >0)
			{
				for(InstallsAppList::iterator ittempapp=tempapplist->begin();ittempapp!=tempapplist->end();ittempapp++)
				{
					InstallsAppInfo* tempapp = *ittempapp;
					if(tempapp->m_strNewVersion.length() >0)//m_strNewVersion是否已经赋值
						continue;
					if(CStrOperation::CompareUnCase(tempapp->m_strPackageID, upapp->_identifier) == 0 
						&& UIStringFormat::IsNewVersion(tempapp->m_strVersion.c_str(),upapp->_newVersion.c_str()))
					{
						tempapp->m_bUpdateable = INSTALLS_CASE_UPDATEABLE_YES;
						tempapp->m_strDownLoadURL = upapp->_url;
						tempapp->m_fid = upapp->_nfid;
						tempapp->m_strNewVersion = upapp->_newVersion;
					}
				}
			}
		}
	}
}

void InstallCaseManagerEx::SetAppsLegalUpgrade( vector<AppUpdateInfo*> updateapps )
{INTERFUN;
	for(vector<InstallsCaseData*>::iterator it=InstallCaseDataList.begin();it!=InstallCaseDataList.end();it++)
	{
		InstallsCaseData* tempCase=*it;
		if( tempCase->CaseType()!=_INSTALLS_CASE_DATA_IOS || tempCase->PlanType()!=_INSTALL_PLAN_TYPE_USER &&  tempCase->PlanType()!= _INSTALL_PLAN_TYPE_SYS)
			continue;
		InstallsAppList* tempapplist = tempCase->ListApp();
		for(vector<AppUpdateInfo*>::iterator itupapp=updateapps.begin(); itupapp!=updateapps.end();itupapp++)
		{
			AppUpdateInfo* upapp = *itupapp;
			if(upapp->_newVersion.length() >0)
			{
				for(InstallsAppList::iterator ittempapp=tempapplist->begin();ittempapp!=tempapplist->end();ittempapp++)
				{
					InstallsAppInfo* tempapp = *ittempapp;
					if(tempapp->m_strNewLegalVersion.length() >0)
						continue;
					if(CStrOperation::CompareUnCase(tempapp->m_strPackageID, upapp->_identifier) == 0
						&& CStrOperation::CompareUnCase(tempapp->m_strVersion, upapp->_newVersion) !=0)
					{
						tempapp->m_bLegalUpdateable = INSTALLS_CASE_LEGALUPDATEABLE_YES;
						tempapp->m_strLegalDownLoadURL = upapp->_url;
						//tempapp->m_fid = upapp->_nfid;
						tempapp->m_strNewLegalVersion = upapp->_newVersion;
					}
				}
			}
		}
	}
}

void InstallCaseManagerEx::ReAnalyzeAppForItemID()
{
	vector<wstring> anylyzedPaths;
	for(vector<InstallsCaseData*>::iterator itcaseData=InstallCaseDataList.begin(); itcaseData!=InstallCaseDataList.end();itcaseData++)
	{
		InstallsCaseData* caseData = *itcaseData;
		if(_INSTALL_PLAN_TYPE_SYS==caseData->PlanType() || _INSTALL_PLAN_TYPE_USER==caseData->PlanType())
		{
			if(_INSTALLS_CASE_DATA_IOS==caseData->CaseType())
			{
				InstallsAppList* appList = caseData->ListApp();
				for(InstallsAppList::iterator itappInfo=appList->begin();itappInfo!=appList->end();itappInfo++)
				{
					InstallsAppInfo* appInfo = *itappInfo;
					if(CFileOperation::IsFileExist(appInfo->m_strSavePath.c_str()))
					{
						InstallsAppInfo* reanyappInfo = this->AnalyzeApp(appInfo->m_strSavePath);
						if(reanyappInfo && reanyappInfo->m_itemID>1000000)
						{
							vector<wstring>::iterator it = std::find(anylyzedPaths.begin(), anylyzedPaths.end(), appInfo->m_strSavePath);
							if (it == anylyzedPaths.end())
							{
								anylyzedPaths.push_back(appInfo->m_strSavePath);
								_pInstallsCaseDB->UpdateItemIDByPath(_INSTALL_PLAN_TYPE_SYS==caseData->PlanType()?L"SoftLibFile":L"InstallPlanFile", appInfo->m_strSavePath, reanyappInfo->m_itemID);
							}
						}
					}
				}
			}
		}
	}
}

INSTALLCASE_END