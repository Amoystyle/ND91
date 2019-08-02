#pragma once
struct TopTaskCountMsg;
class MainView
	: public BaseView
	, BaseHandler<TopTaskCountMsg>
	, BaseHandler<NotifySourceModule>
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"TopButton", OnTopButtonClick)
	UIEND_MSG_MAP

	MainView();
	~MainView();

	void AddUserModule( SourceModule* pModule );

	void AddModule( SourceModule* pModule, bool bSelect = false);

	CContainerUI* GetTitleToolBar();

	CContainerUI* GetStatusBar();

	void SelectModuleButton( SourceModule* pModule );

private:
	virtual wstring GetStyleID();

	void OnCreate();

	bool OnTopButtonClick(TNotifyUI* pNotify);

	CCheckBoxUI* InsertTopButton(CContainerUI* bar, tstring id, tstring text, tstring icon);

	CContainerUI* GetTopBar();

	CContainerUI* GetUserLayout();

	CContainerUI* GetLeftLayout();

	CContainerUI* GetContainerLayout();

	virtual bool Handle( const TopTaskCountMsg* pMsg );

	virtual bool Handle( const NotifySourceModule* pMsg );

private:
	CCheckBoxUI* _pSelectBtn;

};