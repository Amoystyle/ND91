#ifndef _UICONTAINER_H
#define _UICONTAINER_H

////////////////////////////////////////////////////////////////////////////////////////////////

class  DUI_API CContainerUI : public CControlUI
{
public:
	CContainerUI();
	virtual ~CContainerUI();
	UI_DECLARE_DYNCREATE();

	//控件集合的操作
public:
	virtual CControlUI* GetItem(int iIndex);
	/// 这里要关注一下的格式...
	virtual CControlUI* GetItem(LPCTSTR lpszId);
	virtual int GetCount();
	virtual bool Add(CControlUI* pControl);
	virtual bool Remove(CControlUI* pControl);
	virtual void RemoveAll();
	virtual void SetVisible(bool bShow, bool bRedraw = true);
	virtual void SetInternVisible(bool bShow);
	virtual bool IsContainer();
	virtual bool Insert(CControlUI* pControl, CControlUI* pAfter);
	virtual bool MoveChild(CControlUI* pControl, CControlUI* pAfter);
	void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false, bool bUpdate = true);
	CScrollbarUI* GetHScrollbar();
	CScrollbarUI* GetVScrollbar();
	bool RemoveWithoutDelCtrl(CControlUI* pControl);
	bool Contain(CControlUI* pControl);
	virtual void OnLanguageChange();
public:
	virtual void SetRect(RECT& rectRegion);
	virtual void SetRect( RECT& rectRegion, bool bStyle, bool bRTL = false );
	virtual void SetManager(CWindowUI* pManager, CControlUI* pParent);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	//virtual void OnSize(const RECT& rectRegion);
	virtual void Init();
	virtual SIZE OnChildSize(RECT& rcWinow, RECT& rcClient);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	/// UIFIND_TOP_FIRST 指明控件覆盖的顺序
	virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);
	virtual bool Event(TEventUI& event);
	virtual void ReleaseImage();  //
	virtual bool  HitClientTest(POINT& point);
	virtual RECT GetClientRect();
	virtual void ProcessScrollbar(int cxRequired, int cyRequired);
	virtual void SetINotifyUI(INotifyUI* pINotify);
	CStdPtrArray* GetItemArray();
protected:
	void  CalcChildRect();

protected:
	bool m_bEnableVertical;
	bool m_bEnableHorizontal;
	bool m_bAutoFitHeight;
	bool m_bAutoFitWidth;
	CScrollbarUI* m_pVerticalScrollbar;
	CScrollbarUI* m_pHorizontalScrollbar;
	RECT m_rcHScrollPos, m_rcVScrollPos;
	CStdPtrArray m_items;

	RECT m_rcClient;
	bool m_bScrollProcess; // 防止OnSize循环调用
};


#endif  // _UICONTAINER_H
