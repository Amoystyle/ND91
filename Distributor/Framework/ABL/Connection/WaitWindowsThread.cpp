#include "stdafx.h"
#include "WaitWindowsThread.h"

WaitWindowsThread::WaitWindowsThread( string threadID, wstring deviceID, void* pPhone )
						: DeviceBaseThread(threadID, deviceID)
{
	_pPhone = pPhone;
}

WaitWindowsThread::~WaitWindowsThread()
{
}

unsigned long WaitWindowsThread::Main()
{
	Sleep(3000);

	RETURN_VALUE_IF(CheckExit(), 0);

	DeviceConnectWaitWindowsMsg msg;
	msg.pPhone = this->_pPhone;
	msg.SendToUI();

	return 0;
}