#pragma once
#include "Common/DeviceBaseThread.h"

class RecoveryDeviceThread : public DeviceBaseThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    RecoveryDeviceThread( string threadID, wstring deviceID );
	~RecoveryDeviceThread();

private:
	virtual unsigned long Main();
};