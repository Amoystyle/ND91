#pragma once
#include "Common/DeviceBaseThread.h"

class RebootDeviceThread : public DeviceBaseThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    RebootDeviceThread( string threadID, wstring deviceID );
	~RebootDeviceThread();

private:
	virtual unsigned long Main();
};