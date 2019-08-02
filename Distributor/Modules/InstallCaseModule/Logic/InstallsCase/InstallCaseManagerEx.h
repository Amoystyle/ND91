#pragma once

#include "Module/Server/GetAppUpdateInfo.h"

INSTALLCASE_BEGIN

class InstallsCaseDBEx;
class NetInstallsCaseDB;

class InstallCaseManagerEx
	: public BaseGetData<GetInstallPathByCaseIdMsg>
	, public BaseGetData<AnalyzeAppMsg>
{
public:
	InstallCaseManagerEx(void);
	~InstallCaseManagerEx(void);

public:
	static InstallCaseManagerEx* GetInstance();

	//************************************
	// Method:    Load
	// FullName:  InstallCaseManagerEx::Load
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Explain:		加载数据
	//************************************
	void Load();

	//************************************
	// Method:    LoadWebCase
	// FullName:  InstallCaseManagerEx::LoadWebCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: bool bLoadDataBase
	// Explain:		加载网络方案
	//************************************
	void LoadWebCase(bool bLoadDataBase, int nUserId = 0, bool bCpa = false);

	//************************************
	// Method:    CheckPiracyUpgrateAppList
	// FullName:  InstallCaseManagerEx::CheckPiracyUpgrateAppList
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: InstallsCaseData * caseData
	// Explain:		检查软件更新
	//************************************
	int CheckPiracyUpgrateAppList(InstallsCaseData* caseData);

	//************************************
	// Method:    CheckUpgrateLegalAppList
	// FullName:  InstallCaseManagerEx::CheckUpgrateLegalAppList
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: InstallsCaseData * caseData
	// Explain:		检查正版软件更新
	//************************************
	int CheckLegalUpgrateAppList(InstallsCaseData* caseData);

	//************************************
	// Method:    LoadAppList
	// FullName:  InstallCaseManagerEx::LoadAppList
	// Access:    public 
	// Returns:   InstallsCaseData*
	// Qualifier:
	// Parameter: int caseId	方案Id
	// Explain:		加载方案中的软件列表
	//************************************
	InstallsCaseData* LoadAppList(int caseId);

	//************************************
	// Method:    CreateCase
	// FullName:  InstallCaseManagerEx::CreateCase
	// Access:    public 
	// Returns:   InstallsCaseData*
	// Qualifier:
	// Parameter: wstring caseName	方案名称
	// Parameter: _INSTALLS_CASE_DATA_TYPE type	方案类型
	// Explain:		创建方案
	//************************************
	InstallsCaseData* CreateCase(wstring caseName,_INSTALLS_CASE_DATA_TYPE type);

	//************************************
	// Method:    GetCaseById
	// FullName:  InstallCaseManagerEx::GetCaseById
	// Access:    public 
	// Returns:   InstallsCaseData*
	// Qualifier:
	// Parameter: int caseId 方案ID
	// Explain:		根据id获取方案
	//************************************
	InstallsCaseData* GetCaseById(int caseId);

	//************************************
	// Method:    AddAppToCaseByPath
	// FullName:  InstallCaseManagerEx::AddAppToCaseByPath
	// Access:    public 
	// Returns:   InstallsAppInfo*
	// Qualifier:
	// Parameter: InstallsCaseData * caseData
	// Parameter: wstring path	软件路径
	// Explain:		添加路径对应的软件到方案
	//************************************
	InstallsAppInfo* AddAppToCaseByPath(InstallsCaseData* caseData, wstring path, bool bRefresh = false, int fid=0 );

	//************************************
	// Method:    AddAppToCase
	// FullName:  InstallCaseManagerEx::AddAppToCase
	// Access:    public 
	// Returns:   InstallsAppInfo*
	// Qualifier:
	// Parameter: InstallsCaseData * caseData
	// Parameter: wstring path	软件path
	// Explain:		从软件库中添加路径对应的软件到方案
	//************************************
	InstallsAppInfo* AddAppToCase(InstallsCaseData* caseData, wstring path);

	//************************************
	// Method:    RemoveCase
	// FullName:  InstallCaseManagerEx::RemoveCase
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: int caseId
	// Explain:		删除方案
	//************************************
	bool RemoveCase(int caseId);

	//************************************
	// Method:    RemoveAppFromCase
	// FullName:  InstallCaseManagerEx::RemoveAppFromCase
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: InstallsCaseData * caseData
	// Parameter: wstring path
	// Explain:		通过软件路径从方案中删除软件
	//************************************
	bool RemoveAppFromCaseByPath(InstallsCaseData* caseData, wstring path);
	
	//************************************
	// Method:    GetOwnerCase
	// FullName:  InstallCaseManagerEx::GetOwnerCase
	// Access:    public 
	// Returns:   InstallsCaseData*
	// Qualifier:
	// Parameter: InstallsAppInfo * appInfo
	// Explain:		获取软件所在的系统方案
	//************************************
	InstallsCaseData* GetSystemCaseData(InstallsAppInfo* appInfo);

	//************************************
	// Method:    GetSystemCaseData
	// FullName:  InstallCaseManagerEx::GetSystemCaseData
	// Access:    public 
	// Returns:   InstallsCaseData*
	// Qualifier:
	// Parameter: _INSTALLS_CASE_DATA_TYPE caseType
	// Explain:		获取系统方案
	//************************************
	InstallsCaseData* GetSystemCaseData(_INSTALLS_CASE_DATA_TYPE caseType );

	//************************************
	// Method:    FindDiffVerApp
	// FullName:  InstallCaseManagerEx::FindDiffVerApp
	// Access:    public 
	// Returns:   InstallsAppInfo*
	// Qualifier:
	// Parameter: InstallsCaseData * caseData
	// Parameter: InstallsAppInfo * appinfo
	// Explain:		查找底版本软件
	//************************************
	InstallsAppInfo*  FindDiffVerApp(InstallsCaseData* caseData, InstallsAppInfo* appinfo);

	//************************************
	// Method:    EditCaseName
	// FullName:  InstallCaseManagerEx::EditCaseName
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: InstallsCaseData * caseData
	// Parameter: wstring caseName
	// Explain:		重命名方案
	//************************************
	void EditCaseName(InstallsCaseData* caseData, wstring caseName);

	//************************************
	// Method:    AddFolder
	// FullName:  InstallCaseManagerEx::AddFolder
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: InstallsAppInfo * appInfo
	// Parameter: _INSTALLS_CASE_DATA_TYPE type
	// Explain:		添加文件夹
	//************************************
	void AddFolder(InstallsAppInfo* appInfo, _INSTALLS_CASE_DATA_TYPE type );


	//************************************
	// Method:    AddFolder
	// FullName:  InstallCaseManagerEx::AddFolder
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: wstring folderPath
	// Parameter: _INSTALLS_CASE_DATA_TYPE type
	// Explain:		添加空软件库文件夹
	//************************************
	void AddFolder(wstring folderPath, _INSTALLS_CASE_DATA_TYPE type);

	//************************************
	// Method:    RemoveFolder
	// FullName:  InstallCaseManagerEx::RemoveFolder
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: InstallsAppInfo * appInfo
	// Explain:		移除文件夹
	//************************************
	void RemoveFolder(InstallsAppInfo* appInfo);


	//************************************
	// Method:    RemoveFolder
	// FullName:  InstallCaseManagerEx::RemoveFolder
	// Access:    public 
	// Returns:   void
	// Qualifier:	移除软件库文件夹（移除空软件库文件夹专用）
	// Parameter: int appFolderID
	//************************************
	void RemoveFolder( int appFolderID);

	void CheckPlanUpgrateAppList();

	void IncCaseInstallCount(InstallsCaseData* caseData);

	//解析软件
	InstallsAppInfo* AnalyzeApp(wstring filePath);

	//sumary:根据包id删除装机方案
	void DeleteCaseByPackId( int PackId );

	bool Handle(GetInstallPathByCaseIdMsg* pMsg);

	bool Handle(AnalyzeAppMsg* pMsg);

	

private:

	//添加软件
	InstallsAppInfo* AddApp(InstallsCaseData* caseData, InstallsAppInfo* appInfo );

	//拷贝
	void CopyTo(InstallsAppInfo* srcInfo, InstallsAppInfo* desInfo);

	//获取库软件
	InstallsAppInfo* GetSystemAppInfo(wstring path);

	//获取相同软件的最低版本
	void GetLastVerAppList(InstallsCaseData* casedata,vector<InstallsAppInfo*>* appList, int isLegal=0 ,vector<InstallsAppInfo*>* pSameAppList = NULL);

	//获取升级信息以后设置方案软件升级属性
	void SetAppsPiracyUpgrade( vector<AppUpdateInfo*> updateapps, _INSTALLS_CASE_DATA_TYPE dataType );

	//获取正版升级信息以后设置方案软件升级属性
	void SetAppsLegalUpgrade( vector<AppUpdateInfo*> updateapps );

	//重新解析软件库以及装机方案软件
	void ReAnalyzeAppForItemID();

public:
	vector<InstallsCaseData*> InstallCaseDataList;	//方案列表

	vector<InstallsCaseData*> NetInstallPlans;	//网络方案列表

	vector<AppCountByFolder*> AppFolderList;

private:
	static InstallCaseManagerEx* _pInstallsCaseManager;

	int _mFolderId;

	bool _bLoaded;									//是否已加载完成
	bool _bIOSChecking;
	bool _bAndroidChecking;

	bool _bIOSLegalChecking;
	bool _bAndroidLegalChecking;

	InstallsCaseDBEx* _pInstallsCaseDB;

	NetInstallsCaseDB* _pNetInstallsCaseDB;

	InstallsCaseData* _pUpgrateCaseDataIOS;//IOS可升级应用（盗版）
	InstallsCaseData* _pUpgrateCaseDataAndroid;//android可升级应用

	InstallsCaseData* _pUpgrateCaseDataIOSLegal;//IOS可升级应用（正版）
};

INSTALLCASE_END