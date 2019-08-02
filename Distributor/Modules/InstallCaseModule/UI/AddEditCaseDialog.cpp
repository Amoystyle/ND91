#include "stdafx.h"
#include "MainView.h"
#include "InstallCaseAppInfoListUI.h"
#include "AddEditCaseDialog.h"
INSTALLCASE_BEGIN

AddEditCaseDialog::AddEditCaseDialog(InstallCaseUI* owner, InstallsCaseData* editCaseData)
{INTERFUN;
	_pOwner = owner;
	_pIOSAppList = NULL;
	_pAndroidAppList = NULL;
	_pEditCaseData = editCaseData;

	m_nRoundRgn = 0;
	_bCanLoad = false;

	_mDefaultType = _INSTALLS_CASE_DATA_IOS;
}

AddEditCaseDialog::~AddEditCaseDialog()
{INTERFUN;
}

void AddEditCaseDialog::CreateView(InstallsCaseData* caseData)
{INTERFUN;
	if (NULL==caseData)
	{
		return;
	}
	if(caseData->PlanType() == _INSTALL_PLAN_TYPE_SYS)
	{
		if(caseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
		{
			if(_pIOSAppList == NULL)
			{
				_pIOSAppList = new InstallCaseAppInfoListUI(this->GetHWND(), _pOwner, caseData);
				_pIOSAppList->SetSelectListVisible(true);
				_pIOSAppList->SetAppSelectObserve(this);
			}
		}
		else
		{
			if(_pAndroidAppList == NULL)
			{
				_pAndroidAppList = new InstallCaseAppInfoListUI(this->GetHWND(), _pOwner, caseData);
				_pAndroidAppList->SetSelectListVisible(true);
				_pAndroidAppList->SetAppSelectObserve(this);
			}
		}
	}
}

void AddEditCaseDialog::AddAppInfoToView( int caseId, InstallsAppInfo* appInfo)
{INTERFUN;
	if(!_bCanLoad)
		return;

	if(_pIOSAppList->GetCaseData()->Id() == caseId)
	{
		_pIOSAppList->AddAppInfoToView(caseId, appInfo);
	}
	else if(_pAndroidAppList->GetCaseData()->Id() == caseId)
	{
		_pAndroidAppList->AddAppInfoToView(caseId, appInfo);
	}
}

void AddEditCaseDialog::RemoveAppInfoFromView( int caseId, wstring appPath )
{INTERFUN;
	if(!_bCanLoad)
		return;

	if(_pIOSAppList->GetCaseData()->Id() == caseId)
	{
		_pIOSAppList->RemoveAppInfoFromView(appPath);
	}
	else if(_pAndroidAppList->GetCaseData()->Id() == caseId)
	{
		_pAndroidAppList->RemoveAppInfoFromView(appPath);
	}
}

void AddEditCaseDialog::UpdateCaseView(int caseId)
{INTERFUN;
	if(!_bCanLoad)
		return;

	if(caseId == _pIOSAppList->GetCaseData()->Id())
		SetTabName(_pIosAppRadio, CommonUIFunc::LoadStringW(_T("InstallCase_Text_SoftWareiOS")), _pIOSAppList->GetCaseData());
	else if(caseId == _pAndroidAppList->GetCaseData()->Id())
		SetTabName(_pAndroidAppRadio, CommonUIFunc::LoadStringW(_T("InstallCase_Text_SoftWareAndroid")), _pAndroidAppList->GetCaseData());
}

void AddEditCaseDialog::ClearAppFromView(int caseId, AppSearchContextEx* context)
{INTERFUN;
	if(!_bCanLoad)
		return;

	if(_pIOSAppList->GetCaseData()->Id() == caseId)
	{
		_pIOSAppList->ClearAppFromView(context);
	}
	else if(_pAndroidAppList->GetCaseData()->Id() == caseId)
	{
		_pAndroidAppList->ClearAppFromView(context);
	}
}

void AddEditCaseDialog::SetDefaultCaseName(wstring caseName)
{INTERFUN;
	_mDefaultCaseName = caseName;
	if(_pEdit != NULL)
		_pEdit->SetText(this->_mDefaultCaseName.c_str());
}

void AddEditCaseDialog::SetDefaultCaseType( _INSTALLS_CASE_DATA_TYPE caseType )
{INTERFUN;
	_mDefaultType = caseType;
	if(_pAndroidAppRadio != NULL)
	{
		if(_mDefaultType == _INSTALLS_CASE_DATA_ANDROID)
		{
			_pAndroidAppRadio->SetChecked(true);
			this->OnSwitchAndroid(NULL);
		}
	}
}

void AddEditCaseDialog::GetCaseInfo(wstring& caseName, _INSTALLS_CASE_DATA_TYPE& caseType, vector<wstring>* pathList)
{INTERFUN;
	wstring strSpace = L" ";
	caseName = CStrOperation::trimLeftW(_pEdit->GetText(), strSpace);
	if(_pIOSAppList->IsVisible())
	{
		caseType = _INSTALLS_CASE_DATA_IOS;
		_pIOSAppList->GetSelectPathList(*pathList);
	}
	else
	{
		caseType = _INSTALLS_CASE_DATA_ANDROID;
		_pAndroidAppList->GetSelectPathList(*pathList);
	}
}

void AddEditCaseDialog::OnCreate()
{INTERFUN;
	SetWndStyle(GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	_pContainer = dynamic_cast<CContainerUI*>(GetItem(_T("Middle.Container")));

	_pIOSAppList->Create(L"InstallCaseAppList", _pContainer);
	_pAndroidAppList->Create(L"InstallCaseAppList", _pContainer);

	_pIOSAppList->HideOperateColumn();
	_pAndroidAppList->HideOperateColumn();
	
	_pEdit = dynamic_cast<CEditUI*>(GetItem(_T("Top.Edit")));
	_pTabBar = dynamic_cast<CLayoutUI*>(GetItem(_T("Middle.TabBar")));
	_pIosAppRadio= dynamic_cast<CRadioUI*>(GetItem(_T("Middle.TabBar.IosAppList")));
	_pAndroidAppRadio= dynamic_cast<CRadioUI*>(GetItem(_T("Middle.TabBar.AndroidAppList")));

	_pLastVerChk = dynamic_cast<CCheckBoxUI*>(GetItem(_T("Bottom.lastVerChk")));
	_pCaseInfo = dynamic_cast<CLabelUI*>(GetItem(_T("Bottom.CaseInfo")));

	InstallCaseAppInfoListUI* pList = NULL;
	if(_pEditCaseData != NULL)
	{
		_pTabBar->SetVisible(false);
		if(_pEditCaseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
		{
			_pAndroidAppList->SetVisible(false);
			_pIOSAppList->SetVisible(true);
			pList = _pIOSAppList;
		}
		else
		{
			_pIOSAppList->SetVisible(false);
			_pAndroidAppList->SetVisible(true);
			pList = _pAndroidAppList;
		}
		_pEdit->SetText(_pEditCaseData->CaseName().c_str());
	}
	else
	{
		_pAndroidAppList->SetVisible(false);
		_pIOSAppList->SetVisible(true);
		_pEdit->SetText(this->_mDefaultCaseName.c_str());
		SetTabName(_pIosAppRadio, CommonUIFunc::LoadStringW(_T("InstallCase_Text_SoftWareiOS")), _pIOSAppList->GetCaseData());
		SetTabName(_pAndroidAppRadio, CommonUIFunc::LoadStringW(_T("InstallCase_Text_SoftWareAndroid")), _pAndroidAppList->GetCaseData());
	}

	if(pList != NULL)
	{
		for (InstallsAppList::iterator it = _pEditCaseData->ListApp()->begin(); it != _pEditCaseData->ListApp()->end(); it++)
		{
			InstallsAppInfo* appInfo = *it;
			pList->AddButtonItem(appInfo);
		}
	}

	if(_pEditCaseData != NULL)
	{
		_bCanLoad = true;
		if(_pEditCaseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
		{
			Search(_pIOSAppList);
		}
		else
		{
			Search(_pAndroidAppList);
		}
	}
	else
	{
		if(_mDefaultType == _INSTALLS_CASE_DATA_ANDROID)
		{
			_pAndroidAppRadio->SetChecked(true);
			this->OnSwitchAndroid(NULL);
		}
		_bCanLoad = true;

		Search(_pIOSAppList);
		Search(_pAndroidAppList);
	}
	Selected();
}

void AddEditCaseDialog::SetTabName(CRadioUI* tabCtrl, wstring text, InstallsCaseData* caseData)
{INTERFUN;
	wstring tabName = text;
	if(caseData->GetAppNum() > 0)
	{
		tabName += L"(";
		tabName += CStrOperation::IntToWString(caseData->GetAppNum());
		tabName += L")";
	}
	tabCtrl->SetText(tabName.c_str());
	tabCtrl->Resize();
	tabCtrl->Invalidate();
}

bool AddEditCaseDialog::OnBtnShowLaseVerOnly(TNotifyUI* pNotify)
{INTERFUN;
	if(_pIOSAppList->IsVisible())
	{
		_pIOSAppList->SetLastVerChecked(_pLastVerChk->IsChecked());
	}
	else
	{
		_pAndroidAppList->SetLastVerChecked(_pLastVerChk->IsChecked());
	}
	return true;
}

void AddEditCaseDialog::Search(InstallCaseAppInfoListUI* listUI)
{INTERFUN;
	listUI->SetLastVerChecked(_pLastVerChk->IsChecked());
	listUI->Search();
}

bool AddEditCaseDialog::OnSwitchIOS(TNotifyUI* pNotify)
{INTERFUN;
	_pEdit->SetText(_pOwner->Model.GetNewDefaultCaseName(_pIOSAppList->GetCaseData()->CaseType()).c_str());

	_pAndroidAppList->SetVisible(false);
	_pIOSAppList->SetVisible(true);

	this->Selected();

	_pLastVerChk->SetChecked(_pIOSAppList->IsLastVerChecked());

	_pContainer->Resize();
	_pContainer->Invalidate();

	return true;
}

bool AddEditCaseDialog::OnSwitchAndroid(TNotifyUI* pNotify)
{INTERFUN;
	_pEdit->SetText(_pOwner->Model.GetNewDefaultCaseName(_pAndroidAppList->GetCaseData()->CaseType()).c_str());

	_pIOSAppList->SetVisible(false);
	_pAndroidAppList->SetVisible(true);

	this->Selected();

	_pLastVerChk->SetChecked(_pAndroidAppList->IsLastVerChecked());

	_pContainer->Resize();
	_pContainer->Invalidate();

	return true;
}

bool AddEditCaseDialog::OnBtnOk( TNotifyUI* pNotify )
{INTERFUN;
	wstring strSpace = L" ";
	wstring caseName = _pEdit->GetText();
	if(CStrOperation::trimLeftW(caseName, strSpace).empty())
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_InputCaseName")), ICON_WARNING);
	}
	else
	{
		EndDialog(1);
	}
	return true;
}

bool AddEditCaseDialog::OnBtnCancel( TNotifyUI* pNotify )
{INTERFUN;
	EndDialog(0);
	return true;
}

void AddEditCaseDialog::Selected()
{INTERFUN;
	vector<wstring> pathList;

	InstallsCaseData* caseData = NULL;
	if(_pIOSAppList->IsVisible())
	{
		_pIOSAppList->GetSelectPathList(pathList);
		caseData = _pIOSAppList->GetCaseData();
	}
	else
	{
		_pAndroidAppList->GetSelectPathList(pathList);
		caseData = _pAndroidAppList->GetCaseData();
	}

	UINT totalSize = 0;
	for (vector<wstring>::iterator it = pathList.begin(); it != pathList.end(); it++)
	{
		wstring path = *it;
		InstallsAppInfo* appInfo = caseData->GetAppBySavePath(path);
		if(appInfo == NULL && _pEditCaseData != NULL)
		{
			appInfo = _pEditCaseData->GetAppBySavePath(path);
		}
		if(appInfo != NULL)
			totalSize += appInfo->m_FileSize;
	}

	if(pathList.size() > 0 )
	{
		wstring caseInfo = L"已选中";
		caseInfo += CStrOperation::IntToWString(pathList.size());
		caseInfo += L"款应用， 共";
		caseInfo += CStrOperation::FormatFileSize(totalSize,1);
		caseInfo += L"。";
		_pCaseInfo->SetText(caseInfo.c_str());
	}
	else
	{
		_pCaseInfo->SetText(L"");
	}
}

INSTALLCASE_END