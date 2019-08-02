#include "StdAfx.h"
#include "AppLoadIconThread.h"

AppLoadIconThread::AppLoadIconThread(AppModel* pAppModel)
{
	pVoidParam = NULL;
	_pAppModel = pAppModel;
}

AppLoadIconThread::~AppLoadIconThread(void)
{
}

unsigned long AppLoadIconThread::Main()
{
	if (NULL == _pAppModel)
	{
		return 0;
	}

	try
	{
			_pAppModel->LoadAppIcon((vector<AppData*>*)pVoidParam);
	}
	catch (AssistantException e)
	{
		LOG->WriteDebug(_T("�߳�AppLoadIconThread����"));
	}
	catch (...)
	{
		LOG->WriteDebug(_T("�߳�AppLoadIconThread����"));
	}
	return 0;
}