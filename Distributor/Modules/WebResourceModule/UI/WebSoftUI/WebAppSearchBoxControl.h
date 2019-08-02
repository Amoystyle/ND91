#pragma once

#include "WebUIReceiver.h"

//说明：
//1.需要设置消息接收者 SetReceive
//2.接收者添加消息处理
//    消息类型：WebAppSearchBox_Search
//    wParam：  要搜索的文本
//3.Style:HomeSearchBox
class WebAppSearchBoxControl
	: public BaseView
	, public WebUIReceiver
{
public:
	enum event
	{
		WebAppSearchBox_Search,
	};
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_EN_KEYDOWN, L"Edit", OnKeyDown);
		EVNET_ID_HANDLER(UINOTIFY_KILLFOCUS,  L"Edit", OnKillFocus);
		EVNET_ID_HANDLER(UINOTIFY_SETFOCUS,   L"Edit", OnSetFocuns);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_search", OnSearchBtn);
	UIEND_MSG_MAP

	WebAppSearchBoxControl(void);
	~WebAppSearchBoxControl(void);


	wstring GetStyleID();

	void Init(CContainerUI* pContainer);
private:

	void OnCreate();

	bool OnKeyDown(TNotifyUI* pNotify);
	bool OnKillFocus(TNotifyUI* pNotify);
	bool OnSetFocuns(TNotifyUI* pNotify);

	bool OnSearchBtn(TNotifyUI* pNotify);

private:
	INotifyUI* _pINotifyUI;
	CEditUI* _pEdit;
};
