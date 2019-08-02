#include "stdafx.h"
#include "FixFlashExitThread.h"
#include "Module/Application/AppManager.h"

FixFlashExitThread::FixFlashExitThread( string threadID, wstring deviceID )
						: DeviceBaseThread(threadID, deviceID)
{
    _autoDelete = true;
}

FixFlashExitThread::~FixFlashExitThread()
{
}

unsigned long FixFlashExitThread::Main()
{
	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_deviceID);
	bool ret = false;
	if (pDevice && pDevice->GetDevInfo()->_deviceType == Ios)
	{
		ret = pDevice->GetAppManager()->FixFlashExit() == 0;
	}

	RETURN_VALUE_IF(CheckExit(), 0);

	ResponseRepairDeviceMsg msg;
	msg.bResult = ret;
	msg.PostToUI();

	return 0;
}