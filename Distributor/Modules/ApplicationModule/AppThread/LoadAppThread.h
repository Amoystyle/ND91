#pragma once
#include "Logic/AppModel.h"

class LoadAppThread: public BaseThread
{
public:
	LoadAppThread(AppModel* pAppModel=NULL);
	~LoadAppThread(void);

	unsigned long Main();

private:
	AppModel* _pAppModel;

};
