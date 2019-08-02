/*********************************************************************
*            Copyright (C) 2010, �����������Ӧ�������
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
	bool SelectItemAdd(CControlUI* pControl);//��סctrl
	bool SelectItemEnd(CControlUI* pControl);//��סshift
	bool SelectItemAll(bool bSelect = true);//ȫѡ
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
	bool m_bEnableSelect;//�Ƿ����б�����ѡ��Ч��
	bool m_bEnableItemBkg;//�Ƿ����б����б���
	bool m_bEnableItemOver;//�Ƿ����б����л�������
	bool m_bDrawLimit;//�Ƿ�ֻ����Ч����
	int m_nFirstVisible;//��һ���ɼ��������
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

//���ڵ�n�У��ؼ�A�Ϳؼ�B�Ƚϡ�
//����bool �� ����ֵһ����ʱ����н���
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
	bool SetHeaderText(int nCol, LPCTSTR lpszText);         //�����б�������
	void SetItemText(int nIndex, int nCol, LPCTSTR lpszText);                   //���õ�Ԫ������
	LPCTSTR GetItemText(int nIndex, int nCol);              //��ȡ��Ԫ�����֣���ΪNULL

	void SetSortCallBack(LISTSORTPROC);         //��������ȽϺ���
	int  GetSortCol();                //��ȡ��ǰ������
	void SetSortCol(int nCol);          //���õ�ǰ������
	void Sort();                        //����ǰ�������ñȽϺ���

	UINT GetCols();
	CListHeaderItemUI* GetCol(int nIndex);
	RECT GetColRect(int nIndex);
	bool InsertCol(CListHeaderItemUI* pItem, int nIndex = -1);                  //����б���

	void CheckInvert();//��ѡ

	void CheckItem(bool bCheck, int nIndex = -1, bool bNotify = false);//-1Ϊȫѡ
	bool IsCheckItem(int nIndex);

	void CheckItemEnabled(bool bEnable, int nIndex = -1);//-1Ϊȫѡ
	bool IsCheckItemEnabled(int nIndex);

	void CheckItemVisible(bool bVisible, int nIndex = -1);//-1Ϊȫѡ
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
	int m_nHeaderHeight;//��ͷ�߶�
	int m_nCheckWidth;
	RECT m_rcOldRectInset;
	bool m_bHeaderVisible;//��ͷ�Ƿ���ʾ
	bool m_bEnableHeaderSort;//��ͷ�Ƿ���Ե������
	bool m_bEnableCheck;//�Ƿ���check
	bool m_bVisibleHeaderCheck;//�Ƿ���ʾ��ͷ��check
	LISTSORTPROC m_pCompareProc;//nCol ��ǰѡ���������-1Ϊû��ѡ����
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