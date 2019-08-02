#pragma once
#include "../Logic/UserCenterInfoMsg.h"
#include "Common/UserCenter/UserCenterMsg.h"
#include "../Logic/SaveUserInfoModel.h"
#include "Framework/Singleton/Manager/EventManager.h"

class MainView
	: public BaseView
	, public BaseHandler<ResponseUserLoginMsg>
	, public BaseHandler<GetCustomerInfoMsg>
	, public BaseHandler<SendCustomerInfoMsg>
	, public IExit
{
public:
	
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Button_Goto", OnUserGotoClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Button_Exit", OnUserExitClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Button_Login", OnUserLoginClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Button_Register", OnUserRegisterClick)
	UIEND_MSG_MAP

	MainView();
	~MainView();

	void StatusChange( bool isLongin, wstring name = L"" );

private:

	virtual wstring GetStyleID();

	bool OnUserLoginClick(TNotifyUI* pNotify = NULL);

	bool OnUserExitClick(TNotifyUI* pNotify = NULL);

	bool OnUserGotoClick(TNotifyUI* pNotify = NULL);

	bool OnUserRegisterClick(TNotifyUI* pNotify = NULL);

	virtual bool Handle(const ResponseUserLoginMsg* pMsg);

	virtual bool Handle(const GetCustomerInfoMsg* pMsg);

	virtual bool Handle(const SendCustomerInfoMsg* pMsg);

	void Exit();
private:

	SaveUserInfoModel*	_pUserInfoModel;

};