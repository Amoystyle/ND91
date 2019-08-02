#pragma once

#include "WebIEUI.h"

class CWebCpaView: 
	public CWindowUI
	,public IWebObserver
{
public:
	CWebCpaView(void);
	~CWebCpaView(void);

	void ShowIE(wstring url=L"");

private:
	bool OnBeforeNavigate(LPCWSTR pszURL);

	bool OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL);

	void OnNavigateComplete(IDispatch *pDispatch, LPCWSTR pszURL);

private:
		int		_nDownloadFlag;		//����ҳ�����ӻ���ת2�Σ����ϳ�һ�δ���
};
