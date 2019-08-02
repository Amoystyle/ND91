#ifndef _TASK_CONTROL_H_
#define _TASK_CONTROL_H_
#pragma  once

#ifndef _AFX
#include <windows.h>
#else
#include "vector"
#endif
#include "TaskList.h"
#include "Core/DeviceData.h"
#include "Core/DeviceManager.h"
#include "TaskCenterNotifyObserver.h"
#include "Common/Log.h"
#include "Common/StrOperation.h"
//

#pragma once
#pragma warning( disable: 4251 )
#define TASK_RESULT_SUC		0x01
#define TASK_RESULT_FAIL	0x02

#define SET_DEALING_TASK_NOTE(p,i)	{g_gtiGlobalTaskInfo->m_tlTaskList.AppendTaskNote(g_gtiGlobalTaskInfo->m_dtiDealingTaskInfo[i].m_tiTaskItem.m_uID, p);}
#define CHECK_TASK_AND_RETURN(p)	{if(CheckDealingTaskEnable(p)){return TASK_DEAL_FAIL;}}
#define MAX_DEALING_TASK_NUM 5
typedef void (*onFinish)(LPVOID);
using namespace ND91Assistant;
using namespace std;
struct TaskDealParm
{
	UINT ThreadNum;
	//onFinish onfinish;
	void (*onfinish)(LPVOID);
	UINT Result;
	DeviceData* _pDeviceData;
	wstring deviceID;
	int     taskId;
	CTaskItem* Taskitem;
	CTaskItem* DealingTaskitem;
	bool  userpause;
};
struct CustomTaskInfo
{
	DWORD httpCode;
	int m_iTotalBytesCount;
	int m_iDealedBytesCount;
	HANDLE hTaskEvent;			//控制是否结束时的信号量
	
	
	
protected:
#ifndef _AFX
	CRITICAL_SECTION   _m_csLock;
#else
	CCriticalSection _m_csLock;						//临界区
#endif
	void Lock() 		{
#ifndef _AFX
		EnterCriticalSection(&_m_csLock);
#else
		_m_csLock.Lock();
#endif
	}
	void Unlock() 	{	
#ifndef _AFX
		LeaveCriticalSection(&_m_csLock);
#else
		_m_csLock.Unlock();	
#endif
	}
	HANDLE _hTaskHandle;			//任务执行的句柄

public:
	CustomTaskInfo()
	{
		InitializeCriticalSection(&_m_csLock);
		httpCode = -1;
		m_iTotalBytesCount = 0;
		m_iDealedBytesCount = 0;
		hTaskEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		_hTaskHandle = NULL;
	}
	~CustomTaskInfo(void)
	{

		CloseTaskHandle();

		if(NULL != hTaskEvent)
		{
			::CloseHandle(hTaskEvent);
		}
		DeleteCriticalSection(&_m_csLock);
	}

	void SetTaskHandle(HANDLE handle)
	{
		Lock();
		_hTaskHandle = handle;
		Unlock();
	}
	HANDLE GetTaskHandle(void)
	{
		return _hTaskHandle;
	}
	void CloseTaskHandle(void)
	{
		Lock();
		if(NULL != _hTaskHandle)
		{
			_hTaskHandle = NULL;
		}
		Unlock();

		m_iTotalBytesCount = 0;
		m_iDealedBytesCount = 0;
	}

	//终止任务，关闭任务句柄
	void StopTask(void)
	{
		CloseTaskHandle();

		//设置信号量
		::SetEvent(hTaskEvent);
	}
};

/**
*	----当前正在执行的任务相关信息对象结构体
**/
struct DealingTaskInfo
{
	CTaskItem m_tiTaskItem;
	int m_iTotalCount;
	int m_iDealedCount;
	UINT m_uiTaskResult;				//任务是否成功，在回调函数内设置
	int ThreadNum;
	CustomTaskInfo m_ctiCustomTaskInfo;

	DealingTaskInfo(void)
	{
		m_iTotalCount = 0;
		m_iDealedCount = 0;
		ThreadNum=-1;
	}
	~DealingTaskInfo(void)
	{
	}
	void SuspendTask(void)
	{
		m_tiTaskItem.SetState(TASK_STATUS::SUSPENDED);

		//设置结果
		m_uiTaskResult = TASK_RESULT_FAIL;

		m_ctiCustomTaskInfo.StopTask();

		//ThreadNum=-1;
	}
};


/**
 *	----全局存储的任务执行队列相关信息
 **/
class _DLL_EXPOR GlobalTaskInfo
{
public:
	CTaskList m_tlTaskList;	
	DealingTaskInfo m_dtiDealingTaskInfo[MAX_DEALING_TASK_NUM];
	UINT DealThreadNum;					//正在处理的线程个数
	bool m_bDealEnable;
	bool TaskEnd;
	int TaskNum;
	wstring deviceId;

	bool  _beRemoved;
	bool  _iosdeamonInstalled;
protected:
	HANDLE _m_hDealEvent;

public:
	int getDealTaskThreadNumByTaskID(int taskid)
	{
		for(int i=0;i<MAX_DEALING_TASK_NUM-1;i++)
		{
			if(m_dtiDealingTaskInfo[i].m_tiTaskItem.m_uID == taskid && m_dtiDealingTaskInfo[i].m_tiTaskItem.m_uStatus == PROCEDING)
				return  m_dtiDealingTaskInfo[i].ThreadNum;
		}
		return -1;
	}
	GlobalTaskInfo(void)
	{
		_m_hDealEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		m_bDealEnable = true;
		TaskEnd=false;
		TaskNum=0;
		DealThreadNum = 0;
		_beRemoved=false;
		_iosdeamonInstalled=false;
	}
	~GlobalTaskInfo(void)
	{
		TaskEnd=true;
		ClearTask();
		CloseHandle(_m_hDealEvent);
	}
	void StartThread(void)
	{
		::SetEvent(_m_hDealEvent);
	}
	void WaitForStart(void);
	void SetThreadExitEnable(void)
	{
		m_bDealEnable = false;
	}
	void SuspendDealingTask(int i)
	{
		m_tlTaskList.SetTaskState(m_dtiDealingTaskInfo[i].m_tiTaskItem.m_uID, TASK_STATUS::SUSPENDED);

		m_dtiDealingTaskInfo[i].SuspendTask();
	}
	bool ClearTask(void)
	{
		if(m_tlTaskList.GetStateTaskCount(WAITING) > 0 || m_tlTaskList.GetStateTaskCount(PROCEDING) > 0)
		{
			//当前存在未完成的任务
			int ret = IDYES;
			try
			{
			//	ret = ::AfxMessageBox(_T("当前存在未完成的任务，确认清除所有任务信息！"), MB_YESNO);
			}
			catch(...)
			{
			}
			if(ret == IDNO)
			{
				//选择了NO则直接返回
				return false;
			}
		}

		//清空任务队列
		m_tlTaskList.ClearList();
		//终止当前执行任务
		for(size_t i =0;i<DealThreadNum;i++)
		{
			SuspendDealingTask(i);
		}
		DealThreadNum=0;
		//_beRemoved = true;
		return true;
	}
};


#define TASK_DEAL_SUC			0x01
#define TASK_DEAL_ERROR			0x02
#define TASK_DEAL_PAUSE			0x04
#define TASK_DEAL_FAIL			0x08
class ThreadControl;
class _DLL_EXPOR TaskControl
{
	friend class GlobalTaskInfo;
public:
	static GlobalTaskInfo * g_gtiGlobalTaskInfo;//任务列表信息
	static ThreadControl * g_tcTaskThreadControl;//线程管理
	static vector<GlobalTaskInfo*> g_gtiGlobalInstallTaskInfo;//安装任务列表信息（每个设备一个）
	static vector<ThreadControl*> g_tcInstallTaskThreadControl;//线程管理
	static vector<TaskDealParm*> ParmList;
	static TaskCenterNotifyObserver*  obServer;
	static vector<TaskDealParm*> ParmListDownload;
	static vector<int> ThreadIndex;
	static vector<TaskDealParm*> _parmInstall;
	static map<wstring,vector<int>> _installThreadIndex;
	static CRITICAL_SECTION   _m_csThreadIndexLock;
	static CRITICAL_SECTION   _m_csSaveTaskLock;

	static int _TaskDeviceNum;
public:
	//设备接入需要启动安装任务处理线程
	static BOOL OnDeviceConnect(DeviceData* _pDeviceData); 
	//设备断开停止安装任务线程
	static BOOL OnDeviceDisconnect(const wchar_t* Devid);
	//根据设备指针得到任务队列
	static GlobalTaskInfo * GetInstallTaskIndexByDevice(wstring deviceId);
	//初始化任务管理器
	static void Init();
	//销毁任务管理器
	static void Destroy();
	//获取处理任务线程id
	static int getDealTaskThreadNumByTaskID(int taskid);
	//回报进度
	static UINT ReportProcess(LPVOID parm);	
	static void SetObserver(TaskCenterNotifyObserver *obj);
	static void SaveDownloadTaskInfoToLocal();
private:
	TaskControl();
	~TaskControl();
	//任务处理线程启动
	static DWORD TaskDealThread(LPVOID info);

	static void RemoveThreadListIndex(int index);
	static int GetFreeThreadIndex();
	static int GetFreeInstallThreadIndex(UINT max,wstring deviceId);
	static void RemoveInstallThreadListIndex(int index,wstring deviceId);

	//退出任务处理线程
	static BOOL ExitTaskDealThread(LPVOID info);

	//任务处理线程启动
	static DWORD InstallTaskDealThread(LPVOID info);
	//退出任务处理线程
	static BOOL ExitInstallTaskDealThread(LPVOID info);	
	
	//安装任务处理函数
	static UINT DealInstall(LPVOID lparm);
	//判断任务是否暂停
	static bool CheckDealingTaskEnable(TaskDealParm *taskParm);	
	//任务处理
	static UINT DealGlobalTask(LPVOID lparm);
	//下载任务
	static UINT DealDownLoad(TaskDealParm *taskParm);
	//任务线程结束事件
	static void ONFinish(LPVOID info);	

	static void OnFinishedInstall(LPVOID info);

	static void DealInstallTask(LPVOID lparm);
};
#endif