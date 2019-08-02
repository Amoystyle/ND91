#include "StdAfx.h"
#include "ChkCpaDispatcher.h"
#include "ChkCpaDispatcherThread.h"
#include "Manager/ThreadManager.h"

CChkCpaDispatcher::CChkCpaDispatcher(void)
{
	_curTask = NULL;
	_backgroundTask = NULL;
	_signal = CreateEvent(NULL,false,true,NULL);
	string threadId = "CChkCpaDispatcher_Run_" +  CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	_pThread = new CChkCpaDispatcherThread(threadId);
	_pThread->setChkCpaDispatcher(this);
	_pThread->Run();
}

CChkCpaDispatcher::~CChkCpaDispatcher(void)
{
	if (NULL != _curTask)
	{
		delete _curTask;
		_curTask = NULL;
	}

	if (NULL != _backgroundTask)
	{
		delete _backgroundTask;
		_backgroundTask = NULL;
	}
}

void CChkCpaDispatcher::AddTask( wstring strHashValue,ENUM_PLATFORM plat,wstring guid)
{
	_backgroundTask = new CChkCpaTask();
	_backgroundTask->setPlatfrom(plat);
	_backgroundTask->setId(guid);
	_backgroundTask->setStrHashValue(strHashValue);
	_backgroundTask->setStatus(TaskStatusWaiting);
	if (_curTask != NULL)
	{
		if (TaskStatusRunning == _curTask->getStatus())
		{
			_curTask->setCancel(true);
		}
		else
		{			
			Swap();
			SetEvent(_signal);
		}
	}
	else
	{
		Swap();
		SetEvent(_signal);
	}
}


void CChkCpaDispatcher::Swap()
{
	AutoLock;
	_curTask = _backgroundTask;
	_backgroundTask = NULL;
	
}

void CChkCpaDispatcher::Run()
{
	while (true)
	{
		WaitForSingleObject(this->_signal,INFINITE);

		if (NULL != _curTask)
		{
			CChkCpaTask* pTmp = _curTask;
			if (pTmp->getStatus() ==  TaskStatusWaiting)
			{
				if( pTmp->Run() )
				{
					pTmp->setStatus(TaskStatusComplete);
				}
				else
				{
					pTmp->setStatus(TaskStatusFail);
				}
			}
			if (NULL != pTmp)
			{
				delete pTmp;
				pTmp = NULL;
			}
			SetEvent(_signal);
		}
	}
}

bool CChkCpaDispatcher::Handle( const CpaTaskStatueMsg* pMsg )
{
	if (NULL == pMsg)
	{
		return true;
	}
	if (pMsg->task == _curTask)
	{
		if (pMsg->newStatus != TaskStatusRunning)
		{
			Swap();
		}
	}
	return true;
}
