#pragma once
#include "Base/BaseThread.h"
#include "UbdgLib.h"
#include "RevertData.h"
#include "Web.h"

class GetUserInfoThread:public BaseThread
{
public:
	GetUserInfoThread(const string id);
	~GetUserInfoThread();

	unsigned long Main();

	bool _bShowUserInfo;

	wstring _strDeviceID;

	CUSTOMER_DATA* _pData; 
	
};