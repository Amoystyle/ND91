#include "StdAfx.h"
#include "ThreadInfo.h"
#include "ThreadControl.h"
#include "ThreadTreeControl.h"
#include "ThreadLog.h"
#pragma warning( disable: 4996 )
ThreadInfo::ThreadInfo(LPVOID userData, DELETE_THREAD_FUNCTION exitThreadFunc, LPVOID control, char * name)
{INTERFUN;
	_m_csDealLock = new DealSectionControl();

	_m_uiThreadState = THREAD_STATE_SUC;		//线程状态标志变量，存储当前线程状态

	_m_slGetMutexList.clear();					//本线程获取控制权的锁节点标志队列

	_m_swlWaitSingleList.clear();				//本线程等待的锁或信号量与其获取状态队列

	_m_bResetWaitTimeEnable = FALSE;			//表示本线程在等待多个锁或信号量时，是否在获取到其中的一个时重新设置等待时间
	_m_iWaitTimeInit = 0;						//本线程此次等待的初始时间
	_m_iWaitTimeCount = 0;						//本线程此次等待的时间统计

	_m_uiWaitState = WAIT_INFINTE;				//表示本线程此次等待状态（无限等待还是有限等待）

	_m_hFinalMutex = CreateMutex(NULL, FALSE, NULL);						//表示线程执行完毕的信号量（使用自身的信号量存储树结构进行控制）

	//THREADTARGET		_m_ttThreadTarget;		//存储线程相关信息

	_m_lpUserData = userData;					//用户带入线程的指针

	_m_iChildCount = 0;							//本线程节点下的子线程节点统计

	_m_dtfDeleteThreadFunc = exitThreadFunc;
	
	if(control)
	{
		_m_bAddLogEnable = ((LPTHREADCONTROL)control)->_m_bThreadLogEnable;					//是否输出与本线程有关的线程树LOG记录，默认为false
	}
	else
	{
		_m_bAddLogEnable = false;
	}

	//保存名称
	_m_csThreadName = NULL;
	if(NULL != name)
	{
		size_t len = strlen(name);
		_m_csThreadName = new char[len + 1];
		strcpy(_m_csThreadName, name);
	}

//private:
	_m_lpTreeItem = NULL;

	_m_lpControl = control;
}

ThreadInfo::~ThreadInfo(void)
{INTERFUN;
	_m_csDealLock->Lock();
	//不需要检测是否存在子节点，此操作在树型结构的删除内操作

//private:

//protected:
	//ThreadTreeControl::_LockThreadControlMutex();
	//_m_uiThreadState = THREAD_STATE_SUC;		//线程状态标志变量，存储当前线程状态
	_m_uiThreadState = THREAD_STATE_CLOSE;


	//释放所有已经获取的锁的控制权
	//_m_slGetMutexList.clear();					//本线程获取控制权的锁节点标志队列
	//this->_ReleaseAllControlMutex();
	_m_slGetMutexList.clear();


	//删除所有正在等待的SINGLE队列中的节点,在做此操作时需要获取线程操作锁
	//_m_swlWaitSingleList.clear();				//本线程等待的锁或信号量与其获取状态队列
	//this->_ClearWaitingSingleList();
	//	
	////在有限时间等待线程队列中删除本线程
	//ThreadTreeControl::_DelTimeLimitWaitingThread(this);

	_m_swlWaitSingleList.clear();

	//_m_bResetWaitTimeEnable = FALSE;			//表示本线程在等待多个锁或信号量时，是否在获取到其中的一个时重新设置等待时间
	//_m_iWaitTimeInit = 0;						//本线程此次等待的初始时间
	//_m_iWaitTimeCount = 0;					//本线程此次等待的时间统计

	//_m_uiWaitState = WAIT_INFINTE;			//表示本线程此次等待状态（无限等待还是有限等待）

	if(NULL != _m_hFinalMutex)
	{
		CloseHandle(_m_hFinalMutex);			//表示线程执行完毕的信号量（使用自身的信号量存储树结构进行控制）
		_m_hFinalMutex = NULL;
	}

	//THREADTARGET		_m_ttThreadTarget;		//存储线程相关信息

	_m_lpUserData = NULL;						//用户带入线程的指针

	_m_iChildCount = 0;							//本线程节点下的子线程节点统计

	if(NULL != _m_csThreadName)
	{
		delete[] _m_csThreadName;
		_m_csThreadName = NULL;
	}
	
	_m_csDealLock->UnLock();

	SAFE_DELETE(_m_csDealLock);

	//ThreadTreeControl::_UnLockThreadControlMutex();
}

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [获取用户带入数据]
 * @n<b>函数名称</b>: GetUserInfo
 * @return            [返回值描述]
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-2-13 14:47:18
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-13  [描述]
 ******************************************************************************************************/
LPVOID ThreadInfo::_GetUserInfo(void)
{INTERFUN;
	return _m_lpUserData;
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [等待某个信号量或锁]
 * @n<b>函数名称</b>         : WaitForSingle
 * @n@param SINGLE * single  : [信号量或锁的存储地址]
 * @param   DWORD    millSec : [等待时间， INFINITE 时为无限等待]
 * @return                     [返回等待的结果]
 * @see                        [参见函数]
 * @n<b>作者</b>             :alzq
 * @n<b>创建时间</b>         : 09-2-6 14:12:29
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
UINT ThreadInfo::_TWaitForSingle(SINGLE * single, DWORD millSec)
{INTERFUN;
	if(!_CheckThread())
	{
		//当前线程并非本节点指向线程
		return WAIT_SINGLE_ERROR;
	}

	if(NULL == single)
	{
#ifdef _DEBUG
		DebugBreak();
#endif

		return WAIT_SINGLE_ERROR;
	}

	UINT res = WAIT_SINGLE_ERROR;

	//判断信号类别，后根据具体类别进行处理
	if(single->GetType() == SINGLE_EVENT)
	{
		//为信号量的处理方式
		res = this->_WaitForEvent(single, millSec);
	}
	else if(single->GetType() == SINGLE_MUTEX)
	{
		//为锁时的处理方式
		res = this->_WaitForMutex(single, millSec);
	}
	else
	{
#ifdef _DEBUG
		DebugBreak();
#endif

		res = WAIT_SINGLE_ERROR;
	}

	return res;
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [等待多个锁或信号量的集合]
 * @n<b>函数名称</b>                  : WaitForSingleList
 * @n@param SINGLELIST singleList     : [锁或信号量的节点队列]
 * @param   DWORD      millSec        : [等待时间， INFINITE 时为无限等待]
 * @param   BOOL       resetTimeCount : [在等待一个成功时，是否重新设置等待时间的指示变量]
 * @return                              [返回等待的结果]
 * @see                                 [参见函数]
 * @n<b>作者</b>                      :alzq
 * @n<b>创建时间</b>                  : 09-2-6 14:22:07
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
UINT ThreadInfo::_TWaitForSingleList(SINGLELIST singleList, DWORD millSec, BOOL resetTimeCount)
{INTERFUN;
	/**
	 *	---此函数主要流程如下：
	 *	将等待的信号或锁逐个进行检测，对信号或锁，需要等待的与可直接获取的区别处理
	 *	如果存在需要等待的锁或信号量则挂起线程等待激活
	 **/
	if(!_CheckThread())
	{
		//当前线程并非本节点指向线程
		return WAIT_SINGLE_ERROR;
	}

	if(singleList.empty())
	{
#ifdef _DEBUG
		DebugBreak();
#endif

		return WAIT_SINGLE_ERROR;
	}

	if(INFINITE == millSec)
	{
		_m_uiWaitState = WAIT_INFINTE;
	}
	else
	{
		_m_uiWaitState = WAIT_TIME_LIMIT;
	}

	//清空本线程节点内相关变量与队列
	this->_InitWaitingSituation();

	//设置此次等待初始时间
	_m_iWaitTimeInit = millSec;

	//设置在获取到一个锁后是否重置等待时间
	_m_bResetWaitTimeEnable = resetTimeCount;

	//获取有限时间等待线程队列控制锁
	ThreadTreeControl::_LockThreadControlMutex();

	SINGLELIST::iterator iter = singleList.begin();
	while(iter != singleList.end())
	{
		//逐个获取SINGLE
		SINGLE * single = (*iter);

		if(NULL == single)
		{
			ThreadTreeControl::_UnLockThreadControlMutex();
			return WAIT_SINGLE_ERROR;
		}

		//在等待队列中添加缩
		WAITSTATE * waitState = this->_AddWaitingSingle(single, millSec);

		//根据不同类型获取不同的等待对象
		if(single->GetType() == SINGLE_MUTEX)
		{
			ThreadMutex * cMutex = (ThreadMutex *)single->GetSingle();

			if(cMutex->_ThreadTryLockMutex(this))
			{
				//尝试加锁成功
				
				//将本线程节点获取到的锁节点队列中添加改SINGLE
				this->_AddControlMutex(single);

				//将等待信号量或锁的队列中该节点获取状态设置为已获取
				waitState->state = WAIT_STATE_SUC;
			}
			else
			{
				//尝试加锁失败时：锁状态为不可加锁状态，本线程未拥有控制权

				//判断等待时间是否为0
				if(0 == millSec)
				{
					//将已经获取到的锁或信号量释放，然后返回TIME_OUT

					//释放已经获取到的锁或信号量
					this->_ReleaseAllWaitingSingle();

					//返回超时
					ThreadTreeControl::_UnLockThreadControlMutex();
					return WAIT_SINGLE_TIMEOUT;
				}
				else
				{
					//在锁节点的等待线程队列中添加该线程节点
					cMutex->_AddWaitingThread(this);

					_m_uiThreadState = THREAD_STATE_WAIT;

					//将线程与等待时间带入，添加到全局的等待队列中
					ThreadTreeControl::_AddWaitingThread(this, millSec);

					//设置等待节点状态
					waitState->state = WAIT_STATE_WAIT;
				}
			}
		}
		else if(single->GetType() == SINGLE_EVENT)
		{
			ThreadEvent * cEvent = (ThreadEvent *)single->GetSingle();

			if(cEvent->_ThreadTryGetEvent(this))
			{
				//尝试获取信号量成功

				//将等待信号量或锁的队列中该节点获取状态设置为已获取成功
				waitState->state = WAIT_STATE_SUC;
			}
			else
			{
				//尝试加锁失败时：锁状态为不可加锁状态，本线程未拥有控制权

				//判断等待时间是否为0
				if(0 == millSec)
				{
					//将已经获取到的锁或信号量释放，然后返回TIME_OUT

					//释放已经获取到的锁或信号量
					this->_ReleaseAllWaitingSingle();

					//返回超时
					ThreadTreeControl::_UnLockThreadControlMutex();
					return WAIT_SINGLE_TIMEOUT;
				}
				else
				{
					//在锁节点的等待线程队列中添加该线程节点
					cEvent->_AddWaitingThread(this);

					_m_uiThreadState = THREAD_STATE_WAIT;

					//将线程与等待时间带入，添加到全局的等待队列中
					ThreadTreeControl::_AddWaitingThread(this, millSec);

					//设置等待节点状态
					waitState->state = WAIT_STATE_WAIT;
				}
			}
		}

		iter++;
	}

	//循环加入等待节点后，需要根据线程状态判断是否需要挂起线程，不挂起线程则直接返回

	while(THREAD_STATE_WAIT == _m_uiThreadState)
	{
		//如果是需要等待的状态，在挂起函数内有释放锁函数
		this->_TSuspendThread();

		//获取有限时间等待线程队列控制锁
		ThreadTreeControl::_LockThreadControlMutex();

		//当此线程被激活时，代表其队列中有锁或信号量成功获取，或者超出等待时间
		SINGLE * single = this->_GetFirstWaitSingle();
		//当获取到的下一个等待SINGLE为空时，表示此次等待成功
		if(NULL == single)
		{
			//等待操作成功

			//删除有限时间等待线程队列中该线程节点
			ThreadTreeControl::_DelTimeLimitWaitingThread(this);

			//释放有限时间等待线程队列控制锁
			ThreadTreeControl::_UnLockThreadControlMutex();

			return WAIT_SINGLE_SUC;
		}
		else
		{
			if(THREAD_STATE_TIMEOUT == _m_uiThreadState)
			{
				//结果为超时时，表示等待结果为超时

				//释放已经获取到的锁或信号量
				this->_ReleaseAllWaitingSingle();

				//删除有限时间等待线程队列中该线程节点
				ThreadTreeControl::_DelTimeLimitWaitingThread(this);
			
				//释放有限时间等待线程队列控制锁
				ThreadTreeControl::_UnLockThreadControlMutex();

				return WAIT_SINGLE_TIMEOUT;
			}
			else
			{
				//结果不为超时时，继续等待，返回WHILE前继续挂起

				//判断是否需要重新设置等待时间
				if(_m_bResetWaitTimeEnable)
				{
					_m_iWaitTimeCount = _m_iWaitTimeInit;
				}

				_m_uiThreadState = THREAD_STATE_WAIT;
			}
		}
	}

	//当一次性获取到所有等待的锁与信号时则程序进行到这里

	//释放有限时间等待线程队列控制锁
	ThreadTreeControl::_UnLockThreadControlMutex();

	if(THREAD_STATE_SUC == _m_uiThreadState)
	{
		return WAIT_SINGLE_SUC;
	}
	else if(THREAD_STATE_TIMEOUT == _m_uiThreadState)
	{
		return WAIT_SINGLE_TIMEOUT;
	}
	else
	{
		return WAIT_SINGLE_ERROR;
	}
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [以本线程为父节点创建锁]
 * @n<b>函数名称</b>                : TCreateMutex
 * @n@param BOOL          initState : [初始状态]
 * @param   const WCHAR * name      : [锁名称，默认为空]
 * @return                            [返回值描述]
 * @see                               [参见函数]
 * @n<b>作者</b>                    :alzq
 * @n<b>创建时间</b>                : 09-2-6 14:24:52
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
SINGLE * ThreadInfo::_TCreateMutex(BOOL initOwner, const WCHAR * name)
{INTERFUN;
	if(_CheckThread())
	{
		return ThreadTreeControl::_TCreateMutex(this, initOwner, name);
	}
	else
	{
		return ThreadTreeControl::TCreateMutex(initOwner, name);
	}
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                : TCreateEvent
 * @n@param BOOL          autoReSet : [是否自动转化信号状态]
 * @param   BOOL          initState : [初始状态]
 * @param   const WCHAR * name      : [信号量名称，默认为空]
 * @return                            [返回值描述]
 * @see                               [参见函数]
 * @n<b>作者</b>                    :alzq
 * @n<b>创建时间</b>                : 09-2-6 14:26:27
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
SINGLE * ThreadInfo::_TCreateEvent(BOOL autoReSet, BOOL initState, const WCHAR * name)
{INTERFUN;
	return ThreadTreeControl::_TCreateEvent(autoReSet, initState, name);
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [释放锁]
 * @n<b>函数名称</b>        : TReleaseMutex
 * @n@param SINGLE * single : [锁的节点地址]
 * @return                    [返回值描述]
 * @see                       [参见函数]
 * @n<b>作者</b>            :alzq
 * @n<b>创建时间</b>        : 09-2-6 14:51:04
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
UINT ThreadInfo::_TReleaseMutex(SINGLE * single)
{INTERFUN;
	/*
	 *
	 *	-由于释放锁可能在结束线程时操作，而结束线程时，控制锁释放的可能并非执行线程，所以此步骤的线程审核取消
	 *
	 */
	//if(!_CheckThread())
	//{
	//	//当前线程并非本节点指向线程
	//	return RELEASE_MUTEX_ERROR;
	//}

	if(NULL == single)
	{
#ifdef _DEBUG
		DebugBreak();
#endif

		return RELEASE_MUTEX_ERROR;
	}

	if(SINGLE_MUTEX != single->GetType())
	{
		//检查SINGLE类型是不是锁
#ifdef _DEBUG
		DebugBreak();
#endif

		return RELEASE_MUTEX_ERROR;
	}

	//获取SINGLE中的锁节点
	ThreadMutex * cMutex = (ThreadMutex *)single->GetSingle();

	if(NULL == cMutex)
	{
#ifdef _DEBUG
		DebugBreak();
#endif

		return RELEASE_MUTEX_ERROR;
	}

	if(ThreadTreeControl::_g_bLogWaitObj && _m_bAddLogEnable && single->GetLogEnable())
	{
		//需要输出LOG记录
		USES_CONVERSION;
		CStringA mes;
		mes.Format("release mutex name: \"%s\" -- address: 0x%x "
			,W2A(cMutex->_m_csMutexName), single);

		ThreadLog::AddLog(this, mes);
	}

	//获取有限时间等待线程队列操作锁
	ThreadTreeControl::_LockThreadControlMutex();

	//尝试将锁的加锁次数减1
	if(cMutex->_ReduceLockTimeMutex(this))
	{
		//操作成功

		//判断锁是否能被获取，当能被获取时，需要激活下一个等待该锁的线程
		if(cMutex->_GetMutexLockEnable())
		{
			//锁能被获取

			//将锁节点从本线程获取到控制权的锁队列中删除
			this->_DelControlMutex(single);

			ThreadInfo * nextThread = (ThreadInfo *)cMutex->_GetFirstWaitingThread();

			if(NULL == nextThread)
			{
				//获取有限时间等待线程队列操作锁
				ThreadTreeControl::_UnLockThreadControlMutex();

				return RELEASE_MUTEX_SUC;
			}
			else
			{
				//激活下一个等待线程的执行，但是在激活

				//使用下一线程对锁进行加锁（顺带设置了锁的拥有线程）
				if(cMutex->_ThreadTryLockMutex(nextThread))
				{
					//加锁成功
					cMutex->_DelWaitingThread(nextThread);

					//设置线程中该锁的状态为已获取
					nextThread->_SetSingleState(single, WAIT_STATE_SUC);

					//激活线程执行
					nextThread->_TResumeThread();
				}
				else
				{
				}
			}
		}

		//获取有限时间等待线程队列操作锁
		ThreadTreeControl::_UnLockThreadControlMutex();

		Sleep(0);

		return RELEASE_MUTEX_SUC;
	}
	else
	{

		//获取有限时间等待线程队列操作锁
		ThreadTreeControl::_UnLockThreadControlMutex();

		Sleep(0);

		return RELEASE_MUTEX_FAIL;
	}
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [设置信号量为有信号状态]
 * @n<b>函数名称</b>        : TSetEventSingle
 * @n@param SINGLE * single : [信号量的节点地址]
 * @return                    [返回值描述]
 * @see                       [参见函数]
 * @n<b>作者</b>            :alzq
 * @n<b>创建时间</b>        : 09-2-6 14:57:03
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
UINT ThreadInfo::_TSetEventSingle(SINGLE * single)
{INTERFUN;
	return ThreadTreeControl::TSetEventSingle(single);
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [设置信号量为无信号状态]
 * @n<b>函数名称</b>        : TSetEventNoSingle
 * @n@param SINGLE * single : [信号量的节点地址]
 * @return                    [返回值描述]
 * @see                       [参见函数]
 * @n<b>作者</b>            :alzq
 * @n<b>创建时间</b>        : 09-2-6 14:57:06
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
UINT ThreadInfo::_TSetEventNoSingle(SINGLE * single)
{INTERFUN;
	return ThreadTreeControl::TSetEventNoSingle(single);
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [释放所有控制的锁]
 * @n<b>函数名称</b>: _ReleaseAllControlMutex
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-2-25 10:50:00
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-25  [描述]
 ******************************************************************************************************/
void ThreadInfo::_ReleaseAllControlMutex(void)
{INTERFUN;
	_m_csDealLock->Lock();

	SINGLELIST::iterator singleIter = _m_slGetMutexList.begin();
	while(singleIter != _m_slGetMutexList.end())
	{
		try
		{
			if(SINGLE_MUTEX == (*singleIter)->GetType())
			{
				ThreadMutex * cMutex = (ThreadMutex *)(*singleIter)->GetSingle();

				if(cMutex->_m_tlControlThread == this && cMutex->_m_iLockTime >= 1)
				{
					//设置加锁次数为1后再释放该锁
					cMutex->_m_iLockTime = 1;

					this->_TReleaseMutex((*singleIter));
				}
				else
				{
					_m_slGetMutexList.erase(singleIter);
				}
			}
			else
			{
				_m_slGetMutexList.erase(singleIter);
			}
		}
		catch(...)
		{
			_m_slGetMutexList.pop_front();
		}

		//处理完一个SINGLE则删除队列中该节点
		singleIter = _m_slGetMutexList.begin();
	}

	_m_csDealLock->UnLock();
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [检验当前所在线程是否与本节点所指线程相同]
 * @n<b>函数名称</b>: _CheckThread
 * @return            [返回值描述]
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-2-19 9:02:51
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-19  [描述]
 ******************************************************************************************************/
BOOL ThreadInfo::_CheckThread(void)
{INTERFUN;
	if(::GetCurrentThreadId() == this->_m_ttThreadTarget.threadId)
	{
		return TRUE;
	}
#ifdef _DEBUG
	DebugBreak();
#endif

	return FALSE;
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [根据本线程存储的相关参数挂起本线程]
 * @n<b>函数名称</b>: TSuspendThread
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-2-16 9:12:42
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-16  [描述]
 ******************************************************************************************************/
void ThreadInfo::_TSuspendThread(void)
{INTERFUN;
	ThreadTreeControl::_TSuspendThread(this);
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [根据本线程存储的相关参数恢复本线程]
 * @n<b>函数名称</b>: TResumeThread
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-2-16 9:12:42
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-16  [描述]
 ******************************************************************************************************/
void ThreadInfo::_TResumeThread(void)
{INTERFUN;
	ThreadTreeControl::_TResumeThread(this);
}
	


/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [清空相关的等待队列，初始化相关变量]
 * @n<b>函数名称</b>: _ClearWaitingSingleList
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-2-10 13:33:02
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-10  [描述]
 ******************************************************************************************************/
void ThreadInfo::_InitWaitingSituation(void)
{INTERFUN;
	_m_csDealLock->Lock();

	SINGLEWAITLIST::iterator iter = _m_swlWaitSingleList.begin();
	while(iter != _m_swlWaitSingleList.end())
	{
		SAFE_DELETE(*iter);

		iter++;
	}

	_m_swlWaitSingleList.clear();

	_m_uiWaitState = WAIT_SINGLE_SUC;
	_m_uiThreadState = WAIT_INFINTE;

	_m_csDealLock->UnLock();
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [释放所有等待中的SINGLE队列中已经获取到的锁或信号量]
 * @n<b>函数名称</b>: _ReleaseAllWaitingSingle
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-2-10 13:48:47
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-10  [描述]
 ******************************************************************************************************/
void ThreadInfo::_ReleaseAllWaitingSingle(void)
{INTERFUN;
	_m_csDealLock->Lock();

	SINGLEWAITLIST::iterator iter = _m_swlWaitSingleList.begin();
	while(iter != _m_swlWaitSingleList.end())
	{
		WAITSTATE * state = (*iter);

		//判断是否已经获取到
		if(WAIT_STATE_SUC == state->state)
		{
			SINGLE * single = state->singleTarget;

			if(single->GetType() == SINGLE_EVENT)
			{
				//信号量

				//将信号量恢复
				this->_TSetEventSingle(single);
			}
			else if(single->GetType() == SINGLE_MUTEX)
			{
				//锁

				//释放本线程获取的锁
				this->_TReleaseMutex(single);
			}
		}
		else
		{
			//未获取
			SINGLE * single = state->singleTarget;
			if(SINGLE_EVENT == single->GetType())
			{
				//信号量
				ThreadEvent * cEvent = (ThreadEvent *)single->GetSingle();
				cEvent->_DelWaitingThread(this);
			}
			else
			{
				//锁
				ThreadMutex * cMutex = (ThreadMutex *)single->GetSingle();
				cMutex->_DelWaitingThread(this);
			}
		}

		SAFE_DELETE(state);

		iter = _m_swlWaitSingleList.erase(iter);
	}

	_m_csDealLock->UnLock();
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [设置等待SINGLE的状态]
 * @n<b>函数名称</b>        : _SetSingleState
 * @n@param SINGLE * single : [等待的SINGLE地址]
 * @param   UINT     state  : [需要设置的状态]
 * @see                       [参见函数]
 * @n<b>作者</b>            :alzq
 * @n<b>创建时间</b>        : 09-2-10 17:40:49
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-10  [描述]
 ******************************************************************************************************/
void ThreadInfo::_SetSingleState(SINGLE * single, UINT state)
{INTERFUN;
	if(NULL == single)
	{
		return ;
	}

	_m_csDealLock->Lock();

	SINGLEWAITLIST::iterator iter = _m_swlWaitSingleList.begin();
	while(iter != _m_swlWaitSingleList.end())
	{
		if((*iter)->singleTarget == single)
		{
			(*iter)->state = state;
		}

		iter++;
	}

	_m_csDealLock->UnLock();
}
	


/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [根据等待时间设置等待状态为无限等待还是有限时间等待，并设置等待时间与初始等待时间]
 * @n<b>函数名称</b>         : _SetWaitStateBySec
 * @n@param int millSec      : [等待时间]
 * @see                        [参见函数]
 * @n<b>作者</b>             :alzq
 * @n<b>创建时间</b>         : 09-2-9 15:17:21
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-09  [描述]
 ******************************************************************************************************/
void ThreadInfo::_SetWaitStateBySec(DWORD millSec)
{INTERFUN;
	//设置此次等待初始时间
	_m_iWaitTimeInit = millSec;

	//设置此次等待时间
	_m_iWaitTimeCount = _m_iWaitTimeInit;

	if(INFINITE == millSec)
	{
		_m_uiWaitState = WAIT_INFINTE;
	}
	else
	{
		_m_uiWaitState = WAIT_TIME_LIMIT;
	}
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [设置线程执行状态]
 * @n<b>函数名称</b>   : _SetState
 * @n@param UINT state : [参数描述]
 * @see                  [参见函数]
 * @n<b>作者</b>       :alzq
 * @n<b>创建时间</b>   : 09-2-9 14:05:07
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-09  [描述]
 ******************************************************************************************************/
void ThreadInfo::_SetState(UINT state)
{INTERFUN;
	_m_uiThreadState = state;
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [等待信号量的处理函数]
 * @n<b>函数名称</b>         : _WaitForEvent
 * @n@param SINGLE * single  : [信号量存储结构体]
 * @param   DWORD    millSec : [等待时间，0为马上返回，INFINTE为无限等待]
 * @return                     [返回值描述]
 * @see                        [参见函数]
 * @n<b>作者</b>             :alzq
 * @n<b>创建时间</b>         : 09-2-9 11:55:30
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-09  [描述]
 ******************************************************************************************************/
UINT ThreadInfo::_WaitForEvent(SINGLE * single, DWORD millSec)
{INTERFUN;
	if(NULL == single)
	{
		this->_SetState(THREAD_STATE_ERROR);
		return WAIT_SINGLE_ERROR;
	}

	ThreadEvent * cEvent = (ThreadEvent *)single->GetSingle();
	if(NULL == cEvent)
	{
		this->_SetState(THREAD_STATE_ERROR);
		return WAIT_SINGLE_ERROR;
	}
	
	if(ThreadTreeControl::_g_bLogWaitObj && _m_bAddLogEnable && single->GetLogEnable())
	{
		//需要输出LOG记录
		USES_CONVERSION;
		CStringA mes;
		mes.Format("wait for single name: \"%s\" -- address: 0x%x "
			,W2A(cEvent->_m_csEventName), single);

		ThreadLog::AddLog(this, mes);
	}

	//获取操作锁
	ThreadTreeControl::_LockThreadControlMutex();

	//初始化本线程的等待操作信息（删除等待SINGLE队列等）
	this->_InitWaitingSituation();

	//设置线程等待状态
	this->_SetWaitStateBySec(millSec);

	//尝试获取信号量，返回失败时，可能存在：本指针无效/信号量无信号
	if(cEvent->_ThreadTryGetEvent(this))
	{
		//释放操作锁
		this->_SetState(THREAD_STATE_SUC);

		ThreadTreeControl::_UnLockThreadControlMutex();

		return WAIT_SINGLE_SUC;
	}
	else
	{
		if(0 == millSec)
		{
			//当等待时间为0时，由于获取信号量失败，所以马上返回超时获取
			this->_SetState(THREAD_STATE_TIMEOUT);
			
			ThreadTreeControl::_UnLockThreadControlMutex();

			return WAIT_SINGLE_TIMEOUT;
		}
		else
		{
			//将线程节点加入信号量节点等待线程队列中
			cEvent->_AddWaitingThread(this);

			//设置线程为等待状态
			this->_SetState(THREAD_STATE_WAIT);

			//在本线程的等待节点里添加等待的信号节点
			this->_AddWaitingSingle(single, millSec);

			//等待时间不为0则可能为无限等待或有限时间等待，因此需要在全局有限等待队列中尝试添加本节点
			//是否有限时间等待在本函数中有判断
			ThreadTreeControl::_AddWaitingThread(this, millSec);

			//挂起线程等待
			this->_TSuspendThread();
		}

		/**
		 *
		 *	---此线程被激活后所执行的函数
		 *
		 **/

		//锁住线程操作锁
		ThreadTreeControl::_LockThreadControlMutex();

		//当线程为有限时间等待时，删除有限时间等待线程队列中本线程节点
		if(_m_uiWaitState == WAIT_TIME_LIMIT)
		{
			ThreadTreeControl::_DelTimeLimitWaitingThread(this);
		}

		//在等待的信号量中的等待队列中删除本线程节点
		cEvent->_DelWaitingThread(this);

		if(NULL == this->_GetFirstWaitSingle())
		{
			//当不存在需要等待的SINGLE时，表明此线程已经成功获取
			this->_SetState(THREAD_STATE_SUC);
		}
		else
		{
			//仍然存在需要等待的SINGLE时，表明获取超时
			this->_SetState(THREAD_STATE_TIMEOUT);
		}

		//清空本线程等待队列
		this->_ClearWaitingSingleList();

		//释放线程操作锁
		ThreadTreeControl::_UnLockThreadControlMutex();

		if(THREAD_STATE_TIMEOUT == _m_uiThreadState)
		{
			return WAIT_SINGLE_TIMEOUT;
		}
		else if(THREAD_STATE_SUC == _m_uiThreadState)
		{
			return WAIT_SINGLE_SUC;
		}
		else
		{
			return WAIT_SINGLE_ERROR;
		}

		/**
		 *
		 *	---激活红函数执行完毕
		 *
		 **/
	}
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [等待锁的处理函数]
 * @n<b>函数名称</b>         : _WaitForMutex
 * @n@param SINGLE * single  : [锁指针存储结构体]
 * @param   DWORD    millSec : [等待时间，0为马上返回，INFINTE为无限等待]
 * @return                     [返回值描述]
 * @see                        [参见函数]
 * @n<b>作者</b>             :alzq
 * @n<b>创建时间</b>         : 09-2-9 11:56:09
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-09  [描述]
 ******************************************************************************************************/
UINT ThreadInfo::_WaitForMutex(SINGLE * single, DWORD millSec)
{INTERFUN;
	if(NULL == single)
	{
		this->_SetState(THREAD_STATE_ERROR);
		return WAIT_SINGLE_ERROR;
	}

	ThreadMutex * cMutex = (ThreadMutex *)single->GetSingle();

	if(NULL == cMutex)
	{
		this->_SetState(THREAD_STATE_ERROR);
		return WAIT_SINGLE_ERROR;
	}
	
	if(ThreadTreeControl::_g_bLogWaitObj && _m_bAddLogEnable && single->GetLogEnable())
	{
		//需要输出LOG记录
		USES_CONVERSION;
		CStringA mes;
		mes.Format("wait for single name: \"%s\" -- address: 0x%x "
			,W2A(cMutex->_m_csMutexName), single);

		ThreadLog::AddLog(this, mes);
	}

	//获取操作锁
	ThreadTreeControl::_LockThreadControlMutex();

	//初始化本线程的等待操作信息（删除等待SINGLE队列等）
	this->_InitWaitingSituation();
	
	//设置线程等待状态
	this->_SetWaitStateBySec(millSec);

	//尝试获取信号量，返回失败时，可能存在：本指针无效/信号量无信号
	if(cMutex->_ThreadTryLockMutex(this))
	{
		//将本线程节点获取到的锁节点队列中添加改SINGLE
		this->_AddControlMutex(single);

		//获取锁成功
		this->_SetState(THREAD_STATE_SUC);

		ThreadTreeControl::_UnLockThreadControlMutex();

		return WAIT_SINGLE_SUC;
	}
	else
	{
		if(0 == millSec)
		{
			//当等待时间为0时，由于获取锁失败，所以马上返回超时获取
			this->_SetState(THREAD_STATE_TIMEOUT);

			ThreadTreeControl::_UnLockThreadControlMutex();

			return WAIT_SINGLE_TIMEOUT;
		}
		else
		{
			//将线程加入锁节点等待线程队列中
			cMutex->_AddWaitingThread(this);

			//设置线程为等待状态
			this->_SetState(THREAD_STATE_WAIT);

			//在本线程的等待节点里添加等待的信号节点
			this->_AddWaitingSingle(single, millSec);

			//等待时间不为0则可能为无限等待或有限时间等待，因此需要在全局有限等待队列中尝试添加本节点
			//是否有限时间等待在本函数中有判断
			ThreadTreeControl::_AddWaitingThread(this, millSec);

			//挂起线程等待
			this->_TSuspendThread();
		}

		/**
		 *
		 *	---此线程被激活后所执行的函数
		 *
		 **/

		//锁住线程操作锁
		ThreadTreeControl::_LockThreadControlMutex();

		//删除有限时间等待线程队列中本线程节点
		if(_m_uiWaitState == WAIT_TIME_LIMIT)
		{
			ThreadTreeControl::_DelTimeLimitWaitingThread(this);
		}

		//在等待的信号量中的等待队列中删除本线程节点
		cMutex->_DelWaitingThread(this);

		if(NULL == this->_GetFirstWaitSingle())
		{
			//将本线程节点获取到的锁节点队列中添加改SINGLE
			this->_AddControlMutex(single);

			//当不存在需要等待的SINGLE时，表明此线程已经成功获取
			this->_SetState(THREAD_STATE_SUC);
		}
		else
		{
			//仍然存在需要等待的SINGLE时，表明获取超时
			this->_SetState(THREAD_STATE_TIMEOUT);
		}
		
		//清空本线程等待队列
		this->_ClearWaitingSingleList();

		//释放线程操作锁
		ThreadTreeControl::_UnLockThreadControlMutex();

		if(THREAD_STATE_TIMEOUT == _m_uiThreadState)
		{
			return WAIT_SINGLE_TIMEOUT;
		}
		else if(THREAD_STATE_SUC == _m_uiThreadState)
		{
			return WAIT_SINGLE_SUC;
		}
		else
		{
			return WAIT_SINGLE_ERROR;
		}

		/**
		 *
		 *	---激活后函数执行完毕
		 *
		 **/
	}
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [获取下一个在等待的锁或信号量，没有下一个则返回NULL]
 * @n<b>函数名称</b>: _GetFirstWaitSingle
 * @return            [返回值描述]
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-2-6 16:05:02
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
SINGLE * ThreadInfo::_GetFirstWaitSingle(void)
{INTERFUN;
	if(_m_swlWaitSingleList.empty())
	{
		//等待队列为空
		return NULL;
	}

	_m_csDealLock->Lock();

	//遍历检查还在等待的SINGLE
	SINGLEWAITLIST::iterator iter = _m_swlWaitSingleList.begin();
	while(iter != _m_swlWaitSingleList.end())
	{
		if(WAIT_STATE_WAIT == (*iter)->state)
		{
			_m_csDealLock->UnLock();
			return (*iter)->singleTarget;
		}

		iter++;
	}

	_m_csDealLock->UnLock();

	return NULL;
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [将等待的锁或信号凉添加到相关的队列中]
 * @n<b>函数名称</b>         : _AddWaitingSingle
 * @n@param SINGLE * single  : [参数描述]
 * @param   DWORD    millSec : [等待时间INFINTE 为无限等待]
 * @see                        [参见函数]
 * @n<b>作者</b>             :alzq
 * @n<b>创建时间</b>         : 09-2-6 16:39:11
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
WAITSTATE * ThreadInfo::_AddWaitingSingle(SINGLE * single, DWORD millSec)
{INTERFUN;
	if(NULL == single)
	{
		return NULL;
	}

	WAITSTATE * waitState = new WAITSTATE(single, WAIT_STATE_WAIT);
	_m_csDealLock->Lock();
	_m_swlWaitSingleList.push_back(waitState);
	_m_csDealLock->UnLock();
	return waitState;
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [在本线程拥有控制权的锁队列中添加指定锁节点]
 * @n<b>函数名称</b>        : _AddControlMutex
 * @n@param SINGLE * single : [参数描述]
 * @see                       [参见函数]
 * @n<b>作者</b>            :alzq
 * @n<b>创建时间</b>        : 09-2-9 16:35:30
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-09  [描述]
 ******************************************************************************************************/
void ThreadInfo::_AddControlMutex(SINGLE * single)
{INTERFUN;
	if(NULL == single)
	{
		return ;
	}

	_m_csDealLock->Lock();

	_m_slGetMutexList.push_back(single);

	_m_csDealLock->UnLock();
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [在本线程拥有控制权的锁队列中删除指定锁节点]
 * @n<b>函数名称</b>        : _DelControlMutex
 * @n@param SINGLE * single : [参数描述]
 * @see                       [参见函数]
 * @n<b>作者</b>            :alzq
 * @n<b>创建时间</b>        : 09-2-9 16:35:35
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-09  [描述]
 ******************************************************************************************************/
void ThreadInfo::_DelControlMutex(SINGLE * single)
{INTERFUN;
	if(NULL == single)
	{
		return ;
	}

	if(_m_slGetMutexList.empty())
	{
		return ;
	}
	
	_m_csDealLock->Lock();

	//搜索相同锁后删除
	SINGLELIST::iterator iter = _m_slGetMutexList.begin();
	while(iter != _m_slGetMutexList.end())
	{
		if((* iter) == single)
		{
			iter = _m_slGetMutexList.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	_m_csDealLock->UnLock();
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [清空等待队列中所有SINGLE]
 * @n<b>函数名称</b>: _ClearWaitingSingleList
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-2-18 15:36:03
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-18  [描述]
 ******************************************************************************************************/
void ThreadInfo::_ClearWaitingSingleList(void)
{INTERFUN;
	_m_csDealLock->Lock();

	SINGLEWAITLIST::iterator waitIter = _m_swlWaitSingleList.begin();
	while(waitIter != _m_swlWaitSingleList.end())
	{
		//根据SINGLE是锁还是信号量分别处理，逻辑上基本相同
		SINGLE * single = (*waitIter)->singleTarget;
		if(NULL != single)
		{
			if(SINGLE_EVENT == single->GetType())
			{
				//信号量
				ThreadEvent * cEvent = (ThreadEvent *)single->GetSingle();

				if(WAIT_STATE_SUC != (*waitIter)->state)
				{
					//未获取控制权则在该SINGLE的等待队列中删除本线程
					cEvent->_DelWaitingThread(this);
				}
			}
			else
			{
				//锁
				ThreadMutex * cMutex = (ThreadMutex *)single->GetSingle();
				
				if(WAIT_STATE_SUC != (*waitIter)->state)
				{
					cMutex->_DelWaitingThread(this);
				}
			}

		}

		try
		{
			SAFE_DELETE(*waitIter);
		}
		catch(...)
		{
			TRACE("delete Wait single list error\n");
		}

		//处理完一个SINGLE则删除队列中该节点
		waitIter = _m_swlWaitSingleList.erase(waitIter);
	}

	_m_csDealLock->UnLock();
}
