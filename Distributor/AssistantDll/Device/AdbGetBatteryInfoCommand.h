#pragma once

#include "AdbShellCommand.h"
#include "Common/CodeOperation.h"
#include "BatteryInfo.h"

namespace ND91Assistant
{
	// 读取电池电量，该命令有可能会取不到值，取不到的时候，改用AdbGetBatteryInfoCommand
	class AdbGetBatteryInfoCommand : public AdbShellCommand
	{
	public:
		AdbGetBatteryInfoCommand(const wstring sn) 
		{
			_sn = CCodeOperation::UnicodeToUTF_8(sn);
			_strCommand = "dumpsys battery";
		};

	protected:
		virtual bool ParseReturnData()
		{
			_pBatteryInfo.ParseData(_returnData);
			_success = (_pBatteryInfo._nLever > 0);
			return _success;
		};
	public:
		BatteryInfo _pBatteryInfo;
		
	};
}