#include "StdAfx.h"
#include "InstallCaseButtonItem.h"
#include "UI/MainView.h"
#include "UI/InstallCaseAppInfoListUI.h"

INSTALLCASE_BEGIN

InstallCaseButtonItem::InstallCaseButtonItem(InstallCaseAppInfoListUI* ctrlList)
{INTERFUN;
	_pCtrlList = ctrlList;

	_pLbl = NULL;
	_pIcon = NULL;
}

InstallCaseButtonItem::~InstallCaseButtonItem(void)
{INTERFUN;
}


void InstallCaseButtonItem::Create(wstring templete, wstring appPath, CLayoutUI* parent, InstallsAppInfo* appInfo)
{INTERFUN;
	__super::Create(templete.c_str(), parent);

	this->SetId(appPath.c_str());
	this->SetUserData((UINT)appInfo);
	_pLbl = dynamic_cast<CLabelUI*>(GetItem(_T("Layout.Lbl")));
	_pIcon = dynamic_cast<CControlUI*>(GetItem(_T("Layout.Icon")));
}

void InstallCaseButtonItem::SetText(wstring text)
{INTERFUN;
	if(_pLbl == NULL)
		return;

	_pLbl->SetText(text.c_str());
}

void InstallCaseButtonItem::SetIcon(wstring path)
{INTERFUN;
	if(_pIcon == NULL)
		return;

	if(CFileOperation::IsFileExist(path.c_str()))
	{
		_pIcon->SetAttribute(_T("bk.image"), path.c_str());
	}
	else
	{
		_pIcon->SetAttribute(_T("bk.image"), _T("#InstallCase_Image_Appicon_Default_32"));
	}
	_pIcon->Resize();
	_pIcon->Invalidate();
}

bool InstallCaseButtonItem::OnDeleteBtnClick(TNotifyUI* pNotify)
{INTERFUN;
	_pCtrlList->RemoveButtonItem(this);
	return true;
}

INSTALLCASE_END