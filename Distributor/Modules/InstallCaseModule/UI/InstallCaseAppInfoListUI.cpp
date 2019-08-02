#include "StdAfx.h"
#include "LocalAppinfoList.h"
#include "MainView.h"
#include "InstallCaseAppInfoListUI.h"
#include "InstallCaseButtonItem.h"

INSTALLCASE_BEGIN

bool __stdcall InstallCaseAppListCompare(CListItemUI* pControlA, CListItemUI* pControlB, int nCol, bool exchange)
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
	case 4://
		return PublicFunc::CaseSortByData(pControlA, pControlB, L"LabelDownloadTime");
		break;
	default:
		break;
	}
	return !exchange;
}

InstallCaseAppInfoListUI::InstallCaseAppInfoListUI(HWND hWnd, InstallCaseUI* caseUI, InstallsCaseData* caseData)
{INTERFUN;
	_pInstallCaseUI = caseUI;

	_pCaseData = caseData;

	_pSearchContext = new AppSearchContextEx();
	_pSearchContext->Folder = NULL;
	_pSearchContext->PhoneType = 0;
	_pSearchContext->State = 0;
	_pSearchContext->SearchKey = _T("");
	_pSearchContext->Purchaser = _T("");
	_pSearchContext->CaseID = _pCaseData->Id();
	_pSearchContext->hWnd = hWnd;
	_pSearchContext->ShowLastVer = true;
	_pSearchContext->ExistLastSearch = false;
	_pSearchContext->IsSearching = false;

	_bSelectListVisible = false;
	_pComboBoxDevice = NULL;
	_pCaseInfoLbl = NULL;
	_pAppSelectObserve = NULL;
	_bClearingFilter = false;
	_eAppUpgradeType = _APP_UPGRADE_TYPE_TOSAMELEGAL;
}

InstallCaseAppInfoListUI::~InstallCaseAppInfoListUI(void)
{INTERFUN;

}

void InstallCaseAppInfoListUI::SetSelectListVisible(bool bVisible)
{INTERFUN;
	_bSelectListVisible = bVisible;
}

void InstallCaseAppInfoListUI::SetLastVerChecked(bool bVisible)
{INTERFUN;
	_pSearchContext->ShowLastVer = bVisible;
	_pShowLastVerCheckBox->SetChecked(bVisible);
}

void InstallCaseAppInfoListUI::AddAppInfoToView( int caseId, InstallsAppInfo* appInfo )
{INTERFUN;
	if(_pListOwner == NULL || _pCaseData->Id() != caseId || appInfo == NULL)
		return;

	wstring supportDev = L"";
	if(_pSearchContext->PhoneType == 1)
		supportDev = L"iphone";
	else if(_pSearchContext->PhoneType == 2)
		supportDev = L"ipad";

	//根据条件过滤软件
	
	bool n = appInfo ==NULL;
	int legalSoft = appInfo->m_isLegalSoft == 0?1:2;
	bool isSupport = appInfo->m_SupportDevice.empty() || appInfo->m_SupportDevice.find(supportDev) != std::wstring::npos; //平台过滤
	bool isBuyer = _pSearchContext->Purchaser.empty() || appInfo->m_Purchaser.compare(_pSearchContext->Purchaser) == 0;	//购买者过滤
	bool isState = _pSearchContext->State == 0 || legalSoft == _pSearchContext->State;	//是否正版软件过滤
	bool isnull = appInfo==NULL;
	bool isPath = _pSearchContext->Folder==NULL || CFileOperation::GetFileDir(appInfo->m_strSavePath).compare(_pSearchContext->Folder->FolderName) == 0;	//文件夹过滤
	bool isName = false;

	if(appInfo && !appInfo->m_bRemoving)
	{
		isName = CStrOperation::toLower(CCodeOperation::UnicodeToGB2312(appInfo->m_strName)).find(CStrOperation::toLower(CCodeOperation::UnicodeToGB2312(_pSearchContext->SearchKey))) != string::npos;	//软件名过滤
	}
	else
		return;
	if(!(isSupport && isBuyer && isState && isPath && isName))
	{
		return;
	}

	this->UpdateCaseInfo();

	if(appInfo)
	{
		_pListOwner->AddAppItem(appInfo,_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_UPGRATE, (_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_USER),(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_SYS), this->CheckSelected(appInfo->m_strSavePath.c_str()), false, &_updatePathList);
	}
	else
		wstring s = L"";
	if(!appInfo->m_Purchaser.empty())
	{
		map<wstring, CListItemUI*>::iterator it = _mapPurchaser.find(appInfo->m_Purchaser);
		if(it == _mapPurchaser.end())
		{
			CListItemUI* item = new CListItemUI();
			item->SetId(_T("1"));
			item->SetText(appInfo->m_Purchaser.c_str());
			_pPurchaserComboBox->Add(item);
			_mapPurchaser[appInfo->m_Purchaser] = item;
			_mapPurchaserCount[appInfo->m_Purchaser] = 1;
			_mapPurchaserAppPath.insert(appInfo->m_strSavePath);
		}
		else
		{
			if (_mapPurchaserAppPath.find(appInfo->m_strSavePath) == _mapPurchaserAppPath.end())
			{
				_mapPurchaserCount[appInfo->m_Purchaser]++;
				_mapPurchaserAppPath.insert(appInfo->m_strSavePath);
			}
			
		}
	}
	int count = _pList->GetCount();
	if(count > 0)
	{
		_pListEmpty->Resize();
		_pListEmpty->SetVisible(false);
		_pListEmpty->Invalidate();

		_pListFilterEmpty->SetVisible(false);
		_pListFilterEmpty->Resize();
		_pListFilterEmpty->Invalidate();

		if(_bSelectListVisible && !_pExpander->IsChecked())
			this->_pExpander->SetChecked(true);
	}
	_pFilterInfoLabel->SetText(CStrOperation::IntToWString(count).c_str());
}

void InstallCaseAppInfoListUI::RemoveAppInfoFromView( wstring appPath )
{INTERFUN;
	this->UpdateCaseInfo();

	InstallsAppInfo* appInfo  = _pCaseData->GetAppBySavePath(appPath);
	if (appInfo!=NULL)
	{
		if (!appInfo->m_Purchaser.empty())
		{
			if (_mapPurchaserAppPath.find(appPath) != _mapPurchaserAppPath.end())
			{
				_mapPurchaserAppPath.erase(appPath);
			}
			if (_mapPurchaserCount.find(appInfo->m_Purchaser) != _mapPurchaserCount.end())
			{
				_mapPurchaserCount[appInfo->m_Purchaser]--;
				if (_mapPurchaserCount[appInfo->m_Purchaser]<=0)
				{
					map<wstring, CListItemUI*>::iterator it = _mapPurchaser.find(appInfo->m_Purchaser);
					if ( it!= _mapPurchaser.end())
					{
						
						if (it->second!=NULL)
						{
							_pPurchaserComboBox->Remove(it->second);
							if (_pPurchaserComboBox->GetText() == appInfo->m_Purchaser)
							{
								_pPurchaserComboBox->SetText(L"");
							}
						}
						_mapPurchaser.erase(it);
						
					}

				}
			}
		}
	}

	//List的选中项保存在_pSelectItemContainer里，过滤等操作之后List中appId不存在，但在_pSelectItemContainer中存在。
	//所以当删除时要同时删除两个地方的项
	CControlUI* pControl = _pList->GetItem(appPath.c_str());
	if(pControl != NULL)
	{
		void* appinfo=(void*)pControl->GetUserData();
		_pList->Remove(pControl);
		_pList->Resize();
		_pList->Invalidate();
	}

	InstallCaseButtonItem* btnItem = (InstallCaseButtonItem*)_pSelectItemContainer->GetItem(appPath.c_str());
	if(btnItem != NULL)
		this->RemoveButtonItem(btnItem);
	if(_pList->GetCount() <= 0 && !_pListFilterEmpty->IsVisible())
	{
		//删除全部后，ListHeaderCheckBox仍然选中的问题
		_pList->CheckItem(false, -1, false);
		_pListEmpty->SetVisible(true);

		_pListEmpty->Resize();
		_pListEmpty->Invalidate();

		if(_bSelectListVisible && _pExpander->IsChecked())
			this->_pExpander->SetChecked(false);
	}
	for (vector<wstring>::iterator it=_updatePathList.begin(); it != _updatePathList.end(); it++)
	{
		if (*it == appPath)
		{
			_updatePathList.erase(it);
			break;
		}
	}
}

void InstallCaseAppInfoListUI::CancelAppUpgrate( wstring appPath )
{INTERFUN;
	CControlUI* pControl = _pList->GetItem(appPath.c_str());
	if(pControl == NULL)
		return;

	CContainerUI* container = (CContainerUI*)pControl->GetItem(5);
	CButtonUI* btn = (CButtonUI*)container->GetItem(0);
	if(!btn->IsEnabled())
	{
		btn->SetText(CommonUIFunc::LoadString(L"InstallCase_Text_UpGrade"));
		btn->SetEnabled(true);

		btn->Resize();
		btn->Invalidate();
	}
	for (vector<wstring>::iterator it=_updatePathList.begin(); it != _updatePathList.end(); it++)
	{
		if (*it == appPath)
		{
			_updatePathList.erase(it);
			break;
		}
	}
}

void InstallCaseAppInfoListUI::CancelAppUpgrateEx( wstring appPath )
{INTERFUN;
	CControlUI* pControl = _pList->GetItem(appPath.c_str());
	if(pControl &&(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_USER || _pCaseData->PlanType() == _INSTALL_PLAN_TYPE_SYS))
	{
		_pListOwner->UpdateAppItemByPath(appPath);
	}
}


void InstallCaseAppInfoListUI::HideOperateColumn()
{INTERFUN;
	CListHeaderItemUI* pHeader = _pList->GetCol(5);
	if (pHeader)
		pHeader->SetVisible(false, false);
}

void InstallCaseAppInfoListUI::ClearAppFromView(AppSearchContextEx* context)
{INTERFUN;
	if(context->CaseID == this->_pCaseData->Id())
	{
		_pList->RemoveAll();
		_pList->Resize();
		_pList->Invalidate();
		if(_pCaseData->GetAppNum()>0)
		{
			_pListEmpty->SetVisible(false);
			_pListEmpty->Resize();
			_pListEmpty->Invalidate();

			_pListFilterEmpty->SetVisible(true);
			_pListFilterEmpty->Resize();
			_pListFilterEmpty->Invalidate();
		}
	}
}
void InstallCaseAppInfoListUI::ClearAppFromView()
{INTERFUN;
	_pList->RemoveAll();
	_pList->Resize();
	_pList->Invalidate();
	if(_pCaseData->GetAppNum()>0)
	{
		_pListEmpty->SetVisible(false);
		_pListEmpty->Resize();
		_pListEmpty->Invalidate();

		_pListFilterEmpty->SetVisible(true);
		_pListFilterEmpty->Resize();
		_pListFilterEmpty->Invalidate();
	}
}

void InstallCaseAppInfoListUI::AddButtonItem(InstallsAppInfo* appInfo)
{INTERFUN;
	if(appInfo == NULL || _pSelectItemContainer->GetItem(appInfo->m_strSavePath.c_str()) != NULL)
		return;

	InstallCaseButtonItem* pItem = new InstallCaseButtonItem(this);
	pItem->Create(_T("InstallCaseButtonItem"), appInfo->m_strSavePath, _pSelectItemContainer, appInfo);
	pItem->SetText(appInfo->m_strName);
	pItem->SetIcon(appInfo->m_Iconpath);

	_pSelectItemContainer->Resize();
	_pSelectItemContainer->Invalidate();
}

void InstallCaseAppInfoListUI::RemoveButtonItem(InstallCaseButtonItem* item)
{INTERFUN;
	if(item == NULL)
		return;

	CControlUI* pListItem = _pList->GetItem(item->GetId());
	if(pListItem != NULL)
	{
		_pList->CheckItem(false, pListItem->GetIndex());
	}
	_pSelectItemContainer->Remove(item);

	_pList->Resize();
	_pList->Invalidate();
	_pSelectItemContainer->Resize();
	_pSelectItemContainer->Invalidate();

	if(_pAppSelectObserve != NULL)
	{
		_pAppSelectObserve->Selected();
	}
}

void InstallCaseAppInfoListUI::Search()
{INTERFUN;
	if(_pCaseData->GetAppNum() > 0)
		OnFilter();
	else
		_pInstallCaseUI->Model.LoadAppList(_pCaseData->Id());
}

void InstallCaseAppInfoListUI::SetAppSelectObserve( CaseAppSelectObserve* obs )
{INTERFUN;
	_pAppSelectObserve = obs;
}

bool InstallCaseAppInfoListUI::CheckSelected(wstring id)
{INTERFUN;
	CControlUI* item = NULL;
	if(_pSelectItemContainer != NULL)
		item = _pSelectItemContainer->GetItem(id.c_str());
	return item != NULL;
}

void InstallCaseAppInfoListUI::OnCreate()
{INTERFUN;

	this->InitControl();

	this->InitFolders();

	this->InitDeviceList();
}

void InstallCaseAppInfoListUI::InitControl()
{INTERFUN;
	_pCaseInfoLbl							= (CLabelUI*)GetItem(_T("CaseInfoLbl"));
	_pFilterPanel							= (CControlUI*)GetItem(_T("Top.Filter"));
	_pPhoneTypeComboBox						= (CComboBoxUI*)GetItem(_T("Top.Filter.AppFilterPhoneType"));
	_pPurchaserComboBox						= (CComboBoxUI*)GetItem(_T("Top.Filter.AppFilterPurchaser"));
	_pStateComboBox							= (CComboBoxUI*)GetItem(_T("Top.Filter.AppFilterState"));
	_pFolderComboBox						= (CComboBoxUI*)GetItem(_T("Top.Filter.AppFilterFolder"));
	_pSearchEdit							= (CSearchEditUI*)GetItem(_T("Top.Search.Edit"));
	_pSearchClear							= (CButtonUI*)GetItem(_T("Top.Search.Clear"));
	_pEditCaseBtn							= (CButtonUI*)GetItem(_T("Bottom.Left.EditCaseBtn"));
	_pUpgrateAppBtn							= (CButtonUI*)GetItem(_T("Bottom.Left.UpgrateAppBtn"));
	_pAddNewAppBtn							= (CButtonUI*)GetItem(_T("Bottom.Left.AddNewAppBtn"));
	_pAddToCaseBtn							= (CButtonUI*)GetItem(_T("Bottom.Left.AddToCaseBtn"));
	_pRemoveAppBtn							= (CButtonUI*)GetItem(_T("Bottom.Left.RemoveAppBtn"));
	_pRefreshCaseBtn						= (CButtonUI*)GetItem(_T("Bottom.Left.RefreshCaseBtn"));
	_pOnekeyInstallBtn						= (CButtonUI*)GetItem(_T("Bottom.Right.OnekeyInstallBtn"));
	_pShowLastVerCheckBox					= (CCheckBoxUI*)GetItem(_T("Top.Filter.ShowLastVerCheckBox"));//Bottom.Right.ShowLastVerCheckBox"));
	_pComboBoxDevice						= (CComboBoxUI*)GetItem(_T("Bottom.Right.ComboBoxDevice"));
	_pEmptyAddAppBtn						= (CButtonUI*)GetItem(_T("ListEmpty.EmptyAddAppBtn"));
	//_pClearFilterBtn						= (CButtonUI*)GetItem(_T("ListFilterEmpty.ListFilterEmptyControls.ClearFilterBtn"));	
	_pAndUpgradeAllBtn						= (CButtonUI*)GetItem(_T("Bottom.Left.AndUpgrateAllAppsBtn"));
	_pUpgradeAllBtn							= (CButtonUI*)GetItem(_T("Bottom.Left.Upgrate.UpgrateAllAppsBtn"));
	_pUpgrateAllMenuBtn						= (CButtonUI*)GetItem(_T("Bottom.Left.Upgrate.UpgrateAllAppsMenuBtn"));
	_pUpgradeAllLayout						= (CLayoutUI*)GetItem(_T("Bottom.Left.Upgrate"));

	_pFilterInfo = dynamic_cast<CLayoutUI*>(GetItem(_T("FilterInfo")));
	_pFilterInfoLabel = dynamic_cast<CLabelUI*>(GetItem(_T("FilterInfo.FilterInfoLabel")));
	_pClearFilterInfoBtn = dynamic_cast<CButtonUI*>(GetItem(_T("FilterInfo.ClearFilterBtn")));
	_pCloseFilterInfoBtn = dynamic_cast<CButtonUI*>(GetItem(_T("FilterInfo.CloseFilterInfoBtn")));

	_pExpander = dynamic_cast<CCheckBoxUI*>(GetItem(_T("SelectItemList.Top.Expander")));
	_pSelectListLayout = dynamic_cast<CLayoutUI*>(GetItem(_T("SelectItemList")));
	_pSelectItemContainer = dynamic_cast<CLayoutUI*>(GetItem(_T("SelectItemList.Container")));
	pSelectLayoutClose = (CButtonUI*)GetItem(_T("SelectItemList.Top.SelectLayoutClose"));

	_pListEmpty = dynamic_cast<CLayoutUI*>(GetItem(_T("ListEmpty")));
	_pListFilterEmpty = dynamic_cast<CLayoutUI*>(GetItem(_T("ListFilterEmpty")));
	_pBottom = dynamic_cast<CLayoutUI*>(GetItem(_T("Bottom")));
	_pList = dynamic_cast<CListUI*>(GetItem(_T("List")));
	if(_pCaseData->PlanType() != _INSTALL_PLAN_TYPE_UPGRATE)
		_pList->ModifyControlFlags(UIFLAG_ENABLE_DRAG|UIFLAG_ENABLE_DROP, 0);
	_pList->SetSortCallBack(InstallCaseAppListCompare);
	_pListOwner = new LocalAppinfoList(_pList, this);

	//编辑装机方案情况
	if(_bSelectListVisible)
	{
		_pFilterPanel->SetAttribute(L"mode",L"righttoleft");//
		_pBottom->SetVisible(false);
		_pSelectListLayout->SetVisible(true);
		_pList->SetInitPos(0,39,0,120, 8);
		_pShowLastVerCheckBox->SetVisible(false);
		this->OnExpanderClick(NULL);
	}
	//软件库
	else
	{
		_pFilterPanel->SetAttribute(L"mode",L"lefttoright");//righttoleft
		_pShowLastVerCheckBox->SetVisible(true);
		_pBottom->SetVisible(true);
		_pSelectListLayout->SetVisible(false);
		_pList->SetInitPos(0,39,0,40, 8);
	}

	_pEmptyAddAppBtn->SetVisible(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_USER || _pCaseData->PlanType() == _INSTALL_PLAN_TYPE_SYS);

	if(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_SYS)
	{
		if(_pCaseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
		{
			_pPhoneTypeComboBox->SetVisible(true);
			_pPurchaserComboBox->SetVisible(true);
			_pStateComboBox->SetVisible(true);
			_pFolderComboBox->SetVisible(true);
		}
		else
		{
			_pPhoneTypeComboBox->SetVisible(false);
			_pPurchaserComboBox->SetVisible(false);
			_pStateComboBox->SetVisible(false);
			_pFolderComboBox->SetVisible(true);
		}
	}
	else 
	{
		_pFilterPanel->SetVisible(false);
	}

	_pCaseInfoLbl->SetVisible(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_USER);

	this->UpdateCaseInfo();

	_pEditCaseBtn->SetVisible(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_USER);
	_pUpgrateAppBtn->SetVisible(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_UPGRATE);
	_pAddNewAppBtn->SetVisible(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_SYS);
	_pAddToCaseBtn->SetVisible(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_SYS);
	_pRemoveAppBtn->SetVisible(_pCaseData->PlanType() != _INSTALL_PLAN_TYPE_UPGRATE);
	if (_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_USER)
	{
		_pRemoveAppBtn->SetText(CommonUIFunc::LoadString(_T("InstallCase_Text_Remove")));
	}
	else
	{
		_pRemoveAppBtn->SetText(CommonUIFunc::LoadString(_T("InstallCase_Text_Delete")));
	}
	_pRefreshCaseBtn->SetVisible(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_SYS || _pCaseData->PlanType() == _INSTALL_PLAN_TYPE_UPGRATE);
	_pUpgradeAllLayout->SetVisible((_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_USER || _pCaseData->PlanType() == _INSTALL_PLAN_TYPE_SYS) && _pCaseData->CaseType() == _INSTALLS_CASE_DATA_IOS);
	_pAndUpgradeAllBtn->SetVisible( _pCaseData->CaseType() == _INSTALLS_CASE_DATA_ANDROID);
	_pShowLastVerCheckBox->SetChecked(!(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_USER));
	_pUpgrateAllMenuBtn->SetEnabled(_pCaseData->CaseType() == _INSTALLS_CASE_DATA_IOS);
	_pPhoneTypeComboBox->SelectItem(_pPhoneTypeComboBox->GetItem(0));
	_pPurchaserComboBox->SelectItem(_pPurchaserComboBox->GetItem(0));
	_pStateComboBox->SelectItem(_pStateComboBox->GetItem(0));
}

void InstallCaseAppInfoListUI::InitFolders()
{INTERFUN;
	vector<AppCountByFolder*> folders = _pInstallCaseUI->Model.GetAppFolderList();
	if(folders.size() == _pFolderComboBox->GetCount())
		return;

	_pFolderComboBox->RemoveAll();

	CListItemUI* rItem = new CListItemUI();
	rItem->SetId(L"0");
	rItem->SetText(CommonUIFunc::LoadString(L"InstallCase_Text_all"));	
	_pFolderComboBox->Add(rItem);

	for (vector<AppCountByFolder*>::iterator it = folders.begin(); it != folders.end(); it++)
	{
		AppCountByFolder* folderData = *it;
		if(folderData->Filetype != _pCaseData->CaseType())
			continue;

		CListItemUI* item = new CListItemUI();

		wstring folderName = CStrOperation::trimRightW(folderData->FolderName,L"\\");
		if(folderName == CStrOperation::trimRightW(CONFIG(_strAndroidDownPath),L"\\") && _INSTALLS_CASE_DATA_ANDROID == folderData->Filetype)
			item->SetText(CommonUIFunc::LoadString(L"InstallCase_Text_AndroidFolder"));
		else if(folderName == CStrOperation::trimRightW(CONFIG(_strIosDownPath),L"\\") &&  _INSTALLS_CASE_DATA_IOS== folderData->Filetype)
			item->SetText(CommonUIFunc::LoadString(L"InstallCase_Text_IosFolder"));
		else
			item->SetText(CFileOperation::GetFileName(folderName).c_str());	
		item->SetId(CStrOperation::IntToWString(folderData->m_id).c_str());
		item->SetUserData((UINT)folderData);

		_pFolderComboBox->Add(item);
	}
}

void InstallCaseAppInfoListUI::UpdateCaseInfo()
{INTERFUN;
	if(_pCaseInfoLbl == NULL)
		return;

	int num=0;
	if(_pCaseData->GetAppNum() > 0 && _pCaseData->GetSumFileSize() > 0)
	{
		wstring caseInfo = L"共";
		caseInfo += CStrOperation::FormatFileSize(_pCaseData->GetSumFileSize(),1);
		caseInfo += L"， 预估安装时长";
		caseInfo += UIStringFormat::InstallNeedTime(_pCaseData->ListApp()->size(),_pCaseData->GetSumFileSize(),num);
		caseInfo += L"。";
		_pCaseInfoLbl->SetText(caseInfo.c_str());
	}
	else
	{
		_pCaseInfoLbl->SetText(L"");
	}
	RefreshUpgradeCount();
	_pFilterInfoLabel->SetText(CStrOperation::IntToWString(_pList->GetCount()).c_str());
}

void InstallCaseAppInfoListUI::InitDeviceList()
{INTERFUN;
	vector<wstring> idList = DEVICE_MANAGER->GetAllDevice();
	int size = idList.size();
	if(size > 0)
	{
		for (int i = 0; i < size; i++)
		{
			wstring sn = idList.at(i);
			DeviceData* pDevice = DeviceManager::GetInstance()->FindDevice(sn);
			_INSTALLS_CASE_DATA_TYPE type = _INSTALLS_CASE_DATA_IOS;
			if(pDevice->GetDevInfo()->_deviceType == Android)
				type = _INSTALLS_CASE_DATA_ANDROID;

			if(type==_INSTALLS_CASE_DATA_IOS&&pDevice->GetDevInfo()->_connectType != USB)
				continue;
			if(_pCaseData->CaseType() == type)
			{
				CListItemUI* item = new CListItemUI();
				item->SetId(sn.c_str());
				item->SetText(pDevice->GetDevInfo()->GetDeviceName().c_str());
				item->SetUserData((UINT)pDevice);
				_pComboBoxDevice->Add(item);
			}
		}
		if(_pComboBoxDevice->GetCount() > 0)
		{
			_pComboBoxDevice->SetUserData(_pComboBoxDevice->GetItem(0)->GetUserData());
			_pComboBoxDevice->SelectItem(_pComboBoxDevice->GetItem(0));
		}
	}
}

bool  InstallCaseAppInfoListUI::ListFocused()
{INTERFUN;
	CControlUI* pControl = this->GetWindow()->GetFocus();
	while ( pControl )
	{
		if ( pControl == _pList )
			return true;

		pControl = pControl->GetParent();
	}
	return false;
}

//void InstallCaseAppInfoListUI::Receive( MESSAGE_TYPE msg, const BaseParam* pParm )
//{INTERFUN;
//	RETURN_IF(!ListFocused());
//	switch(pParm->GetInt())
//	{
//	case VK_F5:
//		OnRefreshCaseBtnClick(NULL);
//		//this->_pInstallCaseUI->Model.RefreshCase(_pCaseData->Id());
//		break;
//	case VK_DELETE:
//	case VK_DECIMAL:
//		{
//			if(_pCaseData->PlanType() != _INSTALL_PLAN_TYPE_UPGRATE)
//				this->RemoveSelectApp();
//		}
//		break;
//	}
//}

bool InstallCaseAppInfoListUI::OnSingleInstall(TNotifyUI* pNotify)
{INTERFUN;
	_pInstallCaseUI->StatisSingleInstall();

	CControlUI* item = _pComboBoxDevice->GetCurSel();
	if(item != NULL)
	{
		this->InstallSelectApp(item->GetId());
	}
	else
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_CaseInstallNoDeviceFind")), ICON_WARNING);
	}
	return true;
}

//升级按钮
bool InstallCaseAppInfoListUI::OnSingUpgrade(TNotifyUI* pNotify)
{INTERFUN;
	_pInstallCaseUI->StatisSingleInstall();

	this->UpgrateSelectApp();
	return true;
}

bool InstallCaseAppInfoListUI::OnSingleRemove(TNotifyUI* pNotify)
{INTERFUN;
	_pInstallCaseUI->StatisSingleInstall();

	if(pNotify->pSender->GetParent()->GetParent() == NULL)
		return true;

	CSimpleListItemUI* item = (CSimpleListItemUI*)pNotify->pSender->GetParent()->GetParent();
	item->Select();

	this->RemoveSelectApp();
	return true;
}

bool InstallCaseAppInfoListUI::OnEmptyAddAppBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	if(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_SYS)
	{
		if(_pCaseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
			_pInstallCaseUI->Model.AddLibFolder(_INSTALLS_CASE_DATA_IOS);
		else
			_pInstallCaseUI->Model.AddLibFolder(_INSTALLS_CASE_DATA_ANDROID);	
	}
	else if(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_USER)
	{
		_pInstallCaseUI->Model.EditCase(_pCaseData->Id());
	}
	return true;
}

bool InstallCaseAppInfoListUI::OnClearFilterBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	_bClearingFilter = true;
	_pPhoneTypeComboBox->SelectItem(0);
	_pPurchaserComboBox->SelectItem(0);
	_pStateComboBox->SelectItem(0);
	_pFolderComboBox->SelectItem(0);
	if(_pSearchEdit->IsEmpty())
		OnFilter();
	else
		this->OnSearchClear(NULL);

	if(_pList->GetCount() <= 0)
	{
		//删除全部后，ListHeaderCheckBox仍然选中的问题
		_pList->CheckItem(false, -1, false);
		_pListEmpty->SetVisible(true);

		_pListEmpty->Resize();
		_pListEmpty->Invalidate();

		_pListFilterEmpty->SetVisible(false);
		_pListFilterEmpty->Resize();
		_pListFilterEmpty->Invalidate();
	}

	_bClearingFilter = false;
	return true;
}

bool InstallCaseAppInfoListUI::OnAddToCaseBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	this->ShowAppListMenu(pNotify, false);
	return true;
}

bool InstallCaseAppInfoListUI::OnEditCaseBtnClick(TNotifyUI* pNotify)
{INTERFUN;
	_pInstallCaseUI->Model.EditCase(_pCaseData->Id());
	return true;
}

bool InstallCaseAppInfoListUI::OnUpgrateAppBtnClick(TNotifyUI* pNotify)
{INTERFUN;
	_pInstallCaseUI->StateAddAppAndBatchUpgrate();
	this->UpgrateSelectApp();
	return true;
}

bool InstallCaseAppInfoListUI::OnAddNewAppBtnClick(TNotifyUI* pNotify)
{INTERFUN;
	_pInstallCaseUI->StateAddAppAndBatchUpgrate();

	this->_pInstallCaseUI->Model.AddAppToCaseByPath(_pCaseData->Id(), NULL);
	return true;
}

bool InstallCaseAppInfoListUI::OnRemoveAppBtnClick(TNotifyUI* pNotify)
{INTERFUN;
	this->_pInstallCaseUI->StatisBatchRemove();

	this->RemoveSelectApp();
	return true;
}

bool InstallCaseAppInfoListUI::OnRefreshCaseBtnClick(TNotifyUI* pNotify)
{INTERFUN;
	if(!_pRefreshCaseBtn->IsEnabled())
		return false;
	_pRefreshCaseBtn->SetEnabled(false);
	_pRefreshCaseBtn->GetParent()->Resize();
	_pRefreshCaseBtn->GetParent()->Invalidate();
	int count = _pList->GetCount();
	this->_pInstallCaseUI->Model.RefreshCase(_pSearchContext->ShowLastVer,_pCaseData->Id());
	return true;
}

bool InstallCaseAppInfoListUI::OnShowLastVerCheckBoxClick(TNotifyUI* pNotify)
{INTERFUN;
	this->_pInstallCaseUI->StatisShowLaseVer();

	_pSearchContext->ShowLastVer = _pShowLastVerCheckBox->IsChecked();
	this->OnFilter();
	return true;
}

bool InstallCaseAppInfoListUI::OnOnekeyInstallBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	this->_pInstallCaseUI->StatisOnKeyInstall();

	CControlUI* item = _pComboBoxDevice->GetCurSel();
	if(item != NULL)
	{
		this->InstallSelectApp(item->GetId());
	}
	else
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_CaseInstallNoDeviceFind")), ICON_WARNING);
	}
	return true;
}

bool InstallCaseAppInfoListUI::OnSelectLayoutCloseClick( TNotifyUI* pNotify )
{INTERFUN;
	for(int i = _pSelectItemContainer->GetCount() - 1; i >= 0; i--)
	{
		InstallCaseButtonItem* item = (InstallCaseButtonItem*)_pSelectItemContainer->GetItem(i);
		this->RemoveButtonItem(item);
	}

	if(_pAppSelectObserve != NULL)
	{
		_pAppSelectObserve->Selected();
	}
	return true;
}

bool InstallCaseAppInfoListUI::OnExpanderClick( TNotifyUI* pNotify )
{INTERFUN;
	bool checked = _pExpander->IsChecked();
	if(checked)
	{
		pSelectLayoutClose->SetVisible(true);
		_pSelectItemContainer->SetVisible(true);
		_pSelectListLayout->SetInitPos(0,120,0, 0, 6);
		_pList->SetInitPos(0,39,0,120, 8);
	}
	else
	{
		pSelectLayoutClose->SetVisible(false);
		_pSelectItemContainer->SetVisible(false);
		_pSelectListLayout->SetInitPos(0,20,0, 0, 6);
		_pList->SetInitPos(0,39,0,20, 8);
	}
	this->Resize();
	this->Invalidate();
	return true;
}


bool InstallCaseAppInfoListUI::OnShowMenuAppList(TNotifyUI* pNotify)
{INTERFUN;
	if(_bSelectListVisible)
		return false;
	if(pNotify->lParam==0)
		return true;
	bool noselect=true;
	for (int i=0; i<_pList->GetCount();i++)
	{
		if(_pList->IsCheckItem(i))
		{
			noselect=false;
			break;
		}
	}

	RETURN_TRUE_IF(noselect);

	this->ShowAppListMenu(pNotify, true);

	return true;
}

bool InstallCaseAppInfoListUI::OnItemSelect(TNotifyUI* pNotify)
{INTERFUN;
	bool isSelect = NULL != pNotify->wParam ;
	CControlUI* pCurSel = (CControlUI*)pNotify->lParam;
	if(pCurSel == NULL)
		return true;

	InstallsAppInfo* appInfo = (InstallsAppInfo*)pCurSel->GetUserData();
	if(appInfo == NULL)
		return true;

	if(isSelect)
	{
		this->AddButtonItem(appInfo);
	}
	else
	{
		CControlUI* pItem = _pSelectItemContainer->GetItem(pCurSel->GetId());
		_pSelectItemContainer->Remove(pItem);
	}

	if(_pAppSelectObserve != NULL)
	{
		_pAppSelectObserve->Selected();
	}

	_pSelectItemContainer->Resize();
	_pSelectItemContainer->Invalidate();

	return true;
}

void InstallCaseAppInfoListUI::ShowAppListMenu(TNotifyUI* pNotify, bool isContextMenu /*= true*/ )
{INTERFUN;
	CMenuUI* pMenu = CMenuUI::Load(IDR_MENU_APP_LIST);

	//if(pCaseData->Id() == Sys_Case_IOS_ID || pCaseData->Id() == Sys_Case_Android_ID)
	if(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_SYS)
	{
		this->AddDeviceMenu(pMenu);	

		this->AddCaseMenu(pMenu);
	}
	else if(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_UPGRATE)
	{
		this->AddDeviceMenu(pMenu);	

		CMenuUI* pDevMenu = new CMenuUI();
		pDevMenu->SetUID(IDR_MENU_UPGRATE);
		pDevMenu->SetText(L"升级");
		pMenu->Insert(pDevMenu, BY_FIRST, 0);

		pMenu->Remove(IDR_MENU_ADD_TO_CASE,BY_COMMAND);
	}
	else if(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_USER)
	{
		this->AddDeviceMenu(pMenu);	
		CMenuUI* pDevMenu = (CMenuUI*)pMenu->GetItem(0);
		if(pDevMenu->GetUID() != IDR_MENU_INSTALL_APP)
			pMenu->Remove(1,BY_POSITION);
		pMenu->Remove(IDR_MENU_ADD_TO_CASE,BY_COMMAND);
	}
	else
	{
		pMenu->Remove(IDR_MENU_INSTALL_APP,BY_COMMAND);
		pMenu->Remove(IDR_MENU_ADD_TO_CASE,BY_COMMAND);
	}

	if(isContextMenu)
	{
		if (pMenu)
		{
			pMenu->SetParam(pNotify->wParam,pNotify->lParam);
			pMenu->Show(Singleton::GetMainWnd(), NULL, this);
		}
	}
	else
	{
		if(pMenu)
		{
			CMenuUI* pSubMenu = pMenu->GetItem(IDR_MENU_ADD_TO_CASE, BY_COMMAND);
			CommonUIFunc::ShowMenu(pSubMenu, Singleton::GetMainWnd(), this, pNotify->pSender);
		}
	}
}

bool  InstallCaseAppInfoListUI::OnClickMenu(TNotifyUI* pNotify)
{INTERFUN;
	if (pNotify->wParam == NULL)
		return true;
	UIMENUINFO* pInfo = (UIMENUINFO*)pNotify->wParam;

	bool hasChecked = false;
	for (int i = 0; i < _pList->GetCount(); i++)
	{
		if(_pList->IsCheckItem(i))
		{
			hasChecked = true;
			break;
		}
	}

	switch (pInfo->uID)
	{
	case IDR_MENU_DELETE_SELECT_APP:
		this->RemoveSelectApp();
		break;
	case IDR_MENU_ADD_TO_NEW_CASE:
		this->AddSelectAppToCase(-1);
		break;
	case IDR_MENU_UPGRATE:
		this->UpgrateSelectApp();
		break;
	case IDR_MENU_INSTALL_APP:		
		this->InstallSelectApp(pInfo->szKey);
		break;
	case IDR_MENU_OPENAPPFOLDER:
		this->OpenAppFolder();
		break;
	default:
		this->AddSelectAppToCase(pInfo->uID-IDR_MENU_ADD_TO_NEW_CASE);
		break;
	}

	return true;
}

bool InstallCaseAppInfoListUI::OnPhoneTypeComboChecked( TNotifyUI* pNotify )
{INTERFUN;
	CListItemUI* pListItem = (CListItemUI*)_pPhoneTypeComboBox->GetCurSel();	
	int phoneType = _wtoi(pListItem->GetId());
	if(phoneType == 0)
	{
		_pPhoneTypeComboBox->SetText(CommonUIFunc::LoadStringW(L"InstallCase_Text_AppFilterPhoneType"));
	}
	if(_pSearchContext->PhoneType == phoneType)
		return true;

	_pSearchContext->PhoneType = phoneType;

	if(_bClearingFilter)
		return true;
	OnFilter();
	return true;
}

bool InstallCaseAppInfoListUI::OnPurchaserComboChecked( TNotifyUI* pNotify )
{INTERFUN;
	CListItemUI* pListItem = (CListItemUI*)_pPurchaserComboBox->GetCurSel();
	int id = _wtoi(pListItem->GetId());
	wstring buyer = L"";
	if (id != 0)
	{
		buyer = pListItem->GetText();
	}
	else
	{
		_pPurchaserComboBox->SetText(CommonUIFunc::LoadStringW(L"InstallCase_Text_AppFilterBuyer"));
	}

	if(_pSearchContext->Purchaser == buyer)
		return true;

	_pSearchContext->Purchaser = buyer;

	if(_bClearingFilter)
		return true;
	this->OnFilter();
	return true;
}

bool InstallCaseAppInfoListUI::OnStateComboChecked( TNotifyUI* pNotify )
{INTERFUN;
	CListItemUI* pListItem = (CListItemUI*)_pStateComboBox->GetCurSel();
	int state = _wtoi(pListItem->GetId());
	if(state == 0)
	{
		_pStateComboBox->SetText(CommonUIFunc::LoadStringW(L"InstallCase_Text_AppFilterState"));
	}
	if(_pSearchContext->State == state)
		return true;

	_pSearchContext->State = state;

	if(_bClearingFilter)
		return true;
	OnFilter();
	return true;
}

bool InstallCaseAppInfoListUI::OnFolderComboChecked( TNotifyUI* pNotify )
{INTERFUN;
	CListItemUI* pListItem = (CListItemUI*)_pFolderComboBox->GetCurSel();
	int id = _wtoi(pListItem->GetId());
	if(id == 0)
	{
		_pFolderComboBox->SetText(CommonUIFunc::LoadStringW(L"InstallCase_Text_AppFilterFolder"));
	}
	AppCountByFolder* folder = (AppCountByFolder*)pListItem->GetUserData();

	if(folder == _pSearchContext->Folder)
		return true;

	if(id == 0)
		_pSearchContext->Folder = NULL;
	else
		_pSearchContext->Folder = folder;

	if(_bClearingFilter)
		return true;
	this->OnFilter();
	return true;
}

bool InstallCaseAppInfoListUI::OnFolderComboDropDown(TNotifyUI* pNotify)
{INTERFUN;
	this->InitFolders();
	return true;
}

bool InstallCaseAppInfoListUI::OnSearch(TNotifyUI* pNotify)
{INTERFUN;
	wstring text=_pSearchEdit->GetText();

	if(text.empty())
	{
		_pSearchClear->SetVisible(false);
	}
	else
	{
		_pSearchClear->SetVisible(true);
	}

	if(text.empty() || _pSearchEdit->IsEmpty())
	{
		_pSearchEdit->HideSearch();
	}
	if(_pSearchContext->SearchKey == text)
		return true;

	_pSearchContext->SearchKey = text;

	this->OnFilter();
	return true;
}

bool InstallCaseAppInfoListUI::OnSearchClear( TNotifyUI* pNotify )
{INTERFUN;
	_pSearchEdit->SetText(_T(""));
	_pSearchEdit->Resize();
	_pSearchEdit->Invalidate();
	_pSearchEdit->HideSearch();
	_pSearchContext->SearchKey = L"";
	_pSearchClear->SetVisible(false);

	if(_pList->GetCount() <= 0)
	{
		//删除全部后，ListHeaderCheckBox仍然选中的问题
		_pList->CheckItem(false, -1, false);
		_pListEmpty->SetVisible(true);

		_pListEmpty->Resize();
		_pListEmpty->Invalidate();

		_pListFilterEmpty->SetVisible(false);
		_pListFilterEmpty->Resize();
		_pListFilterEmpty->Invalidate();
	}
	//this->OnFilter();
	return true;

}

void InstallCaseAppInfoListUI::OnFilter()
{INTERFUN;
	this->_pInstallCaseUI->Model.SearchApp(_pSearchContext);
	ShowFilterInfo(true);
}

bool InstallCaseAppInfoListUI::OnDropFiles( TNotifyUI* pNotify )
{INTERFUN;
	HDROP hDrop = (HDROP)pNotify->wParam;
	wchar_t szFileName[MAX_PATH + 1] = {0};

	if ( _pCaseData->CaseType() == _INSTALLS_CASE_DATA_IOS )
	{
		CStaticsticsMessage StaticMsg("32011");
		StaticMsg.Send();
	}
	else if ( _pCaseData->CaseType() == _INSTALLS_CASE_DATA_ANDROID )
	{
		CStaticsticsMessage StaticMsg("33011");
		StaticMsg.Send();

	} 

	UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	vector<wstring> Files;

	for(UINT i=0;i<nFiles;i++)
	{
		DragQueryFile(hDrop, i, szFileName, MAX_PATH);
		if(CFileOperation::IsDirExist(szFileName))
		{
			vector<wstring> subFiles;
			if(_pCaseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
			{
				CFileOperation::FindAllFileInDir(szFileName, subFiles, _T("ipa"));
				Files.insert(Files.end(),subFiles.begin(), subFiles.end());
				subFiles.clear();
				CFileOperation::FindAllFileInDir(szFileName, subFiles, _T("pxl"));
				Files.insert(Files.end(),subFiles.begin(), subFiles.end());
				subFiles.clear();
			}
			else
			{
				CFileOperation::FindAllFileInDir(szFileName, subFiles, _T("apk"));
				Files.insert(Files.end(),subFiles.begin(), subFiles.end());
				subFiles.clear();
				CFileOperation::FindAllFileInDir(szFileName, subFiles, _T("npk"));
				Files.insert(Files.end(),subFiles.begin(), subFiles.end());
			}
		}
		else
		{
			wstring ext = CFileOperation::GetFileExt(szFileName);
			ext = CStrOperation::toLowerW(ext);
			if((ext == L"ipa" || ext == L"pxl") && _pCaseData->CaseType() == _INSTALLS_CASE_DATA_IOS)
			{
				Files.push_back(wstring(szFileName));

			}
			else if((ext == L"apk" || ext == L"npk") && _pCaseData->CaseType() == _INSTALLS_CASE_DATA_ANDROID)
			{
				Files.push_back(wstring(szFileName));
			}
		}
	}

	DragFinish(hDrop);
	if(Files.size() > 0)
	{
		_pInstallCaseUI->Model.AddAppToCaseByPath(_pCaseData->Id(), &Files);
	}
	else
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_FileNoFound")), ICON_WARNING);
	}
	return true;
}

void InstallCaseAppInfoListUI::AddSelectAppToCase( int caseId )
{INTERFUN;
	vector<wstring> pathList;
	this->GetSelectPathList(pathList);
	if(pathList.size() > 0)
	{
		if(caseId > 0)
		{
			_pInstallCaseUI->Model.AddAppToCase(caseId, &pathList);

			//NoticeDialog(L"已添加到方案！", ICON_SUCCESS);
		}
		else
			_pInstallCaseUI->Model.AddCase(_pCaseData->CaseType(), &pathList);
	}
	else
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_SelectTrueAppPls")), ICON_WARNING);
	}
}

void InstallCaseAppInfoListUI::UpgrateSelectApp()
{INTERFUN;
	if(_pList->GetCount()==0)
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_NonUpdateItems")), ICON_WARNING);
		return;
	}
	vector<wstring> pathList;

	int checkcount=0;
	for (int i=0;i<_pList->GetCount();i++)
	{
		if(_pList->IsCheckItem(i))
		{
			checkcount++;
			CListItemUI* item = (CListItemUI*)_pList->GetItem(i);
			CContainerUI* container = (CContainerUI*)item->GetItem(5);
			CButtonUI* btn = (CButtonUI*)container->GetItem(0);
			if(btn->IsEnabled())
			{
				wstring nID = item->GetId();
				pathList.push_back(nID);
				vector<wstring>::iterator it = ::find(_updatePathList.begin(), _updatePathList.end(), nID);
				if(it == _updatePathList.end())//没有
				{
					_updatePathList.push_back(item->GetId());
				}

				btn->SetText(CommonUIFunc::LoadString(L"InstallCase_Text_UpGrading"));
				btn->SetEnabled(false);

				btn->Resize();
				btn->Invalidate();
			}
		}
	}

	if(pathList.size() > 0)
	{
		_pInstallCaseUI->Model.UpgrateApp(_pCaseData->Id(), &pathList);
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_Task_Added_to_list")), ICON_SUCCESS);
	}
	if(checkcount==0)
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_SelectTrueUpdateAppPls")), ICON_WARNING);
	}
	if(pathList.size()==0 && checkcount>0)
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_CheckedItesUpdating")), ICON_WARNING);
	}
	//else
	//{
	//	NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_SelectTrueUpdateAppPls")), ICON_WARNING);
	//}
}

void InstallCaseAppInfoListUI::RemoveSelectApp()
{INTERFUN;
	vector<wstring> pathList;
	this->GetSelectPathList(pathList);
	if(pathList.size() > 0)
	{
		bool bRemoveFile = false;
		if(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_SYS)
			bRemoveFile = true;

		_pInstallCaseUI->Model.RemoveAppFromCase(_pCaseData->Id(), &pathList, bRemoveFile);
	}
	else
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_electSoftWareForRemovePls")), ICON_WARNING);
	}
}

void InstallCaseAppInfoListUI::InstallSelectApp( wstring sn )
{INTERFUN;
	if(DEVICE_MANAGER->FindDevice(sn) == NULL)
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_CaseInstallNoDeviceFind")), ICON_WARNING);
		return;
	}

	vector<wstring> pathList;
	this->GetSelectPathList(pathList);
	if(pathList.size() > 0)
	{
		bool bRemoveFile = false;
		if(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_SYS)
			bRemoveFile = true;

		_pInstallCaseUI->Model.InstallApp(sn, _pCaseData->Id(), &pathList);
	}
	else
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_SelectSoftWarePls")), ICON_WARNING);
	}
}

void InstallCaseAppInfoListUI::OpenAppFolder()
{INTERFUN;
	bool bOpen = false;
	vector<wstring> folders;
	for (int i=0;i<_pList->GetCount();i++)
	{
		if(_pList->IsCheckItem(i))
		{
			CControlUI* item = _pList->GetItem(i);
			InstallsAppInfo* appInfo = (InstallsAppInfo* )item->GetUserData();
			if(appInfo == NULL)
				return;

			wstring filePath = appInfo->m_strSavePath;

			wstring folderPath = CFileOperation::GetFileDir(filePath);
			vector<wstring>::iterator it = find(folders.begin(), folders.end(), folderPath);
			if(it != folders.end())
				continue;

			folders.push_back(folderPath);

			if(CFileOperation::IsFileExist(filePath.c_str()))
			{
				bOpen = true;
				_pInstallCaseUI->Model.OpenDir(filePath, true);
			}
		}
	}

	if(!bOpen)
	{
		CommonUIFunc::NoticeDialog(L"软件对应的文件已经不存在，无法打开！", ICON_WARNING);
	}
}

void InstallCaseAppInfoListUI::GetSelectPathList( vector<wstring>& pathList )
{INTERFUN;
	for(int i = 0; i < _pSelectItemContainer->GetCount(); i++)
	{
		InstallCaseButtonItem* item = (InstallCaseButtonItem*)_pSelectItemContainer->GetItem(i);
		wstring path = item->GetId();
		pathList.push_back(path);
	}
}

void InstallCaseAppInfoListUI::AddDeviceMenu( CMenuUI* pMenu )
{INTERFUN;
	vector<wstring> idList = DEVICE_MANAGER->GetAllDevice();
	int size = idList.size();
	int count = 0;
	CMenuUI* pInstallMenu = pMenu->GetItem(IDR_MENU_INSTALL_APP, BY_COMMAND);
	for (int i = 0; i < size; i++)
	{
		wstring sn = idList.at(i);
		DeviceData* pDevice = DeviceManager::GetInstance()->FindDevice(sn);

		_INSTALLS_CASE_DATA_TYPE type = _INSTALLS_CASE_DATA_IOS;
		if(pDevice->GetDevInfo()->_deviceType == Android)
			type = _INSTALLS_CASE_DATA_ANDROID;

		if(type==_INSTALLS_CASE_DATA_IOS&&pDevice->GetDevInfo()->_connectType != USB)
			continue;
		if(_pCaseData->CaseType() == type)
		{
			CMenuUI* pDevMenu = new CMenuUI();
			pDevMenu->SetUID(IDR_MENU_INSTALL_APP);
			pDevMenu->GetMenuInfo().szKey = sn.c_str();
			pDevMenu->SetText(pDevice->GetDevInfo()->GetDeviceName().c_str());
			pInstallMenu->Insert(pDevMenu);
			count++;
		}
	}
	if(count <= 0)
		pMenu->Remove(IDR_MENU_INSTALL_APP, BY_COMMAND);
}

void InstallCaseAppInfoListUI::AddCaseMenu( CMenuUI* pMenu )
{INTERFUN;
	CMenuUI* pSubMenu = pMenu->GetItem(IDR_MENU_ADD_TO_CASE, BY_COMMAND);
	if(_pCaseData->GetAppNum() > 0)
	{
		vector<InstallsCaseData*> caseDataList = _pInstallCaseUI->Model.GetCaseDataList();
		for(size_t i=0;i< caseDataList.size();i++)
		{
			InstallsCaseData *caseData = caseDataList.at(i);
			if(caseData->CaseType() == _pCaseData->CaseType() && caseData->PlanType() == _INSTALL_PLAN_TYPE_USER)
			{
				CMenuUI* MenuCase = new CMenuUI();
				MenuCase->SetUID(IDR_MENU_ADD_TO_NEW_CASE+caseData->Id());
				MenuCase->SetText(caseData->CaseName().c_str());
				pSubMenu->Insert(MenuCase);
			}
		}
	}
	CMenuUI* pSubADDMenu = pMenu->GetItem(IDR_MENU_ADD_TO_CASE, BY_COMMAND);
	if(pSubADDMenu->GetCount() == 2)
	{
		pSubADDMenu->Remove(1,BY_POSITION);
	}
}

bool InstallCaseAppInfoListUI::Handle(const DeviceConnectMsg* pMsg)
{
LOG->WriteDebug(L"OnUsbConnect");
	this->OnUsbConnect(pMsg->deviceID);
	return true;
}

bool InstallCaseAppInfoListUI::Handle(const DeviceLinkOutMsg* pMsg)
{
	this->OnUsbDisconnect(pMsg->id);
	return true;
}

bool InstallCaseAppInfoListUI::Handle(const DeviceActivatedMsg* pMsg)
{
	if (!pMsg->bActived)
	{
		this->OnDeviceActivateFail(pMsg->deviceID);
	}
	return true;
}

bool InstallCaseAppInfoListUI::Handle(const ResponseDeviceNameMsg* pMsg)
{
	this->OnDeviceNameChanged(pMsg->deviceID);
	return true;
}


void InstallCaseAppInfoListUI::OnUsbConnect( wstring sn )
{INTERFUN;
	if(_pComboBoxDevice == NULL)
		return;

	DeviceData* pDevice = DeviceManager::GetInstance()->FindDevice(sn);
	_INSTALLS_CASE_DATA_TYPE type = _INSTALLS_CASE_DATA_IOS;
	if(!pDevice)
		return;
	if(pDevice->GetDevInfo()->_deviceType == Android)
		type = _INSTALLS_CASE_DATA_ANDROID;

	if(type==_INSTALLS_CASE_DATA_IOS && pDevice->GetDevInfo()->_connectType != USB)
		return;
	for (int i = 0; i < _pComboBoxDevice->GetCount(); i++)
	{
		CControlUI* item = _pComboBoxDevice->GetItem(i);
		wstring id = item->GetId();
		if( id == sn)
		{
			return;
		}
	}

	if(_pCaseData->CaseType() == type)
	{
		CListItemUI* item = new CListItemUI();
		item->SetId(sn.c_str());
		item->SetText(pDevice->GetDevInfo()->GetDeviceName().c_str());	
		item->SetUserData((UINT)pDevice);
		_pComboBoxDevice->Add(item);
		_pComboBoxDevice->SetUserData((UINT)pDevice);
		_pComboBoxDevice->SelectItem(item);
	}
}

void InstallCaseAppInfoListUI::OnUsbDisconnect( wstring sn )
{INTERFUN;
LOG->WriteDebug(L"OnUsbDisconnect");
	RemoveDevice(sn);
}

void InstallCaseAppInfoListUI::OnDeviceActivateFail( wstring sn )
{
	LOG->WriteDebug(L"OnDeviceActivateFail");
	RemoveDevice(sn);
}

void InstallCaseAppInfoListUI:: RemoveDevice(wstring sn)
{
	for (int i = 0; i < _pComboBoxDevice->GetCount(); i++)
	{
		CControlUI* item = _pComboBoxDevice->GetItem(i);
		wstring id = item->GetId();
		if( id == sn)
		{
			_pComboBoxDevice->Remove(item);
LOG->WriteDebug(L"_pComboBoxDevice->Remove(item)");
			break;
		}
	}
	if(_pComboBoxDevice->GetCount() > 0)
	{
		_pComboBoxDevice->SetUserData(_pComboBoxDevice->GetItem(0)->GetUserData());
		_pComboBoxDevice->SelectItem(_pComboBoxDevice->GetItem(0));
	}
	else
	{
		_pComboBoxDevice->SetUserData(0);
		_pComboBoxDevice->SetText(CommonUIFunc::LoadStringW(L"InstallCase_Text_DeviceNotLink"));
	}

}

bool InstallCaseAppInfoListUI::IsLastVerChecked()
{ INTERFUN;
	return _pShowLastVerCheckBox->IsChecked(); 
}

void InstallCaseAppInfoListUI::OnDeviceNameChanged( wstring sn )
{INTERFUN;
	for (int i = 0; i < _pComboBoxDevice->GetCount(); i++)
	{
		CControlUI* item = _pComboBoxDevice->GetItem(i);
		wstring id = item->GetId();
		if( id == sn)
		{
			DeviceData* pDevice = DeviceManager::GetInstance()->FindDevice(sn);
			if(pDevice == NULL)
				return;

			item->SetText(pDevice->GetDevInfo()->GetDeviceName().c_str());
			if(0!=_pComboBoxDevice->GetUserData())
			{
				DeviceData* tempdevice = (DeviceData*)_pComboBoxDevice->GetUserData();
				if(tempdevice->GetId() == pDevice->GetId())
				{
					_pComboBoxDevice->SetText(item->GetText());
				}
			}
			break;
		}
	}
}

int InstallCaseAppInfoListUI::GetAppInfoItemsCount()
{INTERFUN;
	return _pList->GetCount();
}

bool InstallCaseAppInfoListUI::OnDeviceComboChecked( TNotifyUI* pNotify )
{INTERFUN;
	CListItemUI* pListItem = (CListItemUI*)_pComboBoxDevice->GetCurSel();
	_pComboBoxDevice->SetUserData(pListItem->GetUserData());
	return true;
}

bool InstallCaseAppInfoListUI::OnUpgradeBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	if(pNotify->pSender->GetParent()->GetParent() == NULL)
		return true;
	CSimpleListItemUI* item = (CSimpleListItemUI*)pNotify->pSender->GetParent()->GetParent();
	InstallsAppInfo* info = _pCaseData->GetAppBySavePath(item->GetId());
	if(info)
	{
		vector<wstring> pathList;
		pathList.push_back(info->m_strSavePath);
		_pInstallCaseUI->Model.UpgrateApp(_pCaseData->Id(), &pathList);
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_Task_Added_to_list")), ICON_SUCCESS);
		pNotify->pSender->SetEnabled(false);
	}
	return true;
}

bool InstallCaseAppInfoListUI::OnLegalUpgradeBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	if(pNotify->pSender->GetParent()->GetParent() == NULL)
		return true;
	CSimpleListItemUI* item = (CSimpleListItemUI*)pNotify->pSender->GetParent()->GetParent();
	InstallsAppInfo* info = _pCaseData->GetAppBySavePath(item->GetId());
	if(info)
	{
		vector<wstring> pathList;
		pathList.push_back(info->m_strSavePath);
		_pInstallCaseUI->Model.UpgradeAppToLegal(_pCaseData->Id(), &pathList);
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_Task_Added_to_list")), ICON_SUCCESS);
		pNotify->pSender->SetEnabled(false);
	}
	return true;
}

bool InstallCaseAppInfoListUI::OnUpgradeAllAppsBtnClick( TNotifyUI* pNotify )
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
			_pInstallCaseUI->Model.UpgrateApp(_pCaseData->Id(), &pathList);
			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_Task_Added_to_list")), ICON_SUCCESS);
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
			_pInstallCaseUI->Model.UpgradeAppToLegal(_pCaseData->Id(), &pathLegalList);
			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_Task_Added_to_list")), ICON_SUCCESS);
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
			_pInstallCaseUI->Model.UpgradeAppToLegal(_pCaseData->Id(), &pathLegalList);
		if(pathPiracyList.size() > 0)
			_pInstallCaseUI->Model.UpgrateApp(_pCaseData->Id(), &pathPiracyList);
		if(pathLegalList.size() > 0 || pathPiracyList.size() > 0)
			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("InstallCase_Text_Task_Added_to_list")), ICON_SUCCESS);
	}
	return true;
}

bool InstallCaseAppInfoListUI::OnUpgradeAllAppsMenuBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	CMenuUI* pMenu = CMenuUI::Load(IDR_MENU_UPGRADEALL);
	CControlUI* con0 = pMenu->GetItem(0);
	CControlUI* con1 = pMenu->GetItem(1);
	CControlUI* con2 = pMenu->GetItem(2);
	pMenu->GetItem(0)->Check(_eAppUpgradeType==_APP_UPGRADE_TYPE_TOPIRACY);
	pMenu->GetItem(1)->Check(_eAppUpgradeType==_APP_UPGRADE_TYPE_TOLEGAL);
	pMenu->GetItem(2)->Check(_eAppUpgradeType==_APP_UPGRADE_TYPE_TOSAMELEGAL);
	CommonUIFunc::ShowMenu(pMenu, GetWindow()->GetHWND(), this, _pUpgradeAllBtn);
	return true;
}

void InstallCaseAppInfoListUI::FinishRefresh()
{INTERFUN;
	_pRefreshCaseBtn->SetEnabled(true);
	_pRefreshCaseBtn->GetParent()->Resize();
	_pRefreshCaseBtn->GetParent()->Invalidate();
}

bool InstallCaseAppInfoListUI::OnClickUpgradeMenu( TNotifyUI* pNotify )
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

void InstallCaseAppInfoListUI::RefreshUpgradeCount()
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
			_pUpgradeAllBtn->SetText(CommonUIFunc::LoadStringW(L"InstallCase_Text_UpgradeAllAppsNonum"));
			_pUpgradeAllBtn->SetEnabled(false);
			_pUpgradeAllBtn->SetToolTip(CommonUIFunc::LoadStringW(L"InstallCase_Text_UpgradeAllAppsNonum"));
		}
		else
		{
			_pUpgradeAllBtn->SetText(CStrOperation::ReplaceW(CommonUIFunc::LoadStringW(L"InstallCase_Text_UpgradeAllApps"), L"num", CStrOperation::IntToWString(shownum)).c_str());
			_pUpgradeAllBtn->SetEnabled(true);
			_pUpgradeAllBtn->SetToolTip(CStrOperation::ReplaceW(CommonUIFunc::LoadStringW(L"InstallCase_Text_UpgradeAllApps"), L"num", CStrOperation::IntToWString(shownum)).c_str());
		}
		_pUpgradeAllBtn->SetToolTipShow(true);
		_pUpgradeAllBtn->Resize();
		_pUpgradeAllBtn->Invalidate();
	}
	else
	{
		if(shownum==0)
		{
			_pAndUpgradeAllBtn->SetText(CommonUIFunc::LoadStringW(L"InstallCase_Text_UpgradeAllAppsNonum"));
			_pAndUpgradeAllBtn->SetEnabled(false);
				_pAndUpgradeAllBtn->SetToolTip(CommonUIFunc::LoadStringW(L"InstallCase_Text_UpgradeAllAppsNonum"));
		}
		else
		{
			_pAndUpgradeAllBtn->SetText(CStrOperation::ReplaceW(CommonUIFunc::LoadStringW(L"InstallCase_Text_UpgradeAllApps"), L"num", CStrOperation::IntToWString(shownum)).c_str());
			_pAndUpgradeAllBtn->SetEnabled(true);
			_pAndUpgradeAllBtn->SetToolTip(CStrOperation::ReplaceW(CommonUIFunc::LoadStringW(L"InstallCase_Text_UpgradeAllApps"), L"num", CStrOperation::IntToWString(shownum)).c_str());
		}
		_pAndUpgradeAllBtn->GetParent()->Resize();
		_pAndUpgradeAllBtn->GetParent()->Invalidate();
		_pAndUpgradeAllBtn->SetToolTipShow(true);
	}
}

void InstallCaseAppInfoListUI::ShowFilterInfo( bool show )
{
	if(!_pExpander->IsVisible())
	{
		if(L""==_pSearchContext->SearchKey&&NULL==_pSearchContext->Folder&&0==_pSearchContext->PhoneType&&0==_pSearchContext->State&&L""==_pSearchContext->Purchaser)
			show = false;
		if(show&&!_pFilterInfo->IsVisible())
		{
			_pFilterInfo->SetVisible(true);
			_pFilterInfo->SetInitPos(0,60,0,0, 6);
			_pList->SetInitPos(0,39,0,60, 8);
		}
		else if(!show&&_pFilterInfo->IsVisible())
		{
			_pFilterInfo->SetVisible(false);
			_pFilterInfo->SetInitPos(0,0,0,0, 6);
			_pList->SetInitPos(0,39,0,40, 8);
		}
		this->Resize();
		this->Invalidate();
		return;
	}
}

bool InstallCaseAppInfoListUI::OnCloseFilterInfoBtnClick( TNotifyUI* pNotify )
{
	ShowFilterInfo(false);
	return true;
}

INSTALLCASE_END