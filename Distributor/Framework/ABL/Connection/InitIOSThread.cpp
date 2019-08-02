#include "stdafx.h"
#include "InitIOSThread.h"

InitIOSThread::InitIOSThread( string threadID)
						: BaseThread(threadID)
{
}

InitIOSThread::~InitIOSThread()
{
}

unsigned long InitIOSThread::Main()
{
	bool success = false;
	while(!success)
	{
		success = DEVICE_MANAGER->IosInit();

		LOG->WriteDebug(L"InitIOSThread: Result->" + CStrOperation::IntToWString(success?1:0));
		Sleep(2000);
	}
	return 0;
}