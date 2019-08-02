#include "stdafx.h"
#include "DownLoadSystemSetUI.h"
#include "SystemSetDlg.h"

DownLoadSystemSetUI::DownLoadSystemSetUI()
: _model(this)
{INTERFUN;

}


DownLoadSystemSetUI::~DownLoadSystemSetUI()
{INTERFUN;

}

void DownLoadSystemSetUI::OpenFolder(wstring fonlder_dir)
{INTERFUN;
	ShellExecuteW(NULL,L"open",fonlder_dir.c_str(),NULL,NULL,SW_SHOWNORMAL);
}

bool DownLoadSystemSetUI::OnIosSelectBtn( TNotifyUI* pNotify )
{INTERFUN;
	wstring folder = GetIosDir();
	if (::GetFileAttributes(folder.c_str()) == -1)
	{
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(_T("DependenceFuncCenter_FM_NotFound")),CommonUIFunc::LoadString(_T("DependenceFuncCenter_Seting_text")));
		return true;
	}
	OpenFolder(folder);
	return true;
}

bool DownLoadSystemSetUI::OnIosSettingBtn( TNotifyUI* pNotify )
{INTERFUN;
	wstring dir = BaseOperation::DirDialog(Singleton::GetMainWnd());
	if (dir.length() == 0)
	{
		//MessageDialog(L"设置的路径无效，请重新设置！", L"错误");
		return true;
	}
	if (dir.length() > 0 && dir[dir.length()-1] != '\\')
		dir += L"\\";


	SetIosDir(dir);
	((SystemSetDlg*)_dlgpoint)->ActivationApplyBtn(true);
	return true;
}

bool DownLoadSystemSetUI::OnAndroidSelectBtn( TNotifyUI* pNotify )
{INTERFUN;
	wstring folder = GetAndroidDir();
	if (::GetFileAttributes(folder.c_str()) == -1)
	{
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(_T("DependenceFuncCenter_FM_NotFound")),CommonUIFunc::LoadString(_T("DependenceFuncCenter_Seting_text")));
		return true;
	}
	OpenFolder(folder);

	return true;
}

bool DownLoadSystemSetUI::OnAndroidSettingBtn( TNotifyUI* pNotify )
{INTERFUN;
	wstring dir = BaseOperation::DirDialog(Singleton::GetMainWnd());
	if (dir.length() == 0)
	{
		return true;
	}
	if (dir.length() > 0 && dir[dir.length()-1] != '\\')
		dir += L"\\";


	SetAndroidDir(dir);
	((SystemSetDlg*)_dlgpoint)->ActivationApplyBtn(true);
	return true;
}

bool DownLoadSystemSetUI::OnAutoDelUpdatedApp( TNotifyUI* pNotify )
{
	((SystemSetDlg*)_dlgpoint)->ActivationApplyBtn(true);
	return true;
}

std::wstring DownLoadSystemSetUI::GetIosDir()
{INTERFUN;
	CEditUI* pTab = (CEditUI*)GetItem(IosDir);
	if (pTab)
		return pTab->GetText();

	return _T("");
}

void DownLoadSystemSetUI::SetIosDir( wstring dir )
{INTERFUN;
	CEditUI* pTab = (CEditUI*)GetItem(IosDir);
	if (pTab)
		pTab->SetText(dir.c_str());
}

std::wstring DownLoadSystemSetUI::GetAndroidDir()
{INTERFUN;
	CEditUI* pTab = (CEditUI*)GetItem(AndroidDir);
	if (pTab)
		return pTab->GetText();

	return _T("");
}

void DownLoadSystemSetUI::SetAndroidDir( wstring dir )
{INTERFUN;
	CEditUI* pTab = (CEditUI*)GetItem(AndroidDir);
	if (pTab)
		pTab->SetText(dir.c_str());
}

bool DownLoadSystemSetUI::GetCheck( wstring name )
{INTERFUN;
	CCheckBoxUI* pCheckBox = (CCheckBoxUI*)GetItem(name.c_str());
	if (pCheckBox)
		return pCheckBox->IsChecked();
	return false;
}

void DownLoadSystemSetUI::SetCheck( wstring name,bool flag )
{INTERFUN;
	CCheckBoxUI* pCheckBox = (CCheckBoxUI*)GetItem(name.c_str());
	if (pCheckBox)
	{
		pCheckBox->SetChecked(flag);
	}
}

void DownLoadSystemSetUI::OnCreate()
{INTERFUN;
	_model.Init();
}

void DownLoadSystemSetUI::Save_Data()
{INTERFUN;
	wstring dirAnd = L"";
	CEditUI* pTab = (CEditUI*)GetItem(AndroidDir);
	if (pTab)
		dirAnd = pTab->GetText();

	wstring diriOS = L"";
	CEditUI* pTab1 = (CEditUI*)GetItem(IosDir);
	if (pTab1)
		diriOS = pTab1->GetText();
	if(diriOS != CONFIG(_strIosDownPath) || dirAnd != CONFIG(_strAndroidDownPath))
	{
		_model.SaveDownLoadPathSetting();

		AddDownLoadFolderMsg AddFolderMsg;
		AddFolderMsg.androidPath = dirAnd;
		AddFolderMsg.iosPath     = diriOS;
		AddFolderMsg.fromModular = _T("DownLoadSystemSetUI");
		AddFolderMsg.Send();

	}
	_model.SaveSetting();
}

void DownLoadSystemSetUI::SetPoint( void *p )
{INTERFUN;
	_dlgpoint = p;
}