#pragma once
#include "Base/BaseThread.h"

class CLocalCpaCheckThread:
	public BaseThread
{
public:
	CLocalCpaCheckThread(const string threadId);
	~CLocalCpaCheckThread(void);

	virtual unsigned long Main();
};
