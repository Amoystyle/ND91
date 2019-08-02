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
	HANDLE hTaskEvent;			//�����Ƿ����ʱ���ź���
	
	
	
protected:
#ifndef _AFX
	CRITICAL_SECTION   _m_csLock;
#else
	CCriticalSection _m_csLock;						//�ٽ���
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
	HANDLE _hTaskHandle;			//����ִ�еľ��

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

	//��ֹ���񣬹ر�������
	void StopTask(void)
	{
		CloseTaskHandle();

		//�����ź���
		::SetEvent(hTaskEvent);
	}
};

/**
*	----��ǰ����ִ�е����������Ϣ����ṹ��
**/
struct DealingTaskInfo
{
	CTaskItem m_tiTaskItem;
	int m_iTotalCount;
	int m_iDealedCount;
	UINT m_uiTaskResult;				//�����Ƿ�ɹ����ڻص�����������
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

		//���ý��
		m_uiTaskResult = TASK_RESULT_FAIL;

		m_ctiCustomTaskInfo.StopTask();

		//ThreadNum=-1;
	}
};


/**
 *	----ȫ�ִ洢������ִ�ж��������Ϣ
 **/
class _DLL_EXPOR GlobalTaskInfo
{
public:
	CTaskList m_tlTaskList;	
	DealingTaskInfo m_dtiDealingTaskInfo[MAX_DEALING_TASK_NUM];
	UINT DealThreadNum;					//���ڴ�����̸߳���
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
			//��ǰ����δ��ɵ�����
			int ret = IDYES;
			try
			{
			//	ret = ::AfxMessageBox(_T("��ǰ����δ��ɵ�����ȷ���������������Ϣ��"), MB_YESNO);
			}
			catch(...)
			{
			}
			if(ret == IDNO)
			{
				//ѡ����NO��ֱ�ӷ���
				return false;
			}
		}

		//����������
		m_tlTaskList.ClearList();
		//��ֹ��ǰִ������
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
	static GlobalTaskInfo * g_gtiGlobalTaskInfo;//�����б���Ϣ
	static ThreadControl * g_tcTaskThreadControl;//�̹߳���
	static vector<GlobalTaskInfo*> g_gtiGlobalInstallTaskInfo;//��װ�����б���Ϣ��ÿ���豸һ����
	static vector<ThreadControl*> g_tcInstallTaskThreadControl;//�̹߳���
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
	//�豸������Ҫ������װ�������߳�
	static BOOL OnDeviceConnect(DeviceData* _pDeviceData); 
	//�豸�Ͽ�ֹͣ��װ�����߳�
	static BOOL OnDeviceDisconnect(const wchar_t* Devid);
	//�����豸ָ��õ��������
	static GlobalTaskInfo * GetInstallTaskIndexByDevice(wstring deviceId);
	//��ʼ�����������
	static void Init();
	//�������������
	static void Destroy();
	//��ȡ���������߳�id
	static int getDealTaskThreadNumByTaskID(int taskid);
	//�ر�����
	static UINT ReportProcess(LPVOID parm);	
	static void SetObserver(TaskCenterNotifyObserver *obj);
	static void SaveDownloadTaskInfoToLocal();
private:
	TaskControl();
	~TaskControl();
	//�������߳�����
	static DWORD TaskDealThread(LPVOID info);

	static void RemoveThreadListIndex(int index);
	static int GetFreeThreadIndex();
	static int GetFreeInstallThreadIndex(UINT max,wstring deviceId);
	static void RemoveInstallThreadListIndex(int index,wstring deviceId);

	//�˳��������߳�
	static BOOL ExitTaskDealThread(LPVOID info);

	//�������߳�����
	static DWORD InstallTaskDealThread(LPVOID info);
	//�˳��������߳�
	static BOOL ExitInstallTaskDealThread(LPVOID info);	
	
	//��װ��������
	static UINT DealInstall(LPVOID lparm);
	//�ж������Ƿ���ͣ
	static bool CheckDealingTaskEnable(TaskDealParm *taskParm);	
	//������
	static UINT DealGlobalTask(LPVOID lparm);
	//��������
	static UINT DealDownLoad(TaskDealParm *taskParm);
	//�����߳̽����¼�
	static void ONFinish(LPVOID info);	

	static void OnFinishedInstall(LPVOID info);

	static void DealInstallTask(LPVOID lparm);
};
#endif