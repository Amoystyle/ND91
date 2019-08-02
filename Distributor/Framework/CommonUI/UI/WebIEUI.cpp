#include "StdAfx.h"
#include "WebIEUI.h"

CWebIEUI::CWebIEUI( void )
{
	_pOb = NULL;
}

CWebIEUI::~CWebIEUI(void)
{
}

void CWebIEUI::OnNavigateComplete( IDispatch *pDispatch, LPCWSTR pszURL )
{
	if (_pOb)
	{
		_pOb->OnNavigateComplete(pDispatch,pszURL);
	}
}

BOOL CWebIEUI::OpenURL( wstring strUrl )
{
	IWebBrowser2* pWebBrowser = GetWebBrowser2();
	if (pWebBrowser && !strUrl.empty())
	{
		VARIANT v;
		v.vt=VT_BSTR;
		v.bstrVal=(BSTR)strUrl.c_str();
		pWebBrowser->Navigate2(&v,NULL,NULL,NULL,NULL);
	}
//	Show((_bstr_t)strUrl.c_str());
	return TRUE;
}

void CWebIEUI::OnBeforeNavigate( IDispatch *pDispatch, LPCWSTR pszURL, DWORD dwFlags, LPCWSTR pszTargetFrameName, VARIANT *vtPostData, LPCWSTR pszHeaders, VARIANT_BOOL *Cancel )
{
	if(_pOb)
	{
		bool flag = _pOb->OnBeforeNavigate(pszURL);
		if (!flag)
			*Cancel = VARIANT_TRUE;
	}

	if (*Cancel != VARIANT_TRUE)
	{
		__super::OnBeforeNavigate(pDispatch,pszURL,dwFlags,pszTargetFrameName,vtPostData,pszHeaders,Cancel);
	}
//	*Cancel = VARIANT_TRUE;
}

void CWebIEUI::OnNavigateError( IDispatch *pDispatch, LPCWSTR pszURL, LPCWSTR pszTargetFrameName, INT nStatusCode, VARIANT_BOOL *Cancel )
{
	__super::OnNavigateError(pDispatch,pszURL,pszTargetFrameName,nStatusCode,Cancel);
	if(INET_E_DOWNLOAD_FAILURE==nStatusCode)
	{
		wstring url=pszURL;
	}
}

void CWebIEUI::OnDocumentComplete( IDispatch *pDisp, LPCWSTR pszURL )
{
	__super::OnDocumentComplete(pDisp,pszURL);
	if(_pOb&&pszURL)
	{
		_pOb->OnDocumentComplete(pDisp, pszURL);
	}
}
