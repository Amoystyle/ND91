#pragma once
// ��Ϣ����
class COMMONUI_API MessageWnd : public CWindowUI
{
public:
    virtual void OnSetWindowRgn(){};
    UIBEGIN_MSG_MAP
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"ok", OnbtnOk)
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"cancel", OnbtnCancel)
    UIEND_MSG_MAP

    MessageWnd(wstring strMsg,  
               wstring strTitle = Common_APP_TITLE, 
               wstring strIcon = L"",
               UINT nType = MB_OK,
               int height = 0,
               wstring okText=L"");

    int Show(HWND hParent, wstring tip = L"", bool warn=false);

private:
    // ���߳���Ϣѭ��
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    // ��ʼ������
    void WindowInit(wstring tip);

    bool OnbtnOk(TNotifyUI* pNotify);
    bool OnbtnCancel(TNotifyUI* pNotify);

    wstring     _strMsg;                        // ��ʾ��Ϣ
    wstring     _strTitle;                      // ����
    UINT        _nType;                         // �Ի�������
    wstring     _strIcon;                       // ͼ��·��
    int         _height;                        // �Ի���߶�
    wstring     _okText;                        // Ҫ�趨��ȷ����ť���ı�
	bool _bIsWarn;								// �Ƿ��Ǿ�������ʾ
	
};