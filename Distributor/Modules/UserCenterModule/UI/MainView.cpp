#include "stdafx.h"
#include "MainView.h"
#include "../Logic/LoginModel.h"
#include "LoginDlg.h"
#include "UserDataDlg.h"
#include "Common/WebResource/WebResourceMessageDefine.h"
#include "Common/Statistics/StatisticsMessageDefine.h"

MainView::MainView()
{
	LoginModel::GetInstance()->AffirmLogin();

	_pUserInfoModel = new SaveUserInfoModel;

	PostDeviceConnectNumMsg cMsg;
	cMsg.Send();

	PostStartTimeMsg msg;
	msg.Send();

}

MainView::~MainView()
{
	SAFE_DELETE(_pUserInfoModel);
}

std::wstring MainView::GetStyleID()
{
	return L"UserCenter_View_Main";
}

void MainView::StatusChange( bool isLongin, wstring name )
{
	CLayoutUI* pLayLogin = dynamic_cast<CLayoutUI*>(GetItem(L"UserLogin"));
	CLayoutUI* pLayCenter = dynamic_cast<CLayoutUI*>(GetItem(L"UserCenter"));

	RETURN_IF ( !pLayLogin || !pLayCenter );

	if ( isLongin && !name.empty() )
	{
		CButtonUI* Gotobut = (CButtonUI*)GetItem(_T("UserCenter.Button_Goto"));
		if (Gotobut) Gotobut->SetText( name.c_str() );
		
		pLayLogin->SetVisible(false);
		pLayCenter->SetVisible(true);
		pLayCenter->Resize();
	}
	else
	{
		pLayLogin->SetVisible(true);
		pLayCenter->SetVisible(false);
		pLayLogin->Resize();
	}
	//Òþ²ØµÇÂ½½çÃæ
	pLayLogin->SetVisible(false);
	pLayLogin->Resize();
	pLayCenter->SetVisible(false);
	pLayCenter->Resize();
	this->Invalidate();
}

bool MainView::OnUserLoginClick( TNotifyUI* pNotify /*= NULL*/ )
{

	CStaticsticsMessage msg(Statistic_ID_LoginInBnt);
	msg.Send();

	LoginDlg* longin = new LoginDlg;
	longin->DoModal();

	return true;
}

bool MainView::OnUserExitClick( TNotifyUI* pNotify /*= NULL*/ )
{
	wstring iniFile = CONFIG(_strCachePath) + L"ulog";
	DeleteFile(iniFile.c_str());
	StatusChange( false );
	
	LoginModel::GetInstance()->ReSetData();
	LoginModel::GetInstance()->SetBeLogin(false);

	ResponseUserInfoMsg reMsg;
	reMsg.lastTime = L"";
	reMsg.SendToUI();

	//InstallCaseMessageReceiver::DoMessage(WM_USELOGIN_INFO, 0, 0);

	QuitMsg msg;
	msg.Send();

	return true;	
}

bool MainView::Handle( const ResponseUserLoginMsg* pMsg )
{
	if ( pMsg->bLongin )
	{
		StatusChange(true, pMsg->userName);
	}
	return true;
}

bool MainView::Handle( const SendCustomerInfoMsg* pMsg )
{
	wstring strMsg = pMsg->strSaveTime.empty() ? CommonUIFunc::LoadString(L"submitfail") : CommonUIFunc::LoadString(L"success");

	CommonUIFunc::NoticeDialog(strMsg);
	
	return true;
}

bool MainView::Handle( const GetCustomerInfoMsg* pMsg )
{
	if ( LoginModel::GetInstance()->GetBeLogin()  )
	{
		wstring strImgPath;
		bool bIsPad = false;

		UserDataDlg* pDlg = new UserDataDlg;
		pDlg->SetUserInfoModel(_pUserInfoModel);
		pDlg->ShowUserInfo(pMsg->userName, pMsg->phoneNum, pMsg->comment);

		_pUserInfoModel->GetImgPath(strImgPath, bIsPad);

		pDlg->SetDeviceImage(strImgPath, bIsPad);
		pDlg->Invalidate();	
		pDlg->DoModal();
	}
	else
	{
		LoginDlg* login = new LoginDlg;
		login->DoModal();
	}
	return true;
}

bool MainView::OnUserGotoClick( TNotifyUI* pNotify /*= NULL*/ )
{
	CStaticsticsMessage msg(Statistic_ID_Login);
	msg.Send();

	wstring url = L"http://zj.91.com/Index.aspx?userid=" + LoginModel::GetInstance()->GetUserID();

	void* ret = ShellExecute(NULL, L"open", THEIEXPLORE, url.c_str(), NULL, SW_SHOW);

	return true;
}

bool MainView::OnUserRegisterClick( TNotifyUI* pNotify /*= NULL*/ )
{
	wstring url = L"http://zj.91.com/Register.aspx";

	void* ret = ShellExecute(NULL, L"open", THEIEXPLORE, url.c_str(), NULL, SW_SHOW);
	return true;
}

void MainView::Exit()
{
	LoginModel::GetInstance()->ReSetData();
	LoginModel::GetInstance()->SetBeLogin(false);
}