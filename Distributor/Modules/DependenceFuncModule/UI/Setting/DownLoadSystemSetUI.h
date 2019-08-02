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

	virtual wstring GetIosDir();				//��ȡiosĿ¼
	virtual void SetIosDir(wstring dir);		//����iosĿ¼

	virtual wstring GetAndroidDir();			//��ȡAndroidĿ¼
	virtual void SetAndroidDir(wstring dir);	//����AndroidĿ¼


	virtual bool GetCheck(wstring name);		//��ȡcheckbox����
	virtual void SetCheck(wstring name,bool flag);		//����checkbox����

	void Save_Data();		//��������

	void SetPoint(void *p);	//���dlgָ��,���Ӧ�ð�ť

	void OpenFolder(wstring fonlder_dir);

private:
	//ios�鿴��ť
	bool OnIosSelectBtn(TNotifyUI* pNotify);

	//ios�޸İ�ť
	bool OnIosSettingBtn(TNotifyUI* pNotify);

	//android�鿴��ť
	bool OnAndroidSelectBtn(TNotifyUI* pNotify);

	//android�޸İ�ť
	bool OnAndroidSettingBtn(TNotifyUI* pNotify);

	//��������CheckBox
	bool OnAutoDelUpdatedApp(TNotifyUI* pNotify);

	virtual void OnCreate();
private:
	DownLoadSystemSetModel _model;
	void *_dlgpoint;
};

