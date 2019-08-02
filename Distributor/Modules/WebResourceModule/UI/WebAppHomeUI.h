#pragma once

#include "ui/WebSoftUI/WebCategoriesControl.h"
#include "ui/WebSoftUI/WebAppListFilterControl.h"
#include "ui/WebSoftUI/WebAppListPagerControl.h"
#include "WebAppListUI.h"

class WebAppListUI;
class WebSoftData;
class WebSoftCategory;
class WebAppSearchBoxControl;
class WebUIReceiver;

//软件展示页面
class WebAppHomeUI:public BaseView
{
public:
	enum PAGETYPE//页面类型
	{
		SEARCH_PAGE,//搜索主页
		SEARCH_RESULT_PAGE,//搜索结果页
		CATEGORY_PAGE//分类页
	};

	WebAppHomeUI(HWND hWnd,PAGETYPE type, CCompositeUI* parentComposite=NULL);
	~WebAppHomeUI();

	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK,L"btnwebappCategory",OnClickCategorysBtn);
		EVNET_ID_HANDLER(WebCategoriesControl::WebCategories_Click,L"WebAppCategoryCtrl",OnClickCategorysBtn);
		EVNET_ID_HANDLER(WebAppListFilterControl::WebAppListFilter_Click,L"WebAppFilterCtrl",OnClickFilterBtn);
		EVNET_ID_HANDLER(WebAppListPagerControl::WebAppListPager_Event,L"WebAppPagerCtrl",OnClickPageBtn);

	UIEND_MSG_MAP

	void ShowSearchResultPage(wstring searchtext, int page);

	void ShowCategoryPage(int categoryId);

	void ShowSearchHomePage(int pageNum=1);

	void ShowCategorys(vector<WebSoftCategory*>* pWebSoftCategorys, int selID = 0 );

	void ShowAppIcon(int fid, wstring iconpath);

	void ShowList(int pageNum=1);

	void ShowLoadingData();//显示正在加载数据

	int  GetCurPageCount(){return _curpagecount;}

	void OnRefresh();

	void OnRectChanged(RECT& rc,bool refreshdata=false);

	wstring GetStyleID();

	void Init(CContainerUI* pContainer);

private:
	void OnCreate();

	void CreateHeader();

	void CreateBottom();

	bool OnClickCategorysBtn(TNotifyUI* pNotify);

	bool OnClickFilterBtn(TNotifyUI* pNotify);

	bool OnClickPageBtn(TNotifyUI* pNotify);

	void SetAppIcon(int fid, wstring iconpath);

	void ReleaseWebAppData();

	void RefreshData();

private:
	PAGETYPE   _pagetype;

	CCompositeUI* _pParentComposite;

	WebAppListUI*  _pWebAppList;

	WebCategoriesControl* _pBottomcategoryui;
	WebCategoriesControl* _pTopcategoryui;

	WebAppListPagerControl* _pToppage;
	WebAppListPagerControl* _pBottomPage;
	WebAppSearchBoxControl* _pSearchBoxUI;
	WebAppListFilterControl* _pListFilterUI;

	vector<WebSoftData*>  _pWebSofts;
	int  _totalSoftCount;

	int  _curcategoryId;

	int  _curaPage;
	wstring  _curSearchtext;
	string   _flitertypestr;

	int  _curpagecount;

	RECT   _lastrect;

	bool   _needrefreshdata;

};
