#include "StdAfx.h"
#include "InstallAppThread.h"

InstallAppDataThread::InstallAppDataThread(AppModel* pAppModel)
{
	_wsSn = L"";
	_pAppModel = pAppModel;
}

InstallAppDataThread::~InstallAppDataThread(void)
{
}

void InstallAppDataThread::AddStrParam( wstring par )
{
	_strList.push_back(par);
}

void InstallAppDataThread::SetStr(wstring wsTemp)
{
	_wsSn = wsTemp;
}
unsigned long InstallAppDataThread::Main()
{
	if (NULL == _pAppModel)
	{
		return 0;
	}

	try
	{
		_pAppModel->InstallAppThread(_strList, _wsSn);

	}
	catch (AssistantException e)
	{
		LOG->WriteDebug(_T("�߳�InstallAppDataThread����"));
	}
	catch (...)
	{
		LOG->WriteDebug(_T("�߳�InstallAppDataThread����"));
	}

	return 0;
}