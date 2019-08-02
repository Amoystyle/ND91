#include "StdAfx.h"
#include "ThreadControl.h"
#include "ThreadTreeControl.h"
#pragma warning( disable: 4996 )
#pragma warning( disable: 4800 )
ThreadControl::ThreadControl(void)
{INTERFUN;
	_m_tiThreadInfo = NULL;			//�洢�߳̽ڵ��ַ�����Զ��߳̽��в���
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
 * �����Ƿ�Ա��߳����߳���LOG�н��м�¼
 * @n<b>��������</b>: SetLogEnable
 * @see              
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-3-16 15:38:34
 * @version	�޸���        ʱ��        ����@n
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
 * @brief [������Ҫ����]
 * 
 * [�ر��̣߳������ں���ִ�н������̲߳���һ��������ֹ]
 * @n<b>��������</b>      : DeleteThread
 * @n@param BOOL delChild : [�Ƿ�ɾ���ӽڵ��߳�]
 * @see                     [�μ�����]
 * @n<b>����</b>          :alzq
 * @n<b>����ʱ��</b>      : 09-2-19 10:00:19
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-19  [����]
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
