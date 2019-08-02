#pragma once
#include <set>

INSTALLCASE_BEGIN

#define WM_CASE_ADD						(WM_USER + 400) //400~499Ԥ���������������update
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

//���������Ϣ
struct AppSearchContextEx
{
	//����ID
	UINT CaseID;

	//ƽ̨   0ΪAll; 1ΪiPhone; 2ΪiPad
	UINT PhoneType;

	//������
	wstring Purchaser;

	//���״̬	0Ϊȫ��; 1Ϊ�ƽ�; 2Ϊ����
	UINT State;

	//�ļ���
	AppCountByFolder* Folder;

	//����ֵ
	wstring SearchKey;

	bool ShowLastVer;

	//�˳��ϴ�����
	bool ExistLastSearch;

	//��ǰ�Ƿ���������
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
	// Explain:   ���ط����б����ط����������������Web����.
	//				1. ����ǵ�һ�μ��أ����¼���ָ����Ŀ¼
	//				2. IOS/AndroidĬ�Ϸ���
	//				3. IOS/Android�����������
	//				4. �û��Զ��巽��
	//************************************
	void Load();

	//************************************
	// Method:    AsyncLoadNetInstallPlans
	// FullName:  InstallCaseModelEx::AsyncLoadNetInstallPlans
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: bool bLoadDataBase �Ƿ�������ݿ��е�����
	// Explain:		�������緽��
	//************************************
	void LoadWebCase(bool bLoadDataBase);

	//************************************
	// Method:    LoadAppList
	// FullName:  InstallCaseModelEx::LoadAppList
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int caseId ����Id
	// Explain:   ���ݷ���Id���س����б�
	//************************************
	void LoadAppList(int caseId, wstring type = SUPPORTDEVICE_ALL);

	//************************************
	// Method:    SearchApp
	// FullName:  InstallCaseModelEx::SearchApp
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: AppSearchContextEx * context ����������
	//************************************
	void SearchApp(AppSearchContextEx* context);

	//************************************
	// Method:    RefreshCase
	// FullName:  InstallCaseModelEx::RefreshCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int caseId	����id, ��1�����ˢ������
	// Explain:		ɨ�豾���ļ���ˢ�·���
	//					1. ɨ�跽�����������Ƴ������Ѿ������ڵ������Ϣ
	//					2. ������Ϊϵͳ����ʱ��ɨ��ָ����Ĭ��·�����������ڵ�������뵽����
	//************************************
	void RefreshCase(bool isOnlyShowLastVer = false, int caseId = -1);

	//************************************
	// Method:    AddCase
	// FullName:  InstallCaseModelEx::AddCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: _INSTALLS_CASE_DATA_TYPE caseType	��������
	// Parameter: vector<wstring> pathList				�½�������Ҫ���������·���б�
	// Parameter: bool fromLib						�Ƿ�ӿ��д���
	// Explain:	  ����idList������ʽ��ͬ��
	//				1. fromLibΪ���Ǵӿ��д�����������ļ����д���
	//				2. ��idList��Ϊ��ʱ��Ҫ���û����뷽�����ƺ󣬴����·������ҽ�idList��ӵ��·�����
	//************************************
	void AddCase(_INSTALLS_CASE_DATA_TYPE caseType, vector<wstring>* pathList, bool fromLib = true);

	//************************************
	// Method:    AddCase
	// FullName:  InstallCaseModelEx::AddCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: vector<wstring> * pathList ��ӵ��ļ��б�
	// Explain:		����pathList��������
	//************************************
	void AddCase(vector<wstring>* pathList);

	//************************************
	// Method:    EditCase
	// FullName:  InstallCaseModelEx::EditCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int caseId	����Id
	// Parameter: vector<int> idList	�����е����Id�б�
	// Explain:		�Ƴ�idList�в����ڵ����������idList��Ӧ��������뵽����
	//************************************
	void EditCase(int caseId);

	//************************************
	// Method:    AddAppToCase
	// FullName:  InstallCaseModelEx::AddAppToCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int caseId	����Id
	// Parameter: vector<wstring> pathList	���·���б�
	// Parameter: showInfo	Ϊ�˷��ϡ���Ӧ�ÿ���װ��������������ʱ�����е����ʱ���޸���ʾ��Ϊ��������Ѵ�������ѡ���װ�����������������˲����Ѿ���Ч��֪ͨͨ��WM_CASE_APP_ADD_RESULT�����Ϣ����AddAppToCaseByPathNotify(int count);���ݽ��������ʾ
	// Explain:		��pathList�е�������뵽����
	//					����ļ��б�Ϊ�գ�����ʾ�û�ѡ��app�ļ�
	//************************************
	void AddAppToCaseByPath(int caseId, vector<wstring>* pathList = NULL, bool showInfo=true, wstring path=L"");

	//************************************
	// Method:    AddAppToCaseNotify
	// FullName:  InstallCaseModelEx::AddAppToCaseNotify
	// Access:    public 
	// Returns:   void
	// Qualifier: ͨ��·����ӵ���������ʾ
	// Parameter: int count
	//************************************
	void AddAppToCaseByPathNotify(int count);

	//************************************
	// Method:    AddAppToCase
	// FullName:  InstallCaseModelEx::AddAppToCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int caseId	����Id
	// Parameter: vector<wstring> pathList	���·���б�
	// Explain:		��idList�е�������������
	//************************************
	void AddAppToCase(int caseId, vector<wstring>* pathList);	

	//************************************
	// Method:    RemoveCase
	// FullName:  InstallCaseModelEx::RemoveCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int idList	����id�б�
	// Explain:		������ɾ��ϵͳ������ֻ��ɾ���û��Զ��巽��
	//************************************
	void RemoveCase(vector<int>* idList);

	//************************************
	// Method:    RemoveAppFromCase
	// FullName:  InstallCaseModelEx::RemoveAppFromCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int caseId	����Id
	// Parameter: vector<wstring> pathList	Ҫ�Ƴ������·���б�
	// Parameter: bool removeFile	�Ƿ�ɾ���ļ���ֻ�е�ϵͳ��������Ч
	// Explain:		
	//		1. ��caseId��ϵͳ����ʱ����ʾ�û��Ƿ�Ҫɾ��������û�ȷ��ɾ������ɾ��ϵͳ�����е����ʱ��ɾ�������ļ���ͬʱҲҪɾ���û��Զ��巽���е����
	//		2. ��caseIdΪ�û��Զ��巽��ʱ����ʾ�û��Ƿ�Ҫ�Ƴ�����ʱֻ�Ƴ������¼����ɾ�������ļ�
	//		3. ��Ϊ��������ʱ��ִ��1��2�����������˲���ֻ�����������֮��ִ�У�
	//************************************
	void RemoveAppFromCase(int caseId, vector<wstring>* pathList, bool removeFile = false, bool isFolderRemove = false);

	//************************************
	// Method:    UpgrateApp
	// FullName:  InstallCaseModelEx::UpgrateApp
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: vector<wstring> pathList	Ҫ����������б�
	//************************************
	void UpgrateApp(int caseId, vector<wstring>* pathList);

	//************************************
	// Method:    UpgrateAppToLegal
	// FullName:  InstallCaseModelEx::UpgrateAppToLegal
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: vector<wstring> pathList	Ҫ���������������б�
	//************************************
	void UpgradeAppToLegal(int caseId, vector<wstring>* pathList);

	//************************************
	// Method:    InstallApp
	// FullName:  InstallCaseModelEx::InstallApp
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: wstring sn	Ҫ��װ�����豸Id
	// Parameter: int caseId	��������
	// Parameter: vector<wstring> pathList	Ҫ��װ�����·���б�
	// Explain:		��idListΪ��ʱ����װcaseId��Ӧ�ķ����µ��������
	//************************************
	void InstallApp(wstring sn, int caseId, vector<wstring>* pathList);


	//************************************
	// Method:    AddDefaultFolder
	// FullName:  InstallCaseModelEx::AddDefaultFolder
	// Access:    public 
	// Returns:   void
	// Qualifier:	��������ļ���
	// Parameter: wstring iosPath
	// Parameter: wstring androidPath
	//************************************
	void AddDownLoadFolder(wstring iosPath, wstring androidPath,wstring fromModular=_T(""));

	//************************************
	// Method:    AddLibFolder
	// FullName:  InstallCaseModelEx::AddLibFolder
	// Access:    public 
	// Returns:   void
	// Parameter: _INSTALLS_CASE_DATA_TYPE type	������
	// Qualifier:���������ļ���
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
	// Qualifier:��������������
	//************************************
	void AddFilesToLib(vector<wstring>* iosPathList, vector<wstring>* androidPathList);

	//************************************
	// Method:    AddFilesToLib
	// FullName:  InstallCaseModelEx::AddFilesToLib
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: wstring filePath
	// Qualifier:����������ӵ��������ͬʱɾ���װ汾���
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
	// Qualifier:ȡ������
	//************************************
	void AppUpdategradeCancel(CTaskItem* item);

	//************************************
	// Method:    GetAppFolderList
	// FullName:  InstallCaseModelEx::GetAppFolderList
	// Access:    public 
	// Returns:   const vector<AppCountByFolder*>
	// Qualifier:	��ȡ�ļ����б�
	//************************************
	const vector<AppCountByFolder*> GetAppFolderList();

	//************************************
	// Method:    RemoveAppFolder
	// FullName:  InstallCaseModelEx::RemoveAppFolder
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: AppCountByFolder * appFolder
	// Explain:		ɾ��Folder
	//************************************
	void RemoveAppFolder(AppCountByFolder* appFolder);

	//************************************
	// Method:    OpenDir
	// FullName:  InstallCaseModelEx::OpenDir
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: wstring path	Ҫ�򿪵��ļ���
	// Parameter: bool select   ���pathΪ�ļ�·�������Ŀ¼����ѡ���ļ�
	//************************************
	void OpenDir(wstring path, bool select);

	//************************************
	// Method:    GetCaseDataList
	// FullName:  InstallCaseModelEx::GetCaseDataList
	// Access:    public 
	// Returns:   const vector<InstallsCaseData*>
	// Qualifier:
	// Qualifier:	��ȡ�����б�
	//************************************
	const vector<InstallsCaseData*> GetCaseDataList();

	//************************************
	// Method:    GetCaseDataList
	// FullName:  InstallCaseModelEx::GetCaseDataList
	// Access:    public 
	// Returns:   const vector<InstallsCaseData*>
	// Qualifier:
	// Qualifier:	��ȡ���緽���б�
	//************************************
	vector<InstallsCaseData *> GetNetInstallsCaseDataList();

	//************************************
	// Method:    GetSortByUseTimeCaseList
	// FullName:  InstallCaseModelEx::GetSortByUseTimeCaseList
	// Access:    public 
	// Returns:   vector<InstallsCaseData *>
	// Qualifier:
	// Qualifier:	����ʹ��ʱ�����򷽰�
	//************************************
	vector<InstallsCaseData *> GetSortByUseTimeCaseList();

	//************************************
	// Method:    GetCaseById
	// FullName:  InstallCaseManagerEx::GetCaseById
	// Access:    public 
	// Returns:   InstallsCaseData*
	// Qualifier:
	// Parameter: int caseId ����ID
	// Explain:		����id��ȡ����
	//************************************
	InstallsCaseData* GetCaseById(int caseId);

	//************************************
	// Method:    GetSystemCaseData
	// FullName:  InstallCaseManagerEx::GetSystemCaseData
	// Access:    public 
	// Returns:   InstallsCaseData*
	// Qualifier:
	// Parameter: _INSTALLS_CASE_DATA_TYPE caseType
	// Explain:		��ȡϵͳ����
	//************************************
	InstallsCaseData* GetSystemCaseData(_INSTALLS_CASE_DATA_TYPE caseType );

	//��ȡ����Ĭ������
	wstring GetNewDefaultCaseName(_INSTALLS_CASE_DATA_TYPE type);

	//************************************
	// Method:    GetSameAppList
	// FullName:  InstallCaseModelEx::GetSameAppList
	// Access:    public 
	// Returns:   set<wstring>
	// Qualifier: ��ȡcaseData�к�appInfo����һ������������caseData�к���appInfo������appInfo����
	// Parameter: InstallsAppInfo * caseData
	// Parameter: InstallsAppList * newAppList
	//************************************
	set<wstring> GetSameAppList(InstallsCaseData* caseData,InstallsAppInfo* appInfo,vector<wstring>* pathList);

	//ֻ��ȡ���İ汾��applist
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

	//��������
	void LoadDataInternal();

	//����Web����
	//mPaseParam: 
	//	GetBool():	bLoadDataBase
	void LoadWebCaseInternal(bool bLoadDataBase, int nUserId = 0, bool bCpa = false);

	//���ios����
	void CheckIOSUpgrateInternal();

	//���android����
	void CheckAndroidUpgrateInternal();

	//���ios��������
	void CheckIOSLegalUpgradeInternal();

	//ˢ�·���
	//mPaseParam: 
	//	GetInt():	����ID
	void RefreshCaseInternal( int caseId, bool isOnlyShowLaserVer );

	//���ط����µ�����б�
	//mPaseParam: 
	//	GetInt():	����ID
	void LoadAppListInternal(int caseId, wstring type);

	//�������
	//mPaseParam: 
	//	GetVoid():	AppSearchContextEx
	void SearchAppInternal(void* pParam );

	//�������
	//Ϊ��������Ч��ԭ��SearchAppInternal������Ĳ��ִ�������һ���µķ���
	void SearchAppInternalEx(AppSearchContextEx* context, InstallsCaseData* caseData);

	//�༭����
	//mPaseParam: 
	//	GetInt():	����ID
	//	GetVoid():	���ID�б�
	void EditCaseInternal(int caseId, wstring caseName, void* pathList);

	//�������ӵ�����
	//	GetInt():	����ID
	//	GetBool():	�϶�GetVoid()���������
	//	GetStr():	"Refresh"����Ҫˢ�·������ļ��б����½���
	//	GetVoid():	��GetBoolΪtrue�������·���б�Ϊfalse�������ID�б�
	// isShowNotify �Ƿ���ʾ����Ѿ���ӵ�������tureΪ��ʾ��falseΪ����ʾ
	void AddAppToCaseInternal(int caseId, wstring isRefresh, bool isPathList, void* pVoidParam = NULL, \
		void* pWstring = NULL, void* pInt = NULL,bool isShowNotif = true);
	//�Ƴ�����
	//	GetVoid():	����ID�б�
	void RemoveCaseInternal(void* pBaseParam);

	//�Ƴ������е����
	//	GetInt():	����ID
	//	GetVoid():	���ID�б�
	void RemoveAppFromCaseInternal( int caseId, wstring isRefresh, bool removeFile, void* pBaseParam = NULL, bool isUpdate=false);

	//�������
	//	GetInt():	����ID
	//	GetVoid():	���ID�б�
	void UpgrateAppInternal(int caseId, void* pBaseParam=NULL);

	//�������������
	//	GetInt():	����ID
	//	GetVoid():	���ID�б�
	void UpgradeAppToLegalInternal(int caseId, void* pBaseParam=NULL);

	//��װ���
	//	GetStr():	�豸ID
	//	GetInt():	����ID
	//	GetVoid():	���ID�б�
	void InstallAppInternal(int caseId, wstring deviceId, void* pBaseParam=NULL);

	//���app�ļ�����
	//pBaseParam->GetVoidParam: at(0)����ios�б� at(1)����android�б�
	void AddFilesToLibInternal(void* pParam=NULL, void* pSpevparam=NULL);

//private:
	//��������
	InstallsCaseData*  CreateCase(wstring caseName, _INSTALLS_CASE_DATA_TYPE caseType);

	//����ָ���ļ���
	void LoadDefaultFolder();

	//���ط���
	void LoadCaseList();

	//�鿴�ļ����б��Ƿ��б仯
	void CheckFolderChanged(vector<AppCountByFolder*>* folderList);

	//�ӿ��д�������
	void CreateCaseFromLib(_INSTALLS_CASE_DATA_TYPE caseType);

	//���ļ����д�������
	void CreateCaseFromFolder(_INSTALLS_CASE_DATA_TYPE caseType);

	//ͨ������б�������
	void CreateCaseByAppList(_INSTALLS_CASE_DATA_TYPE caseType, vector<wstring>* pathList);

	//�����������Ŀ¼�����ҷ��ص�ǰ������folders�� foldersΪNULLʱ��ֻ����ɨ��
	void GetRefreshFolderList(InstallsCaseData* casedata, vector<AppCountByFolder*>* folders = NULL);

	//����fileList��ȡҪ�������������ͺ����·���б�
	void GetCaseTypeAndPathList(_INSTALLS_CASE_DATA_TYPE& caseType, vector<wstring>* fileList, vector<wstring>* pathList);

	//��ȡҪˢ�µ�����б�
	void GetRefreshFileList(InstallsCaseData* caseData, vector<wstring> &pathList);

	//ɾ���װ汾���
	void RemoveDiffVerApp( InstallsAppInfo* appInfo, wstring oldFilepath );

	//ˢ�·������������
	void RefreshSimpleCase(InstallsCaseData* caseData,bool isOnlyShowLastVer);

	//ˢ�������������
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