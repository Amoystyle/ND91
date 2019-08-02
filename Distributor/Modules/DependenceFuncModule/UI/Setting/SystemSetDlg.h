#pragma once
#include "Logic/Setting/SystemSetDlgInterface.h"
#include "Logic/Setting/SystemSetDlgModel.h"

#include "SystemSetTabUI.h"


class CCompositeTabUI;

class SystemSetDlg : public CWindowUI ,public SystemSetDlgInterface
{
public:
    virtual void OnSetWindowRgn(){};
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Apply", OnApplyBtn)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"OK", OnOkBtn)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Cancel", OnCancelBtn)
	UIEND_MSG_MAP
public:
	SystemSetDlg();
	~SystemSetDlg();
 	virtual void OnCreate();
	//Ӧ�ð�ť
	bool  OnApplyBtn(TNotifyUI* pNotify);
	//ȷ�ϰ�ť
	bool  OnOkBtn(TNotifyUI* pNotify);
	//ȡ����ť
	bool  OnCancelBtn(TNotifyUI* pNotify);
	
	//����Ӧ�ð�ť
	bool ActivationApplyBtn(bool flag);

private:
	SystemSetDlgModel _model;
	SystemSetUI * _systemsetui;
	DownLoadSystemSetUI *_downLoadSystemSetUI;
	SystemSetTabUI * _systemSetTabUI;
};

