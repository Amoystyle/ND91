#pragma once
#include "Common/DeviceBaseThread.h"

class RecoveryDeviceThread : public DeviceBaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    RecoveryDeviceThread( string threadID, wstring deviceID );
	~RecoveryDeviceThread();

private:
	virtual unsigned long Main();
};