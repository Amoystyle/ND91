#pragma once
#include "Common/DeviceBaseThread.h"

class RetryConnectThread : public BaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    RetryConnectThread( string threadID, void* pPhone );
	~RetryConnectThread();

private:
	virtual unsigned long Main();

private:
	void* _pPhone;
};