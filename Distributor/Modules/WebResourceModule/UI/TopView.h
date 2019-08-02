#pragma once

using namespace std;

class TopView
	: public BaseView
{
public:
	// 消息映射
	UIBEGIN_MSG_MAP
	EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"WebRes_PhoneType_Tab", OnWebPlatFormSelChanged);//右边tab选中事件
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_goback", OnBtnCk);//后退按钮响应事件
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_goforword",OnBtnCk); //前进按钮响应事件
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_gohome",OnBtnCk);//回到首页按钮响应事件
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_refresh",OnRefresh);//刷新按钮响应事件
	EVNET_ID_HANDLER(UINOTIFY_CLICK, L"web_search",OnSearch);//搜索按钮响应事件

	EVNET_ID_HANDLER(UINOTIFY_EN_LIMIT,L"web_search_edit", OnSearch);//编辑框输入完成
	EVNET_ID_HANDLER(UINOTIFY_EN_KEYDOWN,L"web_search_edit", OnSearch);//编辑框输入完成
	EVNET_ID_HANDLER(UINOTIFY_EN_CHANGE,L"web_search_edit", OnSearchEditChange);
	EVNET_ID_HANDLER(UINOTIFY_CLICK,L"web_searchBox_Clear", OnSearchClear);//清空编辑框输入内容
	EVNET_ID_HANDLER(UINOTIFY_CLICK,L"refresh", OnRefresh);//刷新
	UIEND_MSG_MAP

	TopView(CContainerUI* pParent=NULL);
	~TopView(void);

	void OnCreate();

	wstring GetStyleID();

	bool OnRefresh(TNotifyUI* pNotify);
	//点击左边树节点，需要传节点消息近来
	void ClickLeftTree(NetNode& stNetNode);
	//是否隐藏搜索功能
	void HideSearch(bool bHide=true);

private:

	bool OnWebPlatFormSelChanged(TNotifyUI* pNotify);

	//点击按钮事件
	bool OnBtnCk(TNotifyUI* pNotify);

	//搜索
	bool OnSearch(TNotifyUI* pNotify);

	bool OnSearchClear(TNotifyUI* pNotify);

	bool OnSearchEditChange(TNotifyUI* pNotify);
	//
	bool OnBeforeNavigate( LPCWSTR pszURL);

	bool OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL);


private:
	ENUM_PLATFORM _ePlatform;
	NetNode       _stNetNode;
};
