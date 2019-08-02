#pragma once
/*! @class
********************************************************************************
<PRE>
类名称   : DeviceInfoManager
功能     : 设备管理，整合AssistantDll和DDM的处理
异常类   :  
--------------------------------------------------------------------------------
备注     : 
典型用法 : 
</PRE>
*******************************************************************************/
class DeviceInfoManager
	: public BaseCSLock
	, public BaseHandler<DeviceConnectMsg>
	, public BaseGetData<RequestDeviceTypeMsg>
	, public BaseHandler<RequestDeviceInfoMsg>
	, public BaseHandler<RequestDeviceSpaceMsg>
	, public BaseHandler<RequestDevicePowerInfoMsg>
	, public BaseHandler<RequestDeviceImageMsg>
	, public BaseHandler<RequestRebootDeviceMsg>
	, public BaseHandler<RequestRecoveryDeviceMsg>
	, public BaseHandler<RequestExitRecoveryDeviceMsg>
	, public BaseHandler<RequestClearDeviceMemoryMsg>
	, public BaseHandler<RequestRepairDeviceMsg>
	, public BaseHandler<RequestChangeInstallPositionMsg>
	, public BaseGetData<RequestDeviceStateMsg>
	, public BaseGetData<GetAblDeviceInfoMsg>
	, public BaseGetData<RequestInstallPositionMsg>
{
public:
    DeviceInfoManager();
    ~DeviceInfoManager();

protected:
	bool Handle( const DeviceConnectMsg* pMsg);

    bool Handle( RequestDeviceTypeMsg* pMsg );

	bool Handle( const RequestDeviceInfoMsg* pMsg );

	bool Handle( const RequestDeviceSpaceMsg* pMsg );

	bool Handle( const RequestDevicePowerInfoMsg* pMsg );

	bool Handle( const RequestDeviceImageMsg* pMsg );

	bool Handle( const RequestRebootDeviceMsg* pMsg );

	bool Handle( const RequestRecoveryDeviceMsg* pMsg );

	bool Handle( const RequestExitRecoveryDeviceMsg* pMsg );

	bool Handle( const RequestClearDeviceMemoryMsg* pMsg );

	bool Handle( const RequestRepairDeviceMsg* pMsg );

	bool Handle( const RequestChangeInstallPositionMsg* pMsg );

	bool Handle( RequestDeviceStateMsg* pMsg );

	bool Handle( GetAblDeviceInfoMsg* pMsg );

	bool Handle( RequestInstallPositionMsg* pMsg );

    AblDeviceInfo* GetDeviceInfo(wstring deviceID);
private:
    map<wstring, AblDeviceInfo*> _infoMap;
};