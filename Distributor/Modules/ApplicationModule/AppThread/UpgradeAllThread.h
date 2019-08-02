#pragma once
#include "Logic/AppModel.h"
class UpgradeAllThread : public BaseThread
{
public:
	UpgradeAllThread(AppModel* pAppModel=NULL);
	~UpgradeAllThread(void);

	void SetUpgradeType(int ntype){ _nType=ntype;}
	unsigned long Main();
private:
	int _nType;
	AppModel* _pAppModel;
};
