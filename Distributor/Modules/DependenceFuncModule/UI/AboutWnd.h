#pragma once

//关于对话框类
class AboutWnd :
	public CWindowUI
{
public:
    virtual void OnSetWindowRgn(){};
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"OK", OnOkBtn)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"GAddressValue", OnAddrBtn)	//官网地址按钮
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"MailValue", OnMailBtn)
	UIEND_MSG_MAP

public:
	AboutWnd();
	~AboutWnd(void);

	//OK按钮
	bool OnOkBtn(TNotifyUI* pNotify);

	//官网地址
	bool OnAddrBtn(TNotifyUI* pNotify);

	//邮箱按钮
	bool OnMailBtn(TNotifyUI* pNotify);

	virtual void OnCreate();

	//显示对话框
	int Show(HWND hParent);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

};
