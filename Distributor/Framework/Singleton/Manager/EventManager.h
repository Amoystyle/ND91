#pragma once

#include "Singleton.h"
#include "Base/BaseCSLock.h"
class IExit;
class IExitDB;

// 系统退出事件
class SINGLETON_API EventManager :BaseCSLock
{
    friend class Singleton;
public:

    void ExitSystem();          // 系统退出通知，MainDlg::OnClose调用

    HANDLE SystemExitEvent();       // 系统退出的Event，调用者负责CloseHandle
    const bool* SystemExitFlag();   // 系统退出标志变量

	void Attach(IExit* pExit);

	void AttachDB(IExitDB* pExit);

	void DeAttach(IExit* pExit);

	void DeAttachDB(IExitDB* pExit);

private:
    EventManager();     // private，只能在Singleton中创建
    ~EventManager();

    HANDLE DupHandle(HANDLE h); // 复制句柄

    HANDLE  _systemExitEvent;   // 系统退出事件
    bool    _systemExit;        // 系统退出标志：TRUE表示退出
                                // 附注：X86硬件保证了读写操作原子性

	vector<IExit*>    _vecExitObserver;//所有退出
	vector<IExitDB*>  _vecExitDB;
};

//接受退出接口
class SINGLETON_API IExit
{
public:
	IExit(){Singleton::GetEventManager()->Attach(this);}
	virtual ~IExit(){Singleton::GetEventManager()->DeAttach(this);}
	virtual void Exit() = 0;//新增一个任务
};

class SINGLETON_API IExitDB
{
public:
	IExitDB(){Singleton::GetEventManager()->AttachDB(this);}
	virtual ~IExitDB(){Singleton::GetEventManager()->DeAttachDB(this);}
	virtual void ExitDB() = 0;//新增一个任务
};