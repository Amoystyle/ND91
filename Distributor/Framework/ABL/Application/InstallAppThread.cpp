#include "stdafx.h"
#include "InstallAppThread.h"

InstallAppThread::InstallAppThread( string threadID, int requestID, wstring deviceID )
						: DeviceBaseThread(threadID, deviceID)
{
    _autoDelete = true;
	_nRequestID = requestID;
}

InstallAppThread::~InstallAppThread()
{
}

unsigned long InstallAppThread::Main()
{

	return 0;
}