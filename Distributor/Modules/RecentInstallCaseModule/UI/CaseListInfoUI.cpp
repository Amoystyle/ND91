#include "stdafx.h"
#include "CaseListInfoUI.h"

bool __stdcall InstallAppListCompare(CListItemUI* pControlA, CListItemUI* pControlB, int nCol, bool exchange)
{INTERFUN;
	RETURN_FALSE_IF(!pControlA || !pControlB);

	switch (nCol)
	{
		case 0://名称
			return PublicFunc::CaseSortRult(pControlA, pControlB, L"LabContainer.LabelAppName");
			break;
		case 1:// 
			return PublicFunc::CaseSortRult(pControlA, pControlB, L"VersionContainer.LabelAppVer");
			break;
		case 2://
			return PublicFunc::CaseSortByData(pControlA, pControlB, L"LabelAppSize");
			break;
		case 3:// 
			return PublicFunc::CaseSortRult(pControlA, pControlB, L"SDKVerContainer.LabelAppType");
			break;
		default:
			break;
	}
	return !exchange;
}


CaseListInfoUI::CaseListInfoUI(int caseDataid, wstring devId, CButtonUI* ownerBtn,NetInstallPlanTaskManager* pNetCaseManager)
{INTERFUN;
	_pAppinfoListControl	= NULL;
	_LabInfo				= NULL;
	_pTab					= NULL;
	_appListCtrl			= NULL;
	_pCaseData				= NULL;
	_CaseDataid				= caseDataid;
	m_nRoundRgn				= 0;
	_mCurrentDevId			= devId;
	_pOwnerButton			= ownerBtn;
	_pNetCaseManager		= pNetCaseManager;
	_eAppUpgradeType = _APP_UPGRADE_TYPE_TOSAMELEGAL;
	
}
CaseListInfoUI::~CaseListInfoUI()
{INTERFUN;
	SAFE_DELETE(_pAppinfoListControl);
}

void CaseListInfoUI::OnCreate()
{INTERFUN;
	SetWndStyle(GWL_EXSTYLE, WS_EX_TOOLWINDOW);

	int num = 0;
	_appListCtrl = (CListUI*)GetItem(_T("AppList"));
	_appListCtrl->SetSortCallBack(InstallAppListCompare);
	_LabInfo=(CLabelUI*)GetItem(_T("CaseInfo"));
	_pInstallBtn = (CButtonUI*)GetItem(_T("Bottom.InstallBtn"));
	if(_pOwnerButton&&_pInstallBtn)
	{
		_pInstallBtn->SetText(_pOwnerButton->GetText());
		_pInstallBtn->SetEnabled(_pOwnerButton->IsEnabled());
	}
	_pAppinfoListControl = new LocalAppinfoList(_appListCtrl,NULL);

	_pAndUpgradeAllBtn	= (CButtonUI*)GetItem(_T("Bottom.AndUpgradeAllAppsBtn"));
	_pUpgradeAllBtn		= (CButtonUI*)GetItem(_T("Bottom.Upgrade.UpgradeAllAppsBtn"));
	_pUpgrateAllMenuBtn	= (CButtonUI*)GetItem(_T("Bottom.Upgrade.UpgradeAllAppsMenuBtn"));
	_pUpgradeAllLayout	= (CLayoutUI*)GetItem(_T("Bottom.Upgrade"));


	GetCaseByIdMsg GetCaseMsg;
	GetCaseMsg.CaseDataid = _CaseDataid;
	GetCaseMsg.SendAndGetReturn();

	_pCaseData = (InstallsCaseData*)GetCaseMsg.pDataCase;
	wstring info = L"";
	wstring Allinfo = L"";
	wstring Tempinfo = L"";

	this->SetAttribute(_T("title"), _pCaseData->CaseName().c_str());
	this->SetTitle(_pCaseData->CaseName().c_str());
	if(_pCaseData->CaseName().length() >=16)
	{
		info = _pCaseData->CaseName().substr(0, 16) + L"...";
	}
	else
	{
		info = _pCaseData->CaseName();
	}
	Allinfo = _pCaseData->CaseName();
	Tempinfo= CommonUIFunc::LoadString(L"RecentInstallCase_Text_CaseSum");
	Tempinfo+= CStrOperation::FormatFileSize((int64_t)_pCaseData->GetSumFileSize());
	Tempinfo+=CommonUIFunc::LoadString(L"RecentInstallCase_Text_CaseNeedTime");
	Tempinfo+=UIStringFormat::InstallNeedTime(_pCaseData->GetAppNum(),_pCaseData->GetSumFileSize(), num);
	info+=Tempinfo;
	Allinfo+=Tempinfo;

	_LabInfo->SetText(info.c_str());

	_LabInfo->SetToolTipShow(true);
	_LabInfo->SetToolTip(Allinfo.c_str());

	RefreshUpgradeCount();

	_pTab = (CTabUI*)GetItem(_T("TypeTab"));
	if ( _pCaseData->CaseType() == _INSTALLS_CASE_DATA_IOS )
	{
		_pTab->GetHeader()->SetAttribute(_T("bk.visible"), _T("0"));
		_pTab->SelectItem(0);
	}
	else
	{
		_pTab->SetVisible(false);
		_appListCtrl->SetInitPos(1,29,1,40,8);
	}

	_pUpgradeAllLayout->SetVisible((_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_USER || _pCaseData->PlanType() == _INSTALL_PLAN_TYPE_SYS) && _pCaseData->CaseType() == _INSTALLS_CASE_DATA_IOS);
	_pAndUpgradeAllBtn->SetVisible(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_USER && _pCaseData->CaseType() == _INSTALLS_CASE_DATA_ANDROID);
	_pUpgrateAllMenuBtn->SetEnabled(_pCaseData->CaseType() == _INSTALLS_CASE_DATA_IOS);

	LoadAppListMsg LoadMsg;
	LoadMsg.caseId = _CaseDataid;
	LoadMsg.Send();

}

void CaseListInfoUI::InsertAppInfo(InstallsAppInfo* appInfo, bool isUpdate) 
{INTERFUN;
	if(_pAppinfoListControl)
		_pAppinfoListControl->AddAppItem(appInfo,isUpdate,true,false);
}

//更新app列表UI
void CaseListInfoUI::RefreshAppInfoList()
{INTERFUN;
	_appListCtrl->Resize();
	_appListCtrl->Invalidate();
}

bool CaseListInfoUI::Handle(const DeviceLinkOutMsg* pMsg)
{
	if (pMsg->id != _mCurrentDevId)
	{
		return true;
	}

	EndDialog(IDCLOSE);
	return true;
}

bool CaseListInfoUI::Handle(const InstallCaseMsg* pMsg)
{INTERFUN;
	if (NULL == pMsg)
	{
		return false;
	}

	switch (pMsg->message)
	{
	case WM_CASE_APP_ADD:
		{
			int caseId = (int)pMsg->wParam;
			if(_CaseDataid == caseId)
			{
				InstallsAppInfo* appInfo =(InstallsAppInfo*)pMsg->lParam;
				InsertAppInfo(appInfo, false);
				_appListCtrl->CheckItem(true,-1);
			}
		}
		break;
	default:
		break;
	}

	return true;
}

void CaseListInfoUI::GetIdList( vector<wstring>& pathList )
{INTERFUN;
	for (int i=0;i<_appListCtrl->GetCount();i++)
	{
		CControlUI* item = _appListCtrl->GetItem(i);
		if (item->IsSelect())
		{
			wstring nPath = item->GetId();

			pathList.push_back(nPath);
		}
		
	}
}

bool CaseListInfoUI::OnTypeTabChange( TNotifyUI* pNotify )
{
	RETURN_TRUE_IF ( !_pTab );

	ClearUIData();

	if ( 0 == _pTab->GetCurSel() )
	{
		LoadAppListMsg LoadMsg;
		LoadMsg.caseId = _CaseDataid;
		LoadMsg.Send();

	}
	else if ( 1 == _pTab->GetCurSel() )
	{
		LoadAppListMsg LoadMsg;
		LoadMsg.caseId = _CaseDataid;
		LoadMsg.type = SUPPORTDEVICE_IPHONE;
		LoadMsg.Send();
	}
	else
	{
		LoadAppListMsg LoadMsg;
		LoadMsg.caseId = _CaseDataid;
		LoadMsg.type = SUPPORTDEVICE_IPAD;
		LoadMsg.Send();
	}
	return true;
}

bool CaseListInfoUI::OnUpgradeAllAppsMenuBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	CMenuUI* pMenu = CMenuUI::Load(IDR_MENU_UPGRADEALL);
	CControlUI* con0 = pMenu->GetItem(0);
	CControlUI* con1 = pMenu->GetItem(1);
	CControlUI* con2 = pMenu->GetItem(2);
	pMenu->GetItem(0)->Check(_eAppUpgradeType==_APP_UPGRADE_TYPE_TOPIRACY);
	pMenu->GetItem(1)->Check(_eAppUpgradeType==_APP_UPGRADE_TYPE_TOLEGAL);
	pMenu->GetItem(2)->Check(_eAppUpgradeType==_APP_UPGRADE_TYPE_TOSAMELEGAL);
	CommonUIFunc::ShowMenu(pMenu, this->GetHWND(), this, _pUpgradeAllBtn);
	return true;
}

bool CaseListInfoUI::OnClickUpgradeMenu( TNotifyUI* pNotify )
{INTERFUN;
	if (pNotify->wParam == NULL)
		return true;
	UIMENUINFO* pInfo = (UIMENUINFO*)pNotify->wParam;

	switch (pInfo->uID)
	{
	case IDR_MENU_UPGRADEALLTOPIRACY:
		{
			if(_eAppUpgradeType!=_APP_UPGRADE_TYPE_TOPIRACY)
			{
				_eAppUpgradeType = _APP_UPGRADE_TYPE_TOPIRACY;
				RefreshUpgradeCount();
			}
		}
		break;
	case IDR_MENU_UPGRADEALLTOLEGAL:
		{
			if(_eAppUpgradeType!=_APP_UPGRADE_TYPE_TOLEGAL)
			{
				_eAppUpgradeType = _APP_UPGRADE_TYPE_TOLEGAL;
				RefreshUpgradeCount();
			}
		}
		break;
	case IDR_MENU_UPGRADEALLTOSAMELEGAL:
		{
			if(_eAppUpgradeType!=_APP_UPGRADE_TYPE_TOSAMELEGAL)
			{
				_eAppUpgradeType = _APP_UPGRADE_TYPE_TOSAMELEGAL;
				RefreshUpgradeCount();
			}
		}
		break;
	}
	return true;
}

void CaseListInfoUI::RefreshUpgradeCount()
{
	int uppiracynum = 0;
	int uplegalnum = 0;
	int upsamelegalnum = 0;
	int shownum = 0;
	InstallsAppList* applist= _pCaseData->ListApp();
	for(InstallsAppList::iterator it=applist->begin();it!=applist->end();it++)
	{
		InstallsAppInfo* info = *it;
		if(info->m_bUpdateable == INSTALLS_CASE_UPDATEABLE_YES)
			uppiracynum++;
		if(info->m_bLegalUpdateable == INSTALLS_CASE_LEGALUPDATEABLE_YES)
			uplegalnum++;
		if(!info->m_isLegalSoft && info->m_bUpdateable == INSTALLS_CASE_UPDATEABLE_YES
			|| info->m_isLegalSoft && info->m_bLegalUpdateable == INSTALLS_CASE_LEGALUPDATEABLE_YES )
			upsamelegalnum++;
	}
	switch (_eAppUpgradeType)
	{
	case _APP_UPGRADE_TYPE_TOPIRACY:
		shownum = uppiracynum;
		break;
	case _APP_UPGRADE_TYPE_TOLEGAL:
		shownum = uplegalnum;
		break;
	case _APP_UPGRADE_TYPE_TOSAMELEGAL:
		shownum = upsamelegalnum;
		break;
	}

	CButtonUI* pBtn = _pCaseData->CaseType() ==_INSTALLS_CASE_DATA_IOS? _pUpgradeAllBtn:_pAndUpgradeAllBtn;
	if(_pCaseData->CaseType() ==_INSTALLS_CASE_DATA_IOS)
	{
		if(shownum==0)
		{
			_pUpgradeAllBtn->SetText(CommonUIFunc::LoadStringW(L"RecentInstallCase_Text_UpgradeAllAppsNonum"));
			_pUpgradeAllBtn->SetEnabled(false);
			_pUpgradeAllBtn->Resize();
			_pUpgradeAllBtn->Invalidate();
		}
		else
		{
			_pUpgradeAllBtn->SetText(CStrOperation::ReplaceW(CommonUIFunc::LoadStringW(L"RecentInstallCase_Text_UpgradeAllApps"), L"num", CStrOperation::IntToWString(shownum)).c_str());
			_pUpgradeAllBtn->SetEnabled(true);
			_pUpgradeAllBtn->Resize();
			_pUpgradeAllBtn->Invalidate();
		}
	}
	else
	{
		if(shownum==0)
		{
			_pAndUpgradeAllBtn->SetText(CommonUIFunc::LoadStringW(L"RecentInstallCase_Text_UpgradeAllAppsNonum"));
			_pAndUpgradeAllBtn->SetEnabled(false);
			_pAndUpgradeAllBtn->GetParent()->Resize();
			_pAndUpgradeAllBtn->GetParent()->Invalidate();
		}
		else
		{
			_pAndUpgradeAllBtn->SetText(CStrOperation::ReplaceW(CommonUIFunc::LoadStringW(L"RecentInstallCase_Text_UpgradeAllApps"), L"num", CStrOperation::IntToWString(shownum)).c_str());
			_pAndUpgradeAllBtn->SetEnabled(true);
			_pAndUpgradeAllBtn->GetParent()->Resize();
			_pAndUpgradeAllBtn->GetParent()->Invalidate();
		}
	}
}

bool CaseListInfoUI::OnInstallBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	
	vector<wstring> pathList;
	this->GetIdList(pathList);
	if(pathList.empty())
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("RecentInstallCase_Text_SelectSoftWarePls")), ICON_WARNING);
		return true;
	}

	if(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_NET)
	{
		if(_pNetCaseManager)
		{
			_pNetCaseManager->btnInstall = _pOwnerButton;
			for (int i=0;i<_appListCtrl->GetCount();i++)
			{
				CControlUI* item = _appListCtrl->GetItem(i);
				if (item->IsSelect())
				{
					bool isSelect = false;

					for (InstallsAppList::iterator it = _pNetCaseManager->mNetInstallPlanItem->ListApp()->begin(); it != _pNetCaseManager->mNetInstallPlanItem->ListApp()->end(); it++)
					{
						InstallsAppInfo* appInfo = *it;
						if (appInfo->m_strSavePath == item->GetId())
						{
							appInfo->m_bSelected=true;
							isSelect = true;
							break;
						}
					}
					if (!isSelect)
					{
						for (InstallsAppList::iterator it = _pNetCaseManager->mNetInstallPlanItem->NetAppItems->begin(); it != _pNetCaseManager->mNetInstallPlanItem->NetAppItems->end(); it++)
						{
							InstallsAppInfo* appInfo = *it;
							if (appInfo->m_strSavePath == item->GetId())
							{
								appInfo->m_bSelected=true;
								isSelect = true;
								break;
							}
						}
					}
				}
			}
			_pNetCaseManager->Run();
			InstallsAppList *listApp = _pNetCaseManager->mNetInstallPlanItem->ListApp();
			InstallsAppList *netListApp = _pNetCaseManager->mNetInstallPlanItem->NetAppItems;
			for (InstallsAppList::iterator it = _pNetCaseManager->mNetInstallPlanItem->ListApp()->begin(); it != _pNetCaseManager->mNetInstallPlanItem->ListApp()->end(); it++)
			{
				InstallsAppInfo* appInfo = *it;
				appInfo->m_bSelected = false;
			}
	
			for (InstallsAppList::iterator it = _pNetCaseManager->mNetInstallPlanItem->NetAppItems->begin(); it != _pNetCaseManager->mNetInstallPlanItem->NetAppItems->end(); it++)
			{
				InstallsAppInfo* appInfo = *it;
				appInfo->m_bSelected = false;
			}

		}
	}
	else
	{
		if(pathList.size()>0)
		{
			bool bRemoveFile = false;
			if(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_SYS)
				bRemoveFile = true;

			if(DEVICE_MANAGER->FindDevice(_mCurrentDevId))
			{

				SetCaseSizeMsg CaseSizeMsg;
				CaseSizeMsg.casename = CStrOperation::IntToWString(_CaseDataid);
				CaseSizeMsg.deviceId = _mCurrentDevId;
				CaseSizeMsg.count    = pathList.size();
				CaseSizeMsg.Send();

				InstallAppByCaseIdMsg InstallMsg;
				InstallMsg.sn = _mCurrentDevId;
				InstallMsg.caseId = _pCaseData->Id();
				InstallMsg.pathList = &pathList;
				InstallMsg.Send();

			}

			EndDialog(100+pathList.size());
			return true;
		}
	}

	EndDialog(IDOK);
	return true;
}

bool CaseListInfoUI::OnCloseBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	EndDialog(IDCLOSE);
	return true;
}

bool CaseListInfoUI::OnUpdateBtnClick( TNotifyUI* pNotify )
{
	if(pNotify->pSender->GetParent()->GetParent() == NULL)
		return true;
	CSimpleListItemUI* item = (CSimpleListItemUI*)pNotify->pSender->GetParent()->GetParent();
	InstallsAppInfo* info = _pCaseData->GetAppBySavePath(item->GetId());
	if(info)
	{
		vector<wstring> pathList;
		pathList.push_back(info->m_strSavePath);
		UpgrateAppMsg UpgrateMsg;
		UpgrateMsg.caseId = _pCaseData->Id();
		UpgrateMsg.pPathList = &pathList;
		UpgrateMsg.Send();

		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("RecentInstallCase_Text_Task_Added_to_list")), ICON_SUCCESS);
		pNotify->pSender->SetEnabled(false);
	}
	return true;
}

bool CaseListInfoUI::OnLegalUpdateBtnClick( TNotifyUI* pNotify )
{
	if(pNotify->pSender->GetParent()->GetParent() == NULL)
		return true;
	CSimpleListItemUI* item = (CSimpleListItemUI*)pNotify->pSender->GetParent()->GetParent();
	InstallsAppInfo* info = _pCaseData->GetAppBySavePath(item->GetId());
	if(info)
	{
		vector<wstring> pathList;
		pathList.push_back(info->m_strSavePath);
		UpgradeAppToLegalMsg UpgradeLegalMsg;
		UpgradeLegalMsg.caseId = _pCaseData->Id();
		UpgradeLegalMsg.pPathList = &pathList;
		UpgradeLegalMsg.Send();

		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("RecentInstallCase_Text_Task_Added_to_list")), ICON_SUCCESS);
		pNotify->pSender->SetEnabled(false);
	}
	return true;
}

bool CaseListInfoUI::OnUpdateAllAppsBtnClick( TNotifyUI* pNotify )
{INTERFUN;

	InstallsAppList* applist= _pCaseData->ListApp();
	if(_APP_UPGRADE_TYPE_TOPIRACY==_eAppUpgradeType)
	{
		vector<wstring> pathList;
		for(InstallsAppList::iterator it=applist->begin();it!=applist->end();it++)
		{
			InstallsAppInfo* info = *it;
			if(info->m_bUpdateable == INSTALLS_CASE_UPDATEABLE_YES)
				pathList.push_back(info->m_strSavePath);
		}
		if(pathList.size() > 0)
		{
			UpgrateAppMsg UpgrateMsg;
			UpgrateMsg.caseId = _pCaseData->Id();
			UpgrateMsg.pPathList = &pathList;
			UpgrateMsg.Send();

			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("RecentInstallCase_Text_Task_Added_to_list")), ICON_SUCCESS);
		}
	}
	else if(_APP_UPGRADE_TYPE_TOLEGAL==_eAppUpgradeType)
	{
		vector<wstring> pathLegalList;
		for(InstallsAppList::iterator it=applist->begin();it!=applist->end();it++)
		{
			InstallsAppInfo* info = *it;
			if(info->m_bLegalUpdateable == INSTALLS_CASE_LEGALUPDATEABLE_YES)
				pathLegalList.push_back(info->m_strSavePath);
		}
		if(pathLegalList.size() > 0)
		{
			UpgradeAppToLegalMsg UpgradeLegalMsg;
			UpgradeLegalMsg.caseId = _pCaseData->Id();
			UpgradeLegalMsg.pPathList = &pathLegalList;
			UpgradeLegalMsg.Send();

			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("RecentInstallCase_Text_Task_Added_to_list")),ICON_SUCCESS);
		}

	}
	else if(_APP_UPGRADE_TYPE_TOSAMELEGAL==_eAppUpgradeType)
	{
		vector<wstring> pathLegalList;
		vector<wstring> pathPiracyList;
		for(InstallsAppList::iterator it=applist->begin();it!=applist->end();it++)
		{
			InstallsAppInfo* info = *it;
			if(info->m_bLegalUpdateable == INSTALLS_CASE_LEGALUPDATEABLE_YES && info->m_isLegalSoft)
				pathLegalList.push_back(info->m_strSavePath);
			if(info->m_bUpdateable == INSTALLS_CASE_UPDATEABLE_YES && !info->m_isLegalSoft)
				pathPiracyList.push_back(info->m_strSavePath);
		}
		if(pathLegalList.size() > 0)
		{
			UpgradeAppToLegalMsg UpgradeLegalMsg;
			UpgradeLegalMsg.caseId = _pCaseData->Id();
			UpgradeLegalMsg.pPathList = &pathLegalList;
			UpgradeLegalMsg.Send();
		}
		if(pathPiracyList.size() > 0)
		{
			UpgrateAppMsg UpgrateMsg;
			UpgrateMsg.caseId = _pCaseData->Id();
			UpgrateMsg.pPathList = &pathPiracyList;
			UpgrateMsg.Send();
		}

		if(pathLegalList.size() > 0 || pathPiracyList.size() > 0)
			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("RecentInstallCase_Text_Task_Added_to_list")), ICON_SUCCESS);
	}
	return true;
}

void CaseListInfoUI::ClearUIData()
{
	RETURN_IF (!_appListCtrl);
	_appListCtrl->RemoveAll();
	_appListCtrl->Resize();
	_appListCtrl->Invalidate();
}
