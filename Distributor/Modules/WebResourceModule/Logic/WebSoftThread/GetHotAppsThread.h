#pragma once
#include "Base/BaseThread.h"
#include "Logic/WebSoftInfo.h"

class GetHotAppsThread:public BaseThread
{
public:
	GetHotAppsThread(const string id);
	~GetHotAppsThread();

	unsigned long Main();

	PlatFrom			_platType;
	vector<WebSoftData*>		*_VerDatas;	// ���������б�
	string				_url;				
};