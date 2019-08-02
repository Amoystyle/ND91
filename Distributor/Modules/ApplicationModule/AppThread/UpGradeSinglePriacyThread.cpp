#include "StdAfx.h"
#include "UpGradeSinglePriacyThread.h"
#include "Logic/AppDataUpgrade.h"

UpGradeSinglePriacyThread::UpGradeSinglePriacyThread(AppModel* pAppModel)
{
	_pParam = NULL;
	_bLegal = false;
	_pAppModel = pAppModel;
}

UpGradeSinglePriacyThread::~UpGradeSinglePriacyThread(void)
{
}

unsigned long UpGradeSinglePriacyThread::Main()
{
	if (NULL == _pAppModel || NULL == (AppModuleSpace::AppDataUpgrade*)_pParam)
	{
		return 0;
	}

	try
	{
		_pAppModel->UpgradeSingleAppDataInternal((AppModuleSpace::AppDataUpgrade*)_pParam, _bLegal);
	}
	catch (AssistantException e)
	{
		LOG->WriteDebug(_T("线程UpGradeSinglePriacyThread出错"));
	}
	catch (...)
	{
		LOG->WriteDebug(_T("线程UpGradeSinglePriacyThread出错"));
	}
	if (NULL == _pAppModel)
	{
		return 0;
	}
	return 0;
}
