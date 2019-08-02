#pragma once
#include "Base/BaseThread.h"
#include "Logic/WebSoftInfo.h"

class GetSoftCategorysThread:public BaseThread
{
public:
	GetSoftCategorysThread(const string id);
	~GetSoftCategorysThread();

	unsigned long Main();

	PlatFrom			_platType;
	vector<WebSoftCategory*>	*_VerCats;	// ���������б�
	string				_url;				
};