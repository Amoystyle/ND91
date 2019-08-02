#include "StdAfx.h"
#include "ThreadEvent.h"
#pragma warning( disable: 4996 )
ThreadEvent::ThreadEvent(BOOL autoReSet, BOOL initState, const CTCHAR * name)
{INTERFUN;
	_m_csDealLock = new DealSectionControl();

	_m_tlWaitThreadList.clear();				//等待本锁的线程节点地址队列

	_m_iInitSourceCount = 1;				//初始化的资源数量

	if(initState)
	{
		//initState为TRUE表示初始化信号量为有信号状态
		_m_iSourceCount = 1;					//信号量的剩余资源数统计（大于0为可获取状态
	}
	else
	{
		_m_iSourceCount = 0;					//信号量的剩余资源数统计（大于0为可获取状态
	}

	//信号量的名称
	if(NULL == name)
	{
		_m_csEventName = NULL;
	}
	else
	{
		size_t len = _tcslen(name);
		_m_csEventName = new CTCHAR[len + 1];
		_tcscpy(_m_csEventName, name);
	}

	_m_bAutoReset = autoReSet;						//是否自动改变状态的信号量

//private:
	//DWORD				_m_dwTreeID;						//此锁在树型结构中的编号
}

ThreadEvent::~ThreadEvent(void)
{INTERFUN;
	_m_csDealLock->Lock();

	_m_tlWaitThreadList.clear();				//等待本锁的线程节点地址队列

	if(NULL != _m_csEventName)
	{
		delete [] _m_csEventName;
		_m_csEventName = NULL;
	}

	_m_csDealLock->UnLock();

	SAFE_DELETE(_m_csDealLock);
}

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [获取信号量是否为有信号状态]
 * @n<b>函数名称</b>: _GetEventSingleEnable
 * @return            [返回值描述]
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-2-6 16:10:36
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
BOOL ThreadEvent::_GetEventSingleEnable(void)
{INTERFUN;
	if(this->_m_iSourceCount > 0)
	{
		//当资源统计大于0时
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
 * [线程尝试获取信号，返回是否获取成功。]
 * @n<b>函数名称</b>        : _ThreadTryGetEvent
 * @n@param LPVOID lpThread : [线程节点地址]
 * @return                    [返回值描述]
 * @see                       [参见函数]
 * @n<b>作者</b>            :alzq
 * @n<b>创建时间</b>        : 09-2-6 16:25:34
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
BOOL ThreadEvent::_ThreadTryGetEvent(LPVOID lpThread)
{INTERFUN;
	//先判断线程指针是否为空
	if(NULL == lpThread)
	{
		return FALSE;
	}

	//检查信号量是否仍然有资源
	if(this->_GetEventSingleEnable())
	{
		//仍然有资源时
		if(_m_bAutoReset)
		{
			//需要自动设置信号量时
			_m_iSourceCount--;
		}

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
 * [在本信号量的等待线程队列中添加线程节点地址]
 * @n<b>函数名称</b>        : _AddWaitingThread
 * @n@param LPVOID lpThread : [参数描述]
 * @see                       [参见函数]
 * @n<b>作者</b>            :alzq
 * @n<b>创建时间</b>        : 09-2-6 16:38:40
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
void ThreadEvent::_AddWaitingThread(LPVOID lpThread)
{INTERFUN;
	//先判断线程指针是否为空
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
 * [释放信号量（将信号量中资源统计加1）]
 * @n<b>函数名称</b>        : _ReleaseEvent
 * @return                    [返回是否释放成功，当线程不拥有控制权时则释放不成功]
 * @see                       [参见函数]
 * @n<b>作者</b>            :alzq
 * @n<b>创建时间</b>        : 09-2-6 17:02:22
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-06  [描述]
 ******************************************************************************************************/
BOOL ThreadEvent::_ReleaseEvent(void)
{INTERFUN;
	if(_m_iSourceCount < _m_iInitSourceCount)
	{
		//当资源统计小于初始化的资源统计时
		_m_iSourceCount++;
		return TRUE;
	}

	return FALSE;
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
LPVOID ThreadEvent::_GetFirstWaitingThread(void)
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
void ThreadEvent::_DelWaitingThread(LPVOID lpThread)
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
