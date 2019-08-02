#include "StdAfx.h"
#include "ThreadTreeControl.h"
#include "ThreadLog.h"
#include "process.h"

/**
 *	--�����ڱ����������ͷ�ļ�
 **/
//int ThreadTreeControl::_g_iThreadExitTimeLimit = INFINITE;						//�߳��˳�ʱ��ʱ������
//CRITICAL_SECTION ThreadTreeControl::_g_csThreadControlSection;		//�߳̽ڵ������
//CRITICAL_SECTION ThreadTreeControl::_g_csSingleControlSection;		//�ź����������в�����
//HANDLE ThreadTreeControl::_g_hTimeThreadEvent = NULL;						//��ʱѭ���ź�
//HANDLE ThreadTreeControl::_g_hTimeThreadFinishEvent = NULL;				//����˳���ʱ�߳�ʱ���źŵ��ź���
//EventList ThreadTreeControl::_g_elNamedEventList;					//�Ѿ����������ź����ڵ��б�
//MutexList ThreadTreeControl::_g_mlNamedMutexList;					//�Ѿ������������ڵ��б�
//SINGLELIST ThreadTreeControl::_g_slSingleList;						//�����߳�Ϊ�յ�SINGLE����
//RUNNINGTHREADLIST ThreadTreeControl::_g_rtlThreadList;				//�����˵��߳̽ڵ�洢����
//WAITINGTHREADLIST ThreadTreeControl::_g_wtlTimeLimitWaitingThread;	//����ʱ��ȴ��̶߳���
//TreeItem_t * ThreadTreeControl::_g_treThreadTree = NULL;					//�߳����ĸ��ڵ�
//DeleteThreadInfo * ThreadTreeControl::_g_dtiDeleteThreadInfo;			//��Ҫɾ�����߳̽ڵ��ַ
//CRuntimeClass * ThreadTreeControl::_g_lpUIWndClass;
//CreateUIWndInfo * ThreadTreeControl::_g_lpUIWndInfo;
ThreadTreeGlobalInfo * ThreadTreeControl::_g_giGlobalInfo = NULL;

bool ThreadTreeControl::_g_bLogSingleCreate;					//�Ƿ����Single������¼
bool ThreadTreeControl::_g_bLogSetSingle;						//�Ƿ���������ź�����¼
bool ThreadTreeControl::_g_bLogWaitObj;							//�Ƿ�����ȴ���¼

/**
 *
 *	---�Զ�����̺߳���
 *
 **/
unsigned int WINAPI ThreadTreeControl::_CustomThread(LPVOID lpParameter)
{INTERFUN;
	bool threadError = false;				//�߳�ִ���Ƿ���ִ���

	if(NULL == lpParameter)
	{
		//����ղ���
#ifdef _DEBUG
		DebugBreak();
#endif

		threadError = true;
	}
	
	CUSTOMTHREADINFO * customThreadInfo = (CUSTOMTHREADINFO *)lpParameter;	//�����߳�ִ����ز���

	//��ȡ�̱߳�ʶID
	ThreadInfo * info = customThreadInfo->threadInfo;

	//��ȡ������
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
				//�û�����ָ��Ϊ��
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
			//����ִ�г���
			ThreadLog::AddLog(info, "error finish!");
#ifdef _DEBUG
			DebugBreak();
#endif

			threadError = true;
		}
#endif
	}

	//ɾ���߳̽ڵ����������Ƿ����δ�ͷ���
	if(threadError)
	{
		//�߳�ִ�й����г��ֹ�����
	}
	else
	{
		//����ִ�н���
	}

	//ɾ�����ڵ㣬��ɾ���ӽڵ㣬�������߳��й�
	_DisposeThread(info);

	//DeleteThread(customThreadInfo->threadInfo, FALSE);

	//���ý����ź�
	_SetThreadFinish(info);

	::_endthreadex(0);
	return 0;
} 
	

/**
 *
 *	---��ʱ�����Ƶ��̹߳����߳�
 *
 **/
DWORD WINAPI ThreadTreeControl::_TimeLimitThreadControlThread(LPVOID lpParameter)
{INTERFUN;
	//��ȡ�����ź�����ɾ��ָ��
	TimeControlEvents * events = (TimeControlEvents *)lpParameter;
	HANDLE timeEvent = events->timeEvent;
	HANDLE finishEvent = events->finishEvent;
	int printTimeCount = 0;
	SAFE_DELETE(events);

	ThreadLog::AddLog("Start time limit thread control thread...");
	//ѭ��10���룬ÿ�ζ���ʱ�����Ƶ��߳̽��м�⣬����ʱ�߳�����״̬�����¼���
	while(WaitForSingleObject(timeEvent, 10) == WAIT_TIMEOUT)
	{
		//����ʱʱ����������ʱ��ȴ��̶߳��н��д���
		_DealThreadTimeCount();

		/**
		 *	--��ʱ����ڴ��߳���Ϣ�������֣�ʱ����Ҫ*10����
		 **/
		printTimeCount++;

		if(printTimeCount >= 1000)
		{
			//60�����һ��
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
 *	---����ɾ���̵߳Ĺ����߳�,����ÿ���߳������Ŀ���Ȩ������ȡ��ʱ��ɾ��
 *
 **/
DWORD WINAPI ThreadTreeControl::_ThreadDeleteThread(LPVOID lpParameter)
{INTERFUN;
	DeleteThreadInfo * info = (DeleteThreadInfo *)lpParameter;
	HANDLE timeEvent = info->timeEvent;
	HANDLE finishEvent = info->finishEvent;
	
	//��ʱѭ�����ɴ���ṹ���б��������Ƿ��˳�ѭ��
	ThreadLog::AddLog("Start thread delete thread...");
	while(!info->exitThread || !info->threadList.empty())
	{
		//��ʱ����
		//ÿ1�����һ��ѭ�����
		WaitForSingleObject(timeEvent, 1000);

		//ѭ�����ÿ���̵߳Ľ�������������ɾ��ʱ����ɾ��
		_TestAndDeleteThread();
	}
	ThreadLog::AddLog("Finish thread delete thread...");

	//���ý����ź�Ϊ���ź�
	SetEvent(finishEvent);

	return 0;
}
	


ThreadTreeControl::ThreadTreeControl(void)
{INTERFUN;
	//_g_treThreadTree = NULL;

	////_g_hThreadControlMutex = NULL;				//�߳̽ڵ������
	////_g_hSingleControlMutex = NULL;				//�ź����������в�����
	////_g_csThreadControlSection = NULL;				//�߳̽ڵ������
	////_g_csSingleControlSection = NULL;				//�ź����������в�����
	//_g_hTimeThreadEvent = NULL;						//��ʱѭ���ź�
	//_g_hTimeThreadFinishEvent = NULL;				//����˳���ʱ�߳�ʱ���źŵ��ź���

	//_g_elNamedEventList.clear();					//�Ѿ����������ź����ڵ��б�
	//_g_mlNamedMutexList.clear();					//�Ѿ������������ڵ��б�
	//_g_slSingleList.clear();
	//_g_wtlTimeLimitWaitingThread.clear();			//����ʱ��ȴ��̶߳���
}

ThreadTreeControl::~ThreadTreeControl(void)
{INTERFUN;
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�߳����������ĳ�ʼ������]
 * @n<b>��������</b>: InitThreadTreeControl
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-11 10:26:39
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-11  [����]
 ******************************************************************************************************/
void ThreadTreeControl::InitThreadTreeControl(const CTCHAR * logPath)
{INTERFUN;
	if(NULL == _g_giGlobalInfo)
	{
		_g_giGlobalInfo = new ThreadTreeGlobalInfo();

		ThreadLog::InitThreadLog(logPath);

		size_t len = _tcslen(logPath);
		_g_giGlobalInfo->_g_sAliveThreadFile = new CTCHAR[len + 2];	//��׺��L������\0
		_tcsncpy_s(_g_giGlobalInfo->_g_sAliveThreadFile, len + 2, logPath, len);
		_g_giGlobalInfo->_g_sAliveThreadFile[len] = _T('L');
		_g_giGlobalInfo->_g_sAliveThreadFile[len + 1] = _T('\0');

		//�жϸ��ڵ��Ƿ���ڣ��������򴴽�
		if(NULL == _g_giGlobalInfo->_g_treThreadTree)
		{
			//�������ڵ��������Ϣ��ΪNULL
			CUSTOMTHREADINFO * customThread = new CUSTOMTHREADINFO();
			customThread->userFunc = NULL;
			customThread->threadInfo = new ThreadInfo(NULL, NULL, NULL, NULL);

			//�����߳̽ڵ��е��߳���Ϣ
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
			//�����ڵ㲻Ϊ�ձ�ʾ�Ѿ������˳�ʼ�������ֱ�ӷ���
			return ;
		}

		_g_giGlobalInfo->_g_iThreadExitTimeLimit = INFINITE;

		//_g_hThreadControlMutex = CreateMutex(NULL, FALSE, NULL);		//�߳̽ڵ������
		//_g_hSingleControlMutex = CreateMutex(NULL, FALSE, NULL);		//�߳̽ڵ������
		//InitializeCriticalSection(&_g_giGlobalInfo->_g_csThreadControlSection);			//�߳̽ڵ������
		//InitializeCriticalSection(&_g_giGlobalInfo->_g_csSingleControlSection);			//�ź����������в�����
		_g_giGlobalInfo->_g_csThreadControlMutex = ::CreateMutex(NULL, FALSE, NULL);		//�߳̽ڵ������
		_g_giGlobalInfo->_g_csSingleControlMutex = ::CreateMutex(NULL, FALSE, NULL);;		//�ź����������в�����
		_g_giGlobalInfo->_g_hTimeThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		_g_giGlobalInfo->_g_hTimeThreadFinishEvent = CreateEvent(NULL, FALSE, FALSE, NULL);			//����˳���ʱ�߳�ʱ���źŵ��ź���

		_g_giGlobalInfo->_g_elNamedEventList.clear();					//�Ѿ����������ź����ڵ��б�
		_g_giGlobalInfo->_g_mlNamedMutexList.clear();					//�Ѿ������������ڵ��б�
		_g_giGlobalInfo->_g_slSingleList.clear();
		_g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.clear();			//����ʱ��ȴ��̶߳���

		TimeControlEvents * events = new TimeControlEvents();
		events->timeEvent = _g_giGlobalInfo->_g_hTimeThreadEvent;
		events->finishEvent = _g_giGlobalInfo->_g_hTimeThreadFinishEvent;

		_g_giGlobalInfo->_g_dtiDeleteThreadInfo = new DeleteThreadInfo();
		
	#ifdef _DEBUG
		_g_bLogSingleCreate = true;					//�Ƿ�����߳̿�������ȡ����
		_g_bLogSetSingle = true;						//�Ƿ�����̶߳�SINGLE����ĵȴ�����ǰ��
		_g_bLogWaitObj = true;							//�Ƿ��������ͻָ��߳�
	#else
		_g_bLogSingleCreate = true;					//�Ƿ�����߳̿�������ȡ����
		_g_bLogSetSingle = false;						//�Ƿ�����̶߳�SINGLE����ĵȴ�����ǰ��
		_g_bLogWaitObj = false;							//�Ƿ��������ͻָ��߳�
	#endif

		::CreateThread(NULL, NULL, ThreadTreeControl::_TimeLimitThreadControlThread, events, NULL, NULL);
		::CreateThread(NULL, NULL, ThreadTreeControl::_ThreadDeleteThread, _g_giGlobalInfo->_g_dtiDeleteThreadInfo, NULL, NULL);

//		_g_giGlobalInfo->_g_lpUIWndClass = NULL;
//		_g_giGlobalInfo->_g_lpUIWndInfo = NULL;
	}
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�߳�������������������]
 * @n<b>��������</b>: DestroyThreadTreeControl
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-11 10:38:23
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-11  [����]
 ******************************************************************************************************/
void ThreadTreeControl::DestroyThreadTreeControl(void)
{INTERFUN;
	//�жϸ��ڵ��Ƿ���ڣ��������򴴽�
	if(NULL != _g_giGlobalInfo)
	{
		//�ر������߳�
		ThreadLog::AddLog("Start destroy all thread...\n");

		//����ɾ�����ڵ��µ������߳̽ڵ�
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

			//ɾ���������нڵ�
			DeleteThread(_g_giGlobalInfo->_g_treThreadTree, TRUE);
		}
		else
		{
			//�����ڵ�Ϊ�ձ�ʾδ������ʼ�������ֱ�ӷ���
			return ;
		}

		_g_giGlobalInfo->_g_treThreadTree = NULL;

		ThreadLog::AddLog("Finish time thread ...\n");
		//�رն�ʱ�߳�
		SetEvent(_g_giGlobalInfo->_g_hTimeThreadEvent);
		WaitForSingleObject(_g_giGlobalInfo->_g_hTimeThreadFinishEvent, INFINITE);

		ThreadLog::AddLog("Finish delete thread ...\n");
		_g_giGlobalInfo->_g_dtiDeleteThreadInfo->exitThread = true;
		WaitForSingleObject(_g_giGlobalInfo->_g_dtiDeleteThreadInfo->finishEvent, INFINITE);

		ThreadLog::AddLog("Finish destroy all thread...\n");

		//ɾ�����д����߳�ΪNULL��SINGLE
		ThreadLog::AddLog("Start destroy all single...\n");

		_LockSingleControlMutex();

#ifdef _DEBUG
		//DEBUGģʽ����ʾ��SINGLEδɾ��
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
					//��Ҫ��SINGLE����������������
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
					//��Ҫ��SINGLE����������������
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
		
		//�������ƵĴ洢�������
		_g_giGlobalInfo->_g_elNamedEventList.clear();					//�Ѿ����������ź����ڵ��б�
		_g_giGlobalInfo->_g_mlNamedMutexList.clear();					//�Ѿ������������ڵ��б�
		
		_UnLockSingleControlMutex();
		ThreadLog::AddLog("Finish destroy all single...\n");

		//CloseHandle(_g_hThreadControlMutex);		//�߳̽ڵ������
		//CloseHandle(_g_hSingleControlMutex);		//�߳̽ڵ������
		//DeleteCriticalSection(&_g_giGlobalInfo->_g_csThreadControlSection);			//�߳̽ڵ������
		//DeleteCriticalSection(&_g_giGlobalInfo->_g_csSingleControlSection);			//�ź����������в�����
		::CloseHandle(_g_giGlobalInfo->_g_csThreadControlMutex);		//�߳̽ڵ������
		::CloseHandle(_g_giGlobalInfo->_g_csSingleControlMutex);		//�ź����������в�����
		CloseHandle(_g_giGlobalInfo->_g_hTimeThreadEvent);
		CloseHandle(_g_giGlobalInfo->_g_hTimeThreadFinishEvent);
		_g_giGlobalInfo->_g_elNamedEventList.clear();							//�Ѿ����������ź����ڵ��б�
		_g_giGlobalInfo->_g_mlNamedMutexList.clear();							//�Ѿ������������ڵ��б�
		_g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.clear();					//����ʱ��ȴ��̶߳���

		_g_giGlobalInfo->_g_treThreadTree = NULL;					//�߳����ĸ��ڵ�
		
		SAFE_DELETE(_g_giGlobalInfo->_g_dtiDeleteThreadInfo);

		SAFE_DELETE(_g_giGlobalInfo);

		ThreadLog::CloseThreadLog();
	}
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�ͷ��ź�����������]
 * @n<b>��������</b>        : TCloseSingle
 * @n@param LPSINGLE single : [��������]
 * @return                    [����ֵ����]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-17 14:29:10
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-17  [����]
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
			//��ѯ���û���Ҫɾ����SINGLE
			LPSINGLE lpSingle = (*iter);

			//��SINGLE������ʱ���ڶ�Ӧ�����ƵĶ�����ɾ��
			if(SINGLE_EVENT == lpSingle->GetType())
			{
				//�ź���
				ThreadEvent * cEvent = (ThreadEvent *)lpSingle->GetSingle();

				if(NULL != cEvent->_m_csEventName)
				{
					//�������ƵĶ�����ɾ�����ź���
					EventList::iterator eventIter = _g_giGlobalInfo->_g_elNamedEventList.begin();
					while(eventIter != _g_giGlobalInfo->_g_elNamedEventList.end())
					{
						if((*eventIter) == lpSingle)
						{
							//�������Ƶ��ź���������ɾ�����ź���
							_g_giGlobalInfo->_g_elNamedEventList.erase(eventIter);
							break;
						}

						eventIter++;
					}
				}

				if(_g_bLogSingleCreate)
				{
					//��Ҫ��SINGLE����������������
					//��ѯ��ǰ�߳����ڽڵ�
					ThreadInfo * threadInfo = _FindRunningThread(::GetCurrentThreadId());

					USES_CONVERSION;
					CStringA mes;
					mes.Format("close event name: \"%s\" -- address: 0x%x "
						,W2A(cEvent->_m_csEventName), single);

					ThreadLog::AddLog(threadInfo, mes);
				}

				//ɾ������
				SAFE_DELETE(cEvent);
			}
			else
			{
				//��
				ThreadMutex * cMutex = (ThreadMutex *)lpSingle->GetSingle();

				if(NULL != cMutex->_m_csMutexName)
				{
					//�������ƵĶ�����ɾ�����ź���
					EventList::iterator mutexIter = _g_giGlobalInfo->_g_mlNamedMutexList.begin();
					while(mutexIter != _g_giGlobalInfo->_g_mlNamedMutexList.end())
					{
						if((*mutexIter) == lpSingle)
						{
							//�������Ƶ��ź���������ɾ�����ź���
							_g_giGlobalInfo->_g_mlNamedMutexList.erase(mutexIter);
							break;
						}

						mutexIter++;
					}
				}
			
				if(_g_bLogSingleCreate)
				{
					//��Ҫ��SINGLE����������������
					//��ѯ��ǰ�߳����ڽڵ�
					ThreadInfo * threadInfo = _FindRunningThread(::GetCurrentThreadId());

					USES_CONVERSION;
					CStringA mes;
					mes.Format("close mutex name: \"%s\" -- address: 0x%x "
						,W2A(cMutex->_m_csMutexName), single);

					ThreadLog::AddLog(threadInfo, mes);
				}

				//��ӵ�д�������Ȩ���߳��ڵ��������ڵ�ɾ��
				if(NULL != cMutex->_m_tlControlThread)
				{
					((ThreadInfo *)(cMutex->_m_tlControlThread))->_DelControlMutex(single);
				}

				//ɾ������
				SAFE_DELETE(cMutex);
			}

			//ɾ��SINGLE�ṹ��
			SAFE_DELETE(*iter);

			//��SINGLE������ɾ��
			_g_giGlobalInfo->_g_slSingleList.erase(iter);
			break;
		}

		iter++;
	}

	_UnLockSingleControlMutex();

	return TRUE;
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [ʹ�ø��ڵ��ͷ���]
 * @n<b>��������</b>        : TReleaseMutex
 * @n@param SINGLE * single : [��������]
 * @see                       [�μ�����]
 * @n<b>����</b>            :alzq
 * @n<b>����ʱ��</b>        : 09-2-13 16:13:23
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-13  [����]
 ******************************************************************************************************/
UINT ThreadTreeControl::TReleaseMutex(SINGLE * single)
{INTERFUN;
	if(NULL == _g_giGlobalInfo->_g_treThreadTree)
	{
		return RELEASE_MUTEX_ERROR;
	}


	//��ѯ��ǰ�߳����ڽڵ�
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
 * @brief [������Ҫ����]
 * 
 * [ʹ�ø��ڵ�ȴ��ź�������]
 * @n<b>��������</b>         : WaitForSingle
 * @n@param SINGLE * single  : [��������]
 * @param   DWORD    millSec : [��������]
 * @return                     [����ֵ����]
 * @see                        [�μ�����]
 * @n<b>����</b>             :alzq
 * @n<b>����ʱ��</b>         : 09-2-13 16:16:50
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-13  [����]
 ******************************************************************************************************/
UINT ThreadTreeControl::TWaitForSingle(SINGLE * single, DWORD millSec)
{INTERFUN;
	if(NULL == _g_giGlobalInfo->_g_treThreadTree)
	{
		return WAIT_SINGLE_ERROR;
	}


	//��ѯ��ǰ�߳����ڽڵ�
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
 * @brief [������Ҫ����]
 * 
 * [�����߳̽ڵ㣬�����ڵ�����������]
 * @n<b>��������</b>                              : CreateThread
 * @n@param ThreadControl *       control         : [���Դ洢�߳̽ڵ���Ϣ���߳̿��ƶ���]
 * @param   LPSECURITY_ATTRIBUTES lpsa            : [�ο�API]
 * @param   DWORD                 dwStackSize     : [�ο�API]
 * @param   USER_FUNCTION         pfnThreadProc   : [�û������壬��API��ͬ��������������߳̽ڵ�Ŀ�����]
 * @param   LPVOID                lParam          : [�û���Ҫ�����߳��еĲ���]
 * @param   DWORD                 dwCreationFlags : [�ο�API]
 * @param   DWORD *               pdwThreadId     : [�ο�API]
 * @return                                          [����ֵ����]
 * @see                                             [�μ�����]
 * @n<b>����</b>                                  :alzq
 * @n<b>����ʱ��</b>                              : 09-2-5 14:53:07
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-05  [����]
 ******************************************************************************************************/
bool ThreadTreeControl::TCreateThread(ThreadControl * control, LPSECURITY_ATTRIBUTES lpsa, DWORD dwStackSize
									  , USER_FUNCTION pfnThreadProc, DELETE_THREAD_FUNCTION exitThread
									  , LPVOID lParam, DWORD dwCreationFlags, DWORD * pdwThreadId, char * name)
{INTERFUN;
	_LockThreadControlMutex();

	//���������߳��ڲ�����Ϣ
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
		//����ȡ�����߳̾��Ϊ�ձ�ʾ����ʧ��
		delete customThread->threadInfo;
		customThread->threadInfo = NULL;
		SAFE_DELETE(customThread);

		_UnLockThreadControlMutex();
		return false;
	}

	//�����߳�ID
	customThread->threadInfo->_m_ttThreadTarget.threadId = threadID;
	
	ThreadLog::AddLog(customThread->threadInfo, "create thread suc");

	TreeItem_t * parent = NULL;

	//���븸�ڵ�Ϊ��ʱ��ʹ�ø��ڵ�
	//parent = _g_treThreadTree;
	ThreadInfo * parentThread = _FindRunningThread(::GetCurrentThreadId());
	if(NULL == parentThread)
	{
		//���߳̿����߳�
		parent = _g_giGlobalInfo->_g_treThreadTree;
	}
	else
	{
		parent = (TreeItem_t *)parentThread->_m_lpTreeItem;
	}

	//�����߳̽ڵ��е��߳���Ϣ
	customThread->threadInfo->_m_ttThreadTarget.isWinThread = false;
	customThread->threadInfo->_m_ttThreadTarget.handle = threadHandle;

	//�������ڵ��������Ϣ��ΪNULL

	//�����洢�����е���Ϣ
	Data_t date;
	//���߳���Ϣ�������нڵ���Ϣ��
	date.customThreadInfo = customThread;
	//�ڸ��׽ڵ��´����ӽڵ㲢���������Ϣ
	TreeItem_t * newThread = AppendTreeItem(parent, &date);
	if(NULL != control)
	{
		if(NULL != control->_m_tiThreadInfo)
		{
			//��CONTROL�����Ѿ��п��Ƶ��߳�ʱ����Ҫ��ԭ�������̵߳�CONTROL��������Ϊ��
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
 * @brief [������Ҫ����]
 * 
 * [�����߳̽ڵ�ļ򵥷�ʽ]
 * @n<b>��������</b>                      : CreateThread
 * @n@param ThreadControl * control       : [���Դ洢�߳̽ڵ���Ϣ���߳̿��ƶ���]
 * @param   TreeItem_t *    parentThread  : [�����̵߳Ľڵ��ַ]
 * @param   USER_FUNCTION   pfnThreadProc : [������ָ��]
 * @param   LPVOID          lParam        : [�������]
 * @see                                     [�μ�����]
 * @n<b>����</b>                          :alzq
 * @n<b>����ʱ��</b>                      : 09-2-5 14:58:52
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-05  [����]
 ******************************************************************************************************/
bool ThreadTreeControl::TCreateThread(ThreadControl * control, USER_FUNCTION pfnThreadProc, DELETE_THREAD_FUNCTION exitThread, LPVOID lParam, char * name)
{INTERFUN;
	return TCreateThread(control, NULL, 0, pfnThreadProc, exitThread, lParam, 0, NULL, name);
}
	


/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�������ڵ�]
 * @n<b>��������</b>                : TCreateMutex
 * @n@param ThreadInfo *  thread    : [�����ýڵ���߳���Ϣ�ڵ��ַ]
 * @param   BOOL          initState : [��ʼ����״̬��TRUEΪ���źţ�FALSEΪ���ź�]
 * @param   const WCHAR * name      : [�����ƣ�Ĭ��Ϊ��]
 * @return                            [����ֵ����]
 * @see                               [�μ�����]
 * @n<b>����</b>                    :alzq
 * @n<b>����ʱ��</b>                : 09-2-6 9:35:35
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
LPSINGLE ThreadTreeControl::TCreateMutex(BOOL initOwner, const CTCHAR * name)
{INTERFUN;
	if(NULL == _g_giGlobalInfo->_g_treThreadTree)
	{
		return NULL;
	}

	//��ѯ��ǰ�߳����ڽڵ�
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
			//��Ҫ��SINGLE����������������
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
 * @brief [������Ҫ����]
 * 
 * [�����źŽڵ�]
 * @n<b>��������</b>                 : TCreateEvent
 * @n@param ThreadInfo *  threadInfo : [�����ýڵ���߳���Ϣ�ڵ��ַ]
 * @param   BOOL          autoReSet  : [�Ƿ��Զ�ת���ź�״̬]
 * @param   BOOL          initState  : [��ʼ״̬]
 * @param   const WCHAR * name       : [�ź������ƣ�Ĭ��Ϊ��]
 * @return                             [����ֵ����]
 * @see                                [�μ�����]
 * @n<b>����</b>                     :alzq
 * @n<b>����ʱ��</b>                 : 09-2-6 9:44:15
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
LPSINGLE ThreadTreeControl::TCreateEvent(BOOL autoReSet, BOOL initState, const CTCHAR * name)
{INTERFUN;
	if(NULL == _g_giGlobalInfo->_g_treThreadTree)
	{
		return NULL;
	}

	//��ȡ��ǰ�����߳�HANDLE�����ڴ��в�ѯ����߳̽ڵ�
	LPSINGLE single = _TCreateEvent(autoReSet, initState, name);

	if(_g_bLogSingleCreate)
	{
		//��Ҫ��SINGLE����������������
		//��ѯ��ǰ�߳����ڽڵ�
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
 * @brief [������Ҫ����]
 * 
 * [�����ź���Ϊ���ź�״̬]
 * @n<b>��������</b>                : TSetEventSingle
 * @param   SINGLE *     single     : [�ź����Ľڵ��ַ]
 * @return                            [����ֵ����]
 * @see                               [�μ�����]
 * @n<b>����</b>                    :alzq
 * @n<b>����ʱ��</b>                : 09-2-6 15:14:36
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
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
		//���SINGLE�����ǲ�����
#ifdef _DEBUG
		DebugBreak();
#endif

		return SET_EVENT_ERROR;
	}

	//��ȡSINGLE�е����ڵ�
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
		//��Ҫ��SINGLE����������������
		//��ѯ��ǰ�߳����ڽڵ�
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

	//���Խ��ź������źż�1
	if(cEvent->_ReleaseEvent())
	{
		//�����ɹ�

		//��ȡ����ʱ��ȴ��̶߳��в�����
		ThreadTreeControl::_LockThreadControlMutex();

		//�ж��ź����Ƿ�Ϊ���ź�״̬
		while(cEvent->_GetEventSingleEnable())
		{
			//���ź�״̬ʱ���ϻ�ȡ��һ���ȴ��߳̽ڵ㣬����������һ���ȴ��߳̽ڵ�ʱ����


			//��ȡ��һ���ȴ��źŵ��߳̽ڵ�
			ThreadInfo * nextThread = (ThreadInfo *)cEvent->_GetFirstWaitingThread();

			if(NULL == nextThread)
			{
				//�ͷ�����ʱ��ȴ��̶߳��в�����
				ThreadTreeControl::_UnLockThreadControlMutex();
				return SET_EVENT_SUC;
			}
			else
			{
				//������һ���ȴ��̵߳�ִ�У������ڼ���

				//ʹ����һ�̶߳������м�����˳������������ӵ���̣߳�
				if(cEvent->_ThreadTryGetEvent(nextThread))
				{
					//�����ɹ�
					cEvent->_DelWaitingThread(nextThread);

					//�����߳��и�����״̬Ϊ�ѻ�ȡ
					nextThread->_SetSingleState(single, WAIT_STATE_SUC);

					//�����߳�ִ��
					nextThread->_TResumeThread();
				}
			}
		}

		//�ͷ�����ʱ��ȴ��̶߳��в�����
		ThreadTreeControl::_UnLockThreadControlMutex();
		return SET_EVENT_SUC;
	}
	else
	{
		return SET_EVENT_FAIL;
	}
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�����ź���Ϊ���ź�״̬]
 * @n<b>��������</b>                : TSetEventNoSingle
 * @param   SINGLE *     single     : [�ź����Ľڵ��ַ]
 * @return                            [����ֵ����]
 * @see                               [�μ�����]
 * @n<b>����</b>                    :alzq
 * @n<b>����ʱ��</b>                : 09-2-6 15:15:05
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
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
		//SINGLE���Ͳ���EVENTʱ
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
		//��Ҫ��SINGLE����������������
		//��ѯ��ǰ�߳����ڽڵ�
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

	//�����������ź������ź�ʱ�������������ĵȴ��������ж��������໥Ӱ�죬������Ҫ��ȡ��������
	//��ȡ����ʱ��ȴ��̶߳��в�����
	ThreadTreeControl::_LockThreadControlMutex();

	cEvent->_m_iSourceCount = 0;

	//�ͷ�����ʱ��ȴ��̶߳��в�����
	ThreadTreeControl::_UnLockThreadControlMutex();

	return SET_EVENT_SUC;
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [ɾ�����ڵ㣬�Լ����ӽڵ�]
 * @n<b>��������</b>                     : DeleteThread
 * @n@param TreeItem_t * treeItem        : [��������]
 * @see                                    [�μ�����]
 * @n<b>����</b>                         :alzq
 * @n<b>����ʱ��</b>                     : 09-2-18 9:17:13
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-18  [����]
 ******************************************************************************************************/
void ThreadTreeControl::DeleteThread(TreeItem_t * treeItem, BOOL delChild, DWORD delayTime)
{INTERFUN;
	if(NULL == treeItem)
	{
		return ;
	}

	if(delChild)
	{
		//ɾ���ӽڵ�����
		_DestroyThreadTreeChild(treeItem);
	}

	//ɾ������
	_DisposeTreeItem(treeItem, delayTime);
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [ɾ��ָ���߳��Լ������߳�]
 * @n<b>��������</b>          : DeleteThread
 * @n@param ThreadInfo * info : [��������]
 * @see                         [�μ�����]
 * @n<b>����</b>              :alzq
 * @n<b>����ʱ��</b>          : 09-2-13 14:26:40
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-13  [����]
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
 * @brief [������Ҫ����]
 * 
 * [ɾ�����ô˺������̣߳����߳���Ч]
 * @n<b>��������</b>      : DeleteThread
 * @n@param BOOL delChild : [��������]
 * @see                     [�μ�����]
 * @n<b>����</b>          :alzq
 * @n<b>����ʱ��</b>      : 09-2-23 15:28:33
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-23  [����]
 ******************************************************************************************************/
void ThreadTreeControl::DeleteThread(BOOL delChild, DWORD delayTime)
{INTERFUN;
	ThreadInfo * threadInfo = _FindRunningThread(::GetCurrentThreadId());

	DeleteThread(threadInfo, delChild, delayTime);
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�ͷ��߳������Դ�������߳���ӵ�ɾ��������]
 * @n<b>��������</b>          : _DisposeThread
 * @n@param ThreadInfo * info : [��������]
 * @see                         [�μ�����]
 * @n<b>����</b>              :alzq
 * @n<b>����ʱ��</b>          : 09-2-24 14:31:29
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-24  [����]
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
		
		//������ʱ��ȴ��̶߳�����ɾ�����߳�
		ThreadTreeControl::_DelTimeLimitWaitingThread(info);

		//���̴߳洢������ɾ�����߳�
		ThreadTreeControl::_DelRunningThread(info->_m_ttThreadTarget.threadId);

		TreeItem_t * treeItem = (TreeItem_t *)info->_m_lpTreeItem;

		CUSTOMTHREADINFO * customThreadInfo = (CUSTOMTHREADINFO *)treeItem->data.customThreadInfo;
		delete customThreadInfo;
		treeItem->data.customThreadInfo = NULL;

		//ɾ���ڵ�
		info->_m_lpTreeItem = NULL;

		DestroyTree(treeItem, FALSE);
	
		_g_giGlobalInfo->_g_dtiDeleteThreadInfo->threadList.push_back(info);
		ThreadLog::AddLog(info, "add to del list");
	}

	_UnLockThreadControlMutex();
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [���̴߳洢�����в�ѯ]
 * @n<b>��������</b>            : _FindRunningThread
 * @n@param HANDLE threadHandle : [��������]
 * @return                        [����ֵ����]
 * @see                           [�μ�����]
 * @n<b>����</b>                :alzq
 * @n<b>����ʱ��</b>            : 09-2-18 14:24:37
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-18  [����]
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
 * @brief [������Ҫ����]
 * 
 * [���̴߳洢�����в�ѯ]
 * @n<b>��������</b>            : _FindRunningThread
 * @n@param CWinThread * winThread : [��������]
 * @return                        [����ֵ����]
 * @see                           [�μ�����]
 * @n<b>����</b>                :alzq
 * @n<b>����ʱ��</b>            : 09-2-18 14:24:37
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-18  [����]
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
 * @brief [������Ҫ����]
 * 
 * [���̴߳洢������ɾ��ָ���ڵ�]
 * @n<b>��������</b>            : _DelRunningThread
 * @n@param HANDLE threadHandle : [��������]
 * @return                        [����ֵ����]
 * @see                           [�μ�����]
 * @n<b>����</b>                :alzq
 * @n<b>����ʱ��</b>            : 09-2-18 14:24:37
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-18  [����]
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
 * @brief [������Ҫ����]
 * 
 * [���ݱ��̴߳洢����ز��������߳�]
 * @n<b>��������</b>: TSuspendThread
 * @n@param ThreadInfo * info : [��������]
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-16 9:12:42
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-16  [����]
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
	//	//��ΪCWINTHREAD���CWINTHREADʱ
	//	DWORD word = SuspendThread(info->_m_ttThreadTarget.handle);
	//}

	ResetEvent(info->_m_ttThreadTarget.controlEvent);

	//�ڴ˴��ͷſ���������ֹRESET��֮ǰ�պý���SET��������
	ThreadTreeControl::_UnLockThreadControlMutex();

	WaitForSingleObject(info->_m_ttThreadTarget.controlEvent, INFINITE);
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [���ݱ��̴߳洢����ز����ָ����߳�]
 * @n<b>��������</b>: TResumeThread
 * @n@param ThreadInfo * info : [��������]
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-16 9:12:42
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-16  [����]
 ******************************************************************************************************/
void ThreadTreeControl::_TResumeThread(ThreadInfo * info)
{INTERFUN;
	//if(info->_m_ttThreadTarget.isMainThread)
	//{
	//	SetEvent(info->_m_ttThreadTarget.mainThreadEvent);
	//}
	//else
	//{
	//	//��ΪCWINTHREAD���CWINTHREADʱ
	//	ResumeThread(info->_m_ttThreadTarget.handle);
	//}

	SetEvent(info->_m_ttThreadTarget.controlEvent);
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [���ڵ��ɾ������]
 * @n<b>��������</b>                 : _DisposeTreeItem
 * @n@param TreeItem_t * treeItem    : [�ڵ��ַ]
 * @see                                [�μ�����]
 * @n<b>����</b>                     :alzq
 * @n<b>����ʱ��</b>                 : 09-2-18 9:13:02
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-18  [����]
 ******************************************************************************************************/
void ThreadTreeControl::_DisposeTreeItem(TreeItem_t * treeItem, DWORD delayTime)
{INTERFUN;
	try
	{
		//ɾ�����ڵ��ڴ洢����Ĳ���
		CUSTOMTHREADINFO * threadInfo = (CUSTOMTHREADINFO *)treeItem->data.customThreadInfo;
		ThreadInfo * info = threadInfo->threadInfo;

		ThreadLog::AddLog(info, "start dispose tree item");

		if(NULL != threadInfo)
		{
			//�ر��̣߳������̵߳�ַ��ӵ�ɾ���̶߳�����
			if(info->_m_ttThreadTarget.isWinThread)
			{
				//winthread
				BOOL res = FALSE;	//��ʾ�Ƿ�����ǿ����ֹ

				//�����û��������ֹ����
				if(NULL != info->_m_dtfDeleteThreadFunc)
				{
					//ִ���û�����ֹ����
					DELETE_THREAD_FUNCTION deleteFunc = threadInfo->threadInfo->_m_dtfDeleteThreadFunc;
					res = deleteFunc(info->_GetUserInfo());

					DWORD time = delayTime;
					if(0 == time)
					{
						//���ȴ��˳�ʱ��Ϊ0ʱ�滻Ϊ���޵ȴ�
						time = INFINITE;
					}

					if(WaitForSingleObject(info->_m_hFinalMutex, time) != WAIT_TIMEOUT)
					{
						ReleaseMutex(info->_m_hFinalMutex);

						//�����˳��Ѿ�������֮����ص�����ɾ�����������Դ˴�ֱ�ӷ���
						return ;
					}
				}
				else
				{
					if(delayTime != 0)
					{
						//���ӳٵȴ��˳��ģ��ȴ������źź��ж��Ƿ���Ҫǿ����ֹ
						if(WaitForSingleObject(info->_m_hFinalMutex, delayTime) != WAIT_TIMEOUT)
						{
							ReleaseMutex(info->_m_hFinalMutex);

							//�����˳��Ѿ�������֮����ص�����ɾ�����������Դ˴�ֱ�ӷ���
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
// 						//�ɹ�������Ϣ���ȴ���ȫ�˳�
// 						WaitForSingleObject(info->_m_hFinalMutex, INFINITE);
// 						ReleaseMutex(info->_m_hFinalMutex);
// 					}
// 
// 					return ;
// 				}
			}
			else
			{
				DWORD exitCode;			//�߳�״̬��
				GetExitCodeThread(info->_m_ttThreadTarget.handle, &exitCode);	//��ȡ�߳�״̬��
				if(exitCode==STILL_ACTIVE)
				{
					BOOL res = FALSE;	//��ʾ�Ƿ�����ǿ����ֹ
					if(NULL != info->_m_dtfDeleteThreadFunc)
					{
						//ִ���û�����ֹ����
						DELETE_THREAD_FUNCTION deleteFunc = info->_m_dtfDeleteThreadFunc;
						res = deleteFunc(info->_GetUserInfo());

						DWORD time = delayTime;
						if(0 == time)
						{
							//���ȴ��˳�ʱ��Ϊ0ʱ�滻Ϊ���޵ȴ�
							time = INFINITE;
						}

						if(res && WaitForSingleObject(info->_m_hFinalMutex, time) != WAIT_TIMEOUT)
						{
							ReleaseMutex(info->_m_hFinalMutex);
		
							//�����˳��Ѿ�������֮����ص�����ɾ�����������Դ˴�ֱ�ӷ���
							return ;
						}
					}
					else
					{
						if(delayTime != 0)
						{
							//���ӳٵȴ��˳��ģ��ȴ������źź��ж��Ƿ���Ҫǿ����ֹ
							if(WaitForSingleObject(info->_m_hFinalMutex, delayTime) != WAIT_TIMEOUT)
							{
								ReleaseMutex(info->_m_hFinalMutex);

								//�����˳��Ѿ�������֮����ص�����ɾ�����������Դ˴�ֱ�ӷ���
								return ;
							}
						}
					}
					
					if(!res)
					{
						//CloseHandle(threadInfo->threadInfo->_m_ttThreadTarget.handle);
						if(0 != TerminateThread(info->_m_ttThreadTarget.handle, 0))
						{
							//�ɹ��ر��̣߳��ȴ���ȫ�˳�
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
			//ɾ���ڵ�
#ifdef _DEBUG
			DebugBreak();
#endif
			DestroyTree(treeItem, FALSE);
		}
	}
	catch(...)
	{
		//���񵽴��󣬷���
		TRACE("thread error, -- maybe dispose something has been disposed\n");
		return ;
	}
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [ɾ��ĳ���ڵ�������ӽڵ�]
 * @n<b>��������</b>                     : _DestroyThreadTreeChild
 * @n@param TreeItem_t * treeItem        : [��������]
 * @param   bool         terminateEnable : [��������]
 * @see                                    [�μ�����]
 * @n<b>����</b>                         :alzq
 * @n<b>����ʱ��</b>                     : 09-2-18 9:06:35
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-18  [����]
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
 * @brief [������Ҫ����]
 * 
 * [ɾ��ĳһ���ڵ㼰���ӽڵ�]
 * @n<b>��������</b>              : _DestroyThreadTreeItems
 * @n@param TreeItem_t * treeItem : [��������]
 * @see                             [�μ�����]
 * @n<b>����</b>                  :alzq
 * @n<b>����ʱ��</b>              : 09-2-13 13:55:00
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-13  [����]
 ******************************************************************************************************/
void ThreadTreeControl::_DestroyThreadTreeItems(TreeItem_t * treeItem)
{INTERFUN;
	if(NULL == treeItem)
	{
		return ;
	}

	//����ɾ����һ�ڵ㣬����ɾ���ӽڵ�(�ӽڵ�Ⱥ�һ�ڵ�����)
	_DestroyThreadTreeItems(treeItem->nextsibling);
	_DestroyThreadTreeItems(treeItem->firstchild);

	_DisposeTreeItem(treeItem);
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [������Ҫɾ�����̶߳��У�������Ҫɾ�����߳̽ڵ�ɾ��]
 * @n<b>��������</b>: TestAndDeleteThread
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-17 9:34:35
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-17  [����]
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

		//�����̵߳Ľ�����
		if(thread->_m_ttThreadTarget.isMainThread)
		{
			//���ڵ�Ϊ���̣߳������ڵ�ʱֱ��ɾ��
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
			//�����ǳ�ʱʱ�������߳��Ѿ��˳�
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
 * @brief [������Ҫ����]
 * 
 * [����һ���߳̿�ʼ����ȡ��������]
 * @n<b>��������</b>          : SetThreadStart
 * @n@param ThreadInfo * info : [��������]
 * @see                         [�μ�����]
 * @n<b>����</b>              :alzq
 * @n<b>����ʱ��</b>          : 09-2-13 14:24:55
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-13  [����]
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
 * @brief [������Ҫ����]
 * 
 * [����һ���߳�Ϊ���������ý�������]
 * @n<b>��������</b>          : SetThreadFinish
 * @n@param ThreadInfo * info : [��������]
 * @see                         [�μ�����]
 * @n<b>����</b>              :alzq
 * @n<b>����ʱ��</b>          : 09-2-13 14:24:55
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-13  [����]
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
 * @brief [������Ҫ����]
 * 
 * [�������ڵ�]
 * @n<b>��������</b>                : TCreateMutex
 * @n@param ThreadInfo *  thread    : [�����ýڵ���߳���Ϣ�ڵ��ַ]
 * @param   BOOL          initState : [��ʼ����״̬��TRUEΪ���źţ�FALSEΪ���ź�]
 * @param   const WCHAR * name      : [�����ƣ�Ĭ��Ϊ��]
 * @return                            [����ֵ����]
 * @see                               [�μ�����]
 * @n<b>����</b>                    :alzq
 * @n<b>����ʱ��</b>                : 09-2-6 9:35:35
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
SINGLE * ThreadTreeControl::_TCreateMutex(ThreadInfo * threadInfo, BOOL initOwner, const CTCHAR * name)
{INTERFUN;
	SINGLE * single = _FindMutexByName(name);
	
	if(NULL != single)
	{
		//����ѯ��ͬ����SINGLE
		return single;
	}
	
	_LockSingleControlMutex();

	ThreadMutex * cMutex = new ThreadMutex(threadInfo, initOwner, name);
	single = new SINGLE(SINGLE_MUTEX, cMutex, 0);

	if(NULL != name)
	{
		//���Ʋ�ΪNULLʱ����ӽ������Ƶ�������
		_g_giGlobalInfo->_g_mlNamedMutexList.push_back(single);
	}
	
	//��SINGLE��ӵ�SINGLE������
	_g_giGlobalInfo->_g_slSingleList.push_back(single);
	
	_UnLockSingleControlMutex();

	return single;
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [������������ʱ��ȴ��̶߳���]
 * @n<b>��������</b>: DealThreadTimeCount
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-12 18:46:26
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-12  [����]
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
			//���ȴ���ʱʱ������״̬�󼤻��߳�
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
 * @brief [������Ҫ����]
 * 
 * [�����źŽڵ�]
 * @n<b>��������</b>                 : TCreateEvent
 * @n@param ThreadInfo *  threadInfo : [�����ýڵ���߳���Ϣ�ڵ��ַ]
 * @param   BOOL          autoReSet  : [�Ƿ��Զ�ת���ź�״̬]
 * @param   BOOL          initState  : [��ʼ״̬]
 * @param   const WCHAR * name       : [�ź������ƣ�Ĭ��Ϊ��]
 * @return                             [����ֵ����]
 * @see                                [�μ�����]
 * @n<b>����</b>                     :alzq
 * @n<b>����ʱ��</b>                 : 09-2-6 9:44:15
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
SINGLE * ThreadTreeControl::_TCreateEvent(BOOL autoReSet, BOOL initState, const CTCHAR * name)
{INTERFUN;
	SINGLE * single = _FindMutexByName(name);
	
	if(NULL != single)
	{
		//����ѯ��ͬ����SINGLE
		return single;
	}
	
	_LockSingleControlMutex();

	ThreadEvent * cEvent = new ThreadEvent(autoReSet, initState, name);
	single = new SINGLE(SINGLE_EVENT, cEvent, 0);

	if(NULL != name)
	{
		//���Ʋ�ΪNULLʱ����ӽ������Ƶ��źŶ���
		_g_giGlobalInfo->_g_elNamedEventList.push_back(single);
	}
	
	//��SINGLE��ӵ�SINGLE������
	_g_giGlobalInfo->_g_slSingleList.push_back(single);
	
	_UnLockSingleControlMutex();

	return single;
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [���ȴ��̶߳��в���������]
 * @n<b>��������</b>: _LockThreadControlMutex
 * @return            [����ֵ����]
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-6 16:03:47
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
void ThreadTreeControl::_LockThreadControlMutex(void)
{INTERFUN;
	//EnterCriticalSection(&_g_giGlobalInfo->_g_csThreadControlSection);
	WaitForSingleObject(_g_giGlobalInfo->_g_csThreadControlMutex, INFINITE);
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [���ȴ��̶߳��в���������]
 * @n<b>��������</b>: _UnLockThreadControlMutex
 * @return            [����ֵ����]
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-6 16:04:04
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
void ThreadTreeControl::_UnLockThreadControlMutex(void)
{INTERFUN;
	//LeaveCriticalSection(&_g_giGlobalInfo->_g_csThreadControlSection);
	ReleaseMutex(_g_giGlobalInfo->_g_csThreadControlMutex);
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [��ȡ�ź����������в�����]
 * @n<b>��������</b>: _LockSingleControlMutex
 * @return            [����ֵ����]
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-17 14:47:59
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-17  [����]
 ******************************************************************************************************/
void ThreadTreeControl::_LockSingleControlMutex(void)
{INTERFUN;
	//EnterCriticalSection(&_g_giGlobalInfo->_g_csSingleControlSection);
	WaitForSingleObject(_g_giGlobalInfo->_g_csSingleControlMutex, INFINITE);
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�ͷ��ź����������в�����]
 * @n<b>��������</b>: _UnLockSingleControlMutex
 * @return            [����ֵ����]
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-2-17 14:48:19
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-17  [����]
 ******************************************************************************************************/
void ThreadTreeControl::_UnLockSingleControlMutex(void)
{INTERFUN;
	//return ReleaseMutex(_g_hSingleControlMutex);
	//LeaveCriticalSection(&_g_giGlobalInfo->_g_csSingleControlSection);
	ReleaseMutex(_g_giGlobalInfo->_g_csSingleControlMutex);
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�����̵߳ȴ���ʱ�䣬����ض���������߳̽ڵ㣬�����ݵȴ�ʱ������Ƿ���ӵ�����ʱ��ȴ��̶߳�����]
 * @n<b>��������</b>              : _AddWaitingThread
 * @n@param ThreadInfo * lpThread : [��������]
 * @param   DWORD        millSec  : [��������]
 * @see                             [�μ�����]
 * @n<b>����</b>                  :alzq
 * @n<b>����ʱ��</b>              : 09-2-6 16:42:52
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
 ******************************************************************************************************/
void ThreadTreeControl::_AddWaitingThread(ThreadInfo * lpThread, DWORD millSec)
{INTERFUN;
	if(NULL == lpThread)
	{
		return ;
	}

	if(0 == millSec || INFINITE == millSec)
	{
		//��Ϊ�ǵȴ�ģʽ�����޵ȴ�ģʽ��ֱ�ӷ���
		return ;
	}

	_LockThreadControlMutex();

	WAITINGTHREADLIST::iterator iter = _g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.begin();
	while(iter != _g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.end())
	{
		if((*iter)->_m_iWaitTimeCount > millSec)
		{
			//����ǰ�ڵ�ĵȴ�ʱ�������Ҫ������̵߳ĵȴ�ʱ��ʱ�����̼߳��뵽��ǰ�ڵ�ǰ
			_g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.insert(iter, lpThread);

			_UnLockThreadControlMutex();

			return ;
		}

		iter++;
	}

	//���������ӵ�����ĩβ
	_g_giGlobalInfo->_g_wtlTimeLimitWaitingThread.push_back(lpThread);

	_UnLockThreadControlMutex();
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [����ʱ�����Ƶĵȴ��̶߳�����ɾ���ƶ����߳̽ڵ�]
 * @n<b>��������</b>              : _DelTimeLimitWaitingThread
 * @n@param ThreadInfo * lpThread : [��������]
 * @see                             [�μ�����]
 * @n<b>����</b>                  :alzq
 * @n<b>����ʱ��</b>              : 09-2-6 16:54:38
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-06  [����]
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
 * @brief [������Ҫ����]
 * 
 * [�������ֲ�ѯͬ����]
 * @n<b>��������</b>            : _FindMutexByName
 * @n@param const CTCHAR * name : [��������]
 * @return                        [����ֵ����]
 * @see                           [�μ�����]
 * @n<b>����</b>                :alzq
 * @n<b>����ʱ��</b>            : 09-2-12 13:49:50
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-12  [����]
 ******************************************************************************************************/
SINGLE * ThreadTreeControl::_FindMutexByName(const CTCHAR * name)
{INTERFUN;
	if(NULL == name)
	{
		return NULL;
	}

	_LockSingleControlMutex();

	//������ѯ
	MutexList::iterator iter = _g_giGlobalInfo->_g_mlNamedMutexList.begin();
	while(iter != _g_giGlobalInfo->_g_mlNamedMutexList.end())
	{
		ThreadMutex * cMutex = (ThreadMutex *)(*iter)->GetSingle();
		if(_tcscmp(cMutex->_m_csMutexName, name) == 0)
		{
			//���������ʱ
			_UnLockSingleControlMutex();

			return (*iter);
		}

		iter++;
	}
	
	_UnLockSingleControlMutex();

	//��ѯ����ͬ�����򷵻�NULL
	return NULL;
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�������ֲ�ѯͬ���ź���]
 * @n<b>��������</b>            : _FindEventByName
 * @n@param const CTCHAR * name : [��������]
 * @return                        [����ֵ����]
 * @see                           [�μ�����]
 * @n<b>����</b>                :alzq
 * @n<b>����ʱ��</b>            : 09-2-12 14:48:41
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-02-12  [����]
 ******************************************************************************************************/
SINGLE * ThreadTreeControl::_FindEventByName(const CTCHAR * name)
{INTERFUN;
	if(NULL == name)
	{
		return NULL;
	}
	
	_LockSingleControlMutex();

	//������ѯ
	EventList::iterator iter = _g_giGlobalInfo->_g_elNamedEventList.begin();
	while(iter != _g_giGlobalInfo->_g_elNamedEventList.end())
	{
		ThreadEvent * cMutex = (ThreadEvent *)(*iter)->GetSingle();
		if(_tcscmp(cMutex->_m_csEventName, name) == 0)
		{
			//���������ʱ
			_UnLockSingleControlMutex();

			return (*iter);
		}

		iter++;
	}
	
	_UnLockSingleControlMutex();

	//��ѯ����ͬ�����򷵻�NULL
	return NULL;
}

void ThreadTreeControl::_OutputThread(TreeItem_t * treeItem, fstream * stream, LPSYSTEMTIME systemTime)
{INTERFUN;
	if(NULL == treeItem || NULL == stream)
	{
		return ;
	}

	//�ӽڵ�
	_OutputThread(treeItem->firstchild, stream, systemTime);

	//��һ�ڵ�
	_OutputThread(treeItem->nextsibling, stream, systemTime);

	//�������
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

//�����Ȼ�ڻ���ڴ����߳���Ϣ
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

		//��ȡ��ǰʱ��
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
	
