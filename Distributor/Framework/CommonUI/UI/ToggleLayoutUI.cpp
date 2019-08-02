#include "stdafx.h"
#include "ToggleLayoutUI.h"
#include <math.h>
#include "AnimationRender.h"

CToggleLayoutUI::CToggleLayoutUI()
{
	m_bMouseEnable = true;
	InitializeCriticalSection(&m_csLock); 
	m_bLockRender = false;
	m_pAnimationRenderCur = new AnimationRender;
	m_pAnimationRenderNext = new AnimationRender;
	m_pAnimationRenderCur->SetAnimations(50, 10);
	m_pAnimationRenderNext->SetAnimations(50, 10);
	SetType(_T("ToggleLayout"));

	m_pRadioLayout = NULL;
	m_nRadioSize = 20;
	m_bShowRadio = true;
	SetAttribute(_T("ShowRadio"), _T("1"));

	if (m_bShowRadio && m_pRadioLayout == NULL)
	{
		m_pRadioLayout = new CLayoutUI;
		m_pRadioLayout->SetAttribute(_T("mode"), _T("lefttoright"));
	}
    _pCallBack = NULL;
}

CToggleLayoutUI::~CToggleLayoutUI()
{
	DeleteCriticalSection(&m_csLock);
	if (m_pRadioLayout)
		delete m_pRadioLayout;
	m_pRadioLayout = NULL;
	if (m_pAnimationRenderCur)
		delete m_pAnimationRenderCur;
	m_pAnimationRenderCur = NULL;
	if (m_pAnimationRenderNext)
		delete m_pAnimationRenderNext;
	m_pAnimationRenderNext = NULL;
	if (m_pRadioLayout)
		delete m_pRadioLayout;
	m_pRadioLayout = NULL;
}

UI_IMPLEMENT_DYNCREATE(CToggleLayoutUI);

void CToggleLayoutUI::Init()
{
	CControlUI* pControl = NULL;
	if (!m_bShowRadio && m_pRadioLayout)
	{
		delete m_pRadioLayout;
		m_pRadioLayout = NULL;
	}

	if (m_bShowRadio)
	{
		AddStaticItem(m_pRadioLayout);
		m_pRadioLayout->SetManager(GetWindow(), this);
		int nCnt = m_pRadioLayout->GetCount();
		for (int i = 0; i <nCnt; i++)
		{
			pControl = m_pRadioLayout->GetItem(i);
			if (pControl)
				pControl->SetManager(GetWindow(), m_pRadioLayout);
		}
	}

	int nCnt = GetCount();
	for (int i = 0; i < nCnt; i++)
	{
		pControl = GetItem(i);
		if (pControl)
			pControl->SetVisible(false, false);
	}
	if (m_pRadioLayout)
	{
		CControlUI* pItem = GetItem(0);
		if (pItem)
			SelectItem(pItem);
	}
}

bool CToggleLayoutUI::Event(TEventUI& event)
{
	switch (event.Type)
	{
	case UIEVENT_SCROLLWHEEL:
		{
			int nIndex = GetCurSel();
			int zDelta = (int) (short) HIWORD(event.wParam);
			if (zDelta > 0)
			{
				nIndex--;
				if (nIndex < 0)
					nIndex = 0;
			}
			else
			{
				nIndex++;
				int nCnt = GetCount();
				if (nIndex >= nCnt)
					nIndex = nCnt-1;
			}


			SelectItem(GetItem(nIndex));
		}
		break;
	}
	return __super::Event(event);
}

bool CToggleLayoutUI::Add(CControlUI* pControl)
{
	if (pControl)
		pControl->AddEventDelegate(&MakeDelegate(this, &CToggleLayoutUI::ChildEvent));
	if (m_pRadioLayout)
	{
		CRadioUI* pRadio = new CRadioUI;
		pRadio->SetINotifyUI(this);
		pRadio->SetInitPos(0,0,m_nRadioSize,m_nRadioSize,0);
		m_pRadioLayout->Add(pRadio);
	}
	return __super::Add(pControl);
}

bool CToggleLayoutUI::Remove(CControlUI* pControl)
{
	if (m_pRadioLayout)
	{
		CControlUI* pItem = m_pRadioLayout->GetItem(pControl->GetIndex());
		if (pItem)
			m_pRadioLayout->Remove(pItem);
	}
	return __super::Remove(pControl);
}

void CToggleLayoutUI::RemoveAll()
{
	if (m_pRadioLayout)
		m_pRadioLayout->RemoveAll();
	__super::RemoveAll();
}

void CToggleLayoutUI::SetRect(RECT& rc)
{
	__super::SetRect(rc);
	if (m_pRadioLayout)
	{
		int nCnt = m_pRadioLayout->GetCount();
		int nWidth = m_nRadioSize * nCnt;
		RECT rect = GetClientRect();
		m_pRadioLayout->SetInitPos(-1, (rect.bottom - rect.top) - m_nRadioSize - 10, nWidth, m_nRadioSize, 9);
		m_pRadioLayout->OnSize(rect);
	}
}

void CToggleLayoutUI::SetAttribute( LPCTSTR lpszName, LPCTSTR lpszValue )
{
	if (_tcsicmp(lpszName, _T("RadioSize")) == 0)
	{
		m_nRadioSize = _ttoi(lpszValue);
	} 
	else if (_tcsicmp(lpszName, _T("ShowRadio")) == 0)
	{
		m_bShowRadio = (bool)(_ttoi(lpszValue) != 0);
	}
	else
	{
		__super::SetAttribute(lpszName,lpszValue);
	}	
}

CControlUI* CToggleLayoutUI::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags)
{
	CControlUI* pControl = NULL;
	if (m_pRadioLayout && m_pRadioLayout->IsVisible())
		pControl = m_pRadioLayout->FindControl(Proc, pData, uFlags);

	if (pControl != NULL)
		return pControl;

	pControl = __super::FindControl(Proc, pData, uFlags);

	return pControl;
}

void CToggleLayoutUI::Render(IRenderDC* pRenderDC, RECT& rcPaint)
{
	if (m_bLockRender)
		return;

	if (m_pAnimationRenderCur == NULL || m_pAnimationRenderCur && !m_pAnimationRenderCur->IsAnimation())
	{
		__super::Render(pRenderDC, rcPaint);
		StaticUIRender(pRenderDC,rcPaint);
		return;
	}

	if (m_pAnimationRenderCur && m_pAnimationRenderCur->IsAnimation())
	{
		if (m_pAnimationRenderCur->m_nAnimationIndex != -1)
		{
			int nCurRenderX = m_pAnimationRenderCur->m_nCurRenderX;
			int nCurRenderY = m_pAnimationRenderCur->m_nCurRenderY;
			m_pAnimationRenderCur->Render(pRenderDC, nCurRenderX, nCurRenderY);
			m_pAnimationRenderNext->Render(pRenderDC, nCurRenderX, nCurRenderY);
		}
	}

	StaticUIRender(pRenderDC,rcPaint);
}

bool CToggleLayoutUI::SelectIndex( int index )
{
	if (index<0)
		return false;

	CControlUI* pItem = GetItem(index);
	if (pItem)
	{
		SelectItem(pItem);
		return true;
	}
	else
	{
		return false;
	}
}

bool CToggleLayoutUI::SelectItem(CControlUI* pControl)
{
	EnterCriticalSection(&m_csLock);
	if (m_pAnimationRenderCur == NULL || m_pAnimationRenderCur && !m_pAnimationRenderCur->IsAnimation())
	{
		if (pControl)
		{
			int nLastIndex = GetCurSel();
			int nIndex = pControl->GetIndex();

			int nMoveType = nLastIndex < nIndex? AnimationRender::ANIMATIONRight2Left : nLastIndex != nIndex?AnimationRender::ANIMATIONLeft2Right:AnimationRender::ANIMATIONUnset;

			if (nLastIndex != -1)
			{
				OnCurAnimation(nMoveType);
			}

			//////////////////////////////////////////////////////////////////////////
			{
				if (m_pRadioLayout && pControl)
				{
					CRadioUI* pRadio = (CRadioUI*)m_pRadioLayout->GetItem(pControl->GetIndex());
					if (pRadio) pRadio->SetChecked(true,false);
                    if (_pCallBack)
                        _pCallBack(pControl->GetIndex(), _pCallBackParam);
				}
				__super::SelectItem(pControl);
			}
			//////////////////////////////////////////////////////////////////////////
			if (nLastIndex != -1)
			{
				OnNextAnimation(nMoveType);
			}
            int nIndex2 = pControl->GetIndex();
            bool b = nIndex2 == nIndex;
		}
	}
	LeaveCriticalSection(&m_csLock); 
	return true;
}

bool CToggleLayoutUI::OnCurAnimation(int nMoveType)
{
	if (m_pAnimationRenderCur && !m_pAnimationRenderCur->IsAnimation())
	{
		m_pAnimationRenderCur->DrawFrameToMemDC(this, nMoveType);
		m_bLockRender = true;
	}
	return true;
}

inline int GetWidth(RECT& rc) 
{
	return rc.right - rc.left;
}
inline int GetHeight(RECT& rc)
{
	return rc.bottom - rc.top;
}

bool CToggleLayoutUI::OnNextAnimation(int nMoveType)
{
	if (m_pAnimationRenderCur && !m_pAnimationRenderCur->IsAnimation())
	{
		m_bLockRender = false;

		m_pAnimationRenderNext->DrawFrameToMemDC(this, nMoveType);

		int nWidth = GetWidth(GetRect());
		switch(nMoveType)
		{
		case 0:
			{
				m_pAnimationRenderNext->SetOffset(-nWidth, 0);
			}
			break;
		case 1:
			{
				m_pAnimationRenderNext->SetOffset(nWidth, 0);
			}
			break;
		}
		m_pAnimationRenderCur->StartAnimation();
	}
	return true;
}

bool CToggleLayoutUI::OnBtnRadio(TNotifyUI* pNotify)
{
	bool bCheck = pNotify->wParam != 0;
	if (bCheck)
	{
		CControlUI* pControl = pNotify->pSender;
		if (pControl)
		{
			CControlUI* pItem = GetItem(pControl->GetIndex());
			if (pItem)
				SelectItem(pItem);
		}
	}
	return true;
}

bool CToggleLayoutUI::ChildEvent(TEventUI& event)
{
	switch(event.Type)
	{
	case UIEVENT_SCROLLWHEEL:
		{
			Event(event);
			event.Type = UIEVENT_UNUSED;
		}
		break;
	}
	return true;
}

void CToggleLayoutUI::SetSelectItemCallBack( ToggleLayout_OnSelectItem_CallBack pCallBack, void* pParam )
{
    _pCallBack = pCallBack;
    _pCallBackParam = pParam;
}
