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
//接收任务变化接口
class TaskUIObserver
{
public:
	TaskUIObserver(){}
	virtual ~TaskUIObserver(){}
	virtual void OneTaskAdded(CTaskItem* item){}//新增一个任务
	virtual void TaskUpdatedSpeedNotify(CTaskItem* item ,wstring deviceId){}//任务进度变化
	virtual void TaskFinishedNotify(CTaskItem* item, wstring deviceId){}//任务完成
	virtual void TaskFailNotify(CTaskItem* item, wstring deviceId){}//任务失败
	virtual void TaskCountChangeNotify(wstring deviceId){}//任务数变化
	virtual void TaskRemoveNotify(int taskid, wstring deviceId){}//删除
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


	/**********************************************华丽的分割线，线下是以前的代码*****************************************************************************/
public:
	static wstring GetAppInfo(wstring appFilePath,wstring& version);

	//添加对象接收任务变化消息
	void AddTaskUIObserver(TaskUIObserver* pObserver);
	void RemoveTaskUIObsercer(TaskUIObserver* pObserver);

	void Notify(UINT message, wstring curID, LPARAM lParam );

	// Summary  :添加下载任务
	// Returns	: void
	// Parameter: wstring url		下载网址
	// Parameter: wstring savepath	存储路径
	// Parameter: wstring appname	程序名
	// Parameter: wstring version	程序版本号
	bool StartDownLoad(wstring url, wstring savepath,wstring appname=L"",wstring version=L"", wstring platform=L"",int fid=0, bool bUpdategrade=false,bool needInstall=false,wstring oldappfullpath=L"",bool isLegalSoft=false, TASK_UPGRADE_TYPE taskupgradetype=NOTUPGRADE, wstring appID=L"", void* pCpaInfo =NULL);
	int  DownLoadSoft(wstring url, wstring savepath,wstring appname=L"",wstring version=L"", wstring platform=L"",int fid=0,bool bUpdategrade=false,bool needInstall=false,wstring oldappfullpath=L"",bool isLegalSoft=false, TASK_UPGRADE_TYPE taskupgradetype=NOTUPGRADE, wstring appID=L"", void* pCpaInfo = NULL);
	// Summary  :添加安装任务
	// Returns	: void
	// Parameter: wstring appname		程序名
	// Parameter: wstring appfilepath	程序包路径
	// Parameter: wstring version		程序版本
	// Parameter: long filesize			程序包大小
	// Parameter: wstring deviceId		要安装的设备ID
	// Parameter: bool onSDcard			是否装到SD卡
	// Parameter: wstring plat			安装程序平台
	// Parameter: wstring casename		装机方案名称
	int InstallApp( wstring appname, wstring appfilepath,wstring version, long filesize, wstring deviceId, bool onSDcard,wstring plat=L"",int fid=0,wstring casename=L"",wstring icon=L"", bool isLegalSoft=false);

	void MoveApp(AppData* pApp,long filesize,wstring deviceId);

	//加载已有任务
	//void StartLoad();

	// 启动任务
	bool StartTask(int taskId,wstring deviceId);
	//重试
	bool RestartTask(int taskId,wstring deviceId);

	//删除任务
	bool DeleteTask(int taskId,wstring deviceId,bool notice=true);

	// 暂停任务
	void StopDown(int taskId);

	void InstallApp(int taskId,wstring device);

	// 获得未完成任务数
	int GetTaskCount(bool recount=false);

	// Summary  :通过设备ID和是否已完成标志获取对应任务列表，设备id为空表示获取下载任务
	// Returns	: const vector<CTaskItem*>
	// Parameter: wstring device 设备id，获取下载任务填L""，
	// Parameter: bool finished 是否获取已完成队列false=获取未完成队列 true=获取已完成队列
	const vector<CTaskItem*>* GetTaskList(wstring device, TASK_LIST_TYPE listtype);

	//根据任务id找到任务
	CTaskItem* FindTaskItemById(int taskId);

	// Summary  : 获得节点任务数
	// Returns	: int 节点总任务数
	// Parameter: wstring deviceId 设备id，空下载任务
	// Parameter: int & unfinished 未完成的任务数
	int GetTaskCountOnDevice(wstring deviceId, int& unfinished);

	void SetCaseCount(wstring deviceId, wstring casename,int count);

	void LoadIcons();

	void DeleteAllFinishedInstallTask();

	void dealDeamonInstall(wstring deviceId);

	void DealLastUnfinishedTasksOnDevice(wstring deviceId);

	//根据文件下载的url返回任务id
	int  GetTaskIdByUrl(wstring wsUrl);

protected:

	//加载当前任务列表
	void UIShowTaskList(wstring device, TASK_LIST_TYPE finished);

	//插入一条任务
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

	wstring _curDeviceId;//当前设备ID

	GlobalTaskInfo*  _curTaskList;//当前任务队列

	//bool    _isFinishedList;//是否已完成队列

	TASK_LIST_TYPE   _listtype;

	vector<TaskInstallObserver*> _pInstallObservers;//安装进度接收列表

	vector<TaskUIObserver*> _taskUIObserver;//任务变化通知接收者列表

	//static TaskCenterModel* _instance;//单列

	int _unfinishedTaskCount;//未完成任务数

	map<wstring, int>  _unDealCountInCase;//装机方案中未完成的任务数

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