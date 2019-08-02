#include "StdAfx.h"
#include "ThreadControl.h"
#include "ThreadTreeControl.h"
#pragma warning( disable: 4996 )
#pragma warning( disable: 4800 )
ThreadControl::ThreadControl(void)
{INTERFUN;
	_m_tiThreadInfo = NULL;			//存储线程节点地址，用以对线程进行操作
	ThreadNum=0;
	_m_bThreadLogEnable = false;
}
ThreadControl::~ThreadControl(void)
{INTERFUN;
	ThreadTreeControl::_LockThreadControlMutex();
	if(NULL != _m_tiThreadInfo && NULL != _m_tiThreadInfo->data.customThreadInfo)
	{
		((CUSTOMTHREADINFO *)_m_tiThreadInfo->data.customThreadInfo)->threadInfo->_m_lpControl = NULL;
	}
	ThreadTreeControl::_UnLockThreadControlMutex();

	_m_tiThreadInfo = NULL;
}

/************************************************************************************
 * @brief
 * 
 * 设置是否对本线程在线程树LOG中进行记录
 * @n<b>函数名称</b>: SetLogEnable
 * @see              
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-3-16 15:38:34
 * @version	修改者        时间        描述@n
 * @n		alzq          2009-03-16 
 ************************************************************************************/
void ThreadControl::SetLogEnable(bool enable)
{INTERFUN;
	_m_bThreadLogEnable = enable;

	ThreadTreeControl::_LockThreadControlMutex();
	if(NULL != _m_tiThreadInfo && NULL != _m_tiThreadInfo->data.customThreadInfo)
	{
		((CUSTOMTHREADINFO *)_m_tiThreadInfo->data.customThreadInfo)->threadInfo->_m_bAddLogEnable = _m_bThreadLogEnable;
	}
	ThreadTreeControl::_UnLockThreadControlMutex();
}
	

BOOL ThreadControl::ThreadAlive(void)
{INTERFUN;
	if(NULL != _m_tiThreadInfo)
	{
		return TRUE;
	}

	return FALSE;
}
HANDLE ThreadControl::GetThreadHandle(void)
{INTERFUN;
	ThreadInfo * info = this->_GetThreadInfo();

	if(NULL == info)
		return NULL;

	return info->_m_ttThreadTarget.handle;
}
DWORD ThreadControl::GetThreadId(void)
{INTERFUN;
	ThreadInfo * info = this->_GetThreadInfo();

	if(NULL == info)
		return NULL;

	return info->_m_ttThreadTarget.threadId;
}
bool ThreadControl::GetThreadIsUIThread(void)
{INTERFUN;
	ThreadInfo * info = this->_GetThreadInfo();

	if(NULL == info)
		return false;

	return info->_m_ttThreadTarget.isWinThread;
}
LPVOID ThreadControl::GetLPWinThread(void)
{INTERFUN;
	ThreadInfo * info = this->_GetThreadInfo();

	if(NULL == info)
		return NULL;

	return info->_m_ttThreadTarget.winThread;
}
/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [关闭线程，但是在函数执行结束后本线程并不一定马上终止]
 * @n<b>函数名称</b>      : DeleteThread
 * @n@param BOOL delChild : [是否删除子节点线程]
 * @see                     [参见函数]
 * @n<b>作者</b>          :alzq
 * @n<b>创建时间</b>      : 09-2-19 10:00:19
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-02-19  [描述]
 ******************************************************************************************************/
void ThreadControl::DeleteThread(BOOL delChild, DWORD delayTime)
{INTERFUN;
	ThreadTreeControl::DeleteThread(_m_tiThreadInfo, delChild, delayTime);

	_m_tiThreadInfo = NULL;
}

ThreadInfo * ThreadControl::_GetThreadInfo(void)
{INTERFUN;
	if(NULL == _m_tiThreadInfo || NULL == _m_tiThreadInfo->data.customThreadInfo)
		return NULL;

	CUSTOMTHREADINFO * info = (CUSTOMTHREADINFO *)_m_tiThreadInfo->data.customThreadInfo;

	if(NULL == info->threadInfo)
		return NULL;

	return info->threadInfo;
}
