#pragma once
#include "Common/DeviceBaseThread.h"

class ActiveDeviceThread : public DeviceBaseThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    ActiveDeviceThread( string threadID, int requestID, wstring deviceID, void* pPhone );
	~ActiveDeviceThread();

private:
	virtual unsigned long Main();

private:
	int _nRequestID;
	void* _pPhone;
};