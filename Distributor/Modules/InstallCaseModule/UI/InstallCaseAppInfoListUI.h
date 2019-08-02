#pragma once

INSTALLCASE_BEGIN

#define IDR_MENU_APP_LIST 54000
#define IDR_MENU_ADD_TO_CASE 54001
#define IDR_MENU_ADD_TO_NEW_CASE 54001001
#define IDR_MENU_DELETE_SELECT_APP 54002
#define IDR_MENU_INSTALL_APP 54003
#define IDR_MENU_OPENAPPFOLDER 54004
#define IDR_MENU_UPGRATE 54005
#define IDR_MENU_UPGRADEALL 54006
#define IDR_MENU_UPGRADEALLTOPIRACY 54007
#define IDR_MENU_UPGRADEALLTOLEGAL 54008
#define IDR_MENU_UPGRADEALLTOSAMELEGAL 54009

struct AppSearchContextEx;

class InstallCaseUI;
class LocalAppinfoList;
class InstallCaseButtonItem;

class CaseAppSelectObserve
{
public:
	virtual void Selected() = 0;
};

class InstallCaseAppInfoListUI
	: public CCompositeUI
	, public BaseHandler<DeviceConnectMsg>
	, public BaseHandler<DeviceLinkOutMsg>
	, public BaseHandler<DeviceActivatedMsg>
	, public BaseHandler<ResponseDeviceNameMsg>
	//, public MessageReceiver
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CaseFormBtnInstall", OnSingleInstall)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CaseFormBtnUpdate", OnSingUpgrade)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CaseFormBtnRemove", OnSingleRemove)
		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"AppFilterPhoneType", OnPhoneTypeComboChecked)
		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"AppFilterPurchaser", OnPurchaserComboChecked)
		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"AppFilterState", OnStateComboChecked)
		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"AppFilterFolder", OnFolderComboChecked)
		EVNET_ID_HANDLER(UINOTIFY_CBN_BEFOREDROPDOWN, L"AppFilterFolder", OnFolderComboDropDown)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"EmptyAddAppBtn", OnEmptyAddAppBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"ClearFilterBtn", OnClearFilterBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"EditCaseBtn", OnEditCaseBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"UpgrateAppBtn", OnUpgrateAppBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AddNewAppBtn", OnAddNewAppBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AddToCaseBtn", OnAddToCaseBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"RemoveAppBtn", OnRemoveAppBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"RefreshCaseBtn", OnRefreshCaseBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"ShowLastVerCheckBox", OnShowLastVerCheckBoxClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"OnekeyInstallBtn", OnOnekeyInstallBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"SelectLayoutClose", OnSelectLayoutCloseClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CloseFilterInfoBtn", OnCloseFilterInfoBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Expander", OnExpanderClick)
		EVNET_ID_HANDLER(UINOTIFY_EN_CHANGE,L"Edit", OnSearch)//�༭���������
		EVNET_ID_HANDLER(UINOTIFY_CLICK,L"Clear", OnSearchClear)//��ձ༭����������
		EVNET_ID_HANDLER(UINOTIFY_TVN_CHECK, L"List", OnItemSelect)
		EVNET_ID_HANDLER(UINOTIFY_DROPFILES, L"List", OnDropFiles)
		EVNET_ID_HANDLER(UINOTIFY_RCLICK, L"List", OnShowMenuAppList)
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_APP_LIST, OnClickMenu)
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_ADD_TO_CASE, OnClickMenu)
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_UPGRADEALL, OnClickUpgradeMenu)
		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"ComboBoxDevice", OnDeviceComboChecked)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"UpgradeBtn", OnUpgradeBtnClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"LegalUpgradeBtn", OnLegalUpgradeBtnClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"UpgrateAllAppsBtn", OnUpgradeAllAppsBtnClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AndUpgrateAllAppsBtn", OnUpgradeAllAppsBtnClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"UpgrateAllAppsMenuBtn", OnUpgradeAllAppsMenuBtnClick);
	UIEND_MSG_MAP
	InstallCaseAppInfoListUI(HWND hWnd, InstallCaseUI* caseUI, InstallsCaseData* caseData);
	~InstallCaseAppInfoListUI(void);

public:
	//���б�������
	void AddAppInfoToView(int caseId, InstallsAppInfo* appInfo );

	//���б��Ƴ����
	void RemoveAppInfoFromView(wstring appPath);

	//����б�
	void ClearAppFromView(AppSearchContextEx* context);

	//����б�
	void ClearAppFromView();

	//�����Ƿ����ֵײ���ѡ�����б�
	void SetSelectListVisible(bool bVisible);

	//��ӵײ���ѡ�����б���
	void AddButtonItem(InstallsAppInfo* appInfo);

	//�Ƴ��ײ���ѡ�����б���
	void RemoveButtonItem(InstallCaseButtonItem* item);

	//ȡ������
	void CancelAppUpgrate( wstring appPath );

	void CancelAppUpgrateEx( wstring appPath);

	void HideOperateColumn();

	//��ȡ��ǰ�б���������
	InstallsCaseData* GetCaseData(){return _pCaseData;}

	//ѡ�����б�
	void GetSelectPathList(vector<wstring>& pathList);

	//�����Ƿ���ʾ���°汾
	void SetLastVerChecked(bool bVisible);

	//�Ƿ���ʾ���°汾
	bool IsLastVerChecked();

	//����
	void Search();

	//List��ѡ�����
	void SetAppSelectObserve(CaseAppSelectObserve* obs);

	//�����б������ʾ
	void UpdateCaseInfo();

	//��ȡ��ǰListItem�������
	int GetAppInfoItemsCount();

	//���ˢ�²���
	void FinishRefresh();

	void OnFilter();

private:
	void OnCreate();	

	void InitControl();

	void InitFolders();

	void InitDeviceList();

	void AddSelectAppToCase(int caseId);

	void AddDeviceMenu(CMenuUI* pMenu);

	void AddCaseMenu(CMenuUI* pMenu);

	void RemoveSelectApp();

	void UpgrateSelectApp();

	void InstallSelectApp(wstring sn);

	void OpenAppFolder();

	bool CheckSelected(wstring id);

	bool  ListFocused();

	// ������Ϣ
	//��ݼ���Ϣ��ʱ��������
	//virtual void Receive(MESSAGE_TYPE msg, const BaseParam* pParm);

	bool OnSingleInstall(TNotifyUI* pNotify);

	bool OnSingUpgrade(TNotifyUI* pNotify);

	bool OnSingleRemove(TNotifyUI* pNotify);

	bool OnEmptyAddAppBtnClick(TNotifyUI* pNotify);

	bool OnClearFilterBtnClick(TNotifyUI* pNotify);

	bool OnAddToCaseBtnClick(TNotifyUI* pNotify);

	bool OnEditCaseBtnClick(TNotifyUI* pNotify);

	bool OnUpgrateAppBtnClick(TNotifyUI* pNotify);

	bool OnAddNewAppBtnClick(TNotifyUI* pNotify);

	bool OnRemoveAppBtnClick(TNotifyUI* pNotify);

	bool OnRefreshCaseBtnClick(TNotifyUI* pNotify);

	bool OnShowLastVerCheckBoxClick(TNotifyUI* pNotify);

	bool OnOnekeyInstallBtnClick(TNotifyUI* pNotify);

	bool OnSelectLayoutCloseClick(TNotifyUI* pNotify);

	bool OnCloseFilterInfoBtnClick(TNotifyUI* pNotify);

	bool OnExpanderClick(TNotifyUI* pNotify);

	bool OnShowMenuAppList(TNotifyUI* pNotify);

	bool OnItemSelect(TNotifyUI* pNotify);

	bool OnClickMenu(TNotifyUI* pNotify);

	bool OnSearch(TNotifyUI* pNotify);

	bool OnSearchClear(TNotifyUI* pNotify);

	bool OnPhoneTypeComboChecked(TNotifyUI* pNotify);

	bool OnPurchaserComboChecked(TNotifyUI* pNotify);

	bool OnStateComboChecked(TNotifyUI* pNotify);

	bool OnFolderComboChecked(TNotifyUI* pNotify);

	bool OnFolderComboDropDown(TNotifyUI* pNotify);

	bool OnDropFiles(TNotifyUI* pNotify);

	

	void ShowAppListMenu(TNotifyUI* pNotify, bool isContextMenu = true);
	
	bool Handle(const DeviceConnectMsg* pMsg);

	bool Handle(const DeviceLinkOutMsg* pMsg);

	bool Handle(const DeviceActivatedMsg* pMsg);

	bool Handle(const ResponseDeviceNameMsg* pMsg);

	virtual void OnUsbConnect(wstring sn);

	virtual void OnUsbDisconnect(wstring sn);

	virtual void OnDeviceActivateFail(wstring sn);

	virtual void OnDeviceNameChanged(wstring sn);

	void RemoveDevice(wstring sn);

	bool OnDeviceComboChecked(TNotifyUI* pNotify);

	bool OnUpgradeBtnClick(TNotifyUI* pNotify);

	bool OnLegalUpgradeBtnClick(TNotifyUI* pNotify);

	bool OnUpgradeAllAppsBtnClick(TNotifyUI* pNotify);

	bool OnUpgradeAllAppsMenuBtnClick(TNotifyUI* pNotify);

	bool OnClickUpgradeMenu(TNotifyUI* pNotify);
	
	void RefreshUpgradeCount();

	void ShowFilterInfo(bool show);
private:
	map<wstring, CListItemUI*> _mapPurchaser;
	map<wstring, int> _mapPurchaserCount;
	set<wstring> _mapPurchaserAppPath;

	InstallsCaseData* _pCaseData;

	AppSearchContextEx* _pSearchContext;

	InstallCaseUI* _pInstallCaseUI;

	CLayoutUI* _pListEmpty;
	CLayoutUI* _pListFilterEmpty;
	CListUI* _pList;	
	LocalAppinfoList* _pListOwner;
	CLayoutUI* _pBottom;

	CLayoutUI* _pFilterInfo;
	CLabelUI* _pFilterInfoLabel;
	CButtonUI* _pClearFilterInfoBtn;
	CButtonUI* _pCloseFilterInfoBtn;
	
	CLayoutUI* _pSelectListLayout;
	CLayoutUI* _pSelectItemContainer;
	CCheckBoxUI* _pExpander;
	CButtonUI* pSelectLayoutClose;

	CLabelUI* _pCaseInfoLbl;

	CControlUI* _pFilterPanel;
	CComboBoxUI* _pPhoneTypeComboBox;
	CComboBoxUI* _pPurchaserComboBox;
	CComboBoxUI* _pStateComboBox;
	CComboBoxUI* _pFolderComboBox;
	CSearchEditUI* _pSearchEdit;
	CButtonUI* _pSearchClear;

	CButtonUI* _pEmptyAddAppBtn;
	CButtonUI* _pEditCaseBtn;
	CButtonUI* _pUpgrateAppBtn;
	CButtonUI* _pAddNewAppBtn;
	CButtonUI* _pAddToCaseBtn;
	CButtonUI* _pRemoveAppBtn;
	CButtonUI* _pRefreshCaseBtn;
	CButtonUI* _pOnekeyInstallBtn;
	CButtonUI* _pClearFilterBtn;
	CButtonUI* _pAndUpgradeAllBtn;
	CButtonUI* _pUpgradeAllBtn;
	CButtonUI* _pUpgrateAllMenuBtn;
	CLayoutUI* _pUpgradeAllLayout;

	CCheckBoxUI* _pShowLastVerCheckBox;
	CComboBoxUI* _pComboBoxDevice;

	CaseAppSelectObserve* _pAppSelectObserve;

	bool _bSelectListVisible;

	vector<wstring> _updatePathList;

	bool _bRefreshing;
	bool _bClearingFilter;

	_APP_UPGRADE_TYPE _eAppUpgradeType;
};


INSTALLCASE_END