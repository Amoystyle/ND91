#pragma once

class CommonInfoView;

class MainView
	: public BaseView
	, public BaseHandler<ResponseDeviceBaseInfoMsg>
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btnExitRecovery", OnButtonExitRecoveryClick)
		EVNET_ID_HANDLER(UINOTIFY_DROPFILES, L"Container", OnDropFiles)
	UIEND_MSG_MAP
	MainView(wstring deviceID, void* phone);
	~MainView();

private:
	virtual wstring GetStyleID();

	void OnCreate();

	bool OnDropFiles(TNotifyUI* pNotify);

	bool OnButtonExitRecoveryClick(TNotifyUI* pNotify);

	void SetItemInfo( CLayoutUI* pLayout, wstring strKey, wstring strValue );

	bool Handle( const ResponseDeviceBaseInfoMsg* pMsg );


private:
	wstring _deviceID;
	void* _pPhone;

	bool _bExitRecovery;

	DeviceModule* _pFuncModule;
	DeviceModule* _pRecentModule;
	CommonInfoView* _pCommonInfoView;
};