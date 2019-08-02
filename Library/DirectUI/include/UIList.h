/*********************************************************************
*            Copyright (C) 2010, 网龙天晴程序应用软件部
**********************************************************************
*   Date        Name        Description
*   10/20/2010  hanzp		Create.
*   05/05/2011  hanzp       Add Class CListCtrlUI
*********************************************************************/

#ifndef UILIST_H
#define UILIST_H

class DUI_API CListUI;
class DUI_API CListItemUI;
class CCheckListItemUI;
class CListHeaderUI;
class DUI_API CListInsideUI :public CLayoutUI
{
	friend class CListUI;
	friend class CComboBoxUI;
public:
	CListInsideUI();
	virtual ~CListInsideUI();
public:
	CControlUI*	GetCurSel();
	void SetSelEnd(int nPos = -1);
	virtual void RemoveAll();
	virtual bool Remove(CControlUI* pControl);
	virtual bool SelectItem(CControlUI* pControl);
	bool SelectItemAdd(CControlUI* pControl);//按住ctrl
	bool SelectItemEnd(CControlUI* pControl);//按住shift
	bool SelectItemAll(bool bSelect = true);//全选
	bool IsMultiSelect(){return m_bMultiSelect;}
	bool IsEnableSelect(){return m_bEnableSelect;}

	virtual ImageObj* GetStateImage(UITYPE_STATE enState = UISTATE_NORMAL);
	virtual ImageObj* GetItemBkgImage();
	bool IsEnableItemBkg();
	virtual void EnsureVisible(CControlUI* pControl);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);

	virtual bool ChildEvent(TEventUI& event);
	CControlUI* ItemFromPos(POINT pt);
	CControlUI* GetCurPosItem();
protected:
	virtual void RenderItemBkg(ImageObj* pImgState, CControlUI* pControl, IRenderDC* pRenderDC, RECT& rcPaint);
	virtual void Init();
	virtual bool Event(TEventUI& event);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual SIZE OnChildSize(RECT& rcWinow, RECT& rcClient);
	CControlUI* GetNextVisible(CControlUI* pControl);
	CControlUI* GetPrevVisible(CControlUI* pControl);
protected:
	CControlUI* m_pCurSel;
	CControlUI* m_pHighlight;
private:
	tstring m_szCurSel;
	bool m_bMultiSelect;
	bool m_bEnableSelect;//是否让列表项有选择效果
	bool m_bEnableItemBkg;//是否让列表项有背景
	bool m_bEnableItemOver;//是否让列表项有划过背景
	bool m_bDrawLimit;//是否只画有效区域
	int m_nFirstVisible;//第一个可见项的索引
};

//////////////////////////////////////////////////////////////////////////
class DUI_API CListHeaderItemUI : public CControlUI
{
	friend class CListHeaderUI;
	friend class CListItemUI;
	friend class CListContainerItemUI;
	static int ms_nMinWidth;
public:
	CListHeaderItemUI();
	virtual ~CListHeaderItemUI();
	UI_DECLARE_DYNCREATE();

	CControlUI* GetOwner();
	void SetOwner(CControlUI* val);
	bool IsAsc();
	UINT GetColAlign();
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);

	int GetPercentWidth();
protected:
	virtual bool Event(TEventUI& event);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	RECT GetArrowRect();
	int GetArrowRight(POINT& pt);
	virtual void OnLanguageChange();

private:
	bool	m_bDragStatus;
	bool	m_bDragable;
	bool	m_bEnableSort;
	bool	m_bAsc;
	POINT   m_ptLastMouse;
	UINT	m_uColAlignStyle;
	int m_nPercentWidth;
	CControlUI* m_pOwner;
};

//////////////////////////////////////////////////////////////////////////
class CListHeaderUI : public CLayoutUI
{
public:
	CListHeaderUI();
	virtual ~CListHeaderUI();
	UI_DECLARE_DYNCREATE();

public:
	SIZE GetChildSize();

protected:
	virtual SIZE OnChildSize(RECT& rcWinow, RECT& rcClient);
private:
	SIZE m_szChildSize;
};

//对于第n列，控件A和控件B比较。
//参数bool 和 返回值一样的时候进行交换
typedef bool (__stdcall* LISTSORTPROC)(CListItemUI*, CListItemUI*, int, bool);
//////////////////////////////////////////////////////////////////////////
class DUI_API CListUI : public CListInsideUI, public INotifyUI
{
	friend class CListInsideUI;
	friend class CListItemUI;
	friend class CListHeaderUI;
	friend class CListHeaderItemUI;
	friend class CListContainerItemUI;
	friend class CCheckListItemUI;
public:
	CListUI();
	virtual ~CListUI();
	UI_DECLARE_DYNCREATE();


	UIBEGIN_MSG_MAP
		EVNET_HANDLER(UINOTIFY_CLICK, OnBtnClick)
	UIEND_MSG_MAP

	bool  OnBtnClick(TNotifyUI* pNotify);

	bool IsHeaderEnableSort();
	bool SetHeaderText(int nCol, LPCTSTR lpszText);         //设置列标题文字
	void SetItemText(int nIndex, int nCol, LPCTSTR lpszText);                   //设置单元格文字
	LPCTSTR GetItemText(int nIndex, int nCol);              //获取单元格文字，空为NULL

	void SetSortCallBack(LISTSORTPROC);         //设置排序比较函数
	int  GetSortCol();                //获取当前排序列
	void SetSortCol(int nCol);          //设置当前排序列
	void Sort();                        //排序前须先配置比较函数

	UINT GetCols();
	CListHeaderItemUI* GetCol(int nIndex);
	RECT GetColRect(int nIndex);
	bool InsertCol(CListHeaderItemUI* pItem, int nIndex = -1);                  //添加列标题

	void CheckInvert();//反选

	void CheckItem(bool bCheck, int nIndex = -1, bool bNotify = false);//-1为全选
	bool IsCheckItem(int nIndex);

	void CheckItemEnabled(bool bEnable, int nIndex = -1);//-1为全选
	bool IsCheckItemEnabled(int nIndex);

	void CheckItemVisible(bool bVisible, int nIndex = -1);//-1为全选
	bool IsCheckItemVisible(int nIndex);

	void CheckLegal();

	virtual bool Add(CControlUI* pControl);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual bool Remove(CControlUI* pControl);
	virtual void RemoveAll();
	virtual bool Insert(CControlUI* pControl, CControlUI* pAfter);
	virtual void Init();
	virtual bool Resize();
protected:
	virtual void OnLanguageChange();
	virtual SIZE OnChildSize(RECT& rcWinow, RECT& rcClient);
	virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual void SetRect(RECT& rectRegion);
	CListHeaderUI* m_pHeader;
	CCheckListItemUI* m_pCheck;
private:
	int m_nCurSortCol;
	int m_nHeaderHeight;//表头高度
	int m_nCheckWidth;
	RECT m_rcOldRectInset;
	bool m_bHeaderVisible;//表头是否显示
	bool m_bEnableHeaderSort;//表头是否可以点击排序
	bool m_bEnableCheck;//是否有check
	bool m_bVisibleHeaderCheck;//是否显示表头的check
	LISTSORTPROC m_pCompareProc;//nCol 当前选择的列数，-1为没有选择列
};

//////////////////////////////////////////////////////////////////////////

class DUI_API CListItemUI : public CControlUI
{
public:
	CListItemUI();
	virtual ~CListItemUI();
	UI_DECLARE_DYNCREATE();

public:
	void			SetSubItemText(int nCol, LPCTSTR lpszText);
	LPCTSTR			GetSubItemText(int nCol);
	void			SetSubItemUserdata(int nCol, unsigned long long uUserData);
	unsigned long long			GetSubItemUserdata(int nCol);
	virtual bool    IsSelect();
	virtual void	Select(bool bSelect = true, bool bRedraw = true);

	CCheckListItemUI*	GetCheck();
	void			SetCheck(CCheckListItemUI* pCheck);
protected:
	virtual void	Render(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual bool	Event(TEventUI& event);
	virtual bool	IsToolTipShow();
	virtual bool    Activate();
	virtual bool    IsActivateByReturn();
	std::map<UINT, tstring> m_mapColsText;
	std::map<UINT, unsigned long long> m_mapColsUserdata;
private:
	CCheckListItemUI* m_pCheck;
};
//////////////////////////////////////////////////////////////////////
class DUI_API CListContainerItemUI :public CListItemUI
{
public:
	CListContainerItemUI();
	~CListContainerItemUI();
	UI_DECLARE_DYNCREATE();

	int GetCount();
	CControlUI* GetItem(int iIndex);
	CControlUI* GetItem(LPCTSTR lpszId);
	bool Add(CControlUI* pControl);
	bool Remove(CControlUI* pControl);
	void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	void ReCalcRect();

protected:
	bool Activate();
	void SetRect(RECT& rectRegion);
	bool ChildEvent(TEventUI& event);
	void SetINotifyUI(INotifyUI* pINotify);
	void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);
	void SetManager(CWindowUI* pManager, CControlUI* pParent);
	void CalcRect();
private:
	CStdPtrArray m_items;
	bool m_bCalc;
	bool m_bMaskClick;
};


//////////////////////////////////////////////////////////////////////////
class DUI_API CToolListUI : public CListUI
{
public:
	CToolListUI();
	virtual ~CToolListUI();
	UI_DECLARE_DYNCREATE();
protected:
	SIZE OnChildSize(RECT& rcWinow, RECT& rcClient);
};
//////////////////////////////////////////////////////////////////////////
class DUI_API CToolItemUI : public CListItemUI
{
public:
	CToolItemUI();
	virtual ~CToolItemUI();
	UI_DECLARE_DYNCREATE();
protected:
	virtual bool    Activate();
	virtual bool	Event(TEventUI& event);
	virtual void	Render(IRenderDC* pRenderDC, RECT& rcPaint);
};

//////////////////////////////////////////////////////////////////////////
#endif