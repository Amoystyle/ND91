#pragma once
#include "Common/DeviceBaseThread.h"

class InstallAppThread : public DeviceBaseThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    InstallAppThread( string threadID, int requestID, wstring deviceID );
	~InstallAppThread();

private:
	virtual unsigned long Main();

private:
	int _nRequestID;
};