#include "StdAfx.h"
#include "ThreadTreeControl.h"
#include "ThreadLog.h"
#include "process.h"

/**
 *	--区域内变量含义参照头文件
 **/
//int ThreadTreeControl::_g_iThreadExitTimeLimit = INFINITE;						//线程退出时的时间限制
//CRITICAL_SECTION ThreadTreeControl::_g_csThreadControlSection;		//线程节点操作锁
//CRITICAL_SECTION ThreadTreeControl::_g_csSingleControlSection;		//信号量或锁队列操作锁
//HANDLE ThreadTreeControl::_g_hTimeThreadEvent = NULL;						//定时循环信号
//HANDLE ThreadTreeControl::_g_hTimeThreadFinishEvent = NULL;				//完成退出定时线程时有信号的信号量
//EventList ThreadTreeControl::_g_elNamedEventList;					//已经被命名的信号量节点列表
//MutexList ThreadTreeControl::_g_mlNamedMutexList;					//已经被命名的锁节点列表
//SINGLELIST ThreadTreeControl::_g_slSingleList;						//创建线程为空的SINGLE队列
//RUNNINGTHREADLIST ThreadTreeControl::_g_rtlThreadList;				//开启了的线程节点存储队列
//WAITINGTHREADLIST ThreadTreeControl::_g_wtlTimeLimitWaitingThread;	//有限时间等待线程队列
//TreeItem_t * ThreadTreeControl::_g_treThreadTree = NULL;					//线程树的根节点
//DeleteThreadInfo * ThreadTreeControl::_g_dtiDeleteThreadInfo;			//需要删除的线程节点地址
//CRuntimeClass * ThreadTreeControl::_g_lpUIWndClass;
//CreateUIWndInfo * ThreadTreeControl::_g_lpUIWndInfo;
ThreadTreeGlobalInfo * ThreadTreeControl::_g_giGlobalInfo = NULL;

bool ThreadTreeControl::_g_bLogSingleCreate;					//是否输出Single创建记录
bool ThreadTreeControl::_g_bLogSetSingle;						//是否输出设置信号量记录
bool ThreadTreeControl::_g_bLogWaitObj;							//是否输出等待记录

/**
 *
 *	---自定义的线程函数
 *
 **/
unsigned int WINAPI ThreadTreeControl::_CustomThread(LPVOID lpParameter)
{INTERFUN;
	bool threadError = false;				//线程执行是否出现错误

	if(NULL == lpParameter)
	{
		//带入空参数
#ifdef _DEBUG
		DebugBreak();
#endif

		threadError = true;
	}
	
	CUSTOMTHREADINFO * customThreadInfo = (CUSTOMTHREADINFO *)lpParameter;	//带入线程执行相关参数

	//获取线程标识ID
	ThreadInfo * info = customThreadInfo->threadInfo;

	//获取结束锁
	_SetThreadStart(info);

	if(!threadError)
	{
#ifndef _DEBUG
		try
		{
#endif
			USER_FUNCTION userFunc = NULL;

			userFunc = customThreadInfo->userFunc;

			if(NULL == userFunc)
			{
				//用户函数指针为空
#ifdef _DEBUG
				DebugBreak();
#endif

				threadError = true;
			}

			if(!threadError)
			{
				userFunc(info->_GetUserInfo());
			}
#ifndef _DEBUG
		}
		catch(...)
		{
			//函数执行出错
			ThreadLog::AddLog(info, "error finish!");
#ifdef _DEBUG
			DebugBreak();
#endif

			threadError = true;
		}
#endif
	}

	//删除线程节点操作，检查是否存在未释放锁
	if(threadError)
	{
		//线程执行过程中出现过错误
	}
	else
	{
		//正常执行结束
	}

	//删除本节点，不删除子节点，并将子线程托管
	_DisposeThread(info);

	//DeleteThread(customThreadInfo->threadInfo, FALSE);

	//设置结束信号
	_SetThreadFinish(info);

	::_endthreadex(0);
	return 0;
} 
	

/**
 *
 *	---有时间限制的线程管理线程
 *
 **/
DWORD WINAPI ThreadTreeControl::_TimeLimitThreadControlThread(LPVOID lpParameter)
{INTERFUN;
	//获取两个信号量后删除指针
	TimeControlEvents * events = (TimeControlEvents *)lpParameter;
	HANDLE timeEvent = events->timeEvent;
	HANDLE finishEvent = events->finishEvent;
	int printTimeCount = 0;
	SAFE_DELETE(events);

	ThreadLog::AddLog("Start time limit thread control thread...");
	//循环10毫秒，每次对有时间限制的线程进行检测，将超时线程设置状态并重新激活
	while(WaitForSingleObject(timeEvent, 10) == WAIT_TIMEOUT)
	{
		//当超时时对所有有限时间等待线程队列进行处理
		_DealThreadTimeCount();

		/**
		 *	--定时输出内存线程信息函数部分，时间需要*10毫秒
		 **/
		printTimeCount++;

		if(printTimeCount >= 1000)
		{
			//60秒输出一次
			_OutPutAliveThread();

			printTimeCount = 0;
		}
	}
	ThreadLog::AddLog("Finish time limit thread control thread...");

	SetEvent(finishEvent);

	return 0;
}
	

/**
 *
 *	---用于删除线程的管理线程,测试每个线程中锁的控制权，当获取到时则删除
 *
 **/
DWORD WINAPI ThreadTreeControl::_ThreadDeleteThread(LPVOID lpParameter)
{INTERFUN;
	DeleteThreadInfo * info = (DeleteThreadInfo *)lpParameter;
	HANDLE timeEvent = info->timeEvent;
	HANDLE finishEvent = info->finishEvent;
	
	//定时循环，由带入结构体中变量决定是否退出循环
	ThreadLog::AddLog("Start thread delete thread...");
	while(!info->exitThread || !info->threadList.empty())
	{
		//定时操作
		//每1秒进行一次循环检查
		WaitForSingleObject(timeEvent, 1000);

		//循环检测每个线程的结束锁，当可以删除时进行删除
		_TestAndDeleteThread();
	}
	ThreadLog::AddLog("Finish thread delete thread...");

	//设置结束信号为有信号
	SetEvent(finishEvent);

	return 0;
}
	


ThreadTreeControl::ThreadTreeControl(void)
{INTERFUN;
	//_g_treThreadTree = NULL;

	////_g_hThreadControlMutex = NULL;				//线程节点操作锁
	////_g_hSingleControlMutex = NULL;				//信号量或锁队列操作锁
	////_g_csThreadControlSection = NULL;				//线程节点操作锁
	////_g_csSingleControlSection = NULL;				//信号量或锁队列操作锁
	//_g_hTimeThreadEvent = NULL;						//定时循环信号
	//_g_hTimeThreadFinishEvent = NULL;				//完成退出定时线程时有信号的信号量

	//_g_elNamedEventList.clear();					//已经被命名的信号量节点列表
	//_g_mlNamedMutexList.clear();					//已经被命名的锁节点列表
	//_g_slSingleList.clear();
	//_g_wtlTimeLimitWaitingThread.clear();			//有限时间等待线程队列
}

ThreadTreeControl::~ThreadTreeControl(void)
{INTERFUN;
}
	

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
void ThreadTreeControl::InitThreadTreeControl(const CTCHAR * logPath)
{INTERFUN;
	if(NULL == _g_giGlobalInfo)
	{
		_g_giGlobalInfo = new ThreadTreeGlobalInfo();

		ThreadLog::InitThreadLog(logPath);

		size_t len = _tcslen(logPath);
		_g_giGlobalInfo->_g_sAliveThreadFile = new CTCHAR[len + 2];	//后缀多L，并多\0
		_tcsncpy_s(_g_giGlobalInfo->_g_sAliveThreadFile, len + 2, logPath, len);
		_g_giGlobalInfo->_g_sAliveThreadFile[len] = _T('L');
		_g_giGlobalInfo->_g_sAliveThreadFile[len + 1] = _T('\0');

		//判断根节点是否存在，不存在则创建
		if(NULL == _g_giGlobalInfo->_g_treThreadTree)
		{
			//创建根节点中相关信息，为NULL
			CUSTOMTHREADINFO * customThread = new CUSTOMTHREADINFO();
			customThread->userFunc = NULL;
			customThread->threadInfo = new ThreadInfo(NULL, NULL, NULL, NULL);

			//设置线程节点中的线程信息
			customThread->threadInfo->_m_ttThreadTarget.isMainThread = TRUE;
			customThread->threadInfo->_m_ttThreadTarget.isWinThread = false;
			customThread->threadInfo->_m_ttThreadTarget.winThread = NULL;
			customThread->threadInfo->_m_ttThreadTarget.threadId = ::GetCurrentThreadId();

			Data_t date;
			date.customThreadInfo = customThread;
			_g_giGlobalInfo->_g_treThreadTree = CreateTree(&date);
			customThread->threadInfo->_m_lpTreeItem = _g_giGlobalInfo->_g_treThreadTree;
			_g_giGlobalInfo->_g_rtlThreadList.push_back(customThread->threadInfo);
		}
		else
		{
			//当根节点不为空表示已经经过了初始化，因此直接返回
			return ;
		}

		_g_giGlobalInfo->_g_iThreadExitTimeLimit = INFINITE;

		//_g_hThreadControlMutex = CreateMutex(NULL, FALSE, NULL);		//线程节点操作锁
		//_g_hSingleControlMutex = CreateMutex(NULL, FALSE, NULL);		//线程节点操作锁
		//InitializeCriticalSection(&_g_giGlobalInfo->_g_csThreadControlSection);			//线程节点操作锁
		//InitializeCriticalSection(&_g_giGlobalInfo->_g_csSingleControlSection);			//信号量或锁队列操作锁
		_g_giGlobalInfo->_g_csThreadControlMutex = ::CreateMutex(NULL, FALSE, NULL);		//线程节点操作锁
		_g_giGlobalInfo->_g_csSingleControlMutex = ::CreateMutex(NULL, FALSE, NULL);;		//信号量或锁队列操作锁
		_g_giGlobalInfo->_g_hTimeThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		_g_giGlobalInfo->_g_hTimeThreadFinishEvent = CreateEvent(NULL, FALSE, FALSE, NULL);			//完成退出定时线程时有信号的信号量

		_g_giGlobalInfo->_g_elNamedEventList.clear();					//已经被命名的信号量节点列表
		_g_giGlobalInfo->_g_mlNamedMutexList.clear();					//已经被命名的锁节点列表
		_g_giGlobalInfo->_g_slSingleList.clear();
		_g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.clear();			//有限时间等待线程队列

		TimeControlEvents * events = new TimeControlEvents();
		events->timeEvent = _g_giGlobalInfo->_g_hTimeThreadEvent;
		events->finishEvent = _g_giGlobalInfo->_g_hTimeThreadFinishEvent;

		_g_giGlobalInfo->_g_dtiDeleteThreadInfo = new DeleteThreadInfo();
		
	#ifdef _DEBUG
		_g_bLogSingleCreate = true;					//是否输出线程控制锁获取部分
		_g_bLogSetSingle = true;						//是否输出线程对SINGLE对象的等待过程前后
		_g_bLogWaitObj = true;							//是否输出挂起和恢复线程
	#else
		_g_bLogSingleCreate = true;					//是否输出线程控制锁获取部分
		_g_bLogSetSingle = false;						//是否输出线程对SINGLE对象的等待过程前后
		_g_bLogWaitObj = false;							//是否输出挂起和恢复线程
	#endif

		::CreateThread(NULL, NULL, ThreadTreeControl::_TimeLimitThreadControlThread, events, NULL, NULL);
		::CreateThread(NULL, NULL, ThreadTreeControl::_ThreadDeleteThread, _g_giGlobalInfo->_g_dtiDeleteThreadInfo, NULL, NULL);

//		_g_giGlobalInfo->_g_lpUIWndClass = NULL;
//		_g_giGlobalInfo->_g_lpUIWndInfo = NULL;
	}
}
	

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
void ThreadTreeControl::DestroyThreadTreeControl(void)
{INTERFUN;
	//判断根节点是否存在，不存在则创建
	if(NULL != _g_giGlobalInfo)
	{
		//关闭所有线程
		ThreadLog::AddLog("Start destroy all thread...\n");

		//遍历删除根节点下的所有线程节点
		if(NULL != _g_giGlobalInfo->_g_treThreadTree)
		{
			CUSTOMTHREADINFO * threadInfo = (CUSTOMTHREADINFO *)_g_giGlobalInfo->_g_treThreadTree->data.customThreadInfo;
			if(NULL != threadInfo)
			{
				threadInfo->threadInfo->_m_ttThreadTarget.isWinThread = false;
				threadInfo->threadInfo->_m_ttThreadTarget.handle = NULL;
				threadInfo->threadInfo->_m_ttThreadTarget.winThread = NULL;
			}
			else
			{
#ifdef _DEBUG
				DebugBreak();
#endif
			}

			//删除树中所有节点
			DeleteThread(_g_giGlobalInfo->_g_treThreadTree, TRUE);
		}
		else
		{
			//当根节点为空表示未经过初始化，因此直接返回
			return ;
		}

		_g_giGlobalInfo->_g_treThreadTree = NULL;

		ThreadLog::AddLog("Finish time thread ...\n");
		//关闭定时线程
		SetEvent(_g_giGlobalInfo->_g_hTimeThreadEvent);
		WaitForSingleObject(_g_giGlobalInfo->_g_hTimeThreadFinishEvent, INFINITE);

		ThreadLog::AddLog("Finish delete thread ...\n");
		_g_giGlobalInfo->_g_dtiDeleteThreadInfo->exitThread = true;
		WaitForSingleObject(_g_giGlobalInfo->_g_dtiDeleteThreadInfo->finishEvent, INFINITE);

		ThreadLog::AddLog("Finish destroy all thread...\n");

		//删除所有创建线程为NULL的SINGLE
		ThreadLog::AddLog("Start destroy all single...\n");

		_LockSingleControlMutex();

#ifdef _DEBUG
		//DEBUG模式下提示有SINGLE未删除
		if(!_g_giGlobalInfo->_g_slSingleList.empty())     
		{
			TRACE(_T("--debug warning--\n    some singles have not been destroyed!\n--end debug warning--\n"));
		}
#endif

		USES_CONVERSION;
		SINGLELIST::iterator iter = _g_giGlobalInfo->_g_slSingleList.begin();
		while(iter != _g_giGlobalInfo->_g_slSingleList.end())
		{
			SINGLE * single = (*iter);
			if(SINGLE_EVENT == single->GetType())
			{
				ThreadEvent * cEvent = (ThreadEvent *)single->GetSingle();

				if(_g_bLogSingleCreate)
				{
					//需要对SINGLE创建进行输出则输出
					CStringA mes;
					mes.Format("destroy thread tree close event name: \"%s\" -- address: 0x%x "
						,W2A(cEvent->_m_csEventName), single);

					ThreadLog::AddLog(mes);
				}

				SAFE_DELETE(cEvent);
			}
			else if(SINGLE_MUTEX == single->GetType())
			{
				ThreadMutex * cMutex = (ThreadMutex *)single->GetSingle();

				if(_g_bLogSingleCreate)
				{
					//需要对SINGLE创建进行输出则输出
					CStringA mes;
					mes.Format("destroy thread tree close mutex name: \"%s\" -- address: 0x%x "
						,W2A(cMutex->_m_csMutexName), single);

					ThreadLog::AddLog(mes);
				}

				SAFE_DELETE(cMutex);
			}

			SAFE_DELETE(*iter);

			iter++;
		}
		_g_giGlobalInfo->_g_slSingleList.clear();
		
		//将有名称的存储队列清空
		_g_giGlobalInfo->_g_elNamedEventList.clear();					//已经被命名的信号量节点列表
		_g_giGlobalInfo->_g_mlNamedMutexList.clear();					//已经被命名的锁节点列表
		
		_UnLockSingleControlMutex();
		ThreadLog::AddLog("Finish destroy all single...\n");

		//CloseHandle(_g_hThreadControlMutex);		//线程节点操作锁
		//CloseHandle(_g_hSingleControlMutex);		//线程节点操作锁
		//DeleteCriticalSection(&_g_giGlobalInfo->_g_csThreadControlSection);			//线程节点操作锁
		//DeleteCriticalSection(&_g_giGlobalInfo->_g_csSingleControlSection);			//信号量或锁队列操作锁
		::CloseHandle(_g_giGlobalInfo->_g_csThreadControlMutex);		//线程节点操作锁
		::CloseHandle(_g_giGlobalInfo->_g_csSingleControlMutex);		//信号量或锁队列操作锁
		CloseHandle(_g_giGlobalInfo->_g_hTimeThreadEvent);
		CloseHandle(_g_giGlobalInfo->_g_hTimeThreadFinishEvent);
		_g_giGlobalInfo->_g_elNamedEventList.clear();							//已经被命名的信号量节点列表
		_g_giGlobalInfo->_g_mlNamedMutexList.clear();							//已经被命名的锁节点列表
		_g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.clear();					//有限时间等待线程队列

		_g_giGlobalInfo->_g_treThreadTree = NULL;					//线程树的根节点
		
		SAFE_DELETE(_g_giGlobalInfo->_g_dtiDeleteThreadInfo);

		SAFE_DELETE(_g_giGlobalInfo);

		ThreadLog::CloseThreadLog();
	}
}
	

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
BOOL ThreadTreeControl::TCloseSingle(LPSINGLE single)
{INTERFUN;
	if(NULL == single)
	{
		return FALSE;
	}

	_LockSingleControlMutex();

	SINGLELIST::iterator iter = _g_giGlobalInfo->_g_slSingleList.begin();
	while(iter != _g_giGlobalInfo->_g_slSingleList.end())
	{
		if((*iter) == single)
		{
			//查询到用户需要删除的SINGLE
			LPSINGLE lpSingle = (*iter);

			//当SINGLE有名称时，在对应有名称的队列中删除
			if(SINGLE_EVENT == lpSingle->GetType())
			{
				//信号量
				ThreadEvent * cEvent = (ThreadEvent *)lpSingle->GetSingle();

				if(NULL != cEvent->_m_csEventName)
				{
					//在有名称的队列中删除该信号量
					EventList::iterator eventIter = _g_giGlobalInfo->_g_elNamedEventList.begin();
					while(eventIter != _g_giGlobalInfo->_g_elNamedEventList.end())
					{
						if((*eventIter) == lpSingle)
						{
							//在有名称的信号量队列中删除该信号量
							_g_giGlobalInfo->_g_elNamedEventList.erase(eventIter);
							break;
						}

						eventIter++;
					}
				}

				if(_g_bLogSingleCreate)
				{
					//需要对SINGLE创建进行输出则输出
					//查询当前线程所在节点
					ThreadInfo * threadInfo = _FindRunningThread(::GetCurrentThreadId());

					USES_CONVERSION;
					CStringA mes;
					mes.Format("close event name: \"%s\" -- address: 0x%x "
						,W2A(cEvent->_m_csEventName), single);

					ThreadLog::AddLog(threadInfo, mes);
				}

				//删除对象
				SAFE_DELETE(cEvent);
			}
			else
			{
				//锁
				ThreadMutex * cMutex = (ThreadMutex *)lpSingle->GetSingle();

				if(NULL != cMutex->_m_csMutexName)
				{
					//在有名称的队列中删除该信号量
					EventList::iterator mutexIter = _g_giGlobalInfo->_g_mlNamedMutexList.begin();
					while(mutexIter != _g_giGlobalInfo->_g_mlNamedMutexList.end())
					{
						if((*mutexIter) == lpSingle)
						{
							//在有名称的信号量队列中删除该信号量
							_g_giGlobalInfo->_g_mlNamedMutexList.erase(mutexIter);
							break;
						}

						mutexIter++;
					}
				}
			
				if(_g_bLogSingleCreate)
				{
					//需要对SINGLE创建进行输出则输出
					//查询当前线程所在节点
					ThreadInfo * threadInfo = _FindRunningThread(::GetCurrentThreadId());

					USES_CONVERSION;
					CStringA mes;
					mes.Format("close mutex name: \"%s\" -- address: 0x%x "
						,W2A(cMutex->_m_csMutexName), single);

					ThreadLog::AddLog(threadInfo, mes);
				}

				//将拥有此锁控制权的线程内的所有锁节点删除
				if(NULL != cMutex->_m_tlControlThread)
				{
					((ThreadInfo *)(cMutex->_m_tlControlThread))->_DelControlMutex(single);
				}

				//删除对象
				SAFE_DELETE(cMutex);
			}

			//删除SINGLE结构体
			SAFE_DELETE(*iter);

			//在SINGLE队列中删除
			_g_giGlobalInfo->_g_slSingleList.erase(iter);
			break;
		}

		iter++;
	}

	_UnLockSingleControlMutex();

	return TRUE;
}
	

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
UINT ThreadTreeControl::TReleaseMutex(SINGLE * single)
{INTERFUN;
	if(NULL == _g_giGlobalInfo->_g_treThreadTree)
	{
		return RELEASE_MUTEX_ERROR;
	}


	//查询当前线程所在节点
	ThreadInfo * threadInfo = _FindRunningThread(::GetCurrentThreadId());

	if(NULL != threadInfo)
	{
		return threadInfo->_TReleaseMutex(single);
	}
	else
	{
#ifdef _DEBUG
		DebugBreak();
#endif
		return RELEASE_MUTEX_ERROR;
	}
}
	

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
UINT ThreadTreeControl::TWaitForSingle(SINGLE * single, DWORD millSec)
{INTERFUN;
	if(NULL == _g_giGlobalInfo->_g_treThreadTree)
	{
		return WAIT_SINGLE_ERROR;
	}


	//查询当前线程所在节点
	ThreadInfo * threadInfo = _FindRunningThread(::GetCurrentThreadId());

	if(NULL != threadInfo)
	{
#ifdef _DEBUG
		if(false && INFINITE == millSec && threadInfo->_m_ttThreadTarget.isMainThread)
		{
			UINT res = threadInfo->_TWaitForSingle(single, 5000);
			if(WAIT_SINGLE_TIMEOUT == res)
			{
				DebugBreak();
				res = TWaitForSingle(single, INFINITE);
			}
			return res;
		}
		else
		{
			return threadInfo->_TWaitForSingle(single, millSec);
		}
#else
		return threadInfo->_TWaitForSingle(single, millSec);
#endif
	}
	else
	{
#ifdef _DEBUG
		DebugBreak();
#endif
		return WAIT_SINGLE_ERROR;
	}
}
	

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
bool ThreadTreeControl::TCreateThread(ThreadControl * control, LPSECURITY_ATTRIBUTES lpsa, DWORD dwStackSize
									  , USER_FUNCTION pfnThreadProc, DELETE_THREAD_FUNCTION exitThread
									  , LPVOID lParam, DWORD dwCreationFlags, DWORD * pdwThreadId, char * name)
{INTERFUN;
	_LockThreadControlMutex();

	//创建带入线程内参数信息
	CUSTOMTHREADINFO * customThread = new CUSTOMTHREADINFO();
	customThread->userFunc = pfnThreadProc;
	customThread->threadInfo = new ThreadInfo(lParam, exitThread, control, name);

	unsigned int threadID;
	//HANDLE threadHandle = ::CreateThread(lpsa, dwStackSize, _CustomThread, customThread, dwCreationFlags, &threadID);
	HANDLE threadHandle = (HANDLE)::_beginthreadex(lpsa, dwStackSize, _CustomThread, customThread, dwCreationFlags, &threadID);

	if(NULL != pdwThreadId)
	{
		* pdwThreadId = threadID;
	}

	if(NULL == threadHandle)
	{
		//当获取到的线程句柄为空表示创建失败
		delete customThread->threadInfo;
		customThread->threadInfo = NULL;
		SAFE_DELETE(customThread);

		_UnLockThreadControlMutex();
		return false;
	}

	//设置线程ID
	customThread->threadInfo->_m_ttThreadTarget.threadId = threadID;
	
	ThreadLog::AddLog(customThread->threadInfo, "create thread suc");

	TreeItem_t * parent = NULL;

	//带入父节点为空时，使用根节点
	//parent = _g_treThreadTree;
	ThreadInfo * parentThread = _FindRunningThread(::GetCurrentThreadId());
	if(NULL == parentThread)
	{
		//主线程开启线程
		parent = _g_giGlobalInfo->_g_treThreadTree;
	}
	else
	{
		parent = (TreeItem_t *)parentThread->_m_lpTreeItem;
	}

	//设置线程节点中的线程信息
	customThread->threadInfo->_m_ttThreadTarget.isWinThread = false;
	customThread->threadInfo->_m_ttThreadTarget.handle = threadHandle;

	//创建根节点中相关信息，为NULL

	//声明存储于树中的信息
	Data_t date;
	//将线程信息放入树中节点信息内
	date.customThreadInfo = customThread;
	//在父亲节点下创建子节点并带如相关信息
	TreeItem_t * newThread = AppendTreeItem(parent, &date);
	if(NULL != control)
	{
		if(NULL != control->_m_tiThreadInfo)
		{
			//当CONTROL对象已经有控制的线程时，需要将原本控制线程的CONTROL对象设置为空
			((CUSTOMTHREADINFO *)control->_m_tiThreadInfo->data.customThreadInfo)->threadInfo->_m_lpControl = NULL;
		}
		control->_m_tiThreadInfo = newThread;
	}
	customThread->threadInfo->_m_lpTreeItem = newThread;

	_g_giGlobalInfo->_g_rtlThreadList.push_back(customThread->threadInfo);

	_UnLockThreadControlMutex();

	return true;
}

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [创建线程节点的简单方式]
 * @n<b>函数名称</b>                      : CreateThread
 * @n@param ThreadControl * control       : [用以存储线程节点信息的线程控制对象]
 * @param   TreeItem_t *    parentThread  : [父亲线程的节点地址]
 * @param   USER_FUNCTION   pfnThreadProc : [函数体指针]
 * @param   LPVOID          lParam        : [带入参数]
 * @see                                     [参见函数]
 * @n<b>作者</b>                          :alzq
 * @n<b>创建时间</b>                      : 09-2-5 14:58:52
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-05  [描述]
 ******************************************************************************************************/
bool ThreadTreeControl::TCreateThread(ThreadControl * control, USER_FUNCTION pfnThreadProc, DELETE_THREAD_FUNCTION exitThread, LPVOID lParam, char * name)
{INTERFUN;
	return TCreateThread(control, NULL, 0, pfnThreadProc, exitThread, lParam, 0, NULL, name);
}
	


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
LPSINGLE ThreadTreeControl::TCreateMutex(BOOL initOwner, const CTCHAR * name)
{INTERFUN;
	if(NULL == _g_giGlobalInfo->_g_treThreadTree)
	{
		return NULL;
	}

	//查询当前线程所在节点
	ThreadInfo * threadInfo = _FindRunningThread(::GetCurrentThreadId());

	if(NULL == threadInfo)
	{
		return NULL;
	}
	else
	{
		LPSINGLE single = _TCreateMutex(threadInfo, initOwner, name);

		if(_g_bLogSingleCreate)
		{
			//需要对SINGLE创建进行输出则输出
			USES_CONVERSION;
			CStringA mes;
			mes.Format("created mutex name: \"%s\" -- address: 0x%x "
				,W2A(name), single);

			ThreadLog::AddLog(threadInfo, mes);
		}

		return single;
	}
}
	

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
LPSINGLE ThreadTreeControl::TCreateEvent(BOOL autoReSet, BOOL initState, const CTCHAR * name)
{INTERFUN;
	if(NULL == _g_giGlobalInfo->_g_treThreadTree)
	{
		return NULL;
	}

	//获取当前所在线程HANDLE，在内存中查询相关线程节点
	LPSINGLE single = _TCreateEvent(autoReSet, initState, name);

	if(_g_bLogSingleCreate)
	{
		//需要对SINGLE创建进行输出则输出
		//查询当前线程所在节点
		ThreadInfo * threadInfo = _FindRunningThread(::GetCurrentThreadId());

		USES_CONVERSION;
		CStringA mes;
		mes.Format("created event name: \"%s\" -- address: 0x%x "
			,W2A(name), single);

		ThreadLog::AddLog(threadInfo, mes);
	}

	return single;
}
	

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
UINT ThreadTreeControl::TSetEventSingle(SINGLE * single)
{INTERFUN;
	if(NULL == single)
	{
#ifdef _DEBUG
		DebugBreak();
#endif
		return SET_EVENT_ERROR;
	}

	if(SINGLE_EVENT != single->GetType())
	{
		//检查SINGLE类型是不是锁
#ifdef _DEBUG
		DebugBreak();
#endif

		return SET_EVENT_ERROR;
	}

	//获取SINGLE中的锁节点
	ThreadEvent * cEvent = (ThreadEvent *)single->GetSingle();

	if(NULL == cEvent)
	{
#ifdef _DEBUG
		DebugBreak();
#endif

		return SET_EVENT_ERROR;
	}

	if(_g_bLogSetSingle && single->GetLogEnable())
	{
		//需要对SINGLE创建进行输出则输出
		//查询当前线程所在节点
		ThreadInfo * threadInfo = _FindRunningThread(::GetCurrentThreadId());

		if(threadInfo->_m_bAddLogEnable)
		{
			USES_CONVERSION;
			CStringA mes;
			mes.Format("set event single event name: \"%s\" -- address: 0x%x "
				,W2A(cEvent->_m_csEventName), single);

			ThreadLog::AddLog(threadInfo, mes);
		}
	}

	//尝试将信号量的信号加1
	if(cEvent->_ReleaseEvent())
	{
		//操作成功

		//获取有限时间等待线程队列操作锁
		ThreadTreeControl::_LockThreadControlMutex();

		//判断信号量是否为有信号状态
		while(cEvent->_GetEventSingleEnable())
		{
			//有信号状态时不断获取下一个等待线程节点，当不存在下一个等待线程节点时返回


			//获取下一个等待信号的线程节点
			ThreadInfo * nextThread = (ThreadInfo *)cEvent->_GetFirstWaitingThread();

			if(NULL == nextThread)
			{
				//释放有限时间等待线程队列操作锁
				ThreadTreeControl::_UnLockThreadControlMutex();
				return SET_EVENT_SUC;
			}
			else
			{
				//激活下一个等待线程的执行，但是在激活

				//使用下一线程对锁进行加锁（顺带设置了锁的拥有线程）
				if(cEvent->_ThreadTryGetEvent(nextThread))
				{
					//加锁成功
					cEvent->_DelWaitingThread(nextThread);

					//设置线程中该锁的状态为已获取
					nextThread->_SetSingleState(single, WAIT_STATE_SUC);

					//激活线程执行
					nextThread->_TResumeThread();
				}
			}
		}

		//释放有限时间等待线程队列操作锁
		ThreadTreeControl::_UnLockThreadControlMutex();
		return SET_EVENT_SUC;
	}
	else
	{
		return SET_EVENT_FAIL;
	}
}
	

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
UINT ThreadTreeControl::TSetEventNoSingle(SINGLE * single)
{INTERFUN;
	if(NULL == single)
	{
#ifdef _DEBUG
		DebugBreak();
#endif

		return SET_EVENT_ERROR;
	}

	if(SINGLE_EVENT != single->GetType())
	{
		//SINGLE类型不是EVENT时
#ifdef _DEBUG
		DebugBreak();
#endif

		return SET_EVENT_ERROR;
	}

	ThreadEvent * cEvent = (ThreadEvent *)single->GetSingle();

	if(NULL == cEvent)
	{
#ifdef _DEBUG
		DebugBreak();
#endif

		return SET_EVENT_ERROR;
	}

	if(_g_bLogSetSingle && single->GetLogEnable())
	{
		//需要对SINGLE创建进行输出则输出
		//查询当前线程所在节点
		ThreadInfo * threadInfo = _FindRunningThread(::GetCurrentThreadId());

		if(threadInfo->_m_bAddLogEnable)
		{
			USES_CONVERSION;
			CStringA mes;
			mes.Format("set event no single event name: \"%s\" -- address: 0x%x "
				,W2A(cEvent->_m_csEventName), single);

			ThreadLog::AddLog(threadInfo, mes);
		}
	}

	//由于在设置信号量有信号时，可能与其他的等待操作的判断条件有相互影响，所以需要获取到操作锁
	//获取有限时间等待线程队列操作锁
	ThreadTreeControl::_LockThreadControlMutex();

	cEvent->_m_iSourceCount = 0;

	//释放有限时间等待线程队列操作锁
	ThreadTreeControl::_UnLockThreadControlMutex();

	return SET_EVENT_SUC;
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [删除树节点，以及其子节点]
 * @n<b>函数名称</b>                     : DeleteThread
 * @n@param TreeItem_t * treeItem        : [参数描述]
 * @see                                    [参见函数]
 * @n<b>作者</b>                         :alzq
 * @n<b>创建时间</b>                     : 09-2-18 9:17:13
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-18  [描述]
 ******************************************************************************************************/
void ThreadTreeControl::DeleteThread(TreeItem_t * treeItem, BOOL delChild, DWORD delayTime)
{INTERFUN;
	if(NULL == treeItem)
	{
		return ;
	}

	if(delChild)
	{
		//删除子节点优先
		_DestroyThreadTreeChild(treeItem);
	}

	//删除自身
	_DisposeTreeItem(treeItem, delayTime);
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [删除指定线程以及其子线程]
 * @n<b>函数名称</b>          : DeleteThread
 * @n@param ThreadInfo * info : [参数描述]
 * @see                         [参见函数]
 * @n<b>作者</b>              :alzq
 * @n<b>创建时间</b>          : 09-2-13 14:26:40
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-13  [描述]
 ******************************************************************************************************/
void ThreadTreeControl::DeleteThread(ThreadInfo * info, BOOL delChild, DWORD delayTime)
{INTERFUN;
	if(NULL == info)
	{
		return ;
	}

	DeleteThread((TreeItem_t *)info->_m_lpTreeItem, delChild, delayTime);
}
	

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
void ThreadTreeControl::DeleteThread(BOOL delChild, DWORD delayTime)
{INTERFUN;
	ThreadInfo * threadInfo = _FindRunningThread(::GetCurrentThreadId());

	DeleteThread(threadInfo, delChild, delayTime);
}
	

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
void ThreadTreeControl::_DisposeThread(ThreadInfo * info)
{INTERFUN;
	_LockThreadControlMutex();

	if(NULL != info->_m_lpControl)
	{
		((ThreadControl *)info->_m_lpControl)->_m_tiThreadInfo = NULL;
		info->_m_lpControl = NULL;
	}

	if(info->_m_lpTreeItem)
	{
		ThreadLog::AddLog(info, "delete tree item");
		
		info->_ReleaseAllControlMutex();

		info->_ClearWaitingSingleList();
		
		//在有限时间等待线程队列中删除本线程
		ThreadTreeControl::_DelTimeLimitWaitingThread(info);

		//在线程存储队列中删除本线程
		ThreadTreeControl::_DelRunningThread(info->_m_ttThreadTarget.threadId);

		TreeItem_t * treeItem = (TreeItem_t *)info->_m_lpTreeItem;

		CUSTOMTHREADINFO * customThreadInfo = (CUSTOMTHREADINFO *)treeItem->data.customThreadInfo;
		delete customThreadInfo;
		treeItem->data.customThreadInfo = NULL;

		//删除节点
		info->_m_lpTreeItem = NULL;

		DestroyTree(treeItem, FALSE);
	
		_g_giGlobalInfo->_g_dtiDeleteThreadInfo->threadList.push_back(info);
		ThreadLog::AddLog(info, "add to del list");
	}

	_UnLockThreadControlMutex();
}
	

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
ThreadInfo * ThreadTreeControl::_FindRunningThread(DWORD threadId)
{INTERFUN;
	if(NULL == threadId)
	{
		return NULL;
	}

	_LockThreadControlMutex();

	RUNNINGTHREADLIST::iterator iter = _g_giGlobalInfo->_g_rtlThreadList.begin();
	while(iter != _g_giGlobalInfo->_g_rtlThreadList.end())
	{
		if((*iter)->_m_ttThreadTarget.threadId == threadId)
		{
			_UnLockThreadControlMutex();
			return (*iter);
		}

		iter++;
	}
	
	_UnLockThreadControlMutex();

	return NULL;
}
	

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
// ThreadInfo * ThreadTreeControl::_FindRunningThread(CWinThread * winThread)
// {
// 	if(NULL == winThread)
// 	{
// 		return NULL;
// 	}
// 	
// 	_LockThreadControlMutex();
// 
// 	RUNNINGTHREADLIST::iterator iter = _g_giGlobalInfo->_g_rtlThreadList.begin();
// 	while(iter != _g_giGlobalInfo->_g_rtlThreadList.end())
// 	{
// 		if((*iter)->_m_ttThreadTarget.isWinThread && (*iter)->_m_ttThreadTarget.winThread == winThread)
// 		{
// 			_UnLockThreadControlMutex();
// 			return (*iter);
// 		}
// 
// 		iter++;
// 	}
// 	
// 	_UnLockThreadControlMutex();
// 
// 	return NULL;
// }
	

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
void ThreadTreeControl::_DelRunningThread(DWORD threadId)
{INTERFUN;
	if(NULL == threadId)
	{
		return ;
	}

	_LockThreadControlMutex();

	RUNNINGTHREADLIST::iterator iter = _g_giGlobalInfo->_g_rtlThreadList.begin();
	while(iter != _g_giGlobalInfo->_g_rtlThreadList.end())
	{
		if((*iter)->_m_ttThreadTarget.threadId == threadId)
		{
			_g_giGlobalInfo->_g_rtlThreadList.erase(iter);
			
			_UnLockThreadControlMutex();

			return ;
		}

		iter++;
	}
	
	_UnLockThreadControlMutex();

	return ;
}
	

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
void ThreadTreeControl::_TSuspendThread(ThreadInfo * info)
{INTERFUN;
	//if(info->_m_ttThreadTarget.isMainThread)
	//{
	//	ResetEvent(info->_m_ttThreadTarget.mainThreadEvent);
	//	WaitForSingleObject(info->_m_ttThreadTarget.mainThreadEvent, INFINITE);
	//}
	//else
	//{
	//	//当为CWINTHREAD或非CWINTHREAD时
	//	DWORD word = SuspendThread(info->_m_ttThreadTarget.handle);
	//}

	ResetEvent(info->_m_ttThreadTarget.controlEvent);

	//在此处释放控制锁，防止RESET的之前刚好进行SET导致死锁
	ThreadTreeControl::_UnLockThreadControlMutex();

	WaitForSingleObject(info->_m_ttThreadTarget.controlEvent, INFINITE);
}
	

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
void ThreadTreeControl::_TResumeThread(ThreadInfo * info)
{INTERFUN;
	//if(info->_m_ttThreadTarget.isMainThread)
	//{
	//	SetEvent(info->_m_ttThreadTarget.mainThreadEvent);
	//}
	//else
	//{
	//	//当为CWINTHREAD或非CWINTHREAD时
	//	ResumeThread(info->_m_ttThreadTarget.handle);
	//}

	SetEvent(info->_m_ttThreadTarget.controlEvent);
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [树节点的删除函数]
 * @n<b>函数名称</b>                 : _DisposeTreeItem
 * @n@param TreeItem_t * treeItem    : [节点地址]
 * @see                                [参见函数]
 * @n<b>作者</b>                     :alzq
 * @n<b>创建时间</b>                 : 09-2-18 9:13:02
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-18  [描述]
 ******************************************************************************************************/
void ThreadTreeControl::_DisposeTreeItem(TreeItem_t * treeItem, DWORD delayTime)
{INTERFUN;
	try
	{
		//删除本节点内存储对象的操作
		CUSTOMTHREADINFO * threadInfo = (CUSTOMTHREADINFO *)treeItem->data.customThreadInfo;
		ThreadInfo * info = threadInfo->threadInfo;

		ThreadLog::AddLog(info, "start dispose tree item");

		if(NULL != threadInfo)
		{
			//关闭线程，并将线程地址添加到删除线程队列中
			if(info->_m_ttThreadTarget.isWinThread)
			{
				//winthread
				BOOL res = FALSE;	//表示是否跳过强制终止

				//调用用户自身的终止函数
				if(NULL != info->_m_dtfDeleteThreadFunc)
				{
					//执行用户的终止函数
					DELETE_THREAD_FUNCTION deleteFunc = threadInfo->threadInfo->_m_dtfDeleteThreadFunc;
					res = deleteFunc(info->_GetUserInfo());

					DWORD time = delayTime;
					if(0 == time)
					{
						//当等待退出时间为0时替换为无限等待
						time = INFINITE;
					}

					if(WaitForSingleObject(info->_m_hFinalMutex, time) != WAIT_TIMEOUT)
					{
						ReleaseMutex(info->_m_hFinalMutex);

						//正常退出已经进行了之后相关的链表删除操作，所以此处直接返回
						return ;
					}
				}
				else
				{
					if(delayTime != 0)
					{
						//有延迟等待退出的，等待结束信号后判断是否需要强制终止
						if(WaitForSingleObject(info->_m_hFinalMutex, delayTime) != WAIT_TIMEOUT)
						{
							ReleaseMutex(info->_m_hFinalMutex);

							//正常退出已经进行了之后相关的链表删除操作，所以此处直接返回
							return ;
						}
					}
				}

// 				if(!res)
// 				{
// 					//TerminateThread(info->_m_ttThreadTarget.handle, 0);
// 					TUIThread * uiThread = (TUIThread *)info->_m_ttThreadTarget.winThread;
// 					if(PostMessage(uiThread->_m_wnd->m_hWnd, WM_QUIT, NULL, NULL))
// 					{
// 						//成功发送消息，等待完全退出
// 						WaitForSingleObject(info->_m_hFinalMutex, INFINITE);
// 						ReleaseMutex(info->_m_hFinalMutex);
// 					}
// 
// 					return ;
// 				}
			}
			else
			{
				DWORD exitCode;			//线程状态码
				GetExitCodeThread(info->_m_ttThreadTarget.handle, &exitCode);	//获取线程状态码
				if(exitCode==STILL_ACTIVE)
				{
					BOOL res = FALSE;	//表示是否跳过强制终止
					if(NULL != info->_m_dtfDeleteThreadFunc)
					{
						//执行用户的终止函数
						DELETE_THREAD_FUNCTION deleteFunc = info->_m_dtfDeleteThreadFunc;
						res = deleteFunc(info->_GetUserInfo());

						DWORD time = delayTime;
						if(0 == time)
						{
							//当等待退出时间为0时替换为无限等待
							time = INFINITE;
						}

						if(res && WaitForSingleObject(info->_m_hFinalMutex, time) != WAIT_TIMEOUT)
						{
							ReleaseMutex(info->_m_hFinalMutex);
		
							//正常退出已经进行了之后相关的链表删除操作，所以此处直接返回
							return ;
						}
					}
					else
					{
						if(delayTime != 0)
						{
							//有延迟等待退出的，等待结束信号后判断是否需要强制终止
							if(WaitForSingleObject(info->_m_hFinalMutex, delayTime) != WAIT_TIMEOUT)
							{
								ReleaseMutex(info->_m_hFinalMutex);

								//正常退出已经进行了之后相关的链表删除操作，所以此处直接返回
								return ;
							}
						}
					}
					
					if(!res)
					{
						//CloseHandle(threadInfo->threadInfo->_m_ttThreadTarget.handle);
						if(0 != TerminateThread(info->_m_ttThreadTarget.handle, 0))
						{
							//成功关闭线程，等待完全退出
							WaitForSingleObject(info->_m_hFinalMutex, INFINITE);
							ReleaseMutex(info->_m_hFinalMutex);
						}
						else
						{
						
							exit(0);
						}
					}
				}
			}

			_DisposeThread(info);
		}
		else
		{
			//删除节点
#ifdef _DEBUG
			DebugBreak();
#endif
			DestroyTree(treeItem, FALSE);
		}
	}
	catch(...)
	{
		//捕获到错误，返回
		TRACE("thread error, -- maybe dispose something has been disposed\n");
		return ;
	}
}
	

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
void ThreadTreeControl::_DestroyThreadTreeChild(TreeItem_t * treeItem)
{INTERFUN;
	if(NULL == treeItem)
	{
		return ;
	}

	if(treeItem->childCount > 0)
	{
		_DestroyThreadTreeItems(treeItem->firstchild);
	}
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [删除某一树节点及其子节点]
 * @n<b>函数名称</b>              : _DestroyThreadTreeItems
 * @n@param TreeItem_t * treeItem : [参数描述]
 * @see                             [参见函数]
 * @n<b>作者</b>                  :alzq
 * @n<b>创建时间</b>              : 09-2-13 13:55:00
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-13  [描述]
 ******************************************************************************************************/
void ThreadTreeControl::_DestroyThreadTreeItems(TreeItem_t * treeItem)
{INTERFUN;
	if(NULL == treeItem)
	{
		return ;
	}

	//优先删除后一节点，优先删除子节点(子节点比后一节点优先)
	_DestroyThreadTreeItems(treeItem->nextsibling);
	_DestroyThreadTreeItems(treeItem->firstchild);

	_DisposeTreeItem(treeItem);
}
	

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
void ThreadTreeControl::_TestAndDeleteThread(void)
{INTERFUN;
	if(NULL == _g_giGlobalInfo->_g_dtiDeleteThreadInfo)
	{

		return ;
	}

	_LockThreadControlMutex();

	THREADLIST::iterator iter = _g_giGlobalInfo->_g_dtiDeleteThreadInfo->threadList.begin();
	while(iter != _g_giGlobalInfo->_g_dtiDeleteThreadInfo->threadList.end())
	{
		ThreadInfo * thread = (ThreadInfo *)(*iter);

		//测试线程的结束锁
		if(thread->_m_ttThreadTarget.isMainThread)
		{
			//当节点为主线程，即根节点时直接删除
			try
			{
				SAFE_DELETE(thread);
			}
			catch(...)
			{
				TRACE("delete main thread tree item error\n");
			}

			iter = _g_giGlobalInfo->_g_dtiDeleteThreadInfo->threadList.erase(iter);
		}
		else if(WaitForSingleObject(thread->_m_hFinalMutex, 0) != WAIT_TIMEOUT)
		{
			//当不是超时时表明该线程已经退出
			ThreadLog::AddLog(thread, "delete thread message");

			try
			{
				SAFE_DELETE(thread);
			}
			catch(...)
			{
				TRACE("delete thread error\n");
			}

			iter = _g_giGlobalInfo->_g_dtiDeleteThreadInfo->threadList.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	_UnLockThreadControlMutex();
}
	

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
void ThreadTreeControl::_SetThreadStart(ThreadInfo * info)
{INTERFUN;
	if(NULL == info)
	{
		return ;
	}

	WaitForSingleObject(info->_m_hFinalMutex, INFINITE);
}
	

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
void ThreadTreeControl::_SetThreadFinish(ThreadInfo * info)
{INTERFUN;
	if(NULL == info)
	{
		return ;
	}

	ReleaseMutex(info->_m_hFinalMutex);
}
	

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
SINGLE * ThreadTreeControl::_TCreateMutex(ThreadInfo * threadInfo, BOOL initOwner, const CTCHAR * name)
{INTERFUN;
	SINGLE * single = _FindMutexByName(name);
	
	if(NULL != single)
	{
		//当查询到同名的SINGLE
		return single;
	}
	
	_LockSingleControlMutex();

	ThreadMutex * cMutex = new ThreadMutex(threadInfo, initOwner, name);
	single = new SINGLE(SINGLE_MUTEX, cMutex, 0);

	if(NULL != name)
	{
		//名称不为NULL时，添加进有名称的锁队列
		_g_giGlobalInfo->_g_mlNamedMutexList.push_back(single);
	}
	
	//将SINGLE添加到SINGLE队列中
	_g_giGlobalInfo->_g_slSingleList.push_back(single);
	
	_UnLockSingleControlMutex();

	return single;
}
	

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
void ThreadTreeControl::_DealThreadTimeCount(void)
{INTERFUN;
	_LockThreadControlMutex();

	WAITINGTHREADLIST::iterator iter = _g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.begin();
	while(iter != _g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.end())
	{
		ThreadInfo * threadInfo = (*iter);

		if(NULL != threadInfo && threadInfo->_m_iWaitTimeCount < 10)
		{
			//当等待超时时，设置状态后激活线程
			threadInfo->_m_uiThreadState = THREAD_STATE_TIMEOUT;

			threadInfo->_TResumeThread();

//			_DelTimeLimitWaitingThread(threadInfo);
			iter = _g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.erase(iter);
		}
		else if(NULL == threadInfo)
		{
#ifdef _DEBUG
			DebugBreak();
#endif
			iter = _g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.erase(iter);
		}
		else
		{
			threadInfo->_m_iWaitTimeCount -= 10;
			iter++;
		}
	}

	_UnLockThreadControlMutex();
}
	

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
SINGLE * ThreadTreeControl::_TCreateEvent(BOOL autoReSet, BOOL initState, const CTCHAR * name)
{INTERFUN;
	SINGLE * single = _FindMutexByName(name);
	
	if(NULL != single)
	{
		//当查询到同名的SINGLE
		return single;
	}
	
	_LockSingleControlMutex();

	ThreadEvent * cEvent = new ThreadEvent(autoReSet, initState, name);
	single = new SINGLE(SINGLE_EVENT, cEvent, 0);

	if(NULL != name)
	{
		//名称不为NULL时，添加进有名称的信号队列
		_g_giGlobalInfo->_g_elNamedEventList.push_back(single);
	}
	
	//将SINGLE添加到SINGLE队列中
	_g_giGlobalInfo->_g_slSingleList.push_back(single);
	
	_UnLockSingleControlMutex();

	return single;
}
	

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
void ThreadTreeControl::_LockThreadControlMutex(void)
{INTERFUN;
	//EnterCriticalSection(&_g_giGlobalInfo->_g_csThreadControlSection);
	WaitForSingleObject(_g_giGlobalInfo->_g_csThreadControlMutex, INFINITE);
}
	

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
void ThreadTreeControl::_UnLockThreadControlMutex(void)
{INTERFUN;
	//LeaveCriticalSection(&_g_giGlobalInfo->_g_csThreadControlSection);
	ReleaseMutex(_g_giGlobalInfo->_g_csThreadControlMutex);
}
	

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
void ThreadTreeControl::_LockSingleControlMutex(void)
{INTERFUN;
	//EnterCriticalSection(&_g_giGlobalInfo->_g_csSingleControlSection);
	WaitForSingleObject(_g_giGlobalInfo->_g_csSingleControlMutex, INFINITE);
}
	

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
void ThreadTreeControl::_UnLockSingleControlMutex(void)
{INTERFUN;
	//return ReleaseMutex(_g_hSingleControlMutex);
	//LeaveCriticalSection(&_g_giGlobalInfo->_g_csSingleControlSection);
	ReleaseMutex(_g_giGlobalInfo->_g_csSingleControlMutex);
}
	

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
void ThreadTreeControl::_AddWaitingThread(ThreadInfo * lpThread, DWORD millSec)
{INTERFUN;
	if(NULL == lpThread)
	{
		return ;
	}

	if(0 == millSec || INFINITE == millSec)
	{
		//当为非等待模式或无限等待模式下直接返回
		return ;
	}

	_LockThreadControlMutex();

	WAITINGTHREADLIST::iterator iter = _g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.begin();
	while(iter != _g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.end())
	{
		if((*iter)->_m_iWaitTimeCount > millSec)
		{
			//当当前节点的等待时间大于需要加入的线程的等待时间时，将线程加入到当前节点前
			_g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.insert(iter, lpThread);

			_UnLockThreadControlMutex();

			return ;
		}

		iter++;
	}

	//遍历完后添加到队列末尾
	_g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.push_back(lpThread);

	_UnLockThreadControlMutex();
}
	

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
void ThreadTreeControl::_DelTimeLimitWaitingThread(ThreadInfo * lpThread)
{INTERFUN;
	if(NULL == lpThread)
	{
		return ;
	}

	_LockThreadControlMutex();

	WAITINGTHREADLIST::iterator iter = _g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.begin();
	while(iter != _g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.end())
	{
		if((*iter) == lpThread)
		{
			_g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.erase(iter);
			
			_UnLockThreadControlMutex();

			return ;
		}

		iter++;
	}

	_UnLockThreadControlMutex();
}
	

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
SINGLE * ThreadTreeControl::_FindMutexByName(const CTCHAR * name)
{INTERFUN;
	if(NULL == name)
	{
		return NULL;
	}

	_LockSingleControlMutex();

	//遍历查询
	MutexList::iterator iter = _g_giGlobalInfo->_g_mlNamedMutexList.begin();
	while(iter != _g_giGlobalInfo->_g_mlNamedMutexList.end())
	{
		ThreadMutex * cMutex = (ThreadMutex *)(*iter)->GetSingle();
		if(_tcscmp(cMutex->_m_csMutexName, name) == 0)
		{
			//当名称相等时
			_UnLockSingleControlMutex();

			return (*iter);
		}

		iter++;
	}
	
	_UnLockSingleControlMutex();

	//查询不到同名锁则返回NULL
	return NULL;
}
	

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
SINGLE * ThreadTreeControl::_FindEventByName(const CTCHAR * name)
{INTERFUN;
	if(NULL == name)
	{
		return NULL;
	}
	
	_LockSingleControlMutex();

	//遍历查询
	EventList::iterator iter = _g_giGlobalInfo->_g_elNamedEventList.begin();
	while(iter != _g_giGlobalInfo->_g_elNamedEventList.end())
	{
		ThreadEvent * cMutex = (ThreadEvent *)(*iter)->GetSingle();
		if(_tcscmp(cMutex->_m_csEventName, name) == 0)
		{
			//当名称相等时
			_UnLockSingleControlMutex();

			return (*iter);
		}

		iter++;
	}
	
	_UnLockSingleControlMutex();

	//查询不到同名锁则返回NULL
	return NULL;
}

void ThreadTreeControl::_OutputThread(TreeItem_t * treeItem, fstream * stream, LPSYSTEMTIME systemTime)
{INTERFUN;
	if(NULL == treeItem || NULL == stream)
	{
		return ;
	}

	//子节点
	_OutputThread(treeItem->firstchild, stream, systemTime);

	//下一节点
	_OutputThread(treeItem->nextsibling, stream, systemTime);

	//输出自身
	if(NULL == treeItem->data.customThreadInfo)
	{
		return ;
	}

	ThreadInfo * info = ((CUSTOMTHREADINFO *)treeItem->data.customThreadInfo)->threadInfo;

	if(NULL == info)
	{
		return ;
	}

	CStringA str;

	if(info->_m_ttThreadTarget.isMainThread)
	{
		str.Format("%i-%i-%i %i:%i:%i -- Alive mainthread --\n"
			, systemTime->wYear, systemTime->wMonth, systemTime->wDay, systemTime->wHour, systemTime->wMinute
			, systemTime->wSecond);
	}
	else
	{
		str.Format("%i-%i-%i %i:%i:%i -- Alive Thread name: \"%s\" id  : %i -- address: 0x%x\n"
			, systemTime->wYear, systemTime->wMonth, systemTime->wDay, systemTime->wHour, systemTime->wMinute
			, systemTime->wSecond, info->_m_csThreadName, info->_m_ttThreadTarget.threadId, info);
	}

	stream->write(str, str.GetLength());
	stream->write("\n", 1);
}

//输出仍然在活动的内存中线程信息
void ThreadTreeControl::_OutPutAliveThread(void)
{INTERFUN;
	if(NULL == _g_giGlobalInfo->_g_sAliveThreadFile)
	{
		return ;
	}

	try
	{
		fstream file;
		file.open(_g_giGlobalInfo->_g_sAliveThreadFile, ios::out | ios::trunc);

		//获取当前时间
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);
		
		_LockThreadControlMutex();
		_OutputThread(_g_giGlobalInfo->_g_treThreadTree, &file, &systemTime);
		_UnLockThreadControlMutex();

		file.flush();
		file.close();

		return ;
	}
	catch(...)
	{
		return ;
	}
}
	
