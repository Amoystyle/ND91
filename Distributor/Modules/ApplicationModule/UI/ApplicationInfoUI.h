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

// 程序管理界面类
class ApplicationInfoUI
	: public ApplicationInfoInterface  // 实现Model所需的方法
	//, public TaskUIObserver            // 接收任务中心消息
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

		EVNET_ID_HANDLER(UINOTIFY_EN_CHANGE,L"Edit", OnSearch)//编辑框输入完成
		EVNET_ID_HANDLER(UINOTIFY_CLICK,L"Clear", OnSearchClear)//清空编辑框输入内容
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

	// 控件响应函数
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
	virtual void OnShowComponent();; //界面显示触发

	// 设置控件属性
	virtual void SetControlAttribute();

	// 添加鼠标在list控件上移动事件
	bool AddListEventDelegate();
	void AddListEventDelegate(CListUI* pList);
	void RemoveListEventDelegate(CListUI* pList);
	bool MoveEventOnList(TEventUI& event);
	void ShowListItemButton(CControlUI* pControl, bool bShow);

	//判断是否当前list获得焦点
	bool  ListFocused();

	bool DropFilesHandle( vector<wstring> files );

	void InstallCaseLogic(vector<wstring>* files_case, wstring filePath);

	bool HandleAppMsg(UINT message, WPARAM wParam, LPARAM lParam);

	bool HandleCenterMsg(int  nParam);

	bool HandleRefreshModuleMsg(wstring deviceID);

	bool HandleTaskItemMsg(void* pTask, UINT message, wstring curID ,int taskid);
private:
	// 创建list控件用户程序项
	void CreateListBodyItemUser(AppData* pApp, DEVICE_OS_TYPE deviceType, bool bSave);

	// 创建list控件系统程序项
	void CreateListBodyItemSystem(AppData* pApp, DEVICE_OS_TYPE deviceType, bool bSave);

	// 根据搜索字段匹配,创建临时列表
	void CreateTempListItem(vector<AppData*>& verDatas, wstring strText, DEVICE_OS_TYPE deviceType, bool bLoad);

	// 创建list控件项
	AppListBodyItemData* CreateListBodyItem(CListUI* pList,
		AppData* pApp, 
		bool isAndroid);

	// 移除list其中的项
	void RemoveFromList(AppData* pApp,
		CListUI* pList,
		vector<AppListBodyItemData*>& itemList);

	vector<AppData*> GetSelectList(CListUI* pList, 
		vector<AppListBodyItemData*>* _pItemList);

	// ios一些不显示的USER程序
	bool IsNotShowUserAppIos(AppData* pApp);

	//andiroid一些不显示的system程序
	bool IsNotShowSystemrAppAndroid(AppData* pApp);

	// android一些不显示的USER程序
	bool IsNotShowUserAppAndroid(AppData* pApp);

	// 根据程序数据获取指定的项
	AppListBodyItemData* GetItemData(AppData* pApp, vector<AppListBodyItemData*> itemList);

	// 多选移动程序
	bool MultipleMoveToApp(bool bToSDCard);

	// 卸载多选程序
	bool MultipleUninstallApp();

	// 卸载用户应用
	void UserSingleUninstallApp(wstring appId);

	// 卸载系统应用
	void SystemSingleUninstallApp(wstring appId);

	// 移动应用
	void SingleMoveTo(wstring appId);

	// 根据ID从list控件找相应的项
	AppListBodyItemData* FindItemData(vector<AppListBodyItemData*> itemList, wstring id);

	bool IsExisAppData(vector<AppData*> appDataList, wstring id);

	// 添加需要更新的应用数据
	void AddUpdateData(AppData* pApp);
	void ClearUpdateDate();

	// 清理List列表
	void ClearAppListItem();

	//Tab按钮上应用程序的数量
	void ShowTabAppCount();

    //更新列表信息
    void RefreshAppInfo(AppData* pData, bool isMoveAndroidApp=false);

	//更新移动按钮的状态
	void RefreshMoveBtn(bool bShow);

	bool OnUpgradeBtnClick(TNotifyUI* pNotify);

	bool OnLegalUpgradeBtnClick(TNotifyUI* pNotify);

	bool OnUpgradeAllAppsBtnClick(TNotifyUI* pNotify);

	bool OnUpgradeAllAppsMenuBtnClick(TNotifyUI* pNotify);

	bool OnClickUpgradeMenu(TNotifyUI* pNotify);

	void RefreshUpgradeCount();

	void ShowFilterInfo(bool show);

	// 用户程序list控件项集合
	vector<AppListBodyItemData*> _userItemList;

	// 保存待更新的应用数据
	vector<AppData*> _updateAppDatas;

	// 释放应用数据
	void ReleaseAppData();

	// 修改搜索时容器里面的数据
	void SetSearchData(AppData* pdata);

	// 保存用户应用数据 (搜索使用)
	vector<AppData*> _userAppDatas;
	// 保存系统应用数据 (搜索使用)
	vector<AppData*> _systemAppDatas;

	// 用户程序list控件
	CListUI*        _pUserList;

	// 系统程序list控件项集合
	vector<AppListBodyItemData*> _systemItemList;

	// 系统程序list控件
	CListUI*        _pSystemList;

	CTabUI*         _pTab;

	AppModel    _model;    // 业务逻辑类

	bool        _hasSDCard;

	wstring     _appId;     // 待操作的应用ID（卸载，移动）

	bool _bShowSystemAppTab;//是否显示系统tab
	CRITICAL_SECTION _updateAppDataLock;       //操作更新应用的锁

	CRITICAL_SECTION _csListLock;       //操作列表的锁
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

	bool       _bDelete;//本身是否被释放掉
public:
	// ApplicationInfoInterface
	// 显示正在加载数据图片
	virtual void ShowLoadingPicture(bool bShow);

	// 清空界面数据
	virtual void ClearData();
	virtual void ClearDataAction(bool bUser = true);

	// 获取list控件所选择的数据
	virtual vector<AppData*> GetSelectDatas();

	//@zjf summary:解决debug下，ios设备插上去经常崩溃问题.有调用者主动调用ClearTempData释放内存
	void BuildeTempData(const vector<AppData*>* pDatas, vector<AppData*>& vecTempData);
	void ClearTempData(vector<AppData*>& vecTempData);

	// 往list控件插入数据
	virtual void InsertToAppView(const vector<AppData*>* pDatas);
	virtual void InsertToAppViewAction(const vector<AppData*>* pDatas);

	// 从list控件移除数据
	virtual void RemoveFromAppView(AppData* pApp);
	virtual void RemoveFromAppViewAction(AppData* pApp);

	// 删除app时从临时搜索app数据队列去除
	virtual void RemoveFromTempListData(AppData* pApp, vector<AppData*>& verDatas);

	// 显示应用ICON
	virtual void SetAppLogo(AppData* pApp, const DeviceData* pCurDevice);
	virtual void SetAppLogo(const vector<AppData*>* pDatas, const DeviceData* pCurDevice);
	virtual void SetAppLogoWithoutLoad(const vector<AppData*>* pDatas, const DeviceData* pCurDevice);

	// 显示应用大小
	virtual void SetAppSize(const vector<AppData*>* pDatas, const DeviceData* pCurDevice) ;
	virtual void SetAppSizeAction(AppData* pApp, const DeviceData* pCurDevice);

	//设置升级按钮
	virtual void SetUpgradeBtn(const vector<AppDataUpgrade*>* pDatas, const DeviceData* pCurDevice);
	virtual void SetUpgradeBtn(AppDataUpgrade* pApp, const DeviceData* pCurDevice);
	virtual void SetAllUpgradeBtn(bool enabled);

	//设置升级按钮
	virtual void SetLegalUpgradeBtn(const vector<AppDataUpgrade*>* pDatas, const DeviceData* pCurDevice);
	virtual void SetLegalUpgradeBtn(AppDataUpgrade* pApp, const DeviceData* pCurDevice);

	virtual void SetUpgradeBtnEnabled(AppDataUpgrade* pData, bool enabled);

	// 安装程序
	virtual bool InstallApp(bool isFile, wstring wsTitle=L"");

	// 安装程序
	virtual bool InstallApp( vector<wstring> files );

	// 更新数据
	virtual void UpdateData();
	virtual void UpdateDataAction();

	// 刷新数据
	virtual void RefreshData();

	void RefreshUnstallBtn(bool bShow);

	//获得打开安装程序的路径
	void GetAppFilePath(vector<wstring>& file);

    virtual bool IsHaveCheckSDCardState(){ return _bHaveCheckSDCardState;};
    virtual bool GetSDCardState();
    virtual void SendToUI(int nMessage);
};
App_CC_END