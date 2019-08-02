/*********************************************************************
*       Copyright (C) 2010,应用软件开发
*********************************************************************
*   Date             Name                 Description
*   2010-11-26       hanzp				  Create.
*********************************************************************/
#ifndef UIIE_H
#define UIIE_H

class CIEUI;
typedef STDMETHODIMP ClientCall_Callback(DISPPARAMS* pDispParams, VARIANT* pVarResult, LPVOID pControlUI);
typedef STDMETHODIMP JsFunction_Callback(DISPPARAMS* pDispParams, VARIANT* pVarResult, LPVOID pControlUI);

class DUI_API IIEMenuCall
{
public:
	virtual bool OnShowContentMenu(DWORD dwID, POINT *ppt, LPCTSTR lpszID, LPCTSTR lpszTagname, IDispatch* pTarget, CControlUI* pControl) = 0;
};
class DUI_API IEExternal:public IDispatch
{
public:
	IEExternal();
	~IEExternal(void);
	void SetOwner(CIEUI* pOwner){m_pOwner = pOwner;}
public:
	STDMETHODIMP QueryInterface(REFIID iid,void**ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	virtual STDMETHODIMP GetTypeInfoCount(unsigned int * pctinfo);
	virtual STDMETHODIMP GetTypeInfo(unsigned int iTInfo,LCID lcid,ITypeInfo FAR* FAR* ppTInfo);
	virtual STDMETHODIMP GetIDsOfNames(REFIID riid, OLECHAR FAR* FAR* rgszNames, unsigned int cNames, LCID lcid, DISPID FAR* rgDispId );
	virtual STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, unsigned int* puArgErr);

	void AddExtFunc(LPCTSTR lpszMethodName, ClientCall_Callback* extMethod);
	void AddExtDisp(LPCTSTR lpszDispName, IDispatch* extDisp);
	IDispatch* GetExtDisp(LPCTSTR lpszDispName);

protected:
	CIEUI* m_pOwner;
	long _refNum;

private:
	int m_nLastFuncID;
	int m_nLastDispID;
	std::map<tstring, int> m_FuncNameToID;
	std::map<int, ClientCall_Callback*> m_IDToFuncCallback;
	std::map<tstring, int> m_IDispNameToID;
	std::map<int, IDispatch*> m_IDToDispatch;
};

class DUI_API CIEUI : public CActiveXUI
{
public:
	CIEUI();
	~CIEUI();
	UI_DECLARE_DYNCREATE();
public:
	void Show(BSTR URL);
	void Forward();
	bool CanForward();
	void GoBack();
	bool CanGoBack();
	void Refresh();
	void ExecCommand(LPOLESTR lpszCommand);
	bool ExecWB(
		/* [in] */ OLECMDID cmdID,
		/* [in] */ OLECMDEXECOPT cmdexecopt,
		/* [optional][in] */ __RPC__in VARIANT *pvaIn,
		/* [optional][in][out] */ __RPC__inout VARIANT *pvaOut);
	bool HaveSelection();
	LPCTSTR GetElementValue(LPOLESTR lpszID);
	bool SetElementInnerHtml(LPOLESTR lpszID, LPOLESTR lpszHtml);
	LPCTSTR GetElementInnerHtml(LPOLESTR lpszID);
	LPCTSTR GetElementInnerText(LPOLESTR lpszID);
	IDispatch* GetElement(LPOLESTR lpszID);
	LPCTSTR GetSelectionHtml();
	LPCTSTR GetSelectionText();

	void AddExtFunc(LPCTSTR lpszMethodName, ClientCall_Callback* extMethod);
	void AddExtDisp(LPCTSTR lpszMethodName, IDispatch* extDisp);
	IDispatch* GetExtDisp(LPCTSTR lpszMethodName);
	IEExternal* GetExternalCall();
	void SetMenuCall(IIEMenuCall* pExternal);
	IIEMenuCall* GetMenuCall();

	void SetEnableSelect(bool bEnable){m_bEnableSelect = bEnable;}
	bool IsEnableSelect(){return m_bEnableSelect;}
	void SetEnableScroll(bool bEnable){m_bEnableScroll = bEnable;}
	bool IsEnableScroll(){return m_bEnableScroll;}
	void SetEnableDownload(bool bEnable){m_bEnableDownload = bEnable;}
	bool IsEnableDownload(){return m_bEnableDownload;}
	void SetEnableNewWindow(bool bEnable){m_bEnableNewWindow = bEnable;}
	bool IsEnableNewWindow(){return m_bEnableNewWindow;}
	void SetEnableNativeWeb(bool bEnable){m_bEnableNativeWeb = bEnable;}
	bool IsEnableNativeWeb(){return m_bEnableNativeWeb;}
	void SetEnableRedirect(bool bEnable){m_bEnableRedirect = bEnable;}
	bool IsEnableRedirect(){return m_bEnableRedirect;}
	void SetCookiesDir(LPCTSTR lpszText){m_strCookiesPath = lpszText;}
	LPCTSTR GetCookiesDir(){return m_strCookiesPath.c_str();}
	void SetCacheDir(LPCTSTR lpszText){m_strCachePath = lpszText;}
	LPCTSTR GetCacheDir(){return m_strCachePath.c_str();}

	virtual void OnBeforeNavigate(IDispatch *pDispatch, LPCWSTR pszURL, DWORD dwFlags, LPCWSTR pszTargetFrameName, VARIANT *vtPostData, LPCWSTR pszHeaders, VARIANT_BOOL *Cancel);
	virtual void OnNavigateError(IDispatch *pDispatch, LPCWSTR pszURL, LPCWSTR pszTargetFrameName, INT nStatusCode, VARIANT_BOOL *Cancel);
	virtual void OnNavigateComplete(IDispatch *pDispatch, LPCWSTR pszURL);
	virtual void OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL);
	virtual void OnDownloadBegin(void);
	virtual void OnDownloadComplete(void);
	virtual void OnFileDownload(VARIANT_BOOL *ActiveDocument, VARIANT_BOOL *Cancel);
	virtual void OnNewWindow2(IDispatch **ppDisp, VARIANT_BOOL *Cancel);
	virtual void OnProgressChange(long Progress, long ProgressMax);
	virtual void OnStatusTextChange(LPCWSTR pszText);
	virtual void OnCommandStateChange(long nCommand, BOOL bEnable);
	virtual void OnDocumentLode(bool bDone, int nProgress, int nProgressMax);   //nProgress = -1时，完成下载


	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	tstring m_strTranslateUrl;
protected:
	bool m_bEnableBack;
	bool m_bEnableNext;
	IEExternal* m_pExternal;
	IIEMenuCall* m_pMenuCall;

	virtual void OnActiveXCreate();
	virtual void SetRect(RECT& rectRegion);
protected:
	bool m_bEnableScroll;
	bool m_bEnableSelect;
	bool m_bEnableNewWindow;
	bool m_bEnableDownload;
	bool m_bEnableExternal;
	bool m_bLoading;
	bool m_bEnableRedirect;//是否允许改变路径
	bool m_bChangedDirect;//是否改变过缓存路径
	bool m_bEnableNativeWeb;//新窗口弹出到内置web窗口，需要m_bEnableNewWindow为true
	wstring m_strCookiesPath;
	wstring m_strCachePath;
	BSTR m_Url;
};
#endif