#pragma once
#include "WebUIReceiver.h"

//说明：
//1.需要设置消息接收者 SetReceive
//2.接收者添加消息处理
//    消息类型：WebAppListPager_Event
//    wParam：  当前页Index
//3.Style:WebAppPagerCtrl
class WebAppListPagerControl
	: public BaseView
	, public WebUIReceiver
{
public:
	enum event
	{
		//分页事件
		WebAppListPager_Event
	};

	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_first", OnFirstBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_pre", OnPreBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_next", OnNextBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_last", OnLastBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_page", OnPageBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_goto", OnGotoBtnClick)
		EVNET_ID_HANDLER(UINOTIFY_EN_KEYDOWN, L"gotoEdit", OnKeyDown);
	UIEND_MSG_MAP

	WebAppListPagerControl(HWND hWnd,bool isTop = true, int pageSize = 20);
	~WebAppListPagerControl(void);

public:
	void SetTotalCount(int count = 0);

	void SetPageCount(int pageSize = 20);

	void SetPageIndex(int index=0);

	wstring GetStyleID();

	void Init(CContainerUI* pContainer);
private:
	void OnCreate();
	void UpdatePageInfo();
	void CreatePager();
	bool FixPageBar();
	bool GotoPage();

	bool OnFirstBtnClick(TNotifyUI* pNotify);
	bool OnPreBtnClick(TNotifyUI* pNotify);
	bool OnNextBtnClick(TNotifyUI* pNotify);
	bool OnLastBtnClick(TNotifyUI* pNotify);
	bool OnPageBtnClick(TNotifyUI* pNotify);
	bool OnGotoBtnClick(TNotifyUI* pNotify);
	bool OnKeyDown(TNotifyUI* pNotify);
private:
	bool _isTop;
	
	CLayoutUI* _pTopModeLayout;
	CLayoutUI* _pBottomModeLayout;
	CLabelUI* _pTopPageInfo;
	CLabelUI* _pBottomPageInfo;
	CLayoutUI* _pContainer;
	CEditUI* _pEdit;

	int _recordCount;
	int _recordPageSize;
	int _recordPageCount;
	int _recordPageIndex;

	int _pageCount;
	int _pagePageSize;
	int _pagePageCount;
	int _pagePageIndex;
};
