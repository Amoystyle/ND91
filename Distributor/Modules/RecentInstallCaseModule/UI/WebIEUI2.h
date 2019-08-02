#pragma once

class IWebObserver2
{
public:
	virtual bool OnBeforeNavigate(LPCWSTR pszURL)=0;

	virtual bool OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL)=0;

	virtual void OnNavigateComplete(IDispatch *pDispatch, LPCWSTR pszURL) = 0;
};

//�̳�ie�࣬����OnBeforeNavigate�¼�url
class CWebIEUI2:public CIEUI
{
public:
	CWebIEUI2(void);
	~CWebIEUI2(void);

	virtual void OnBeforeNavigate(IDispatch *pDispatch, LPCWSTR pszURL, DWORD dwFlags, LPCWSTR pszTargetFrameName, VARIANT *vtPostData, LPCWSTR pszHeaders, VARIANT_BOOL *Cancel);

	virtual void OnNavigateError(IDispatch *pDispatch, LPCWSTR pszURL, LPCWSTR pszTargetFrameName, INT nStatusCode, VARIANT_BOOL *Cancel);

	virtual void OnNavigateComplete(IDispatch *pDispatch, LPCWSTR pszURL);

	virtual void OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL);

	// ��url���Ի���
	BOOL OpenURL( wstring strUrl);

	IWebObserver2*  _pOb;//�۲�OnBeforeNavigate��OnDocumentComplete�¼�
};
