#pragma once
#include "../Logic/UserCenterInfoMsg.h"

// 用户登入窗口管理类
class LoginDlg : public CWindowUI, public IWebObserver, public BaseHandler<CheckConnectInfoMsg>
{
public:
    virtual void OnSetWindowRgn(){};
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"closedlg", OnExitBnt)
	UIEND_MSG_MAP

	LoginDlg(void);
	virtual ~LoginDlg(void);

 	virtual void OnCreate();

	// 退出用户登录窗口
	bool OnExitBnt(TNotifyUI* pNotify);


private:


    void ReleaseData();


	virtual bool Handle(const CheckConnectInfoMsg* pMsg);


	// Summary  : 获取用户是否点击页面起始操作
	// Returns  : bool
	// Parameter: LPCWSTR pszURL
	bool OnBeforeNavigate(LPCWSTR pszURL){return true;}

	// Summary  : 获取用户是否成功登陆
	// Returns  : bool
	// Parameter: LPCWSTR pszURL
	bool OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL);

	void OnNavigateComplete(IDispatch *pDispatch, LPCWSTR pszURL){;}

	// DocumentComplete后用户成功登录的次数
	int _nLoginTime;
};
