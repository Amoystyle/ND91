#pragma once
#include "Base/BaseThread.h"

class CheckConnectThread:public BaseThread
{
public:
	CheckConnectThread(const string id);
	~CheckConnectThread();

	unsigned long Main();

	
};