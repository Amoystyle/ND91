#pragma once

#include "UI/CommonUIFunc.h"
#include "Logic/AppModel.h"
#include "Logic/ApplicationInfoInterface.h"
#include "AppListBodyItemData.h"

App_CC_BEGIN
#define MENU_ID_INSTALL          51000
#define MENU_ID_INSTALL_FILE     51001
#define MENU_ID_INSTALL_FOLDER   51002

#define MENU_ID_TWO_LIST             51003
#define MENU_ID_LIST_MOVE            51004
#define MENU_ID_LIST_UNINSTALL       51005

#define MENU_ID_ONE_LIST             51006

#define IDR_MENU_UPGRADEALL				51010
#define IDR_MENU_UPGRADEALLTOPIRACY		51011
#define IDR_MENU_UPGRADEALLTOLEGAL		51012
#define IDR_MENU_UPGRADEALLTOSAMELEGAL	51013

// ������������
class ApplicationInfoUI
	: public ApplicationInfoInterface  // ʵ��Model����ķ���
	//, public TaskUIObserver            // ��������������Ϣ
	, public BaseView
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"InstallBtn", OnInstallApp);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"InstallBtnMenu", OnInstallApp);

		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Uninstall", OnMultipleUninstallApp)
		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"AppTab", OnAppTabChange)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"ButtonMoveTo", OnSingleMoveTo)

		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"UserButtonUninstall", OnUserSingleUninstallApp)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"SystemButtonUninstall", OnSystemSingleUninstallApp)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"MoveToSD", OnMultipleMoveToSD)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"MoveToPhone", OnMultipleMoveToPhone)

		EVNET_ID_HANDLER(UINOTIFY_EN_CHANGE,L"Edit", OnSearch)//�༭���������
		EVNET_ID_HANDLER(UINOTIFY_CLICK,L"Clear", OnSearchClear)//��ձ༭����������
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"ClearFilterBtn", OnClearFilterBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CloseFilterInfoBtn", OnCloseFilterInfoBtnClick)

		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, MENU_ID_INSTALL, OnMenuItemClick)
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, MENU_ID_TWO_LIST, OnMenuItemClick)
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, MENU_ID_ONE_LIST, OnMenuItemClick)
		
		EVNET_ID_HANDLER(UINOTIFY_RCLICK, L"UserListCtrl", OnListRightClickList);
		EVNET_ID_HANDLER(UINOTIFY_RCLICK, L"SystemListCtrl", OnListRightClickList);

		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_UPGRADEALL, OnClickUpgradeMenu);

		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"PiracyUpBtn", OnUpgradeBtnClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"LegalUpBtn", OnLegalUpgradeBtnClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"UpgradeAllAppsBtn", OnUpgradeAllAppsBtnClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AndUpgradeAllAppsBtn", OnUpgradeAllAppsBtnClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"UpgradeAllAppsMenuBtn", OnUpgradeAllAppsMenuBtnClick);
	UIEND_MSG_MAP

	ApplicationInfoUI(HWND hWnd, wstring id);
	~ApplicationInfoUI();

	virtual wstring GetStyleID();

	void Release();

	// �ؼ���Ӧ����
	bool OnInstallApp(TNotifyUI* pNotify);
	bool OnMultipleUninstallApp(TNotifyUI* pNotify);
	bool OnAppTabChange(TNotifyUI* pNotify);
	bool OnSingleMoveTo(TNotifyUI* pNotify);

	bool OnUserSingleUninstallApp(TNotifyUI* pNotify);
	bool OnSystemSingleUninstallApp(TNotifyUI* pNotify);
	bool OnMultipleMoveToSD(TNotifyUI* pNotify);
	bool OnMultipleMoveToPhone(TNotifyUI* pNotify);
	bool OnMenuItemClick(TNotifyUI* pNotify);

	bool OnSearch(TNotifyUI* pNotify);
	bool OnSearchClear(TNotifyUI* pNotify);
	bool OnClearFilterBtnClick(TNotifyUI* pNotify);
	bool OnCloseFilterInfoBtnClick(TNotifyUI* pNotify);

	bool OnListRightClickList(TNotifyUI* pNotify);

	virtual void OnClose();
	virtual void OnCreate();
	virtual void OnShowComponent();; //������ʾ����

	// ���ÿؼ�����
	virtual void SetControlAttribute();

	// ��������list�ؼ����ƶ��¼�
	bool AddListEventDelegate();
	void AddListEventDelegate(CListUI* pList);
	void RemoveListEventDelegate(CListUI* pList);
	bool MoveEventOnList(TEventUI& event);
	void ShowListItemButton(CControlUI* pControl, bool bShow);

	//�ж��Ƿ�ǰlist��ý���
	bool  ListFocused();

	bool DropFilesHandle( vector<wstring> files );

	void InstallCaseLogic(vector<wstring>* files_case, wstring filePath);

	bool HandleAppMsg(UINT message, WPARAM wParam, LPARAM lParam);

	bool HandleCenterMsg(int  nParam);

	bool HandleRefreshModuleMsg(wstring deviceID);

	bool HandleTaskItemMsg(void* pTask, UINT message, wstring curID ,int taskid);
private:
	// ����list�ؼ��û�������
	void CreateListBodyItemUser(AppData* pApp, DEVICE_OS_TYPE deviceType, bool bSave);

	// ����list�ؼ�ϵͳ������
	void CreateListBodyItemSystem(AppData* pApp, DEVICE_OS_TYPE deviceType, bool bSave);

	// ���������ֶ�ƥ��,������ʱ�б�
	void CreateTempListItem(vector<AppData*>& verDatas, wstring strText, DEVICE_OS_TYPE deviceType, bool bLoad);

	// ����list�ؼ���
	AppListBodyItemData* CreateListBodyItem(CListUI* pList,
		AppData* pApp, 
		bool isAndroid);

	// �Ƴ�list���е���
	void RemoveFromList(AppData* pApp,
		CListUI* pList,
		vector<AppListBodyItemData*>& itemList);

	vector<AppData*> GetSelectList(CListUI* pList, 
		vector<AppListBodyItemData*>* _pItemList);

	// iosһЩ����ʾ��USER����
	bool IsNotShowUserAppIos(AppData* pApp);

	//andiroidһЩ����ʾ��system����
	bool IsNotShowSystemrAppAndroid(AppData* pApp);

	// androidһЩ����ʾ��USER����
	bool IsNotShowUserAppAndroid(AppData* pApp);

	// ���ݳ������ݻ�ȡָ������
	AppListBodyItemData* GetItemData(AppData* pApp, vector<AppListBodyItemData*> itemList);

	// ��ѡ�ƶ�����
	bool MultipleMoveToApp(bool bToSDCard);

	// ж�ض�ѡ����
	bool MultipleUninstallApp();

	// ж���û�Ӧ��
	void UserSingleUninstallApp(wstring appId);

	// ж��ϵͳӦ��
	void SystemSingleUninstallApp(wstring appId);

	// �ƶ�Ӧ��
	void SingleMoveTo(wstring appId);

	// ����ID��list�ؼ�����Ӧ����
	AppListBodyItemData* FindItemData(vector<AppListBodyItemData*> itemList, wstring id);

	bool IsExisAppData(vector<AppData*> appDataList, wstring id);

	// �����Ҫ���µ�Ӧ������
	void AddUpdateData(AppData* pApp);
	void ClearUpdateDate();

	// ����List�б�
	void ClearAppListItem();

	//Tab��ť��Ӧ�ó��������
	void ShowTabAppCount();

    //�����б���Ϣ
    void RefreshAppInfo(AppData* pData, bool isMoveAndroidApp=false);

	//�����ƶ���ť��״̬
	void RefreshMoveBtn(bool bShow);

	bool OnUpgradeBtnClick(TNotifyUI* pNotify);

	bool OnLegalUpgradeBtnClick(TNotifyUI* pNotify);

	bool OnUpgradeAllAppsBtnClick(TNotifyUI* pNotify);

	bool OnUpgradeAllAppsMenuBtnClick(TNotifyUI* pNotify);

	bool OnClickUpgradeMenu(TNotifyUI* pNotify);

	void RefreshUpgradeCount();

	void ShowFilterInfo(bool show);

	// �û�����list�ؼ����
	vector<AppListBodyItemData*> _userItemList;

	// ��������µ�Ӧ������
	vector<AppData*> _updateAppDatas;

	// �ͷ�Ӧ������
	void ReleaseAppData();

	// �޸�����ʱ�������������
	void SetSearchData(AppData* pdata);

	// �����û�Ӧ������ (����ʹ��)
	vector<AppData*> _userAppDatas;
	// ����ϵͳӦ������ (����ʹ��)
	vector<AppData*> _systemAppDatas;

	// �û�����list�ؼ�
	CListUI*        _pUserList;

	// ϵͳ����list�ؼ����
	vector<AppListBodyItemData*> _systemItemList;

	// ϵͳ����list�ؼ�
	CListUI*        _pSystemList;

	CTabUI*         _pTab;

	AppModel    _model;    // ҵ���߼���

	bool        _hasSDCard;

	wstring     _appId;     // ��������Ӧ��ID��ж�أ��ƶ���

	bool _bShowSystemAppTab;//�Ƿ���ʾϵͳtab
	CRITICAL_SECTION _updateAppDataLock;       //��������Ӧ�õ���

	CRITICAL_SECTION _csListLock;       //�����б����
    bool _bHaveCheckSDCardState;       

	CButtonUI* _pAndUpgradeAllBtn;
	CButtonUI* _pUpgradeAllBtn;
	CButtonUI* _pUpgrateAllMenuBtn;
	CLayoutUI* _pUpgradeAllLayout;
	_APP_UPGRADE_TYPE _eAppUpgradeType;

	CSearchEditUI* _pSearchEdit;
	CButtonUI* _pSearchClear;
	CLayoutUI* _pFilterInfo;
	CLabelUI* _pFilterInfoLabel;
	CButtonUI* _pClearFilterInfoBtn;
	CButtonUI* _pCloseFilterInfoBtn;

	bool       _bDelete;//�����Ƿ��ͷŵ�
public:
	// ApplicationInfoInterface
	// ��ʾ���ڼ�������ͼƬ
	virtual void ShowLoadingPicture(bool bShow);

	// ��ս�������
	virtual void ClearData();
	virtual void ClearDataAction(bool bUser = true);

	// ��ȡlist�ؼ���ѡ�������
	virtual vector<AppData*> GetSelectDatas();

	//@zjf summary:���debug�£�ios�豸����ȥ������������.�е�������������ClearTempData�ͷ��ڴ�
	void BuildeTempData(const vector<AppData*>* pDatas, vector<AppData*>& vecTempData);
	void ClearTempData(vector<AppData*>& vecTempData);

	// ��list�ؼ���������
	virtual void InsertToAppView(const vector<AppData*>* pDatas);
	virtual void InsertToAppViewAction(const vector<AppData*>* pDatas);

	// ��list�ؼ��Ƴ�����
	virtual void RemoveFromAppView(AppData* pApp);
	virtual void RemoveFromAppViewAction(AppData* pApp);

	// ɾ��appʱ����ʱ����app���ݶ���ȥ��
	virtual void RemoveFromTempListData(AppData* pApp, vector<AppData*>& verDatas);

	// ��ʾӦ��ICON
	virtual void SetAppLogo(AppData* pApp, const DeviceData* pCurDevice);
	virtual void SetAppLogo(const vector<AppData*>* pDatas, const DeviceData* pCurDevice);
	virtual void SetAppLogoWithoutLoad(const vector<AppData*>* pDatas, const DeviceData* pCurDevice);

	// ��ʾӦ�ô�С
	virtual void SetAppSize(const vector<AppData*>* pDatas, const DeviceData* pCurDevice) ;
	virtual void SetAppSizeAction(AppData* pApp, const DeviceData* pCurDevice);

	//����������ť
	virtual void SetUpgradeBtn(const vector<AppDataUpgrade*>* pDatas, const DeviceData* pCurDevice);
	virtual void SetUpgradeBtn(AppDataUpgrade* pApp, const DeviceData* pCurDevice);
	virtual void SetAllUpgradeBtn(bool enabled);

	//����������ť
	virtual void SetLegalUpgradeBtn(const vector<AppDataUpgrade*>* pDatas, const DeviceData* pCurDevice);
	virtual void SetLegalUpgradeBtn(AppDataUpgrade* pApp, const DeviceData* pCurDevice);

	virtual void SetUpgradeBtnEnabled(AppDataUpgrade* pData, bool enabled);

	// ��װ����
	virtual bool InstallApp(bool isFile, wstring wsTitle=L"");

	// ��װ����
	virtual bool InstallApp( vector<wstring> files );

	// ��������
	virtual void UpdateData();
	virtual void UpdateDataAction();

	// ˢ������
	virtual void RefreshData();

	void RefreshUnstallBtn(bool bShow);

	//��ô򿪰�װ�����·��
	void GetAppFilePath(vector<wstring>& file);

    virtual bool IsHaveCheckSDCardState(){ return _bHaveCheckSDCardState;};
    virtual bool GetSDCardState();
    virtual void SendToUI(int nMessage);
};
App_CC_END