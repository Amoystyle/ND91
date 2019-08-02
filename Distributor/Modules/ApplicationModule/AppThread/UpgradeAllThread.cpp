#include "StdAfx.h"
#include "UpgradeAllThread.h"

UpgradeAllThread::UpgradeAllThread(AppModel* pAppModel)
{
	_pAppModel = pAppModel;
}

UpgradeAllThread::~UpgradeAllThread(void)
{
}

unsigned long UpgradeAllThread::Main()
{

	if (NULL==_pAppModel)
	{
		return 0;
	}

	try
	{
		_APP_UPGRADE_TYPE type = (_APP_UPGRADE_TYPE)_nType;
		_pAppModel->UpgradeAllAppDataInternal(type);
	}
	catch (AssistantException e)
	{
		LOG->WriteDebug(_T("�߳�UpgradeAllThread����"));
	}
	catch (...)
	{
		LOG->WriteDebug(_T("�߳�UpgradeAllThread����"));
	}
	return 0;
}