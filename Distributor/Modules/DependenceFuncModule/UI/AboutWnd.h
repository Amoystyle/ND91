#pragma once

//���ڶԻ�����
class AboutWnd :
	public CWindowUI
{
public:
    virtual void OnSetWindowRgn(){};
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"OK", OnOkBtn)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"GAddressValue", OnAddrBtn)	//������ַ��ť
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"MailValue", OnMailBtn)
	UIEND_MSG_MAP

public:
	AboutWnd();
	~AboutWnd(void);

	//OK��ť
	bool OnOkBtn(TNotifyUI* pNotify);

	//������ַ
	bool OnAddrBtn(TNotifyUI* pNotify);

	//���䰴ť
	bool OnMailBtn(TNotifyUI* pNotify);

	virtual void OnCreate();

	//��ʾ�Ի���
	int Show(HWND hParent);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

};
