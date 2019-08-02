#pragma once

class MainView;
class ConnectGuideController
	: BaseHandler<DeviceConnectInitMsg>
	, BaseHandler<DeviceConnectWaitWindowsMsg>
	, BaseHandler<DeviceDriverInstallInitMsg>
	, BaseHandler<DeviceDriverInstallProgressMsg>
	, BaseHandler<DeviceConnectErrorMsg>
	, BaseHandler<DeviceConnectMsg>
{
public:
	ConnectGuideController(wstring deviceID, void* phone);
	~ConnectGuideController();

	bool Init(CContainerUI* pContainer);

	void Show();

	void Hide();

	static DWORD WINAPI StateTest(PVOID lpParam);

	virtual bool Handle( const DeviceConnectInitMsg* pMsg );

	virtual bool Handle( const DeviceDriverInstallInitMsg* pMsg );

	virtual bool Handle( const DeviceDriverInstallProgressMsg* pMsg );

	virtual bool Handle( const DeviceConnectErrorMsg* pMsg );

	virtual bool Handle( const DeviceConnectMsg* pMsg );

	virtual bool Handle( const DeviceConnectWaitWindowsMsg* pMsg );

private:
	MainView* _pMainView;

	wstring _deviceID;
	void* _pPhone;
};