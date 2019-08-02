#ifndef _UILAYOUT_H
#define _UILAYOUT_H

class DUI_API CLayoutUI: public CContainerUI
{
public:
	CLayoutUI();
	virtual ~CLayoutUI();
	UI_DECLARE_DYNCREATE();
public:
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	SIZE OnChildSize(RECT& rcWinow, RECT& rcClient);

protected:
	SIZE OnNormal(RECT& rcWinow, RECT& rcClient);
	SIZE OnLeftToRight(RECT& rcWinow, RECT& rcClient);
	SIZE OnRightToLeft(RECT& rcWinow, RECT& rcClient);
	SIZE OnTopToBottom(RECT& rcWinow, RECT& rcClient);
	SIZE OnBottomToTop(RECT& rcWinow, RECT& rcClient);
	SIZE OnHorz(RECT& rcWinow, RECT& rcClient);
	SIZE OnVert(RECT& rcWinow, RECT& rcClient);
	SIZE OnGrid(RECT& rcWinow, RECT& rcClient);

	SIZE OnLeftToRightExtend(RECT& rcWinow, RECT& rcClient);
	int m_nLayoutType;
	int m_nStartX;
	int m_nStartY;
	int m_nSpaceX;
	int m_nSpaceY;
};

#endif // _UILAYOUT_H