#pragma once
#include "Common/DeviceBaseThread.h"

class LoadDeviceImageThread : public DeviceBaseThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    LoadDeviceImageThread( string threadID, wstring deviceID, bool bRefresh );
	~LoadDeviceImageThread();

private:
	virtual unsigned long Main();
		
	bool _bRefresh;
};