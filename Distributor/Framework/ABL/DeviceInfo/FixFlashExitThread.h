#pragma once
#include "Common/DeviceBaseThread.h"

class FixFlashExitThread : public DeviceBaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    FixFlashExitThread( string threadID, wstring deviceID );
	~FixFlashExitThread();

private:
	virtual unsigned long Main();
};