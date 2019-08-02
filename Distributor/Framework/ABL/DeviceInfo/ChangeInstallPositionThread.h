#pragma once
#include "Base/BaseThread.h"

class ChangeInstallPositionThread : public BaseThread
{
public:
    // 构造函数 threadID线程唯一标识
    ChangeInstallPositionThread( string threadID, bool bInstallOnPhone);
	~ChangeInstallPositionThread();

private:
	virtual unsigned long Main();

	bool _bInstallOnPhone;
};