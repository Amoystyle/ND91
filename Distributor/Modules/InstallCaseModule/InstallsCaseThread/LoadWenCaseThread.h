#pragma once
#include "Logic/InstallsCase/InstallCaseModelEx.h"

class LoadWenCaseThread : public BaseThread
{
public:
	LoadWenCaseThread(void);
	~LoadWenCaseThread(void);

	unsigned long Main();

	void SetLoadCaseInfo(InstallCaseModelEx* pModel, bool bLoadDataBase, int nUserId = 0, bool bCpa = false);

private:
	bool _bLoadDataBase;
	int  _nUserId;
	bool _bCpa;

	InstallCaseModelEx* pInstallCaseModelEx;
};
