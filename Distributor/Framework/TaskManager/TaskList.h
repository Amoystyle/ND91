#ifndef _TASK_LIST_H_
#define  _TASK_LIST_H_

#pragma once
#pragma warning(disable:4482)
#ifndef _AFX
#include "windows.h"
#endif
#include <list>
#include "TaskItem.h"
using namespace std; 

/************************************************************************************
* @brief
* �ź������
* 
************************************************************************************/
typedef struct
{
	HANDLE hEventStop; 
	HANDLE hEventSuspend;  
	HANDLE hEventContinue;
}LIST_EVENTS, *PLIST_EVENTS;

typedef struct
{
	UINT uPos;//ָ��Ҫ�ƶ���һ��������
	UINT uBytesTotal;
	UINT uBytesGot;
}PARAM_MOVE_PROGRESS;

const int SYN_THREADS_NUM = 5;//����ִ�е��߳���

#ifdef _TASK_DLL
#define _DLL_EXPOR __declspec(dllexport)
#else
#define _DLL_EXPOR __declspec(dllimport)
#endif

class _DLL_EXPOR CTaskList
{
public:
	static UINT g_iIDCount;							//��ǰ����������ID, ����

	bool   _bPxlRunning;
private:
	TaskList m_TaskList;							//�����д��ListItem����ָ��
#ifndef _AFX
	CRITICAL_SECTION   _m_csLock;
#else
	CCriticalSection _m_csLock;						//�ٽ���
#endif


	//�ⲿҲ���õ�m_TaskList��Ҫ�������Ȱ�����
public:
void TASK_LOCK() 		{
	#ifndef _AFX
			EnterCriticalSection(&_m_csLock);
	#else
			_m_csLock.Lock();
	#endif
}


void TASK_UNLOCK() 	{	
		#ifndef _AFX
				LeaveCriticalSection(&_m_csLock);
		#else
				_m_csLock.Unlock();	
		#endif
}
public:

	CTaskList(void);
	virtual ~CTaskList(void);

	TaskList * getTaskList(){return &m_TaskList;}
	//���������ɰٷֱ�
	int GetTaskPercent(int iID);
	//��ȡ����ָ��
	CTaskItem * GetTaskByID(int id);
	//����һ������ĩβ
	int AddItem(CTaskItem& pitem);

	int AddItem(wstring TaskName,wstring URL,wstring m_csFileSavePath,TASK_KIND kind,
		unsigned long size,DeviceData *pdevicedata,wstring version,
		ProgressObserver* obs,bool sdcard,bool updategrade = false,wstring casename=L"",
		void* pCpaInfo = NULL);

	CTaskItem* findSameItem(wstring URL,wstring m_csFileSavePath,TASK_KIND kind,bool bUpdategrade=false,wstring casename=L"");

	void FindAllCaseInstallTask(wstring caseId,vector<int>& taskIds);//��ȡ���������İ�װ����

	//��������
	bool SuspendItem(int id);
	//��������
	bool ActivateItem(int id);
	//��������
	bool RetryItem(int id);
	//ɾ������
	bool DeleteTaskByID(int id);
	//��������б�
	void ClearList(void);
	//����Ѿ��������
	void ClearDone(std::list<int>* pList);
	//������ǰ���
	bool MoveForward(int taskID);
	//�������
	bool MoveBackwrad(int taskID);
	//ȡ�����һ������������
	bool GetFirstWaitingItem(CTaskItem * recTaskInfo);
	//����һ���������������
	bool GetFirstWaitingItemAndSetProceding(CTaskItem * recTaskInfo);
	//��������״̬
	void SetTaskState(int id, TASK_STATUS state);
	//�������״̬
	BOOL GetTaskState(int iTaskID, TASK_STATUS& stateTask);
	//����������ʱ��
	BOOL GetTaskAddTime(int iTaskID, CTime& time);
	//�������ʼʱ��
	BOOL GetTaskStartTime(int iTaskID, CTime& time);
	//�������ֹͣʱ��
	BOOL GetTaskEndTime(int iTaskID, CTime& time);
	//����������Դ���
	BOOL GetTaskTimeTried(int iTaskID, int& iTimeTried);
	//�����������
	BOOL GetTaskType(int iTaskID, TASK_KIND& kindTask);
	//�޸�����
	bool ModifyTask(int id, CTaskItem* pItem);
	//������������
	void SetTaskNote(int iTaskID, LPCTSTR szNote);
	//�����������
	LPCTSTR GetTaskNote(int iTaskID);
	//����ָ��״̬������
	int GetStateTaskCount(TASK_STATUS iState);
	//����������	
	void SetTaskName(int iTaskID, LPCTSTR szName);
	//���������
	LPCTSTR GetTaskName(int iTaskID);
	//Note����һ��
	void AppendTaskNote(int iTaskID, LPCTSTR);
	//��ȡ�����б��и��������ID
	BOOL GetTaskIDs(std::list<int>* pList);

	int GetUnfinishedTaskCount();

	int RestartAllUnfinishedTask();

	void GetTaskByStatus(vector<int>* items, bool finished);

	DeviceData *CTaskList::GetTaskDevice(int iTaskID);

	int  GetIDByUrl(wstring wsUrl);
	protected:
		//����idȡ������
		CTaskItem * _GetTaskInfoByID(int id);
};

#endif