#pragma once
#include "Common/DeviceBaseThread.h"

class FixFlashExitThread : public DeviceBaseThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    FixFlashExitThread( string threadID, wstring deviceID );
	~FixFlashExitThread();

private:
	virtual unsigned long Main();
};