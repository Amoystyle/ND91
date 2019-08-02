#pragma once
#include "WebUIReceiver.h"

class WebSoftData;
class WebAppListItemFloatLayer;
class WebAppListItemControl :
	public BaseView,
	public WebUIReceiver
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"BtnHover", OnBtnClick);
	UIEND_MSG_MAP
	WebAppListItemControl(HWND hWnd,const WebSoftData* pSoftBase);
	~WebAppListItemControl(void);

	void SetStyle( int style );
	void OnCreate();
	void SetIcon( wstring path);

	wstring GetStyleID();

	void Init(CContainerUI* pContainer);

private:
	const WebSoftData* m_pSoftData;
	CLayoutUI* m_pLayout;
	CControlUI* m_pIcon;
	CLayoutUI* m_pConScore;
	CLabelUI* m_pLbName;
	CLabelUI* m_pLbDownNum;
	CButtonUI* m_pBtnHover;
	int m_iStyle;
private:
	void InitControls();
	//所有控件单独注册事件（Layout无法接受鼠标事件）
	bool OnEventHover( TEventUI& event );

	bool OnBtnClick(TNotifyUI* pNotify);

	//WebAppListItemFloatLayer* _pAppItemFloatlayer;
};
