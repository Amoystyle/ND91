#pragma once

class VersionController
	: public INotifyUI
	, public BaseHandler<UpdateVersion_Msg>
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"BtnNewVersion", OnNewVersion)

	UIEND_MSG_MAP
public:
	VersionController();
	~VersionController();

	bool Handle(const UpdateVersion_Msg* pMsg);

	bool OnNewVersion(TNotifyUI* pNotify);

	void Update_Msg();

	void Init(CContainerUI* pTitleToolBar, CContainerUI* pStatusBar);

private:
	CButtonUI *_pbtn;
};