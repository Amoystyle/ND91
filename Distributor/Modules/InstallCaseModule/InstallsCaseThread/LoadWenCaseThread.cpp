#include "StdAfx.h"
#include "LoadWenCaseThread.h"

LoadWenCaseThread::LoadWenCaseThread(void)
{
	_nUserId = 0;
	_bCpa = false;

	pInstallCaseModelEx = NULL;
}

LoadWenCaseThread::~LoadWenCaseThread(void)
{
}

void LoadWenCaseThread::SetLoadCaseInfo(InstallCaseModelEx* pModel, bool bLoadDataBase, int nUserId, bool bCpa)
{
	_bLoadDataBase = bLoadDataBase;
	_nUserId = nUserId;
	_bCpa = bCpa;

	pInstallCaseModelEx = pModel;
}

unsigned long LoadWenCaseThread::Main()
{
	if (NULL == pInstallCaseModelEx)
	{
		return 0;
	}

	pInstallCaseModelEx->LoadWebCaseInternal(_bLoadDataBase, _nUserId, _bCpa);

	return 0;
}