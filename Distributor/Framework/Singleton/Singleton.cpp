#include "stdafx.h"
#include "Singleton.h"

#include "Message/Message.h"
#include "Manager/ThreadManager.h"
#include "Manager/EventManager.h"

Singleton*      Singleton::_pInstance       = NULL;
HWND            Singleton::_hWnd            = NULL;
MessageCenter*  Singleton::_pMessageCenter  = NULL;
ThreadManager*  Singleton::_pThreadManager  = NULL;
EventManager*	Singleton::_pEventManager	= NULL;

Singleton::Singleton()
{
}

void Singleton::Destory()
{
	SAFE_DELETE(_pMessageCenter );
	SAFE_DELETE(_pThreadManager );
	SAFE_DELETE(_pEventManager );
}

MessageCenter* Singleton::GetMessageCenter()
{
    if ( ! _pMessageCenter )
        _pMessageCenter = new MessageCenter();
    return _pMessageCenter;
}

ThreadManager* Singleton::GetThreadManager()
{
	if ( ! _pThreadManager )     
		_pThreadManager = new ThreadManager();

	return _pThreadManager;
}

EventManager* Singleton::GetEventManager()
{
	if ( ! _pEventManager )     
		_pEventManager = new EventManager();

	return _pEventManager;
}
