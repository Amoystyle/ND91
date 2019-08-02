#include "stdafx.h"
#include "FeedbackUI.h"
#include <WinInet.h>
#pragma comment(lib, "wininet.lib")
#include "FeedbackController.h"

CWebIEUI*	_pIE;
std::string _Url;
CLayoutUI* _LAY;
CControlUI* _cCui1;
CControlUI* _cCui2;

FeedbackDlg::FeedbackDlg()
{
	_pIE = NULL;
	Create(Singleton::GetMainWnd(),_T("FeedbackDlg"));
	CenterWindow(Singleton::GetMainWnd());
	SetWndStyle(GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	SetEnableResize(false);
}

FeedbackDlg::~FeedbackDlg()
{
}

DWORD WINAPI FeedbackDlg::CheckAction( PVOID lpParam )
{
	BOOL bConnect=InternetCheckConnection(_T("http://zj.91.com/"),FLAG_ICC_FORCE_CONNECTION,0);
	if ( !bConnect )
	{
		_LAY->SetVisible(false);
		_cCui1->SetVisible(true);
	}
	else
	{
		FeedbackUiMsg msg;
		msg.SendToUI();
	}

 	_cCui2->SetVisible(false);

	return 0;
}

void FeedbackDlg::OnCreate()
{
	_cCui1 = GetItem(_T("Web_Error"));
	_cCui2 = GetItem(_T("LoadingUI"));

	_cCui1->SetVisible(false);
	_pIE = new CWebIEUI();
	_LAY =	(CLayoutUI*)GetItem(_T("Feedback_webres"));
	if ( _LAY )
	{
		_LAY->RemoveAll();
		_pIE->_pOb=this;
		_pIE->SetAttribute(_T("id"),_T("webie"));
		_pIE->SetAttribute(_T("pos"),_T("8|0,0,0,0"));
		_pIE->SetAttribute(_T("clsid"),_T("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
		_pIE->SetAttribute(_T("enableScroll"),_T("0"));
		_pIE->SetAttribute(_T("enableTextSelect"),_T("1"));
		_pIE->SetAttribute(_T("enableMenu"),_T("1"));
		_pIE->SetAttribute(_T("enableNewWindow"),_T("1"));
		_pIE->SetAttribute(_T("enableloading"),_T("0"));
		_pIE->SetAttribute(_T("enableexternal"),_T("0"));
		_LAY->Add(_pIE);
		_LAY->Resize();
		_LAY->SetVisible(false);
	}

	_cCui2->SetVisible(true);
	
	FeedbackController model;
	string url = model.GetUrl();
	
	_Url = url;
	DWORD nThreadID;
	HANDLE nHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)(CheckAction), NULL, 0, &nThreadID);

}

bool FeedbackDlg::OnBeforeNavigate( LPCWSTR pszURL )
{
	RETURN_FALSE_IF ( !pszURL );

	wstring tmp= pszURL;
	if ( (int)tmp.find(L"+") > 0 )
	{
	}
	return true;
}

bool FeedbackDlg::OnDocumentComplete( IDispatch *pDisp, LPCWSTR pszURL )
{
	RETURN_FALSE_IF ( !pszURL );
	
	wstring tmp= pszURL;
	if ( (int)tmp.find(L"isSuccess=1") > 0 )
	{
		EndDialog();
	}
	if ( (int)tmp.find(L"closed=1") > 0 )
	{
		EndDialog();
	}

	return true;
}


std::wstring FeedbackDlg::GetStyleID()
{
	return L"FeedbackDlg";
}

void FeedbackDlg::OnNavigateComplete( IDispatch *pDispatch, LPCWSTR pszURL )
{

}

bool FeedbackDlg::Handle( const FeedbackUiMsg* pMsg )
{
	_LAY->SetVisible(true);
	_pIE->OpenURL( (BSTR)CCodeOperation::GB2312ToUnicode(_Url).c_str() );
	Invalidate();
	return true;
}
