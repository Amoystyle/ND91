#include "StdAfx.h"
#include "MessageWnd.h"

MessageWnd::MessageWnd(wstring strMsg,  wstring strTitle, wstring strIcon, UINT nType, int height, wstring okText)
{
    _strMsg     = strMsg;
    _strTitle   = strTitle;
    _strIcon    = strIcon;
    _nType      = nType;
    _height     = height;
    _okText     = okText;
	m_nRoundRgn = 0;
	_bIsWarn = false;
}

void MessageWnd::WindowInit(wstring tip)
{
    SetTitle(_strTitle.c_str());

    RECT mySize;
    ::GetWindowRect(m_hWnd, &mySize);
    int width = mySize.right - mySize.left;

    if (_height != 0)
    {
        SetMinSize(width, _height);
		CControlUI* pLabelmsg = GetItem(L"centerlayout.message");
		CControlUI* pLabelmsgwarn = GetItem(L"centerlayout.messagewarn");
		if(_bIsWarn)
		{
			pLabelmsg->SetVisible(false);
			pLabelmsgwarn->SetVisible(true);
			pLabelmsgwarn->SetInitPos(115,0,385,170,0);
		}
		else
		{
			pLabelmsgwarn->SetVisible(false);
			pLabelmsg->SetVisible(true);
			pLabelmsg->SetInitPos(115,0,385,170,0);
		}
    }

	CControlUI *pimg = GetItem(_T("Imgicon"));
	if (pimg) 
	{
		pimg->SetAttribute(L"bk.image" ,L"#CommonUi_CommonUi_question");
		pimg->Resize();
		pimg->Invalidate();
	}
    if (_nType == MB_OK)
    {
        CControlUI* p = GetItem(_T("buttom_Layout.cancel"));
        if ( p ) p->SetVisible(false);

        p = GetItem(_T("buttom_Layout.ok"));
        if (p) p->SetAttribute(L"pos", L"5|80,10,10,5");

		CControlUI *pimg = GetItem(_T("Imgicon"));
		if (pimg) 
		{
			pimg->SetAttribute(L"bk.image" ,L"#CommonUi_CommonUi_warning");
			pimg->Resize();
			pimg->Invalidate();
		}
		
    }
	if (_nType == MB_YESNO)
	{
		CControlUI* p = GetItem(_T("buttom_Layout.ok"));
		if ( p )
		{
			p->SetText(CommonUIFunc::LoadString(_T("CommonUi_yes")));
		}

		p = GetItem(_T("buttom_Layout.cancel"));
		if (p)
		{
			p->SetText(CommonUIFunc::LoadString(_T("CommonUi_no")));
		}

	}
	

    if (!_strIcon.empty())
    {
        CControlUI* p = GetItem(_T("centerlayout.tipicon"));
        if ( p ) p->SetAttribute(L"bk.image", _strIcon.c_str());
    }

    CLabelUI* p = (CLabelUI*)GetItem(_T("centerlayout.message"));
	CLabelUI* pwarn = (CLabelUI*)GetItem(_T("centerlayout.messagewarn"));
    if (p && pwarn)
	{
		if (p && !_bIsWarn)
		{
			p->SetVisible(true);
			pwarn->SetVisible(false);
			p->SetText(_strMsg.c_str());
			if ( ! tip.empty() )
			{
				p->SetAttribute(L"enablemouse", L"1");
				p->SetAttribute(L"tooltip", tip.c_str());
			}
		}
		else if(pwarn && _bIsWarn)
		{
			p->SetVisible(false);
			pwarn->SetVisible(true);
			pwarn->SetText(_strMsg.c_str());
			if ( ! tip.empty() )
			{
				pwarn->SetAttribute(L"enablemouse", L"1");
				pwarn->SetAttribute(L"tooltip", tip.c_str());
			}
		}
	}
    if (_okText.length() > 0)
    {
        CControlUI* p = GetItem(_T("buttom_Layout.ok"));
        if ( p )
        {
            p->SetText(_okText.c_str());
        }
    }
	CButtonUI* pBtnOK = dynamic_cast<CButtonUI*>(GetItem(_T("buttom_Layout.ok")));    
	if (pBtnOK)
		pBtnOK->SetFocus();
}

int MessageWnd::Show(HWND hParent, wstring tip, bool warn)
{
	_bIsWarn=warn;
    if(!m_hWnd)
        Create(hParent, _T("MessageWnd"));
    RETURN_VALUE_IF(!m_hWnd, 0);
    SetWndStyle(GWL_EXSTYLE, WS_EX_TOOLWINDOW);

    WindowInit(tip);

    CenterWindow(hParent);
    return this->DoModal();
}

bool MessageWnd::OnbtnOk(TNotifyUI* pNotify)
{
    EndDialog(IDOK);
    return true;
}
bool MessageWnd::OnbtnCancel(TNotifyUI* pNotify)
{
    EndDialog(IDCANCEL);
    return true;
}

LRESULT MessageWnd::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
    CButtonUI* pBtnOK = dynamic_cast<CButtonUI*>(GetItem(_T("buttom_Layout.ok")));    
    CButtonUI* pBtnCancel = dynamic_cast<CButtonUI*>(GetItem(_T("buttom_Layout.cancel")));
    if (message == WM_KEYDOWN)
    {
        switch (wParam)
        {
        case VK_RETURN:
            EndDialog(IDOK);
            break;
        case VK_ESCAPE:
            EndDialog(IDCANCEL);
            break;
        case VK_LEFT:
            if (pBtnOK && pBtnCancel)
            {                

                bool bOK = pBtnOK->IsFocused();
                bool bCancel = pBtnCancel->IsFocused();
                if (pBtnCancel->IsVisible() == 0)
                    pBtnOK->SetFocus();
                else if (!bOK && !bCancel)
                    pBtnCancel->SetFocus();                    
                else if (bCancel)
                    pBtnOK->SetFocus();                
            }
            break;
        case VK_RIGHT:
            if (pBtnOK && pBtnCancel)
            {
                bool bOK = pBtnOK->IsFocused();
                bool bCancel = pBtnCancel->IsFocused();
                if (pBtnCancel->IsVisible() == 0)
                    pBtnOK->SetFocus();
                else if (!bOK && !bCancel)
                    pBtnOK->SetFocus();                    
                else if (bOK)
                    pBtnCancel->SetFocus();                
            }
            break;
        }
    }
    return CWindowUI::WindowProc(message,wParam,lParam);
}
