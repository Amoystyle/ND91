#include "stdafx.h"
#include "AnimationRender.h"
#include <math.h>


inline int GetWidth(RECT& rc) 
{
	return rc.right - rc.left;
}

inline int GetHeight(RECT& rc)
{
	return rc.bottom - rc.top;
}



AnimationRender::AnimationRender()
{
	m_nOffsetX = 0;
	m_nOffsetY = 0;
	m_uFrames = 0;
	m_uElapse = 0;
	m_hThreadID = 0;

	m_nAnimationIndex = -1;
	m_bAnimation = false;
	m_enMoveType = ANIMATIONUnset;
	m_nWidth = 0;
	m_nHeight = 0;
	m_pOwner = NULL;
	m_hwndOwner = NULL;

	//StopAnimation();
}

AnimationRender::~AnimationRender()
{
	StopAnimation();
}

void AnimationRender::SetAnimations(UINT uElapse, UINT uFrames)
{
	m_uFrames = uFrames;
	m_uElapse = uElapse;
}

void AnimationRender::SetOffset(int nOffsetX, int nOffsetY)
{
	m_nOffsetX = nOffsetX;
	m_nOffsetY = nOffsetY;
}

void AnimationRender::DrawFrameToMemDC( CControlUI* pOwner, int nMoveType )
{
	m_hwndOwner = NULL;
	m_pOwner = NULL;
	StopAnimation();
	if (pOwner && nMoveType != ANIMATIONUnset)
	{
		m_pOwner = pOwner;

		RECT rcControl = m_pOwner->GetRect();
		m_hwndOwner = m_pOwner->GetWindow()->GetHWND();
		RECT rcClient;
		::GetClientRect(m_hwndOwner, &rcClient);

		DibObj dibobj;
		IRenderDC RenderDC;
		RenderDC.SetDevice(&dibobj);
		dibobj.Create(m_pOwner->GetWindow()->GetPaintDC(), GetWidth(rcClient), GetHeight(rcClient));

		IStaticUI* pStatic = dynamic_cast<IStaticUI*>(pOwner);
		if (pStatic)
		{
			pStatic->Hide();
			m_pOwner->GetWindow()->GetRoot()->Render(&RenderDC, rcClient);
			pStatic->Show();
		}
		else
		{
			m_pOwner->GetWindow()->GetRoot()->Render(&RenderDC, rcClient);
		}		

		m_enMoveType = nMoveType;
		m_nWidth = GetWidth(rcControl);
		m_nHeight = GetHeight(rcControl);
		if (m_DibObj.IsValid())
			m_DibObj.Clear();
		m_DibObj.Create(pOwner->GetWindow()->GetMemDC(), GetWidth(rcControl), GetHeight(rcControl));
		::BitBlt(m_DibObj.GetSafeHdc(), 0, 0, GetWidth(rcControl), GetHeight(rcControl), RenderDC.GetDC(), rcControl.left, rcControl.top, SRCCOPY);
	}
}

void AnimationRender::Render(IRenderDC* pRenderDC)
{
	if (m_DibObj.IsValid() && m_pOwner)
	{
		RECT rcControl = m_pOwner->GetRect();
		::BitBlt(pRenderDC->GetDC(), rcControl.left + m_nCurRenderX, rcControl.top + m_nCurRenderY, GetWidth(rcControl), GetHeight(rcControl), m_DibObj.GetSafeHdc(), 0, 0, SRCCOPY);
	}
}


void AnimationRender::Render( IRenderDC* pRenderDC, int nCurRenderX, int nCurRenderY )
{
	if (m_DibObj.IsValid() && m_pOwner)
	{
		RECT rcControl = m_pOwner->GetRect();
		::BitBlt(pRenderDC->GetDC(), rcControl.left + nCurRenderX + m_nOffsetX, rcControl.top + nCurRenderY + m_nOffsetY, GetWidth(rcControl), GetHeight(rcControl), m_DibObj.GetSafeHdc(), 0, 0, SRCCOPY);
	}
}

bool AnimationRender::StartAnimation()
{
	if (m_bAnimation || m_enMoveType == ANIMATIONUnset)
		return false;
	m_bAnimation = true;
	m_nAnimationIndex = 0;
	UpdateAnimation();
	return CreateThread(NULL, 0, AnimationProc, this, 0, &m_hThreadID) != NULL;
}


bool AnimationRender::StopAnimation()
{
	if (m_hwndOwner && IsWindow(m_hwndOwner) && m_pOwner)
	{
		RECT rcControl = m_pOwner->GetRect();
		::InvalidateRect(m_hwndOwner, &rcControl, TRUE);
	}
	m_hThreadID = 0;
	m_nAnimationIndex = -1;
	m_bAnimation = false;
	m_enMoveType = ANIMATIONUnset;
	m_nWidth = 0;
	m_nHeight = 0;
	m_pOwner = NULL;
	m_hwndOwner = NULL;

	return true;
}

void AnimationRender::UpdateAnimation()
{
	if (m_bAnimation)
	{
		CalcFrame(m_nAnimationIndex);

		if (m_hwndOwner && IsWindow(m_hwndOwner) && m_pOwner)
		{
			RECT rcControl = m_pOwner->GetRect();
			::InvalidateRect(m_hwndOwner, &rcControl, TRUE);
			UpdateWindow(m_hwndOwner);
		}
	}
}

DWORD CALLBACK AnimationRender::AnimationProc(LPVOID lp)
{
	AnimationRender* pAnimation = (AnimationRender*)lp;
	while(true)
	{
		pAnimation->m_nAnimationIndex++;
		pAnimation->UpdateAnimation();
		if ( pAnimation->m_nAnimationIndex == pAnimation->m_uFrames )
		{
			pAnimation->StopAnimation();
			break;
		}
		Sleep(pAnimation->m_uElapse);
	}
	return 0;
}

void AnimationRender::CalcFrame( int nFrameIndex )
{
	switch(m_enMoveType)
	{
	case ANIMATIONLeft2Right:
		{
			int disX = (int)CTweenAlgorithm::Back(nFrameIndex, 0, m_nWidth, m_uFrames , 1);
			m_nCurRenderX = disX + m_nOffsetX;
			m_nCurRenderY = m_nOffsetY;
		}
		break;
	case ANIMATIONRight2Left:
		{
			int disX = (int)CTweenAlgorithm::Back(nFrameIndex, 0, m_nWidth, m_uFrames , 1);
			m_nCurRenderX = -disX + m_nOffsetX;
			m_nCurRenderY = m_nOffsetY;
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////

IStaticUI::IStaticUI()
:m_vCtrls(),m_vTemp()
{
	InitializeCriticalSection(&m_vcs);
}

IStaticUI::~IStaticUI()
{
	DeleteCriticalSection(&m_vcs);
	m_vCtrls.clear();
	m_vTemp.clear();
}

void IStaticUI::AddStaticItem( CControlUI* item )
{
	if (item)
	{
		EnterCriticalSection(&m_vcs);

		m_vCtrls.push_back(item);
	
		LeaveCriticalSection(&m_vcs);
	}
}

void IStaticUI::RemoveStaticItem( CControlUI* item )
{
	if (item)
	{
		EnterCriticalSection(&m_vcs);

		vector<CControlUI*>::iterator itor = m_vCtrls.begin();
		for (;itor!=m_vCtrls.end();)
		{
			if ((*itor) == item)
			{
				m_vCtrls.erase(itor);
			}
			else
			{
				itor++;
			}
		}

		LeaveCriticalSection(&m_vcs);
	}
}

void IStaticUI::StaticUIRender( IRenderDC* pRenderDC, RECT& rcPaint )
{
	EnterCriticalSection(&m_vcs);

	for (vector<CControlUI*>::iterator itor = m_vCtrls.begin();itor!=m_vCtrls.end();itor++)
	{
		if ((*itor) && (*itor)->IsVisible())
		{
			(*itor)->Render(pRenderDC,rcPaint);
		}
	}

	LeaveCriticalSection(&m_vcs);
}

void IStaticUI::Hide()
{
	EnterCriticalSection(&m_vcs);

	m_vTemp.clear();
	for (vector<CControlUI*>::iterator itor = m_vCtrls.begin();itor!=m_vCtrls.end();itor++)
	{
		if ((*itor) && (*itor)->IsVisible())
		{
			(*itor)->SetVisible(false,false);
			m_vTemp.push_back(*itor);
		}
	}

	LeaveCriticalSection(&m_vcs);
}

void IStaticUI::Show()
{
	EnterCriticalSection(&m_vcs);

	for (vector<CControlUI*>::iterator itor = m_vTemp.begin();itor!=m_vTemp.end();itor++)
	{
		if ((*itor))
		{
			(*itor)->SetVisible(true,false);
		}
	}

	LeaveCriticalSection(&m_vcs);
}
