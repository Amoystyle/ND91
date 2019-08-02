#pragma once

#include "TaskCenterNotifyObserver.h"
#include "TaskItem.h"
#include "Module/Application/AppData.h"
#include "Base/BaseCSLock.h"

class MainView;
class LeftView;
class TaskInstallObserver;
class CTaskThread;
class GlobalTaskInfo;
class TaskCenterControlUI;
struct CTaskModuleInnerMsg;
struct TaskModuleNotifyMsg;
struct CTaskModuleGetValueInnerMsg;

#include "TaskControl.h"
#include "Manager/ThreadManager.h"
#include "Logic/TaskThread.h"
#include "TaskModuleInnerMsg.h"
#include "TaskInstallObserver.h"
#include "UI/CommonUIFunc.h"
#include "Common/MyFileOperation.h"
#include "Common/SystemInfo.h"
#include "Module/Application/AppManager.h"
#include "Manager/EventManager.h"

enum TASK_LIST_TYPE{ALL_TASK=0,UNFINISHED_TASK,FINISHED_TASK};
//��������仯�ӿ�
class TaskUIObserver
{
public:
	TaskUIObserver(){}
	virtual ~TaskUIObserver(){}
	virtual void OneTaskAdded(CTaskItem* item){}//����һ������
	virtual void TaskUpdatedSpeedNotify(CTaskItem* item ,wstring deviceId){}//������ȱ仯
	virtual void TaskFinishedNotify(CTaskItem* item, wstring deviceId){}//�������
	virtual void TaskFailNotify(CTaskItem* item, wstring deviceId){}//����ʧ��
	virtual void TaskCountChangeNotify(wstring deviceId){}//�������仯
	virtual void TaskRemoveNotify(int taskid, wstring deviceId){}//ɾ��
	virtual void NotifyCaseInsatllFinished(int caseId,wstring deviceId){}
	virtual void OnDeviceHasUnfinishedTasks(wstring deviceId){}
};

class TaskCenterControlUI;


class TaskCenterController
	: private BaseCSLock
	, public BaseHandler< TaskModuleNotifyMsg >
	, public BaseHandler<CTaskModuleInnerMsg>
	, public TaskCenterNotifyObserver
	, public BaseGetData<CTaskModuleGetValueInnerMsg>
	, public BaseGetData<GetTaskByIDMsg>
	, public BaseHandler<SetCaseSizeMsg>
	, public BaseGetData<GetInstallPlanIdMsg>
	, public BaseGetData<DownloadTaskItemMsg>
	, public BaseHandler<MoveTaskItemMsg>
	, public IExit
	, public BaseGetData<InstallTaskItemMsg>
{
	friend class CTaskThread;
public:
	TaskCenterController();
	~TaskCenterController();

	bool Init(CContainerUI* pLeft, CContainerUI* pContainer);
	void Show();

	void Hide();

private:
	LeftView* _pLeftView;
	TaskCenterControlUI* _pMainView;
	//TaskCenterControlUI* _oldUI;


	/**********************************************�����ķָ��ߣ���������ǰ�Ĵ���*****************************************************************************/
public:
	static wstring GetAppInfo(wstring appFilePath,wstring& version);

	//��Ӷ����������仯��Ϣ
	void AddTaskUIObserver(TaskUIObserver* pObserver);
	void RemoveTaskUIObsercer(TaskUIObserver* pObserver);

	void Notify(UINT message, wstring curID, LPARAM lParam );

	// Summary  :�����������
	// Returns	: void
	// Parameter: wstring url		������ַ
	// Parameter: wstring savepath	�洢·��
	// Parameter: wstring appname	������
	// Parameter: wstring version	����汾��
	bool StartDownLoad(wstring url, wstring savepath,wstring appname=L"",wstring version=L"", wstring platform=L"",int fid=0, bool bUpdategrade=false,bool needInstall=false,wstring oldappfullpath=L"",bool isLegalSoft=false, TASK_UPGRADE_TYPE taskupgradetype=NOTUPGRADE, wstring appID=L"", void* pCpaInfo =NULL);
	int  DownLoadSoft(wstring url, wstring savepath,wstring appname=L"",wstring version=L"", wstring platform=L"",int fid=0,bool bUpdategrade=false,bool needInstall=false,wstring oldappfullpath=L"",bool isLegalSoft=false, TASK_UPGRADE_TYPE taskupgradetype=NOTUPGRADE, wstring appID=L"", void* pCpaInfo = NULL);
	// Summary  :��Ӱ�װ����
	// Returns	: void
	// Parameter: wstring appname		������
	// Parameter: wstring appfilepath	�����·��
	// Parameter: wstring version		����汾
	// Parameter: long filesize			�������С
	// Parameter: wstring deviceId		Ҫ��װ���豸ID
	// Parameter: bool onSDcard			�Ƿ�װ��SD��
	// Parameter: wstring plat			��װ����ƽ̨
	// Parameter: wstring casename		װ����������
	int InstallApp( wstring appname, wstring appfilepath,wstring version, long filesize, wstring deviceId, bool onSDcard,wstring plat=L"",int fid=0,wstring casename=L"",wstring icon=L"", bool isLegalSoft=false);

	void MoveApp(AppData* pApp,long filesize,wstring deviceId);

	//������������
	//void StartLoad();

	// ��������
	bool StartTask(int taskId,wstring deviceId);
	//����
	bool RestartTask(int taskId,wstring deviceId);

	//ɾ������
	bool DeleteTask(int taskId,wstring deviceId,bool notice=true);

	// ��ͣ����
	void StopDown(int taskId);

	void InstallApp(int taskId,wstring device);

	// ���δ���������
	int GetTaskCount(bool recount=false);

	// Summary  :ͨ���豸ID���Ƿ�����ɱ�־��ȡ��Ӧ�����б��豸idΪ�ձ�ʾ��ȡ��������
	// Returns	: const vector<CTaskItem*>
	// Parameter: wstring device �豸id����ȡ����������L""��
	// Parameter: bool finished �Ƿ��ȡ����ɶ���false=��ȡδ��ɶ��� true=��ȡ����ɶ���
	const vector<CTaskItem*>* GetTaskList(wstring device, TASK_LIST_TYPE listtype);

	//��������id�ҵ�����
	CTaskItem* FindTaskItemById(int taskId);

	// Summary  : ��ýڵ�������
	// Returns	: int �ڵ���������
	// Parameter: wstring deviceId �豸id������������
	// Parameter: int & unfinished δ��ɵ�������
	int GetTaskCountOnDevice(wstring deviceId, int& unfinished);

	void SetCaseCount(wstring deviceId, wstring casename,int count);

	void LoadIcons();

	void DeleteAllFinishedInstallTask();

	void dealDeamonInstall(wstring deviceId);

	void DealLastUnfinishedTasksOnDevice(wstring deviceId);

	//�����ļ����ص�url��������id
	int  GetTaskIdByUrl(wstring wsUrl);

protected:

	//���ص�ǰ�����б�
	void UIShowTaskList(wstring device, TASK_LIST_TYPE finished);

	//����һ������
	void UIInsertTask(CTaskItem* item);

	//virtual bool Action(int nAction, BaseParam* pParam);

	void detach(TaskUIObserver* pObserver);

	bool DeleteTaskAction(int taskId,wstring deviceId);

	bool StartTaskAction( int taskId,wstring deviceId);

	void DescCaseCount( wstring deviceId, wstring casename);

	bool LoadDownedAppIcon(CTaskItem* item);

	void FindAllCaseInstallTask(wstring deviceId, wstring caseId, vector<int>& installTaskIds);

	virtual bool Handle( const TaskModuleNotifyMsg* pMsg );

	virtual bool Handle( const CTaskModuleInnerMsg* pMsg );

	virtual bool Handle( CTaskModuleGetValueInnerMsg* pMsg );

	virtual bool Handle( GetTaskByIDMsg* pMsg );

	virtual bool Handle( const SetCaseSizeMsg* pMsg );

	virtual bool Handle( GetInstallPlanIdMsg* pMsg );

	virtual bool Handle( DownloadTaskItemMsg* pMsg );

	virtual bool Handle( const MoveTaskItemMsg* pMsg );

	virtual bool Handle( InstallTaskItemMsg* pMsg );

	wstring _curDeviceId;//��ǰ�豸ID

	GlobalTaskInfo*  _curTaskList;//��ǰ�������

	//bool    _isFinishedList;//�Ƿ�����ɶ���

	TASK_LIST_TYPE   _listtype;

	vector<TaskInstallObserver*> _pInstallObservers;//��װ���Ƚ����б�

	vector<TaskUIObserver*> _taskUIObserver;//����仯֪ͨ�������б�

	//static TaskCenterModel* _instance;//����

	int _unfinishedTaskCount;//δ���������

	map<wstring, int>  _unDealCountInCase;//װ��������δ��ɵ�������

	bool   _IconLoaded;

	map<wstring, bool>  _installdeamon;

private:
	CRITICAL_SECTION _csLock;
	bool _bExit;

	void LoadIconsInternal(); 

public:
	virtual void TaskDeleteNotify( int taskId, wstring deviceId=L"" );

	virtual void TaskUpdatedSpeedNotify( CTaskItem* item,wstring deviceId=L"" );

	virtual void TaskFinishedNotify( CTaskItem* item,wstring deviceId=L"" );

	virtual void TaskFailNotify( CTaskItem* item,wstring deviceId=L"" );

	virtual void RemoveInstalldeamon(wstring sn);

	virtual void Exit();

	/************************************************************************************************************************************************************/

};