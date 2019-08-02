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
	//应用按钮
	bool  OnApplyBtn(TNotifyUI* pNotify);
	//确认按钮
	bool  OnOkBtn(TNotifyUI* pNotify);
	//取消按钮
	bool  OnCancelBtn(TNotifyUI* pNotify);
	
	//激活应用按钮
	bool ActivationApplyBtn(bool flag);

private:
	SystemSetDlgModel _model;
	SystemSetUI * _systemsetui;
	DownLoadSystemSetUI *_downLoadSystemSetUI;
	SystemSetTabUI * _systemSetTabUI;
};

