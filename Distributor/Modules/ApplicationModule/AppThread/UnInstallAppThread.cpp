#include "StdAfx.h"
#include "UnInstallAppThread.h"

UnInstallAppThread::UnInstallAppThread(AppModel* pAppModel)
{
	_voidList.clear();
	_pAppModel = pAppModel;
}

UnInstallAppThread::~UnInstallAppThread(void)
{
}

void UnInstallAppThread::AddVoidParam( void* par )
{
	_voidList.push_back(par);
}

const vector<void*>* UnInstallAppThread::GetVoidParam() const
{
	return &_voidList;
}

unsigned long UnInstallAppThread::Main()
{
	if (NULL == _pAppModel)
	{
		return 0;
	}
	try
	{
		_pAppModel->UnInstallApp(&_voidList);

	}
	catch (AssistantException e)
	{
		LOG->WriteDebug(_T("线程UnInstallAppThread出错"));
	}
	catch (...)
	{
		LOG->WriteDebug(_T("线程UnInstallAppThread出错"));
	}

	return 0;
}
