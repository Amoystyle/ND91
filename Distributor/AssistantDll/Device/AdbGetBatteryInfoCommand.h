#pragma once

#include "AdbShellCommand.h"
#include "Common/CodeOperation.h"
#include "BatteryInfo.h"

namespace ND91Assistant
{
	// ��ȡ��ص������������п��ܻ�ȡ����ֵ��ȡ������ʱ�򣬸���AdbGetBatteryInfoCommand
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