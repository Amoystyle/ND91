#pragma once

#include "SystemSetUI.h"
#include "DownLoadSystemSetUI.h"


class SystemSetTabUI 
	: public CCompositeTabUI
	, public CTabItem
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"SystemSetTab", OnTabChange)
	UIEND_MSG_MAP
public:
	SystemSetTabUI();
	~SystemSetTabUI();

	void Save_Data();
	void SetPoint(void *p);	//获得dlg指针,针对应用按钮

private:
	//tab响应
	bool  OnTabChange(TNotifyUI* pNotify);

	virtual void OnCreate();

private:
	DownLoadSystemSetUI * _downLoadSystemSetUI;
	SystemSetUI * _systemSetUI;
};

