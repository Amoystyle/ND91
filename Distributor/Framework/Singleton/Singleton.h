#pragma once

#include "Base/SingletonDefine.h"
#include "Base/Config.h"

#pragma warning (disable:4251)

class MessageCenter;
class ThreadManager;
class EventManager;

// Singleton： 单例类
// 需要单例的类都放在这里，以便控制初始化顺序
// 需要单例的类：须将构造函数私有化，并设置Singleton为friend class
class SINGLETON_API Singleton
{
public:
    static void         Destory();

    static void Init(HWND hWnd) { _hWnd = hWnd; }
    static HWND GetMainWnd() { return _hWnd; }

	static MessageCenter*   GetMessageCenter();
	static ThreadManager*   GetThreadManager();
	static EventManager*	GetEventManager();

private:
    Singleton();
    ~Singleton(){}
    static Singleton*   _pInstance;
    static HWND         _hWnd;

	static MessageCenter*	_pMessageCenter;
	static ThreadManager*	_pThreadManager;
	static EventManager*	_pEventManager;
};