#pragma once

//输入窗口封装
enum FileExistConfirmReturn
{
    FileExistConfirmReturn_Rewrite,
    FileExistConfirmReturn_Rename,
    FileExistConfirmReturn_Cancel,
    FileExistConfirmReturn_RewriteAll,
    FileExistConfirmReturn_RenameAll,
};

class COMMONUI_API FileExistConfirmWnd : public CWindowUI
{
public:
    virtual void OnSetWindowRgn(){};
    UIBEGIN_MSG_MAP
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Rewrite", OnRewrite)
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Rename", OnRename)
        EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Cancel", OnCancel)
    UIEND_MSG_MAP

    FileExistConfirmWnd(wstring strTitle, wstring name);

    int Show(HWND hParent);

private:
    // 初始化窗口
    void WindowInit();

    bool OnRewrite(TNotifyUI* pNotify);
    bool OnRename(TNotifyUI* pNotify);
    bool OnCancel(TNotifyUI* pNotify);
    wstring     _strTitle;                      // 标题
    wstring     _name;  // 同名的文件/目录
};