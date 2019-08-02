#pragma once
#include "Base/BaseThread.h"
#include "Logic/WebSoftInfo.h"

class GetAppIconThread:public BaseThread
{
public:
	GetAppIconThread(const string id);
	~GetAppIconThread();

	unsigned long Main();

	vector<int>		_fids;
	vector<wstring>	_urls;
	PlatFrom		_platType;
};