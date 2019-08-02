/*********************************************************************
*       Copyright (C) 2010,应用软件开发
*********************************************************************
*   Date             Name                 Description
*   2010-11-26       hanzp				  Create.
*********************************************************************/
#ifndef __UIACTIVEX_H__
#define __UIACTIVEX_H__

#include <mshtmhst.h>
#include <mshtml.h>
#include <Exdisp.h>
#include <exdispid.h>

//////////////////////////////////////////////////////////////////////////
enum WebContextMenuMode
{
	kDefaultMenuSupport = 0,
	kNoContextMenu,
	kTextSelectionOnly,
	kAllowAllButViewSource,
	kCustomMenuSupport,
	kWebContextMenuModeLimit
};
//////////////////////////////////////////////////////////////////////////

class CVariant : public VARIANT
{
public:
	CVariant();
	CVariant(int i);
	CVariant(float f);
	CVariant(LPOLESTR s);
	CVariant(IDispatch *disp);
	~CVariant();
};

struct IOleObject;

/////////////////////////////////////////////////////////////////////////////////////
//

class CActiveXCtrl;
class CActiveXWnd;

template< class T >
class CSafeRelease
{
public:
	CSafeRelease(T* p) : m_p(p) { };
	~CSafeRelease() { if( m_p != NULL ) m_p->Release(); };
	T* Detach() { T* t = m_p; m_p = NULL; return t; };
	T* m_p;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class DUI_API CActiveXUI : public CControlUI, public IMessageFilterUI
{
	friend class CActiveXCtrl;
	friend class CActiveXWnd;
public:
	CActiveXUI();
	virtual ~CActiveXUI();
	UI_DECLARE_DYNCREATE();

	HWND GetHostWindow() const;

	HRESULT GetControl(const IID iid, LPVOID* ppRet);
	CLSID GetClisd() const;

	virtual void Init();
	virtual void SetVisible(bool bVisible, bool bRedraw  = true );
	virtual void SetInternVisible(bool bVisible);
	virtual void SetRect(RECT& rectRegion);
	virtual void Render(IRenderDC* pRenderDC, RECT& rcPaint);

	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);

	virtual LRESULT MessageFilter(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	//存放控件ID值
	LPCTSTR GetSubId(){ return m_strSubId.c_str();}
	void SetSubId(LPCTSTR lpszSubId){m_strSubId = lpszSubId;}
	void SetContextMenuMode(int nMode){m_contextMenuMode = nMode;}
	int GetContextMenuMode(){ return m_contextMenuMode; }
	void SetEnableLoading(bool bEnable){m_bEnableLoading = bEnable;}
	bool IsEnableLoading(){return m_bEnableLoading;}
	void SetLoadingWindow(CWindowUI* pWindow){m_pLoadingWindow = pWindow;}
	CWindowUI* GetLodingWindow(){return m_pLoadingWindow;}
	void SetEnableDrop(bool bEnable){m_bEnableDrop = bEnable;}
	bool IsEnableDrop(){return m_bEnableDrop;}

	IWebBrowser2*      GetWebBrowser2();
	IDispatch*		   GetHtmlWindow();

	static DISPID FindId(IDispatch *pObj, LPOLESTR pName);
	static HRESULT InvokeMethod(IDispatch *pObj, LPOLESTR pMethod, VARIANT *pVarResult, VARIANT *ps, int cArgs);
	static HRESULT GetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue);
	static HRESULT SetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue);
	static void GetDefaultValue();
	static void SetDefaultValue(wstring strCookiesPath, wstring strCachePath);

	void SetDocHostUIHandler(IDocHostUIHandler* handler);
	void SetDispatchHandler(IDispatch* handler);

	virtual void OnWindowClose(void);
protected:
	bool DoCreateControl();
	void ReleaseControl();
	void OffsetActiveXRect();
	virtual void OnActiveXCreate(){}
	IDispatch* DisplayWeb(tstring strUrl, HWND hWndParent);

protected:
	wstring m_strSubId;
	CLSID m_clsid;
	bool m_bCreated;
	bool m_bDelayCreate;
	IOleObject* m_pUnk;
	CActiveXCtrl* m_pControl;
	HWND m_hwndHost;
	HWND m_hwndActive;
	HWND m_hwndWeb;
	IDocHostUIHandler* m_HostUIHandler;
	IDispatch*	m_pHostDispatch;

	bool m_bEnableDrop;
	bool m_bEnableLoading;
	CWindowUI* m_pLoadingWindow;
	int m_contextMenuMode;
	static wstring ms_strCookiesPath;
	static wstring ms_strCachePath;

};

#endif // __UIACTIVEX_H__
