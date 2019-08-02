#pragma once
#include "Common/DeviceBaseThread.h"

class WaitWindowsThread : public DeviceBaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    WaitWindowsThread( string threadID, wstring deviceID, void* pPhone );
	~WaitWindowsThread();

private:
	virtual unsigned long Main();

private:
	int _nRequestID;
	void* _pPhone;
};