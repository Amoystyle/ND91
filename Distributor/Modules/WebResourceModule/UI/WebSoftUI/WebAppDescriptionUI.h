#pragma once
#include "Logic/WebSoftInfo.h"
#include "WebUIReceiver.h"
#include "UI/WebIEUI.h"

class MessageBoxUI;
//说明：
//1.需要设置消息接收者 SetReceive
//2.接收者添加消息处理
//    消息类型：WebAppDescription_Install
//    wParam：  AppId
class WebAppDescriptionUI
	: public WebUIReceiver
	, public IWebObserver
	, public BaseView
{
public:
	enum event
	{
		WebAppDescription_Install
	};

	WebAppDescriptionUI(HWND hWnd);
	~WebAppDescriptionUI(void);

	void ShowAppItemDetail(int appID, PlatFrom platForm,bool Ishot24);

	void LoadCompleted();

	bool OnBeforeNavigate( LPCWSTR pszURL);
	bool OnDocumentComplete(IDispatch *pDisp, LPCWSTR pszURL);
	void OnNavigateComplete(IDispatch *pDispatch, LPCWSTR pszURL);

	void OnRefresh();

	wstring GetStyleID();

	void Init(CContainerUI* pContainer);

private:
	void OnCreate();

private:
	HWND _lastMsgBoxHanle;
	MessageBoxUI* _box;
	PlatFrom _mPlatForm;
	CWebIEUI* _pIE;
	CAnimationUI* _pLoading;
	//当前网页类型名，“分类名_平台”
	tstring  _contenttype; 
	wstring m_sURL;

	bool    _IsHot24;
	bool    _bDownLoad;
};
