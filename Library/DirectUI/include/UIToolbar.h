#ifndef __UITOOLBAR_H_
#define __UITOOLBAR_H_

class CToolBarUI : public CContainerUI
{
public:
	CToolBarUI();
	virtual ~CToolBarUI();
	UI_DECLARE_DYNCREATE();

	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
protected:
	virtual void Init();
	virtual bool ChildEvent(TEventUI& event);
	virtual SIZE OnChildSize(RECT& rcWinow, RECT& rcClient);
	virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);
	SIZE OnLeftToRight(RECT& rcWinow, RECT& rcClient);
	SIZE OnTopToBottom(RECT& rcWinow, RECT& rcClient);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	int m_nToolBarType;
	int m_nStartX;
	int m_nStartY;
	int m_nSpaceX;
	int m_nSpaceY;
	int m_nLimitIndex;
	CControlUI* m_pIconButton;
};

#endif