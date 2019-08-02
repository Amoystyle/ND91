#pragma once
// 消息窗口
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
    // 主线程消息循环
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    // 初始化窗口
    void WindowInit(wstring tip);

    bool OnbtnOk(TNotifyUI* pNotify);
    bool OnbtnCancel(TNotifyUI* pNotify);

    wstring     _strMsg;                        // 提示信息
    wstring     _strTitle;                      // 标题
    UINT        _nType;                         // 对话框类型
    wstring     _strIcon;                       // 图标路径
    int         _height;                        // 对话框高度
    wstring     _okText;                        // 要设定的确定按钮的文本
	bool _bIsWarn;								// 是否是警告型提示
	
};