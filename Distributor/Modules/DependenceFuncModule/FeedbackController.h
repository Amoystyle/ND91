#pragma once
#include "UI/FeedbackUI.h"
#include "common/DependenceFuncCenter/DependenceFuncCenterMessage.h"

class FeedbackController
	: public INotifyUI
	, public BaseHandler<RequestFeedbackMsg>

{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"BtnFeedback", OnFeedbackBtn)
	UIEND_MSG_MAP
public:
	FeedbackController();
	~FeedbackController();

	bool  OnFeedbackBtn(TNotifyUI* pNotify);

	void Init(CContainerUI* pTitleToolBar, CContainerUI* pStatusBar);
	string GetOSName();
	std::string GetUrl();
	virtual bool Handle( const RequestFeedbackMsg* pMsg );


};