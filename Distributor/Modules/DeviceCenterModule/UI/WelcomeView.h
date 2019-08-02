#pragma once

class WelcomeView
	: public BaseView
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Button_GotoInstallCaseModule", OnGotoInstallCaseClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Button_GotoWebResourceModule", OnGotoWebResourceClick);
		EVNET_ID_HANDLER(UINOTIFY_DROPFILES, L"Device_View_Welcome", OnDropFiles)
	UIEND_MSG_MAP
	WelcomeView();
	~WelcomeView();


protected:
	virtual wstring GetStyleID();

	void OnCreate();

	bool OnGotoInstallCaseClick(TNotifyUI* pNotify);

	bool OnGotoWebResourceClick(TNotifyUI* pNotify);

private:
	bool OnDropFiles(TNotifyUI* pNotify);

};