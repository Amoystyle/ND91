#pragma once

class MainView;
class DeviceInfoController
	: public BaseHandler<NotifyRefreshModuleMsg>
	, public BaseHandler<ResponseDeviceImageMsg>
	, public BaseHandler<ResponseDeviceSpaceMsg>
	, public BaseHandler<NotifyLoadDeviceInfoFinishMsg>
	, public BaseHandler<DeviceActivatedMsg>
{
public:
	DeviceInfoController(wstring deviceID, void* phone);
	~DeviceInfoController();

	bool Init(CContainerUI* pContainer);

	void Show();

	void Hide();

	virtual bool Handle( const NotifyRefreshModuleMsg* pMsg );

	virtual bool Handle( const ResponseDeviceImageMsg* pMsg );

	virtual bool Handle( const NotifyLoadDeviceInfoFinishMsg* pMsg );

	virtual bool Handle( const DeviceActivatedMsg* pMsg );

	virtual bool Handle( const ResponseDeviceSpaceMsg* pMsg );

private:
	wstring _deviceID;
	void* _pPhone;
	MainView* _pMainView;

	bool _bImgRrefreshing;
	bool _bInfoRefreshing;
	bool _bSpaceRefreshing;
	bool _bActiveRefreshing;
};