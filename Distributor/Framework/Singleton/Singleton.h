#pragma once

#include "Base/SingletonDefine.h"
#include "Base/Config.h"

#pragma warning (disable:4251)

class MessageCenter;
class ThreadManager;
class EventManager;

// Singleton�� ������
// ��Ҫ�������඼��������Ա���Ƴ�ʼ��˳��
// ��Ҫ�������ࣺ�뽫���캯��˽�л���������SingletonΪfriend class
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