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
		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"Tree", OnCaseSelChanged)//左边树选中事件
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

	//编辑方案，弹出编辑方案窗体
	virtual void GetCaseInfoFromLib(wstring& caseName, _INSTALLS_CASE_DATA_TYPE& caseType, vector<wstring>* pathList);

	//从库中创建方案，弹出新建方案窗体
	virtual void GetEditCaseIdList(InstallsCaseData* caseData, wstring& caseName, vector<wstring>* pathList);

	//从文件夹中创建方案，让用户输入方案名称并选择文件夹路径
	virtual void GetCaseInfoFromFolder(wstring& caseName, _INSTALLS_CASE_DATA_TYPE& caseType, wstring& folder);

	//获取方案名称
	virtual wstring GetFolder(_INSTALLS_CASE_DATA_TYPE type);

	virtual void GetAppPath(_INSTALLS_CASE_DATA_TYPE type, vector<wstring>* pathList);

	//确定是否要删除
	virtual void GetCaseName(wstring& caseName);

	//获取文件夹路径
	virtual bool RemoveAppConfirm(bool isRemoveFile);

	virtual bool RemoveCaseConfirm();

	virtual bool RemoveFolderConfirm();

	virtual bool IsNeedInstalliTunesConfirm();

	//选中方案
	void SelectCaseNode(wstring caseId);

	//消息处理
	bool InstallCaseUI::Handle(const InstallCaseMsg* pMsg=NULL);


    //激活方案列表节点
    void ShowCaseList();

	//统计接口
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
	//创建控件
	void OnCreate();

	//初始化列表
	void OnInitComposite();

	//添加方案到界面
	void AddCaseToView(InstallsCaseData* caseData);

	//从界面移除方案
	void RemoveCaseFromView(int caseId);

	//添加软件
	void AddAppToView(int caseId, InstallsAppInfo* appInfo);

	//删除软件
	void RemoveAppFromView(int caseId, wstring appPath);

	void ClearAppFromView( int caseId, AppSearchContextEx* context );

	void ClearAppFromView( int caseId );

	//更新升级软件个数
	void UpdateUpgrateCount(_INSTALLS_CASE_DATA_TYPE caseType, int count);

	//更新方案信息
	void UpdateCaseView(InstallsCaseData* caseData);

	//生成方案个数
	wstring GetCountStr(InstallsCaseData* caseData);

	//显示方案列表树菜单
	bool OnShowTreeMenu(TNotifyUI* pNotify);

	//新建方案
	bool OnClickMenuNewCase(TNotifyUI* pNotify);

	//编辑和删除方案
	bool OnClickMenuCaseList(TNotifyUI* pNotify);

	//树选中项改变
	bool OnCaseSelChanged(TNotifyUI* pNotify);

	//软件列表和升级列表切换
	bool OnTabItemClick(TNotifyUI* pNotify);

	//添加用户方案
	void AddUserCaseToView(InstallsCaseData* caseData);

	void CancelAppUpgrate( int caseId, wstring appPath );	

	//刷新方案Items图标
	void RefreshCaseItemsIcon( int caseId );

	//显示方案软件列表
	void ShowUserCaseAppList(CLayoutUI* pContainer, InstallsCaseData* caseData);

	//更新应用库显示软件个数
	void UpdateSystemCaseCount( int caseID, int count );

	//完成更新
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