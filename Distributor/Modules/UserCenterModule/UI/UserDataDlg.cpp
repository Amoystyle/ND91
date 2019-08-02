#include "stdafx.h"
#include "UserDataDlg.h"

UserDataDlg::UserDataDlg()
: CWindowUI()
{
	m_nRoundRgn = 0;

	Create(Singleton::GetMainWnd(),_T("UserDataDlg"));
	CenterWindow(Singleton::GetMainWnd());
	SetWndStyle(GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	SetEnableResize(false);

	_pUserInfoModel = NULL;
}

UserDataDlg::~UserDataDlg()
{
	SAFE_DELETE(_pUserInfoModel);
}

void UserDataDlg::OnCreate()
{
}

bool UserDataDlg::ShowUserInfo(wstring name, wstring phone, wstring comment)
{
	CEditUI* cedit = (CEditUI*)GetItem(_T("user_Data.username"));
	RETURN_FALSE_IF( !cedit );
	cedit->SetText( name.c_str() );
	CEditUI* ceditu = (CEditUI*)GetItem(_T("user_Data.tel"));
	RETURN_FALSE_IF( !ceditu );
	ceditu->SetText( phone.c_str() );
	CMultiLineEdit* remarkcedit = (CMultiLineEdit*)GetItem(_T("user_Data.remarks"));
	RETURN_FALSE_IF( !remarkcedit );
	remarkcedit->SetText( comment.c_str() );
	
	this->Invalidate();
	return true;
}

bool UserDataDlg::OnCancelBnt( TNotifyUI* pNotify )
{

	EndDialog();
	//SetFocus(MainDlg::GetInstance()->GetHWND());
	//MainDlg::GetInstance()->GetFocus();
	return true;
}

bool UserDataDlg::OnSaveBnt( TNotifyUI* pNotify )
{
	wstring name, userTel, comment;
	CEditUI* cedit = (CEditUI*)GetItem(_T("user_Data.username"));
	RETURN_FALSE_IF( !cedit );
	name = cedit->GetText();
	cedit = (CEditUI*)GetItem(_T("user_Data.tel"));
	RETURN_FALSE_IF( !cedit );
	userTel = cedit->GetText();
	CMultiLineEdit* remarkcedit = (CMultiLineEdit*)GetItem(_T("user_Data.remarks"));
	RETURN_FALSE_IF( !remarkcedit );
	comment = remarkcedit->GetText();

	if ( name.empty() || userTel.empty() )
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("mustInput")));
		return false;
	}

	if ( !userTel.empty() )
	{
		if ( !isNumString(userTel) || (userTel.length() != 11 && userTel.length() != 7 &&
			 userTel.length() != 8 && userTel.length() != 12) || _wtoi(userTel.c_str()) < 1000000 )
		{
			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("errorInput")));
			return false;
		}
	}
 	EndDialog();

	RETURN_FALSE_IF ( !_pUserInfoModel->SendUserInfo(name, userTel, comment) );

	return true;
}	

bool UserDataDlg::SetDeviceImage( wstring imgPath, bool isPad)
{
	CControlUI* pControl = dynamic_cast<CControlUI*>(GetItem(_T("user_Data.DeviceImg")));
	RETURN_FALSE_IF( !pControl );
	pControl->SetAttribute(_T("bk.image"),imgPath.c_str());
	if ( isPad )
		pControl->SetInitPos(30,20,150,200,9);
	
	return true;
}

bool UserDataDlg::isNumString( wstring str )
{
	//std::string strNum = CCodeOperation::UnicodeToGB2312(str);

	for ( int i=0; i<(int)str.length(); i++ )
	{
		RETURN_FALSE_IF ( !isdigit( str.at(i) ) );		
	}
	return true;
}

bool UserDataDlg::Handle( const SendCustomerInfoMsg* pMsg )
{
	EndDialog();
	return true;
}