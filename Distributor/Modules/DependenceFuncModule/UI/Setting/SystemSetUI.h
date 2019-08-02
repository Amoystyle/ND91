#pragma once
#include "Logic/Setting/SystemSetModel.h"
#include "Logic/Setting/SystemSetInterface.h"



class SystemSetUI 
    : public CTabItem
    , public SystemSetInterface
{
public:
	SystemSetUI();
	~SystemSetUI();
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"ContactPerson", OnContactPersonClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Minimize_to_tray", OnMinimizeToTrayClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"ExitApp", OnExitAppClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"DownLoadMsg", OnDownLoadMsgCilck)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"SoftInstallMsg",OnSoftInstallMsg)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AutoDeleteTask",OnAutoDeleteTask)
	UIEND_MSG_MAP

private:

	bool  OnContactPersonClick(TNotifyUI* pNotify);

	bool  OnMinimizeToTrayClick(TNotifyUI* pNotify);

	bool  OnExitAppClick(TNotifyUI* pNotify);

	bool  OnDownLoadMsgCilck(TNotifyUI* pNotify);

	bool  OnSoftInstallMsg(TNotifyUI* pNotify);

	bool  OnAutoDeleteTask(TNotifyUI* pNotify);

public:
	virtual void SetCheck(wstring name,bool flag);		//设置checkbox属性
	virtual void SetRadio(wstring name);				//设置radio选择
	
	virtual bool GetCheck(wstring name) ;				//获取checkbox属性
	virtual bool GetRadio( wstring name ) ;				//获取radio选择
	
	void Save_Data();		//用户点击确认，保存数据到ini文件
	void SetPoint(void *p);	//获得dlg指针,针对应用按钮

	virtual void OnCreate();
private:
	SystemSetModel _model;
	void *_dlgpoint;

};
