#include "stdafx.h"
#include "AutoGridLayoutUI.h"
#include <math.h>

CAutoGridLayoutUI::CAutoGridLayoutUI()
{
	SetType(_T("AutoGridLayout"));
	SetAttribute(_T("ShowRadio"), _T("1"));
	m_nCurIndex = 0;
	m_nAvgPageCount = 0;
	m_bMouseEnable = true;
	m_nGridWidth = 0;
	m_nGridHeight = 0;

	m_bShowRadio = false;
	m_pRadioLayout = NULL;
	m_nRadioSize = 20;
	m_enablePage = true;
	m_nPages = 0;

	InitializeCriticalSection(&m_csLock); 
	m_bLockRender = false;
	m_pAnimationRenderCur = new AnimationRender;
	m_pAnimationRenderNext = new AnimationRender;
	m_pAnimationRenderCur->SetAnimations(50, 10);
	m_pAnimationRenderNext->SetAnimations(50, 10);

	m_ptLast = NULL;
}

CAutoGridLayoutUI::~CAutoGridLayoutUI()
{
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

UI_IMPLEMENT_DYNCREATE(CAutoGridLayoutUI);

void CAutoGridLayoutUI::Init()
{
	if (m_pRadioLayout)
		delete m_pRadioLayout;
	m_pRadioLayout = NULL;
	if (m_bShowRadio)
	{
		m_pRadioLayout = new CLayoutUI;
		m_pRadioLayout->SetManager(GetWindow(), this);
		m_pRadioLayout->SetAttribute(_T("mode"), _T("lefttoright"));
		AddStaticItem(m_pRadioLayout); // 不参与动画
	}
}

bool CAutoGridLayoutUI::Event(TEventUI& event)
{
	switch (event.Type)
	{
	case UIEVENT_SCROLLWHEEL:
		{
			if(!m_enablePage)
				return __super::Event(event);	

			EnterCriticalSection(&m_csLock);
			if (m_pAnimationRenderCur == NULL || (m_pAnimationRenderCur && !m_pAnimationRenderCur->IsAnimation()))
			{
				int nLastIndex = m_nCurIndex;

				int nCnt = GetCount();
				int zDelta = (int) (short) HIWORD(event.wParam);
				if (zDelta > 0)
				{
					m_nCurIndex = m_nCurIndex - m_nAvgPageCount;
					if (m_nCurIndex < 0)
						m_nCurIndex = 0;
				}
				else
				{
					if (m_nCurIndex + m_nAvgPageCount < nCnt)
					{
						m_nCurIndex = m_nCurIndex + m_nAvgPageCount;
					}
				}

				int nIndex = m_nCurIndex;

				int nMoveType = nLastIndex < nIndex? AnimationRender::ANIMATIONRight2Left : nLastIndex != nIndex?AnimationRender::ANIMATIONLeft2Right:AnimationRender::ANIMATIONUnset;

				OnCurAnimation(nMoveType);
				Resize();
				OnNextAnimation(nMoveType);
				if(nLastIndex != nIndex)
				{
					int nPageIndex=0;
					if ((m_nCurIndex % m_nAvgPageCount) == 0)
						nPageIndex = (m_nCurIndex/m_nAvgPageCount);
					else
						nPageIndex = (int)ceil((double)m_nCurIndex/(double)m_nAvgPageCount);
					if (nPageIndex < 0)
						nPageIndex = 0;
					if (nPageIndex >= m_nPages)
						nPageIndex = m_nPages - 1;
					SendNotify(UINOTIFY__AutoGrid_PageChanged,NULL,nPageIndex+1);
				}
			}

			LeaveCriticalSection(&m_csLock); 
		}
		break;
	}
	return __super::Event(event);
}

bool CAutoGridLayoutUI::Add(CControlUI* pControl)
{
	if (pControl)
		pControl->AddEventDelegate(&MakeDelegate(this, &CAutoGridLayoutUI::ChildEvent));
	return __super::Add(pControl);
}

bool CAutoGridLayoutUI::Insert(CControlUI* pControl, CControlUI* pAfter)
{
	if (pControl)
		pControl->AddEventDelegate(&MakeDelegate(this, &CAutoGridLayoutUI::ChildEvent));
	return __super::Insert(pControl, pAfter);
}

void CAutoGridLayoutUI::SetRect(RECT& rc)
{
	__super::SetRect(rc); 
	if (m_pRadioLayout) 
	{
		int nCnt = m_pRadioLayout->GetCount();
		int nWidth = m_nRadioSize * nCnt;
		RECT rect = GetRect();
		m_pRadioLayout->SetInitPos(-1, (rect.bottom - rect.top) - m_nRadioSize - 2, nWidth, m_nRadioSize, 9);
		m_pRadioLayout->OnSize(rect);
	}
}

SIZE CAutoGridLayoutUI::OnChildSize(RECT& rcWinow, RECT& rcClient)
{
	int nCount = GetCount();
	CControlUI* pControl = NULL;
	RECT rcEmpty = {0};
	for (int i = 0; i < nCount; i++)
	{
		pControl = GetItem(i);
		if (pControl)
		{
            pControl->SetRect(rcEmpty, false);
		}
	}

	SIZE sz = {rcClient.right - rcClient.left, rcClient.bottom - rcClient.top};

	int x = 1;
	int y = 1;
	if ((m_nGridWidth + m_nSpaceX) != 0)
		x = max((int)floor((double)(sz.cx - m_nStartX + m_nSpaceX)/(double)(m_nGridWidth + m_nSpaceX)), 1);
	if ((m_nGridHeight + m_nSpaceY) != 0)
		y = max((int)floor((double)(sz.cy - m_nStartY + m_nSpaceY)/(double)(m_nGridHeight + m_nSpaceY)), 1);
	m_nAvgPageCount = x*y;
	if(m_nAvgPageCount<=0)
		return sz;
	int lastpagescount=m_nPages;
	if ((m_nCurIndex % m_nAvgPageCount) == 0||nCount<=m_nAvgPageCount)
	{
		m_nPages = (int)ceil((double)(nCount)/(double)m_nAvgPageCount);
	}
	else 
	{
		m_nPages = (int)ceil((double)(nCount - m_nCurIndex)/(double)m_nAvgPageCount) + (int)ceil((double)(m_nCurIndex)/(double)m_nAvgPageCount);
	}
	if(lastpagescount!=m_nPages)
	{
		int nPageIndex=0;
		if ((m_nCurIndex % m_nAvgPageCount) == 0)
			nPageIndex = (m_nCurIndex/m_nAvgPageCount);
		else
			nPageIndex = (int)ceil((double)m_nCurIndex/(double)m_nAvgPageCount);
		if (nPageIndex >= m_nPages)
			nPageIndex = m_nPages - 1;
		if (nPageIndex < 0)
			nPageIndex = 0;
		m_nCurIndex=nPageIndex*m_nAvgPageCount;
		SendNotify(UINOTIFY__AutoGrid_PageChanged,m_nPages,nPageIndex+1);
	}
	RECT rcChild = {0};
	int nWidth = m_nGridWidth;
	int nHeight = m_nGridHeight;
	int nStartX = rcClient.left + m_nStartX;
	int nStartY = rcClient.top + m_nStartY;
	int nCurMaxY = nStartY;
	for (int i = m_nCurIndex; i < nCount; i++)
	{
		pControl = GetItem(i);
		if (pControl)
		{
			if (nStartX + nWidth > rcClient.right)
			{
				nStartX = rcClient.left + m_nStartX;
				nStartY = nCurMaxY + m_nSpaceY;
			}
			if (nStartY + nHeight > rcClient.bottom)
			{
				pControl->SetRect(rcEmpty, false);
				break;
			}
			rcChild.left = nStartX;
			rcChild.top = nStartY;
			rcChild.right = rcChild.left + nWidth;
			rcChild.bottom = rcChild.top + nHeight;
			pControl->SetRect(rcChild);
			nStartX = rcChild.right + m_nSpaceX;
			nCurMaxY = max(nCurMaxY, rcChild.bottom);
		}
	}
	if (m_nPages > 0 && m_pRadioLayout)
	{
		m_pRadioLayout->RemoveAll();
		if (m_nPages != 1)
		{
			for (int i = 0; i < m_nPages; i++)
			{
				CRadioUI* pRadio = new CRadioUI;
				if(i == 0)
					pRadio->SetStyle(this->m_RadioStyleL.c_str());
				else if(i == m_nPages - 1)
					pRadio->SetStyle(this->m_RadioStyleR.c_str());
				else
					pRadio->SetStyle(this->m_RadioStyle.c_str());

				pRadio->SetINotifyUI(this);
				pRadio->SetInitPos(0,0,m_nRadioSize,m_nRadioSize,0);
				m_pRadioLayout->Add(pRadio);
			}
			//计算当前页面值
			{
				int nPageIndex = 0;
				if (m_nAvgPageCount != 0)
				{
					if ((m_nCurIndex % m_nAvgPageCount) == 0)
					{
						nPageIndex = (m_nCurIndex/m_nAvgPageCount);
					}
					else
					{
						nPageIndex = (int)ceil((double)m_nCurIndex/(double)m_nAvgPageCount);
					}
				}
				if (nPageIndex < 0)
					nPageIndex = 0;
				if (nPageIndex >= m_nPages)
					nPageIndex = m_nPages - 1;
				if (m_pRadioLayout)
				{
					CRadioUI* pRadio = dynamic_cast<CRadioUI*>(m_pRadioLayout->GetItem(nPageIndex));
					if (pRadio)
						pRadio->SetChecked(true, false);
				}
			}
		}
	}
	return sz;
}

void CAutoGridLayoutUI::Render(IRenderDC* pRenderDC, RECT& rcPaint)
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

void CAutoGridLayoutUI::SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue)
{
	if (_tcsicmp(lpszName, _T("RadioSize")) == 0)
	{
		m_nRadioSize = _ttoi(lpszValue);
	} 
	else if (_tcsicmp(lpszName, _T("ShowRadio")) == 0)
	{
		m_bShowRadio = (bool)(_ttoi(lpszValue) != 0);
	}
	else if (_tcsicmp(lpszName, _T("GridWidth")) == 0)
	{
		m_nGridWidth = _ttoi(lpszValue);
	} 
	else if (_tcsicmp(lpszName, _T("GridHeight")) == 0)
	{
		m_nGridHeight = _ttoi(lpszValue);
	} 
	else if(_tcsicmp(lpszName,_T("EnablePage"))==0)
	{
		m_enablePage=(bool)(_ttoi(lpszValue) != 0);
	}
	else if(_tcsicmp(lpszName, _T("RadioStyle")) == 0)
	{
		m_RadioStyle = lpszValue;
	}
	else if(_tcsicmp(lpszName, _T("RadioStyleL")) == 0)
	{
		m_RadioStyleL = lpszValue;
	}
	else if(_tcsicmp(lpszName, _T("RadioStyleR")) == 0)
	{
		m_RadioStyleR = lpszValue;
	}
	else
	{
		__super::SetAttribute(lpszName, lpszValue);
	}
}

CControlUI* CAutoGridLayoutUI::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags)
{
	CControlUI* pControl = NULL;
	if (m_pRadioLayout && m_pRadioLayout->IsVisible())
	{
		pControl = m_pRadioLayout->FindControl(Proc, pData, uFlags);

		if (pControl != NULL)
			return pControl;
	}

	pControl = __super::FindControl(Proc, pData, uFlags);

	return pControl;
}

void CAutoGridLayoutUI::SelectPage( int nPageIndex )
{
	EnterCriticalSection(&m_csLock);

	if (m_pAnimationRenderCur == NULL || m_pAnimationRenderCur && !m_pAnimationRenderCur->IsAnimation())
	{
		int nLastIndex = m_nCurIndex;

		{
			if (nPageIndex < 0)
				nPageIndex = 0;
			if (nPageIndex >= m_nPages)
				nPageIndex = m_nPages - 1;
			m_nCurIndex = nPageIndex * m_nAvgPageCount;
		}

		int nIndex = m_nCurIndex;

		int nMoveType = nLastIndex < nIndex? AnimationRender::ANIMATIONRight2Left : nLastIndex != nIndex?AnimationRender::ANIMATIONLeft2Right:AnimationRender::ANIMATIONUnset;

		OnCurAnimation(nMoveType);

		if (m_pRadioLayout)
		{
			CRadioUI* pRadio = dynamic_cast<CRadioUI*>(m_pRadioLayout->GetItem(nPageIndex));
			if (pRadio)
				pRadio->SetChecked(true, false);
		}
		Resize();

		OnNextAnimation(nMoveType);
	}

	LeaveCriticalSection(&m_csLock); 
}

bool CAutoGridLayoutUI::OnBtnRadio(TNotifyUI* pNotify)
{
	bool bCheck = pNotify->wParam != 0;
	if (bCheck)
	{
		CControlUI* pControl = pNotify->pSender;
		if (pControl)
		{
			SelectPage(pControl->GetIndex());
		}
	}
	
	return true;
}

bool CAutoGridLayoutUI::ChildEvent(TEventUI& event)
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

bool CAutoGridLayoutUI::OnCurAnimation(int nMoveType)
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

bool CAutoGridLayoutUI::OnNextAnimation(int nMoveType)
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