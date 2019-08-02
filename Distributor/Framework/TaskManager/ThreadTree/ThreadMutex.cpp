#include "StdAfx.h"
#include "ThreadMutex.h"
#pragma warning( disable: 4996 )
ThreadMutex::ThreadMutex(LPVOID controlThread, BOOL initOwner, const CTCHAR * name)
{INTERFUN;
	_m_csDealLock = new DealSectionControl();

	_m_tlWaitThreadList.clear();					//等待本锁的线程节点地址队列

	if(initOwner)
	{
		//初始为获取状态
		_m_iLockTime = 1;							//本锁的加锁次数
		_m_tlControlThread = controlThread;			//获取本锁控制权的线程节点地址
	}
	else
	{
		_m_iLockTime = 0;							//本锁的加锁次数，当锁为可控制状态时，加锁次数为0
		_m_tlControlThread = NULL;					//获取本锁控制权的线程节点地址
	}


	//锁的名称
	if(NULL == name)
	{
		_m_csMutexName = NULL;
	}
	else
	{
		size_t len = _tcslen(name);
		_m_csMutexName = new CTCHAR[len + 1];
		_tcsncpy(_m_csMutexName, name, len);
		_m_csMutexName[len] = _T('\0');
	}

//private:
	//DWORD				_m_dwTreeID;						//此锁在树型结构中的编号
}

ThreadMutex::~ThreadMutex(void)
{INTERFUN;
	_m_csDealLock->Lock();

	_m_tlWaitThreadList.clear();

	if(NULL != _m_csMutexName)
	{
		delete [] _m_csMutexName;
		_m_csMutexName = NULL;
	}

	_m_csDealLock->UnLock();

	SAFE_DELETE(_m_csDealLock);
}

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [获取锁是否能控制]
 * @n<b>函数名称</b>: _GetMutexLockEnable
 * @return            [返回值描述]
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-2-6 16:10:36
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
BOOL ThreadMutex::_GetMutexLockEnable(void)
{INTERFUN;
	if(_m_iLockTime == 0)
	{
		//当锁加锁次数为0时，表示可以对其加锁
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [检查线程是否拥有控制权]
 * @n<b>函数名称</b>        : _CheckThreadGetControl
 * @n@param LPVOID lpThread : [线程指针]
 * @return                    [返回值描述]
 * @see                       [参见函数]
 * @n<b>作者</b>            :alzq
 * @n<b>创建时间</b>        : 09-2-6 16:21:50
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
BOOL ThreadMutex::_CheckThreadGetControl(LPVOID lpThread)
{INTERFUN;
	//先判断线程指针是否为空
	if(NULL == lpThread)
	{
		return FALSE;
	}

	//检查获取控制权的指针是否与该指针相等
	if(_m_tlControlThread == lpThread)
	{
		//相等
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [线程对锁进行锁定，返回是否获取锁成功]
 * @n<b>函数名称</b>        : _ThreadTryLockMutex
 * @n@param LPVOID lpThread : [线程节点地址]
 * @return                    [返回值描述]
 * @see                       [参见函数]
 * @n<b>作者</b>            :alzq
 * @n<b>创建时间</b>        : 09-2-6 16:25:34
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
BOOL ThreadMutex::_ThreadTryLockMutex(LPVOID lpThread)
{INTERFUN;
	//先判断线程指针是否为空
	if(NULL == lpThread)
	{
		return FALSE;
	}

	//检查信号量是否仍然有资源
	if(this->_GetMutexLockEnable())
	{
		//仍然有资源时
		_m_tlControlThread = lpThread;

		//加锁次数加1
		_m_iLockTime++;

		return TRUE;
	}
	else
	{
		if(_m_tlControlThread == lpThread)
		{
			//加锁次数加1
			_m_iLockTime++;

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [在本锁的等待线程队列中添加线程节点地址]
 * @n<b>函数名称</b>        : _AddWaitingThread
 * @n@param LPVOID lpThread : [参数描述]
 * @see                       [参见函数]
 * @n<b>作者</b>            :alzq
 * @n<b>创建时间</b>        : 09-2-6 16:38:40
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
void ThreadMutex::_AddWaitingThread(LPVOID lpThread)
{INTERFUN;
	if(NULL == lpThread)
	{
		return ;
	}

	_m_csDealLock->Lock();

	//检查等待线程队列，发现相同指针则删除该队列节点，并在等待队列末尾添加对应节点
	THREADLIST::iterator iter = _m_tlWaitThreadList.begin();
	while(iter != _m_tlWaitThreadList.end())
	{
		if((*iter) == lpThread)
		{
			_m_tlWaitThreadList.erase(iter);
		}

		iter++;
	}

	//在等待队列结尾添加线程节点地址
	_m_tlWaitThreadList.push_back(lpThread);

	_m_csDealLock->UnLock();
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [释放锁（将锁的加锁次数减1，需要对锁的拥有权进行判断]
 * @n<b>函数名称</b>        : _ReduceLockTimeMutex
 * @n@param LPVOID lpThread : [参数描述]
 * @return                    [返回是否释放成功，当线程不拥有控制权时则释放不成功]
 * @see                       [参见函数]
 * @n<b>作者</b>            :alzq
 * @n<b>创建时间</b>        : 09-2-6 17:02:22
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
BOOL ThreadMutex::_ReduceLockTimeMutex(LPVOID lpThread)
{INTERFUN;
	if(NULL == lpThread)
	{
		return FALSE;
	}

	//检查该线程是否获取到锁的控制权
	if(_m_tlControlThread != lpThread)
	{
		return FALSE;
	}

	//将加锁次数减1，当锁变为无拥有权时，设置获取指针为NULL
	_m_iLockTime--;

	if(0 == _m_iLockTime)
	{
		_m_tlControlThread = NULL;
	}

	return TRUE;
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [获取下一个在等待该锁的线程节点地址]
 * @n<b>函数名称</b>: _GetFirstWaitingThread
 * @return            [返回值描述]
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-2-6 17:08:54
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
LPVOID ThreadMutex::_GetFirstWaitingThread(void)
{INTERFUN;
	if(_m_tlWaitThreadList.empty())
	{
		return NULL;
	}

	return (*_m_tlWaitThreadList.begin());
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [将等待线程节点从本锁的等待线程队列中删除]
 * @n<b>函数名称</b>        : _DelWaitingThread
 * @n@param LPVOID lpThread : [参数描述]
 * @see                       [参见函数]
 * @n<b>作者</b>            :alzq
 * @n<b>创建时间</b>        : 09-2-6 17:11:19
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
void ThreadMutex::_DelWaitingThread(LPVOID lpThread)
{INTERFUN;
	if(NULL == lpThread)
	{
		return ;
	}

	if(_m_tlWaitThreadList.empty())
	{
		//等待线程队列为空
		return ;
	}

	_m_csDealLock->Lock();

	//遍历查找相同线程指针
	THREADLIST::iterator iter = _m_tlWaitThreadList.begin();
	while(iter != _m_tlWaitThreadList.end())
	{
		if((*iter) != lpThread)
		{
			iter++;
		}
		else
		{
			//当指针相同时删除对应队列节点
			_m_tlWaitThreadList.erase(iter);
			_m_csDealLock->UnLock();
			return ;
		}
	}

	_m_csDealLock->UnLock();
}
