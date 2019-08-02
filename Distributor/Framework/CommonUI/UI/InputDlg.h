#ifndef InputDlg_h__
#define InputDlg_h__
//ͨ�����봰��
class InputDlg : public CWindowUI
{
public:
    virtual void OnSetWindowRgn(){};
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"ok", OnbtnOk)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"cancel", OnbtnCancel)
        EVNET_ID_HANDLER(UINOTIFY_EN_KEYDOWN, L"input", OnKeyDown)
	UIEND_MSG_MAP

    //������ʾ��Ϣ�����뱣�������ָ��,ͬʱ��ΪĬ��ֵָ��
	InputDlg(wstring strMsg,  wstring* pDefault);

	//���������CEditUI��MacChar����
	void SetMaxChar(int maxChar);
private:
    // ���߳���Ϣѭ��
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();

    bool OnOK();
	bool OnbtnOk(TNotifyUI* pNotify);
	bool OnbtnCancel(TNotifyUI* pNotify);
    bool OnKeyDown(TNotifyUI* pNotify);
    wstring _strMsg;
	wstring* _pDefault;	//��ŷ���ֵ�ı���ָ��
};
#endif // InputDlg_h__