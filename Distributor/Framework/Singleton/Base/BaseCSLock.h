#pragma once

#include "Base/SingletonDefine.h"

#define AutoLock CSLock_Ext lockCS(this->GetLock());

/************************************************************************/
/* �����ࣺ��Ҫһ�������ֱ࣬����private�̳У��ڷ�����ʹ��AutoLock
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
/* �����ࣺ��Ҫһ�������ֱ࣬�Ӽ̳м���
/* ע�⣺������Ҫ�����Ķ���������Ӧ��װ������
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