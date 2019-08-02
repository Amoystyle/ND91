#pragma once
#include "DirectUI.h"
#include "AnimationRender.h"

class COMMONUI_API CAutoGridLayoutUI : public CLayoutUI, public INotifyUI, public IStaticUI
{
public:
	UIBEGIN_MSG_MAP
		EVNET_HANDLER(UINOTIFY_CLICK, OnBtnRadio)
	UIEND_MSG_MAP

	CAutoGridLayoutUI();
	~CAutoGridLayoutUI();

	UI_DECLARE_DYNCREATE();

	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual bool Add(CControlUI* pControl);

	virtual bool Insert(CControlUI* pControl, CControlUI* pAfter);

	virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

	void SelectPage(int nPageIndex);

	int GetPages(){return m_nPages;}

protected:
	bool OnBtnRadio(TNotifyUI* pNotify);
	bool ChildEvent(TEventUI& event);

protected:
	virtual void Init();
	virtual void SetRect(RECT& rc);
	virtual bool Event(TEventUI& event);

	virtual SIZE OnChildSize(RECT& rcWinow, RECT& rcClient);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);

	bool OnCurAnimation(int nMoveType);
	bool OnNextAnimation(int nMoveType);
private:
	int m_nCurIndex;
	int m_nAvgPageCount;
	int m_nPages;
	int m_nGridWidth;
	int m_nGridHeight;

	CRITICAL_SECTION m_csLock;
	bool m_bLockRender;
	AnimationRender* m_pAnimationRenderCur;
	AnimationRender* m_pAnimationRenderNext;
	LPPOINT m_ptLast;
	LPPOINT m_ptMove;

	wstring m_RadioStyle;
	wstring m_RadioStyleL;
	wstring m_RadioStyleR;
	bool m_bShowRadio;
	CLayoutUI* m_pRadioLayout;
	int m_nRadioSize;
	bool m_enablePage;
};


