#pragma once
#include "UI/TrayIcon.h"

class SysSettingController
	: public INotifyUI

{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"BtnSystemMenu", OnBtnSystemMenu)
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,MENU_ID_SYSTEMMENU,OnMenuClick);
	UIEND_MSG_MAP
public:
	SysSettingController();
	~SysSettingController();

	bool  OnMenuClick( TNotifyUI* pNotify );

	bool  OnBtnSystemMenu(TNotifyUI* pNotify);

	void Init(CContainerUI* pTitleToolBar, CContainerUI* pStatusBar);
};