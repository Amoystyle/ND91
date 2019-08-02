#include "stdafx.h"
#include "LoadAppThread.h"

LoadAppThread::LoadAppThread(AppModel* pAppModel)
{
	_pAppModel = pAppModel;
}
LoadAppThread::~LoadAppThread(void)
{

}

unsigned long LoadAppThread::Main()
{
	if(NULL == _pAppModel)
	{
		return 0;
	}

	try
	{
		_pAppModel->LoadAppData();
	}
	catch (AssistantException e)
	{
		LOG->WriteDebug(_T("线程LoadAppThread出错"));
	}
	catch (...)
	{
		LOG->WriteDebug(_T("线程LoadAppThread出错"));
	}
	return 0;
}