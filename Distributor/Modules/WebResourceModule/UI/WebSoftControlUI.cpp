#include "StdAfx.h"
#include "WebSoftControlUI.h"
#include "WebAppHomeUI.h"
#include "Logic/WebSoftModel.h"
#include "Logic/WebSoftInfo.h"
#include "ui/WebSoftUI/WebAppDescriptionUI.h"
WebSoftControlUI::WebSoftControlUI(HWND hWnd,bool IsPlatformiPad)
{
	_pSearchMainUI=NULL;
	_pSearchResultUI=NULL;
	_pCatagoryUI=NULL;
	_pCurVisibleUI=NULL;
	_pDescriptionUI=NULL;
	_platformiPad = IsPlatformiPad;
	_pWebsoftModel=new WebSoftModel(_platformiPad?IPAD:IPHONE);
}

WebSoftControlUI::~WebSoftControlUI(void)
{
	for(size_t i=0;i<_pWebSoftCategorys.size();i++)
	{
		SAFE_DELETE(_pWebSoftCategorys[i]);
	}
	_pWebSoftCategorys.clear();
	SAFE_DELETE(_pWebsoftModel);
}

void WebSoftControlUI::OnCreate()
{
	WebSoftCategory* game=new WebSoftCategory();
	game->_id=6014;
	game->_name=L"ÓÎÏ·";
	_pWebSoftCategorys.push_back(game);
	_pWebsoftModel->GetSoftCategoryList(false);

	ShowSearchHome();
}

WebAppHomeUI* WebSoftControlUI::CreateAppHomeUI(int type)
{
	RETURN_NULL_IF(type<0||type>2);
	WebAppHomeUI* pUI=new WebAppHomeUI(Singleton::GetMainWnd(),(WebAppHomeUI::PAGETYPE)type,this);

	pUI->Init(dynamic_cast<CContainerUI*>(GetItem(L"WebSoftControl_layout.WebAppHomeComposite")));
	pUI->SetVisible(false);
	return pUI;
}

void WebSoftControlUI::ShowSearchResult(wstring searchtext, int page)
{
	if(!_pSearchResultUI)
	{
		_pSearchResultUI=CreateAppHomeUI(WebAppHomeUI::SEARCH_RESULT_PAGE);
		//by12.12.1 00:10 ÐÞÕýbug25190
		_pSearchResultUI->OnRectChanged(GetRect());
	}
	if( _pDescriptionUI&&_pDescriptionUI->IsVisible())
		_pDescriptionUI->SetVisible(false);
	_pCurVisibleUI->SetVisible(false);
	_pSearchResultUI->SetVisible(true);
	_pCurVisibleUI=_pSearchResultUI;
	if(!searchtext.empty()&&page!=-1)
	{
		_pWebsoftModel->SearchAppList(searchtext,_pSearchResultUI->GetCurPageCount(),page);
		_pSearchResultUI->ShowSearchResultPage(searchtext,page);
	}
	//_pSearchResultUI->ShowSearchResultPage(searchstr,page);
	GetItem(L"WebSoftControl_layout")->Resize();
	GetItem(L"WebSoftControl_layout")->Invalidate();
}

void WebSoftControlUI::ShowCategoryUI(int selID)
{
	if(!_pCatagoryUI)
	{
		_pCatagoryUI=CreateAppHomeUI(WebAppHomeUI::CATEGORY_PAGE);
		_pCatagoryUI->ShowCategorys(&_pWebSoftCategorys, selID);
		_pCatagoryUI->OnRectChanged(GetRect());
	}
	if( _pDescriptionUI&&_pDescriptionUI->IsVisible())
		_pDescriptionUI->SetVisible(false);
	_pCurVisibleUI->SetVisible(false);
	_pCatagoryUI->SetVisible(true);
	_pCurVisibleUI=_pCatagoryUI;
	_pCatagoryUI->ShowCategoryPage(selID);
	GetItem(L"WebSoftControl_layout")->Resize();
	GetItem(L"WebSoftControl_layout")->Invalidate();
}

void WebSoftControlUI::ShowSearchHome(int page)
{
	if(!_pSearchMainUI)
	{
		_pSearchMainUI=CreateAppHomeUI(WebAppHomeUI::SEARCH_PAGE);
		_pSearchMainUI->ShowLoadingData();
		_pWebsoftModel->Get24HoursHotAppList();
	}
	if(_pCurVisibleUI)
		_pCurVisibleUI->SetVisible(false);
	if(_pDescriptionUI&&_pDescriptionUI->IsVisible())
		_pDescriptionUI->SetVisible(false);
	_pSearchMainUI->SetVisible(true);
	_pCurVisibleUI=_pSearchMainUI;
	if(page!=-1)
		_pSearchMainUI->ShowSearchHomePage(page);
	GetItem(L"WebSoftControl_layout")->Resize();
	GetItem(L"WebSoftControl_layout")->Invalidate();
}

WebSoftModel* WebSoftControlUI::GetWebsoftModel()
{
	return _pWebsoftModel;
}

bool WebSoftControlUI::OnClickSearchbox( TNotifyUI* pNotify )
{
	wstring text= (wchar_t*)pNotify->wParam;
	if(!text.empty())
		ShowSearchResult(text,1);
	
	bool ret = false;
	Statistic_StatFuncTab( "900008", CCodeOperation::UnicodeToUTF_8(text).c_str(), ret );

	return true;
}

bool WebSoftControlUI::OnClickGotoDes( TNotifyUI* pNotify )
{
	if(!pNotify)
		return true;
	int id = pNotify->wParam;
	ShowAppType type= (ShowAppType)pNotify->lParam; 
	_pCurVisibleUI->SetVisible(false);
	if(!_pDescriptionUI)
	{
		_pDescriptionUI = new WebAppDescriptionUI(Singleton::GetMainWnd());
		_pDescriptionUI->Init(this);
	}
	_pDescriptionUI->ShowAppItemDetail(id, _platformiPad? IPAD:IPHONE,type==HOT24APP);
	_pDescriptionUI->SetVisible(true);

	WebResourceInnserMsg msg;
	msg.type = WebResourceMsg_ClickGotoDes;
	msg.Send();
	return true;
}

void WebSoftControlUI::GoBack()
{
	if(_pDescriptionUI&&_pDescriptionUI->IsVisible())
	{
		_pDescriptionUI->SetVisible(false);
		if(_pCurVisibleUI)
			_pCurVisibleUI->SetVisible(true);
	}
	else
	{
		ShowSearchHome();
	}
}

void WebSoftControlUI::GotoDescription( int id, bool IsHot24)
{
	_pCurVisibleUI->SetVisible(false);
	if(!_pDescriptionUI)
	{
		_pDescriptionUI = new WebAppDescriptionUI(Singleton::GetMainWnd());
		_pDescriptionUI->Init(this);
	}
	_pDescriptionUI->ShowAppItemDetail(id, _platformiPad? IPAD:IPHONE,IsHot24);
	_pDescriptionUI->SetVisible(true);
}

void WebSoftControlUI::OnRefresh()
{
	if(_pCurVisibleUI&&_pCurVisibleUI->IsVisible())
	{
		((WebAppHomeUI*)_pCurVisibleUI)->OnRefresh();
	}
	else if(_pDescriptionUI&&_pDescriptionUI->IsVisible())
	{
		_pDescriptionUI->OnRefresh();
	}
}

void WebSoftControlUI::SetRect( RECT& rc )
{
	__super::SetRect(rc); 
	RECT subrc;
	subrc.left=rc.left;
	subrc.right=rc.right;
	subrc.top=rc.top;
	subrc.bottom=rc.bottom;
	if(_pCurVisibleUI&&	_pCurVisibleUI->IsVisible())
		((WebAppHomeUI*)_pCurVisibleUI)->OnRectChanged(subrc,true);
	if(_pSearchMainUI&&_pSearchMainUI!=_pCurVisibleUI)
		_pSearchMainUI->OnRectChanged(subrc);
	if(_pSearchResultUI&&_pSearchResultUI!=_pCurVisibleUI)
		_pSearchResultUI->OnRectChanged(subrc);
	if(_pCatagoryUI&&_pCatagoryUI!=_pCurVisibleUI)
		_pCatagoryUI->OnRectChanged(subrc);
}

bool WebSoftControlUI::Handle( const WebSoftMsg* pMsg )
{
	int type=(int)pMsg->platType;
 	RETURN_TRUE_IF( (_platformiPad&&type!=IPAD) || (!_platformiPad&&type!=IPHONE));


	switch(pMsg->message)
	{
		case WM_WebLegalSoft_24HotHasLoad:
			{
				if(_pSearchMainUI)
					_pSearchMainUI->ShowList();
			}
			break;
		case WM_WebLegalSoft_CategorysHasLoad:
			{
				_pWebsoftModel->GetCategoryDatas(_pWebSoftCategorys);
				if(_pWebSoftCategorys.size()<=1)
				{
					_pWebsoftModel->GetSoftCategoryList(false);
					return true;
				}
				if(_pSearchMainUI)
					_pSearchMainUI->ShowCategorys(&_pWebSoftCategorys);
				if(_pCatagoryUI)
					_pCatagoryUI->ShowCategorys(&_pWebSoftCategorys);
			}
			break;
		case WM_WebLegalSoft_AppDatasHasLoad:
			{
				if(_pCatagoryUI)
					_pCatagoryUI->ShowList();
			}
			break;
		case WM_WebLegalSoft_GameDatasHasLoad:
			{
				if(_pCatagoryUI)
					_pCatagoryUI->ShowList();
			}
			break;
		case WM_WebLegalSoft_OneAppInfoHasLoad:	
			break;
		case WM_WebLegalSoft_SearchAppsHasLoad:	
			{
				if(_pSearchResultUI)
					_pSearchResultUI->ShowList();
			}
			break;
		case WM_WebLegalSoft_IconHasDownload:	
			{
				if(_pCurVisibleUI)
				{
					WebAppHomeUI* curapphomeui=dynamic_cast<WebAppHomeUI*>(_pCurVisibleUI);
					if(curapphomeui)
					{
						int fid=pMsg->f_id;
						wstring path=pMsg->path;
						curapphomeui->ShowAppIcon(fid,path);
					}
				}
			}
			break;
		case WM_WebLegalSoft_BeginDownload:	
			break;
		case WM_WebLegalSoft_EndDownload:	
			break;
		case WM_WebLegalSoft_BeginInstall:	
			break;
		case WM_WebLegalSoft_EndInstall:	
			break;
		case WM_WebLegalSoft_GotoDes:
			{
				GotoDescription(pMsg->id,pMsg->apptype);
			}
			break;
		default:
			break;
	}

	return true;
}

std::wstring WebSoftControlUI::GetStyleID()
{
	return _T("WebSoftControlComposite");
}