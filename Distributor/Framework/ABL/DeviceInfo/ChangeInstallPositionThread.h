#pragma once
#include "Base/BaseThread.h"

class ChangeInstallPositionThread : public BaseThread
{
public:
    // ���캯�� threadID�߳�Ψһ��ʶ
    ChangeInstallPositionThread( string threadID, bool bInstallOnPhone);
	~ChangeInstallPositionThread();

private:
	virtual unsigned long Main();

	bool _bInstallOnPhone;
};