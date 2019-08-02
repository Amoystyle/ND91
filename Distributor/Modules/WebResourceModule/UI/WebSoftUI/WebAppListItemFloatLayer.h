#pragma once
#include "WebUIReceiver.h"

class WebSoftData;
class MessageBoxUI;
class WebAppListItemFloatLayer:
	public CWindowUI,public WebUIReceiver
{
public:
	enum event
	{
		WebAppItemGotoDescription_Click
	};
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"BtnDownLoad", OnDownLoadBtnClick)
	UIEND_MSG_MAP
	WebAppListItemFloatLayer();
	~WebAppListItemFloatLayer(void);

	static WebAppListItemFloatLayer* GetInstance();
	void Show(HWND hParent,const WebSoftData* pSoftBase,RECT rect);
	void Hide();
	bool m_bIsHide;
private:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void initcontrols(const WebSoftData* pSoftBase);

	void OnCreate();
private:
	HWND m_hparHWND;
	const WebSoftData* m_pSoftData;
	bool OnDownLoadBtnClick(TNotifyUI* pNotify);
	bool OnMainBtnClick(TNotifyUI* pNotify);
	MessageBoxUI* _box;
	static  WebAppListItemFloatLayer* _instance;
	RECT m_rect;
};
