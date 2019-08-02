#pragma once

#include "Base/SingletonDefine.h"

#define AutoLock CSLock_Ext lockCS(this->GetLock());

/************************************************************************/
/* 锁基类：需要一个锁的类，直接用private继承，在方法中使用AutoLock
/************************************************************************/
class SINGLETON_API BaseCSLock
{
public:
    BaseCSLock() 
    {
        InitializeCriticalSection(&_lock);
    }

    virtual ~BaseCSLock() 
    {
        DeleteCriticalSection(&_lock);
    }

    LPCRITICAL_SECTION GetLock() { return &_lock; }

private:
    CRITICAL_SECTION	_lock;
};

#define CSLOCKExt CSLock_Ext lockCS(&_csLock);
#define SetCSLockExt(e) CSLock_Ext lockCS(&e);

/************************************************************************/
/* 锁基类：需要一个锁的类，直接继承即可
/* 注意：凡是需要加锁的对象，锁处理应封装在类中
/************************************************************************/
class SINGLETON_API CSLock_Ext
{
public:
    CSLock_Ext(LPCRITICAL_SECTION pLock) 
        : _pLock(pLock) 
    {
        EnterCriticalSection(pLock);
    }
    virtual ~CSLock_Ext() 
    {
        LeaveCriticalSection(_pLock);
    }

private:
    LPCRITICAL_SECTION	_pLock;
};