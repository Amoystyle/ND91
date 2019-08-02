#include "StdAfx.h"
#include "TopView.h"
#include "WebResourceModule.h"
#include "Singleton.h"
TopView::TopView(CContainerUI* pParent)
{
	_ePlatform = PLATFORM_Android;
}

TopView::~TopView(void)
{
}

void TopView::OnCreate()
{
	CTabUI* pwebTab = (CTabUI*)GetItem(_T("WebRes_PhoneType_Tab"));
#if _DEBUG
	assert(pwebTab);
#endif
	pwebTab->GetHeader()->SetAttribute(_T("bk.visible"),_T("0"));
	pwebTab->GetHeader()->SetAttribute(_T("startx"),_T("10"));
	pwebTab->GetHeader()->SetAttribute(_T("spacex"),_T("20"));
	pwebTab->SetTabHeaderAttribute(0,_T("txt.textcolor"),_T("02baee"));
}

wstring TopView::GetStyleID()
{
	return L"WebResource_View_Top";
}

bool TopView::OnBtnCk( TNotifyUI* pNotify )
{INTERFUN;

	WebResourceInnserMsg pMsg;
	pMsg.type     = WebResourceMsg_TopOperate;
	pMsg.lParam   = _ePlatform;
	LPCTSTR id=pNotify->pSender->GetId();
	if(_tcscmp(id, _T("btn_goback"))==0)
	{
		pMsg.wParam = Etype_UrlOperate_Back;
	}
	if(_tcscmp(id, _T("btn_goforword"))==0)
	{
		pMsg.wParam = Etype_UrlOperate_Forword;
	}
	if(_tcscmp(id, _T("btn_gohome"))==0)
	{
		pMsg.wParam = Etype_UrlOperate_Home;
	}
	
	pMsg.Send();

	return true;
}


bool TopView::OnRefresh(TNotifyUI* pNotify)
{INTERFUN;

	WebResourceInnserMsg pMsg;
	pMsg.type     = WebResourceMsg_TopOperate;
	pMsg.wParam   = Etype_UrlOperate_Reflash;
	pMsg.lParam   = _ePlatform;
	pMsg.Send();

	return true;
}

bool TopView::OnSearch( TNotifyUI* pNotify )
{INTERFUN;

	CEditUI* edit=(CEditUI*)GetItem(_T("layout_search.web_search_edit"));
	if (NULL==edit)
	{
		return false;
	}

	wstring text=edit->GetText();
	wstring tmptext=CStrOperation::ReplaceW(text,L" ",L"");
	//if (tmptext.empty())
	//{
	//	//MessageDialog(BaseUI::LoadString(L"EmptyValueFail"),BaseUI::LoadString(L"PROMPT"));
	//	return true;
	//}
	tmptext=CStrOperation::ReplaceW(tmptext,L"\\\\",L"");
	RETURN_TRUE_IF(!tmptext.empty()&&tmptext.substr(tmptext.length()-1,1)==L"\\");
	
	//搜索
	WebResourceInnserMsg pMsg;
	pMsg.type     = WebResourceMsg_TopSearch;
	pMsg.strParam = tmptext;
	pMsg.lParam   = _ePlatform;
	pMsg.Send();

	return true;
}

bool TopView::OnSearchEditChange( TNotifyUI* pNotify )
{INTERFUN;

	CEditUI* pEdit=(CEditUI*)pNotify->pSender;
	if (NULL==pEdit)
	{
		return false;
	}
	bool empty=pEdit->IsEmpty();
	wstring text=pEdit->GetText();
	if(!text.empty())
	{
		GetItem(_T("layout_search.web_searchBox_Clear"))->SetVisible(true);
	}
	else
	{
		GetItem(_T("layout_search.web_searchBox_Clear"))->SetVisible(false);
	}
	return true;
}

bool TopView::OnSearchClear( TNotifyUI* pNotify )
{INTERFUN;
	CEditUI* edit=(CEditUI*)GetItem(_T("layout_search.web_search_edit"));
	if(edit)
	{
		edit->SetText(_T(""));
		edit->Resize();
		edit->Invalidate();

		return true;
	}
	return false;
}

bool TopView::OnWebPlatFormSelChanged( TNotifyUI* pNotify )
{

	CTabUI* pwebTab = (CTabUI*)GetItem(_T("WebRes_PhoneType_Tab"));
	if (NULL==pwebTab)
	{
		return false;
	}

	ENUM_PLATFORM eTemp =(ENUM_PLATFORM)pwebTab->GetCurSel();
	if (_ePlatform==eTemp)
	{
		return true;
	}

	pwebTab->SetTabHeaderAttribute(_ePlatform,_T("txt.textcolor"),_T("3c3d42"));
	_ePlatform = eTemp;
	pwebTab->SetTabHeaderAttribute(_ePlatform,_T("txt.textcolor"),_T("02baee"));

	WebResourceInnserMsg pMsg;
	pMsg.type     = WebResourceMsg_LeftLoad;
	pMsg.lParam   = _ePlatform;//平台
	pMsg.wParam   = Etype_Application;//应用软件
	pMsg.SendToUI();

	return true;
}

void TopView::ClickLeftTree(NetNode& stNetNode)
{
	_stNetNode = stNetNode;
	if (stNetNode.nodeName==WebResourceModule::LoadString(L"WebPhoneOS_GenuineFree"))
	{
		GetItem(_T("webres_right_operate.btn_goforword"))->SetVisible(false);
	}
	else
	{
		GetItem(_T("webres_right_operate.btn_goforword"))->SetVisible(true);
	}
	this->Resize();
	this->Invalidate(true);
}

void TopView::HideSearch(bool bHide)
{
	GetItem(_T("layout_search"))->SetVisible(!bHide);
	GetItem(_T("web_search"))->SetVisible(!bHide);
}
