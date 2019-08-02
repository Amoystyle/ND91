#ifndef __UICOMBOBOX_H_
#define __UICOMBOBOX_H_

class CComboBoxDropDownWnd;
class DUI_API CComboBoxUI : public CEditUI
{
	friend class CComboBoxDropDownWnd;
public:
	CComboBoxUI();
	virtual ~CComboBoxUI();
	UI_DECLARE_DYNCREATE();

	virtual bool Add(CControlUI* pControl);
	CControlUI* GetItem(int index);
	CControlUI* FindFirstItem(LPCTSTR lpszText);
	CControlUI* GetCurSel();

	virtual void SetText(LPCTSTR lpszText);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual void SelectItem(int index);
	virtual void SelectItem(CControlUI* pControl);
	virtual bool Remove(CControlUI* pControl);
	bool RemoveAll();
	int GetCount();

	bool IsEnableCheck();
	bool IsDrop();
	bool IsKillFocus();

	void CheckInvert();//反选
	void CheckItem(bool bCheck, int nIndex = -1);//-1为全选
	bool IsCheckItem(int nIndex);
protected:
	void SelectItem(int index, bool bTextChange);
	CListUI* GetList();
	RECT GetArrowRect();
	virtual void ShowDrop();//显示下拉框，并设置位置等
	void Show();
	void Hide();
	bool IsShow();
	virtual void SetRect(RECT& rectRegion);
	virtual bool Event(TEventUI& event);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual void OnChar(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnLanguageChange();
	ImageObj* m_pImageArrow;
	CWindowUI* m_pToolWindow;
	CListUI* m_pList;
	int m_nItemsEnsureVisible;
	int m_nIconIndex;
	bool m_bDropList;
	bool m_bEnableCheck;
};

#endif