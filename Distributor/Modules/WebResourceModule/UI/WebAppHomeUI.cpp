#include "StdAfx.h"
#include "math.h"

#include "WebAppHomeUI.h"
#include "ui/WebSoftControlUI.h"
#include "Logic/WebSoftModel.h"

#include "Logic/WebSoftInfo.h"

#include "ui/WebSoftUI/WebAppListPagerControl.h"
#include "ui/WebSoftUI/WebAppSearchBoxControl.h"


WebAppHomeUI::WebAppHomeUI(HWND hWnd,PAGETYPE type,CCompositeUI* parentComposite)
:_pagetype(type)
,_pParentComposite(parentComposite)
{
	_pWebAppList=NULL;
	_pBottomcategoryui=NULL;
	_pTopcategoryui=NULL;
	_pToppage=NULL;
	_pBottomPage=NULL;
	_pSearchBoxUI=NULL;
	_pListFilterUI=NULL;
	_totalSoftCount=0;
	_curcategoryId=6014;
	_curaPage=1;
	_curpagecount=20;
	_needrefreshdata=false;
	_flitertypestr=DownnumDESC;
}

WebAppHomeUI::~WebAppHomeUI(void)
{
	ReleaseWebAppData();
}

void WebAppHomeUI::OnCreate()
{
	CLayoutUI*	lay=(CLayoutUI*)GetItem(L"WebAppHome_layout");
	if(_pagetype==SEARCH_PAGE)
	{
		CLayoutUI* searchlay=new CLayoutUI();
		searchlay->SetInitPos(0,0,0,130,4);
		searchlay->SetId(L"WebSoft_Search_layout");
		_pSearchBoxUI=new WebAppSearchBoxControl();
		_pSearchBoxUI->SetReceive(_pParentComposite);
		lay->Insert(searchlay,NULL);
		_pSearchBoxUI->Init(searchlay);
		GetItem(L"WebAppHome_layout.websoftshow_layout")->SetInitPos(10,130,10,15,8);
		GetItem(L"WebAppHome_layout")->Resize();
		GetItem(L"WebAppHome_layout")->Invalidate();
	}
	CreateHeader();
	_pWebAppList=new WebAppListUI();
	_pWebAppList->Init(dynamic_cast<CContainerUI*>(GetItem(L"WebAppHome_layout.websoftshow_layout.WebAppListComposite")));

	_lastrect=GetRect();
	_curpagecount=_pagetype==SEARCH_PAGE?16:20;
	CreateBottom();

	GetItem(L"WebAppHome_layout")->Resize();
	GetItem(L"WebAppHome_layout")->Invalidate();
}

void WebAppHomeUI::CreateHeader()
{
	CLayoutUI* lay=(CLayoutUI*)GetItem(L"WebAppHome_layout.websoftshow_layout.websoftshow_header_layout.headerleft_layout");
	if(lay)
	{
		if(_pagetype!=CATEGORY_PAGE)
		{
			CLabelUI* lable=new CLabelUI();
			lable->SetInitPos(0,-1,50,30,9);
			lable->SetAttribute(L"sizewithcontent",L"1");
			lay->Add(lable);
			if(_pagetype==SEARCH_RESULT_PAGE)
			{
				CLabelUI* lable1=new CLabelUI();
				lable1->SetInitPos(0,-1,50,30,9);
				lable1->SetAttribute(L"sizewithcontent",L"1");
				lable1->SetAttribute(L"txt.textcolor",L"ff0000");
				lay->Add(lable1);
				CLabelUI* lable2=new CLabelUI();
				lable2->SetInitPos(0,-1,50,30,9);
				lable2->SetAttribute(L"sizewithcontent",L"1");
				lay->Add(lable2);
			}
			else
			{
				lable->SetText(L"24小时热门应用");
				lable->SetAttribute(L"txt.font", L"font12b");
				lable->SetAttribute(L"txt.textcolor",L"2D5384");
				lable->SetInitPos(0,-1,100,30,9);
			}
		}
		else
		{
			CLayoutUI* topcategory=new CLayoutUI();
			topcategory->SetInitPos(0,4,100,28,0);
			lay->Add(topcategory);
			_pTopcategoryui=new WebCategoriesControl(Singleton::GetMainWnd(),true);
			_pTopcategoryui->Init(topcategory);
			_pTopcategoryui->SetReceive(this);
		}
		lay->Resize();
		lay->Invalidate();
	}

	CLayoutUI* rightlay=(CLayoutUI*)GetItem(L"WebAppHome_layout.websoftshow_layout.websoftshow_header_layout.headerright_layout");
	if(rightlay)
	{
		if(_pagetype==CATEGORY_PAGE)
		{
			_pListFilterUI=new WebAppListFilterControl(Singleton::GetMainWnd());
			_pListFilterUI->Init(rightlay);
			_pListFilterUI->SetReceive(this);
		}
		else if(_pagetype==SEARCH_PAGE)
		{
			_pToppage=new WebAppListPagerControl(Singleton::GetMainWnd(),true,_curpagecount);
			_pToppage->Init(rightlay);
			_pToppage->SetReceive(this);
		}
		rightlay->Resize();
		rightlay->Invalidate();
	}
}

void WebAppHomeUI::CreateBottom()
{
	CLayoutUI* lay=(CLayoutUI*)GetItem(L"WebAppHome_layout.websoftshow_layout.webapphome_bottom_layout");
	if(_pagetype!=SEARCH_PAGE)
	{
		_pBottomPage=new WebAppListPagerControl(Singleton::GetMainWnd(),false,_curpagecount);
		_pBottomPage->Init(lay);
		_pBottomPage->SetReceive(this);
	}
	else
	{
		_pBottomcategoryui=new WebCategoriesControl(Singleton::GetMainWnd(),false);
		_pBottomcategoryui->Init(lay);
		_pBottomcategoryui->SetReceive(this);
	}
	lay->Resize();
	lay->Invalidate();
}

void WebAppHomeUI::ShowSearchResultPage( wstring searchtext, int page)
{
	RETURN_IF(_pagetype!=SEARCH_RESULT_PAGE);
	_curSearchtext=searchtext;
	_curaPage=page;
	ShowLoadingData();
}

void WebAppHomeUI::ShowCategoryPage( int categoryId )
{
	RETURN_IF(_pagetype!=CATEGORY_PAGE);
	_curaPage=1;
	_curcategoryId=categoryId;
	_pTopcategoryui->SetTopSelCategory(_curcategoryId);
	ShowLoadingData();
}

void WebAppHomeUI::ShowSearchHomePage(int pageNum)
{
	RETURN_IF(_pagetype!=SEARCH_PAGE);
	_pWebAppList->RemoveAll();
	GetItem(_T("WebAppHome_layout.websoftshow_layout.Loading"))->SetVisible(true);
}


bool WebAppHomeUI::OnClickCategorysBtn( TNotifyUI* pNotify )
{
	wstring categoryname=pNotify->pSender->GetText();
	int id=(int)pNotify->wParam;
	WebSoftControlUI* pWebsoftui= dynamic_cast<WebSoftControlUI*>(_pParentComposite);
	if(pWebsoftui)
	{
		_curcategoryId=id;
		pWebsoftui->ShowCategoryUI(id);
		pWebsoftui->GetWebsoftModel()->GetAppDataList(id,_curpagecount,1,_flitertypestr);
	}
	if(_pagetype==SEARCH_PAGE)
	{
		Statistic_StatFunc(Statistic_ID_WebSoft_Category);
	}
	else
	{
		Statistic_StatFunc(Statistic_ID_PullDownChange);
	}

	return true;
}

bool WebAppHomeUI::OnClickFilterBtn( TNotifyUI* pNotify )
{
	RETURN_TRUE_IF(_pagetype!=CATEGORY_PAGE);
	int type=(int)pNotify->wParam;
	WebSoftControlUI* pWebsoftui= dynamic_cast<WebSoftControlUI*>(_pParentComposite);
	RETURN_TRUE_IF(!pWebsoftui);
	switch(type)
	{
		case WebAppListFilterControl::DownloadLevel:
			{
				_flitertypestr=DownnumDESC;
				Statistic_StatFunc(Statistic_ID_DownLoadMostBnt);
			}
			break;
		case WebAppListFilterControl::PriceLevel:
			{
				_flitertypestr=PriceDESC;
				Statistic_StatFunc(Statistic_ID_PriceHighBnt);
			}
			break;
		case WebAppListFilterControl::VersionLevel:
			{
				_flitertypestr=UpttimeDESC;
				Statistic_StatFunc(Statistic_ID_LastRelease);
			}
			break;
		default:
			return true;
	}
	pWebsoftui->GetWebsoftModel()->GetAppDataList(_curcategoryId,_curpagecount,1,_flitertypestr);
	ShowLoadingData();
	_curaPage=1;
	return true;
}

void WebAppHomeUI::ShowCategorys( vector<WebSoftCategory*>* pWebSoftCategorys, int selID )
{
	if(_pTopcategoryui)
		_pTopcategoryui->SetCategories(pWebSoftCategorys,selID);
	if(_pBottomcategoryui)
		_pBottomcategoryui->SetCategories(pWebSoftCategorys);
	GetItem(L"WebAppHome_layout")->Resize();
	GetItem(L"WebAppHome_layout")->Invalidate();
}

void WebAppHomeUI::ShowAppIcon( int fid, wstring iconpath )
{
	SetAppIcon(fid,iconpath);
	_pWebAppList->ShowAppIcon(fid);
} 

void WebAppHomeUI::SetAppIcon( int fid, wstring iconpath )
{
	for(size_t i=0;i<_pWebSofts.size();i++)
	{
		if(fid==_pWebSofts[i]->_id)
		{
			_pWebSofts[i]->_localpath=iconpath;
			break;
		}
	}
}

void WebAppHomeUI::ShowList(int pagecode)
{
	GetItem(_T("WebAppHome_layout.websoftshow_layout.Loading"))->SetVisible(false);
	WebSoftControlUI* pWebsoftui= dynamic_cast<WebSoftControlUI*>(_pParentComposite);
	if(pWebsoftui&&_pWebSofts.empty())
	{
		pWebsoftui->GetWebsoftModel()->GetAppDatas(_pWebSofts,_totalSoftCount);
		//临时解决TdBUG ID27573【网络资源】免越狱安装，经常点击首页下方分类按钮，进入的界面只显示4行软件，刷新后才可显示5行。1.2.3版本跟进修改
		/*if (_pagetype==CATEGORY_PAGE && (_pWebSofts.size() == 16) && _totalSoftCount > 16 )
		{
			RefreshData();
		}*/
		
	}
	if(_pagetype==SEARCH_PAGE)
	{
		_totalSoftCount=_pWebSofts.size();
		int pagecount=_totalSoftCount/_curpagecount+1;
		_pToppage->SetTotalCount(_totalSoftCount);
		_pToppage->SetPageIndex(pagecode-1);
		vector<WebSoftData*> hotsofts;
		if(pagecode>=1&&pagecode<=pagecount)
		{
			for (size_t i=_curpagecount*(pagecode-1);(int)i<(_curpagecount*pagecode)&&i<_pWebSofts.size();i++)
			{
				hotsofts.push_back(_pWebSofts[i]);
			}
		}
		_pWebAppList->ShowListData(&hotsofts);
	}
	else
	{
		if(_curaPage==1)
			_pBottomPage->SetTotalCount(_totalSoftCount);
		_pWebAppList->ShowListData(&_pWebSofts);

		if(_pagetype==SEARCH_RESULT_PAGE)
		{
			CLayoutUI* lay=(CLayoutUI*)GetItem(L"WebAppHome_layout.websoftshow_layout.websoftshow_header_layout.headerleft_layout");
			lay->GetItem(0)->SetText(L"共为你找到");
			wstring countstr=CStrOperation::IntToWString(_totalSoftCount);
			lay->GetItem(1)->SetText(countstr.c_str());
			wstring text=L"个\"";text+=_curSearchtext+L"\"相关应用";
			lay->GetItem(2)->SetText(text.c_str());
		}
	}
	GetItem(L"WebAppHome_layout")->Resize();
	GetItem(L"WebAppHome_layout")->Invalidate();
	vector<int> fids;
	vector<wstring> urls;
	for (size_t i=0;i<_pWebSofts.size();i++)
	{
		if(!CFileOperation::IsFileExist(_pWebSofts[i]->_localpath.c_str()))
		{
			fids.push_back(_pWebSofts[i]->_id);
			urls.push_back(_pWebSofts[i]->_imgSrc);
			if((int)fids.size()>=4)
			{
				pWebsoftui->GetWebsoftModel()->GetAppsIconFromWeb(fids,urls);
				fids.clear();
				urls.clear();
			}
		}
	}
	if(!fids.empty())
		pWebsoftui->GetWebsoftModel()->GetAppsIconFromWeb(fids,urls);
}

void WebAppHomeUI::ShowLoadingData()
{
	GetItem(_T("WebAppHome_layout.websoftshow_layout.Loading"))->SetVisible(true);
	ReleaseWebAppData();
	_pWebAppList->RemoveAll();
	GetItem(L"WebAppHome_layout")->Resize();
	GetItem(L"WebAppHome_layout")->Invalidate();
}

bool WebAppHomeUI::OnClickPageBtn( TNotifyUI* pNotify )
{
	if(_pagetype==SEARCH_PAGE)
		Statistic_StatFunc(Statistic_ID_PageOf24Hot);
	int page=(int)pNotify->wParam+1;
	_curaPage=page;

	RefreshData();

	return true;
}

void WebAppHomeUI::RefreshData()
{
	WebSoftControlUI* pWebsoftui= dynamic_cast<WebSoftControlUI*>(_pParentComposite);
	if(pWebsoftui)
	{

		if(_pagetype==SEARCH_RESULT_PAGE)
		{
			pWebsoftui->ShowSearchResult(_curSearchtext,_curaPage);
		}
		else if(_pagetype==CATEGORY_PAGE)
		{
			pWebsoftui->GetWebsoftModel()->GetAppDataList(_curcategoryId,_curpagecount,_curaPage,_flitertypestr);
			ShowLoadingData();
		}
		else if(_pagetype==SEARCH_PAGE)
		{
			ShowList(_curaPage);
		}
	}
}

void WebAppHomeUI::ReleaseWebAppData()
{
	for (size_t i=0;i<_pWebSofts.size();i++)
	{
		SAFE_DELETE(_pWebSofts[i]);
	}
	_pWebSofts.clear();
}

void WebAppHomeUI::OnRectChanged( RECT& rc,bool refreshdata)
{
	int newvcount=(rc.right-rc.left-20)/190;
	int newhcount=(rc.bottom-rc.top-75-(_pagetype==SEARCH_PAGE?130:0))/88;
	RETURN_IF(newvcount<=0||newhcount<=0);
	if(_lastrect.left==0)
	{
		_lastrect=rc;
		_curpagecount=newvcount*newhcount;
		_curaPage=1;
		if(_pToppage)
			_pToppage->SetPageCount(_curpagecount);
		else if(_pBottomPage)
			_pBottomPage->SetPageCount(_curpagecount);
		if(refreshdata)
			RefreshData();
		return;
	}

	int vcount=(_lastrect.right-_lastrect.left-20)/190;
	int hcount=(_lastrect.bottom-_lastrect.top-75-(_pagetype==SEARCH_PAGE?130:0))/88;
	if(vcount-newvcount<=-1||vcount-newvcount>=1||hcount-newhcount<=-1||hcount-newhcount>=1)
	{
		_lastrect=rc;
		bool smaller= ((newvcount*newhcount)<(vcount*hcount));
		int smallercount=smaller?newvcount*newhcount:vcount*hcount;
		int startcount=((_curaPage-1)*vcount*hcount+1)<_totalSoftCount?((_curaPage-1)*vcount*hcount+1):_totalSoftCount;
		int index=(startcount+vcount*hcount)%(newvcount*newhcount);
		startcount+=((newvcount*newhcount)-index)/2;
		int page=(int)ceil((double)startcount/(newvcount*newhcount));
		_curaPage=page;
		_curpagecount=newhcount*newvcount;
		if(_pToppage)
		{
			_pToppage->SetPageCount(_curpagecount);
			_pToppage->SetTotalCount(_totalSoftCount);
			_pToppage->SetPageIndex(_curaPage-1);
		}
		else if(_pBottomPage)
		{
			_pBottomPage->SetPageCount(_curpagecount);
			_pBottomPage->SetTotalCount(_totalSoftCount);
			_pBottomPage->SetPageIndex(_curaPage-1);
		}
		if(refreshdata)
			RefreshData();
		else
			_needrefreshdata=true;
	}

	if(_pBottomcategoryui)
	{
		_pBottomcategoryui->OnRectChanged();
	}
}

void WebAppHomeUI::OnRefresh()
{
	if(_pagetype==SEARCH_PAGE)
	{
		WebSoftControlUI* pWebsoftui= dynamic_cast<WebSoftControlUI*>(_pParentComposite);
		if(pWebsoftui)
		{
			pWebsoftui->GetWebsoftModel()->Get24HoursHotAppList();
			ShowLoadingData();
		}
	}
	else
		RefreshData();
}

std::wstring WebAppHomeUI::GetStyleID()
{
	return _T("WebAppHomeComposite");
}

void WebAppHomeUI::Init( CContainerUI* pContainer )
{
	BaseView::Init(pContainer);
}
