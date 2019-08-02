#pragma once

class CommonInfoView
	: public BaseView
	, public BaseHandler<ResponseDeviceBaseInfoMsg>
	, public BaseHandler<NotifyLoadDeviceInfoFinishMsg>
	, public BaseHandler<ResponseDeviceStateMsg>
	, public BaseHandler<ResponseDeviceNameMsg>
	, public BaseHandler<ResponseDeviceSpaceMsg>
	, public BaseHandler<ResponseDevicePowerInfoMsg>
	, public BaseHandler<ResponseDeviceImageMsg>
	, public BaseHandler<DeviceActivatedMsg>
	, public BaseHandler<ResponseSDCardStateMsg>
	, public BaseHandler<ResponseChangeInstallPositionMsg>
	, public BaseHandler<ResponseUserInfoMsg>
	, public BaseHandler<ResponseUserLoginMsg>
{
public:
	UIBEGIN_MSG_MAP
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Button_CopyToClipBoard", OnTopButtonClipBoardClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"SaveInfo", OnSaveInfoClick)
		EVNET_ID_HANDLER(UINOTIFY_CLICK, L"Button_OpenNotePad", OnButtonNotePadClick)
		EVNET_ID_HANDLER(UINOTIFY_SELCHANGE, L"InstallSpace", OnComboBoxNotify)
	UIEND_MSG_MAP
	CommonInfoView(wstring deviceID, void* phone);
	~CommonInfoView();

private:
	virtual wstring GetStyleID();

	void OnCreate();

	bool OnTopButtonClipBoardClick(TNotifyUI* pNotify = NULL);

	bool OnSaveInfoClick(TNotifyUI* pNotify = NULL);

	bool OnButtonNotePadClick(TNotifyUI* pNotify = NULL);

	bool OnComboBoxNotify(TNotifyUI* pNotify);

	virtual bool Handle( const ResponseDeviceBaseInfoMsg* pMsg );

	virtual bool Handle( const NotifyLoadDeviceInfoFinishMsg* pMsg );

	virtual bool Handle( const ResponseDeviceStateMsg* pMsg );

	virtual bool Handle( const ResponseDeviceNameMsg* pMsg );

	virtual bool Handle( const ResponseDeviceSpaceMsg* pMsg );

	virtual bool Handle( const ResponseDevicePowerInfoMsg* pMsg );

	virtual bool Handle( const ResponseDeviceImageMsg* pMsg );

	virtual bool Handle( const DeviceActivatedMsg* pMsg );

	virtual bool Handle( const ResponseSDCardStateMsg* pMsg );

	virtual bool Handle( const ResponseChangeInstallPositionMsg* pMsg );

	virtual bool Handle( const ResponseUserInfoMsg* pMsg );

	virtual bool Handle( const ResponseUserLoginMsg* pMsg );

	void InitIOSInfo( ResponseDeviceBaseInfoMsg* pMsg );

	void InitAndroidInfo( ResponseDeviceBaseInfoMsg* pMsg );

	bool ParseDeviceType( EnumPhonePlatForm nPlatForm );

	void SetItemInfo( CLayoutUI* pLayout, wstring strKey, wstring strValue );

	void SetDeviceName( wstring deviceHWModel );

	void UpdateAndroidSpace( AblSpaceInfoItem info );

	void UpdateIOSSpace( AblSpaceInfoItem info );

	void UpdateProgressBar( CLayoutUI* pLayout, wstring id, __int64 maxValue, __int64 nValue );

	void UpdateProgressLbl( CLayoutUI* pLayout, wstring id, __int64 nLeft );

	void UpdatePowerLbl( CLayoutUI* pLayout, int nPower );

	wstring CreateInfoString();

	wstring GetItemInfo( CLayoutUI* pLayout, wstring id );

	void LoadSpaceAndPower();
private:
	wstring _deviceID;
	void* _pPhone;
};