#include "stdafx.h"
#include "SysSettingController.h"

SysSettingController::SysSettingController()
{
	
}

void SysSettingController::Init(CContainerUI* pTitleToolBar, CContainerUI* pStatusBar)
{
	CButtonUI *pButton = new CButtonUI;
	pButton->SetINotifyUI(this);
	pButton->SetInitPos(0,0,16,16,0);
	pButton->SetId(L"BtnSystemMenu");
	pButton->SetAttribute(_T("bk.image"), _T("#DependenceFuncCenter_window_setting"));
	pButton->SetAttribute(_T("bk.imgseq"), _T("0,1,2,2,0"));
	pButton->SetAttribute(_T("bk.pos"), _T("9|-1,-1,18,18"));
	pButton->SetAttribute(_T("cursor"),_T("hand"));
	pTitleToolBar->Add(pButton);

	pTitleToolBar->Resize();
	pTitleToolBar->Invalidate();
}

SysSettingController::~SysSettingController()
{

}

bool SysSettingController::OnBtnSystemMenu(TNotifyUI* pNotify)
{
	if (_tcscmp(pNotify->pSender->GetId(),_T("BtnSystemMenu"))==0)
	{
		CMenuUI* pMenu = CMenuUI::Load(MENU_ID_SYSTEMMENU);
		pMenu->Remove(pMenu->GetCount() - 1 ,BY_POSITION);
		pMenu->Remove(pMenu->GetCount() - 1 ,BY_POSITION);
		pMenu->Remove(2 ,BY_POSITION);
		//----隐藏官网和分割线----
		pMenu->Remove(2 ,BY_POSITION);
		pMenu->Remove(2 ,BY_POSITION);
		//隐藏新功能向导
		pMenu->Remove(3 ,BY_POSITION);
		pMenu->SetParam(NULL,1);
		pMenu->Show(Singleton::GetMainWnd(), NULL, (INotifyUI*)this);
	}
	return true;
}

bool SysSettingController::OnMenuClick( TNotifyUI* pNotify )
{
	if(pNotify->wParam == NULL)
		return true;

	UIMENUINFO* pInfo = (UIMENUINFO*)pNotify->wParam;
	if (NULL == pInfo)
	{
		return false;
	}
	int menutype = pInfo->lParamNotify;

	MenuNotifyMsg msg;
	msg.uMsgType  = pInfo->uID;
	msg.nMenutype = menutype;
	msg.SendToUI();

	return true;
}