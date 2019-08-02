#include "StdAfx.h"
#include "AboutWnd.h"

AboutWnd::AboutWnd()
{
	m_nRoundRgn = 0;
}

AboutWnd::~AboutWnd(void)
{
}

void AboutWnd::OnCreate()
{
	//获取当前版本

	string strVer = CClientInfo::GetInstance()->GetVersion();
	CLabelUI* label = (CLabelUI*)GetItem(_T("Body.Version.ProductVerValue"));
	label->SetText((CCodeOperation::GB2312ToUnicode(strVer)).c_str());
	label->Invalidate();	
}

int AboutWnd::Show( HWND hParent )
{
	Create(hParent,_T("AboutWnd"));
	CenterWindow(hParent);
	SetWndStyle(GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	SetEnableResize(false);
	return this->DoModal();
}


bool AboutWnd::OnOkBtn( TNotifyUI* pNotify )
{
	EndDialog();
	return true;
}

bool AboutWnd::OnAddrBtn( TNotifyUI* pNotify )
{
	CButtonUI* addrBtn = (CButtonUI*)pNotify->pSender;
	tstring s = addrBtn->GetText();
	ShellExecute(NULL, L"open" ,s.c_str(), NULL, NULL, SW_SHOW);
	return true;
}

bool AboutWnd::OnMailBtn( TNotifyUI* pNotify )
{
	CButtonUI* mailBtn = (CButtonUI*)pNotify->pSender;
	tstring s = _T("mailto:");
	s += mailBtn->GetText();
	ShellExecute(NULL, L"open", s.c_str(), NULL, NULL, SW_SHOW);
	return true;
}

LRESULT AboutWnd::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
    if (message == WM_KEYDOWN)
    {
        switch (wParam)
        {
        case VK_RETURN:
            EndDialog();
            break;
        case VK_LEFT:
        case VK_RIGHT:
            {
                CButtonUI* pBtn = dynamic_cast<CButtonUI*>(GetItem(_T("OK")));
                if (pBtn)
                    pBtn->SetFocus();
            }
            break;
        }
    }
    return CWindowUI::WindowProc(message,wParam,lParam);
}



