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
	// Explain:		��������
	//************************************
	void Load();

	//************************************
	// Method:    LoadWebCase
	// FullName:  InstallCaseManagerEx::LoadWebCase
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: bool bLoadDataBase
	// Explain:		�������緽��
	//************************************
	void LoadWebCase(bool bLoadDataBase, int nUserId = 0, bool bCpa = false);

	//************************************
	// Method:    CheckPiracyUpgrateAppList
	// FullName:  InstallCaseManagerEx::CheckPiracyUpgrateAppList
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: InstallsCaseData * caseData
	// Explain:		����������
	//************************************
	int CheckPiracyUpgrateAppList(InstallsCaseData* caseData);

	//************************************
	// Method:    CheckUpgrateLegalAppList
	// FullName:  InstallCaseManagerEx::CheckUpgrateLegalAppList
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: InstallsCaseData * caseData
	// Explain:		��������������
	//************************************
	int CheckLegalUpgrateAppList(InstallsCaseData* caseData);

	//************************************
	// Method:    LoadAppList
	// FullName:  InstallCaseManagerEx::LoadAppList
	// Access:    public 
	// Returns:   InstallsCaseData*
	// Qualifier:
	// Parameter: int caseId	����Id
	// Explain:		���ط����е�����б�
	//************************************
	InstallsCaseData* LoadAppList(int caseId);

	//************************************
	// Method:    CreateCase
	// FullName:  InstallCaseManagerEx::CreateCase
	// Access:    public 
	// Returns:   InstallsCaseData*
	// Qualifier:
	// Parameter: wstring caseName	��������
	// Parameter: _INSTALLS_CASE_DATA_TYPE type	��������
	// Explain:		��������
	//************************************
	InstallsCaseData* CreateCase(wstring caseName,_INSTALLS_CASE_DATA_TYPE type);

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
	// Method:    AddAppToCaseByPath
	// FullName:  InstallCaseManagerEx::AddAppToCaseByPath
	// Access:    public 
	// Returns:   InstallsAppInfo*
	// Qualifier:
	// Parameter: InstallsCaseData * caseData
	// Parameter: wstring path	���·��
	// Explain:		���·����Ӧ�����������
	//************************************
	InstallsAppInfo* AddAppToCaseByPath(InstallsCaseData* caseData, wstring path, bool bRefresh = false, int fid=0 );

	//************************************
	// Method:    AddAppToCase
	// FullName:  InstallCaseManagerEx::AddAppToCase
	// Access:    public 
	// Returns:   InstallsAppInfo*
	// Qualifier:
	// Parameter: InstallsCaseData * caseData
	// Parameter: wstring path	���path
	// Explain:		������������·����Ӧ�����������
	//************************************
	InstallsAppInfo* AddAppToCase(InstallsCaseData* caseData, wstring path);

	//************************************
	// Method:    RemoveCase
	// FullName:  InstallCaseManagerEx::RemoveCase
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: int caseId
	// Explain:		ɾ������
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
	// Explain:		ͨ�����·���ӷ�����ɾ�����
	//************************************
	bool RemoveAppFromCaseByPath(InstallsCaseData* caseData, wstring path);
	
	//************************************
	// Method:    GetOwnerCase
	// FullName:  InstallCaseManagerEx::GetOwnerCase
	// Access:    public 
	// Returns:   InstallsCaseData*
	// Qualifier:
	// Parameter: InstallsAppInfo * appInfo
	// Explain:		��ȡ������ڵ�ϵͳ����
	//************************************
	InstallsCaseData* GetSystemCaseData(InstallsAppInfo* appInfo);

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

	//************************************
	// Method:    FindDiffVerApp
	// FullName:  InstallCaseManagerEx::FindDiffVerApp
	// Access:    public 
	// Returns:   InstallsAppInfo*
	// Qualifier:
	// Parameter: InstallsCaseData * caseData
	// Parameter: InstallsAppInfo * appinfo
	// Explain:		���ҵװ汾���
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
	// Explain:		����������
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
	// Explain:		����ļ���
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
	// Explain:		��ӿ�������ļ���
	//************************************
	void AddFolder(wstring folderPath, _INSTALLS_CASE_DATA_TYPE type);

	//************************************
	// Method:    RemoveFolder
	// FullName:  InstallCaseManagerEx::RemoveFolder
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: InstallsAppInfo * appInfo
	// Explain:		�Ƴ��ļ���
	//************************************
	void RemoveFolder(InstallsAppInfo* appInfo);


	//************************************
	// Method:    RemoveFolder
	// FullName:  InstallCaseManagerEx::RemoveFolder
	// Access:    public 
	// Returns:   void
	// Qualifier:	�Ƴ�������ļ��У��Ƴ���������ļ���ר�ã�
	// Parameter: int appFolderID
	//************************************
	void RemoveFolder( int appFolderID);

	void CheckPlanUpgrateAppList();

	void IncCaseInstallCount(InstallsCaseData* caseData);

	//�������
	InstallsAppInfo* AnalyzeApp(wstring filePath);

	//sumary:���ݰ�idɾ��װ������
	void DeleteCaseByPackId( int PackId );

	bool Handle(GetInstallPathByCaseIdMsg* pMsg);

	bool Handle(AnalyzeAppMsg* pMsg);

	

private:

	//������
	InstallsAppInfo* AddApp(InstallsCaseData* caseData, InstallsAppInfo* appInfo );

	//����
	void CopyTo(InstallsAppInfo* srcInfo, InstallsAppInfo* desInfo);

	//��ȡ�����
	InstallsAppInfo* GetSystemAppInfo(wstring path);

	//��ȡ��ͬ�������Ͱ汾
	void GetLastVerAppList(InstallsCaseData* casedata,vector<InstallsAppInfo*>* appList, int isLegal=0 ,vector<InstallsAppInfo*>* pSameAppList = NULL);

	//��ȡ������Ϣ�Ժ����÷��������������
	void SetAppsPiracyUpgrade( vector<AppUpdateInfo*> updateapps, _INSTALLS_CASE_DATA_TYPE dataType );

	//��ȡ����������Ϣ�Ժ����÷��������������
	void SetAppsLegalUpgrade( vector<AppUpdateInfo*> updateapps );

	//���½���������Լ�װ���������
	void ReAnalyzeAppForItemID();

public:
	vector<InstallsCaseData*> InstallCaseDataList;	//�����б�

	vector<InstallsCaseData*> NetInstallPlans;	//���緽���б�

	vector<AppCountByFolder*> AppFolderList;

private:
	static InstallCaseManagerEx* _pInstallsCaseManager;

	int _mFolderId;

	bool _bLoaded;									//�Ƿ��Ѽ������
	bool _bIOSChecking;
	bool _bAndroidChecking;

	bool _bIOSLegalChecking;
	bool _bAndroidLegalChecking;

	InstallsCaseDBEx* _pInstallsCaseDB;

	NetInstallsCaseDB* _pNetInstallsCaseDB;

	InstallsCaseData* _pUpgrateCaseDataIOS;//IOS������Ӧ�ã����棩
	InstallsCaseData* _pUpgrateCaseDataAndroid;//android������Ӧ��

	InstallsCaseData* _pUpgrateCaseDataIOSLegal;//IOS������Ӧ�ã����棩
};

INSTALLCASE_END