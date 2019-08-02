#ifndef CaseListInfoUI_h__
#define CaseListInfoUI_h__

#include "LocalAppinfoList.h"
#include "Logic/NetInstallsCase/NetInstallPlanTaskManager.h"

#define IDR_MENU_UPGRADEALL				55000
#define IDR_MENU_UPGRADEALLTOPIRACY		55001
#define IDR_MENU_UPGRADEALLTOLEGAL		55002
#define IDR_MENU_UPGRADEALLTOSAMELEGAL	55003

class CaseListInfoUI 
	: public CWindowUI
	, public INotifyUI
	, public BaseHandler<InstallCaseMsg>
	, public BaseHandler<DeviceLinkOutMsg>
{
public:
    virtual void OnSetWindowRgn(){};
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"TypeTab", OnTypeTabChange)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"UpgradeBtn", OnUpdateBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"LegalUpgradeBtn", OnLegalUpdateBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"UpgradeAllAppsBtn", OnUpdateAllAppsBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AndUpgradeAllAppsBtn", OnUpdateAllAppsBtnClick)
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_UPGRADEALL, OnClickUpgradeMenu)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"UpgradeAllAppsMenuBtn", OnUpgradeAllAppsMenuBtnClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"InstallBtn", OnInstallBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CloseBtn", OnCloseBtnClick)
	UIEND_MSG_MAP

	CaseListInfoUI(int caseDataid = 0, wstring devId = L"", CButtonUI* ownerBtn = NULL,NetInstallPlanTaskManager* pNetCaseManager=NULL);
	~CaseListInfoUI();

	virtual void OnCreate();

	virtual void InsertAppInfo(InstallsAppInfo* appInfo, bool isUpdate = false);

	//更新app列表UI
	virtual void RefreshAppInfoList();

	bool Handle(const InstallCaseMsg* pMsg);

	bool Handle(const DeviceLinkOutMsg* pMsg);
private:

	void GetIdList( vector<wstring>& pathList );

	bool OnTypeTabChange(TNotifyUI* pNotify);

	bool OnInstallBtnClick(TNotifyUI* pNotify);

	bool OnCloseBtnClick(TNotifyUI* pNotify);

	bool OnUpdateBtnClick(TNotifyUI* pNotify);

	bool OnLegalUpdateBtnClick(TNotifyUI* pNotify);

	bool OnUpdateAllAppsBtnClick(TNotifyUI* pNotify);

	bool OnClickUpgradeMenu(TNotifyUI* pNotify);

	bool OnUpgradeAllAppsMenuBtnClick(TNotifyUI* pNotify);

	void ClearUIData();

	void RefreshUpgradeCount();

private:

	LocalAppinfoList* _pAppinfoListControl;	//list子项显示内容生成器
	CListUI* _appListCtrl;					//list控件
	CLabelUI* _LabInfo;

	InstallsCaseData* _pCaseData;

	int _CaseDataid;
	wstring _mCurrentDevId;
	CButtonUI* _pInstallBtn;
	CButtonUI* _pOwnerButton;

	CTabUI*         _pTab;

	_APP_UPGRADE_TYPE _eAppUpgradeType;

	CButtonUI* _pAndUpgradeAllBtn;
	CButtonUI* _pUpgradeAllBtn;
	CButtonUI* _pUpgrateAllMenuBtn;
	CLayoutUI* _pUpgradeAllLayout;

	NetInstallPlanTaskManager* _pNetCaseManager;
};

#endif