#pragma once
#include "Common/DeviceBaseThread.h"

class LoadDeviceSpaceThread : public DeviceBaseThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
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