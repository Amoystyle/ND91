/*********************************************************************
 *       Copyright (C) 2010,应用软件开发
 *********************************************************************
 *   Date             Name                 Description
 *   2010-10-22       xqb
*********************************************************************/
#ifndef _UIBUTTON_H
#define _UIBUTTON_H
#include "UIComposite.h"

#define  TIMER_MOUSE_IN  90
#define  TIMER_MOUSE_OUT 91
class CButtonDropDownWnd;
class DUI_API CButtonUI:public CControlUI
{
	friend class CButtonDropDownWnd;
public:
	CButtonUI();
	virtual ~CButtonUI();
	UI_DECLARE_DYNCREATE();
	int GetAnimationIndex(){return m_iAnimationIndex;}

	void ShowDrop(CCompositeUI* pContent, LPCTSTR lpszTemplateName, int nX, int nY, int nOffsetY = 0);
	void ReleaseDrop();
	virtual void SetStatus(UINT uUserStatus){}
	virtual void GetStatus(UINT &uUserStatus){}
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
protected:
	virtual bool Event(TEventUI& event);
	virtual bool Activate();

	CWindowUI* m_pDropDown;
private:
	int m_iAnimationIndex;
	bool m_bAsnycEvent;
};


////////////////////////////////////////////////////////////////////////
class DUI_API CSpliterButtonUI:public CButtonUI
{
public:
	CSpliterButtonUI();
	virtual ~CSpliterButtonUI();
	UI_DECLARE_DYNCREATE();
protected:
	virtual bool Event(TEventUI& event);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	bool m_bMouseInRightRect;//是否在菜单区域。true为在菜单区域。
	uint8 m_nStateLeft;
	uint8 m_nStateRight;
};

////////////////////////////////////////////////////////////////////////
class DUI_API CAutoSizeButtonUI:public CButtonUI
{
public:
	CAutoSizeButtonUI();
	virtual ~CAutoSizeButtonUI();
	UI_DECLARE_DYNCREATE();

	void SetText(LPCTSTR lpszText);
protected:
	virtual void  SetRect(RECT& rectRegion);
};

#endif