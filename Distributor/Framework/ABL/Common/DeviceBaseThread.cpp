#include "stdafx.h"
#include "DeviceBaseThread.h"

DeviceBaseThread::DeviceBaseThread(string threadID, wstring deviceID)
	: BaseThread(threadID)
{
	_deviceID = deviceID;
	this->_deviceExit = false;
}

DeviceBaseThread::~DeviceBaseThread()
{
}

bool DeviceBaseThread::CheckExit()
{
	return __super::CheckExit() || this->_deviceExit;
}
