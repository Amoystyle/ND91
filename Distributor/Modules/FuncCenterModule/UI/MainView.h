#pragma once

class FuncCenterController;
class PcSuitFuncUIItem;
class MainView
	: public BaseView
	, public BaseHandler<ResponseClearDeviceMemoryMsg>
	, public BaseHandler<ResponseRepairDeviceMsg>
	, public BaseHandler<ResponseNeedInstallAppSyncMsg>
	, public BaseHandler<ReponsePcSuitFunctionMsg>
	, public BaseHandler<RequestInstallAppsyncMsg>
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Button_InstallApp", OnInstallAppClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Button_Reboot", OnButtonRebootClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Button_Recovery", OnButtonRecoveryClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Button_ClearMemory", OnButtonClearMemoryClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Button_Repair", OnButtonRepairClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Button_AppSync", OnButtonAppSyncClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Left_Func", OnLeftFunc)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"right_Func", OnRightFunc)
		EVNET_ID_HANDLER(UINOTIFY__AutoGrid_PageChanged,L"Func_panel",OnFuncPageChanged);

	UIEND_MSG_MAP

	MainView(wstring deviceID, void* phone);
	~MainView();

	void CreatePcSuitFunc();

	virtual void Show();
private:
	virtual wstring GetStyleID();

	bool OnFuncPageChanged(TNotifyUI* pNotify);

	void OnCreate();

	bool OnInstallAppClick(TNotifyUI* pNotify = NULL);

	bool OnButtonRebootClick(TNotifyUI* pNotify = NULL);

	bool OnButtonRecoveryClick(TNotifyUI* pNotify = NULL);

	bool OnButtonClearMemoryClick(TNotifyUI* pNotify = NULL);

	bool OnButtonRepairClick(TNotifyUI* pNotify = NULL);

	bool OnButtonAppSyncClick(TNotifyUI* pNotify = NULL);

	void InitAndroidButton();

	void InitIOSButton();

	virtual bool Handle( const ResponseClearDeviceMemoryMsg* pMsg );

	virtual bool Handle( const ResponseRepairDeviceMsg* pMsg );

	virtual bool Handle( const ResponseNeedInstallAppSyncMsg* pMsg );

	virtual bool Handle( const RequestInstallAppsyncMsg* pMsg );
	//接受发送过来的功能大全
	virtual bool Handle( const ReponsePcSuitFunctionMsg* pMsg );

	//初始化第三方功能大全
	void InitPcSuitFunc();

	bool OnLeftFunc(TNotifyUI* pNotify = NULL);

	bool OnRightFunc(TNotifyUI* pNotify = NULL);

	//换页做处理
	void FuncPageChange();

	//创建 AUTOGRID控件的子控件项
	void CreateFuncAutoGridItem();

	//创建btn
	void CreateBtnFuncItem(wstring wsLayoutID, wstring wsID, wstring wsText, wstring wsImg);
	//创建Container
	void CreateContainerFuncItem(wstring wsLayoutID, wstring wsBtnID, wstring wsAnimationID, wstring wsText, wstring wsImg, wstring wsAniImg, bool bAppPos = false, CControlUI* pControlUI=NULL);

	//解决autogrid 控件表示还没完成，直接点击导致的控件显示不对
	static void WINAPI TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime);
private:
	wstring _deviceID;
	void* _pPhone;
	EnumPhoneOS _CurPlatform;

	vector<FunctionItem> _vecPcSuitFunction;
	bool  _bInitPcSuitFunction;//初始化功能大全功能
//当前页索引
	int   _nPageIndex;
	int   _nPageSum;

	CAutoGridLayoutUI* _pFunc_panel;

	CLayoutUI* _Reparelay;

	vector<PcSuitFuncUIItem*> _vec_FuncUIItem;

	static bool _bCanBtnClick;

};