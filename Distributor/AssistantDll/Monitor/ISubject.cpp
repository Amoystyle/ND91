#include "stdafx.h"
#include "Monitor/ISubject.h"

ISubject::ISubject()
{
}

ISubject::~ISubject()
{
    _observers.clear();
}

void ND91Assistant::ISubject::Attach( IObserver* pObserver )
{
    RETURN_IF( !pObserver );

    // 检查该Observer是否已经在队列中
    std::vector<IObserver*>::iterator it;
    for(it = _observers.begin(); it != _observers.end(); ++it)
    {
        if ( pObserver == *it )
        {
            return;
        }
    }

    // 不在队列中则添加
    _observers.push_back(pObserver);
}

void ND91Assistant::ISubject::Detach( IObserver* pObserver )
{
    RETURN_IF( !pObserver );

    std::vector<IObserver*>::iterator it;
    for(it = _observers.begin(); it != _observers.end(); ++it)
    {
        if ( pObserver == *it )
        {
            _observers.erase(it);
            break;
        }
    }
}

IObserver* ND91Assistant::ISubject::GetFirstObserver()
{
    if (_observers.size() > 0)
        return _observers[0];
    return NULL;
}
