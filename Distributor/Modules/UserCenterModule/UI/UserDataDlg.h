#pragma once
#include "../Logic/SaveUserInfoModel.h"
#include "../Logic/UserCenterInfoMsg.h"

// �����û����Ϲ�����
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

	// ���水ť
	bool OnSaveBnt(TNotifyUI* pNotify);

	// ȡ����ť
	bool OnCancelBnt(TNotifyUI* pNotify);

	// ���û���ͼƬ
	bool SetDeviceImage(wstring imgPath, bool isPad = false);

	// ��ʾ�û�����
	bool ShowUserInfo(wstring name, wstring phone, wstring comment);

	void SetUserInfoModel( SaveUserInfoModel* pModel ) { _pUserInfoModel = pModel;}

private:

	virtual bool Handle(const SendCustomerInfoMsg* pMsg);

	bool isNumString(wstring str);

	SaveUserInfoModel*	_pUserInfoModel;

};
