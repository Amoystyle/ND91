#pragma once
#include "WebIEUI2.h"
class ChkCpaDispatcher;
class CWebCpaView
	:public CWindowUI
	,public IWebObserver2
	,BaseHandler<CpaWebButtonStatusChangedMsgEx>
	,BaseHandler<HomeWebCpaButtonStatusChangedMsg>
{
public:
	CWebCpaView(wstring deviceId, wstring url=L"");
	~CWebCpaView(void);

	void ShowIE(wstring url=L"");
	void OnCreate();
	void OnInitShow();
	virtual void OnSetWindowRgn(){};
private:
	bool OnBeforeNavigate(LPCWSTR pszURL);

	bool OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL);

	void OnNavigateComplete(IDispatch *pDispatch, LPCWSTR pszURL);

	virtual bool Handle( const CpaWebButtonStatusChangedMsgEx* pMsg );

	virtual bool Handle( const HomeWebCpaButtonStatusChangedMsg* pMsg );

	string  GetUrlActionValue( string url, string action );

	void  Split( const string &s, char delim, vector<string> &elems );

	void  CheckInstalledCpa(string strHash);
private:
		int		_nDownloadFlag;		//由于页面连接会跳转2次，整合成一次处理
		int     _nPlatForm;
		wstring _deviceId;
		int     _cpadispatherGUID;
		CWebIEUI2*	LoginIE;
		wstring _wsUrl;
};
