#pragma once
#include "Base/BaseThread.h"
#include "UbdgLib.h"
#include "RevertData.h"
#include "Web.h"

class SendUserInfoThread:public BaseThread
{
public:
	SendUserInfoThread(const string id);
	~SendUserInfoThread();

	unsigned long Main();

	CUSTOMER_DATA* _pData; 
	
};