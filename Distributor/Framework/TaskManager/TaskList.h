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
* 信号量句柄
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
	UINT uPos;//指明要移动哪一个滚动条
	UINT uBytesTotal;
	UINT uBytesGot;
}PARAM_MOVE_PROGRESS;

const int SYN_THREADS_NUM = 5;//并行执行的线程数

#ifdef _TASK_DLL
#define _DLL_EXPOR __declspec(dllexport)
#else
#define _DLL_EXPOR __declspec(dllimport)
#endif

class _DLL_EXPOR CTaskList
{
public:
	static UINT g_iIDCount;							//当前队列中最大的ID, 递增

	bool   _bPxlRunning;
private:
	TaskList m_TaskList;							//队列中存放ListItem对象指针
#ifndef _AFX
	CRITICAL_SECTION   _m_csLock;
#else
	CCriticalSection _m_csLock;						//临界区
#endif


	//外部也有用到m_TaskList需要加锁，先把锁打开
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
	//获得任务完成百分比
	int GetTaskPercent(int iID);
	//获取任务指针
	CTaskItem * GetTaskByID(int id);
	//增加一个任务到末尾
	int AddItem(CTaskItem& pitem);

	int AddItem(wstring TaskName,wstring URL,wstring m_csFileSavePath,TASK_KIND kind,
		unsigned long size,DeviceData *pdevicedata,wstring version,
		ProgressObserver* obs,bool sdcard,bool updategrade = false,wstring casename=L"",
		void* pCpaInfo = NULL);

	CTaskItem* findSameItem(wstring URL,wstring m_csFileSavePath,TASK_KIND kind,bool bUpdategrade=false,wstring casename=L"");

	void FindAllCaseInstallTask(wstring caseId,vector<int>& taskIds);//获取单个方案的安装任务

	//挂起任务
	bool SuspendItem(int id);
	//激活任务
	bool ActivateItem(int id);
	//重试任务
	bool RetryItem(int id);
	//删除任务
	bool DeleteTaskByID(int id);
	//清楚任务列表
	void ClearList(void);
	//清楚已经完成任务
	void ClearDone(std::list<int>* pList);
	//任务往前插队
	bool MoveForward(int taskID);
	//任务后移
	bool MoveBackwrad(int taskID);
	//取得最近一个待处理任务
	bool GetFirstWaitingItem(CTaskItem * recTaskInfo);
	//处理一个最近待处理任务
	bool GetFirstWaitingItemAndSetProceding(CTaskItem * recTaskInfo);
	//设置任务状态
	void SetTaskState(int id, TASK_STATUS state);
	//获得任务状态
	BOOL GetTaskState(int iTaskID, TASK_STATUS& stateTask);
	//获得任务添加时间
	BOOL GetTaskAddTime(int iTaskID, CTime& time);
	//获得任务开始时间
	BOOL GetTaskStartTime(int iTaskID, CTime& time);
	//获得任务停止时间
	BOOL GetTaskEndTime(int iTaskID, CTime& time);
	//获得任务重试次数
	BOOL GetTaskTimeTried(int iTaskID, int& iTimeTried);
	//获得任务类型
	BOOL GetTaskType(int iTaskID, TASK_KIND& kindTask);
	//修改任务
	bool ModifyTask(int id, CTaskItem* pItem);
	//设置任务描述
	void SetTaskNote(int iTaskID, LPCTSTR szNote);
	//获得任务描述
	LPCTSTR GetTaskNote(int iTaskID);
	//设置指定状态任务数
	int GetStateTaskCount(TASK_STATUS iState);
	//设置任务名	
	void SetTaskName(int iTaskID, LPCTSTR szName);
	//获得任务名
	LPCTSTR GetTaskName(int iTaskID);
	//Note增加一行
	void AppendTaskNote(int iTaskID, LPCTSTR);
	//获取任务列表中各个任务的ID
	BOOL GetTaskIDs(std::list<int>* pList);

	int GetUnfinishedTaskCount();

	int RestartAllUnfinishedTask();

	void GetTaskByStatus(vector<int>* items, bool finished);

	DeviceData *CTaskList::GetTaskDevice(int iTaskID);

	int  GetIDByUrl(wstring wsUrl);
	protected:
		//根据id取得任务
		CTaskItem * _GetTaskInfoByID(int id);
};

#endif