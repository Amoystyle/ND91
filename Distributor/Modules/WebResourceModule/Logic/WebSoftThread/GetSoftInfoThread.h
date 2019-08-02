#pragma once
#include "Base/BaseThread.h"
#include "Logic/WebSoftInfo.h"

class GetSoftInfoThread:public BaseThread
{
public:
	GetSoftInfoThread(const string id);
	~GetSoftInfoThread();

	unsigned long Main();

	PlatFrom			_platType;
	string				_url;				
};