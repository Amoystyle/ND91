#ifndef _UIEDIT_AR_H
#define _UIEDIT_AR_H

#ifdef DIRECTUI_LAYOUT_RTL
#include "UIToolTip.h"

enum
{
	enEditLimitNULL = 0x0000,
	enEditLimitNumber = 0x0001,
	enEditLimitLetter = 0x0002,
	enEditLimitChinese = 0x0008,
	enEditLimitString = 0x0010,
	enEditExceptNumber = 0x0020,
	enEditExceptLetter = 0x0040,
	enEditExceptChinese = 0x0080,
	enEditExceptString = 0x01000,
};

class DUI_API CMultiLineEdit:public CRichEditUI
{
public:
	UI_DECLARE_DYNCREATE();
	CMultiLineEdit();
	~CMultiLineEdit();
public:
	LPCTSTR GetText();	
	void SetText(LPCTSTR lpszText);
	virtual void Init();
	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);

	FontObj* GetFontObj();

protected:
	bool m_bInit;
	uint8 m_nFontIndex;               //文字样式索引，配置参见xml
};


///////////////////////////////////////////////////////

class DUI_API CEditUI:public CLabelUI, public INotifyUI
{
public:
	//消息映射
	UIBEGIN_MSG_MAP
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK, IDR_MENU_UIEDIT,  OnMenuItemClick)
	UIEND_MSG_MAP

	bool OnMenuItemClick(TNotifyUI* pNotify);

	UI_DECLARE_DYNCREATE();
	CEditUI();
	~CEditUI();
public:
	LPCTSTR GetText();	
	virtual void SetText(LPCTSTR lpszText);

	bool IsReadOnly();
	void SetReadOnly(bool bReadOnly = true);
	void SetTextWithoutNotify(LPCTSTR lpszText);  //配合SearchEdit 特增添此函数，设置text后不对外界发送消息。
	void SelectAllText();  //全部选定文本，并刷新
	void EnableIME();
	void DisableIME();
	bool IsEmpty();
	bool IsMailFormat();
	bool IsNumberFormat();
	bool IsLetterFormat();
	bool IsChineseFormat();
	void HideToolTip();
	void ShowToolTip(LPCTSTR lpszContent, CToolTipUI::ENToolTipType type = CToolTipUI::enToolTipTypeInfo);

	void GetSel(int& nStartChar, int& nEndChar);
	virtual bool OnLimitChar(UINT nChar);

	void Paste();
	void Copy();
	void Cut();
	virtual void OnPaste(){}
	virtual bool SetSelEnd(int pos = -1);  //若有选定则取消选定，并设置光标的位置
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnChar(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnMouseMove(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnLButtonDown(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnLButtonDblick(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);
	virtual void OnRButtonUp(WPARAM wParam, LPARAM lParam, LPPOINT ppt = NULL);

	void OnSetFocus();
	virtual bool IsKillFocus();

	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual void Render(IRenderDC* pRenderDC, RECT &rcPaint);
	virtual bool Event(TEventUI& event);
	virtual void SetRect(RECT& rect);
	virtual void Init();
	UINT GetAlign();
	void SetAlign(UINT uStyle); 

	FontObj* GetFontObj();
protected:
	virtual void Notify(TNotifyUI* msg);
	bool IsBindTextHost();

protected:
	COLORREF m_clrText;
	UINT m_uLimitType;
	bool m_bReadOnly;
	bool m_bTransparent;
	bool m_bKillFocus;
	bool m_bLimit;
	bool m_bhasCaret;
	bool m_bMenuEnable;
	HWND m_hParentWnd;
	tstring m_strLimitString;
	tstring m_strToolTipAlign;
	HWND m_hToolTipWnd;
	bool m_bPassword;
	tstring m_strEmptyTips;
	COLORREF m_clrEmptyTipsText;

	bool m_bAutoSize;
	bool m_bMaxSize;
	SIZE m_cxyMaxSize;
	bool m_bRTLReading;
	bool m_bInit;

	UINT    m_uAlignStyle;              //文字显示方式
	UITYPE_FONT   m_nFontEffect;              //文字样式特效索引，配置参见xml
	uint8 m_nFontIndex;               //文字样式索引，配置参见xml
};



#endif // DIRECTUI_LAYOUT_RTL

#endif // _UIEDIT_AR_H