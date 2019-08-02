#pragma once
#include "Common/DeviceBaseThread.h"

class LoadDeviceImageThread : public DeviceBaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    LoadDeviceImageThread( string threadID, wstring deviceID, bool bRefresh );
	~LoadDeviceImageThread();

private:
	virtual unsigned long Main();
		
	bool _bRefresh;
};