#pragma once
#include "logic/InstallsCase/InstallCaseModelEx.h"
#include "logic/InstallsCase/InstallCaseInterface.h"
#include "LeftView.h"

INSTALLCASE_BEGIN

#define IDR_MENU_NEW_CASE 54017

#define IDR_MENU_CASE_LIST 54030

class InstallCaseAppFolderListUI;
class InstallCaseAppInfoListUI;
class InstallCaseListUI;
class AddEditCaseDialog;

class InstallCaseUI
	: public BaseView
	, public InstallCaseInterface
	, public BaseHandler<InstallCaseMsg>
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"Tree", OnCaseSelChanged)//�����ѡ���¼�
		EVNET_ID_HANDLER(UINOTIFY_RCLICK, L"Tree", OnShowTreeMenu)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AppListIOSBtn", OnTabItemClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"UpgrateIOSBtn", OnTabItemClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AppListAndroidBtn", OnTabItemClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"UpgrateAndroidBtn", OnTabItemClick)
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_NEW_CASE, OnClickMenuNewCase)
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_CASE_LIST, OnClickMenuCaseList)
	UIEND_MSG_MAP

	InstallCaseUI(LeftView* pLeftView = NULL);
	~InstallCaseUI(void);

	virtual wstring GetStyleID();

	//�༭�����������༭��������
	virtual void GetCaseInfoFromLib(wstring& caseName, _INSTALLS_CASE_DATA_TYPE& caseType, vector<wstring>* pathList);

	//�ӿ��д��������������½���������
	virtual void GetEditCaseIdList(InstallsCaseData* caseData, wstring& caseName, vector<wstring>* pathList);

	//���ļ����д������������û����뷽�����Ʋ�ѡ���ļ���·��
	virtual void GetCaseInfoFromFolder(wstring& caseName, _INSTALLS_CASE_DATA_TYPE& caseType, wstring& folder);

	//��ȡ��������
	virtual wstring GetFolder(_INSTALLS_CASE_DATA_TYPE type);

	virtual void GetAppPath(_INSTALLS_CASE_DATA_TYPE type, vector<wstring>* pathList);

	//ȷ���Ƿ�Ҫɾ��
	virtual void GetCaseName(wstring& caseName);

	//��ȡ�ļ���·��
	virtual bool RemoveAppConfirm(bool isRemoveFile);

	virtual bool RemoveCaseConfirm();

	virtual bool RemoveFolderConfirm();

	virtual bool IsNeedInstalliTunesConfirm();

	//ѡ�з���
	void SelectCaseNode(wstring caseId);

	//��Ϣ����
	bool InstallCaseUI::Handle(const InstallCaseMsg* pMsg=NULL);


    //������б�ڵ�
    void ShowCaseList();

	//ͳ�ƽӿ�
	void StatisSingleInstall();
	void StatisSingUpgrade();
	void StatisSingleRemove();
	void StatisRemoveCase();
	void StatisShowLaseVer();
	void StatisCaseNodeClick();
	void StateAddAppAndBatchUpgrate();
	void StatisCreateNewPlan();
	void StatisAddAndroidFolder();
	void StatisAddIOSFolder();
	void StatisSwitchToUpgrate();
	void StatisBatchRemove();
	void StatisOnKeyInstall();

private:
	//�����ؼ�
	void OnCreate();

	//��ʼ���б�
	void OnInitComposite();

	//��ӷ���������
	void AddCaseToView(InstallsCaseData* caseData);

	//�ӽ����Ƴ�����
	void RemoveCaseFromView(int caseId);

	//������
	void AddAppToView(int caseId, InstallsAppInfo* appInfo);

	//ɾ�����
	void RemoveAppFromView(int caseId, wstring appPath);

	void ClearAppFromView( int caseId, AppSearchContextEx* context );

	void ClearAppFromView( int caseId );

	//���������������
	void UpdateUpgrateCount(_INSTALLS_CASE_DATA_TYPE caseType, int count);

	//���·�����Ϣ
	void UpdateCaseView(InstallsCaseData* caseData);

	//���ɷ�������
	wstring GetCountStr(InstallsCaseData* caseData);

	//��ʾ�����б����˵�
	bool OnShowTreeMenu(TNotifyUI* pNotify);

	//�½�����
	bool OnClickMenuNewCase(TNotifyUI* pNotify);

	//�༭��ɾ������
	bool OnClickMenuCaseList(TNotifyUI* pNotify);

	//��ѡ����ı�
	bool OnCaseSelChanged(TNotifyUI* pNotify);

	//����б�������б��л�
	bool OnTabItemClick(TNotifyUI* pNotify);

	//����û�����
	void AddUserCaseToView(InstallsCaseData* caseData);

	void CancelAppUpgrate( int caseId, wstring appPath );	

	//ˢ�·���Itemsͼ��
	void RefreshCaseItemsIcon( int caseId );

	//��ʾ��������б�
	void ShowUserCaseAppList(CLayoutUI* pContainer, InstallsCaseData* caseData);

	//����Ӧ�ÿ���ʾ�������
	void UpdateSystemCaseCount( int caseID, int count );

	//��ɸ���
	void FinishRefresh(int caseID);

public:
	InstallCaseModelEx Model;

private:
	map<int,InstallCaseAppInfoListUI*> _mapUserCaseAppList;

	AddEditCaseDialog* _pAddEditCaseDialog;

	CTreeUI* _pTree;
	CTreeNodeUI* _pSoftLibNode;
	CTreeNodeUI* _pCaseListNode;

	CRadioUI* _pAppListIOSBtn;
	CRadioUI* _pUpgrateIOSBtn;
	CRadioUI* _pAppListAndroidBtn;
	CRadioUI* _pUpgrateAndroidBtn;

	CLayoutUI* _pFolderLayout;
	CLayoutUI* _pIOSLibSoftLayout;
	CLayoutUI* _pAndroidLibSoftLayout;
	CLayoutUI* _pUserCaseListLayout;
	CLayoutUI* _pUserCaseLayout;

	InstallCaseAppFolderListUI* _pFolderListUI;
	InstallCaseListUI* _pCaseListUI;

	LeftView*   _pLeftView;
};

INSTALLCASE_END