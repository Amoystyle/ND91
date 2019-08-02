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
	virtual void SetCheck(wstring name,bool flag);		//����checkbox����
	virtual void SetRadio(wstring name);				//����radioѡ��
	
	virtual bool GetCheck(wstring name) ;				//��ȡcheckbox����
	virtual bool GetRadio( wstring name ) ;				//��ȡradioѡ��
	
	void Save_Data();		//�û����ȷ�ϣ��������ݵ�ini�ļ�
	void SetPoint(void *p);	//���dlgָ��,���Ӧ�ð�ť

	virtual void OnCreate();
private:
	SystemSetModel _model;
	void *_dlgpoint;

};
