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
	void SetPoint(void *p);	//���dlgָ��,���Ӧ�ð�ť

private:
	//tab��Ӧ
	bool  OnTabChange(TNotifyUI* pNotify);

	virtual void OnCreate();

private:
	DownLoadSystemSetUI * _downLoadSystemSetUI;
	SystemSetUI * _systemSetUI;
};

