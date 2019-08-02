#pragma once
#include "Common/DeviceBaseThread.h"

class RebootDeviceThread : public DeviceBaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    RebootDeviceThread( string threadID, wstring deviceID );
	~RebootDeviceThread();

private:
	virtual unsigned long Main();
};