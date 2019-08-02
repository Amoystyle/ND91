/*********************************************************************
*       Copyright (C) 2010,应用软件开发
*********************************************************************
*   Date             Name                 Description
*   2010-12-15       hanzp				  Create.
*********************************************************************/
#ifndef __TOOLTIP_H_
#define __TOOLTIP_H_


//BL bottom-left
//BR bottom-right
//LB left-bottom
//LT left-top
//RB right-bottom
//RT right-top
//TL top-left
//TR top-right
typedef STDMETHODIMP ToolTip_Callback(CWindowUI* pWindow, WPARAM wParam, LPARAM lParam);

class DUI_API CToolTipUI : public CNoActiveWindowUI
{
public:
	typedef enum _tagToolTipType
	{
		enToolTipTypeSucceed = 0,
		enToolTipTypeInfo,
		enToolTipTypeWarn,
		enToolTipTypeError,
		enToolTipTypeQuestion
	}ENToolTipType;
public:
	CToolTipUI(LPCTSTR lpszContent, ENToolTipType type = enToolTipTypeInfo, HWND hParentWnd = HWND_DESKTOP);
	~CToolTipUI();
	LPCTSTR GetWindowClassName();
	static CToolTipUI* GetInstance();
	static CToolTipUI* GetInstance(LPCTSTR lpszContent, ENToolTipType type = enToolTipTypeInfo);


	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_TIMER, L"content", OnShowTimer)
	UIEND_MSG_MAP

	bool OnShowTimer(TNotifyUI* pNotify);

	void SetLockTimer(bool bLock);
	void SetPointTo(POINT pt);
	void SetAlign(LPCTSTR lpszAlign);
	void SetWidth(int nWidth);
	void SetElapse(int nElapse);
	void SetContent(LPCTSTR lpszContent);
	void SetToolTipType(UINT uType) {m_uIconType = uType;}
	void SetCallBack(ToolTip_Callback* pCallBack, WPARAM wParam, LPARAM lParam);
	virtual bool ShowWindow(int nCmdShow = SW_SHOW);
protected:
	tstring  GetModeValueFromStr(LPCTSTR lpszValue);
	void ChangePosFromStr(LPCTSTR lpszValue);
private:
	ToolTip_Callback* m_pCallBack;//关闭前的回调
	WPARAM m_pCallBackWParam;
	LPARAM m_pCallBackLParam;
	POINT m_ptPointTo;
	RECT m_rcOffset;
	UINT m_nMaxWidth;
	UINT m_nElapse;
	UINT m_uIconType;
	bool m_bLockTimer;
};

#endif