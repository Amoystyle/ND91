#pragma once
#include "Logic/AppModel.h"

class AppLoadIconThread : public BaseThread
{
public:
	AppLoadIconThread(AppModel* pAppModel=NULL);
	~AppLoadIconThread(void);

	void SetVoidParam(void* pVoidParam){this->pVoidParam = pVoidParam;}
	unsigned long Main();

private:
	void* pVoidParam;
	AppModel* _pAppModel;
};
