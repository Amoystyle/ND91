#pragma once

class MainView
	: public BaseView
{
public:
	MainView(void* pPhone);
	~MainView();

public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"ReConnect", OnReConnectButtonClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Feedback", OnFeedbackButtonClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"DownloadiTunes", OnDownloadiTunesButtonClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"ViewMoreDebugSettings", OnViewMoreDebugSettings);
	UIEND_MSG_MAP

	void SetStep( ConnectGuideStep step );

	void OnDriverInstallInit();

	void OnDriverDownloading();

	void OnDriverInstalling();

	void OnDriverProgress( DriverInstallState nState, int nStep, int nProgress );

	void OnDriverInstallEnd();

	void OnError( DeviceConnectError error );

	bool OnReConnectButtonClick(TNotifyUI* pNotify);

	bool OnFeedbackButtonClick(TNotifyUI* pNotify);

	bool OnDownloadiTunesButtonClick(TNotifyUI* pNotify);

	bool OnViewMoreDebugSettings(TNotifyUI* pNotify);

	void SetControlVisible(wstring id, bool bVisible = true);

private:
	void ShowInstallWaitView();

	void ShowXpTip(wstring id, bool bShow = true);
private:
	virtual wstring GetStyleID();
	void* _pPhone;
};