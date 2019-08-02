#include "stdafx.h"
#include "LoginDlg.h"
#include "../Logic/LoginModel.h"
#include "Common/WebResource/WebResourceMessageDefine.h"
#include "../Logic/UserCenterThread/CheckConnectThread.h"
#include "Common/UserCenter/UserCenterMsg.h"


CWebIEUI*	_LoginIE;
CWindowUI*  _pWind;
CControlUI* _cLoginCui;
CControlUI* _cLoginCui1;
CControlUI* _cLoginCui2;

LoginDlg::LoginDlg()
{
	m_nRoundRgn = 0;
	_nLoginTime = 0;

	Create(Singleton::GetMainWnd(),_T("LoginDlg"));
	CenterWindow(Singleton::GetMainWnd());
	SetWndStyle(GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	SetEnableResize(false);

}

LoginDlg::~LoginDlg()
{
	_nLoginTime = 0;
}

void LoginDlg::OnCreate()
{
	GetItem(_T("Web_Error"))->SetVisible(false);

	_cLoginCui = GetItem(_T("Login_webres"));
	_cLoginCui1 = GetItem(_T("Web_Error"));
	_cLoginCui2 = GetItem(_T("LoadingUI"));

	_cLoginCui1->SetVisible(false);
	
	if ( !LoginModel::GetInstance()->GetBeLogin() )
	{
		_LoginIE = new CWebIEUI();
		_pWind	= this;
		CLayoutUI* LAY=	(CLayoutUI*)GetItem(_T("Login_webres"));
		RETURN_IF (!LAY);

		LAY->RemoveAll();
		_LoginIE->_pOb=this;
		_LoginIE->SetAttribute(_T("id"),_T("LongIn_ie"));
		_LoginIE->SetAttribute(_T("pos"),_T("8|0,0,0,0"));
		_LoginIE->SetAttribute(_T("clsid"),_T("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
		_LoginIE->SetAttribute(_T("enableScroll"),_T("0"));
		_LoginIE->SetAttribute(_T("enableTextSelect"),_T("1"));
		_LoginIE->SetAttribute(_T("enableMenu"),_T("1"));
		_LoginIE->SetAttribute(_T("enableNewWindow"),_T("1"));
		_LoginIE->SetAttribute(_T("enableNativeWeb"),_T("0"));
		_LoginIE->SetAttribute(_T("enableloading"),_T("0"));
		_LoginIE->SetAttribute(_T("enableexternal"),_T("0"));
		LAY->Add(_LoginIE);
		LAY->Resize();
		LAY->SetVisible(false);

		CheckConnectThread* pThread = new CheckConnectThread("");
		pThread->Run();
	
	}	
	_cLoginCui2->SetVisible(true);

}

void LoginDlg::ReleaseData()
{
// 	CCheckBoxUI* check = (CCheckBoxUI*)GetItem(_T("CheckNext"));
// 	RETURN_IF( !check );
// 	bool bch = check->IsChecked();
// 	
// 	if ( !bch )
	{
		// 删除Cookie，删除文件
		InternetSetCookie(USERCENTERURL, L"ZJZS_Account", L"");
		wstring iniPath = CONFIG(_strCachePath) + L"user.ini";
		DeleteFile(iniPath.c_str());
	}
}

bool LoginDlg::OnExitBnt( TNotifyUI* pNotify )
{

    ReleaseData();

	EndDialog();

	CStaticsticsMessage msg(Statistic_ID_CloseBnt);
	msg.Send();

	return true;
}

bool LoginDlg::OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL)
{
	if ( _nLoginTime == 2 )
	{
		_nLoginTime = 0; 
		return true;
	}

	RETURN_FALSE_IF ( !pszURL );
	wstring tmp= pszURL;
	int pos = tmp.find(L"checklogin.aspx?isauto");

	bool bsave = (int)tmp.find(L"isauto=1") > 0;

	wstring username = LoginModel::GetInstance()->LoginSuccess(bsave);
	if ( !username.empty() && pos > 0 )
	{
		_nLoginTime++;

		if ( _nLoginTime == 1 )
		{
			// 关闭窗口
			EndDialog();

			// 发消息给主界面，用户登录成功
			ResponseUserLoginMsg useMsg;
			useMsg.nCpaUser		= LoginModel::GetInstance()->GetBeCpaUser() == 1;
			useMsg.bLongin		= true;
			useMsg.userName		= username;
			useMsg.channelID	= LoginModel::GetInstance()->GetChannelID();
			useMsg.userID		= LoginModel::GetInstance()->GetUserID();
			useMsg.SendToUI();

		}

	}
	return true;
}

bool LoginDlg::Handle( const CheckConnectInfoMsg* pMsg )
{
	if ( !pMsg->bConnectOK )
	{
		_cLoginCui->SetVisible(false);
		_cLoginCui1->SetVisible(true);		
	}
	else
	{
		_cLoginCui->SetVisible(true);
		_LoginIE->Show( _T("http://zj.91.com/Login_New.aspx?c=1") );
	}

	_cLoginCui2->SetVisible(false);

	return true;
	 
}