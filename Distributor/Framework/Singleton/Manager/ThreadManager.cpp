#include "stdafx.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager()
{   
    _nGUID = 1;
}

ThreadManager::~ThreadManager()
{
    AutoLock;
	_threads.clear();
}

void ThreadManager::SetThreadID( int nID )
{
    _nThreadID = nID;
}

int ThreadManager::GetThreadID() const
{
    return _nThreadID;
}

void ThreadManager::InsertThread(BaseThread* pThread, string id)
{
    AutoLock;
    _threads[id] = pThread;
}

void ThreadManager::RemoveThread( BaseThread* pThread )
{
    AutoLock;
    for(THREAD_MAP::iterator it = _threads.begin(); it != _threads.end(); ++it)
    {
        if (it->second == pThread)
        {
            _threads.erase(it);
            return;
        }
    }
}

BaseThread* ThreadManager::GetThread( string id )
{
    return _threads[id];
}

BaseThread* ThreadManager::GetThread( int nGUID )
{
    AutoLock;
    for(THREAD_MAP::iterator it = _threads.begin(); it != _threads.end(); ++it)
    {
		if ( it->second )
		{
			if (it->second->_nGUID == nGUID)
			{
				return it->second;
			}
		}
    }
    return NULL;
}

int ThreadManager::GetGUID()
{
    return _nGUID ++;
}

void ThreadManager::ExitThread( string id )
{
    AutoLock;
	if (!id.empty() && _threads.find(id) != _threads.end())
	{
		_threads[id]->Exit();
	}
}

void ThreadManager::ExitThread( int nGUID )
{
    AutoLock;
    for(THREAD_MAP::iterator it = _threads.begin(); it != _threads.end(); ++it)
    {
		if ( it->second )
		{
			if (it->second->_nGUID == nGUID)
			{
				it->second->Exit();
				return;
			}
		}
    }    
}

void ThreadManager::ExitThreads( wstring deviceID )
{
    AutoLock;
    for(THREAD_MAP::iterator it = _threads.begin(); it != _threads.end(); ++it)
    {
		if ( it->second )
		{
			if (it->second->_deviceID == deviceID)
			{
				it->second->Exit();
			}
		}
    }
}


