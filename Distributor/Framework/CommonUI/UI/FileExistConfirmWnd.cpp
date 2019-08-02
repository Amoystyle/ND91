#include "StdAfx.h"
#include "FileExistConfirmWnd.h"

FileExistConfirmWnd::FileExistConfirmWnd(wstring strTitle, wstring name)
{
    _strTitle   = strTitle;
    _name = name;
	m_nRoundRgn = 0;
}

void FileExistConfirmWnd::WindowInit()
{
    SetTitle(_strTitle.c_str());
}

int FileExistConfirmWnd::Show(HWND hParent)
{
    if(!m_hWnd)
        Create(hParent, _T("FileExistConfirmWnd"));
    RETURN_VALUE_IF(!m_hWnd, 0);
    SetWndStyle(GWL_EXSTYLE, WS_EX_TOOLWINDOW);

    WindowInit();
    CenterWindow(hParent);

    CControlUI* pLabel = GetItem(L"centerlayout.file_name");
    if ( pLabel )
        pLabel->SetAttribute(L"text", _name.c_str());

    return this->DoModal();
}

bool FileExistConfirmWnd::OnRewrite(TNotifyUI* pNotify)
{
    CCheckBoxUI* pCheckBox = (CCheckBoxUI*)GetItem(L"buttom_Layout.All");
    RETURN_TRUE_IF(pCheckBox == NULL);
    if (pCheckBox->IsSelect())
        EndDialog(FileExistConfirmReturn_RewriteAll);
    else
        EndDialog(FileExistConfirmReturn_Rewrite);
    return true;
}

bool FileExistConfirmWnd::OnRename(TNotifyUI* pNotify)
{
    CCheckBoxUI* pCheckBox = (CCheckBoxUI*)GetItem(L"buttom_Layout.All");
    RETURN_TRUE_IF(pCheckBox == NULL);
    if (pCheckBox->IsSelect())
        EndDialog(FileExistConfirmReturn_RenameAll);
    else
        EndDialog(FileExistConfirmReturn_Rename);
    return true;
}

bool FileExistConfirmWnd::OnCancel(TNotifyUI* pNotify)
{
    EndDialog(FileExistConfirmReturn_Cancel);
    return true;
}

