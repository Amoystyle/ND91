#include "StdAfx.h"
#include "ThreadInfo.h"
#include "ThreadControl.h"
#include "ThreadTreeControl.h"
#include "ThreadLog.h"
#pragma warning( disable: 4996 )
ThreadInfo::ThreadInfo(LPVOID userData, DELETE_THREAD_FUNCTION exitThreadFunc, LPVOID control, char * name)
{INTERFUN;
	_m_csDealLock = new DealSectionControl();

	_m_uiThreadState = THREAD_STATE_SUC;		//�߳�״̬��־�������洢��ǰ�߳�״̬

	_m_slGetMutexList.clear();					//���̻߳�ȡ����Ȩ�����ڵ��־����

	_m_swlWaitSingleList.clear();				//���̵߳ȴ��������ź��������ȡ״̬����

	_m_bResetWaitTimeEnable = FALSE;			//��ʾ���߳��ڵȴ���������ź���ʱ���Ƿ��ڻ�ȡ�����е�һ��ʱ�������õȴ�ʱ��
	_m_iWaitTimeInit = 0;						//���̴߳˴εȴ��ĳ�ʼʱ��
	_m_iWaitTimeCount = 0;						//���̴߳˴εȴ���ʱ��ͳ��

	_m_uiWaitState = WAIT_INFINTE;				//��ʾ���̴߳˴εȴ�״̬�����޵ȴ��������޵ȴ���

	_m_hFinalMutex = CreateMutex(NULL, FALSE, NULL);						//��ʾ�߳�ִ����ϵ��ź�����ʹ��������ź����洢���ṹ���п��ƣ�

	//THREADTARGET		_m_ttThreadTarget;		//�洢�߳������Ϣ

	_m_lpUserData = userData;					//�û������̵߳�ָ��

	_m_iChildCount = 0;							//���߳̽ڵ��µ����߳̽ڵ�ͳ��

	_m_dtfDeleteThreadFunc = exitThreadFunc;
	
	if(control)
	{
		_m_bAddLogEnable = ((LPTHREADCONTROL)control)->_m_bThreadLogEnable;					//�Ƿ�����뱾�߳��йص��߳���LOG��¼��Ĭ��Ϊfalse
	}
	else
	{
		_m_bAddLogEnable = false;
	}

	//��������
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
	//����Ҫ����Ƿ�����ӽڵ㣬�˲��������ͽṹ��ɾ���ڲ���

//private:

//protected:
	//ThreadTreeControl::_LockThreadControlMutex();
	//_m_uiThreadState = THREAD_STATE_SUC;		//�߳�״̬��־�������洢��ǰ�߳�״̬
	_m_uiThreadState = THREAD_STATE_CLOSE;


	//�ͷ������Ѿ���ȡ�����Ŀ���Ȩ
	//_m_slGetMutexList.clear();					//���̻߳�ȡ����Ȩ�����ڵ��־����
	//this->_ReleaseAllControlMutex();
	_m_slGetMutexList.clear();


	//ɾ���������ڵȴ���SINGLE�����еĽڵ�,�����˲���ʱ��Ҫ��ȡ�̲߳�����
	//_m_swlWaitSingleList.clear();				//���̵߳ȴ��������ź��������ȡ״̬����
	//this->_ClearWaitingSingleList();
	//	
	////������ʱ��ȴ��̶߳�����ɾ�����߳�
	//ThreadTreeControl::_DelTimeLimitWaitingThread(this);

	_m_swlWaitSingleList.clear();

	//_m_bResetWaitTimeEnable = FALSE;			//��ʾ���߳��ڵȴ���������ź���ʱ���Ƿ��ڻ�ȡ�����е�һ��ʱ�������õȴ�ʱ��
	//_m_iWaitTimeInit = 0;						//���̴߳˴εȴ��ĳ�ʼʱ��
	//_m_iWaitTimeCount = 0;					//���̴߳˴εȴ���ʱ��ͳ��

	//_m_uiWaitState = WAIT_INFINTE;			//��ʾ���̴߳˴εȴ�״̬�����޵ȴ��������޵ȴ���

	if(NULL != _m_hFinalMutex)
	{
		CloseHandle(_m_hFinalMutex);			//��ʾ�߳�ִ����ϵ��ź�����ʹ��������ź����洢���ṹ���п��ƣ�
		_m_hFinalMutex = NULL;
	}

	//THREADTARGET		_m_ttThreadTarget;		//�洢�߳������Ϣ

	_m_lpUserData = NULL;						//�û������̵߳�ָ��

	_m_iChildCount = 0;							//���߳̽ڵ��µ����߳̽ڵ�ͳ��

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
 * @brief [������Ҫ����]
 * 
 * [��ȡ�û���������]
 * @n<b>��������</b>: GetUserInfo
 * @return            [����ֵ����]
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-13 14:47:18
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-13  [����]
 ******************************************************************************************************/
LPVOID ThreadInfo::_GetUserInfo(void)
{INTERFUN;
	return _m_lpUserData;
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�ȴ�ĳ���ź�������]
 * @n<b>��������</b>         : WaitForSingle
 * @n@param SINGLE * single  : [�ź��������Ĵ洢��ַ]
 * @param   DWORD    millSec : [�ȴ�ʱ�䣬 INFINITE ʱΪ���޵ȴ�]
 * @return                     [���صȴ��Ľ��]
 * @see                        [�μ�����]
 * @n<b>����</b>             :alzq
 * @n<b>����ʱ��</b>         : 09-2-6 14:12:29
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
UINT ThreadInfo::_TWaitForSingle(SINGLE * single, DWORD millSec)
{INTERFUN;
	if(!_CheckThread())
	{
		//��ǰ�̲߳��Ǳ��ڵ�ָ���߳�
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

	//�ж��ź���𣬺���ݾ��������д���
	if(single->GetType() == SINGLE_EVENT)
	{
		//Ϊ�ź����Ĵ���ʽ
		res = this->_WaitForEvent(single, millSec);
	}
	else if(single->GetType() == SINGLE_MUTEX)
	{
		//Ϊ��ʱ�Ĵ���ʽ
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
 * @brief [������Ҫ����]
 * 
 * [�ȴ���������ź����ļ���]
 * @n<b>��������</b>                  : WaitForSingleList
 * @n@param SINGLELIST singleList     : [�����ź����Ľڵ����]
 * @param   DWORD      millSec        : [�ȴ�ʱ�䣬 INFINITE ʱΪ���޵ȴ�]
 * @param   BOOL       resetTimeCount : [�ڵȴ�һ���ɹ�ʱ���Ƿ��������õȴ�ʱ���ָʾ����]
 * @return                              [���صȴ��Ľ��]
 * @see                                 [�μ�����]
 * @n<b>����</b>                      :alzq
 * @n<b>����ʱ��</b>                  : 09-2-6 14:22:07
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
UINT ThreadInfo::_TWaitForSingleList(SINGLELIST singleList, DWORD millSec, BOOL resetTimeCount)
{INTERFUN;
	/**
	 *	---�˺�����Ҫ�������£�
	 *	���ȴ����źŻ���������м�⣬���źŻ�������Ҫ�ȴ������ֱ�ӻ�ȡ��������
	 *	���������Ҫ�ȴ��������ź���������̵߳ȴ�����
	 **/
	if(!_CheckThread())
	{
		//��ǰ�̲߳��Ǳ��ڵ�ָ���߳�
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

	//��ձ��߳̽ڵ�����ر��������
	this->_InitWaitingSituation();

	//���ô˴εȴ���ʼʱ��
	_m_iWaitTimeInit = millSec;

	//�����ڻ�ȡ��һ�������Ƿ����õȴ�ʱ��
	_m_bResetWaitTimeEnable = resetTimeCount;

	//��ȡ����ʱ��ȴ��̶߳��п�����
	ThreadTreeControl::_LockThreadControlMutex();

	SINGLELIST::iterator iter = singleList.begin();
	while(iter != singleList.end())
	{
		//�����ȡSINGLE
		SINGLE * single = (*iter);

		if(NULL == single)
		{
			ThreadTreeControl::_UnLockThreadControlMutex();
			return WAIT_SINGLE_ERROR;
		}

		//�ڵȴ������������
		WAITSTATE * waitState = this->_AddWaitingSingle(single, millSec);

		//���ݲ�ͬ���ͻ�ȡ��ͬ�ĵȴ�����
		if(single->GetType() == SINGLE_MUTEX)
		{
			ThreadMutex * cMutex = (ThreadMutex *)single->GetSingle();

			if(cMutex->_ThreadTryLockMutex(this))
			{
				//���Լ����ɹ�
				
				//�����߳̽ڵ��ȡ�������ڵ��������Ӹ�SINGLE
				this->_AddControlMutex(single);

				//���ȴ��ź��������Ķ����иýڵ��ȡ״̬����Ϊ�ѻ�ȡ
				waitState->state = WAIT_STATE_SUC;
			}
			else
			{
				//���Լ���ʧ��ʱ����״̬Ϊ���ɼ���״̬�����߳�δӵ�п���Ȩ

				//�жϵȴ�ʱ���Ƿ�Ϊ0
				if(0 == millSec)
				{
					//���Ѿ���ȡ���������ź����ͷţ�Ȼ�󷵻�TIME_OUT

					//�ͷ��Ѿ���ȡ���������ź���
					this->_ReleaseAllWaitingSingle();

					//���س�ʱ
					ThreadTreeControl::_UnLockThreadControlMutex();
					return WAIT_SINGLE_TIMEOUT;
				}
				else
				{
					//�����ڵ�ĵȴ��̶߳�������Ӹ��߳̽ڵ�
					cMutex->_AddWaitingThread(this);

					_m_uiThreadState = THREAD_STATE_WAIT;

					//���߳���ȴ�ʱ����룬��ӵ�ȫ�ֵĵȴ�������
					ThreadTreeControl::_AddWaitingThread(this, millSec);

					//���õȴ��ڵ�״̬
					waitState->state = WAIT_STATE_WAIT;
				}
			}
		}
		else if(single->GetType() == SINGLE_EVENT)
		{
			ThreadEvent * cEvent = (ThreadEvent *)single->GetSingle();

			if(cEvent->_ThreadTryGetEvent(this))
			{
				//���Ի�ȡ�ź����ɹ�

				//���ȴ��ź��������Ķ����иýڵ��ȡ״̬����Ϊ�ѻ�ȡ�ɹ�
				waitState->state = WAIT_STATE_SUC;
			}
			else
			{
				//���Լ���ʧ��ʱ����״̬Ϊ���ɼ���״̬�����߳�δӵ�п���Ȩ

				//�жϵȴ�ʱ���Ƿ�Ϊ0
				if(0 == millSec)
				{
					//���Ѿ���ȡ���������ź����ͷţ�Ȼ�󷵻�TIME_OUT

					//�ͷ��Ѿ���ȡ���������ź���
					this->_ReleaseAllWaitingSingle();

					//���س�ʱ
					ThreadTreeControl::_UnLockThreadControlMutex();
					return WAIT_SINGLE_TIMEOUT;
				}
				else
				{
					//�����ڵ�ĵȴ��̶߳�������Ӹ��߳̽ڵ�
					cEvent->_AddWaitingThread(this);

					_m_uiThreadState = THREAD_STATE_WAIT;

					//���߳���ȴ�ʱ����룬��ӵ�ȫ�ֵĵȴ�������
					ThreadTreeControl::_AddWaitingThread(this, millSec);

					//���õȴ��ڵ�״̬
					waitState->state = WAIT_STATE_WAIT;
				}
			}
		}

		iter++;
	}

	//ѭ������ȴ��ڵ����Ҫ�����߳�״̬�ж��Ƿ���Ҫ�����̣߳��������߳���ֱ�ӷ���

	while(THREAD_STATE_WAIT == _m_uiThreadState)
	{
		//�������Ҫ�ȴ���״̬���ڹ����������ͷ�������
		this->_TSuspendThread();

		//��ȡ����ʱ��ȴ��̶߳��п�����
		ThreadTreeControl::_LockThreadControlMutex();

		//�����̱߳�����ʱ��������������������ź����ɹ���ȡ�����߳����ȴ�ʱ��
		SINGLE * single = this->_GetFirstWaitSingle();
		//����ȡ������һ���ȴ�SINGLEΪ��ʱ����ʾ�˴εȴ��ɹ�
		if(NULL == single)
		{
			//�ȴ������ɹ�

			//ɾ������ʱ��ȴ��̶߳����и��߳̽ڵ�
			ThreadTreeControl::_DelTimeLimitWaitingThread(this);

			//�ͷ�����ʱ��ȴ��̶߳��п�����
			ThreadTreeControl::_UnLockThreadControlMutex();

			return WAIT_SINGLE_SUC;
		}
		else
		{
			if(THREAD_STATE_TIMEOUT == _m_uiThreadState)
			{
				//���Ϊ��ʱʱ����ʾ�ȴ����Ϊ��ʱ

				//�ͷ��Ѿ���ȡ���������ź���
				this->_ReleaseAllWaitingSingle();

				//ɾ������ʱ��ȴ��̶߳����и��߳̽ڵ�
				ThreadTreeControl::_DelTimeLimitWaitingThread(this);
			
				//�ͷ�����ʱ��ȴ��̶߳��п�����
				ThreadTreeControl::_UnLockThreadControlMutex();

				return WAIT_SINGLE_TIMEOUT;
			}
			else
			{
				//�����Ϊ��ʱʱ�������ȴ�������WHILEǰ��������

				//�ж��Ƿ���Ҫ�������õȴ�ʱ��
				if(_m_bResetWaitTimeEnable)
				{
					_m_iWaitTimeCount = _m_iWaitTimeInit;
				}

				_m_uiThreadState = THREAD_STATE_WAIT;
			}
		}
	}

	//��һ���Ի�ȡ�����еȴ��������ź�ʱ�������е�����

	//�ͷ�����ʱ��ȴ��̶߳��п�����
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
 * @brief [������Ҫ����]
 * 
 * [�Ա��߳�Ϊ���ڵ㴴����]
 * @n<b>��������</b>                : TCreateMutex
 * @n@param BOOL          initState : [��ʼ״̬]
 * @param   const WCHAR * name      : [�����ƣ�Ĭ��Ϊ��]
 * @return                            [����ֵ����]
 * @see                               [�μ�����]
 * @n<b>����</b>                    :alzq
 * @n<b>����ʱ��</b>                : 09-2-6 14:24:52
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
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
 * @brief [������Ҫ����]
 * 
 * [������ϸ����]
 * @n<b>��������</b>                : TCreateEvent
 * @n@param BOOL          autoReSet : [�Ƿ��Զ�ת���ź�״̬]
 * @param   BOOL          initState : [��ʼ״̬]
 * @param   const WCHAR * name      : [�ź������ƣ�Ĭ��Ϊ��]
 * @return                            [����ֵ����]
 * @see                               [�μ�����]
 * @n<b>����</b>                    :alzq
 * @n<b>����ʱ��</b>                : 09-2-6 14:26:27
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
SINGLE * ThreadInfo::_TCreateEvent(BOOL autoReSet, BOOL initState, const WCHAR * name)
{INTERFUN;
	return ThreadTreeControl::_TCreateEvent(autoReSet, initState, name);
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�ͷ���]
 * @n<b>��������</b>        : TReleaseMutex
 * @n@param SINGLE * single : [���Ľڵ��ַ]
 * @return                    [����ֵ����]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-6 14:51:04
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
UINT ThreadInfo::_TReleaseMutex(SINGLE * single)
{INTERFUN;
	/*
	 *
	 *	-�����ͷ��������ڽ����߳�ʱ�������������߳�ʱ���������ͷŵĿ��ܲ���ִ���̣߳����Դ˲�����߳����ȡ��
	 *
	 */
	//if(!_CheckThread())
	//{
	//	//��ǰ�̲߳��Ǳ��ڵ�ָ���߳�
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
		//���SINGLE�����ǲ�����
#ifdef _DEBUG
		DebugBreak();
#endif

		return RELEASE_MUTEX_ERROR;
	}

	//��ȡSINGLE�е����ڵ�
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
		//��Ҫ���LOG��¼
		USES_CONVERSION;
		CStringA mes;
		mes.Format("release mutex name: \"%s\" -- address: 0x%x "
			,W2A(cMutex->_m_csMutexName), single);

		ThreadLog::AddLog(this, mes);
	}

	//��ȡ����ʱ��ȴ��̶߳��в�����
	ThreadTreeControl::_LockThreadControlMutex();

	//���Խ����ļ���������1
	if(cMutex->_ReduceLockTimeMutex(this))
	{
		//�����ɹ�

		//�ж����Ƿ��ܱ���ȡ�����ܱ���ȡʱ����Ҫ������һ���ȴ��������߳�
		if(cMutex->_GetMutexLockEnable())
		{
			//���ܱ���ȡ

			//�����ڵ�ӱ��̻߳�ȡ������Ȩ����������ɾ��
			this->_DelControlMutex(single);

			ThreadInfo * nextThread = (ThreadInfo *)cMutex->_GetFirstWaitingThread();

			if(NULL == nextThread)
			{
				//��ȡ����ʱ��ȴ��̶߳��в�����
				ThreadTreeControl::_UnLockThreadControlMutex();

				return RELEASE_MUTEX_SUC;
			}
			else
			{
				//������һ���ȴ��̵߳�ִ�У������ڼ���

				//ʹ����һ�̶߳������м�����˳������������ӵ���̣߳�
				if(cMutex->_ThreadTryLockMutex(nextThread))
				{
					//�����ɹ�
					cMutex->_DelWaitingThread(nextThread);

					//�����߳��и�����״̬Ϊ�ѻ�ȡ
					nextThread->_SetSingleState(single, WAIT_STATE_SUC);

					//�����߳�ִ��
					nextThread->_TResumeThread();
				}
				else
				{
				}
			}
		}

		//��ȡ����ʱ��ȴ��̶߳��в�����
		ThreadTreeControl::_UnLockThreadControlMutex();

		Sleep(0);

		return RELEASE_MUTEX_SUC;
	}
	else
	{

		//��ȡ����ʱ��ȴ��̶߳��в�����
		ThreadTreeControl::_UnLockThreadControlMutex();

		Sleep(0);

		return RELEASE_MUTEX_FAIL;
	}
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�����ź���Ϊ���ź�״̬]
 * @n<b>��������</b>        : TSetEventSingle
 * @n@param SINGLE * single : [�ź����Ľڵ��ַ]
 * @return                    [����ֵ����]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-6 14:57:03
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
UINT ThreadInfo::_TSetEventSingle(SINGLE * single)
{INTERFUN;
	return ThreadTreeControl::TSetEventSingle(single);
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�����ź���Ϊ���ź�״̬]
 * @n<b>��������</b>        : TSetEventNoSingle
 * @n@param SINGLE * single : [�ź����Ľڵ��ַ]
 * @return                    [����ֵ����]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-6 14:57:06
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
UINT ThreadInfo::_TSetEventNoSingle(SINGLE * single)
{INTERFUN;
	return ThreadTreeControl::TSetEventNoSingle(single);
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�ͷ����п��Ƶ���]
 * @n<b>��������</b>: _ReleaseAllControlMutex
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-25 10:50:00
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-25  [����]
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
					//���ü�������Ϊ1�����ͷŸ���
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

		//������һ��SINGLE��ɾ�������иýڵ�
		singleIter = _m_slGetMutexList.begin();
	}

	_m_csDealLock->UnLock();
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [���鵱ǰ�����߳��Ƿ��뱾�ڵ���ָ�߳���ͬ]
 * @n<b>��������</b>: _CheckThread
 * @return            [����ֵ����]
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-19 9:02:51
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-19  [����]
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
 * @brief [������Ҫ����]
 * 
 * [���ݱ��̴߳洢����ز��������߳�]
 * @n<b>��������</b>: TSuspendThread
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-16 9:12:42
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-16  [����]
 ******************************************************************************************************/
void ThreadInfo::_TSuspendThread(void)
{INTERFUN;
	ThreadTreeControl::_TSuspendThread(this);
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [���ݱ��̴߳洢����ز����ָ����߳�]
 * @n<b>��������</b>: TResumeThread
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-16 9:12:42
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-16  [����]
 ******************************************************************************************************/
void ThreadInfo::_TResumeThread(void)
{INTERFUN;
	ThreadTreeControl::_TResumeThread(this);
}
	


/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�����صĵȴ����У���ʼ����ر���]
 * @n<b>��������</b>: _ClearWaitingSingleList
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-10 13:33:02
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-10  [����]
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
 * @brief [������Ҫ����]
 * 
 * [�ͷ����еȴ��е�SINGLE�������Ѿ���ȡ���������ź���]
 * @n<b>��������</b>: _ReleaseAllWaitingSingle
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-10 13:48:47
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-10  [����]
 ******************************************************************************************************/
void ThreadInfo::_ReleaseAllWaitingSingle(void)
{INTERFUN;
	_m_csDealLock->Lock();

	SINGLEWAITLIST::iterator iter = _m_swlWaitSingleList.begin();
	while(iter != _m_swlWaitSingleList.end())
	{
		WAITSTATE * state = (*iter);

		//�ж��Ƿ��Ѿ���ȡ��
		if(WAIT_STATE_SUC == state->state)
		{
			SINGLE * single = state->singleTarget;

			if(single->GetType() == SINGLE_EVENT)
			{
				//�ź���

				//���ź����ָ�
				this->_TSetEventSingle(single);
			}
			else if(single->GetType() == SINGLE_MUTEX)
			{
				//��

				//�ͷű��̻߳�ȡ����
				this->_TReleaseMutex(single);
			}
		}
		else
		{
			//δ��ȡ
			SINGLE * single = state->singleTarget;
			if(SINGLE_EVENT == single->GetType())
			{
				//�ź���
				ThreadEvent * cEvent = (ThreadEvent *)single->GetSingle();
				cEvent->_DelWaitingThread(this);
			}
			else
			{
				//��
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
 * @brief [������Ҫ����]
 * 
 * [���õȴ�SINGLE��״̬]
 * @n<b>��������</b>        : _SetSingleState
 * @n@param SINGLE * single : [�ȴ���SINGLE��ַ]
 * @param   UINT     state  : [��Ҫ���õ�״̬]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-10 17:40:49
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-10  [����]
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
 * @brief [������Ҫ����]
 * 
 * [���ݵȴ�ʱ�����õȴ�״̬Ϊ���޵ȴ���������ʱ��ȴ��������õȴ�ʱ�����ʼ�ȴ�ʱ��]
 * @n<b>��������</b>         : _SetWaitStateBySec
 * @n@param int millSec      : [�ȴ�ʱ��]
 * @see                        [�μ�����]
 * @n<b>����</b>             :alzq
 * @n<b>����ʱ��</b>         : 09-2-9 15:17:21
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-09  [����]
 ******************************************************************************************************/
void ThreadInfo::_SetWaitStateBySec(DWORD millSec)
{INTERFUN;
	//���ô˴εȴ���ʼʱ��
	_m_iWaitTimeInit = millSec;

	//���ô˴εȴ�ʱ��
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
 * @brief [������Ҫ����]
 * 
 * [�����߳�ִ��״̬]
 * @n<b>��������</b>   : _SetState
 * @n@param UINT state : [��������]
 * @see                  [�μ�����]
 * @n<b>����</b>       :alzq
 * @n<b>����ʱ��</b>   : 09-2-9 14:05:07
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-09  [����]
 ******************************************************************************************************/
void ThreadInfo::_SetState(UINT state)
{INTERFUN;
	_m_uiThreadState = state;
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�ȴ��ź����Ĵ�����]
 * @n<b>��������</b>         : _WaitForEvent
 * @n@param SINGLE * single  : [�ź����洢�ṹ��]
 * @param   DWORD    millSec : [�ȴ�ʱ�䣬0Ϊ���Ϸ��أ�INFINTEΪ���޵ȴ�]
 * @return                     [����ֵ����]
 * @see                        [�μ�����]
 * @n<b>����</b>             :alzq
 * @n<b>����ʱ��</b>         : 09-2-9 11:55:30
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-09  [����]
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
		//��Ҫ���LOG��¼
		USES_CONVERSION;
		CStringA mes;
		mes.Format("wait for single name: \"%s\" -- address: 0x%x "
			,W2A(cEvent->_m_csEventName), single);

		ThreadLog::AddLog(this, mes);
	}

	//��ȡ������
	ThreadTreeControl::_LockThreadControlMutex();

	//��ʼ�����̵߳ĵȴ�������Ϣ��ɾ���ȴ�SINGLE���еȣ�
	this->_InitWaitingSituation();

	//�����̵߳ȴ�״̬
	this->_SetWaitStateBySec(millSec);

	//���Ի�ȡ�ź���������ʧ��ʱ�����ܴ��ڣ���ָ����Ч/�ź������ź�
	if(cEvent->_ThreadTryGetEvent(this))
	{
		//�ͷŲ�����
		this->_SetState(THREAD_STATE_SUC);

		ThreadTreeControl::_UnLockThreadControlMutex();

		return WAIT_SINGLE_SUC;
	}
	else
	{
		if(0 == millSec)
		{
			//���ȴ�ʱ��Ϊ0ʱ�����ڻ�ȡ�ź���ʧ�ܣ��������Ϸ��س�ʱ��ȡ
			this->_SetState(THREAD_STATE_TIMEOUT);
			
			ThreadTreeControl::_UnLockThreadControlMutex();

			return WAIT_SINGLE_TIMEOUT;
		}
		else
		{
			//���߳̽ڵ�����ź����ڵ�ȴ��̶߳�����
			cEvent->_AddWaitingThread(this);

			//�����߳�Ϊ�ȴ�״̬
			this->_SetState(THREAD_STATE_WAIT);

			//�ڱ��̵߳ĵȴ��ڵ�����ӵȴ����źŽڵ�
			this->_AddWaitingSingle(single, millSec);

			//�ȴ�ʱ�䲻Ϊ0�����Ϊ���޵ȴ�������ʱ��ȴ��������Ҫ��ȫ�����޵ȴ������г�����ӱ��ڵ�
			//�Ƿ�����ʱ��ȴ��ڱ����������ж�
			ThreadTreeControl::_AddWaitingThread(this, millSec);

			//�����̵߳ȴ�
			this->_TSuspendThread();
		}

		/**
		 *
		 *	---���̱߳��������ִ�еĺ���
		 *
		 **/

		//��ס�̲߳�����
		ThreadTreeControl::_LockThreadControlMutex();

		//���߳�Ϊ����ʱ��ȴ�ʱ��ɾ������ʱ��ȴ��̶߳����б��߳̽ڵ�
		if(_m_uiWaitState == WAIT_TIME_LIMIT)
		{
			ThreadTreeControl::_DelTimeLimitWaitingThread(this);
		}

		//�ڵȴ����ź����еĵȴ�������ɾ�����߳̽ڵ�
		cEvent->_DelWaitingThread(this);

		if(NULL == this->_GetFirstWaitSingle())
		{
			//����������Ҫ�ȴ���SINGLEʱ���������߳��Ѿ��ɹ���ȡ
			this->_SetState(THREAD_STATE_SUC);
		}
		else
		{
			//��Ȼ������Ҫ�ȴ���SINGLEʱ��������ȡ��ʱ
			this->_SetState(THREAD_STATE_TIMEOUT);
		}

		//��ձ��̵߳ȴ�����
		this->_ClearWaitingSingleList();

		//�ͷ��̲߳�����
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
		 *	---����캯��ִ�����
		 *
		 **/
	}
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�ȴ����Ĵ�����]
 * @n<b>��������</b>         : _WaitForMutex
 * @n@param SINGLE * single  : [��ָ��洢�ṹ��]
 * @param   DWORD    millSec : [�ȴ�ʱ�䣬0Ϊ���Ϸ��أ�INFINTEΪ���޵ȴ�]
 * @return                     [����ֵ����]
 * @see                        [�μ�����]
 * @n<b>����</b>             :alzq
 * @n<b>����ʱ��</b>         : 09-2-9 11:56:09
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-09  [����]
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
		//��Ҫ���LOG��¼
		USES_CONVERSION;
		CStringA mes;
		mes.Format("wait for single name: \"%s\" -- address: 0x%x "
			,W2A(cMutex->_m_csMutexName), single);

		ThreadLog::AddLog(this, mes);
	}

	//��ȡ������
	ThreadTreeControl::_LockThreadControlMutex();

	//��ʼ�����̵߳ĵȴ�������Ϣ��ɾ���ȴ�SINGLE���еȣ�
	this->_InitWaitingSituation();
	
	//�����̵߳ȴ�״̬
	this->_SetWaitStateBySec(millSec);

	//���Ի�ȡ�ź���������ʧ��ʱ�����ܴ��ڣ���ָ����Ч/�ź������ź�
	if(cMutex->_ThreadTryLockMutex(this))
	{
		//�����߳̽ڵ��ȡ�������ڵ��������Ӹ�SINGLE
		this->_AddControlMutex(single);

		//��ȡ���ɹ�
		this->_SetState(THREAD_STATE_SUC);

		ThreadTreeControl::_UnLockThreadControlMutex();

		return WAIT_SINGLE_SUC;
	}
	else
	{
		if(0 == millSec)
		{
			//���ȴ�ʱ��Ϊ0ʱ�����ڻ�ȡ��ʧ�ܣ��������Ϸ��س�ʱ��ȡ
			this->_SetState(THREAD_STATE_TIMEOUT);

			ThreadTreeControl::_UnLockThreadControlMutex();

			return WAIT_SINGLE_TIMEOUT;
		}
		else
		{
			//���̼߳������ڵ�ȴ��̶߳�����
			cMutex->_AddWaitingThread(this);

			//�����߳�Ϊ�ȴ�״̬
			this->_SetState(THREAD_STATE_WAIT);

			//�ڱ��̵߳ĵȴ��ڵ�����ӵȴ����źŽڵ�
			this->_AddWaitingSingle(single, millSec);

			//�ȴ�ʱ�䲻Ϊ0�����Ϊ���޵ȴ�������ʱ��ȴ��������Ҫ��ȫ�����޵ȴ������г�����ӱ��ڵ�
			//�Ƿ�����ʱ��ȴ��ڱ����������ж�
			ThreadTreeControl::_AddWaitingThread(this, millSec);

			//�����̵߳ȴ�
			this->_TSuspendThread();
		}

		/**
		 *
		 *	---���̱߳��������ִ�еĺ���
		 *
		 **/

		//��ס�̲߳�����
		ThreadTreeControl::_LockThreadControlMutex();

		//ɾ������ʱ��ȴ��̶߳����б��߳̽ڵ�
		if(_m_uiWaitState == WAIT_TIME_LIMIT)
		{
			ThreadTreeControl::_DelTimeLimitWaitingThread(this);
		}

		//�ڵȴ����ź����еĵȴ�������ɾ�����߳̽ڵ�
		cMutex->_DelWaitingThread(this);

		if(NULL == this->_GetFirstWaitSingle())
		{
			//�����߳̽ڵ��ȡ�������ڵ��������Ӹ�SINGLE
			this->_AddControlMutex(single);

			//����������Ҫ�ȴ���SINGLEʱ���������߳��Ѿ��ɹ���ȡ
			this->_SetState(THREAD_STATE_SUC);
		}
		else
		{
			//��Ȼ������Ҫ�ȴ���SINGLEʱ��������ȡ��ʱ
			this->_SetState(THREAD_STATE_TIMEOUT);
		}
		
		//��ձ��̵߳ȴ�����
		this->_ClearWaitingSingleList();

		//�ͷ��̲߳�����
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
		 *	---�������ִ�����
		 *
		 **/
	}
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [��ȡ��һ���ڵȴ��������ź�����û����һ���򷵻�NULL]
 * @n<b>��������</b>: _GetFirstWaitSingle
 * @return            [����ֵ����]
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-6 16:05:02
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
SINGLE * ThreadInfo::_GetFirstWaitSingle(void)
{INTERFUN;
	if(_m_swlWaitSingleList.empty())
	{
		//�ȴ�����Ϊ��
		return NULL;
	}

	_m_csDealLock->Lock();

	//������黹�ڵȴ���SINGLE
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
 * @brief [������Ҫ����]
 * 
 * [���ȴ��������ź�����ӵ���صĶ�����]
 * @n<b>��������</b>         : _AddWaitingSingle
 * @n@param SINGLE * single  : [��������]
 * @param   DWORD    millSec : [�ȴ�ʱ��INFINTE Ϊ���޵ȴ�]
 * @see                        [�μ�����]
 * @n<b>����</b>             :alzq
 * @n<b>����ʱ��</b>         : 09-2-6 16:39:11
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
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
 * @brief [������Ҫ����]
 * 
 * [�ڱ��߳�ӵ�п���Ȩ�������������ָ�����ڵ�]
 * @n<b>��������</b>        : _AddControlMutex
 * @n@param SINGLE * single : [��������]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-9 16:35:30
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-09  [����]
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
 * @brief [������Ҫ����]
 * 
 * [�ڱ��߳�ӵ�п���Ȩ����������ɾ��ָ�����ڵ�]
 * @n<b>��������</b>        : _DelControlMutex
 * @n@param SINGLE * single : [��������]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-9 16:35:35
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-09  [����]
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

	//������ͬ����ɾ��
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
 * @brief [������Ҫ����]
 * 
 * [��յȴ�����������SINGLE]
 * @n<b>��������</b>: _ClearWaitingSingleList
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-18 15:36:03
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-18  [����]
 ******************************************************************************************************/
void ThreadInfo::_ClearWaitingSingleList(void)
{INTERFUN;
	_m_csDealLock->Lock();

	SINGLEWAITLIST::iterator waitIter = _m_swlWaitSingleList.begin();
	while(waitIter != _m_swlWaitSingleList.end())
	{
		//����SINGLE���������ź����ֱ����߼��ϻ�����ͬ
		SINGLE * single = (*waitIter)->singleTarget;
		if(NULL != single)
		{
			if(SINGLE_EVENT == single->GetType())
			{
				//�ź���
				ThreadEvent * cEvent = (ThreadEvent *)single->GetSingle();

				if(WAIT_STATE_SUC != (*waitIter)->state)
				{
					//δ��ȡ����Ȩ���ڸ�SINGLE�ĵȴ�������ɾ�����߳�
					cEvent->_DelWaitingThread(this);
				}
			}
			else
			{
				//��
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

		//������һ��SINGLE��ɾ�������иýڵ�
		waitIter = _m_swlWaitSingleList.erase(waitIter);
	}

	_m_csDealLock->UnLock();
}
