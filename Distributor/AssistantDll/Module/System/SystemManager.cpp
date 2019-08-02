#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "Module/System/SystemManager.h"
#include "GetIMEICommand.h"
#include "GetIMSICommand.h"
#include "GetBatteryLeverCommand.h"
#include "GetIosDeviceTypeCommand.h"
#include "SetDeviceNameCommand.h"
#include "LogOffCommand.h"
#include "RebootCommand.h"
#include "ShoutdownCommand.h"
#include "GetIsRootCommand.h"
#include "GetIsWriteCommand.h"
#include "GetIsRemountCommand.h"
#include "GetResolutionCommand.h"
#include "GetDeviceNameCommand.h"
#include "EnterRecoveryModeCommand.h"

#include "Module/ScreenShot/ScreenImageData.h"
#include "Module/ScreenShot/GetScreenImageInfoCommand.h"

#include "Module/File/DirectoryOperationCommand.h"
#include "Module/File/GetSDCardStateCommand.h"

#include "Module/Common/IosDB/IosDBPath.h"
#include "Module/Message/IosMessageDB.h"
#include "Device/IosRecoveryConnection.h"

SystemManager::SystemManager( const DeviceData* pDevice )
: IManager (pDevice)
{

}

SystemManager::~SystemManager()
{

}

wstring ND91Assistant::SystemManager::GetIMEI()
{
	LOG->WriteDebug(L"begin GetIMEI() ");
    GetIMEICommand imeiCmd(_pDevice);
    imeiCmd.Execute();
	LOG->WriteDebug(L"GetIMEI() OK!");
    return imeiCmd.GetIMEI();
}

wstring ND91Assistant::SystemManager::GetIMSI()
{
    GetIMSICommand imsiCmd(_pDevice);
    imsiCmd.Execute();
    return imsiCmd.GetIMSI();
}

int ND91Assistant::SystemManager::GetBatteryLever()
{
	LOG->WriteDebug(L"begin GetBatteryLever() ");
    GetBatteryLeverCommand cmd(_pDevice);
    cmd.Execute(); 
	LOG->WriteDebug(L"GetBatteryLever() OK!");
    return cmd.GetBatteryLever();
}

void ND91Assistant::SystemManager::RepairIdleDevice()
{
    
}

wstring ND91Assistant::SystemManager::GetIosDeviceType()
{
	LOG->WriteDebug(L"begin GetIosDeviceType() ");
    GetIosDeviceTypeCommand devTypeCmd(_pDevice);
    devTypeCmd.Execute();
	LOG->WriteDebug(L"GetIosDeviceType() OK!");
    return devTypeCmd.GetDeviceType();
}

SDCardState ND91Assistant::SystemManager::GetSDCardState()
{
	LOG->WriteDebug(L"begin GetSDCardState() ");
    GetSDCardStateCommand cmd(_pDevice);
    cmd.Execute();
	LOG->WriteDebug(L"GetSDCardState() OK!");
    return cmd.GetSDCardState();
}

wstring ND91Assistant::SystemManager::GetDeviceName()
{
	LOG->WriteDebug(L"begin GetDeviceName() ");
    wstring deviceName;
    GetDeviceNameCommand rCmd(_pDevice);
    rCmd.Execute();
    deviceName = rCmd.GetDeviceName();
	LOG->WriteDebug(L"GetDeviceName() OK!");


	if(_pDevice != NULL)
	{
		if(_pDevice->GetDevInfo()->_deviceType == Android)
		{
			DeviceData* pDevice = (DeviceData*)_pDevice;
			pDevice->GetMutableDevInfo()->_deviceName = deviceName;
		}
		if(deviceName.empty())
			deviceName = _pDevice->GetDevInfo()->GetDeviceName();

	}
    return deviceName;
}

bool ND91Assistant::SystemManager::SetDevName(wstring strNewName)
{
    SetDeviceNameCommand cmd(_pDevice, strNewName);
    cmd.Execute();
    return cmd.Success();
}

#ifndef ASSISTANT_LITE
bool ND91Assistant::SystemManager::EnterRecoveryMode()
{
	EnterRecoveryModeCommand cmd(_pDevice);
	cmd.Execute();
	return cmd.Success();
}
#endif

bool ND91Assistant::SystemManager::RebootToNormal()
{
	const DeviceInfo* devinfo = _pDevice->GetDevInfo();
	IosRecoveryConnection recoveryConn((DeviceInfo*)devinfo);
	recoveryConn.RebootToNormal();
	return true;
}

bool ND91Assistant::SystemManager::LogoffDev()
{
    LogOffCommand loCmd(_pDevice);
    loCmd.Execute();
    return loCmd.Success();
}

bool ND91Assistant::SystemManager::RebootDev()
{
    RebootCommand cmd(_pDevice);
    cmd.Execute();
    return cmd.Success();
}

bool ND91Assistant::SystemManager::ShoutdownDev()
{
    ShoutdownCommand cmd(_pDevice);
    cmd.Execute();
    return cmd.Success();
}

bool ND91Assistant::SystemManager::GetIsRoot()
{
	LOG->WriteDebug(L"begin GetIsRoot() ");
	RETURN_FALSE_IF ( _pDevice->GetDevInfo()->_deviceType != Android )

	GetIsRootCommand cmd(_pDevice);
	cmd.Execute();
	bool bRoot = cmd.GetResult();

	((DeviceInfo*)_pDevice->GetDevInfo())->_bRoot = bRoot;	
	LOG->WriteDebug(L"GetIsRoot() ok!");
	return bRoot;
}

bool ND91Assistant::SystemManager::GetIsRemount()
{
	LOG->WriteDebug(L"begin GetIsRemount() ");
	RETURN_FALSE_IF ( _pDevice->GetDevInfo()->_deviceType != Android );

    bool res = false;
	if ( _pDevice->GetDevInfo()->_connectType == USB )
	{
		DirectoryOperationCommand dircmd(_pDevice);
		GetIsWriteCommand wCmd(_pDevice);
		wCmd.Execute();
				
		if ( dircmd.IsMountSystem() && wCmd.GetResult() )
            res = true;
		else
		{
			GetIsRemountCommand rCmd(_pDevice);
			rCmd.Execute();
			res = rCmd.GetResult();
		}
	}
	else
	{
		GetIsRemountCommand cmd(_pDevice);
		cmd.Execute();
		res = cmd.GetResult();
	}

    LOG->WriteDebug(L"GetIsRemount() ok!");
    return res;
}

wstring ND91Assistant::SystemManager::GetDeviceResolution()
{
	LOG->WriteDebug(L"begin GetDeviceResolution() ");
	wstring strResolution = L"";
	RETURN_VALUE_IF(!_pDevice, L"");

	if ( _pDevice->GetDevInfo()->_deviceType == Ios )
	{
		WCHAR szSize[16] = {0};
		swprintf(szSize, L"%d*%d", _pDevice->GetDevInfo()->GetScreenWidth(), _pDevice->GetDevInfo()->GetScreenHeight() );
		strResolution = szSize;
	}
	else if ( _pDevice->GetDevInfo()->_deviceType == Android )
	{
		if ( _pDevice->GetDevInfo()->_connectType == USB )
		{
			ScreenImageData ScreenData;

			GetScreenImageInfoCommand cmd(_pDevice);
			cmd.SetParam(&ScreenData);
			cmd.Execute();

			WCHAR szSize[16] = {0};
			swprintf(szSize, L"%d*%d", ScreenData.GetWidth(), ScreenData.GetHeight() );
			strResolution = szSize;
		}
		else
		{
			GetResolutionCommand get(_pDevice);
			get.Execute();
			strResolution = get.GetResult();
		}
	}
	else
	{
		LOG->WriteDebug(L"GetDeviceResolution() ok!");
		return L"";
	}

	LOG->WriteDebug(L"GetDeviceResolution() ok!");
	return strResolution;
}

#ifndef ASSISTANT_LITE
bool ND91Assistant::SystemManager::FixMessageDB()
{
    // 没连接上设备则返回
    if (_pDevice->GetDevInfo() == NULL || !_pDevice->GetDevInfo()->_ios_bAFCConn)
        return false;

    // 修改数据库文件
    IosDBPath dbPath(_pDevice,ISMESSAGEDB);
    IosMessageDB messageDB(dbPath);
    messageDB.OpenDB();
    messageDB.BeginTransaction();
    try
    {
        messageDB.FixDB();
        messageDB.Commit();
        messageDB.CloseDB();
        return messageDB.SyncToIos(_pDevice);// 同步到IOS设备
    }
    catch (CppSQLite3Exception& e)
    {
        int iError = e.ErrorCode();
        const char* szError = e.ErrorMessage();
        messageDB.Rollback();
    }
    messageDB.CloseDB();

    return false;
}

#endif
#endif
