#pragma once
#include "Logic/Setting/DownLoadSystemSetModel.h"
#include "Logic/Setting/DownLoadSystemSetInterface.h"

#define IosDir _T("IosDir")
#define AndroidDir _T("AndroidDir")

class DownLoadSystemSetUI 
    : public CTabItem
    , public DownLoadSystemSetInterface
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"IosSelect", OnIosSelectBtn)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"IosSetting", OnIosSettingBtn)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AndroidSelect", OnAndroidSelectBtn)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AndroidSetting", OnAndroidSettingBtn)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"AutoDelUpdatedApp", OnAutoDelUpdatedApp)
	UIEND_MSG_MAP
public:
	DownLoadSystemSetUI();
	~DownLoadSystemSetUI();

	virtual wstring GetIosDir();				//获取ios目录
	virtual void SetIosDir(wstring dir);		//设置ios目录

	virtual wstring GetAndroidDir();			//获取Android目录
	virtual void SetAndroidDir(wstring dir);	//设置Android目录


	virtual bool GetCheck(wstring name);		//获取checkbox属性
	virtual void SetCheck(wstring name,bool flag);		//设置checkbox属性

	void Save_Data();		//保存数据

	void SetPoint(void *p);	//获得dlg指针,针对应用按钮

	void OpenFolder(wstring fonlder_dir);

private:
	//ios查看按钮
	bool OnIosSelectBtn(TNotifyUI* pNotify);

	//ios修改按钮
	bool OnIosSettingBtn(TNotifyUI* pNotify);

	//android查看按钮
	bool OnAndroidSelectBtn(TNotifyUI* pNotify);

	//android修改按钮
	bool OnAndroidSettingBtn(TNotifyUI* pNotify);

	//升级设置CheckBox
	bool OnAutoDelUpdatedApp(TNotifyUI* pNotify);

	virtual void OnCreate();
private:
	DownLoadSystemSetModel _model;
	void *_dlgpoint;
};

