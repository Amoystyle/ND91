#include "StdAfx.h"
#include "ThreadEvent.h"
#pragma warning( disable: 4996 )
ThreadEvent::ThreadEvent(BOOL autoReSet, BOOL initState, const CTCHAR * name)
{INTERFUN;
	_m_csDealLock = new DealSectionControl();

	_m_tlWaitThreadList.clear();				//�ȴ��������߳̽ڵ��ַ����

	_m_iInitSourceCount = 1;				//��ʼ������Դ����

	if(initState)
	{
		//initStateΪTRUE��ʾ��ʼ���ź���Ϊ���ź�״̬
		_m_iSourceCount = 1;					//�ź�����ʣ����Դ��ͳ�ƣ�����0Ϊ�ɻ�ȡ״̬
	}
	else
	{
		_m_iSourceCount = 0;					//�ź�����ʣ����Դ��ͳ�ƣ�����0Ϊ�ɻ�ȡ״̬
	}

	//�ź���������
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

	_m_bAutoReset = autoReSet;						//�Ƿ��Զ��ı�״̬���ź���

//private:
	//DWORD				_m_dwTreeID;						//���������ͽṹ�еı��
}

ThreadEvent::~ThreadEvent(void)
{INTERFUN;
	_m_csDealLock->Lock();

	_m_tlWaitThreadList.clear();				//�ȴ��������߳̽ڵ��ַ����

	if(NULL != _m_csEventName)
	{
		delete [] _m_csEventName;
		_m_csEventName = NULL;
	}

	_m_csDealLock->UnLock();

	SAFE_DELETE(_m_csDealLock);
}

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [��ȡ�ź����Ƿ�Ϊ���ź�״̬]
 * @n<b>��������</b>: _GetEventSingleEnable
 * @return            [����ֵ����]
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-6 16:10:36
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
BOOL ThreadEvent::_GetEventSingleEnable(void)
{INTERFUN;
	if(this->_m_iSourceCount > 0)
	{
		//����Դͳ�ƴ���0ʱ
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
 * [�̳߳��Ի�ȡ�źţ������Ƿ��ȡ�ɹ���]
 * @n<b>��������</b>        : _ThreadTryGetEvent
 * @n@param LPVOID lpThread : [�߳̽ڵ��ַ]
 * @return                    [����ֵ����]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-6 16:25:34
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
BOOL ThreadEvent::_ThreadTryGetEvent(LPVOID lpThread)
{INTERFUN;
	//���ж��߳�ָ���Ƿ�Ϊ��
	if(NULL == lpThread)
	{
		return FALSE;
	}

	//����ź����Ƿ���Ȼ����Դ
	if(this->_GetEventSingleEnable())
	{
		//��Ȼ����Դʱ
		if(_m_bAutoReset)
		{
			//��Ҫ�Զ������ź���ʱ
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
 * @brief [������Ҫ����]
 * 
 * [�ڱ��ź����ĵȴ��̶߳���������߳̽ڵ��ַ]
 * @n<b>��������</b>        : _AddWaitingThread
 * @n@param LPVOID lpThread : [��������]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-6 16:38:40
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
void ThreadEvent::_AddWaitingThread(LPVOID lpThread)
{INTERFUN;
	//���ж��߳�ָ���Ƿ�Ϊ��
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
 * [�ͷ��ź��������ź�������Դͳ�Ƽ�1��]
 * @n<b>��������</b>        : _ReleaseEvent
 * @return                    [�����Ƿ��ͷųɹ������̲߳�ӵ�п���Ȩʱ���ͷŲ��ɹ�]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-6 17:02:22
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
BOOL ThreadEvent::_ReleaseEvent(void)
{INTERFUN;
	if(_m_iSourceCount < _m_iInitSourceCount)
	{
		//����Դͳ��С�ڳ�ʼ������Դͳ��ʱ
		_m_iSourceCount++;
		return TRUE;
	}

	return FALSE;
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
LPVOID ThreadEvent::_GetFirstWaitingThread(void)
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
void ThreadEvent::_DelWaitingThread(LPVOID lpThread)
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
