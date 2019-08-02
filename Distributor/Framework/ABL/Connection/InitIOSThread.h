#pragma once
#include "Common/DeviceBaseThread.h"

class InitIOSThread : public BaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    InitIOSThread( string threadID );
	~InitIOSThread();

private:
	virtual unsigned long Main();
};