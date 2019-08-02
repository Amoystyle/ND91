#pragma once

#include "WebUIReceiver.h"

//说明：
//1.需要设置消息接收者 SetReceive
//2.接收者添加消息处理
//    消息类型：WebAppListFilter_Click
//    wParam：  Filter
//3.Style:WebAppFilterCtrl
class WebAppListFilterControl
	: public BaseView
	, public WebUIReceiver
{
public:
	enum event
	{
		//过滤按钮Click
		WebAppListFilter_Click,
	};
	enum Filter
	{
		//下载次数
		DownloadLevel,
		//价格高低
		PriceLevel,
		//版本更新
		VersionLevel,
	};

	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_downloadlevel", OnBtnClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_pricelevel", OnBtnClick);
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"btn_versionlevel", OnBtnClick);
	UIEND_MSG_MAP

public:
	WebAppListFilterControl(HWND hWnd);
	~WebAppListFilterControl(void);

	wstring GetStyleID();

	void Init(CContainerUI* pContainer);

private:
	bool OnBtnClick(TNotifyUI* pNotify);
	map<Filter,bool> _updowns;
};
