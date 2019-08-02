#pragma once

#include "AdbShellCommand.h"
#include "Common/CodeOperation.h"

namespace ND91Assistant
{
	// ��ȡ��ص������������п��ܻ�ȡ����ֵ��ȡ������ʱ�򣬸���AdbGetBattery2Command
	class AdbGetBatteryLeverCommand : public AdbShellCommand
	{
	public:
		AdbGetBatteryLeverCommand(const wstring sn) 
		{
			_nBatteryLever = 0;
			_sn = CCodeOperation::UnicodeToUTF_8(sn);
			_strCommand = "cat /sys/devices/platform/rs30100001:00000000/power_supply/battery/capacity";
		};

	protected:
		virtual bool ParseReturnData()
		{
			_nBatteryLever = atoi(_returnData.c_str());
			_success = (_nBatteryLever > 0);
			return _success;
		};
	public:
		int	_nBatteryLever;
	};
}