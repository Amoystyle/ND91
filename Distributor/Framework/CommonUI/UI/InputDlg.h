#ifndef InputDlg_h__
#define InputDlg_h__
//通用输入窗口
class InputDlg : public CWindowUI
{
public:
    virtual void OnSetWindowRgn(){};
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"ok", OnbtnOk)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"cancel", OnbtnCancel)
        EVNET_ID_HANDLER(UINOTIFY_EN_KEYDOWN, L"input", OnKeyDown)
	UIEND_MSG_MAP

    //传入提示信息，输入保存变量的指针,同时作为默认值指针
	InputDlg(wstring strMsg,  wstring* pDefault);

	//设置输入框CEditUI的MacChar属性
	void SetMaxChar(int maxChar);
private:
    // 主线程消息循环
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();

    bool OnOK();
	bool OnbtnOk(TNotifyUI* pNotify);
	bool OnbtnCancel(TNotifyUI* pNotify);
    bool OnKeyDown(TNotifyUI* pNotify);
    wstring _strMsg;
	wstring* _pDefault;	//存放返回值的变量指针
};
#endif // InputDlg_h__