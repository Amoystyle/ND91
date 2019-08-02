#pragma once
#include "Common/DeviceBaseThread.h"

class InstallAppThread : public DeviceBaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    InstallAppThread( string threadID, int requestID, wstring deviceID );
	~InstallAppThread();

private:
	virtual unsigned long Main();

private:
	int _nRequestID;
};