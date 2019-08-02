#pragma once
#include "ui/WebSoftUI/WebUIReceiver.h"
#include "UI/TabItem.h"

class WebSoftControlUI;
class WebSoftMainUI: public BaseView,public WebUIReceiver
{
public:
	enum PlatForm
	{
		Web_Android,
		Web_iPhone,
		Web_iPad
	};

	enum event
	{
		WebSoftMainUI_TabAndroid_Click,
	};

	WebSoftMainUI(HWND hWnd);
	~WebSoftMainUI(void);

	UIBEGIN_MSG_MAP
//		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"WebSoft_PhoneType_Tab", OnWebPageSelChanged);
//		EVNET_ID_HANDLER(UINOTIFY_EN_LIMIT,L"web_search_edit", OnSearch);//编辑框输入完成
//		EVNET_ID_HANDLER(UINOTIFY_EN_KEYDOWN,L"web_search_edit", OnSearch);//编辑框输入完成
//		EVNET_ID_HANDLER(UINOTIFY_EN_CHANGE,L"web_search_edit", OnSearchEditChange);
//		EVNET_ID_HANDLER(UINOTIFY_CLICK,L"web_searchBox_Clear", OnSearchClear);//清空编辑框输入内容
// 		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_gohome",OnBtnGoHome);//回到首页按钮响应事件
// 		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_refresh",OnRefresh);//刷新按钮响应事件
//		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"web_search",OnSearch);//搜索按钮响应事件
//		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_goback",OnGoBack);

	UIEND_MSG_MAP

	void SetPlatform(bool IsIpad);

//	int GetSelPlatformIndex();

	wstring GetStyleID();

	void Init(CContainerUI* pContainer);

	
public:
	void OnCreate();

	bool OnWebPageSelChanged(int sel);	//页面切换

	bool OnSearch(wstring searchbuf);		//搜索

//	bool OnSearchClear(TNotifyUI* pNotify);

//	bool OnSearchEditChange(TNotifyUI* pNotify);

	bool OnBtnGoHome();			//主页

	bool OnRefresh();		//刷新

	bool OnGoBack();		//后退


private:

	WebSoftControlUI*  _piPhoneWebSoftUI;
	WebSoftControlUI*  _piPadWebSoftUI;
	PlatForm           _curplat;

};
