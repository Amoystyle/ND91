#pragma once
#include "Common/DeviceBaseThread.h"

class LoadDeviceSpaceThread : public DeviceBaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    LoadDeviceSpaceThread( string threadID, int nRequestID, wstring deviceID );
	~LoadDeviceSpaceThread();

	void SetDeviceInfo(AblDeviceInfo* pInfo);

private:
	virtual unsigned long Main();

	bool LoadSpace();
private:
	AblDeviceInfo* _pInfo;
	int _nRequestID;
};