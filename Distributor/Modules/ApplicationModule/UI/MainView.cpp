#include "stdafx.h"
#include "MainView.h"
#include "Logic/ApplicationController.h"

MainView::MainView(ApplicationController* pController)
{
	_pController = pController;
}

MainView::~MainView()
{

}

std::wstring MainView::GetStyleID()
{
	return L"Application_View_Main";
}

void MainView::OnCreate()
{
	CTabUI* pTab = (CTabUI*)GetItem(_T("AppTab"));
	if(pTab != NULL)
	{
		pTab->SetTabHeaderAttribute(0,_T("txt.textcolor"),_T("ffffff"));
		pTab->SetTabHeaderAttribute(1,_T("txt.textcolor"),_T("1f547c"));
		pTab->GetHeader()->SetAttribute(_T("bk.visible"), _T("0"));

		RequestDeviceStateMsg msg;
		msg.deviceID = this->_pController->GetDeviceID();
		msg.SendAndGetReturn();
		
		if(msg.nOsType == ePhoneOS_Ios)
		{
			pTab->SetTabHeaderAttribute(1,_T("text"),_T(""));
			pTab->SetTabHeaderAttribute(1,_T("icon.image"), _T(""));
			pTab->SetTabHeaderAttribute(1,_T("enable"),_T("0"));
			pTab->SetTabHeaderAttribute(1,_T("bk.visible"), _T("0"));
		}

		CButtonUI* pAndUpgradeAllBtn = (CButtonUI*)GetItem(_T("DownBar.AndUpgradeAllAppsBtn"));
		CLayoutUI* pUpgradeAllLayout = (CLayoutUI*)GetItem(_T("DownBar.Upgrade"));
		
		if(pAndUpgradeAllBtn != NULL && pUpgradeAllLayout != NULL)
		{
			if(msg.bIsJailBreak)
			{
				pAndUpgradeAllBtn->SetVisible(false);
				pUpgradeAllLayout->SetVisible(true);
			}
			else
			{
				pAndUpgradeAllBtn->SetVisible(true);
				pUpgradeAllLayout->SetVisible(false);
			}
		}
	}

	RefreshMoveBtn(false);
}

void MainView::Release()
{

}

void MainView::UpdateSDCardState( bool bHaveSDCard /*= false*/ )
{
	this->RefreshMoveBtn(bHaveSDCard);
}

bool MainView::OnAppTabChange( TNotifyUI* pNotify )
{
	CTabUI* pTab = (CTabUI*)GetItem(_T("AppTab"));
	CSearchEditUI* pEdit = (CSearchEditUI*)GetItem(_T("Search.Edit"));

	if(pTab == NULL || pEdit == NULL)
	{
		return true;
	}
	
	pTab->SetTabHeaderAttribute(0,_T("txt.textcolor"),_T("1f547c"));
	pTab->SetTabHeaderAttribute(1,_T("txt.textcolor"),_T("1f547c"));
	pTab->SetTabHeaderAttribute(pTab->GetCurSel(),_T("txt.textcolor"),_T("ffffff"));
	if (0 == pTab->GetCurSel())
	{
		RefreshUnstallBtn(true);

		RequestDeviceStateMsg msg;
		msg.deviceID = this->_pController->GetDeviceID();
		msg.SendAndGetReturn();

		RETURN_TRUE_IF(msg.nOsType == ePhoneOS_Ios);

		CButtonUI *pButton = (CButtonUI*)GetItem(_T("DownBar.Uninstall"));
		if (pButton)
			pButton->SetInitPos(150,7,230,32,0);

		if(!pEdit->IsEmpty())
		{
			OnSearchClear(NULL);
		}
	}
	else
	{
		RefreshUnstallBtn(false); // 屏蔽系统应用


		CButtonUI *pButton = (CButtonUI*)GetItem(_T("DownBar.Uninstall"));
		if (pButton)
			pButton->SetInitPos(20,7,112,32,0);

		if(!pEdit->IsEmpty())
		{
			OnSearchClear(NULL);
		}
	}
	

	return true;
}

bool MainView::OnSearch(TNotifyUI* pNotify)
{
	CTabUI* pTab = (CTabUI*)GetItem(_T("AppTab"));
	CSearchEditUI* pEdit = (CSearchEditUI*)GetItem(_T("Search.Edit"));
	CButtonUI* pClear = (CButtonUI*)GetItem(_T("Search.Clear"));
	if (pTab == NULL || pEdit == NULL || pClear == NULL)
	{
		return true;
	}

	wstring text= pEdit->GetText();

	if(text.empty())
	{
		pClear->SetVisible(false);
	}
	else
	{
		pClear->SetVisible(true);
	}

	if(text.empty() || pEdit->IsEmpty())
	{
		pEdit->HideSearch();
	}

	pEdit->GetParent()->Resize();
	pEdit->GetParent()->Invalidate();

	ShowFilterInfo( true );

	return true;
}

bool MainView::OnSearchClear( TNotifyUI* pNotify )
{
	CSearchEditUI* pEdit = (CSearchEditUI*)GetItem(_T("Search.Edit"));
	CButtonUI* pClear = (CButtonUI*)GetItem(_T("Search.Clear"));
	if (pEdit == NULL || pClear == NULL)
	{
		return true;
	}

	pEdit->SetText(_T(""));
	pEdit->Resize();
	pEdit->Invalidate();
	pEdit->HideSearch();
	pClear->SetVisible(false);

	pEdit->GetParent()->Resize();
	pEdit->GetParent()->Invalidate();
	return true;
}


bool MainView::OnClearFilterBtnClick( TNotifyUI* pNotify )
{
	OnSearchClear(NULL);
	
	return true;
}

bool MainView::OnCloseFilterInfoBtnClick( TNotifyUI* pNotify )
{
	ShowFilterInfo(false);
	return true;
}

void MainView::ShowFilterInfo( bool show )
{
	CSearchEditUI* pEdit = (CSearchEditUI*)GetItem(_T("Search.Edit"));
	CButtonUI* pClear = (CButtonUI*)GetItem(_T("Search.Clear"));
	CLayoutUI* pFilterInfo = dynamic_cast<CLayoutUI*>(GetItem(_T("FilterInfo")));
	if (pFilterInfo == NULL || pEdit == NULL || pClear == NULL)
	{
		return;
	}

	wstring text=pEdit->GetText();
	if(text.empty())
		show = false;

	if(show&&!pFilterInfo->IsVisible())
	{
		pFilterInfo->SetVisible(true);
		pFilterInfo->SetInitPos(0,60,0,0, 6);
	}
	else if(!show&&pFilterInfo->IsVisible())
	{
		pFilterInfo->SetVisible(false);
		pFilterInfo->SetInitPos(0,0,0,0, 6);
	}

	this->Resize();
	this->Invalidate();
}


void MainView::RefreshUnstallBtn(bool bShow)
{
	CLayoutUI* pUpgradeAllLayout = (CLayoutUI*)GetItem(_T("DownBar.Upgrade"));
	CButtonUI* pAndUpgradeAllBtn = (CButtonUI*)GetItem(_T("DownBar.AndUpgradeAllAppsBtn"));

	CControlUI* pTem = NULL;
	pTem = GetItem(_T("DownBar.InstallBtn"));
	if (pTem != NULL) 
	pTem->SetVisible(bShow);

	pTem = GetItem(_T("DownBar.InstallBtnMenu"));
	if (pTem != NULL) 
	pTem->SetVisible(bShow);

	RequestDeviceStateMsg msg;
	msg.deviceID = this->_pController->GetDeviceID();
	msg.SendAndGetReturn();

	if (msg.nOsType == ePhoneOS_Ios && msg.bIsJailBreak)
	{
		pUpgradeAllLayout->SetVisible(bShow);
	}
	else
	{
		pAndUpgradeAllBtn->SetVisible(bShow);
	}
}

//更新移动按钮的状态
void MainView::RefreshMoveBtn( bool bShow )
{
	CControlUI* pTem = NULL;
	pTem = GetItem(_T("DownBar.MoveToPhone"));
	if (pTem != NULL) 
		pTem->SetVisible(bShow);

	pTem = GetItem(_T("DownBar.MoveToSD"));
	if (pTem != NULL)
		pTem->SetVisible(bShow);	
}