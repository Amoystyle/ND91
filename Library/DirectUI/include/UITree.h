/*********************************************************************
*       Copyright (C) 2010,应用软件开发
*********************************************************************
*   Date             Name                 Description
*   2010-11-17       hanzp				  Create.
*********************************************************************/
#ifndef UITREE_H
#define UITREE_H


//////////////////////////////////////////////////////////////////////////
#define UITVI_ROOT                ((CTreeNodeUI*)(ULONG_PTR)-0x10000)
#define UITVI_FIRST               ((CTreeNodeUI*)(ULONG_PTR)-0x0FFFF)
#define UITVI_LAST                ((CTreeNodeUI*)(ULONG_PTR)-0x0FFFE)

class ICheckNodeUI;
class DUI_API CTreeNodeUI : public CControlUI
{
	friend class CTreeUI;
	friend class CDrawerBarUI;
public:
	enum
	{
		en_NODE_BIG_SIZE_ = 54,
		en_NODE_MIDDLE_SIZE_ = 46,
		en_NODE_SMALL_SIZE_ = 28,
		en_NODE_BIGICON_SIZE_ = 46,
		en_NODE_MIDDLEICON_SIZE_ = 36
	};

	CTreeNodeUI(bool bGroup = false);
	virtual ~CTreeNodeUI();
	UI_DECLARE_DYNCREATE();

	static bool ms_bTreeNodeExpand;
	static bool ms_bTreeNodeClick;
public:
	virtual ImageObj* GetImage();
	virtual void SetImage(LPVOID lpData, long nSize);
	virtual void SetImage(ImageObj* pImageObj);
	virtual void SetImage(LPCTSTR lpszName);
	void EnableDrag(bool bEnable);
	bool Expand(bool bExpand, bool bDown = true); // returnValue : 是否有刷新
	bool ExpandAll(bool bExpand, bool bDown = true); // returnValue : 是否有刷新
	bool IsExpand();
	bool IsGroup();
	int  num_child();
	int find_child(CTreeNodeUI* node);
	CTreeNodeUI* get_child(int i);
	CTreeNodeUI* get_last_child();
	CTreeNodeUI* get_parent();
	CTreeNodeUI* get_next_sibling();
	CTreeNodeUI* get_prev_sibling();
	CTreeNodeUI* find_child(UINT uData);
	void SetMyData(void *pData);
	void* GetMyData();
	virtual void SetRect(RECT& rectRegion);
	virtual void CalcRect();

	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);

	//checknode type
	void Check(bool bCheck, bool bDown = true, bool bNotify = false);
	bool IsCheck();
	bool CheckActivate();
	void CheckVisible(bool bVisible);
	bool IsCheckVisible(){return m_bCheckVisible;}
	void SetCheckRect(RECT &rc);
	int  CheckOffset();
	void RenderCheck(IRenderDC* pRenderDC, RECT& rcPaint, int nIndex);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual bool Add(CControlUI* pControl);
protected:
	virtual bool Activate();
	virtual bool IsActivateByReturn();
	virtual bool Event(TEventUI& event);
	void add_child(CTreeNodeUI* child, int nIndex);
	void remove_child(CTreeNodeUI* child);
	void remove_all();
	bool  m_bCanDrag;
	bool  m_bExpand;
	bool  m_bEnableExpand;
	bool  m_bCheck;
	bool  m_bShake;
	bool  m_bGroup;
	CStdPtrArray* _child;
	CTreeNodeUI*  _parent;
	ICheckNodeUI* m_pCheckNode;
	bool			m_bCheckVisible;
	bool			m_bCalc;

protected:
	void*			m_pMyData;
};

class DUI_API CTreeContainerNodeUI : public CTreeNodeUI
{
public:
	CTreeContainerNodeUI(bool bGroup = false);
	~CTreeContainerNodeUI();
	UI_DECLARE_DYNCREATE();

	CControlUI* GetItem(int iIndex);
	CControlUI* GetItem(LPCTSTR lpszId);
	int GetCount(){return m_items.GetSize();}
	bool Add(CControlUI* pControl);
	bool Remove(CControlUI* pControl);
	void SetINotifyUI(INotifyUI* pINotify);
	virtual void SetRect(RECT& rectRegion);
protected:
	virtual bool ChildEvent(TEventUI& event);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);
	virtual void CalcRect();
	virtual void SetManager(CWindowUI* pManager, CControlUI* pParent);
protected:
private:
	CStdPtrArray m_items;
};
 
///////////////////////////////////////////////////////////////////////////////////////////
typedef bool UITreeNodeShowPloyFunc(CTreeNodeUI* , CTreeUI* );
class DUI_API CTreeUI : public CContainerUI
{
	friend class CTreeNodeUI;
public:
	CTreeUI();
	virtual ~CTreeUI();
	UI_DECLARE_DYNCREATE();

public:
	CTreeNodeUI* GetRoot();
	virtual CTreeNodeUI* InsertItem(CTreeNodeUI* pControl, CTreeNodeUI* pParent = UITVI_ROOT, CTreeNodeUI* pInsertAfter = UITVI_LAST);
	virtual CTreeNodeUI* InsertItem(CTreeNodeUI* pControl, CTreeNodeUI* pParent, bool(* pCompare)(CTreeNodeUI* ,CTreeNodeUI* ));
	virtual void RemoveItem(CTreeNodeUI* pControl);
	virtual bool Add(CControlUI* pControl);
	virtual bool Remove(CControlUI* pControl);
	virtual void RemoveAll();
	virtual bool SelectItem(CControlUI* pControl);
	virtual bool SelectItem(int nIndex);
	void EnsureVisible(CControlUI* pControl);
	int EnsureVisibleCenter(CControlUI* pControl,int& nBegin, int &nEnd, bool bRedraw = true);
	int SetScrollPos(int nPos);
	bool Sort(bool(* pCompare)(CTreeNodeUI* ,CTreeNodeUI* ), CTreeNodeUI* node = UITVI_ROOT);
	UITreeNodeShowPloyFunc* SetShowPloy(UITreeNodeShowPloyFunc* pFunc);
	int FindItem(UINT uData, CStdPtrArray& items);

	// 设置节点高度
	void SetItemHeight(int nHeight);
	int GetItemHeight(){return m_nItemHeight;};
	void SetGroupHeight(int nHeight);
	int GetGroupHeight(){return m_nGroupHeight;};
	void SetSelectedHeight(int nHeight);
	int GetSelectedHeight(){return m_nSelectedHeight;};
	
	void SetMaginToParent(int nMagin);  //设置子节点与父节点的偏移

	bool IsMutex(){return m_bMutex;}
	virtual bool ChildEvent(TEventUI& event);

	CControlUI*  ItemFromPos(POINT pt);
	CControlUI* GetNextVisible(CControlUI* pControl);
	CControlUI* GetPrevVisible(CControlUI* pControl);
	CControlUI* GetCurSel();
	virtual ImageObj* GetStateImage(UITYPE_STATE enState = UISTATE_NORMAL);
	virtual ImageObj* GetItemBkgImage();

	bool EnableGroupItemSelect() const;
	bool IsEnableItemBkg();
	bool IsEnableLock();
	void EnableLock(bool bLock);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);

protected:
	virtual bool Event(TEventUI& event);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual SIZE OnChildSize(RECT& rcWinow, RECT& rcClient);
	virtual void SetItemInset(CTreeNodeUI* pControl, CTreeNodeUI* pParent);
	virtual void RenderItemBkg(ImageObj* pImgState, CControlUI* pControl, IRenderDC* pRenderDC, RECT& rcPaint);
	void SetItemVisible(CTreeNodeUI* pControl, bool bVisible);
	void Swap(CTreeNodeUI* node, int i, int j);

protected:
	CTreeNodeUI* _root;
	CControlUI* m_pCurSel;
	int m_nItemHeight;
	int m_nGroupHeight;
	int m_nSelectedHeight;
	int m_nMaginToParent;
	bool m_bMutex;
	bool m_bDragNode;
	int m_nFirstVisible;
	bool m_bEnableItemBkg;
	bool m_bEnableSelect;
	bool m_bEnableLock;
	bool m_bLimitGroup;
	bool m_bEnableItemOver;//是否让列表项有鼠标移动过程中的背景
	bool m_bEnableGroupItemSelect;//是否允许组节点显示选中态
	bool m_bDrawLimit;//是否只画有效区域
	UITreeNodeShowPloyFunc* m_pShowPloyFunc;
};

//////////////////////////////////////////////////////////////////////////
class ICheckNodeUI
{
public:
	ICheckNodeUI(CTreeNodeUI* pOwner);
	~ICheckNodeUI();
	int  CheckOffset(){return 17;}
	bool Active();
	bool Check();
	void SetRect(RECT& rectRegion);
	bool HitTest(POINT point);
	void RenderCheck(IRenderDC* pRenderDC, RECT& rcPaint, int nIndex);
protected:
	RECT m_rcItem;
	int  m_nInsetIndex;
	ImageObj* m_pImageObj;
	CTreeNodeUI* m_pOwner;
};
//////////////////////////////////////////////////////////////////////////
class DUI_API CDrawerBarUI : public CTreeUI
{
public:
	CDrawerBarUI();
	virtual ~CDrawerBarUI();
	UI_DECLARE_DYNCREATE();
	bool SelectItem(CControlUI* pControl);
	virtual bool ChildEvent(TEventUI& event);
protected:
	virtual SIZE OnChildSize(RECT& rcWinow, RECT& rcClient);
private:
};

//////////////////////////////////////////////////////////////////////////
class DUI_API CDrawerNodeUI : public CTreeNodeUI
{
public:
	CDrawerNodeUI(bool bGroup = false);
	virtual ~CDrawerNodeUI();

	virtual void SetImage(LPCTSTR lpszName);
protected:
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual bool Event(TEventUI& event);
};

#endif