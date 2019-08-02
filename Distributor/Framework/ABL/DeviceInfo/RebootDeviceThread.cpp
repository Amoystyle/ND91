#include "stdafx.h"
#include "RebootDeviceThread.h"
#include "Module/System/SystemManager.h"

RebootDeviceThread::RebootDeviceThread( string threadID, wstring deviceID )
						: DeviceBaseThread(threadID, deviceID)
{
    _autoDelete = true;
}

RebootDeviceThread::~RebootDeviceThread()
{
}

unsigned long RebootDeviceThread::Main()
{
	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_deviceID);
	if (pDevice)
	{
		pDevice->GetSystemManager()->RebootDev();
	}
	return 0;
}