/*********************************************************************
 *       Copyright (C) 2011,应用软件开发
 **********************************************************************
 *   Date             Name                 Description
 *   2011-01-31       hanzp                Create.
*********************************************************************/
#ifndef _UISEARCHEDIT_H_
#define _UISEARCHEDIT_H_

class CSearchToolWindowUI;
class DUI_API CSearchEditUI : public CEditUI
{
	friend class CSearchToolWindowUI;
public:
	CSearchEditUI(void);
	virtual ~CSearchEditUI();
	UI_DECLARE_DYNCREATE();

	CListUI* GetList();
	void ShowSearch();
	void HideSearch();
	void DelResult(int nIndex = 0);
	LPVOID GetResult(int nIndex = 0);
	void SetResult(LPVOID pResult);
	virtual bool Add(CControlUI* pControl);
	virtual void SetText(LPCTSTR lpszText);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);

protected:
	void ReleaseWnd();
	void InitWnd();
	virtual void HideAuto();
	virtual void OnChar(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnPaste();
	virtual bool Event(TEventUI& event);
	virtual bool ChildEvent(TEventUI& event);
	virtual void SetRect(RECT& rectRegion);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

	CStdPtrArray m_items;
	CNoActiveWindowUI* m_pToolWindow;
	CListUI* m_pList;
	bool m_bDown;
	bool m_bFirstButtonDown;
	bool m_bHideAuto;
	int m_nMaxItems;
	CStdPtrArray m_pSearchResultArr;
};

#endif