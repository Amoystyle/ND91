#pragma once

class COMMONUI_API IWebObserver
{
public:
	virtual bool OnBeforeNavigate(LPCWSTR pszURL)=0;

	virtual bool OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL)=0;

	virtual void OnNavigateComplete(IDispatch *pDispatch, LPCWSTR pszURL) = 0;
};


//继承ie类，监听OnBeforeNavigate事件url
class COMMONUI_API CWebIEUI:public CIEUI
{
public:
	CWebIEUI(void);
	~CWebIEUI(void);

	virtual void OnBeforeNavigate(IDispatch *pDispatch, LPCWSTR pszURL, DWORD dwFlags, LPCWSTR pszTargetFrameName, VARIANT *vtPostData, LPCWSTR pszHeaders, VARIANT_BOOL *Cancel);

	virtual void OnNavigateError(IDispatch *pDispatch, LPCWSTR pszURL, LPCWSTR pszTargetFrameName, INT nStatusCode, VARIANT_BOOL *Cancel);

	virtual void OnNavigateComplete(IDispatch *pDispatch, LPCWSTR pszURL);

	virtual void OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL);

	// 打开url可以回退
	BOOL OpenURL( wstring strUrl);

	IWebObserver*  _pOb;//观察OnBeforeNavigate和OnDocumentComplete事件
};
