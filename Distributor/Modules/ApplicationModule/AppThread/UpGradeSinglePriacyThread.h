#pragma once
#include "Logic/AppModel.h"
class UpGradeSinglePriacyThread : public BaseThread
{
public:
	UpGradeSinglePriacyThread(AppModel* pAppModel=NULL);
	~UpGradeSinglePriacyThread(void);

	void SetVoid(void* p) { _pParam = p;};

	void SetbLegal(bool bLegal){ _bLegal=bLegal;}
	unsigned long Main();
private:
	bool  _bLegal;
	void* _pParam;
	AppModel* _pAppModel;
};
