#include "stdafx.h"
#include "LoadDevicePowerInfoThread.h"

LoadDevicePowerInfoThread::LoadDevicePowerInfoThread( string threadID, int nRequestID, wstring deviceID )
							: DeviceBaseThread(threadID, deviceID)
{
	_autoDelete = true;  
	_nRequestID = nRequestID;
	_pInfo = NULL;
}

LoadDevicePowerInfoThread::~LoadDevicePowerInfoThread()
{
}

void LoadDevicePowerInfoThread::SetDeviceInfo( AblDeviceInfo* pInfo )
{
	_pInfo = pInfo;
}

unsigned long LoadDevicePowerInfoThread::Main()
{
	this->GetPowerInfo();

	return 0;
}

bool LoadDevicePowerInfoThread::GetPowerInfo()
{
	try
	{
		DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_deviceID);
		if (!pDevice) return false;

		RETURN_VALUE_IF(CheckExit(), 0);

		int nBatteryLever = pDevice->GetBatteryLever();

		RETURN_VALUE_IF(CheckExit(), 0);

		if (nBatteryLever > 0)
		{
			ResponseDevicePowerInfoMsg msg;
			msg.deviceID = _deviceID;
			msg.nOsType = pDevice->GetDevInfo()->_deviceType == Ios? ePhoneOS_Ios:ePhoneOS_Android;

			msg.nPower = nBatteryLever;

			RETURN_VALUE_IF(CheckExit(), 0);

			msg.PostToUI();

			RETURN_VALUE_IF(CheckExit(), 0);

			if (_pInfo)
				_pInfo->nBattery = nBatteryLever;
		}
	}
	catch (...)
	{
		return false;
	}    

	return true;
}