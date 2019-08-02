/*********************************************************************
*       Copyright (C) 2010,应用软件开发
*********************************************************************
*   Date             Name                 Description
*   2010-12-15       hanzp				  Create.
*********************************************************************/
#ifndef __MENU_H_
#define __MENU_H_

#define MENU_INSET			9			/**< 菜单边沿 */
#define MENUSEP_HEIGHT	  	4			/**< 菜单分割线所占菜单项高度 */
#define BY_LAST		 		3			/**< 插入菜单位置末尾 */
#define BY_FIRST		 	2			/**< 插入菜单位置开头 */
#define MASK_MATCH(check,mask)	(((check) & (mask)) == (mask))

class CMenuWindowUI;
class CMenuUI;
class DUI_API UIMENUINFO
{
public:
	UIMENUINFO(){
		uID = 0;
		szText = _T("");
		szIconFile = _T("");
		szKey = _T("");
		bCheck = false;
		bSeparator = false;
		bEnableItem = true;
		bAutoDel = true;
		uKey = 0;
		uImageSize = 11;
		wParamNotify = NULL;
		lParamNotify = NULL;
	}
	UINT uID;
	UINT uKey;
	UINT uImageSize;
	tstring szText;
	tstring szIconFile;
	tstring szKey;
	bool bCheck;
	bool bSeparator;
	bool bEnableItem;
	bool bAutoDel;
	WPARAM wParamNotify;
	LPARAM lParamNotify;

	CStdPtrArray MppSubWindow;
};

class DUI_API CMenuUI : public CControlUI
{
	friend class CMenuWindowUI;
public:
	typedef enum
	{
		UIMENU_NOHANDLE = 0x0000,
		UIMENU_CENTERALIGN = 0x0001,
		UIMENU_LEFTALIGN = 0x0002,
		UIMENU_RIGHTALIGN = 0x0004,
		UIMENU_BOTTOMALIGN = 0x0008,
		UIMENU_TOPALIGN = 0x0010,
		UIMENU_VCENTERALIGN = 0x0020
	};
public:
	CMenuUI();
	virtual ~CMenuUI();
	UI_DECLARE_DYNCREATE();
	static void ReleaseInstance();

	static CMenuUI* Load(UINT uMenuID);

	void Show(HWND hWndParent, LPPOINT lpPoint, INotifyUI* pNotify);
	void ShowEx(HWND hWndParent, LPPOINT lpPoint, INotifyUI* pNotify, UINT uFlags);
	bool Insert(UINT uid, LPCTSTR lpszText, UINT uFlag = BY_LAST, UINT nIndex = -1);
	bool Insert(CMenuUI* pMenu, UINT uFlag = BY_LAST, UINT nIndex = -1);
	bool InsertSep(UINT nIndex = -1, UINT uFlag = BY_LAST);
	virtual bool Remove(CControlUI* pControl);
	virtual bool Remove(int nIndex, int uFlag = BY_POSITION);
	CMenuUI* GetItem(int nIndex, int uFlag = BY_POSITION);
	UIMENUINFO& GetMenuInfo();
	void SetUID(UINT uID);
	UINT GetUID();
	bool IsEnableItem();
	void EnableItem(bool bEnable = true);
	bool IsCheck();
	void Check(bool bCheck = true);
	bool IsSeparator();
	void Separator(bool bSeparator = true);
	virtual void SetText(LPCTSTR lpszText);
	virtual LPCTSTR GetText();
	virtual void SetTextColor(DWORD dwTextColor = RGB(0,0,0));
	void SetMenuWindow(CMenuWindowUI* pWindow){m_pWindow = pWindow;}
	CMenuWindowUI* GetMenuWindow(){return m_pWindow;}
	virtual void SetMenuAttribute(int nIndex, LPCTSTR lpszName, LPCTSTR lpszValue, int uFlag = BY_POSITION);
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual void SetActiveShow(bool bActive){m_bActiveShow = bActive;}//打开菜单时是否要激活窗口
	virtual bool IsActiveShow(){return m_bActiveShow;}
	int GetCount(){return m_items.GetSize();}
	void SetParam(WPARAM wParam, LPARAM lParam);
protected:
	void Popup(HWND hParent, int x = -1, int y = -1, INotifyUI* pNotify = NULL, UINT uFlags = UIMENU_NOHANDLE);
	bool InsertAt(int iIndex, CMenuUI* pMenu);
	SIZE GetWindowSize(){return m_szWindow;}
	WPARAM GetWParam();
	LPARAM GetLParam();

	int GetIndex(UINT uID);
	void UnLoadWindow();

	void SetMenuItemRect(RECT& rc);
	RECT GetMenuItemRect();

	int  HitItem(POINT ptMouse);
	bool SelectItem(int nIndex);
	void DrawItem(IRenderDC* pRenderDC, RECT& rcPaint);

	virtual RECT GetRect();
 	virtual void Init();
	virtual bool Add(CControlUI* pControl);
	virtual void SetRect(RECT& rectRegion);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual bool Event(TEventUI& event);
private:
	int  m_nSelect;
	RECT m_rcMenuItem;
	SIZE m_szItem;
	SIZE m_szWindow;
	tstring m_strBackGround;
	CMenuWindowUI*  m_pWindow;
	CStdPtrArray m_items;
	bool m_bActiveShow;

private:
	UIMENUINFO* m_pMenuInfo;
};
#endif