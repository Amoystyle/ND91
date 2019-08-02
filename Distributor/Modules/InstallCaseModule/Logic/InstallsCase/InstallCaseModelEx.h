#pragma once
#include <set>

INSTALLCASE_BEGIN

#define WM_CASE_ADD						(WM_USER + 400) //400~499预留给方案管理界面update
#define WM_CASE_APP_ADD					(WM_CASE_ADD + 1)
#define WM_CASE_APP_REMOVE				(WM_CASE_ADD + 2)
#define WM_CASE_REMOVE					(WM_CASE_ADD + 3)
#define WM_CASE_FOLDER_ADD				(WM_CASE_ADD + 4)
#define WM_CASE_SET_NUMBER				(WM_CASE_ADD + 5)
#define WM_CASE_APP_LOADED				(WM_CASE_ADD + 6)
#define WM_CASE_APP_EMPTY				(WM_CASE_ADD + 7)
#define WM_CASE_IOSAPPCHECKED			(WM_CASE_ADD + 9)
#define WM_CASE_ANDAPPCHECKED			(WM_CASE_ADD + 10)
#define WM_CASE_FOLDER_REMOVE			(WM_CASE_ADD + 11)
#define WM_CASE_CLEARAPPIST				(WM_CASE_ADD + 12)
#define WM_CASE_APP_UPGRATECANCEL		(WM_CASE_ADD + 13)
#define WM_CASE_TIMECHANGE				(WM_CASE_ADD + 14)
#define WM_CASE_FINISHREFRESH			(WM_CASE_ADD + 15)
#define WM_CASE_IOSLEGALAPPCHECKED		(WM_CASE_ADD + 16)
#define WM_CASE_STARTUPREFRESH			(WM_CASE_ADD + 17)
#define WM_CASE_APP_ADD_RESULT          (WM_CASE_ADD + 18)
#define WM_CASE_APP_ADD_FILTER          (WM_CASE_ADD + 19)

#define Sys_Case_IOS_Upgrate_ID			0
#define Sys_Case_Android_Upgrate_ID		1
#define Sys_Case_IOS_ID					2
#define Sys_Case_Android_ID				3

#define SUPPORTDEVICE_ALL				L"iphone/ipod/ipad"
#define SUPPORTDEVICE_IPHONE			L"iphone/ipod"
#define SUPPORTDEVICE_IPAD				L"ipad"

enum CaseAction
{
	Action_LoadCase,
	Action_LoadWebCase,
	Action_RefreshCase,
	Action_LoadAppList,
	Action_AppSearch,
	Action_EditCase,
	Action_AddAppToCase,
	Action_RemoveCase,
	Action_RemoveAppFromCase,
	Action_UpgrateApp,
	Action_UpgrateAppToLegal,
	Action_InstallApp,
	Action_CheckIOSUpgrate,
	Action_CheckAndroidUpgrate,
	Action_AddFilesToLib,
	Action_CheckIOSLegalUpgrade,
};

enum APPSORT
{
	Asc,
	Desc
};

//软件过滤信息
struct AppSearchContextEx
{
	//方案ID
	UINT CaseID;

	//平台   0为All; 1为iPhone; 2为iPad
	UINT PhoneType;

	//购买者
	wstring Purchaser;

	//软件状态	0为全部; 1为破解; 2为正版
	UINT State;

	//文件夹
	AppCountByFolder* Folder;

	//搜索值
	wstring SearchKey;

	bool ShowLastVer;

	//退出上次搜索
	bool ExistLastSearch;

	//当前是否正在搜索
	bool IsSearching;

	HWND hWnd;
};

class InstallCaseInterface;

class InstallCaseModelEx 
	: BaseGetData<GetCaseDataMsg>
	, public BaseHandler<RemoveCaseByCaseIdMsg>
	, public BaseHandler<InstallAppByCaseIdMsg>
	, public BaseHandler<AddCaseMsg>
	, public BaseGetData<GetNetInstallsCaseDataListMsg>
	, public BaseGetData<GetSortByUseTimeCaseListMsg>
	, public BaseGetData<GetCaseByIdMsg>
	, public BaseHandler<LoadAppListMsg>
	, public BaseHandler<UpgrateAppMsg>
	, public BaseHandler<UpgradeAppToLegalMsg>
	, public BaseHandler<AddDownLoadFolderMsg>
	, public BaseHandler<AppUpdategradeCancelMsg>
	, public BaseGetData<AddSingleAppFileToLibMsg>
	, public BaseHandler<AddAppToCaseByPathMsg>
	, public BaseGetData<InstallCaseInfoMsg>
	, public BaseHandler<ResponseUserLoginMsg>
	, public BaseHandler<QuitMsg>
{
public:
	InstallCaseModelEx(InstallCaseInterface* pDelegate);
	~InstallCaseModelEx();

public:	
	//************************************
	// Method:    Load
	// FullName:  InstallCaseModelEx::Load
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Explain:   加载方案列表，加载方案程序个数，加载Web方案.
	//				1. 如果是第一次加载，重新加载指定的目录
	//				2. IOS/Android默认方案
	//				3. IOS/Android软件升级方案
	//				4. 用户自定义方案
	//************************************
	void Load();

	//************************************
	// Method:    AsyncLoadNetInstallPlans
	// FullName:  InstallCaseModelEx::AsyncLoadNetInstallPlans
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: bool bLoadDataBase 是否加载数据库中的数据
	// Explain:		加载网络方案
	//************************************
	void LoadWebCase(bool bLoadDataBase);

	//************************************
	// Method:    LoadAppList
	// FullName:  InstallCaseModelEx::LoadAppList
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int caseId 方案Id
	// Explain:   根据方案Id加载程序列表
	//************************************
	void LoadAppList(int caseId, wstring type = SUPPORTDEVICE_ALL);

	//************************************
	// Method:    SearchApp
	// FullName:  InstallCaseModelEx::SearchApp
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: AppSearchContextEx * context 搜索上下文
	//************************************
	void SearchApp(AppSearchContextEx* context);

	//************************************
	// Method:    RefreshCase
	// FullName:  InstallCaseModelEx::RefreshCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int caseId	方案id, －1外代表刷新所有
	// Explain:		扫描本地文件，刷新方案
	//					1. 扫描方案将方案，移除方案已经不存在的软件信息
	//					2. 当方案为系统方案时，扫描指定的默认路径，将不存在的软件加入到方案
	//************************************
	void RefreshCase(bool isOnlyShowLastVer = false, int caseId = -1);

	//************************************
	// Method:    AddCase
	// FullName:  InstallCaseModelEx::AddCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: _INSTALLS_CASE_DATA_TYPE caseType	方案类型
	// Parameter: vector<wstring> pathList				新建方案所要包含的软件路径列表
	// Parameter: bool fromLib						是否从库中创建
	// Explain:	  根据idList创建方式不同：
	//				1. fromLib为真是从库中创建，否则从文件夹中创建
	//				2. 当idList不为空时，要求用户输入方案名称后，创建新方案并且将idList添加到新方案中
	//************************************
	void AddCase(_INSTALLS_CASE_DATA_TYPE caseType, vector<wstring>* pathList, bool fromLib = true);

	//************************************
	// Method:    AddCase
	// FullName:  InstallCaseModelEx::AddCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: vector<wstring> * pathList 添加的文件列表
	// Explain:		根据pathList创建方案
	//************************************
	void AddCase(vector<wstring>* pathList);

	//************************************
	// Method:    EditCase
	// FullName:  InstallCaseModelEx::EditCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int caseId	方案Id
	// Parameter: vector<int> idList	方案中的软件Id列表
	// Explain:		移除idList中不存在的软件，并将idList对应的软件加入到方案
	//************************************
	void EditCase(int caseId);

	//************************************
	// Method:    AddAppToCase
	// FullName:  InstallCaseModelEx::AddAppToCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int caseId	方案Id
	// Parameter: vector<wstring> pathList	软件路径列表
	// Parameter: showInfo	为了符合【从应用库向装机方案中添加软件时，已有的软件时，修改提示语为“此软件已存在于您选择的装机方案”】这个需求此参数已经无效，通知通过WM_CASE_APP_ADD_RESULT这个消息调用AddAppToCaseByPathNotify(int count);根据结果进行提示
	// Explain:		将pathList中的软件加入到方案
	//					如果文件列表为空，则提示用户选择app文件
	//************************************
	void AddAppToCaseByPath(int caseId, vector<wstring>* pathList = NULL, bool showInfo=true, wstring path=L"");

	//************************************
	// Method:    AddAppToCaseNotify
	// FullName:  InstallCaseModelEx::AddAppToCaseNotify
	// Access:    public 
	// Returns:   void
	// Qualifier: 通过路径添加到方案的提示
	// Parameter: int count
	//************************************
	void AddAppToCaseByPathNotify(int count);

	//************************************
	// Method:    AddAppToCase
	// FullName:  InstallCaseModelEx::AddAppToCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int caseId	方案Id
	// Parameter: vector<wstring> pathList	软件路径列表
	// Explain:		将idList中的软件添加至方案
	//************************************
	void AddAppToCase(int caseId, vector<wstring>* pathList);	

	//************************************
	// Method:    RemoveCase
	// FullName:  InstallCaseModelEx::RemoveCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int idList	方案id列表
	// Explain:		不允许删除系统方案，只能删除用户自定义方案
	//************************************
	void RemoveCase(vector<int>* idList);

	//************************************
	// Method:    RemoveAppFromCase
	// FullName:  InstallCaseModelEx::RemoveAppFromCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int caseId	方案Id
	// Parameter: vector<wstring> pathList	要移除的软件路径列表
	// Parameter: bool removeFile	是否删除文件，只有当系统方案是有效
	// Explain:		
	//		1. 当caseId不系统方案时，提示用户是否要删除，如果用户确认删除，则删除系统方案中的软件时并删除本地文件，同时也要删除用户自定义方案中的软件
	//		2. 当caseId为用户自定义方案时，提示用户是否要移除，此时只移除软件记录，不删除本地文件
	//		3. 当为升级方案时，执行1、2两步操作（此操作只有在升级完成之后执行）
	//************************************
	void RemoveAppFromCase(int caseId, vector<wstring>* pathList, bool removeFile = false, bool isFolderRemove = false);

	//************************************
	// Method:    UpgrateApp
	// FullName:  InstallCaseModelEx::UpgrateApp
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: vector<wstring> pathList	要升级的软件列表
	//************************************
	void UpgrateApp(int caseId, vector<wstring>* pathList);

	//************************************
	// Method:    UpgrateAppToLegal
	// FullName:  InstallCaseModelEx::UpgrateAppToLegal
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: vector<wstring> pathList	要升级至正版的软件列表
	//************************************
	void UpgradeAppToLegal(int caseId, vector<wstring>* pathList);

	//************************************
	// Method:    InstallApp
	// FullName:  InstallCaseModelEx::InstallApp
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: wstring sn	要安装到的设备Id
	// Parameter: int caseId	所属方案
	// Parameter: vector<wstring> pathList	要安装的软件路径列表
	// Explain:		当idList为空时，安装caseId对应的方案下的所有软件
	//************************************
	void InstallApp(wstring sn, int caseId, vector<wstring>* pathList);


	//************************************
	// Method:    AddDefaultFolder
	// FullName:  InstallCaseModelEx::AddDefaultFolder
	// Access:    public 
	// Returns:   void
	// Qualifier:	添加下载文件夹
	// Parameter: wstring iosPath
	// Parameter: wstring androidPath
	//************************************
	void AddDownLoadFolder(wstring iosPath, wstring androidPath,wstring fromModular=_T(""));

	//************************************
	// Method:    AddLibFolder
	// FullName:  InstallCaseModelEx::AddLibFolder
	// Access:    public 
	// Returns:   void
	// Parameter: _INSTALLS_CASE_DATA_TYPE type	库类型
	// Qualifier:向库中添加文件夹
	//************************************
	void AddLibFolder(_INSTALLS_CASE_DATA_TYPE type, wstring path = L"", bool showInfo=true);

	//************************************
	// Method:    AddFilesToLib
	// FullName:  InstallCaseModelEx::AddFilesToLib
	// Access:    
	// Returns:   void
	// Qualifier:
	// Parameter: vector<wstring> * iosPathList
	// Parameter: vector<wstring> * androidPathList
	// Qualifier:向库中添加添加软件
	//************************************
	void AddFilesToLib(vector<wstring>* iosPathList, vector<wstring>* androidPathList);

	//************************************
	// Method:    AddFilesToLib
	// FullName:  InstallCaseModelEx::AddFilesToLib
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: wstring filePath
	// Qualifier:向库中添加添加单个软件，同时删除底版本软件
	//************************************
	InstallsAppInfo* AddSingleAppFileToLib(wstring filePath, int fid=0, wstring oldFilePath=L"");

	InstallsAppInfo* AddSingleAppFileToLib(wstring filePath, bool &isReshUI, int fid=0, wstring oldFilePath=L"");

	//************************************
	// Method:    AppUpdategradeCancel
	// FullName:  InstallCaseModelEx::AppUpdategradeCancel
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: wstring name
	// Parameter: wstring appPath
	// Qualifier:取消升级
	//************************************
	void AppUpdategradeCancel(CTaskItem* item);

	//************************************
	// Method:    GetAppFolderList
	// FullName:  InstallCaseModelEx::GetAppFolderList
	// Access:    public 
	// Returns:   const vector<AppCountByFolder*>
	// Qualifier:	获取文件夹列表
	//************************************
	const vector<AppCountByFolder*> GetAppFolderList();

	//************************************
	// Method:    RemoveAppFolder
	// FullName:  InstallCaseModelEx::RemoveAppFolder
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: AppCountByFolder * appFolder
	// Explain:		删除Folder
	//************************************
	void RemoveAppFolder(AppCountByFolder* appFolder);

	//************************************
	// Method:    OpenDir
	// FullName:  InstallCaseModelEx::OpenDir
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: wstring path	要打开的文件夹
	// Parameter: bool select   如果path为文件路径，则打开目录并且选中文件
	//************************************
	void OpenDir(wstring path, bool select);

	//************************************
	// Method:    GetCaseDataList
	// FullName:  InstallCaseModelEx::GetCaseDataList
	// Access:    public 
	// Returns:   const vector<InstallsCaseData*>
	// Qualifier:
	// Qualifier:	获取方案列表
	//************************************
	const vector<InstallsCaseData*> GetCaseDataList();

	//************************************
	// Method:    GetCaseDataList
	// FullName:  InstallCaseModelEx::GetCaseDataList
	// Access:    public 
	// Returns:   const vector<InstallsCaseData*>
	// Qualifier:
	// Qualifier:	获取网络方案列表
	//************************************
	vector<InstallsCaseData *> GetNetInstallsCaseDataList();

	//************************************
	// Method:    GetSortByUseTimeCaseList
	// FullName:  InstallCaseModelEx::GetSortByUseTimeCaseList
	// Access:    public 
	// Returns:   vector<InstallsCaseData *>
	// Qualifier:
	// Qualifier:	根据使用时间排序方案
	//************************************
	vector<InstallsCaseData *> GetSortByUseTimeCaseList();

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
	// Method:    GetSystemCaseData
	// FullName:  InstallCaseManagerEx::GetSystemCaseData
	// Access:    public 
	// Returns:   InstallsCaseData*
	// Qualifier:
	// Parameter: _INSTALLS_CASE_DATA_TYPE caseType
	// Explain:		获取系统方案
	//************************************
	InstallsCaseData* GetSystemCaseData(_INSTALLS_CASE_DATA_TYPE caseType );

	//获取方案默认名称
	wstring GetNewDefaultCaseName(_INSTALLS_CASE_DATA_TYPE type);

	//************************************
	// Method:    GetSameAppList
	// FullName:  InstallCaseModelEx::GetSameAppList
	// Access:    public 
	// Returns:   set<wstring>
	// Qualifier: 获取caseData中和appInfo报名一样的软件，如果caseData中含有appInfo不包含appInfo自身
	// Parameter: InstallsAppInfo * caseData
	// Parameter: InstallsAppList * newAppList
	//************************************
	set<wstring> GetSameAppList(InstallsCaseData* caseData,InstallsAppInfo* appInfo,vector<wstring>* pathList);

	//只获取最后的版本的applist
	void GetLastVerAppList(InstallsCaseData* caseData,InstallsAppList* newAppList);

	virtual bool Handle(GetCaseDataMsg* pMsg);

	virtual bool Handle(const RemoveCaseByCaseIdMsg* pMsg);
	
	virtual bool Handle(const InstallAppByCaseIdMsg* pMsg);

	virtual bool Handle(const AddCaseMsg* pMsg);

	virtual bool Handle(GetNetInstallsCaseDataListMsg* pMsg);

	virtual bool Handle(GetSortByUseTimeCaseListMsg* pMsg);
	
	virtual bool Handle(GetCaseByIdMsg* pMsg);

	virtual bool Handle(const LoadAppListMsg* pMsg);

	virtual bool Handle(const UpgrateAppMsg* pMsg);

	virtual bool Handle(const UpgradeAppToLegalMsg* pMsg);

	virtual bool Handle(const AddDownLoadFolderMsg* pMsg);

	virtual bool Handle(const AppUpdategradeCancelMsg* pMsg);
	
	virtual bool Handle( AddSingleAppFileToLibMsg* pMsg);

	virtual bool Handle(const AddAppToCaseByPathMsg* pMsg);

	virtual bool Handle( InstallCaseInfoMsg* pMsg);

	virtual bool Handle(const ResponseUserLoginMsg* pMsg);

	virtual bool Handle(const QuitMsg* pMsg);

//private:

	void  Notice(wstring msg, wstring icon = L"", int millisecond=2000);

	void  Notice(wstring msg, int millisecond=2000);

	//virtual bool Action(int nAction, BaseParam* pParam); 

	//加载数据
	void LoadDataInternal();

	//加载Web方案
	//mPaseParam: 
	//	GetBool():	bLoadDataBase
	void LoadWebCaseInternal(bool bLoadDataBase, int nUserId = 0, bool bCpa = false);

	//检查ios升级
	void CheckIOSUpgrateInternal();

	//检查android升级
	void CheckAndroidUpgrateInternal();

	//检查ios正版升级
	void CheckIOSLegalUpgradeInternal();

	//刷新方案
	//mPaseParam: 
	//	GetInt():	方案ID
	void RefreshCaseInternal( int caseId, bool isOnlyShowLaserVer );

	//加载方案下的软件列表
	//mPaseParam: 
	//	GetInt():	方案ID
	void LoadAppListInternal(int caseId, wstring type);

	//搜索软件
	//mPaseParam: 
	//	GetVoid():	AppSearchContextEx
	void SearchAppInternal(void* pParam );

	//搜索软件
	//为了让锁生效把原有SearchAppInternal方法里的部分代码抽提成一个新的方法
	void SearchAppInternalEx(AppSearchContextEx* context, InstallsCaseData* caseData);

	//编辑方案
	//mPaseParam: 
	//	GetInt():	方案ID
	//	GetVoid():	软件ID列表
	void EditCaseInternal(int caseId, wstring caseName, void* pathList);

	//将软件添加到方案
	//	GetInt():	方案ID
	//	GetBool():	断定GetVoid()代表的内容
	//	GetStr():	"Refresh"代表要刷新方案，文件列表重新解析
	//	GetVoid():	当GetBool为true代表软件路径列表，为false代表软件ID列表
	// isShowNotify 是否提示软件已经添加到方案，ture为提示，false为不提示
	void AddAppToCaseInternal(int caseId, wstring isRefresh, bool isPathList, void* pVoidParam = NULL, \
		void* pWstring = NULL, void* pInt = NULL,bool isShowNotif = true);
	//移除方案
	//	GetVoid():	方案ID列表
	void RemoveCaseInternal(void* pBaseParam);

	//移除方案中的软件
	//	GetInt():	方案ID
	//	GetVoid():	软件ID列表
	void RemoveAppFromCaseInternal( int caseId, wstring isRefresh, bool removeFile, void* pBaseParam = NULL, bool isUpdate=false);

	//更新软件
	//	GetInt():	方案ID
	//	GetVoid():	软件ID列表
	void UpgrateAppInternal(int caseId, void* pBaseParam=NULL);

	//更新软件至正版
	//	GetInt():	方案ID
	//	GetVoid():	软件ID列表
	void UpgradeAppToLegalInternal(int caseId, void* pBaseParam=NULL);

	//安装软件
	//	GetStr():	设备ID
	//	GetInt():	方案ID
	//	GetVoid():	软件ID列表
	void InstallAppInternal(int caseId, wstring deviceId, void* pBaseParam=NULL);

	//添加app文件到库
	//pBaseParam->GetVoidParam: at(0)代表ios列表； at(1)代表android列表
	void AddFilesToLibInternal(void* pParam=NULL, void* pSpevparam=NULL);

//private:
	//创建方案
	InstallsCaseData*  CreateCase(wstring caseName, _INSTALLS_CASE_DATA_TYPE caseType);

	//加载指定文件夹
	void LoadDefaultFolder();

	//加载方案
	void LoadCaseList();

	//查看文件夹列表是否有变化
	void CheckFolderChanged(vector<AppCountByFolder*>* folderList);

	//从库中创建方案
	void CreateCaseFromLib(_INSTALLS_CASE_DATA_TYPE caseType);

	//从文件夹中创建方案
	void CreateCaseFromFolder(_INSTALLS_CASE_DATA_TYPE caseType);

	//通过软件列表创建方案
	void CreateCaseByAppList(_INSTALLS_CASE_DATA_TYPE caseType, vector<wstring>* pathList);

	//遍历所有软件目录，并且返回当前方案的folders， folders为NULL时，只进行扫描
	void GetRefreshFolderList(InstallsCaseData* casedata, vector<AppCountByFolder*>* folders = NULL);

	//根据fileList获取要创建主方案类型和软件路径列表
	void GetCaseTypeAndPathList(_INSTALLS_CASE_DATA_TYPE& caseType, vector<wstring>* fileList, vector<wstring>* pathList);

	//获取要刷新的软件列表
	void GetRefreshFileList(InstallsCaseData* caseData, vector<wstring> &pathList);

	//删除底版本软件
	void RemoveDiffVerApp( InstallsAppInfo* appInfo, wstring oldFilepath );

	//刷新非软件升级方案
	void RefreshSimpleCase(InstallsCaseData* caseData,bool isOnlyShowLastVer);

	//刷新软件升级方案
	void RefreshUpgrateCase(InstallsCaseData* caseData,bool isOnlyShowLastVer);

	void RefreshUpgrateCase(InstallsCaseData* caseData);

	static bool SortByLastTime( const InstallsCaseData *v1, const InstallsCaseData *v2) {	return v1->lastUseTime < v2->lastUseTime;  }

	bool IsAppExistInCase(InstallsCaseData* caseData, wstring appPath);

public:
	bool _bRefreshing;
	bool _bOperating;
	bool _bSearching;
private:
	InstallCaseInterface* _pInterface;

	bool _bExit;

	bool _bFirstiOSUPSearchFinish;
	bool _bFirstAndUPSearchFinish;
	map<wstring, int> _mUpdatingLegalApps;
	map<wstring, int> _mUpdatingPiracyApps;

	map<int, CRITICAL_SECTION> _csLockList;
};

INSTALLCASE_END