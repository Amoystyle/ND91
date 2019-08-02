#include "StdAfx.h"
#include "WebAppListUI.h"
#include "UI/AutoGridLayoutUI.h"
#include "Logic/WebSoftInfo.h"
#include "Logic/WebSoftModel.h"
#include "ui/WebSoftUI/WebAppListItemControl.h"
#include "ui/WebSoftUI/WebAppListItemFloatLayer.h"

WebAppListUI::WebAppListUI()
{
}

WebAppListUI::~WebAppListUI(void)
{
}

void WebAppListUI::OnCreate()
{
	CAutoGridLayoutUI* pAppgrid=(CAutoGridLayoutUI*)GetItem(L"webAppList_Layout");
	if (pAppgrid)
	{
		pAppgrid->SetAttribute(L"ShowRadio",L"0");
		pAppgrid->SetAttribute(L"GridWidth",L"190");
		pAppgrid->SetAttribute(L"GridHeight",L"80");
		pAppgrid->SetAttribute(L"EnablePage",L"0");
	}
	
}

void WebAppListUI::AddItem(const WebSoftData* pData)
{
	CAutoGridLayoutUI* pAppgrid=(CAutoGridLayoutUI*)GetItem(L"webAppList_Layout");
	CLayoutUI* itemlayout=new CLayoutUI();
	itemlayout->SetId(CStrOperation::IntToWString(pData->_id).c_str());
	itemlayout->SetInitPos(0,0,0,0,8);
	pAppgrid->Add(itemlayout);
	
	WebAppListItemControl* item=new WebAppListItemControl(Singleton::GetMainWnd(),pData);
	item->SetStyle(0);
	item->Init(itemlayout);
}

void WebAppListUI::ShowListData(const vector<WebSoftData*>* pSofts)
{
	CAutoGridLayoutUI* pAppgrid=(CAutoGridLayoutUI*)GetItem(L"webAppList_Layout");
	pAppgrid->RemoveAll();
	for (size_t i=0;i<pSofts->size();i++)
	{
		AddItem(pSofts->at(i));
	}
	pAppgrid->Resize();
	pAppgrid->Invalidate();
}

void WebAppListUI::ShowAppIcon( int fid )
{
	CAutoGridLayoutUI* pAppgrid=(CAutoGridLayoutUI*)GetItem(L"webAppList_Layout");
	CLayoutUI* itemlay=(CLayoutUI*)pAppgrid->GetItem(CStrOperation::IntToWString(fid).c_str());
	RETURN_IF(!itemlay);
	WebAppListItemControl* item=dynamic_cast<WebAppListItemControl*>(itemlay->GetItem(0));
	if(item)
		item->SetIcon(L"");
}

void WebAppListUI::RemoveAll()
{
	CAutoGridLayoutUI* pAppgrid=(CAutoGridLayoutUI*)GetItem(L"webAppList_Layout");
	if(pAppgrid)
		pAppgrid->RemoveAll();
	WebAppListItemFloatLayer::GetInstance()->ShowWindow(SW_HIDE);;

}

std::wstring WebAppListUI::GetStyleID()
{
	return _T("WebAppListComposite");
}

void WebAppListUI::Init( CContainerUI* pContainer )
{
	BaseView::Init(pContainer);
}
