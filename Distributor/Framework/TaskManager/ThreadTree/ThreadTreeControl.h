/*****************************************************************************
 *  @file 
 *  @brief [线程树的控制类，提供线程树中的全局控制方法，同时控制着线程树，锁节点，信号节点的增删]
 * 
 *  <b>文件名</b>      : ThreadTreeControl.h
 *  @n@n<b>版权所有</b>: 网龙天晴程序部应用软件开发组
 *  @n@n<b>作  者</b>  : alzq
 *  @n@n<b>创建时间</b>: 09-2-5 14:38:51
 *  @n@n<b>文件描述</b>: 
 *  @version  版本        修改者        时间        描述@n
 *  @n        [版本号]    alzq          2009-02-05  [描述]
 * 
*******************************************************************************/
#ifndef __THREAD_TREE_CONTROL__
#define __THREAD_TREE_CONTROL__

#include "ThreadControl.h"
//#include "TUIThread.h"

#pragma once

typedef list<ThreadInfo *>	WAITINGTHREADLIST,RUNNINGTHREADLIST;
typedef SINGLE *			LPSINGLE;
typedef ThreadControl *		LPTHREADCONTROL;
typedef ThreadInfo *		LPTHREADINFO;

//带入自定义的线程中的参数结构体，包含了用户函数指针与相关线程参数
struct CUSTOMTHREADINFO
{
	USER_FUNCTION userFunc;			//用户函数指针
	ThreadInfo * threadInfo;		//线程指针
};

//带入定时线程的两个EVENT的HANDLE
struct TimeControlEvents
{
	HANDLE timeEvent;
	HANDLE finishEvent;
};

//带入线程删除的线程中的结构体
struct DeleteThreadInfo
{
public:
	HANDLE timeEvent;					//定时使用的信号量
	HANDLE finishEvent;					//结束的表示信号量
	bool exitThread;					//是否需要退出删除线程
	THREADLIST threadList;				//等待删除的线程地址队列

	DeleteThreadInfo()
	{
		timeEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		finishEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		threadList.clear();
		exitThread = false;
	}
	~DeleteThreadInfo()
	{
		exitThread = true;
		if(NULL != timeEvent)
		{
			CloseHandle(timeEvent);
			timeEvent = NULL;
		}
		if(NULL != finishEvent)
		{
			CloseHandle(finishEvent);
			finishEvent = NULL;
		}
		threadList.clear();
	}
};

struct ThreadTreeGlobalInfo
{
	/**
	 *	--此变量暂时无效
	 **/
	int _g_iThreadExitTimeLimit;						//线程退出时的时间限制

	//static HANDLE _g_hThreadControlMutex;					//线程节点操作锁
	//static HANDLE _g_hSingleControlMutex;					//信号量或锁队列操作锁
	/**
	 *	--两个SECTION保证了同一时间只有一个线程在对SINGLE或线程队列进行操作
	 **/
	//CRITICAL_SECTION _g_csThreadControlSection;		//线程节点操作锁
	//CRITICAL_SECTION _g_csSingleControlSection;		//信号量或锁队列操作锁
	HANDLE _g_csThreadControlMutex;		//线程节点操作锁
	HANDLE _g_csSingleControlMutex;		//信号量或锁队列操作锁

	/**
	 *	--进行定时控制时的相关信号量（WINDOWS API的信号量）
	 **/
	HANDLE _g_hTimeThreadEvent;						//定时循环信号
	HANDLE _g_hTimeThreadFinishEvent;				//完成退出定时线程时有信号的信号量

	/**
	 *	--存储已经创建的SINGLE的队列，两个命名队列是为了方便创建SINGLE时查询同名SINGLE时建立的
	 **/
	EventList _g_elNamedEventList;					//已经被命名的信号量节点列表
	MutexList _g_mlNamedMutexList;					//已经被命名的锁节点列表
	SINGLELIST _g_slSingleList;						//创建线程为空的SINGLE队列

	/**
	 *	--存储线程相关队列，第一个为了遍历搜索线程而建立的，这样不需要遍历线程树就可以查询
	 *	--有限时间等待队列是在定时线程管理线程中，对有限时间等待线程的操作队列
	 *	--需要删除的线程队列，是对已经关闭的线程节点进行缓存删除的队列，当完全退出线程后才对节点进行删除
	 **/
	RUNNINGTHREADLIST _g_rtlThreadList;				//开启了的线程节点存储队列
	WAITINGTHREADLIST _g_wtlTimeLimitWaitingThread;	//有限时间等待线程队列
	DeleteThreadInfo * _g_dtiDeleteThreadInfo;		//需要删除的线程节点地址

	/**
	 *	--线程树根节点
	 **/
	TreeItem_t * _g_treThreadTree;					//线程树的根节点

	/**
	 *	--用于创建UI线程时，类指针存储对象，只进行临时性存储防止不同线程访问时冲突
	 **/
//	CRuntimeClass * _g_lpUIWndClass;
	//CreateUIWndInfo * _g_lpUIWndInfo;

	/**
	 *	--记录当前存在的线程的LOG文件路径
	 **/
	CTCHAR * _g_sAliveThreadFile;

	ThreadTreeGlobalInfo()
	{
		/**
		 *	--此变量暂时无效
		 **/
		_g_iThreadExitTimeLimit = 0;						//线程退出时的时间限制

		//static HANDLE _g_hThreadControlMutex;					//线程节点操作锁
		//static HANDLE _g_hSingleControlMutex;					//信号量或锁队列操作锁
		/**
		 *	--两个SECTION保证了同一时间只有一个线程在对SINGLE或线程队列进行操作
		 **/
		//CRITICAL_SECTION _g_csThreadControlSection;		//线程节点操作锁
		//CRITICAL_SECTION _g_csSingleControlSection;		//信号量或锁队列操作锁

		/**
		 *	--进行定时控制时的相关信号量（WINDOWS API的信号量）
		 **/
		_g_hTimeThreadEvent = NULL;						//定时循环信号
		_g_hTimeThreadFinishEvent = NULL;				//完成退出定时线程时有信号的信号量

		/**
		 *	--存储已经创建的SINGLE的队列，两个命名队列是为了方便创建SINGLE时查询同名SINGLE时建立的
		 **/
		//EventList _g_elNamedEventList;					//已经被命名的信号量节点列表
		//MutexList _g_mlNamedMutexList;					//已经被命名的锁节点列表
		//SINGLELIST _g_slSingleList;						//创建线程为空的SINGLE队列

		/**
		 *	--存储线程相关队列，第一个为了遍历搜索线程而建立的，这样不需要遍历线程树就可以查询
		 *	--有限时间等待队列是在定时线程管理线程中，对有限时间等待线程的操作队列
		 *	--需要删除的线程队列，是对已经关闭的线程节点进行缓存删除的队列，当完全退出线程后才对节点进行删除
		 **/
		//RUNNINGTHREADLIST _g_rtlThreadList;				//开启了的线程节点存储队列
		//WAITINGTHREADLIST _g_wtlTimeLimitWaitingThread;	//有限时间等待线程队列
		_g_dtiDeleteThreadInfo = NULL;		//需要删除的线程节点地址

		/**
		 *	--线程树根节点
		 **/
		_g_treThreadTree = NULL;					//线程树的根节点

		/**
		 *	--用于创建UI线程时，类指针存储对象，只进行临时性存储防止不同线程访问时冲突
		 **/
	//	_g_lpUIWndClass = NULL;
	//	_g_lpUIWndInfo = NULL;

		/**
		 *	--记录当前存在的线程的LOG文件路径
		 **/
		_g_sAliveThreadFile = NULL;
	}

	~ThreadTreeGlobalInfo()
	{
		/**
		 *	--此变量暂时无效
		 **/
		_g_iThreadExitTimeLimit = 0;						//线程退出时的时间限制

		//static HANDLE _g_hThreadControlMutex;					//线程节点操作锁
		//static HANDLE _g_hSingleControlMutex;					//信号量或锁队列操作锁
		/**
		 *	--两个SECTION保证了同一时间只有一个线程在对SINGLE或线程队列进行操作
		 **/
		//CRITICAL_SECTION _g_csThreadControlSection;		//线程节点操作锁
		//CRITICAL_SECTION _g_csSingleControlSection;		//信号量或锁队列操作锁

		/**
		 *	--进行定时控制时的相关信号量（WINDOWS API的信号量）
		 **/
		_g_hTimeThreadEvent = NULL;						//定时循环信号
		_g_hTimeThreadFinishEvent = NULL;				//完成退出定时线程时有信号的信号量

		/**
		 *	--存储已经创建的SINGLE的队列，两个命名队列是为了方便创建SINGLE时查询同名SINGLE时建立的
		 **/
		_g_elNamedEventList.clear();					//已经被命名的信号量节点列表
		_g_mlNamedMutexList.clear();					//已经被命名的锁节点列表
		_g_slSingleList.clear();						//创建线程为空的SINGLE队列

		/**
		 *	--存储线程相关队列，第一个为了遍历搜索线程而建立的，这样不需要遍历线程树就可以查询
		 *	--有限时间等待队列是在定时线程管理线程中，对有限时间等待线程的操作队列
		 *	--需要删除的线程队列，是对已经关闭的线程节点进行缓存删除的队列，当完全退出线程后才对节点进行删除
		 **/
		_g_rtlThreadList.clear();				//开启了的线程节点存储队列
		_g_wtlTimeLimitWaitingThread.clear();	//有限时间等待线程队列
		SAFE_DELETE(_g_dtiDeleteThreadInfo);		//需要删除的线程节点地址

		/**
		 *	--线程树根节点
		 **/
		SAFE_DELETE(_g_treThreadTree);					//线程树的根节点

		/**
		 *	--用于创建UI线程时，类指针存储对象，只进行临时性存储防止不同线程访问时冲突
		 **/
//		_g_lpUIWndClass = NULL;
//		_g_lpUIWndInfo = NULL;

		if(NULL != _g_sAliveThreadFile)
		{
			delete [] _g_sAliveThreadFile;
			_g_sAliveThreadFile = NULL;
		}
	}
};

#ifdef _LANG_TRANSLATE                  //                  //
#define LANG_EXPORT __declspec(dllexport)                   //                  //
#else                                   //                  //
#define LANG_EXPORT                     //                  //
#endif  

class LANG_EXPORT ThreadTreeControl
{
	friend class ThreadInfo;
	friend class TCWinThread;
	friend class ThreadControl;


public:
	/**
	 *
	 *	---公共变量，控制是否在DEBUG模式下输出线程操作信息
	 *
	 **/
	static bool _g_bLogSingleCreate;					//是否输出Single创建记录
	static bool _g_bLogSetSingle;						//是否输出设置信号量记录
	static bool _g_bLogWaitObj;							//是否输出等待记录

protected:
	/**
	 *	--全局变量结构体
	 **/
	static ThreadTreeGlobalInfo * _g_giGlobalInfo;
public:
	ThreadTreeControl(void);
	~ThreadTreeControl(void);

public:
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [线程树控制器的初始化函数]
	 * @n<b>函数名称</b>: InitThreadTreeControl
	 * @see               [参见函数]
	 * @n<b>作者</b>    :alzq
	 * @n<b>创建时间</b>: 09-2-11 10:26:39
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-11  [描述]
	 ******************************************************************************************************/
	static void InitThreadTreeControl(const CTCHAR * logPath);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [线程树控制器的析构函数]
	 * @n<b>函数名称</b>: DestroyThreadTreeControl
	 * @see               [参见函数]
	 * @n<b>作者</b>    :alzq
	 * @n<b>创建时间</b>: 09-2-11 10:38:23
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-11  [描述]
	 ******************************************************************************************************/
	static void DestroyThreadTreeControl(void);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [释放信号量或锁对象]
	 * @n<b>函数名称</b>        : TCloseSingle
	 * @n@param LPSINGLE single : [参数描述]
	 * @return                    [返回值描述]
	 * @see                       [参见函数]
	 * @n<b>作者</b>            :alzq
	 * @n<b>创建时间</b>        : 09-2-17 14:29:10
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-17  [描述]
	 ******************************************************************************************************/
	static BOOL TCloseSingle(LPSINGLE single);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [使用根节点释放锁]
	 * @n<b>函数名称</b>        : TReleaseMutex
	 * @n@param SINGLE * single : [参数描述]
	 * @see                       [参见函数]
	 * @n<b>作者</b>            :alzq
	 * @n<b>创建时间</b>        : 09-2-13 16:13:23
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-13  [描述]
	 ******************************************************************************************************/
	static UINT TReleaseMutex(SINGLE * single);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [使用根节点等待信号量或锁]
	 * @n<b>函数名称</b>         : WaitForSingle
	 * @n@param SINGLE * single  : [参数描述]
	 * @param   DWORD    millSec : [参数描述]
	 * @return                     [返回值描述]
	 * @see                        [参见函数]
	 * @n<b>作者</b>             :alzq
	 * @n<b>创建时间</b>         : 09-2-13 16:16:50
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-13  [描述]
	 ******************************************************************************************************/
	static UINT TWaitForSingle(SINGLE * single, DWORD millSec = INFINITE);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [创建线程节点，并将节点与控制类相绑定]
	 * @n<b>函数名称</b>                              : CreateThread
	 * @n@param ThreadControl *       control         : [用以存储线程节点信息的线程控制对象]
	 * @param   LPSECURITY_ATTRIBUTES lpsa            : [参考API]
	 * @param   DWORD                 dwStackSize     : [参考API]
	 * @param   USER_FUNCTION         pfnThreadProc   : [用户函数体，与API不同，带入参数将是线程节点的控制类]
	 * @param   LPVOID                lParam          : [用户需要带入线程中的参数]
	 * @param   DWORD                 dwCreationFlags : [参考API]
	 * @param   DWORD *               pdwThreadId     : [参考API]
	 * @return                                          [返回值描述]
	 * @see                                             [参见函数]
	 * @n<b>作者</b>                                  :alzq
	 * @n<b>创建时间</b>                              : 09-2-5 14:53:07
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-05  [描述]
	 ******************************************************************************************************/
	static bool TCreateThread(ThreadControl * control, LPSECURITY_ATTRIBUTES lpsa, DWORD dwStackSize, USER_FUNCTION pfnThreadProc
		, DELETE_THREAD_FUNCTION exitThread, LPVOID lParam, DWORD dwCreationFlags, DWORD * pdwThreadId, char * name = NULL);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [创建线程节点的简单方式]
	 * @n<b>函数名称</b>                      : CreateThread
	 * @n@param ThreadControl * control       : [用以存储线程节点信息的线程控制对象]
	 * @param   USER_FUNCTION   pfnThreadProc : [函数体指针]
	 * @param   LPVOID          lParam        : [带入参数]
	 * @see                                     [参见函数]
	 * @n<b>作者</b>                          :alzq
	 * @n<b>创建时间</b>                      : 09-2-5 14:58:52
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-05  [描述]
	 ******************************************************************************************************/
	static bool TCreateThread(ThreadControl * control, USER_FUNCTION pfnThreadProc, DELETE_THREAD_FUNCTION exitThread = NULL, LPVOID lParam = NULL, char * name = NULL);
	
	
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [创建锁节点]
	 * @n<b>函数名称</b>                : TCreateMutex
	 * @n@param ThreadInfo *  thread    : [创建该节点的线程信息节点地址]
	 * @param   BOOL          initState : [初始化锁状态，TRUE为有信号，FALSE为无信号]
	 * @param   const WCHAR * name      : [锁名称，默认为空]
	 * @return                            [返回值描述]
	 * @see                               [参见函数]
	 * @n<b>作者</b>                    :alzq
	 * @n<b>创建时间</b>                : 09-2-6 9:35:35
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-06  [描述]
	 ******************************************************************************************************/
	static LPSINGLE TCreateMutex(BOOL initOwner = FALSE, const CTCHAR * name = NULL);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [创建信号节点]
	 * @n<b>函数名称</b>                 : TCreateEvent
	 * @n@param ThreadInfo *  threadInfo : [创建该节点的线程信息节点地址]
	 * @param   BOOL          autoReSet  : [是否自动转化信号状态]
	 * @param   BOOL          initState  : [初始状态]
	 * @param   const WCHAR * name       : [信号量名称，默认为空]
	 * @return                             [返回值描述]
	 * @see                                [参见函数]
	 * @n<b>作者</b>                     :alzq
	 * @n<b>创建时间</b>                 : 09-2-6 9:44:15
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-06  [描述]
	 ******************************************************************************************************/
	static LPSINGLE TCreateEvent(BOOL autoReSet = FALSE, BOOL initState = FALSE, const CTCHAR * name = NULL);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [设置信号量为有信号状态]
	 * @n<b>函数名称</b>                : TSetEventSingle
	 * @param   SINGLE *     single     : [信号量的节点地址]
	 * @return                            [返回值描述]
	 * @see                               [参见函数]
	 * @n<b>作者</b>                    :alzq
	 * @n<b>创建时间</b>                : 09-2-6 15:14:36
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-06  [描述]
	 ******************************************************************************************************/
	static UINT TSetEventSingle(SINGLE * single);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [设置信号量为无信号状态]
	 * @n<b>函数名称</b>                : TSetEventNoSingle
	 * @param   SINGLE *     single     : [信号量的节点地址]
	 * @return                            [返回值描述]
	 * @see                               [参见函数]
	 * @n<b>作者</b>                    :alzq
	 * @n<b>创建时间</b>                : 09-2-6 15:15:05
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-06  [描述]
	 ******************************************************************************************************/
	static UINT TSetEventNoSingle(SINGLE * single);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [删除树节点，以及其子节点]
	 * @n<b>函数名称</b>                     : DeleteThread
	 * @n@param TreeItem_t * treeItem        : [参数描述]
	 * @param BOOL delChild                  : [是否删除子节点]
	 * @see                                    [参见函数]
	 * @n<b>作者</b>                         :alzq
	 * @n<b>创建时间</b>                     : 09-2-18 9:17:13
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-18  [描述]
	 ******************************************************************************************************/
	static void DeleteThread(TreeItem_t * treeItem, BOOL delChild, DWORD delayTime = INFINITE);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [删除指定线程以及其子线程]
	 * @n<b>函数名称</b>          : DeleteThread
	 * @n@param ThreadInfo * info : [参数描述]
	 * @param BOOL delChild       : [是否删除子节点]
	 * @see                         [参见函数]
	 * @n<b>作者</b>              :alzq
	 * @n<b>创建时间</b>          : 09-2-13 14:26:40
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-13  [描述]
	 ******************************************************************************************************/
	static void DeleteThread(ThreadInfo * info, BOOL delChild, DWORD delayTime = INFINITE);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [删除调用此函数的线程，主线程无效]
	 * @n<b>函数名称</b>      : DeleteThread
	 * @n@param BOOL delChild : [参数描述]
	 * @see                     [参见函数]
	 * @n<b>作者</b>          :alzq
	 * @n<b>创建时间</b>      : 09-2-23 15:28:33
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-23  [描述]
	 ******************************************************************************************************/
	static void DeleteThread(BOOL delChild, DWORD delayTime = INFINITE);

protected:
	/**
	 *
	 *	---自定义的线程函数
	 *
	 **/
	static unsigned int WINAPI _CustomThread(LPVOID lpParameter);
	/**
	 *
	 *	---有时间限制的线程管理线程
	 *
	 **/
	static DWORD WINAPI _TimeLimitThreadControlThread(LPVOID lpParameter);
	/**
	 *
	 *	---用于删除线程的管理线程,测试每个线程中锁的控制权，当获取到时则删除
	 *
	 **/
	static DWORD WINAPI _ThreadDeleteThread(LPVOID lpParameter);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [释放线程相关资源，并将线程添加到删除队列中]
	 * @n<b>函数名称</b>          : _DisposeThread
	 * @n@param ThreadInfo * info : [参数描述]
	 * @see                         [参见函数]
	 * @n<b>作者</b>              :alzq
	 * @n<b>创建时间</b>          : 09-2-24 14:31:29
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-24  [描述]
	 ******************************************************************************************************/
	static void _DisposeThread(ThreadInfo * info);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [在线程存储队列中查询]
	 * @n<b>函数名称</b>            : _FindRunningThread
	 * @n@param HANDLE threadHandle : [参数描述]
	 * @return                        [返回值描述]
	 * @see                           [参见函数]
	 * @n<b>作者</b>                :alzq
	 * @n<b>创建时间</b>            : 09-2-18 14:24:37
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-18  [描述]
	 ******************************************************************************************************/
	static ThreadInfo * _FindRunningThread(DWORD threadId);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [在线程存储队列中查询]
	 * @n<b>函数名称</b>            : _FindRunningThread
	 * @n@param CWinThread * winThread : [参数描述]
	 * @return                        [返回值描述]
	 * @see                           [参见函数]
	 * @n<b>作者</b>                :alzq
	 * @n<b>创建时间</b>            : 09-2-18 14:24:37
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-18  [描述]
	 ******************************************************************************************************/
	//static ThreadInfo * _FindRunningThread(CWinThread * winThread);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [在线程存储队列中删除指定节点]
	 * @n<b>函数名称</b>            : _DelRunningThread
	 * @n@param HANDLE threadHandle : [参数描述]
	 * @return                        [返回值描述]
	 * @see                           [参见函数]
	 * @n<b>作者</b>                :alzq
	 * @n<b>创建时间</b>            : 09-2-18 14:24:37
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-18  [描述]
	 ******************************************************************************************************/
	static void _DelRunningThread(DWORD threadId);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [根据本线程存储的相关参数挂起本线程]
	 * @n<b>函数名称</b>: TSuspendThread
	 * @n@param ThreadInfo * info : [参数描述]
	 * @see               [参见函数]
	 * @n<b>作者</b>    :alzq
	 * @n<b>创建时间</b>: 09-2-16 9:12:42
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-16  [描述]
	 ******************************************************************************************************/
	static void _TSuspendThread(ThreadInfo * info);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [根据本线程存储的相关参数恢复本线程]
	 * @n<b>函数名称</b>: TResumeThread
	 * @n@param ThreadInfo * info : [参数描述]
	 * @see               [参见函数]
	 * @n<b>作者</b>    :alzq
	 * @n<b>创建时间</b>: 09-2-16 9:12:42
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-16  [描述]
	 ******************************************************************************************************/
	static void _TResumeThread(ThreadInfo * info);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [树节点的删除函数]
	 * @n<b>函数名称</b>                     : _DisposeTreeItem
	 * @n@param TreeItem_t * treeItem        : [节点地址]
	 * @param   bool         terminateEnable : [是否允许强制终止线程]
	 * @see                                    [参见函数]
	 * @n<b>作者</b>                         :alzq
	 * @n<b>创建时间</b>                     : 09-2-18 9:15:53
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-18  [描述]
	 ******************************************************************************************************/
	static void _DisposeTreeItem(TreeItem_t * treeItem, DWORD delayTime = INFINITE);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [删除某树节点的所有子节点]
	 * @n<b>函数名称</b>                     : _DestroyThreadTreeChild
	 * @n@param TreeItem_t * treeItem        : [参数描述]
	 * @param   bool         terminateEnable : [参数描述]
	 * @see                                    [参见函数]
	 * @n<b>作者</b>                         :alzq
	 * @n<b>创建时间</b>                     : 09-2-18 9:06:35
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-18  [描述]
	 ******************************************************************************************************/
	static void _DestroyThreadTreeChild(TreeItem_t * treeItem);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [删除某一树节点及其之后的所有节点与子节点]
	 * @n<b>函数名称</b>              : _DestroyThreadTreeItems
	 * @n@param TreeItem_t * treeItem : [参数描述]
	 * @see                             [参见函数]
	 * @n<b>作者</b>                  :alzq
	 * @n<b>创建时间</b>              : 09-2-13 13:55:00
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-13  [描述]
	 ******************************************************************************************************/
	static void _DestroyThreadTreeItems(TreeItem_t * treeItem);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [设置一个线程开始（获取结束锁）]
	 * @n<b>函数名称</b>          : SetThreadStart
	 * @n@param ThreadInfo * info : [参数描述]
	 * @see                         [参见函数]
	 * @n<b>作者</b>              :alzq
	 * @n<b>创建时间</b>          : 09-2-13 14:24:55
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-13  [描述]
	 ******************************************************************************************************/
	static void _SetThreadStart(ThreadInfo * info);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [设置一个线程为结束（设置结束锁）]
	 * @n<b>函数名称</b>          : SetThreadFinish
	 * @n@param ThreadInfo * info : [参数描述]
	 * @see                         [参见函数]
	 * @n<b>作者</b>              :alzq
	 * @n<b>创建时间</b>          : 09-2-13 14:24:55
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-13  [描述]
	 ******************************************************************************************************/
	static void _SetThreadFinish(ThreadInfo * info);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [测试需要删除的线程队列，并将需要删除的线程节点删除]
	 * @n<b>函数名称</b>: TestAndDeleteThread
	 * @see               [参见函数]
	 * @n<b>作者</b>    :alzq
	 * @n<b>创建时间</b>: 09-2-17 9:34:35
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-17  [描述]
	 ******************************************************************************************************/
	static void _TestAndDeleteThread(void);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [遍历处理有限时间等待线程队列]
	 * @n<b>函数名称</b>: DealThreadTimeCount
	 * @see               [参见函数]
	 * @n<b>作者</b>    :alzq
	 * @n<b>创建时间</b>: 09-2-12 18:46:26
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-12  [描述]
	 ******************************************************************************************************/
	static void _DealThreadTimeCount(void);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [创建锁节点]
	 * @n<b>函数名称</b>                : TCreateMutex
	 * @n@param ThreadInfo *  thread    : [创建该节点的线程信息节点地址]
	 * @param   BOOL          initState : [初始化锁状态，TRUE为有信号，FALSE为无信号]
	 * @param   const WCHAR * name      : [锁名称，默认为空]
	 * @return                            [返回值描述]
	 * @see                               [参见函数]
	 * @n<b>作者</b>                    :alzq
	 * @n<b>创建时间</b>                : 09-2-6 9:35:35
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-06  [描述]
	 ******************************************************************************************************/
	static SINGLE * _TCreateMutex(ThreadInfo * threadInfo, BOOL initOwner = FALSE, const CTCHAR * name = NULL);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [创建信号节点]
	 * @n<b>函数名称</b>                 : TCreateEvent
	 * @n@param ThreadInfo *  threadInfo : [创建该节点的线程信息节点地址]
	 * @param   BOOL          autoReSet  : [是否自动转化信号状态]
	 * @param   BOOL          initState  : [初始状态]
	 * @param   const WCHAR * name       : [信号量名称，默认为空]
	 * @return                             [返回值描述]
	 * @see                                [参见函数]
	 * @n<b>作者</b>                     :alzq
	 * @n<b>创建时间</b>                 : 09-2-6 9:44:15
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-06  [描述]
	 ******************************************************************************************************/
	static SINGLE * _TCreateEvent(BOOL autoReSet = FALSE, BOOL initState = FALSE, const CTCHAR * name = NULL);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [给等待线程队列操作锁加锁]
	 * @n<b>函数名称</b>: _LockThreadControlMutex
	 * @return            [返回值描述]
	 * @see               [参见函数]
	 * @n<b>作者</b>    :alzq
	 * @n<b>创建时间</b>: 09-2-6 16:03:47
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-06  [描述]
	 ******************************************************************************************************/
	static void _LockThreadControlMutex(void);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [给等待线程队列操作锁解锁]
	 * @n<b>函数名称</b>: _UnLockThreadControlMutex
	 * @return            [返回值描述]
	 * @see               [参见函数]
	 * @n<b>作者</b>    :alzq
	 * @n<b>创建时间</b>: 09-2-6 16:04:04
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-06  [描述]
	 ******************************************************************************************************/
	static void _UnLockThreadControlMutex(void);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [获取信号量或锁队列操作锁]
	 * @n<b>函数名称</b>: _LockSingleControlMutex
	 * @return            [返回值描述]
	 * @see               [参见函数]
	 * @n<b>作者</b>    :alzq
	 * @n<b>创建时间</b>: 09-2-17 14:47:59
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-17  [描述]
	 ******************************************************************************************************/
	static void _LockSingleControlMutex(void);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [释放信号量或锁队列操作锁]
	 * @n<b>函数名称</b>: _UnLockSingleControlMutex
	 * @return            [返回值描述]
	 * @see               [参见函数]
	 * @n<b>作者</b>    :alzq
	 * @n<b>创建时间</b>: 09-2-17 14:48:19
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-17  [描述]
	 ******************************************************************************************************/
	static void _UnLockSingleControlMutex(void);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [根据线程等待的时间，在相关队列中添加线程节点，并根据等待时间决定是否添加到有限时间等待线程队列中]
	 * @n<b>函数名称</b>              : _AddWaitingThread
	 * @n@param ThreadInfo * lpThread : [参数描述]
	 * @param   DWORD        millSec  : [参数描述]
	 * @see                             [参见函数]
	 * @n<b>作者</b>                  :alzq
	 * @n<b>创建时间</b>              : 09-2-6 16:42:52
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-06  [描述]
	 ******************************************************************************************************/
	static void _AddWaitingThread(ThreadInfo * lpThread, DWORD millSec);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [在有时间限制的等待线程队列中删除制定的线程节点]
	 * @n<b>函数名称</b>              : _DelTimeLimitWaitingThread
	 * @n@param ThreadInfo * lpThread : [参数描述]
	 * @see                             [参见函数]
	 * @n<b>作者</b>                  :alzq
	 * @n<b>创建时间</b>              : 09-2-6 16:54:38
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-06  [描述]
	 ******************************************************************************************************/
	static void _DelTimeLimitWaitingThread(ThreadInfo * lpThread);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [根据名字查询同名锁]
	 * @n<b>函数名称</b>            : _FindMutexByName
	 * @n@param const CTCHAR * name : [参数描述]
	 * @return                        [返回值描述]
	 * @see                           [参见函数]
	 * @n<b>作者</b>                :alzq
	 * @n<b>创建时间</b>            : 09-2-12 13:49:50
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-12  [描述]
	 ******************************************************************************************************/
	static SINGLE * _FindMutexByName(const CTCHAR * name);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [根据名字查询同名信号量]
	 * @n<b>函数名称</b>            : _FindEventByName
	 * @n@param const CTCHAR * name : [参数描述]
	 * @return                        [返回值描述]
	 * @see                           [参见函数]
	 * @n<b>作者</b>                :alzq
	 * @n<b>创建时间</b>            : 09-2-12 14:48:41
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-12  [描述]
	 ******************************************************************************************************/
	static SINGLE * _FindEventByName(const CTCHAR * name);

	static void _OutputThread(TreeItem_t * treeItem, fstream * stream, LPSYSTEMTIME systemTime);
	//输出仍然在活动的内存中线程信息
	static void _OutPutAliveThread(void);
};

#endif
