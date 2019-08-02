#pragma once
#include "UI/ApplicationInfoUI.h"

class ApplicationController	
	: public BaseHandler<NotifyInstallAppMsg>
	, public BaseHandler<AppThreadMsg>
	, public BaseHandler<NotifyCenterInfoMsg>
	, public BaseHandler<NotifyTaskItemMsg>
	, public BaseHandler<NotifyRefreshModuleMsg>
{
public:
	ApplicationController(wstring deviceID, void* phone);
	~ApplicationController();

	bool Init(CContainerUI* pContainer);

	void Show();

	void Hide();

	void Release();

	wstring GetDeviceID();

	virtual bool Handle(const NotifyInstallAppMsg* pMsg);

	virtual bool Handle(const AppThreadMsg* pMsg);
	virtual bool Handle(const NotifyCenterInfoMsg* pMsg);
	virtual bool Handle(const NotifyTaskItemMsg* pMsg);
	virtual bool Handle(const NotifyRefreshModuleMsg* pMsg);

	bool InstallApp(bool isFile, wstring wsTitle=L"");
	bool InstallApp( vector<wstring> files );

private:
	wstring _deviceID;
	void* _nPhone;

	ApplicationInfoUI* _pAppInfoUI;
};