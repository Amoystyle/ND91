#pragma once
#include "Common/DeviceBaseThread.h"

class InitIOSThread : public BaseThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    InitIOSThread( string threadID );
	~InitIOSThread();

private:
	virtual unsigned long Main();
};