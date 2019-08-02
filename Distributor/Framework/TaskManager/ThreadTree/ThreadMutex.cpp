#include "StdAfx.h"
#include "ThreadMutex.h"
#pragma warning( disable: 4996 )
ThreadMutex::ThreadMutex(LPVOID controlThread, BOOL initOwner, const CTCHAR * name)
{INTERFUN;
	_m_csDealLock = new DealSectionControl();

	_m_tlWaitThreadList.clear();					//�ȴ��������߳̽ڵ��ַ����

	if(initOwner)
	{
		//��ʼΪ��ȡ״̬
		_m_iLockTime = 1;							//�����ļ�������
		_m_tlControlThread = controlThread;			//��ȡ��������Ȩ���߳̽ڵ��ַ
	}
	else
	{
		_m_iLockTime = 0;							//�����ļ�������������Ϊ�ɿ���״̬ʱ����������Ϊ0
		_m_tlControlThread = NULL;					//��ȡ��������Ȩ���߳̽ڵ��ַ
	}


	//��������
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
	//DWORD				_m_dwTreeID;						//���������ͽṹ�еı��
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
 * @brief [������Ҫ����]
 * 
 * [��ȡ���Ƿ��ܿ���]
 * @n<b>��������</b>: _GetMutexLockEnable
 * @return            [����ֵ����]
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-6 16:10:36
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
BOOL ThreadMutex::_GetMutexLockEnable(void)
{INTERFUN;
	if(_m_iLockTime == 0)
	{
		//������������Ϊ0ʱ����ʾ���Զ������
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [����߳��Ƿ�ӵ�п���Ȩ]
 * @n<b>��������</b>        : _CheckThreadGetControl
 * @n@param LPVOID lpThread : [�߳�ָ��]
 * @return                    [����ֵ����]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-6 16:21:50
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
BOOL ThreadMutex::_CheckThreadGetControl(LPVOID lpThread)
{INTERFUN;
	//���ж��߳�ָ���Ƿ�Ϊ��
	if(NULL == lpThread)
	{
		return FALSE;
	}

	//����ȡ����Ȩ��ָ���Ƿ����ָ�����
	if(_m_tlControlThread == lpThread)
	{
		//���
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�̶߳������������������Ƿ��ȡ���ɹ�]
 * @n<b>��������</b>        : _ThreadTryLockMutex
 * @n@param LPVOID lpThread : [�߳̽ڵ��ַ]
 * @return                    [����ֵ����]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-6 16:25:34
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
BOOL ThreadMutex::_ThreadTryLockMutex(LPVOID lpThread)
{INTERFUN;
	//���ж��߳�ָ���Ƿ�Ϊ��
	if(NULL == lpThread)
	{
		return FALSE;
	}

	//����ź����Ƿ���Ȼ����Դ
	if(this->_GetMutexLockEnable())
	{
		//��Ȼ����Դʱ
		_m_tlControlThread = lpThread;

		//����������1
		_m_iLockTime++;

		return TRUE;
	}
	else
	{
		if(_m_tlControlThread == lpThread)
		{
			//����������1
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
 * @brief [������Ҫ����]
 * 
 * [�ڱ����ĵȴ��̶߳���������߳̽ڵ��ַ]
 * @n<b>��������</b>        : _AddWaitingThread
 * @n@param LPVOID lpThread : [��������]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-6 16:38:40
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
void ThreadMutex::_AddWaitingThread(LPVOID lpThread)
{INTERFUN;
	if(NULL == lpThread)
	{
		return ;
	}

	_m_csDealLock->Lock();

	//���ȴ��̶߳��У�������ָͬ����ɾ���ö��нڵ㣬���ڵȴ�����ĩβ��Ӷ�Ӧ�ڵ�
	THREADLIST::iterator iter = _m_tlWaitThreadList.begin();
	while(iter != _m_tlWaitThreadList.end())
	{
		if((*iter) == lpThread)
		{
			_m_tlWaitThreadList.erase(iter);
		}

		iter++;
	}

	//�ڵȴ����н�β����߳̽ڵ��ַ
	_m_tlWaitThreadList.push_back(lpThread);

	_m_csDealLock->UnLock();
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�ͷ����������ļ���������1����Ҫ������ӵ��Ȩ�����ж�]
 * @n<b>��������</b>        : _ReduceLockTimeMutex
 * @n@param LPVOID lpThread : [��������]
 * @return                    [�����Ƿ��ͷųɹ������̲߳�ӵ�п���Ȩʱ���ͷŲ��ɹ�]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-6 17:02:22
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
BOOL ThreadMutex::_ReduceLockTimeMutex(LPVOID lpThread)
{INTERFUN;
	if(NULL == lpThread)
	{
		return FALSE;
	}

	//�����߳��Ƿ��ȡ�����Ŀ���Ȩ
	if(_m_tlControlThread != lpThread)
	{
		return FALSE;
	}

	//������������1��������Ϊ��ӵ��Ȩʱ�����û�ȡָ��ΪNULL
	_m_iLockTime--;

	if(0 == _m_iLockTime)
	{
		_m_tlControlThread = NULL;
	}

	return TRUE;
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [��ȡ��һ���ڵȴ��������߳̽ڵ��ַ]
 * @n<b>��������</b>: _GetFirstWaitingThread
 * @return            [����ֵ����]
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-6 17:08:54
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
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
 * @brief [������Ҫ����]
 * 
 * [���ȴ��߳̽ڵ�ӱ����ĵȴ��̶߳�����ɾ��]
 * @n<b>��������</b>        : _DelWaitingThread
 * @n@param LPVOID lpThread : [��������]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-6 17:11:19
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
void ThreadMutex::_DelWaitingThread(LPVOID lpThread)
{INTERFUN;
	if(NULL == lpThread)
	{
		return ;
	}

	if(_m_tlWaitThreadList.empty())
	{
		//�ȴ��̶߳���Ϊ��
		return ;
	}

	_m_csDealLock->Lock();

	//����������ͬ�߳�ָ��
	THREADLIST::iterator iter = _m_tlWaitThreadList.begin();
	while(iter != _m_tlWaitThreadList.end())
	{
		if((*iter) != lpThread)
		{
			iter++;
		}
		else
		{
			//��ָ����ͬʱɾ����Ӧ���нڵ�
			_m_tlWaitThreadList.erase(iter);
			_m_csDealLock->UnLock();
			return ;
		}
	}

	_m_csDealLock->UnLock();
}
