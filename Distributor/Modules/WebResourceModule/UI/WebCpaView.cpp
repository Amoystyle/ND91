#include "StdAfx.h"
#include "WebCpaView.h"
#include "Logic/DownProtocolAnalysis.h"
#include "BatchDownloadThread.h"
#include "Singleton.h"
#include "Manager/ThreadManager.h"

CWebCpaView::CWebCpaView(void)
{
	_nDownloadFlag = 0;
}

CWebCpaView::~CWebCpaView(void)
{
}


void CWebCpaView::ShowIE(wstring url)
{

	CWebIEUI*	LoginIE;
	LoginIE = new CWebIEUI();
	CLayoutUI* pLayout=	(CLayoutUI*)GetItem(_T("Web.LoadWeb"));
assert(pLayout);

	LoginIE->_pOb=this;
	LoginIE->SetAttribute(_T("id"),_T("WebBrowser"));
	LoginIE->SetAttribute(_T("pos"),_T("8|0,0,0,0"));
	LoginIE->SetAttribute(_T("clsid"),_T("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
	pLayout->Add(LoginIE);

	pLayout->Resize();
	pLayout->Invalidate();

	LoginIE->OpenURL(url);

}

bool CWebCpaView::OnBeforeNavigate( LPCWSTR pszURL)
{
	_nDownloadFlag = 0;
	WebResourceInnserMsg msg;
	msg.type = WebResourceMsg_ClickGotoDes;
	msg.Send();

	wstring action = pszURL;
	if(action.find(_T("batchdownload")) != string::npos)
	{
		const string threadid = "BatchDownloadThread" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
		BatchDownloadThread * batchdownload= new BatchDownloadThread(threadid);
		batchdownload->url = action;
		batchdownload->SetCpa(true);
		batchdownload->Run();
		return false;
	}

	return true;
}

bool CWebCpaView::OnDocumentComplete( IDispatch *pDisp, LPCWSTR pszURL )
{
	return true;
}


void CWebCpaView::OnNavigateComplete( IDispatch *pDispatch, LPCWSTR pszURL )
{
	CAnimationUI *pAnimationUI = dynamic_cast<CAnimationUI *>(GetItem(_T("Web.Loading")));
	if (pAnimationUI)
		pAnimationUI->SetVisible(false);

	wstring action = pszURL;
	if(action.find(_T("download")) != string::npos)
	{
		AnalysisData *data = new AnalysisData;
		bool flag = DownProtocolAnalysis::AnalysisUrl(*data,pszURL,_T("download"));
		if (flag && data->url.length() != 0 && _nDownloadFlag == 0)
		{
			data->islegal = false;
			WebResourceInnserMsg msg;
			msg.type = WebResourceMsg_DownloadResource;
			msg.wParam = (WPARAM)data;
			msg.strParam=L"CPA";
			msg.Send();
			_nDownloadFlag++;
		}
	}
}