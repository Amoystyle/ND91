#pragma once
#include "Logic/AppModel.h"

class UnInstallAppThread : public BaseThread
{
public:
	UnInstallAppThread(AppModel* pAppModel=NULL);
	~UnInstallAppThread(void);

	unsigned long Main();

	void AddVoidParam( void* par );

	const vector<void*>* GetVoidParam() const;

private:
	 vector<void*>   _voidList;
	 AppModel* _pAppModel;
};
