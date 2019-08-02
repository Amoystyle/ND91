#include "stdafx.h"
#include "PopWindowUI.h"

CPopWindowUI* CPopWindowUI::ms_Instance = NULL;

CPopWindowUI::CPopWindowUI(void)
{
	m_dwExStyle &= ~WS_EX_APPWINDOW;
	m_dwExStyle |= WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	m_dwStyle &= ~WS_CAPTION;

	m_bEnableChangeSkin = false;
	m_lpWindowID = NULL;
	m_lpBackgroundID = NULL;
	m_bKillFocus = true;

	m_uDirection = POP_DIRECTION_DEFAULT;
	m_nOffsetX = m_nOffsetY = 0;

	m_bkImg = NULL;
	m_imgInset.left = m_imgInset.top = m_imgInset.right = m_imgInset.bottom = 0;
	m_imgseq = L"0";
	m_bImgInsetEnable = false;

	m_pShowAnimation = NULL;
	m_pSender = NULL;
}

CPopWindowUI::~CPopWindowUI(void)
{
	ms_Instance = NULL;
}

CPopWindowUI* CPopWindowUI::GetInstance( LPCTSTR lpWindowID /*= _T("pop")*/,LPCTSTR lpBackgroundID /*= _T("pop_bk")*/)
{
	if (ms_Instance == NULL)
	{
		ms_Instance = new CPopWindowUI;
		ms_Instance->m_lpWindowID = lpWindowID;
		ms_Instance->m_lpBackgroundID = lpBackgroundID;
	}
	return ms_Instance;
}

void CPopWindowUI::SetWindowID( LPCTSTR lpWindowID /*= _T("pop")*/ )
{
	m_lpWindowID = lpWindowID;
}

void CPopWindowUI::SetBackgroundID( LPCTSTR lpBackgroundID /*= _T("pop_bk")*/ )
{
	m_lpBackgroundID = lpBackgroundID;
}

void CPopWindowUI::SetKillFocus( bool bKillFocus /*= true*/ )
{
	m_bKillFocus = bKillFocus;
}

void CPopWindowUI::SetPopDirection( UINT uDirection /*= POP_DIRECTION_DEFAULT*/,int nOffsetX /*= 0*/,int nOffsetY /*= 0*/ )
{
	if (uDirection>=POP_DIRECTION_LEFTTOP && uDirection <=POP_DIRECTION_BOTTOMRIGHT)
		m_uDirection = uDirection;

	m_nOffsetX = nOffsetX;
	m_nOffsetY = nOffsetY;
}

void CPopWindowUI::SetPopBkImage( LPCTSTR lpbkimg,LPCTSTR imgseq,RECT& imgInset )
{
	m_bkImg = lpbkimg;
	m_imgseq = imgseq;
	m_imgInset = imgInset;
	m_bImgInsetEnable = true;
}

void CPopWindowUI::Close()
{
	CloseWindow();
}

RECT CPopWindowUI::GetPopRect()
{
	RECT rcPop;
	::GetWindowRect(GetHWND(),&rcPop);
	return rcPop;
}

void CPopWindowUI::SetPopRect( const RECT& rcPop,UINT uFlags )
{
	this->SetWindowPos(HWND_TOPMOST, rcPop.left, rcPop.top, rcPop.right-rcPop.left, rcPop.bottom-rcPop.top, uFlags);
}

void CPopWindowUI::SetPopRect( const RECT& rcPop,bool bMove /*= true*/,bool bResize/*=true*/ )
{
	UINT uFlags = SWP_NOACTIVATE;
	if (!bMove)	
		uFlags |= SWP_NOMOVE;
	if (!bResize)
		uFlags |= SWP_NOSIZE;

	SetPopRect(rcPop,uFlags);
}

void CPopWindowUI::Show( CControlUI* pOwner /*= NULL*/,Animation* pAnimation/*=NULL*/ )
{
	m_pSender = pOwner;
	m_pShowAnimation = pAnimation;

	this->Create(HWND_DESKTOP,m_lpWindowID);
	this->SetLayerWindow(true);
	this->SetCloseAnimation(new CCloseAnimation);
	this->ShowWindow();
	if (!m_pSender)
	{
		this->CenterWindow();
	}
}

void CPopWindowUI::ApplyBkImageStyle()
{
	CControlUI* pPopBk = (CControlUI*)GetItem(m_lpBackgroundID);
	if (pPopBk)
	{
		if (m_bkImg)
			pPopBk->SetAttribute(L"bk.image",m_bkImg);
		if(m_imgseq)
			pPopBk->SetAttribute(L"bk.imgseq",m_imgseq);

		if (m_bImgInsetEnable)
		{
			ImageStyle* pStyle = (ImageStyle*)pPopBk->GetStyle(L"bk");
			if (pStyle)
			{
				ImageObj* pImg = pStyle->GetImageObj();
				if (pImg)
					pImg->SetInset(m_imgInset);
			}
		}
	}
}

void CPopWindowUI::ApplyPopPos()
{
	if (m_pSender)
	{
		RECT rcControl = m_pSender->GetRect();
		RECT rcPop;
		if(::GetWindowRect(this->GetHWND(),&rcPop))
		{
			POINT pt = {0,0};
			switch (m_uDirection)
			{
			case POP_DIRECTION_LEFTTOP:
				{
					pt.x = rcControl.left-rcPop.right + m_nOffsetX;
					pt.y = rcControl.top + m_nOffsetY;
				}
				break;
			case POP_DIRECTION_LEFTCENTER:
				{
					pt.x = rcControl.left - rcPop.right + m_nOffsetX;
					pt.y = rcControl.top + (rcControl.bottom-rcControl.top)/2 -rcPop.bottom/2 + m_nOffsetY;
				}
				break;
			case POP_DIRECTION_LEFTBOTTOM:
				{
					pt.x = rcControl.left-rcPop.right + m_nOffsetX;
					pt.y = rcControl.bottom-rcPop.bottom + m_nOffsetY;
				}
				break;
			case POP_DIRECTION_TOPLEFT:
				{
					pt.x = rcControl.left + m_nOffsetX;
					pt.y = rcControl.top - rcPop.bottom + m_nOffsetY;
				}
				break;
			case POP_DIRECTION_TOPCENTER:
				{
					pt.x = rcControl.left + (rcControl.right - rcControl.left)/2 - rcPop.right/2 + m_nOffsetX;
					pt.y = rcControl.top - rcPop.bottom + m_nOffsetY;
				}
				break;
			case POP_DIRECTION_TOPRIGHT:
				{
					pt.x = rcControl.right - rcPop.right + m_nOffsetX;
					pt.y = rcControl.top - rcPop.bottom + m_nOffsetY;
				}
				break;
			case POP_DIRECTION_RIGHTTOP:
				{
					pt.x = rcControl.right + m_nOffsetX;
					pt.y = rcControl.top + m_nOffsetY;
				}
				break;
			case POP_DIRECTION_RIGHTCENTER:
				{
					pt.x = rcControl.right + m_nOffsetX;
					pt.y = rcControl.top + (rcControl.bottom-rcControl.top)/2 -rcPop.bottom/2 + m_nOffsetY;
				}
				break;
			case POP_DIRECTION_RIGHTBOTTOM:
				{
					pt.x = rcControl.right + m_nOffsetX;
					pt.y = rcControl.bottom - rcPop.bottom + m_nOffsetY;
				}
				break;
			case POP_DIRECTION_BOTTOMLEFT:
				{
					pt.x = rcControl.left + m_nOffsetX;
					pt.y = rcControl.bottom + m_nOffsetY;
				}
				break;
			case POP_DIRECTION_BOTTOMCENTER:
				{
					pt.x = rcControl.left + (rcControl.right - rcControl.left)/2 - rcPop.right/2 + m_nOffsetX;
					pt.y = rcControl.bottom + m_nOffsetY;
				}
				break;
			case POP_DIRECTION_BOTTOMRIGHT:
				{
					pt.x = rcControl.right-rcPop.right + m_nOffsetX;
					pt.y = rcControl.bottom + m_nOffsetY;
				}
				break;
			}

			::ClientToScreen(m_pSender->GetWindow()->GetHWND(), &pt);
			this->SetWindowPos(HWND_TOPMOST,pt.x,pt.y,0,0,SWP_NOSIZE | SWP_NOACTIVATE);
		}
	}
}

void CPopWindowUI::OnCreate()
{
	ApplyBkImageStyle();
	ApplyPopPos();
}

bool CPopWindowUI::ShowWindow( int nCmdShow /*= SW_SHOW*/ )
{
	return CWindowUI::ShowWindow(nCmdShow);
}

bool CPopWindowUI::ShowWindow( Animation* pAnimation )
{
	return CWindowUI::ShowWindow(pAnimation);
}

LRESULT CPopWindowUI::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_KILLFOCUS:
		{
			if (m_bKillFocus)
				CloseWindow();
			return S_OK;
		}
		break;
	default:
		break;
	}

	return CWindowUI::WindowProc(message,wParam,lParam);
}
