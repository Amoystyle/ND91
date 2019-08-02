#pragma once

#define MENU_ID_SYSTEMSET		26001
#define MENU_ID_FEEDBACK		26002
#define MENU_ID_OFFICIALWEB		26003
#define MENU_ID_CHECKUP			26004
#define MENU_ID_SHOWWHATSNEW	26005
#define MENU_ID_ABOUT			26006
#define MENU_ID_CLOSE			26007

class CTrayIcon;

class DependenceFuncCenterController
	: public BaseHandler<IconNotifyMsg>
	, public BaseHandler<MenuNotifyMsg>
	, public BaseHandler<WhatsNewDlgMsg>
	, public IExit
{
public:
	DependenceFuncCenterController();
	~DependenceFuncCenterController();

	void Init(CContainerUI* pTitleToolBar, CContainerUI* pStatusBar);

	bool Handle(const IconNotifyMsg* pMsg);

	bool Handle(const MenuNotifyMsg* pMsg);

	bool Handle(const WhatsNewDlgMsg* pMsg);

	void Exit();

private:
	CTrayIcon* _pTrayIcon;

};