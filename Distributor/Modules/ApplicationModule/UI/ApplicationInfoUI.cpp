#include "stdafx.h"
#include "ApplicationInfoUI.h"
#include "Common/Log.h"
#include "Core/DeviceData.h"
#include "Common/BaseOperation.h"


App_CC_BEGIN



ApplicationInfoUI::ApplicationInfoUI(HWND hWnd, wstring id)
: _model(this)
{INTERFUN;
	_model.SetCurrentDeviceID(id);
	_pUserList          = NULL;
	_pSystemList        = NULL;
	_pTab               = NULL;
	_bHaveCheckSDCardState = false;    
	_bShowSystemAppTab  = true;
	_eAppUpgradeType=_APP_UPGRADE_TYPE_TOSAMELEGAL;
	InitializeCriticalSection(&_updateAppDataLock);
	InitializeCriticalSection(&_csListLock);

	_bDelete = false;
}

ApplicationInfoUI::~ApplicationInfoUI()
{INTERFUN;
	_bDelete = true;
	Release();
	ReleaseAppData();
}

wstring ApplicationInfoUI::GetStyleID()
{
	return L"ApplicationInfo";
}

void ApplicationInfoUI::OnClose()
{INTERFUN;
}

bool SortByName(CControlUI* pControlA, CControlUI* pControlB)
{INTERFUN;
	CContainerUI* pTempContainerA = (CContainerUI*)pControlA->GetItem(_T("Container"));
	RETURN_FALSE_IF(pTempContainerA == NULL);
	CLabelUI* pTempLabelA = (CLabelUI*)pTempContainerA->GetItem(_T("LabelAppName"));
	RETURN_FALSE_IF(pTempLabelA == NULL);
	wstring appNameA = pTempLabelA->GetText();

	CContainerUI* pTempContainerB = (CContainerUI*)pControlB->GetItem(_T("Container"));
	RETURN_FALSE_IF(pTempContainerB == NULL);
	CLabelUI* pTempLabelB = (CLabelUI*)pTempContainerB->GetItem(_T("LabelAppName"));
	RETURN_FALSE_IF(pTempLabelB == NULL);
	wstring appNameB = pTempLabelB->GetText();

	appNameA = CStrOperation::trimLeftW(appNameA, L" ");
	appNameB = CStrOperation::trimLeftW(appNameB, L" ");
	return lstrcmpi(appNameA.c_str(), appNameB.c_str()) > 0;

}

bool SortRult(CControlUI* pControlA, CControlUI* pControlB, wstring key1, wstring key2, wstring key3)
{INTERFUN;
	CLabelUI* pTempLabelA = (CLabelUI*)pControlA->GetItem(key1.c_str());
	CLabelUI* pTempLabelB = (CLabelUI*)pControlB->GetItem(key1.c_str());
	if (!pTempLabelA || !pTempLabelB)
	{
		pTempLabelA = (CLabelUI*)pControlA->GetItem(key2.c_str());
		pTempLabelB = (CLabelUI*)pControlB->GetItem(key2.c_str());
		if (!pTempLabelA || !pTempLabelB)
		{
			pTempLabelA = (CLabelUI*)pControlA->GetItem(key3.c_str());
			pTempLabelB = (CLabelUI*)pControlB->GetItem(key3.c_str());
		}
	}
	RETURN_FALSE_IF(!pTempLabelA || !pTempLabelB);

	if (key1 == L"LabelStaticDiskUsage" || key1 == L"LabelDynamicDiskUsage")
	{
		UINT sizeA = pTempLabelA->GetUserData();
		UINT sizeB = pTempLabelB->GetUserData();
		if (sizeA != 0 && sizeB != 0)
			return sizeA > sizeB;
	}

	wstring tempA = pTempLabelA->GetText();
	wstring tempB = pTempLabelB->GetText();

	return tempA.compare(tempB) > 0;
}

bool __stdcall Compare(CListItemUI* pControlA, CListItemUI* pControlB, int nCol, bool exchange)
{INTERFUN;
	RETURN_FALSE_IF(!pControlA || !pControlB);

	switch (nCol)
	{
	case 0://名称
		return SortByName(pControlA, pControlB);
		break;
	case 1:// (版本ios)(版本android user)(版本android system)
		return SortRult(pControlA, pControlB, L"VersionContainer.LabelVersion", L"VersionContainer.LabelVersion", L"");
		break;
	case 2:// (程序大小ios)(大小android user)(操作android system)
		{
			if (_tcscmp(pControlA->GetParent()->GetId(), L"SystemListCtrl") == 0)
				return !exchange;

			return SortRult(pControlA, pControlB, L"LabelStaticDiskUsage", L"LabelStaticDiskUsage", L"");
		}
		break;
	case 3:// (文档大小ios)(安装位置android)
		return SortRult(pControlA, pControlB, L"LabelDynamicDiskUsage", L"LabelInstallPos", L"");
		break;
	case 4:
		return !exchange;
		break;
	default:
		break;
	}
	return false;
}

void ApplicationInfoUI::OnCreate()
{INTERFUN;
	_pTab = (CTabUI*)GetItem(_T("AppTab"));
	_pTab->SetTabHeaderAttribute(0,_T("txt.textcolor"),_T("ffffff"));
	_pTab->SetTabHeaderAttribute(1,_T("txt.textcolor"),_T("1f547c"));
	_pTab->GetHeader()->SetAttribute(_T("bk.visible"), _T("0"));
	_pUserList = (CListUI*)GetItem(_T("AppTab.User.UserListCtrl"));
	_pSystemList = (CListUI*)GetItem(_T("AppTab.System.SystemListCtrl"));
	_pSearchEdit= (CSearchEditUI*)GetItem(_T("Search.Edit"));
	_pSearchClear= (CButtonUI*)GetItem(_T("Search.Clear"));

	_pFilterInfo = dynamic_cast<CLayoutUI*>(GetItem(_T("FilterInfo")));
	_pFilterInfoLabel = dynamic_cast<CLabelUI*>(GetItem(_T("FilterInfo.FilterInfoLabel")));
	_pClearFilterInfoBtn = dynamic_cast<CButtonUI*>(GetItem(_T("FilterInfo.ClearFilterBtn")));
	_pCloseFilterInfoBtn = dynamic_cast<CButtonUI*>(GetItem(_T("FilterInfo.CloseFilterInfoBtn")));


	_pAndUpgradeAllBtn = (CButtonUI*)GetItem(_T("DownBar.AndUpgradeAllAppsBtn"));
	_pUpgradeAllBtn	= (CButtonUI*)GetItem(_T("DownBar.Upgrade.UpgradeAllAppsBtn"));
	_pAndUpgradeAllBtn->SetEnabled(false);
	_pUpgradeAllBtn->SetEnabled(false);
	_pUpgrateAllMenuBtn	= (CButtonUI*)GetItem(_T("DownBar.Upgrade.UpgradeAllAppsMenuBtn"));
	_pUpgradeAllLayout = (CLayoutUI*)GetItem(_T("DownBar.Upgrade"));
	_pUserList->SetSortCallBack(Compare);
	_pSystemList->SetSortCallBack(Compare);
	_model.LoadAndDisplayData(_model.GetCurrentDeviceID());

}

void ApplicationInfoUI::AddListEventDelegate(CListUI* pList)
{INTERFUN;
	RETURN_IF(pList == NULL);
	for (int i = 0; i < pList->GetCount(); i++)
	{
		CControlUI* pControl = pList->GetItem(i);
		if (pControl == NULL)
			continue;

		pControl->RemoveEventDelegate();
		pControl->AddEventDelegate(&MakeDelegate(this, &ApplicationInfoUI::MoveEventOnList));
	}
}

void ApplicationInfoUI::RemoveListEventDelegate( CListUI* pList )
{INTERFUN;
	RETURN_IF(pList == NULL);
	for (int i = 0; i < pList->GetCount(); i++)
	{
		CControlUI* pControl = pList->GetItem(i);
		if (pControl == NULL)
			continue;

		pControl->RemoveEventDelegate();
	}
}

bool ApplicationInfoUI::AddListEventDelegate()
{INTERFUN;
	RETURN_TRUE_IF(_pUserList == NULL && _pSystemList == NULL);
	AddListEventDelegate(_pUserList);
	AddListEventDelegate(_pSystemList);
	return true;
}

void ApplicationInfoUI::ShowListItemButton(CControlUI* pControl, bool bShow)
{INTERFUN;
	RETURN_IF(!pControl);
	CControlUI* pContainer1 = pControl->GetItem(L"Container1"); 
	RETURN_IF(!pContainer1);
	CControlUI* pBtnUninstall = pContainer1->GetItem(L"UserButtonUninstall");

	// 屏蔽系统列表的卸载按钮
	if (pBtnUninstall == NULL)
	pBtnUninstall = pContainer1->GetItem(L"SystemButtonUninstall");


	if (pBtnUninstall)
	pBtnUninstall->SetVisible(bShow, false);


	CControlUI* pBtnMoveTo = pContainer1->GetItem(L"ButtonMoveTo");    
	if (pBtnMoveTo != NULL)
	pBtnMoveTo->SetVisible(bShow, false);

	CListContainerItemUI* pItem = dynamic_cast<CListContainerItemUI*>(pControl);
	if (pItem) pItem->ReCalcRect();

	pControl->Invalidate();
}

bool ApplicationInfoUI::MoveEventOnList(TEventUI& event)
{INTERFUN;
	CControlUI* pControl = event.pSender;
	RETURN_TRUE_IF(pControl == NULL);

	switch(event.Type)
	{
	case UIEVENT_MOUSEENTER:
		ShowListItemButton(pControl, true);
		break;
	case UIEVENT_MOUSELEAVE:
		ShowListItemButton(pControl, false);
		break;
	}
	return true;
}

void ApplicationInfoUI::OnShowComponent()
{INTERFUN;
	_model.LoadAndDisplayData(_model.GetCurrentDeviceID());
}

bool ApplicationInfoUI::OnInstallApp(TNotifyUI* pNotify)
{INTERFUN;
	// 统计功能

	CStaticsticsMessage StaticMsg("11004");
	StaticMsg.Send();

	LPCTSTR pID = pNotify->pSender->GetId();
	if (_tcscmp(pNotify->pSender->GetId(),_T("InstallBtn"))==0)
	{
		if(_model.InstallApp(true))
			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("Application_Text_APP_add_to_list")), ICON_SUCCESS);
	}
	else if (_tcscmp(pNotify->pSender->GetId(),_T("InstallBtnMenu"))==0)
	{
		CommonUIFunc::ShowMenu(MENU_ID_INSTALL, Singleton::GetMainWnd(), this, pNotify->pSender);
	}
	return true;
}

bool ApplicationInfoUI::OnMenuItemClick(TNotifyUI* pNotify)
{INTERFUN;
	RETURN_TRUE_IF(pNotify->wParam == NULL);
	bool bInstall = false;
	UIMENUINFO* pInfo = (UIMENUINFO*)pNotify->wParam;
	if (pInfo->uID == MENU_ID_INSTALL_FILE)         // 选择文件
		bInstall=_model.InstallApp(true);
	else if (pInfo->uID == MENU_ID_INSTALL_FOLDER)  // 选择文件夹
		bInstall=_model.InstallApp(false);
	else if (pInfo->uID == MENU_ID_LIST_MOVE){
		// 判断移动方向
		if (pInfo->szText == CommonUIFunc::LoadString(_T("Application_Text_APP_move_to_phone")))
			MultipleMoveToApp(false);
		else
			MultipleMoveToApp(true);
	}
	//SingleMoveTo(_appId);
	else if (pInfo->uID == MENU_ID_LIST_UNINSTALL)
	{
		MultipleUninstallApp();
		/*
		if (0 == _pTab->GetCurSel())
		UserSingleUninstallApp(_appId);
		else
		SystemSingleUninstallApp(_appId);
		*/
	}
	//if (bInstall)
	//{
	//	CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"Application_Text_HasbeenAddToTaskList"),ICON_SUCCESS);
	//}
	return true;
}


bool ApplicationInfoUI::OnListRightClickList(TNotifyUI* pNotify)
{INTERFUN;
	CListContainerItemUI* pListItem =(CListContainerItemUI*)(pNotify->lParam);
	RETURN_TRUE_IF(pListItem == NULL);
	_appId = pListItem->GetId();

	CListUI* pList = NULL;
	CMenuUI* pMenu = NULL;
	AppListBodyItemData* pItemData = NULL;
	LPCTSTR pID = pNotify->pSender->GetId();

	//没有选中项则退出
	vector<AppData*> datas = GetSelectDatas();
	if (datas.empty()) return true;

	//设置移动菜单显示
	int iPhoneCount = 0;
	int iSdCardCount = 0;
	for (size_t i = 0; i < datas.size(); i++)
	{
		AppData* pApp = datas.at(i);
		if (pApp->_bOnSDCard)
			iSdCardCount++;
		else
			iPhoneCount++;
	}

	//如果选择的应用位置都在手机内存则显示移至SD卡，如果都在SD卡则显示移至手机内存
	//混合则不显示
	wstring strMove = _T("");
	if (iSdCardCount == datas.size())
	{
		strMove = CommonUIFunc::LoadString(_T("Application_Text_APP_move_to_phone"));
	}
	else if (iPhoneCount == datas.size())
	{
		strMove = CommonUIFunc::LoadString(_T("Application_Text_APP_move_to_sd"));
	}


	if (_tcscmp(pID,_T("UserListCtrl")) == 0)
	{

		pList = _pUserList;
		if (_model.GetCurDeviceType() == Ios)
			pMenu = CMenuUI::Load(MENU_ID_ONE_LIST);
		else if (_model.GetCurDeviceType() == Android)
		{
			pMenu = CMenuUI::Load(MENU_ID_TWO_LIST);
			CMenuUI* pSubMenu = pMenu->GetItem(0);
			//判断是否存在SD卡
			if (GetSDCardState())
			{
				pItemData = FindItemData(_userItemList, _appId);
				//pSubMenu->SetText(pItemData->GetMoveToButton()->GetText());
				if (strMove.empty())
					pMenu->Remove(0);
				else
					pSubMenu->SetText(strMove.c_str());

			}
			else
				pMenu->Remove(0);



		}
	}
	else if (_tcscmp(pID,_T("SystemListCtrl")) == 0)
	{
		//return true; // 屏蔽系统应用菜单
		pList = _pSystemList;
		pMenu = CMenuUI::Load(MENU_ID_ONE_LIST);
	}

	RETURN_TRUE_IF(pList->GetCount() == 0);
	pMenu->Show(Singleton::GetMainWnd(), NULL, (INotifyUI*)this);
	return true;
}

bool ApplicationInfoUI::OnMultipleUninstallApp(TNotifyUI* pNotify)
{INTERFUN;
	// 统计功能

	CStaticsticsMessage StaticMsg("11005");
	StaticMsg.Send();

	return MultipleUninstallApp();
}

AppListBodyItemData* ApplicationInfoUI::FindItemData(vector<AppListBodyItemData*> itemList, wstring id)
{INTERFUN;
	AppListBodyItemData* pCurSel = NULL;
	for (size_t i = 0; i < itemList.size(); i++)
	{
		AppListBodyItemData* pItem = itemList.at(i);
		if (pItem->GetId() != id)
			continue;

		pCurSel = pItem;
		break;
	}
	return pCurSel;
}

bool ApplicationInfoUI::OnUserSingleUninstallApp(TNotifyUI* pNotify)
{INTERFUN;
	// 统计功能

	CStaticsticsMessage StaticMsg("11006");
	StaticMsg.Send();

	_appId = pNotify->pSender->GetParent()->GetParent()->GetId();
	UserSingleUninstallApp(_appId);
	return true;
}

void ApplicationInfoUI::UserSingleUninstallApp(wstring appId)
{INTERFUN;
	AppListBodyItemData* pCurSel = FindItemData(_userItemList, appId);
	RETURN_IF(pCurSel == NULL);

	AppData* pApp = (AppData*)pCurSel->GetAppData();
	RETURN_IF( pApp == NULL );

	wstring text = CommonUIFunc::LoadString(_T("Application_Text_APP_is_del_app"))+pCurSel->GetAppData()->_AppName;
	wstring strIconPath = _model.GetIconDir()+L"icon_"+pCurSel->GetAppData()->_packageName+L".png";
	if (CommonUIFunc::ConfirmDialog(text, CommonUIFunc::LoadString(_T("Application_Text_APP_uninstall")), strIconPath))
	{
		if (_bDelete)
		{
			return;
		}
		// 再获取一次，防止对话框一直不关闭
		pCurSel = FindItemData(_userItemList, appId);
		RETURN_IF(!pCurSel);
		_model.UnInstallApp(pApp);
	}
}

bool ApplicationInfoUI::OnSystemSingleUninstallApp(TNotifyUI* pNotify)
{INTERFUN;
	// 统计功能

	CStaticsticsMessage StaticMsg("11006");
	StaticMsg.Send();

	_appId = pNotify->pSender->GetParent()->GetParent()->GetId();
	SystemSingleUninstallApp(_appId);
	return true;
}

void ApplicationInfoUI::SystemSingleUninstallApp(wstring appId)
{INTERFUN;
	AppListBodyItemData* pCurSel = FindItemData(_systemItemList, appId);
	RETURN_IF(pCurSel == NULL);

	AppData* pApp = (AppData*)pCurSel->GetAppData();
	RETURN_IF( pApp == NULL );

	wstring text = CommonUIFunc::LoadString(_T("Application_Text_APP_is_del_app"))+pCurSel->GetAppData()->_AppName;
	wstring strIconPath = _model.GetIconDir()+L"icon_"+pCurSel->GetAppData()->_packageName+L".png";
	if (_model.IsRootDevice())
	{
		if (CommonUIFunc::ConfirmDialog(CommonUIFunc::LoadString(_T("Application_Text_APP_root_uninstall_tip")), CommonUIFunc::LoadString(_T("Application_Text_APP_uninstall")), L"#CommonUi_icon_TipWarning", 260))
		{
			if (_bDelete)
			{
				return;
			}
			// 再获取一次，防止对话框一直不关闭
			pCurSel = FindItemData(_systemItemList, appId);
			RETURN_IF(!pCurSel);
			if (this->IsNotShowSystemrAppAndroid(pApp))
			{
				CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("Application_Text_Not_Uninstall_Tips")));
			}
			else
				_model.UnInstallApp(pApp);
		}
	}
	else
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(_T("Application_Text_APP_noroot_uninstall_tip")), CommonUIFunc::LoadString(_T("Application_Text_APP_uninstall")), L"#CommonUi_icon_TipWarning");
}


bool ApplicationInfoUI::OnSingleMoveTo(TNotifyUI* pNotify)
{INTERFUN;
	_appId = pNotify->pSender->GetParent()->GetParent()->GetId();
	SingleMoveTo(_appId);
	return true;
}

void ApplicationInfoUI::SingleMoveTo(wstring appId)
{INTERFUN;
	AppListBodyItemData* pCurSel = FindItemData(_userItemList, appId);
	RETURN_IF(pCurSel == NULL);

	const AppData* pApp = pCurSel->GetAppData();
	RETURN_IF(pApp == NULL);
	if(MOUNTED!=_model.GetSDCardState())
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadStringW(_T("Application_Text_No_Sdcard_CANT_MOVE")), ICON_WARNING);
		return;
	}

	if ( !pApp->_bOnSDCard )
	{
		// 统计功能
		CStaticsticsMessage StaticMsg(Statistic_ID_AppMoveToSDcard);
		StaticMsg.Send();
	}
	else
	{
		// 统计功能
		CStaticsticsMessage StaticMsg(Statistic_ID_AppMoveToPhone);
		StaticMsg.Send();
	}

	wstring text = CommonUIFunc::LoadString(_T("Application_Text_APP_is_move_app"))+pCurSel->GetAppData()->_AppName;
	wstring strIconPath = _model.GetIconDir()+L"icon_"+pCurSel->GetAppData()->_packageName+L".png";
	if (CommonUIFunc::ConfirmDialog(text, CommonUIFunc::LoadString(_T("Application_Text_APP_move")), strIconPath))
	{
		if (_bDelete)
		{
			return;
		}
		_model.MoveToApp((AppData*)pCurSel->GetAppData());
	}

}

bool ApplicationInfoUI::MultipleUninstallApp()
{INTERFUN;
	vector<AppData*> datas = GetSelectDatas();
	if (datas.empty())
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("Application_Text_APP_no_change")), ICON_WARNING);
		return true;
	}

	AppData* pApp = datas.at(0);
	if (pApp == NULL) return false;

	wchar_t name[128] = {0};
	wsprintf(name, L" %s ",pApp->_AppName.c_str());
	wstring strname = name;
	wchar_t size[128] = {0};
	wsprintf(size, L" %d ",datas.size());
	wstring strsize = size;
	wstring text = CommonUIFunc::LoadString(_T("Application_Text_APP_is_del_app"))+strname+CommonUIFunc::LoadString(_T("Application_Text_APP_and"))+strsize+CommonUIFunc::LoadString(_T("Application_Text_APP_apps"));
	wstring strIconPath = _model.GetIconDir()+L"icon_"+pApp->_packageName+L".png";
	if (pApp->_eAppOwner == APP_OWNER_SYS)
	{
		//return true; // 过滤系统应用卸载
		if (_model.IsRootDevice())
		{
			if (CommonUIFunc::ConfirmDialog(CommonUIFunc::LoadString(_T("Application_Text_APP_root_uninstall_tip")), CommonUIFunc::LoadString(_T("Application_Text_APP_uninstall")), L"#CommonUi_icon_TipWarning", 260))
			{
				if (_bDelete)
				{
					return true;
				}
				datas = GetSelectDatas();
				bool bShowTips = false;
				for (vector<AppData*>::iterator iter = datas.begin(); iter != datas.end();)
				{
					if (this->IsNotShowSystemrAppAndroid(*iter))
					{
						iter = datas.erase(iter);
						bShowTips = true;
					}
					else
						++iter;
				}
				if (bShowTips)
				{
					CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("Application_Text_Not_Uninstall_Tips")));
				}

				_model.UnInstallApp(&datas);
			}
		}
		else
		{
			CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(_T("Application_Text_APP_noroot_uninstall_tip")), CommonUIFunc::LoadString(_T("Application_Text_APP_uninstall")), L"#CommonUi_icon_TipWarning");
			return true;
		}
	}
	else if (pApp->_eAppOwner == APP_OWNER_USER)
	{
		if (CommonUIFunc::ConfirmDialog(text, CommonUIFunc::LoadString(_T("Application_Text_APP_uninstall")), strIconPath))
		{
			if (_bDelete)
			{
				return true;
			}
			datas = GetSelectDatas();
			_model.UnInstallApp(&datas);
		}
	}

	return true;
}


bool ApplicationInfoUI::MultipleMoveToApp(bool bToSDCard)
{INTERFUN;
	if(bToSDCard&&MOUNTED!=_model.GetSDCardState())
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadStringW(_T("Application_Text_No_Sdcard_CANT_MOVE")), ICON_WARNING);
		return false;
	}
	vector<AppData*> datas = GetSelectDatas();
	if (datas.empty())
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("Application_Text_APP_no_change_move")), ICON_WARNING);
		return true;
	}

	for(vector<AppData*>::iterator it = datas.begin(); it != datas.end();)
	{
		AppData *pApp = *it;

		if (pApp == NULL) continue;

		if (bToSDCard && pApp->_bOnSDCard)
			it = datas.erase(it);
		else if (!bToSDCard && !pApp->_bOnSDCard)
			it = datas.erase(it);
		else
			it++;
	}

	if (datas.empty())
	{
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(_T("Application_Text_APP_rechange")), CommonUIFunc::LoadString(_T("Application_Text_APP_move")));
		return true;
	}

	AppData* pApp = datas.at(0);
	if (pApp == NULL) return true;

	wchar_t name[128] = {0};
	wsprintf(name, L" %s ",pApp->_AppName.c_str());
	wstring strname = name;
	wchar_t size[128] = {0};
	wsprintf(size, L" %d ",datas.size());
	wstring strsize = size;
	wstring text = CommonUIFunc::LoadString(_T("Application_Text_APP_is_move_app"))+strname+CommonUIFunc::LoadString(_T("Application_Text_APP_and"))+strsize+CommonUIFunc::LoadString(_T("Application_Text_APP_apps"));
	wstring strIconPath = _model.GetIconDir()+L"icon_"+pApp->_packageName+L".png";
	if (CommonUIFunc::ConfirmDialog(text, CommonUIFunc::LoadString(_T("Application_Text_APP_move")), strIconPath))
	{
		if (_bDelete)
		{
			return true;
		}
		for (size_t i = 0; i < datas.size(); i++)
			_model.MoveToApp(datas.at(i));
	}

	return true;
}

bool ApplicationInfoUI::OnMultipleMoveToSD(TNotifyUI* pNotify)
{INTERFUN;
	// 统计功能
	
	CStaticsticsMessage StaticMsg(Statistic_ID_LotAppMoveToSDcard);
	StaticMsg.Send();
	return MultipleMoveToApp(true);
}

bool ApplicationInfoUI::OnMultipleMoveToPhone(TNotifyUI* pNotify)
{INTERFUN;
	// 统计功能
	CStaticsticsMessage StaticMsg(Statistic_ID_LotAppMoveToPhone);
	StaticMsg.Send();
	return MultipleMoveToApp(false);
}

void ApplicationInfoUI::ClearData()
{INTERFUN;
	AppThreadMsg AppMsg;
	AppMsg.message = WM_APP_CLEARLIST;
	AppMsg.wParam  = (WPARAM)this;
	AppMsg.lParam  = (LPARAM)0;
	AppMsg.SendToUI();

}

void ApplicationInfoUI::ClearDataAction( bool bUser )
{INTERFUN;

	SetCSLock(_csListLock);

	// 是否列表项数据
	vector<AppListBodyItemData*>::iterator itVec = _userItemList.begin();
	if ( bUser )
	{
		RemoveListEventDelegate(_pUserList);
		
		while (itVec != _userItemList.end())
		{
			AppListBodyItemData* pItem = *itVec;
			SAFE_DELETE( pItem );
			itVec++;
		}

		_userItemList.clear();
		_pUserList->RemoveAll();
		_pUserList->Resize();
		_pUserList->Invalidate();

	}
	else
	{
		RemoveListEventDelegate(_pSystemList);

		itVec = _systemItemList.begin();
		while (itVec != _systemItemList.end())
		{
			AppListBodyItemData* pItem = *itVec;
			SAFE_DELETE( pItem );
			itVec++;
		}

		_systemItemList.clear();
		_pSystemList->RemoveAll();
		_pSystemList->Resize();
		_pSystemList->Invalidate();
	}

}

vector<AppData*> ApplicationInfoUI::GetSelectList(CListUI* pList, 
												  vector<AppListBodyItemData*>* _pItemList)
{INTERFUN;
	vector<AppData*> datas;
	for (int i = 0; i < pList->GetCount(); i++)
	{
		if (!pList->IsCheckItem(i))
			continue;

		wstring id = pList->GetItem(i)->GetId();
		for (size_t i = 0; i < _pItemList->size(); i++)
		{
			AppListBodyItemData* pCurSel = _pItemList->at(i);
			if (pCurSel->GetId() != id)
				continue;

			datas.push_back((AppData*)pCurSel->GetAppData());
			break;
		}
	}
	return datas;
}

vector<AppData*> ApplicationInfoUI::GetSelectDatas()
{INTERFUN;
	if (0 == _pTab->GetCurSel())
	return GetSelectList(_pUserList, &_userItemList);
	else
	return GetSelectList(_pSystemList, &_systemItemList);
}

void ApplicationInfoUI::SetControlAttribute()
{INTERFUN;
	if (_model.GetCurDeviceType() == Ios)
	{
		_pUserList->SetHeaderText(1, CommonUIFunc::LoadString(_T("Application_Text_APP_version_header")));
		_pUserList->SetHeaderText(2, CommonUIFunc::LoadString(_T("Application_Text_APP_StaticDiskUsage")));
		_pUserList->SetHeaderText(3, CommonUIFunc::LoadString(_T("Application_Text_APP_DynamicDiskUsage")));

		//_pTab->SetHeaderItemVisible(1, false);
		_bShowSystemAppTab = false;
		_pTab->SetTabHeaderAttribute(1,_T("text"),_T(""));
		_pTab->SetTabHeaderAttribute(1,_T("icon.image"), _T(""));
		_pTab->SetTabHeaderAttribute(1,_T("enable"),_T("0"));
		_pTab->SelectItem(0);
		_pTab->SetTabHeaderAttribute(0,_T("txt.textcolor"),_T("ffffff"));
		_pTab->SetTabHeaderAttribute(1,_T("bk.visible"), _T("0"));
		if(_model.IsJailBreak())
		{
			_pAndUpgradeAllBtn->SetVisible(false);
			_pUpgradeAllLayout->SetVisible(true);
		}
		else
		{
			_pAndUpgradeAllBtn->SetVisible(true);
			_pUpgradeAllLayout->SetVisible(false);
		}
	}
	else if (_model.GetCurDeviceType() == Android && 0 == _pTab->GetCurSel())
	{
		_pUserList->SetHeaderText(1, CommonUIFunc::LoadString(_T("Application_Text_APP_version_header")));
		_pUserList->SetHeaderText(2, CommonUIFunc::LoadString(_T("Application_Text_APP_size_header")));
		_pUserList->SetHeaderText(3, CommonUIFunc::LoadString(_T("Application_Text_APP_pos_header")));

		//_pTab->SetHeaderItemVisible(1, true);
		_bShowSystemAppTab = true;
		_pTab->SetTabHeaderAttribute(1,_T("text"),CommonUIFunc::LoadString(_T("Application_Text_APP_system_app")));
		//_pTab->SetTabHeaderAttribute(1,_T("icon.image"), _T("#icon_System"));
		_pTab->SetTabHeaderAttribute(1,_T("enable"),_T("1"));
		if (GetSDCardState())
		{
			GetItem(_T("DownBar.MoveToPhone"))->SetVisible(true);
			GetItem(_T("DownBar.MoveToSD"))->SetVisible(true);
		}
		_pAndUpgradeAllBtn->SetVisible(true);
		_pUpgradeAllLayout->SetVisible(false);
		bool and = _pAndUpgradeAllBtn->IsVisible();
		bool iws = _pUpgradeAllLayout->IsVisible();
		ShowFilterInfo(false);
	}
	_pTab->Resize();
	_pTab->Invalidate();
}

void ApplicationInfoUI::ShowLoadingPicture(bool bShow)
{INTERFUN;
	GetItem(_T("Loading"))->SetVisible(bShow);
	GetItem(_T("LoadingText"))->SetVisible(bShow);
	this->Resize();
	this->Invalidate();
}

bool ApplicationInfoUI::OnAppTabChange(TNotifyUI* pNotify)
{INTERFUN;
	_pTab->SetTabHeaderAttribute(0,_T("txt.textcolor"),_T("1f547c"));
	_pTab->SetTabHeaderAttribute(1,_T("txt.textcolor"),_T("1f547c"));

	_pTab->SetTabHeaderAttribute(_pTab->GetCurSel(),_T("txt.textcolor"),_T("ffffff"));

	CControlUI* pTem = NULL;
	if (0 == _pTab->GetCurSel())
	{
		// 统计功能
		CStaticsticsMessage StaticMsg("11002");
		StaticMsg.Send();

		RefreshUnstallBtn(true);
		RETURN_TRUE_IF(_model.GetCurDeviceType() == Ios);

		RefreshMoveBtn(_hasSDCard);

		CButtonUI *pButton = (CButtonUI*)GetItem(_T("DownBar.Uninstall"));
		if (pButton)
			pButton->SetInitPos(150,7,230,32,0);

		if(!_pSearchEdit->IsEmpty())
		{
			ClearDataAction(false);
			DEVICE_OS_TYPE deviceType = _model.GetCurDeviceType();
			CreateTempListItem( _systemAppDatas, L"", deviceType, true );	
			OnSearchClear(NULL);
		}


	}
	else
	{
		// 统计功能

		CStaticsticsMessage StaticMsg("11003");
		StaticMsg.Send();

		RefreshMoveBtn(false);
		RefreshUnstallBtn(false); // 屏蔽系统应用


		CButtonUI *pButton = (CButtonUI*)GetItem(_T("DownBar.Uninstall"));
		if (pButton)
			pButton->SetInitPos(20,7,112,32,0);

		if(!_pSearchEdit->IsEmpty())
		{
			ClearDataAction(true);
			DEVICE_OS_TYPE deviceType = _model.GetCurDeviceType();
			CreateTempListItem( _userAppDatas, L"", deviceType, true );	
			OnSearchClear(NULL);
		}
			
	}
	this->Resize();
	this->Invalidate();

	return true;
}

void ApplicationInfoUI::InsertToAppViewAction( const vector<AppData*>* pDatas )
{INTERFUN;
	SetCSLock(_csListLock);

	RETURN_IF(!pDatas);

	DEVICE_OS_TYPE deviceType = _model.GetCurDeviceType();
	for (size_t i = 0; i < pDatas->size(); i++)
	{
		AppData* pApp = pDatas->at(i);
		if (pApp == NULL) continue;
		if (pApp->_packageName.empty()) continue;
		if (pApp->_eAppOwner == APP_OWNER_SYS)
		{
			CreateListBodyItemSystem(pApp, deviceType, true);
		}
		else
		{
			CreateListBodyItemUser(pApp, deviceType, true);
		}
	}

	//Tab按钮上显示应用程序数量
	ShowTabAppCount();
	AddListEventDelegate();
	// 刷新
	if (_pUserList)
	{
		_pUserList->Resize();
		_pUserList->Invalidate();
	}
	if (_pSystemList)
	{
		_pSystemList->Resize();
		_pSystemList->Invalidate();
	}

}

//@zjf --解决debug下，ios手机经常插上去会崩溃问题
//原因：由于其他模块用到传进去的pDatas变量，原本存放这个数据的容器里面地址已经被改变，导致pDatas失效
//解决办法：建立临时变量，由自己主动调用者释放指针避免内存泄露
void ApplicationInfoUI::BuildeTempData(const vector<AppData*>* pDatas, vector<AppData*>& vecTempData)
{

	for (size_t i = 0; i < pDatas->size(); i++)
	{
		AppData* pApp = pDatas->at(i);
		if (pApp == NULL) continue;
		AppData* pTemp = new AppData(pApp);
		if (pTemp == NULL) continue;
		vecTempData.push_back(pTemp);
	}

}

void ApplicationInfoUI::ClearTempData(vector<AppData*>& vecTempData)
{
	for (size_t i = 0; i < vecTempData.size(); i++)
	{
		SAFE_DELETE(vecTempData[i]);
	}
	vecTempData.clear();
}


void ApplicationInfoUI::InsertToAppView(const vector<AppData*>* pDatas)
{INTERFUN;
	AppThreadMsg appMsg;
	appMsg.message = WM_APP_ADDLIST;
	appMsg.wParam  = (WPARAM)this;
	appMsg.lParam  = (LPARAM)pDatas;
	appMsg.SendToUI();

}

void ApplicationInfoUI::CreateListBodyItemSystem(AppData* pApp, DEVICE_OS_TYPE deviceType, bool bSave)
{INTERFUN;
	if (FindItemData(_systemItemList, pApp->_packageName))
	{
		return ;
	}
	if (deviceType == Android)
	{
		//RETURN_IF(IsNotShowSystemrAppAndroid(pApp));
		AppListBodyItemData* item = CreateListBodyItem(_pSystemList, pApp, true);
		_systemItemList.push_back(item);
		if ( bSave )
		{
			AppData* pData = new AppData( pApp );
			_systemAppDatas.push_back(pData);
		}
		

	}
}

bool ApplicationInfoUI::IsNotShowSystemrAppAndroid(AppData* pApp)
{INTERFUN;
	wstring buf;
	if (pApp->_packageName.find(_T("com.andiroid")) != -1)
	return true;
	else if (pApp->_packageName.find(_T("com.google")) != -1)
	return true;

	return false;
}

void ApplicationInfoUI::CreateListBodyItemUser(AppData* pApp, DEVICE_OS_TYPE deviceType, bool bSave)
{INTERFUN;
	RETURN_IF(!pApp);
	if (FindItemData(_userItemList, pApp->_packageName))
	{
		return ;
	}
	
	if (deviceType == Android)
	{
		RETURN_IF(IsNotShowUserAppAndroid(pApp));
		AppListBodyItemData* item = CreateListBodyItem(_pUserList, pApp, true);
		_userItemList.push_back(item);
		if ( bSave )
		{
			if (IsExisAppData(_userAppDatas,pApp->_packageName))
			{
				return;
			}
			AppData* pData = new AppData( pApp );
			_userAppDatas.push_back(pData);
		}
		
	}
	else
	{
		RETURN_IF(IsNotShowUserAppIos(pApp));
		AppListBodyItemData* item = CreateListBodyItem(_pUserList, pApp, false);
		_userItemList.push_back(item);
		if ( bSave )
		{
			if (IsExisAppData(_userAppDatas,pApp->_packageName))
			{
				return;
			}
			AppData* pData = new AppData( pApp );
			_userAppDatas.push_back(pData);
		}
		
	}
}

bool ApplicationInfoUI::IsNotShowUserAppIos(AppData* pApp)
{INTERFUN;
	if (pApp->_eAppType == APP_TYPE_DEB)
		return true;

	return false;
}

bool ApplicationInfoUI::IsNotShowUserAppAndroid(AppData* pApp)
{INTERFUN;
	if (pApp->_packageName == L"com.nd.assistance")//91手机助手
		return true;

	return false;
}

AppListBodyItemData* ApplicationInfoUI::CreateListBodyItem(CListUI* pList, 
														   AppData* pApp, 
														   bool isAndroid)
{INTERFUN;
	AppListBodyItemData* bodyItem = new AppListBodyItemData(pList,
															pApp,
															isAndroid,
															GetSDCardState(),
															this);
	return bodyItem;
}

void ApplicationInfoUI::RemoveFromAppView( AppData* pApp )
{INTERFUN;
	AppThreadMsg appMsg;
	appMsg.message = WM_APP_REMOVE;
	appMsg.wParam  = (WPARAM)this;
	appMsg.lParam  = (LPARAM)pApp;
	appMsg.SendToUI();

}

void ApplicationInfoUI::RemoveFromAppViewAction( AppData* pApp )
{INTERFUN;
	SetCSLock(_csListLock);
	RETURN_IF(!pApp);
	if (pApp->_eAppOwner == APP_OWNER_SYS)
	{
		RemoveFromList(pApp, _pSystemList, _systemItemList);
		RemoveFromTempListData(pApp, _systemAppDatas);
	}
	else
	{
		RemoveFromList(pApp, _pUserList, _userItemList);
		RemoveFromTempListData(pApp, _userAppDatas);
	}

	ShowTabAppCount();
}



void ApplicationInfoUI::RemoveFromList(AppData* pApp, 
									   CListUI* pList,
									   vector<AppListBodyItemData*>& itemList)
{INTERFUN;
	RETURN_IF(!pApp);
	vector<AppListBodyItemData*>::iterator itVec = itemList.begin();
	while (itVec != itemList.end())
	{
		AppListBodyItemData* pItem = *itVec;
		const AppData* ItemData = pItem->GetAppData();
		if (!ItemData) continue;
		if (ItemData->_packageName == pApp->_packageName)
		{
			pList->Remove(pItem->GetContainerItemUI());
			pList->Resize();
			pList->Invalidate();

			itemList.erase(itVec);
			SAFE_DELETE(pItem);
			break;
		}
		else
			++itVec;
	}

	if (pList->GetCount() == 0)
	pList->CheckItem(false);
	AddListEventDelegate();
}

void ApplicationInfoUI::RemoveFromTempListData( AppData* pApp, vector<AppData*>& verDatas )
{
	vector<AppData*>::iterator it = verDatas.begin();
	while ( it != verDatas.end() )
	{
		AppData* p = *it;
		if ( p->_AppName == pApp->_AppName )
		{
			verDatas.erase(it);
			SAFE_DELETE(p);
			break;
		}
		else
			++it;
	}
}

void ApplicationInfoUI::SetAppLogo( AppData* pApp, const DeviceData* pCurDevice )
{INTERFUN;
	AppListBodyItemData* pData = NULL;
	RETURN_IF( !pApp );
	if (pApp->_eAppOwner == APP_OWNER_SYS)
	{
		pData = FindItemData(_systemItemList, pApp->_packageName);
	}
	else
	{
		if (_model.GetCurDeviceType() == Ios
			&& IsNotShowUserAppIos(pApp))
			return;

		if (_model.GetCurDeviceType() == Android
			&& IsNotShowUserAppAndroid(pApp))
			return;

		pData = FindItemData(_userItemList, pApp->_packageName);
	}

	if (pData == NULL) return;
	wstring strLogoPath = _model.GetIconDir()+L"icon_"+pApp->_packageName+L".png";

	if (CFileOperation::IsFileExist(strLogoPath.c_str()))

	{
		pData->GetIconControl()->SetAttribute(_T("bk.image"), strLogoPath.c_str());
		pApp->_strIconPCPath = strLogoPath;
		// 图标路径保存到项的appdata中
		pData->SetAppDataIconPCPath(strLogoPath);
	}
	else
	{
		if (CFileOperation::IsFileExist(pApp->_strIconPCPath.c_str()))	// 安装应用的时候，会被赋值
		{
			pData->GetIconControl()->SetAttribute(_T("bk.image"), pApp->_strIconPCPath.c_str());
		}
		else
		{
			pData->GetIconControl()->SetAttribute(_T("bk.image"), _T("#InstallCase_Image_Appicon_Default_32"));
		}
	}

	pData->GetIconControl()->Resize();
	pData->GetIconControl()->Invalidate();
}

void ApplicationInfoUI::SetAppLogo(const vector<AppData*>* pDatas, const DeviceData* pCurDevice)
{INTERFUN;
	vector<AppData*> vecTempData;
	this->BuildeTempData(pDatas, vecTempData);

	LOG->WriteDebug(L"*************************************** SetAppLogo");
	for (size_t i = 0; i < vecTempData.size(); i++)
	{
		AppData* pApp = vecTempData.at(i);
		if (pApp == NULL) continue;
		// 检查是否存在
		wstring strLogoPath = _model.GetIconDir()+L"icon_"+pApp->_packageName+L".png";

		if (!CFileOperation::IsFileExist(strLogoPath.c_str()))
			pApp->LoadAppIcon(pCurDevice, strLogoPath); //不管是否加载成功都设置应用程序图标，没有则使用默认

		AppThreadMsg appMsg;
		appMsg.message = WM_APP_SETLOGO;
		appMsg.wParam  = (WPARAM)this;
		appMsg.lParam  = (LPARAM)pApp;
		appMsg.SendToUI();

	}

	this->ClearTempData(vecTempData);
}

void ApplicationInfoUI::SetUpgradeBtn( AppDataUpgrade* pApp, const DeviceData* pCurDevice )
{
	AppListBodyItemData* pData = NULL;
	RETURN_IF( !pApp );
	pData = FindItemData(_userItemList, pApp->_packageName);

	if (pData == NULL) 
		return;

	CButtonUI* pPiracyUpBtn = pData->GetPiracyUpButton();
	if(pPiracyUpBtn)
	{
		pPiracyUpBtn->SetVisible(true);
		pPiracyUpBtn->SetUserData((UINT)pApp);
		wstring tooltip = CommonUIFunc::LoadStringW(_T("Application_Text_UpgradeTo")) + pApp->PiracyNewVersion();
		pPiracyUpBtn->SetToolTip(tooltip.c_str());
	}
}

void ApplicationInfoUI::SetUpgradeBtnEnabled( AppDataUpgrade* pApp, bool enabled )
{
	AppListBodyItemData* pData = NULL;
	RETURN_IF( !pApp );
	pData = FindItemData(_userItemList, pApp->_packageName);

	if (pData == NULL) 
		return;

	CButtonUI* pPiracyUpBtn = pData->GetPiracyUpButton();
	if(pPiracyUpBtn && pPiracyUpBtn->IsVisible())
	{
		pPiracyUpBtn->SetEnabled(true);
	}

	CButtonUI* pLegalUpBtn = pData->GetLegalUpButton();
	if(pLegalUpBtn && pLegalUpBtn->IsVisible())
	{
		pLegalUpBtn->SetEnabled(true);
	}
}

void ApplicationInfoUI::SetLegalUpgradeBtn( AppDataUpgrade* pApp, const DeviceData* pCurDevice )
{
	AppListBodyItemData* pData = NULL;
	RETURN_IF( !pApp );
	pData = FindItemData(_userItemList, pApp->_packageName);

	if (pData == NULL) 
		return;

	CButtonUI* pLegalUpBtn = pData->GetLegalUpButton();
	if(pLegalUpBtn)
	{
		pLegalUpBtn->SetVisible(true);
		pLegalUpBtn->SetUserData((UINT)pApp);
		wstring tooltip = CommonUIFunc::LoadStringW(_T("Application_Text_UpgradeTo")) + pApp->LegalNewVersion();
		pLegalUpBtn->SetToolTip(tooltip.c_str());
	}
}

void ApplicationInfoUI::SetAllUpgradeBtn( bool enabled )
{
	if(!_model.IsJailBreak())
		_pAndUpgradeAllBtn->SetEnabled(enabled);
}

void ApplicationInfoUI::SetAppSizeAction( AppData* pApp, const DeviceData* pCurDevice )
{
	AppListBodyItemData* pData = NULL;
	RETURN_IF( !pApp );
	if (pApp->_eAppOwner == APP_OWNER_SYS)
	{
		pData = FindItemData(_systemItemList, pApp->_packageName);
	}
	else
	{
		pData = FindItemData(_userItemList, pApp->_packageName);
	}

	if (pData == NULL) return;

	wstring staticDiskUsage = pApp->_staticDiskUsage == 0 ? CommonUIFunc::LoadString(_T("Application_Text_APP_unknow")):CStrOperation::FormatFileSize(pApp->_staticDiskUsage);
	wstring dynamicDiskUsage = pApp->_dynamicDiskUsage == 0 ? CommonUIFunc::LoadString(_T("Application_Text_APP_unknow")):CStrOperation::FormatFileSize(pApp->_dynamicDiskUsage);

	CLabelUI* pDynamicDiskLabel = pData->GetDynamicDiskLabel();
	if (pDynamicDiskLabel)
	{
		pDynamicDiskLabel->SetText(dynamicDiskUsage.c_str());
		pDynamicDiskLabel->SetUserData((UINT)pApp->_dynamicDiskUsage);
	}

	CLabelUI* pStaticDiskLabel = pData->GetStaticDiskLabel();
	if (pStaticDiskLabel)
	{
		pStaticDiskLabel->SetText(staticDiskUsage.c_str());
		pStaticDiskLabel->SetUserData((UINT)pApp->_staticDiskUsage);
	}
	//ios应用程序由于大小会变化，所以重写tooltip
	pData->SetToolTipText(pApp,false,pData->GetContainerItemUI());
}

void ApplicationInfoUI::SetAppSize( const vector<AppData*>* pDatas, const DeviceData* pCurDevice )
{
	LOG->WriteDebug(L"*************************************** SetAppSize");
	for (size_t i = 0; i < pDatas->size(); i++)
	{
		AppData* pApp = pDatas->at(i);
		if (pApp == NULL) continue;

		AppThreadMsg appMsg;
		appMsg.message = WM_APP_SETSIZE;
		appMsg.wParam  = (WPARAM)this;
		appMsg.lParam  = (LPARAM)pApp;
		appMsg.SendToUI();

	}
	_pUserList->Resize();
	_pUserList->Invalidate();
}

void ApplicationInfoUI::SetUpgradeBtn(const vector<AppDataUpgrade*>* pDatas, const DeviceData* pCurDevice)
{INTERFUN;
	LOG->WriteDebug(L"*************************************** SetUpgradeBtn");

	AppThreadMsg appMsg;
	appMsg.message = WM_APP_REFRESHUPAPPSCOUNT;
	appMsg.wParam  = (WPARAM)this;
	appMsg.lParam  = (LPARAM)NULL;
	appMsg.SendToUI();

	for (size_t i = 0; i < pDatas->size(); i++)
	{
		AppDataUpgrade* pApp = pDatas->at(i);
		if (pApp == NULL) continue;

		AppThreadMsg appMsg;
		appMsg.message = WM_APP_SETUPGRADEBTN;
		appMsg.wParam  = (WPARAM)this;
		appMsg.lParam  = (LPARAM)pApp;
		appMsg.SendToUI();

	}
}

void ApplicationInfoUI::SetLegalUpgradeBtn(const vector<AppDataUpgrade*>* pDatas, const DeviceData* pCurDevice)
{INTERFUN;
	LOG->WriteDebug(L"*************************************** SetLegalUpgradeBtn");
	AppThreadMsg appMsg;
	appMsg.message = WM_APP_REFRESHUPAPPSCOUNT;
	appMsg.wParam  = (WPARAM)this;
	appMsg.lParam  = (LPARAM)NULL;
	appMsg.SendToUI();

	for (size_t i = 0; i < pDatas->size(); i++)
	{
		AppDataUpgrade* pApp = pDatas->at(i);
		if (pApp == NULL) continue;

		AppThreadMsg appMsg;
		appMsg.message = WM_APP_SETLEGALUPGRADEBTN;
		appMsg.wParam  = (WPARAM)this;
		appMsg.lParam  = (LPARAM)pApp;
		appMsg.SendToUI();
		
	}
}

void ApplicationInfoUI::GetAppFilePath(vector<wstring>& file)
{
	_model.GetAppFilePath(file);
}
bool ApplicationInfoUI::InstallApp(bool isFile, wstring wsTitle)
{INTERFUN;
	return _model.InstallApp(isFile, wsTitle);
}

bool ApplicationInfoUI::InstallApp( vector<wstring> files )
{INTERFUN;
	return _model.InstallApp(files);
}

AppListBodyItemData* ApplicationInfoUI::GetItemData(AppData* pApp, vector<AppListBodyItemData*> itemList)
{INTERFUN;
	AppListBodyItemData* pData = NULL;
	for (size_t i=0; i<itemList.size(); i++)
	{
		AppListBodyItemData* pItem = itemList.at(i);
		if (pItem->GetAppData() != pApp)
			continue;

		pData = pItem;
		break;
	}
	return pData;
}

bool  ApplicationInfoUI::ListFocused()
{INTERFUN;
	CControlUI* pControl = this->GetWindow()->GetFocus();
	while ( pControl )
	{
		if ( pControl == _pUserList )
			return true;
		if ( pControl == _pSystemList )
			return true;

		pControl = pControl->GetParent();
	}
	return false;
}

// Summary  : Tab按钮上应用程序的数量
// Returns  : void 
void ApplicationInfoUI::ShowTabAppCount()
{INTERFUN;
	if (_pTab)
	{
		TCHAR strTitle[256] = {0};
		wsprintf(strTitle, _T("%s(%d)"), CommonUIFunc::LoadString(_T("Application_Text_APP_user_app")), _userItemList.size());
		_pTab->SetTabHeaderAttribute(0,_T("text"), strTitle);        
		if (_bShowSystemAppTab)
		{
			wsprintf(strTitle, _T("%s(%d)"), CommonUIFunc::LoadString(_T("Application_Text_APP_system_app")), _systemItemList.size());
			_pTab->SetTabHeaderAttribute(1,_T("text"), strTitle);
		}     
		_pTab->Invalidate();
	}
}

//加载应用线程发送消息处理
bool ApplicationInfoUI::HandleAppMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
	ApplicationInfoUI* pUI = (ApplicationInfoUI*)wParam;    
	if (this == pUI)
	{
		if (message == WM_APP_CLEARLIST)
		{
			pUI->ClearDataAction();	
			pUI->ClearDataAction(false);
			pUI->ClearUpdateDate();
		}
		else if (message == WM_APP_ADDLIST)
		{
			const vector<AppData*>* pDatas = (const vector<AppData*>*)lParam;

			//vector<AppData*> vecTempData;
			//this->BuildeTempData(pDatas, vecTempData);
			////先进行排序
			//sort(vecTempData.begin(), vecTempData.end(), sortByName);

			pUI->InsertToAppViewAction(pDatas);

			//this->ClearTempData(vecTempData);
		}
		else if (message == WM_APP_SETLOGO)
		{
			DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_model.GetCurrentDeviceID());
			pUI->SetAppLogo((AppData*)lParam, pDevice);
		}
		else if (message == WM_APP_SETSIZE)
		{
			DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_model.GetCurrentDeviceID());
			pUI->SetAppSizeAction((AppData*)lParam, pDevice);
			SetSearchData((AppData*)lParam);
		}
		else if (message == WM_APP_REFRESHLINE)
		{
			AppListBodyItemData* pData = (AppListBodyItemData*)wParam;
			if (pData)
				pData->RefreshLine();
		}
		else if (message == WM_APP_UPDATEDATA)
		{
			pUI->UpdateDataAction();
			pUI->ClearUpdateDate();
		}
		else if (message == WM_APP_REMOVE)
		{
			AppData* pApp = (AppData*)lParam;
			pUI->RemoveFromAppViewAction(pApp);
		}
		else if (message == WM_APP_SETAttribute)
		{
			this->SetControlAttribute();
		}
		else if(message == WM_APP_SETUPGRADEBTN)
		{
			DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_model.GetCurrentDeviceID());
			pUI->SetUpgradeBtn((AppDataUpgrade*)lParam, pDevice);
		}
		else if(message == WM_APP_SETLEGALUPGRADEBTN)
		{
			DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_model.GetCurrentDeviceID());
			pUI->SetLegalUpgradeBtn((AppDataUpgrade*)lParam, pDevice);
		}
		else if(message == WM_APP_REFRESHUPAPPSCOUNT)
		{
			pUI->RefreshUpgradeCount();
		}
	}

	if (message == WM_APP_REFRESHAPPDATA)//刷新设备已经安装的软件
	{
		wchar_t* id = (wchar_t*)wParam;  
		wstring deviceId(id);
		if (_model.GetCurrentDeviceID()==deviceId )
		{
			_model.RefreshData();
		}
	}

	return true;
}

//处理消息中转中心的消息
bool ApplicationInfoUI::HandleCenterMsg(int  nParam)
{
	RETURN_TRUE_IF(!ListFocused());
	switch(nParam)
	{
	case VK_F5:
		RefreshData();
		break;
	case VK_DELETE:
	case VK_DECIMAL:
		OnMultipleUninstallApp(NULL);
		break;
	}

	return true;
}

bool ApplicationInfoUI::HandleTaskItemMsg(void* pTask, UINT message, wstring curID ,int taskid)
{

	if(WM_TASK_FINISHED == message)
	{
		CTaskItem* item = (CTaskItem*)pTask;
		if (NULL == item)
		{
			return true;
		}
		_model.TaskFinished(item, curID);
		RETURN_TRUE_IF(curID.empty() || curID != _model.GetCurrentDeviceID());
		if (item->appData)
		{

			// 获取程序大小
			if (item->appData->_staticDiskUsage == 0)
			{ // 去获取大小
				wstring strPath = item->m_csFileSavePath;
				item->appData->_staticDiskUsage = (int64_t)CFileOperation::GetFileSizeW(strPath);
			}
			AppData* newAppData = new AppData( item->appData );
			newAppData->_eAppOwner = APP_OWNER_USER;
			if ( newAppData->_eAppType == APP_TYPE_UNKNOW )
			{
				wstring ext = CFileOperation::GetFileExt(newAppData->_strAppPhonePath);
				if ( ext == L"apk" )
				{
					newAppData->_eAppType = APP_TYPE_APK;
				}
				else if ( ext == L"npk" )
				{
					newAppData->_eAppType = APP_TYPE_NPK;
				}
			}
			if (_model.GetLoadingState() && !_model.GetLoadState())
				//如果此时应用程序列表正在加载，则先保存，加载线程会调用_pUI->UpdateData;
				AddUpdateData(newAppData);
			else
				RefreshAppInfo(newAppData, item->m_uTaskType==MOVEANDROIDAPP); // 只更新安装/转移的应用

			RequestDeviceSpaceMsg spaceMsg;
			spaceMsg.deviceID = _model.GetCurrentDeviceID();
			spaceMsg.Send();
		}
		else
			_model.RefreshData(); // 刷新全部

	}
	else if (WM_TASK_REMOVED == message)
	{
		_model.TaskRemove(taskid, curID);
	}

	return true;
}

bool ApplicationInfoUI::HandleRefreshModuleMsg(wstring deviceID)
{
	if (deviceID == _model.GetCurrentDeviceID() && this->IsVisible())
	{
		this->RefreshData();
	}

	return true;
}

void ApplicationInfoUI::RefreshAppInfo( AppData* pData, bool isMoveAndroidApp )
{INTERFUN;
	RETURN_IF(!pData);
	AppListBodyItemData* pCurSel = FindItemData(_userItemList, pData->_packageName);
	if (!pCurSel)
		pCurSel = FindItemData(_systemItemList, pData->_packageName);

	if (pCurSel)
	{ // 更新
		pCurSel->UpdateInfo(pData, isMoveAndroidApp);
		_pUserList->CheckItem(true, pCurSel->GetContainerItemUI()->GetIndex());
	}
	else
	{ // 插入
		vector<AppData*> list;
		list.push_back(pData);
		InsertToAppView(&list);
		_pUserList->CheckItem(true, _pUserList->GetCount() - 1);
		SetAppLogo(&list, _model.FindCurDevice());
	}
}

void ApplicationInfoUI::RefreshData()
{INTERFUN;

	// 正在卸载直接退出
	RETURN_IF (_model.GetUninstallState());
	// 正在加载直接退出
	RETURN_IF (_model.GetLoadingState());

	_hasSDCard = _model.GetSDCardState() == MOUNTED;
	if (_model.GetCurDeviceType() != Ios)
	RefreshMoveBtn(GetSDCardState() == true);
	if (1 == _pTab->GetCurSel())
	{
		RefreshMoveBtn(false);
		//	RefreshUnstallBtn(false);   // 屏蔽系统应用
	}
	else
	RefreshUnstallBtn(true);
	
	OnSearchClear(NULL);
	ReleaseAppData();
	_model.RefreshData(); 

}

//更新移动按钮的状态
void ApplicationInfoUI::RefreshMoveBtn( bool bShow )
{INTERFUN;
	CControlUI* pTem = NULL;
	pTem = GetItem(_T("DownBar.MoveToPhone"));
	if (pTem != NULL) 
	pTem->SetVisible(bShow);

	pTem = GetItem(_T("DownBar.MoveToSD"));
	if (pTem != NULL)
	pTem->SetVisible(bShow);	
}

void ApplicationInfoUI::UpdateData()
{INTERFUN;
	int nSize = 0;
	{
		SetCSLock(_updateAppDataLock);
		nSize = _updateAppDatas.size();
	}
	if (nSize > 0)
	{
		AppThreadMsg appMsg;
		appMsg.message = WM_APP_UPDATEDATA;
		appMsg.wParam  = (WPARAM)this;
		appMsg.lParam  = (LPARAM)0;
		appMsg.SendToUI();
	}

}

void ApplicationInfoUI::UpdateDataAction()
{INTERFUN;
	SetCSLock(_updateAppDataLock);
	for (size_t i =0; i < _updateAppDatas.size(); i++)
	{
		AppData* appData = _updateAppDatas.at(i);
		if (appData == NULL) continue;
		RefreshAppInfo(appData);
	}	
}

// 添加需要更新的应用数据
void ApplicationInfoUI::AddUpdateData( AppData* pApp )
{INTERFUN;
	SetCSLock(_updateAppDataLock);
	_updateAppDatas.push_back(pApp);	
}

void ApplicationInfoUI::Release()
{INTERFUN;
	_bDelete = true;
}

void ApplicationInfoUI::RefreshUnstallBtn(bool bShow)
{INTERFUN;
	CControlUI* pTem = NULL;
	pTem = GetItem(_T("DownBar.InstallBtn"));
	if (pTem != NULL) 
	pTem->SetVisible(bShow);

	pTem = GetItem(_T("DownBar.InstallBtnMenu"));
	if (pTem != NULL) 
	pTem->SetVisible(bShow);


	if (_model.GetCurDeviceType() == Ios && _model.IsJailBreak())
	{
		_pUpgradeAllLayout->SetVisible(bShow);
	}
	else
	{
		_pAndUpgradeAllBtn->SetVisible(bShow);
	}
	bool b = _pUpgradeAllLayout->IsVisible();
}

void ApplicationInfoUI::ClearUpdateDate()
{
	SetCSLock(_updateAppDataLock);
	_updateAppDatas.clear();	
}

bool ApplicationInfoUI::GetSDCardState()
{
	if (!_bHaveCheckSDCardState)
	{
		_hasSDCard = _model.GetSDCardState() == MOUNTED;
		_bHaveCheckSDCardState = true;
	}
	return _hasSDCard;
}

void ApplicationInfoUI::SendToUI( int nMessage )
{
	AppThreadMsg AppMsg;
	AppMsg.message = nMessage;
	AppMsg.wParam  = (WPARAM)this;
	AppMsg.lParam  = (LPARAM)0;
	AppMsg.SendToUI();

}

bool ApplicationInfoUI::OnUpgradeBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	if(pNotify->pSender == NULL)
	return true;
	CButtonUI* pBtn = (CButtonUI*)pNotify->pSender;
	//pBtn->SetEnabled(false);
	AppDataUpgrade* pApp = (AppDataUpgrade*)pBtn->GetUserData();
	if(pApp)
	{
		_model.UpgradeSinglePiracyAppData(pApp);
		_appId = pNotify->pSender->GetParent()->GetParent()->GetId();
		AppListBodyItemData* data = FindItemData(_userItemList, _appId);
		data->EnabledLegalUpBtn(false);
		data->EnabledPiracyUpBtn(false);
		CControlUI* control = pBtn->GetParent()->GetParent()->GetParent();
		control->Resize();
		control->Invalidate();
	}
	return true;
}

bool ApplicationInfoUI::OnLegalUpgradeBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	if(pNotify->pSender == NULL)
		return true;
	CButtonUI* pBtn = (CButtonUI*)pNotify->pSender;
	AppDataUpgrade* pApp = (AppDataUpgrade*)pBtn->GetUserData();
	if(pApp)
	{
		_model.UpgradeSingleLegalAppData(pApp);
		_appId = pNotify->pSender->GetParent()->GetParent()->GetId();
		AppListBodyItemData* data = FindItemData(_userItemList, _appId);
		data->EnabledLegalUpBtn(false);
		data->EnabledPiracyUpBtn(false);
		CControlUI* control = pBtn->GetParent()->GetParent()->GetParent();
		control->Resize();
		control->Invalidate();
	}
	return true;
}

bool ApplicationInfoUI::OnUpgradeAllAppsBtnClick( TNotifyUI* pNotify )
{INTERFUN;
	_model.UpgradeAllAppData(_eAppUpgradeType);
	if(_pAndUpgradeAllBtn->IsVisible())
		_pAndUpgradeAllBtn->SetEnabled(false);
	//_pUpgradeAllBtn->SetEnabled(false);
	//_pAndUpgradeAllBtn->SetEnabled(false);
	//_pUpgrateAllMenuBtn->SetEnabled(false);
	for (size_t i = 0; i < _userItemList.size(); i++)
	{
		AppListBodyItemData* pItem = _userItemList.at(i);
		const AppData* appdata = pItem->GetAppData();
		if(_eAppUpgradeType==_APP_UPGRADE_TYPE_TOPIRACY)
		{
			if(pItem->GetPiracyUpVisible())
			{
				pItem->EnabledPiracyUpBtn(false);
				if(pItem->GetLegalUpVisible())
					pItem->EnabledLegalUpBtn(false);
			}
		}
		else if(_eAppUpgradeType==_APP_UPGRADE_TYPE_TOLEGAL)
		{
			if(pItem->GetLegalUpVisible())
			{
				pItem->EnabledLegalUpBtn(false);
				if(pItem->GetPiracyUpVisible())
					pItem->EnabledPiracyUpBtn(false);
			}
		}
		else
		{
			if(pItem->GetPiracyUpVisible() && 0==appdata->_isLegalSoft)
			{
				pItem->EnabledPiracyUpBtn(false);
				if(pItem->GetLegalUpVisible())
					pItem->EnabledLegalUpBtn(false);
			}
			if(pItem->GetLegalUpVisible() && 1==appdata->_isLegalSoft)
			{
				pItem->EnabledLegalUpBtn(false);
				if(pItem->GetPiracyUpVisible())
					pItem->EnabledPiracyUpBtn(false);
			}
		}
	}
	_pUserList->Resize();
	_pUserList->Invalidate();
	return true;
}

bool ApplicationInfoUI::OnUpgradeAllAppsMenuBtnClick( TNotifyUI* pNotify )
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

bool ApplicationInfoUI::OnClickUpgradeMenu( TNotifyUI* pNotify )
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

void ApplicationInfoUI::RefreshUpgradeCount()
{
	int num = 0;
	switch(_eAppUpgradeType)
	{
	case _APP_UPGRADE_TYPE_TOPIRACY:
		num = _model.GetPiracyUpgradeAppsCount();
		break;
	case _APP_UPGRADE_TYPE_TOLEGAL:
		num = _model.GetLegalUpgradeAppsCount();
		break;
	case _APP_UPGRADE_TYPE_TOSAMELEGAL:
		num = _model.GetSameUpgradeAppsCount();
		break;
	}
	if (_model.GetCurDeviceType() == Ios && _model.IsJailBreak())
	{
		_pUpgradeAllBtn->SetEnabled(num>0);
		_pUpgradeAllBtn->SetText(CStrOperation::ReplaceW(CommonUIFunc::LoadStringW(L"Application_Text_UpgradeAllApps"), L"num", CStrOperation::IntToWString(num)).c_str());
		_pUpgradeAllBtn->Resize();
		_pUpgradeAllBtn->Invalidate();
	}
	else
	{
		_pAndUpgradeAllBtn->SetEnabled(num>0);
		_pAndUpgradeAllBtn->SetText(CStrOperation::ReplaceW(CommonUIFunc::LoadStringW(L"Application_Text_UpgradeAllApps"), L"num", CStrOperation::IntToWString(num)).c_str());
		_pAndUpgradeAllBtn->Resize();
		_pAndUpgradeAllBtn->Invalidate();
	}
	//if(_pAndUpgradeAllBtn->IsVisible())
	//{

	//}
	//bool b =_pAndUpgradeAllBtn->IsVisible();
	//if(_pUpgradeAllLayout->IsVisible())
	//{

	//}
	//bool a = _pUpgradeAllLayout->IsVisible();
}

bool ApplicationInfoUI::OnSearch(TNotifyUI* pNotify)
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

LOG->WriteDebug(L"ClearDataAction---清空列表");

	ClearDataAction( 0 == _pTab->GetCurSel() );

	DEVICE_OS_TYPE deviceType = _model.GetCurDeviceType();
	if ( text.empty() || text == L"" )
		CreateTempListItem( 0 == _pTab->GetCurSel() ? _userAppDatas : _systemAppDatas, text, deviceType, true );
	else
		CreateTempListItem( 0 == _pTab->GetCurSel() ? _userAppDatas : _systemAppDatas, text, deviceType, false );

LOG->WriteDebug(L"CreateTempListItem---创建列表");

	ShowFilterInfo( true );
	
	return true;
}

bool ApplicationInfoUI::OnSearchClear( TNotifyUI* pNotify )
{INTERFUN;
	_pSearchEdit->SetText(_T(""));
	_pSearchEdit->Resize();
	_pSearchEdit->Invalidate();
	_pSearchEdit->HideSearch();
	//_pSearchContext->SearchKey = L"";
	_pSearchClear->SetVisible(false);
	//this->OnFilter();
	return true;

}

void ApplicationInfoUI::CreateTempListItem( vector<AppData*>& verDatas, wstring strText, DEVICE_OS_TYPE deviceType, bool bLoad )
{
	vector<AppData*> verTempDatas;
	verTempDatas.clear();
	if ( bLoad )
	{
		verTempDatas = verDatas;
	}
	else
	{
		for ( int i=0;i<(int)verDatas.size();i++ )
		{
			AppData* pdata = verDatas[i];
			if ( !pdata ) continue;

			if ( (int)CStrOperation::toLower(CCodeOperation::UnicodeToGB2312(pdata->_AppName)).find(CStrOperation::toLower(CCodeOperation::UnicodeToGB2312(strText))) >= 0 )
				verTempDatas.push_back(pdata);
		}
	}

	for ( int i=0;i<(int)verTempDatas.size();i++ )
	{
		AppData* pApp = verTempDatas[i];
		if (!pApp) continue;
		if (pApp->_packageName.empty()) continue;
		
		if ( pApp->_eAppOwner == APP_OWNER_USER )
			CreateListBodyItemUser( pApp, deviceType, false );
		else
			CreateListBodyItemSystem(pApp, deviceType, false);


		SetAppSizeAction(pApp, _model.FindCurDevice());
		SetUpgradeBtn(_model.GetAppDataUpgrade(), _model.FindCurDevice());
		SetLegalUpgradeBtn(_model.GetLegalAppDataUpgrade(), _model.FindCurDevice());
	}

	//Tab按钮上显示应用程序数量
	ShowTabAppCount();
	_pFilterInfoLabel->SetText(CStrOperation::IntToWString(verTempDatas.size()).c_str());
	AddListEventDelegate();
	// 刷新
	if (_pUserList)
	{
		_pUserList->Resize();
		_pUserList->Invalidate();
	}
	if (_pSystemList)
	{
		_pSystemList->Resize();
		_pSystemList->Invalidate();
	}

	SetAppLogo(&verTempDatas, _model.FindCurDevice());
}

void ApplicationInfoUI::ShowFilterInfo( bool show )
{
	wstring text=_pSearchEdit->GetText();
	//if(!_pExpander->IsVisible())
	{
		if(text.empty())
			show = false;
		if(show&&!_pFilterInfo->IsVisible())
		{
			_pFilterInfo->SetVisible(true);
			_pFilterInfo->SetInitPos(0,60,0,0, 6);
			//_pList->SetInitPos(0,39,0,60, 8);
		}
		else if(!show&&_pFilterInfo->IsVisible())
		{
			_pFilterInfo->SetVisible(false);
			_pFilterInfo->SetInitPos(0,0,0,0, 6);
			//_pList->SetInitPos(0,39,0,40, 8);
		}
		this->Resize();
		this->Invalidate();
		return;
	}
}

bool ApplicationInfoUI::OnClearFilterBtnClick( TNotifyUI* pNotify )
{INTERFUN;

	_model.LoadAndDisplayData(_model.GetCurrentDeviceID());
	if(_pSearchEdit->IsEmpty())
	{
		ClearDataAction(0 == _pTab->GetCurSel());
		DEVICE_OS_TYPE deviceType = _model.GetCurDeviceType();
		CreateTempListItem( 0 == _pTab->GetCurSel() ? _userAppDatas : _systemAppDatas, L"", deviceType, true );	
	}
	else
		OnSearchClear(NULL);

	return true;
}

bool ApplicationInfoUI::OnCloseFilterInfoBtnClick( TNotifyUI* pNotify )
{
	ShowFilterInfo(false);
	return true;
}

void ApplicationInfoUI::ReleaseAppData()
{
	for ( int i=0;i<(int)_userAppDatas.size();i++ )
	{
		AppData* p = _userAppDatas[i];
		if ( p ) SAFE_DELETE(p);
	}

	for ( int i=0;i<(int)_systemAppDatas.size();i++ )
	{
		AppData* p = _systemAppDatas[i];
		if ( p ) SAFE_DELETE(p);
	}

	_userAppDatas.clear();
	_systemAppDatas.clear();
}

void ApplicationInfoUI::SetSearchData( AppData* pdata )
{
	if ( pdata->_eAppOwner == APP_OWNER_USER )
	{
		for ( int i=0;i<(int)_userAppDatas.size();i++ )
		{
			AppData* p = _userAppDatas[i];
			if ( pdata->_packageName == p->_packageName )
			{
				p->_dynamicDiskUsage = pdata->_dynamicDiskUsage;
				p->_staticDiskUsage  = pdata->_staticDiskUsage;
				break;
			}
		}
	}
	else
	{
		for ( int i=0;i<(int)_systemAppDatas.size();i++ )
		{
			AppData* p = _systemAppDatas[i];
			if ( pdata->_packageName == p->_packageName || pdata->_itemId == p->_itemId )
			{
				p->_dynamicDiskUsage = pdata->_dynamicDiskUsage;
				p->_staticDiskUsage  = pdata->_staticDiskUsage;
				break;
			}
		}
	}
}

void ApplicationInfoUI::SetAppLogoWithoutLoad( const vector<AppData*>* pDatas, const DeviceData* pCurDevice )
{
	if (NULL == pDatas)
	{
		return;
	}
	LOG->WriteDebug(L"*************************************** SetAppLogoWithoutLoad");
	for (size_t i = 0; i < pDatas->size(); i++)
	{
		AppData* pApp = pDatas->at(i);
		if (pApp == NULL) continue;

		AppThreadMsg appMsg;
		appMsg.message = WM_APP_SETLOGO;
		appMsg.wParam  = (WPARAM)this;
		appMsg.lParam  = (LPARAM)pApp;
		appMsg.SendToUI();

	}

}

bool ApplicationInfoUI::IsExisAppData( vector<AppData*> appDataList, wstring id )
{
	bool bResult = false;
	for (size_t i = 0; i < appDataList.size(); i++)
	{
		AppData* pItem = appDataList.at(i);
		if (pItem->_packageName == id)
		{
			bResult = true;
			break;
		}
	}
	return bResult;
}


void ApplicationInfoUI::InstallCaseLogic(vector<wstring>* files_case, wstring filePath)
{
	if(CFileOperation::IsDirExist(filePath.c_str()))
	{
		vector<wstring> subFiles;
		CFileOperation::FindAllFileInDir(filePath, subFiles, _T("ipa"));
		files_case->insert(files_case->end(),subFiles.begin(), subFiles.end());
		subFiles.clear();
		CFileOperation::FindAllFileInDir(filePath, subFiles, _T("pxl"));
		files_case->insert(files_case->end(),subFiles.begin(), subFiles.end());
		subFiles.clear();
		CFileOperation::FindAllFileInDir(filePath, subFiles, _T("apk"));
		files_case->insert(files_case->end(),subFiles.begin(), subFiles.end());
		subFiles.clear();
		CFileOperation::FindAllFileInDir(filePath, subFiles, _T("npk"));
		files_case->insert(files_case->end(),subFiles.begin(), subFiles.end());
	}
	else
	{
		files_case->push_back(wstring(filePath));
	}
}

bool ApplicationInfoUI::DropFilesHandle( vector<wstring> files)
{
	vector<wstring>* files_case = new vector<wstring>();
	for (vector<wstring>::size_type i = 0; i < files.size(); ++i)
	{
		//装机方案拖拽逻辑移至此处
		this->InstallCaseLogic(files_case, files[i]);
	}

	vector<wstring> vec_Files;
	for (size_t i=0;i<files_case->size();i++)
	{
		//以下为安装逻辑
		//查找可以用作本设备安装的程序
		wstring strFilePath = wstring(files_case->at(i));
		wstring strExt = CStrOperation::toLowerW(CFileOperation::GetFileExt(files_case->at(i)));

		if (_model.GetCurDeviceType() == Ios && (strExt == L"pxl" || strExt == L"ipa")
			|| _model.GetCurDeviceType() == Android && (strExt == L"apk" || strExt == L"npk"))
		{
			vec_Files.push_back(files_case->at(i));
		}
	}


	if (vec_Files.size() > 0)
	{
		this->InstallApp(vec_Files);
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("Application_Text_APP_add_to_list")), ICON_SUCCESS);
	}

	if (files_case->size()>1)
	{
		vector<wstring> files;
		files.insert(files.end(), files_case->begin(), files_case->end());		

		AddCaseMsg addCaseMsg;
		addCaseMsg.pPathList = (void*)&files;
		addCaseMsg.SendToUI();
	}

	return true;
}

App_CC_END