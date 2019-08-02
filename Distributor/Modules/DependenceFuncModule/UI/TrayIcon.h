#pragma once

#define MENU_ID_SYSTEMMENU 26000

class CTrayIcon 
	: public CCompositeUI
	, public CWindowUI
{
public:
	UIBEGIN_MSG_MAP
		EVNET_MENU_ID_HANDLER(UINOTIFY_CLICK,MENU_ID_SYSTEMMENU,OnMenuClick);
	UIEND_MSG_MAP

	CTrayIcon(void);
	~CTrayIcon(void);

	//初始化托盘图标，在窗口创建完毕后创建
	void InitTrayIcon();

	bool OnTrayClick( WPARAM wParam,LPARAM lParam );

	void Close();

	// 菜单事件
	bool OnMenuClick(TNotifyUI* pNotify);

	void CloseTray();
private:
	NOTIFYICONDATA m_tnid;
};
