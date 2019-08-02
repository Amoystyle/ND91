#pragma once
#include "Base/BaseThread.h"
class CCpaThread:
	public BaseThread
{
public:
	CCpaThread(const string threadId);
	~CCpaThread(void);

	virtual unsigned long Main();

};
