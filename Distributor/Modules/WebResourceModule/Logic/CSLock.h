#pragma once

#define CSLOCK CSLock lockCS(&_csLock);
#define SetCSLock(e) CSLock lockCS(&e);
class CSLock
{
public:
    CSLock(LPCRITICAL_SECTION pLock) 
    : _pLock(pLock) 
    {
        EnterCriticalSection(pLock);
    }
    ~CSLock() 
    {
        LeaveCriticalSection(_pLock);
    }

private:
    LPCRITICAL_SECTION	_pLock;
};