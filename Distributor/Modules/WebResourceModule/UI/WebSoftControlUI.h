#pragma once
#include "ui/WebSoftUI/WebUIReceiver.h"
#include "ui/WebSoftUI/WebAppSearchBoxControl.h"
#include "ui/WebSoftUI/WebAppListItemFloatLayer.h"
#include "ui/WebSoftUI/WebAppListItemControl.h"

class WebAppHomeUI;
class WebSoftModel;
class WebSoftCategory;
class WebAppDescriptionUI;
class WebSoftControlUI:public BaseView, public WebSoftMessageReceiver ,public BaseHandler<WebSoftMsg>
{
public:
	WebSoftControlUI(HWND hWnd,bool IsPlatformiPad);
	~WebSoftControlUI(void);
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(WebAppSearchBoxControl::WebAppSearchBox_Search,L"HomeSearchBox",OnClickSearchbox);
		EVNET_ID_HANDLER(WebAppListItemFloatLayer::WebAppItemGotoDescription_Click, L"AppItemFloat_Layout",OnClickGotoDes );
	UIEND_MSG_MAP
	void ShowSearchHome(int page=-1);

	void ShowSearchResult(wstring searchtext=L"", int page=-1);

	void ShowCategoryUI(int selID=0);

	WebSoftModel* GetWebsoftModel();

	void GoBack();

	void OnRefresh();

	std::wstring GetStyleID();
private:
	void OnCreate();

	bool OnClickSearchbox(TNotifyUI* pNotify);

	bool OnClickGotoDes(TNotifyUI* pNotify);

	WebAppHomeUI* WebSoftControlUI::CreateAppHomeUI(int type);

	void GotoDescription(int id, bool IsHot24);

	virtual void SetRect(RECT& rc);

	virtual bool Handle(const WebSoftMsg* pMsg);
private:

	bool   _platformiPad;
	WebAppHomeUI* _pSearchMainUI;//搜索主页
	WebAppHomeUI* _pSearchResultUI;//搜索结果页
	WebAppHomeUI* _pCatagoryUI;//分类显示页

	WebAppDescriptionUI* _pDescriptionUI;
	CCompositeUI* _pCurVisibleUI;

	WebSoftModel* _pWebsoftModel;

	vector<WebSoftCategory*>  _pWebSoftCategorys;

};
