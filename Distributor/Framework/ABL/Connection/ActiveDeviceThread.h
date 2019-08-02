#pragma once
#include "Common/DeviceBaseThread.h"

class ActiveDeviceThread : public DeviceBaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    ActiveDeviceThread( string threadID, int requestID, wstring deviceID, void* pPhone );
	~ActiveDeviceThread();

private:
	virtual unsigned long Main();

private:
	int _nRequestID;
	void* _pPhone;
};