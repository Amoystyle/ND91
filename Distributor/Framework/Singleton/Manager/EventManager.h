#pragma once

#include "Singleton.h"
#include "Base/BaseCSLock.h"
class IExit;
class IExitDB;

// ϵͳ�˳��¼�
class SINGLETON_API EventManager :BaseCSLock
{
    friend class Singleton;
public:

    void ExitSystem();          // ϵͳ�˳�֪ͨ��MainDlg::OnClose����

    HANDLE SystemExitEvent();       // ϵͳ�˳���Event�������߸���CloseHandle
    const bool* SystemExitFlag();   // ϵͳ�˳���־����

	void Attach(IExit* pExit);

	void AttachDB(IExitDB* pExit);

	void DeAttach(IExit* pExit);

	void DeAttachDB(IExitDB* pExit);

private:
    EventManager();     // private��ֻ����Singleton�д���
    ~EventManager();

    HANDLE DupHandle(HANDLE h); // ���ƾ��

    HANDLE  _systemExitEvent;   // ϵͳ�˳��¼�
    bool    _systemExit;        // ϵͳ�˳���־��TRUE��ʾ�˳�
                                // ��ע��X86Ӳ����֤�˶�д����ԭ����

	vector<IExit*>    _vecExitObserver;//�����˳�
	vector<IExitDB*>  _vecExitDB;
};

//�����˳��ӿ�
class SINGLETON_API IExit
{
public:
	IExit(){Singleton::GetEventManager()->Attach(this);}
	virtual ~IExit(){Singleton::GetEventManager()->DeAttach(this);}
	virtual void Exit() = 0;//����һ������
};

class SINGLETON_API IExitDB
{
public:
	IExitDB(){Singleton::GetEventManager()->AttachDB(this);}
	virtual ~IExitDB(){Singleton::GetEventManager()->DeAttachDB(this);}
	virtual void ExitDB() = 0;//����һ������
};