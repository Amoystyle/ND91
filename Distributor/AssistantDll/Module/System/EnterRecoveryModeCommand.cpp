#include "stdafx.h"

#ifdef MODULE_SYSTEM
#include "SetDeviceNameCommand.h"
#include "Core/DeviceData.h"
#include "Device/IosUsbConnection.h"
#include "Module/AndroidDefine.h"
#include "Module/System/SystemManager.h"
#include "EnterRecoveryModeCommand.h"

EnterRecoveryModeCommand::EnterRecoveryModeCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
}

// 执行命令
void EnterRecoveryModeCommand::Execute()
{
	switch( _pDevice->GetDevInfo()->_deviceType )
	{
	case Android:
		break;

	case Ios:
		{
			if (_pDevice->GetDevInfo()->_connectType == USB)
			{
				IosUsbConnection iosUsb((DeviceInfo*)_pDevice->GetDevInfo());
				_success = iosUsb.EnterRecoveryMode()==0?true:false;
			}
			else if (_pDevice->GetDevInfo()->_connectType == WIFI)
			{
				//wifi模式尚未处理
			}
		}
		break;

	default:
		THROW_ASSISTANT_EXCEPTION(UNKNOWN_DEVICE_OR_CONNECTION);
		break;
	}
}

#endif