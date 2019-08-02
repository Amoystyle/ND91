#pragma once
#include "../Logic/UserCenterInfoMsg.h"

// �û����봰�ڹ�����
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

	// �˳��û���¼����
	bool OnExitBnt(TNotifyUI* pNotify);


private:


    void ReleaseData();


	virtual bool Handle(const CheckConnectInfoMsg* pMsg);


	// Summary  : ��ȡ�û��Ƿ���ҳ����ʼ����
	// Returns  : bool
	// Parameter: LPCWSTR pszURL
	bool OnBeforeNavigate(LPCWSTR pszURL){return true;}

	// Summary  : ��ȡ�û��Ƿ�ɹ���½
	// Returns  : bool
	// Parameter: LPCWSTR pszURL
	bool OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL);

	void OnNavigateComplete(IDispatch *pDispatch, LPCWSTR pszURL){;}

	// DocumentComplete���û��ɹ���¼�Ĵ���
	int _nLoginTime;
};
