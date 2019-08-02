#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "GetBatteryLeverCommand.h"
#include "Core/DeviceData.h"
#include "Device/AdbGetBatteryLeverCommand.h"
#include "Device/AdbGetBatteryInfoCommand.h"
#include "Device/IosUsbConnection.h"
#include "Module/AndroidDefine.h"

GetBatteryLeverCommand::GetBatteryLeverCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
	_nBatteryLever = 0;
	_nAndroidChannelID = SYSINFO;
	_nAndroidCommandID = GET_BATTERY;
}

GetBatteryLeverCommand::~GetBatteryLeverCommand(void)
{
}

std::string ND91Assistant::GetBatteryLeverCommand::GetAndroidBodyData()
{
	BinaryBufferWriter writer;
	writer.WriteInt(_nAndroidChannelID);
	writer.WriteInt(_nAndroidCommandID);
	return writer.GetBuffer();
}

void ND91Assistant::GetBatteryLeverCommand::SetAndroidReturnData(std::string data)
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead_Top16(&reader))
		return;
	reader.ReadInt32(_nBatteryLever);
	_success = true;
}

void ND91Assistant::GetBatteryLeverCommand::Execute()
{
	if (!_pDevice || !_pDevice->GetDevInfo())
		return ;

	if (_pDevice->GetDevInfo()->_deviceType == Android)
	{
		if (_pDevice->GetDevInfo()->_connectType == WIFI)
			DefaultExecute();
		else if (_pDevice->GetDevInfo()->_connectType == USB)
		{
			AdbGetBatteryLeverCommand cmd1(_pDevice->GetDevInfo()->_strSerialNumber);
			cmd1.Execute();
			_nBatteryLever = cmd1._nBatteryLever;
			if (_nBatteryLever == 0)
			{
				AdbGetBatteryInfoCommand cmd2(_pDevice->GetDevInfo()->_strSerialNumber);
				cmd2.Execute();
				_nBatteryLever = cmd2._pBatteryInfo._nLever;
			}
		}
	}
	else if (_pDevice->GetDevInfo()->_deviceType == Ios)
	{
		if (_pDevice->GetDevInfo()->_connectType == USB)
		{
			IosUsbConnection usb((DeviceInfo*)_pDevice->GetDevInfo());
			_nBatteryLever = usb.GetBetteryState();
		}
	}
}


#endif