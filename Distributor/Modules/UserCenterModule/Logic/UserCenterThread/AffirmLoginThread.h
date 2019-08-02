#pragma once
#include "Base/BaseThread.h"

class AffirmLoginThread:public BaseThread
{
public:
	AffirmLoginThread(const string id);
	~AffirmLoginThread();

	unsigned long Main();

	std::string		_postData;	

	
};