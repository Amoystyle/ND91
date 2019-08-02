#pragma once
#include "Logic/AppModel.h"

class InstallAppDataThread : public BaseThread
{
public:
	InstallAppDataThread(AppModel* pAppModel=NULL);
	~InstallAppDataThread(void);

	unsigned long Main();

	void AddStrParam( wstring par );
	void SetStr(wstring wsTemp);
private:
	vector<wstring> _strList;
	wstring _wsSn;
	AppModel* _pAppModel;
};
