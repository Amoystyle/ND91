#include "StdAfx.h"
#include "WebSoftMainUI.h"
#include "ui/WebSoftControlUI.h"
#include "ui/WebSoftUI/WebAppListItemFloatLayer.h"

WebSoftMainUI::WebSoftMainUI(HWND hWnd)
{
	_piPhoneWebSoftUI=NULL;
	_piPadWebSoftUI=NULL;
}

WebSoftMainUI::~WebSoftMainUI(void)
{
}

void WebSoftMainUI::OnCreate()
{
// 	GetItem(_T("WebSoftMain_layout.layout_search.web_searchBox_Clear"))->SetVisible(false);
// 	// 设置tab背景不可见，间距
// 	CTabUI* pwebTab = (CTabUI*)GetItem(_T("WebSoftMain_layout.WebSoft_PhoneType_Tab"));
// 	pwebTab->GetHeader()->SetAttribute(_T("bk.visible"),_T("0"));
// 	pwebTab->GetHeader()->SetAttribute(_T("startx"),_T("10"));
// 	pwebTab->GetHeader()->SetAttribute(_T("spacex"),_T("20"));
// 	pwebTab->SelectItem(1);

 	_curplat= Web_iPhone;
	//SetPlatform(false);
}


// void WebSoftMainUI::SetPlatformIndex(PlatForm plat)
// {
// 	CTabUI* pwebTab = (CTabUI*)GetItem(L"WebSoftMain_layout.WebSoft_PhoneType_Tab");
// 	if(pwebTab)
// 	{
// 		if(plat==Web_Android)
// 			pwebTab->SelectItem(_curplat);
// 		else
// 			pwebTab->SelectItem(plat);
// 	}
// }

void WebSoftMainUI::SetPlatform(bool IsIpad)
{
	if(!IsIpad)
	{
		if(!_piPhoneWebSoftUI)
		{
			_piPhoneWebSoftUI=new WebSoftControlUI(Singleton::GetMainWnd(),false);
			_piPhoneWebSoftUI->Init((CContainerUI*)GetItem(L"WebSoftMain_layout.WebSoftControlComposite"));
		}
		_piPhoneWebSoftUI->SetVisible(true);
		if(_piPadWebSoftUI)
			_piPadWebSoftUI->SetVisible(false);
		WebAppListItemFloatLayer::GetInstance()->SetReceive((INotifyUI*)_piPhoneWebSoftUI);
		Statistic_StatFunc(Statistic_ID_IphonePlatForm);
	}
	else
	{
		if(!_piPadWebSoftUI)
		{
			_piPadWebSoftUI=new WebSoftControlUI(Singleton::GetMainWnd(),true);
			_piPadWebSoftUI->Init((CContainerUI*)GetItem(L"WebSoftMain_layout.WebSoftControlComposite"));
		}
		if(_piPhoneWebSoftUI)
			_piPhoneWebSoftUI->SetVisible(false);
		_piPadWebSoftUI->SetVisible(true);
		WebAppListItemFloatLayer::GetInstance()->SetReceive((INotifyUI*)_piPadWebSoftUI);
		Statistic_StatFunc(Statistic_ID_IpadPlatForm);
	}
	GetItem(L"WebSoftMain_layout")->Resize();
	GetItem(L"WebSoftMain_layout")->Invalidate();

}

bool WebSoftMainUI::OnWebPageSelChanged(int sel)
{
// 	CTabUI* pwebTab = (CTabUI*)GetItem(L"WebSoftMain_layout.WebSoft_PhoneType_Tab");
// 	RETURN_TRUE_IF(!pwebTab);
	if(sel==0)
	{
		Send(WebSoftMainUI_TabAndroid_Click,this,NULL,NULL);

		return true;
	}
	if(_curplat != (PlatForm)sel)
	{
		//pwebTab->SetTabHeaderAttribute(_curplat,_T("txt.textcolor"),_T("3c3d42"));
		_curplat= (PlatForm)sel;
		//pwebTab->SetTabHeaderAttribute(_curplat,_T("txt.textcolor"),_T("02baee"));
	}
	SetPlatform((PlatForm)sel==Web_iPad);
	return true;
}

bool WebSoftMainUI::OnSearch( wstring searchbuf )
{
	Statistic_StatFunc(Statistic_ID_SearchLegalApp);
// 	CEditUI* edit=(CEditUI*)GetItem(_T("WebSoftMain_layout.layout_search.web_search_edit"));
// 	wstring text=edit->GetText();

	bool ret = false;
	Statistic_StatFuncTab( "900008", CCodeOperation::UnicodeToUTF_8(searchbuf).c_str(), ret );

	if(searchbuf.empty())
		 OnBtnGoHome();
	else
	{
		if(_piPadWebSoftUI&&_piPadWebSoftUI->IsVisible())
			_piPadWebSoftUI->ShowSearchResult(searchbuf,1);
		else if(_piPhoneWebSoftUI&&_piPhoneWebSoftUI->IsVisible())
			_piPhoneWebSoftUI->ShowSearchResult(searchbuf,1);
		GetItem(L"WebSoftMain_layout")->Resize();
		GetItem(L"WebSoftMain_layout")->Invalidate();
	}
	return true;
}

// bool WebSoftMainUI::OnSearchClear( TNotifyUI* pNotify )
// {
// 	CEditUI* edit=(CEditUI*)GetItem(_T("WebSoftMain_layout.layout_search.web_search_edit"));
// 	if(edit)
// 	{
// 		edit->SetText(_T(""));
// 		edit->Resize();
// 		edit->Invalidate();
// 	}
// 	return true;
// }

// bool WebSoftMainUI::OnSearchEditChange( TNotifyUI* pNotify )
// {
// 	CEditUI* pEdit=(CEditUI*)pNotify->pSender;
// 	bool empty=pEdit->IsEmpty();
// 	wstring text=pEdit->GetText();
// 	if(!text.empty())
// 	{
// 		GetItem(_T("WebSoftMain_layout.layout_search.web_searchBox_Clear"))->SetVisible(true);
// 	}
// 	else
// 		GetItem(_T("WebSoftMain_layout.layout_search.web_searchBox_Clear"))->SetVisible(false);
// 	return true;
// }

bool WebSoftMainUI::OnBtnGoHome()
{
	if(_piPadWebSoftUI&&_piPadWebSoftUI->IsVisible())
		_piPadWebSoftUI->ShowSearchHome();
	else if(_piPhoneWebSoftUI&&_piPhoneWebSoftUI->IsVisible())
		_piPhoneWebSoftUI->ShowSearchHome();
	GetItem(L"WebSoftMain_layout")->Resize();
	GetItem(L"WebSoftMain_layout")->Invalidate();
	return true;
}

bool WebSoftMainUI::OnRefresh()
{
	if(_piPadWebSoftUI&&_piPadWebSoftUI->IsVisible())
		_piPadWebSoftUI->OnRefresh();
	else if(_piPhoneWebSoftUI&&_piPhoneWebSoftUI->IsVisible())
		_piPhoneWebSoftUI->OnRefresh();
	return true;
}

bool WebSoftMainUI::OnGoBack()
{
	if(_piPadWebSoftUI&&_piPadWebSoftUI->IsVisible())
		_piPadWebSoftUI->GoBack();
	else if(_piPhoneWebSoftUI&&_piPhoneWebSoftUI->IsVisible())
		_piPhoneWebSoftUI->GoBack();
	GetItem(L"WebSoftMain_layout")->Resize();
	GetItem(L"WebSoftMain_layout")->Invalidate();
	return true;
}

// int WebSoftMainUI::GetSelPlatformIndex()
// {
// 	CTabUI* pwebTab = (CTabUI*)GetItem(L"WebSoftMain_layout.WebSoft_PhoneType_Tab");
// 	return pwebTab->GetCurSel();
// }

std::wstring WebSoftMainUI::GetStyleID()
{
	return _T("WebSoftMainComposite");
}

void WebSoftMainUI::Init( CContainerUI* pContainer )
{
	BaseView::Init(pContainer);
}
