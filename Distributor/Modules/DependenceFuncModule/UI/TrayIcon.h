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

	//��ʼ������ͼ�꣬�ڴ��ڴ�����Ϻ󴴽�
	void InitTrayIcon();

	bool OnTrayClick( WPARAM wParam,LPARAM lParam );

	void Close();

	// �˵��¼�
	bool OnMenuClick(TNotifyUI* pNotify);

	void CloseTray();
private:
	NOTIFYICONDATA m_tnid;
};
