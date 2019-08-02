#include "stdafx.h"
#include "MainView.h"

MainView::MainView()
{
	_pSelectBtn = NULL;
}

MainView::~MainView()
{

}

std::wstring MainView::GetStyleID()
{
	return L"ModuleManager_View_Main";
}

void MainView::AddUserModule( SourceModule* pModule )
{
	if(pModule)
		pModule->Init(this->GetUserLayout(), NULL);
}

void MainView::AddModule( SourceModule* pModule, bool bSelect)
{	
	if(pModule == NULL)
		return;

	CCheckBoxUI* pButton = this->InsertTopButton(this->GetTopBar(), pModule->Id(), pModule->Name(), pModule->Icon());
	pButton->SetUserData((UINT)pModule);
	pModule->Init(this->GetLeftLayout(), this->GetContainerLayout());
	if(bSelect)
	{
		pButton->SetChecked(true);
		ModuleManagerInnerMsg msg;
		msg.nAction = ModuleManager_Action_SelectModule;
		msg.wParam = (UINT)pModule;
		msg.SendToUI();
	}
}

CCheckBoxUI* MainView::InsertTopButton(CContainerUI* bar, tstring id, tstring text, tstring icon)
{
	CCompositeUI* pTopBtn = new CCompositeUI();
	pTopBtn->SetInitPos(0,0,165,60, 0);
	pTopBtn->Create(L"ModuleManager_Control_TopButton", bar);
	pTopBtn->SetId(id.c_str());

	CCheckBoxUI* pButton = dynamic_cast<CCheckBoxUI*>(pTopBtn->GetItem(0));
	if(pButton != NULL)
	{
		pButton->SetAttribute(L"group", L"TopBar");
		pButton->SetText(text.c_str());
		pButton->SetAttribute(L"icon.image", icon.c_str());
		pButton->SetINotifyUI(this);
	}

	CControlUI* pLbl = pTopBtn->GetItem(1);
	if(pLbl != NULL)
		pLbl->SetVisible(false);

	bar->Resize();
	bar->Invalidate();

	return pButton;
}

CContainerUI* MainView::GetTopBar()
{
	CContainerUI* pTopBar = dynamic_cast<CContainerUI*>(GetItem(L"TopBar"));
	return pTopBar;
}

CContainerUI* MainView::GetUserLayout()
{
	CContainerUI* pUserLayout = dynamic_cast<CContainerUI*>(GetItem(L"UserLayout"));
	return pUserLayout;
}

CContainerUI* MainView::GetLeftLayout()
{
	CContainerUI* pLeft = dynamic_cast<CContainerUI*>(GetItem(L"MainContainer.Left"));
	return pLeft;
}

CContainerUI* MainView::GetContainerLayout()
{
	CContainerUI* pContainer = dynamic_cast<CContainerUI*>(GetItem(L"MainContainer.Container"));
	return pContainer;
}

CContainerUI* MainView::GetTitleToolBar()
{
	CContainerUI* pContainer = dynamic_cast<CContainerUI*>(GetItem(L"TitleBar.TitleToolBar"));
	return pContainer;
}

CContainerUI* MainView::GetStatusBar()
{
	CContainerUI* pContainer = dynamic_cast<CContainerUI*>(GetItem(L"StatusBar"));
	return pContainer;
}

void MainView::OnCreate()
{
	ModuleManagerInnerMsg msg;
	msg.nAction = ModuleManager_Action_LoadModules;
	msg.SendToUI();
}

void MainView::SelectModuleButton( SourceModule* pModule )
{
	CControlUI* pControl = this->GetTopBar();
	if(pControl == NULL)
		return;

	pControl = pControl->GetItem(pModule->Id().c_str());
	if(pControl == NULL)
		return;

	CCheckBoxUI* pButton = dynamic_cast<CCheckBoxUI*>(pControl->GetItem(0));
	if(pButton)		
	{
		pButton->SetChecked(true);
		ModuleManagerInnerMsg msg;
		msg.nAction = ModuleManager_Action_SelectModule;
		msg.wParam = (UINT)pModule;
		msg.SendToUI();
	}
}


bool MainView::OnTopButtonClick( TNotifyUI* pNotify )
{
	if(_pSelectBtn != NULL && _pSelectBtn != pNotify->pSender)
	{
		_pSelectBtn->SetChecked(false, false);
	}

	_pSelectBtn = dynamic_cast<CCheckBoxUI*>(pNotify->pSender);
	if(!_pSelectBtn->IsChecked())
	{
		_pSelectBtn->SetChecked(true, false);
	}

	SourceModule* pModule = (SourceModule*)pNotify->pSender->GetUserData();

	ModuleManagerInnerMsg msg;
	msg.nAction = ModuleManager_Action_SelectModule;
	msg.wParam = (UINT)pModule;
	msg.SendToUI();
	return true;
}

bool MainView::Handle( const TopTaskCountMsg* pMsg )
{
	CContainerUI* pBar = this->GetTopBar();
	if(pBar == NULL)
		return true;

	CControlUI* pTopBtn = pBar->GetItem(L"TaskCenterModule");
	if(pTopBtn == NULL)
		return true;

	CControlUI* pLbl = pTopBtn->GetItem(1);
	if(pLbl == NULL)
		return true;

	if(pMsg->nParam > 0)
	{
		pLbl->SetVisible(true);
		wstring cnttext =CStrOperation::IntToWString(pMsg->nParam);
		pLbl->GetItem(0)->SetText(cnttext.c_str());
	}
	else
	{
		pLbl->SetVisible(false);
		pLbl->GetItem(0)->SetText(L"0");
	}
	pBar->Resize();
	pBar->Invalidate();

	return true;
}

bool MainView::Handle( const NotifySourceModule* pMsg )
{
	CContainerUI* pBar = this->GetTopBar();
	if(pBar == NULL)
		return true;

	CControlUI* pTopBtn = NULL;
	switch(pMsg->type)
	{
	case SourceModuleType_TaskCenter:
		pTopBtn = pBar->GetItem(L"TaskCenterModule");
		break;
	case SourceModuleType_InstallCase:
		pTopBtn = pBar->GetItem(L"InstallCaseModule");
		break;
	case SourceModuleType_WebResource:
		pTopBtn = pBar->GetItem(L"WebResourceModule");
		break;
	default:
		break;
	}

	if(pTopBtn == NULL)
		return true;

	CCheckBoxUI* pButton = dynamic_cast<CCheckBoxUI*>(pTopBtn->GetItem(0));
	if(pButton == NULL)
		return true;

	pButton->SetChecked(true, true);
	return true;
}
