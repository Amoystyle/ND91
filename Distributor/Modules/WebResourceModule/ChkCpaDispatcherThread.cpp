#include "StdAfx.h"
#include "ChkCpaDispatcherThread.h"
#include "ChkCpaDispatcher.h"

CChkCpaDispatcherThread::CChkCpaDispatcherThread(const string threadId)
:BaseThread(threadId)
{
	_chkCpaDispatcher = NULL;
}

CChkCpaDispatcherThread::~CChkCpaDispatcherThread(void)
{
}

unsigned long CChkCpaDispatcherThread::Main()
{
	if (NULL != _chkCpaDispatcher)
	{
		_chkCpaDispatcher->Run();
	}
	return 0;
}

CChkCpaDispatcher* CChkCpaDispatcherThread::getChkCpaDispatcher()
{
	return _chkCpaDispatcher;
}

void CChkCpaDispatcherThread::setChkCpaDispatcher(CChkCpaDispatcher* var)
{
	this->_chkCpaDispatcher = var;
}


