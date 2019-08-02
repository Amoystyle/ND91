#pragma once
#include "Common/DeviceBaseThread.h"

class WaitWindowsThread : public DeviceBaseThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    WaitWindowsThread( string threadID, wstring deviceID, void* pPhone );
	~WaitWindowsThread();

private:
	virtual unsigned long Main();

private:
	int _nRequestID;
	void* _pPhone;
};