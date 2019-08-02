#include "stdafx.h"
#include "RecoveryDeviceThread.h"
#include "Module/System/SystemManager.h"

RecoveryDeviceThread::RecoveryDeviceThread( string threadID, wstring deviceID )
						: DeviceBaseThread(threadID, deviceID)
{
    _autoDelete = true;
}

RecoveryDeviceThread::~RecoveryDeviceThread()
{
}

unsigned long RecoveryDeviceThread::Main()
{
	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_deviceID);
	if (pDevice && pDevice->GetDevInfo()->_deviceType == Ios)
	{
		pDevice->GetSystemManager()->EnterRecoveryMode();
	}
	return 0;
}