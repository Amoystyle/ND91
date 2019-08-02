/*********************************************************************
 *       Copyright (C) 2011,应用软件开发
 **********************************************************************
 *   Date             Name                 Description
 *   2011-06-07       hanzp
*********************************************************************/

#ifndef __UICONTROLWND_H_
#define __UICONTROLWND_H_

class CControlWndUI;
class CControlWnd;

class DUI_API CControlWndUI : public CControlUI
{
	friend class CControlWnd;
public:
	CControlWndUI(void);
	virtual ~CControlWndUI(void);
	UI_DECLARE_DYNCREATE();

	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual void SetVisible(bool bShow, bool bRedraw = true);
	virtual void SetInternVisible(bool bShow);
	HWND GetHWND();
	bool ShowWnd(bool bShow);
	//************************************
	// @brief:    绑定窗口句柄
	// @note:     
	// @param: 	  HWND hWnd 若当前绑定句柄和hWnd不一致时做一次释放
	// @param: 	  bool bFloatWindow 是否为浮动窗口
	// @param: 	  bool bAutoDel 是否在释放控件时摧毁窗口
	// @param: 	  bool bBindOffset 是否与控件位置一致
	// @return:   void 
	//************************************
	void Docked(HWND hWnd, bool bFloatWindow = false, bool bAutoDel = false);
	void FloatWindow(bool bFloatWindow);
	bool IsFloatWindow();
protected:
	virtual void SetRect(RECT& rectRegion);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);
	virtual void Init();
	void InitControl();
	bool DoCreateControl();
	void ReleaseControl();
	bool IsTop();
	//************************************
	// @brief:    用于控制句柄是否显示
	// @note:     释放控件时必须隐藏，不调用该函数
	// @param: 	  bool bShow 
	// @return:   void 
	//************************************
	void ShowWindow(bool bShow);
private:
	bool m_bShowWnd;
	bool m_bAttach;
	bool m_bCreated;
	bool m_bFloatWindow;
	bool m_bAutoDel;
	HWND m_hWindow;
	bool m_bTop;
};

#endif