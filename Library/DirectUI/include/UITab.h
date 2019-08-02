/*********************************************************************
*       Copyright (C) 2010,应用软件开发
*********************************************************************
*   Date             Name                 Description
*   2010-11-26       hanzp				  Create.
*********************************************************************/
#ifndef UITAB_H
#define UITAB_H

//////////////////////////////////////////////////////////////////////////
class DUI_API CTabLayoutUI : public CContainerUI
{
public:
	CTabLayoutUI();
	virtual ~CTabLayoutUI();
	virtual bool Add(CControlUI* pControl);
	virtual bool Remove(CControlUI* pControl);
	virtual void RemoveAll();
	void		 SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual bool SelectItem(CControlUI* pControl);
	int			 GetCurSel();
protected:
	virtual void Init();
private:
	tstring		m_szCurSel;
	CControlUI* m_pCurSel;
};

class DUI_API CTabHeaderUI : public CLayoutUI
{
public:
	CTabHeaderUI();
	virtual ~CTabHeaderUI();
	UI_DECLARE_DYNCREATE();

	virtual CControlUI*  GetCurSel(){return m_pCurSel;}
	virtual bool SelectItem(CControlUI* pControl);

	virtual void RemoveAll();
	virtual bool Remove(CControlUI* pControl);
	virtual void Init();

	virtual bool ChildEvent(TEventUI& event);
protected:
	CControlUI* m_pCurSel;
};

class DUI_API CTabButtonUI : public CButtonUI
{
public: 
	CTabButtonUI(void);
	virtual ~CTabButtonUI(void);
	UI_DECLARE_DYNCREATE();

	bool IsShowClose(){return m_bShowClose;}
	void ShowClose(bool bShow){m_bShowClose = bShow;}
	void SetCloseState(int nCloseState){m_nCloseState = nCloseState;}
	int GetCloseState(){return m_nCloseState;}
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
protected:
	//virtual bool Event(TEventUI& event);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	int m_nCloseState;
	bool m_bShowClose;
};

class DUI_API CTabUI : public CContainerUI
{
public:
	CTabUI(void);
	virtual ~CTabUI(void);
	UI_DECLARE_DYNCREATE();
public:
	virtual bool Add(CControlUI* pControl);
	virtual bool Remove(CControlUI* pControl);
	virtual void RemoveAll();
	bool		SelectItem(int nIndex);
	bool		SelectItem(LPCTSTR szID);
	bool		SetTabHeaderVisible(int nIndex, bool bVisible = true);             //设置Tab选项卡是否可见
	bool		SetTabHeaderVisible(LPCTSTR lpszPageID, bool bVisible = true);     //设置Tab选项卡是否可见
	bool		IsHeaderItemVisible(int nIndex);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	void		SetTabHeaderAttribute(LPCTSTR lpszPageID, LPCTSTR lpszName, LPCTSTR lpszValue);
	void		SetTabHeaderAttribute(int nIndex, LPCTSTR lpszName, LPCTSTR lpszValue);
	int			GetCurSel();
	CControlUI* GetHeader(int nIndex);
	CControlUI*	GetHeader();
	void		SetHeaderInitPos(int l, int t, int r, int b, int a);
	virtual bool OnEventAddBtn(TEventUI& event);
	virtual void SetRect(RECT& rect);
protected:
	virtual void OnLanguageChange();
 	virtual void Init();
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

	int m_nMaxWidth;
	int m_nMinWidth;

	tstring		m_szCurSel;
	CTabHeaderUI*	m_pHeaderList;
	RECT		m_rcHeaderCfg;
	int			m_nHeaderSep;
	int			m_nHeaderAnchor;
	bool		m_bHeaderHide;
	bool		m_bHeaderVisible;
	bool		m_bAddButton;
};

#endif