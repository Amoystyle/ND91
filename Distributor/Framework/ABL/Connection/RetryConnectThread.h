#pragma once
#include "Common/DeviceBaseThread.h"

class RetryConnectThread : public BaseThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    RetryConnectThread( string threadID, void* pPhone );
	~RetryConnectThread();

private:
	virtual unsigned long Main();

private:
	void* _pPhone;
};