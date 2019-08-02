#pragma once

class LeftView;
class ModulesView;
class WelcomeView;
class DeviceContext;

class DeviceController
	: public BaseHandler<DeviceLinkInMsg>
	, public BaseHandler<DeviceLinkOutMsg>
	, public BaseHandler<DeviceConnectMsg>
	, public BaseHandler<DeviceActivatedMsg>
	, public BaseHandler<DeviceInnerMsg>
	, public BaseHandler<NotifyDeviceKeyChangeMsg>
	, public BaseHandler<DeviceConnectErrorMsg>
{
public:
	DeviceController();
	~DeviceController();

	bool Init(CContainerUI* pLeft, CContainerUI* pContainer);

	void Show();

	void Hide();

private:
	virtual bool Handle(const DeviceLinkInMsg* pMsg);

	virtual bool Handle(const DeviceLinkOutMsg* pMsg);

	virtual bool Handle( const DeviceConnectMsg* pMsg );

	virtual bool Handle( const DeviceActivatedMsg* pMsg );

	virtual bool Handle( const NotifyDeviceKeyChangeMsg* pMsg );

	virtual bool Handle( const DeviceInnerMsg* pMsg );

	virtual bool Handle( const DeviceConnectErrorMsg* pMsg );

	void CreateContext( wstring id, void* pPhone );

	void RemoveContext( void* pPhone );

	void Refresh();

	void ChangeView( void* key, DeviceModuleType type);

	void InitModule( void* key, DeviceModuleType type );
private:
	LeftView* _pLeftView;
	ModulesView* _pModulesView;
	WelcomeView* _pWelcomeView;

	DeviceContext* _pDeviceContext;
	map<void*, DeviceContext*> _mapContext;
};