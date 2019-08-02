#include "stdafx.h"
#include "EventManager.h"

EventManager::EventManager()
{
    _systemExitEvent = CreateEvent(NULL, true, false, NULL);
    _systemExit = false;
}

EventManager::~EventManager()
{
    if ( _systemExitEvent )
        CloseHandle(_systemExitEvent);
}

HANDLE EventManager::SystemExitEvent()
{
    HANDLE returnEvent = NULL;

    if ( ! DuplicateHandle(GetCurrentProcess(), 
        _systemExitEvent, 
        GetCurrentProcess(),
        &returnEvent, 
        0,
        FALSE,
        DUPLICATE_SAME_ACCESS))
    {
        LOG->Write(Log::LOG_ALERT, L"EventManager::DupHandle() fail: %d", GetLastError());
    }

    return returnEvent;
}

void EventManager::ExitSystem()
{
    _systemExit = true;
    SetEvent(_systemExitEvent);

	AutoLock;

	for (vector<IExit*>::iterator iter=_vecExitObserver.begin(); iter != _vecExitObserver.end(); ++iter)
	{
		if (*iter)
		{
			(*iter)->Exit();
		}
	}

	//¹Ø±ÕÊý¾Ý¿â
	for (vector<IExitDB*>::iterator iter=_vecExitDB.begin(); iter != _vecExitDB.end(); ++iter)
	{
		if (*iter)
		{
			(*iter)->ExitDB();
		}
	}

	return;
}

const bool* EventManager::SystemExitFlag()
{
    return &_systemExit;
}

void EventManager::AttachDB(IExitDB* pExit)
{
	if (NULL == pExit)
	{
		return;
	}

	AutoLock;

	for (vector<IExitDB*>::iterator iter=_vecExitDB.begin(); iter != _vecExitDB.end(); ++iter)
	{
		if (*iter && pExit==*iter)
		{
			return;
		}
	}
	_vecExitDB.push_back(pExit);

	return;
}

void EventManager::Attach(IExit* pExit)
{
	if (NULL == pExit)
	{
		return;
	}

	AutoLock;

	for (vector<IExit*>::iterator iter=_vecExitObserver.begin(); iter != _vecExitObserver.end(); ++iter)
	{
		if (*iter && pExit==*iter)
		{
			return;
		}
	}

	_vecExitObserver.push_back(pExit);

	return;
}

void EventManager::DeAttach(IExit* pExit)
{
	if (NULL == pExit)
	{
		return;
	}

	AutoLock;

	for (vector<IExit*>::iterator iter=_vecExitObserver.begin(); iter != _vecExitObserver.end(); ++iter)
	{
		if (*iter && pExit==*iter)
		{
			_vecExitObserver.erase(iter);
			break;
		}
	}

	return;
}

void EventManager::DeAttachDB(IExitDB* pExit)
{
	if (NULL == pExit)
	{
		return;
	}

	AutoLock;

	for (vector<IExitDB*>::iterator iter=_vecExitDB.begin(); iter != _vecExitDB.end(); ++iter)
	{
		if (*iter && pExit==*iter)
		{
			_vecExitDB.erase(iter);
			break;
		}
	}

	return;
}