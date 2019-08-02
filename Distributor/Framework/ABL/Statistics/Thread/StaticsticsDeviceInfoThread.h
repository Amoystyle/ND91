#pragma once
#include "Base/BaseThread.h"
class CStatistics;
class StaticsticsDeviceInfoThread 
	: public BaseThread
{
public:
	StaticsticsDeviceInfoThread(const string threadId,CStatistics* pHelper);
	~StaticsticsDeviceInfoThread(void);
private:
	virtual unsigned long Main();
	CStatistics* _pHelper;
};
