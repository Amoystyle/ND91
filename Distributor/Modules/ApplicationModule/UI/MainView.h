#pragma once

class ApplicationController;
class AblAppItem;

class MainView
	: public BaseView
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"AppTab", OnAppTabChange)

		EVNET_ID_HANDLER(UINOTIFY_EN_CHANGE,L"Edit", OnSearch)
		EVNET_ID_HANDLER(UINOTIFY_CLICK,L"Clear", OnSearchClear)
		
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"ClearFilterBtn", OnClearFilterBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"CloseFilterInfoBtn", OnCloseFilterInfoBtnClick)
	UIEND_MSG_MAP

	MainView(ApplicationController* pController);
	~MainView();

	void AddItemView(AblAppItem* appItem){}

	void RemoveItemView(wstring appId){}

	void UpdateItemView(AblAppItem* appItem){}

	void UpdateSDCardState(bool bHaveSDCard = false);

	void ClearItems(){}

	void ShowLoading(bool bShow = true){}

	void Release();

private:
	void OnCreate();

	bool OnAppTabChange(TNotifyUI* pNotify);

	bool OnSearch(TNotifyUI* pNotify);

	bool OnSearchClear(TNotifyUI* pNotify);

	bool OnClearFilterBtnClick(TNotifyUI* pNotify);

	bool OnCloseFilterInfoBtnClick(TNotifyUI* pNotify);

	void ShowFilterInfo( bool show );

	void RefreshUnstallBtn(bool bShow);

	void RefreshMoveBtn(bool bShow);

private:
	virtual wstring GetStyleID();

	ApplicationController* _pController;
};