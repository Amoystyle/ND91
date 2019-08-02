#pragma once
#include "../Logic/SaveUserInfoModel.h"
#include "../Logic/UserCenterInfoMsg.h"

// 保存用户资料管理类
class UserDataDlg
	: public CWindowUI
	, public BaseHandler<SendCustomerInfoMsg>
{
public:
    virtual void OnSetWindowRgn(){};
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"save", OnSaveBnt)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"cancel", OnCancelBnt)
	UIEND_MSG_MAP

	UserDataDlg(void);
	virtual ~UserDataDlg(void);

 	virtual void OnCreate();

	// 保存按钮
	bool OnSaveBnt(TNotifyUI* pNotify);

	// 取消按钮
	bool OnCancelBnt(TNotifyUI* pNotify);

	// 设置机型图片
	bool SetDeviceImage(wstring imgPath, bool isPad = false);

	// 显示用户资料
	bool ShowUserInfo(wstring name, wstring phone, wstring comment);

	void SetUserInfoModel( SaveUserInfoModel* pModel ) { _pUserInfoModel = pModel;}

private:

	virtual bool Handle(const SendCustomerInfoMsg* pMsg);

	bool isNumString(wstring str);

	SaveUserInfoModel*	_pUserInfoModel;

};
