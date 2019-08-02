#ifndef NetInstallPlanTaskManager_h__
#define NetInstallPlanTaskManager_h__

#define NET_CASE_START_INDEX   4

#include "Logic/NetInstallsCase/NetInstallsCaseData.h"
/// <summary>
/// 装机方案状态
/// </summary>
enum _INSTALL_PLAN_STATUS
{
	/// <summary>
	/// 等待中
	/// </summary>
	_INSTALL_PLAN_STATUS_Waiting,
	/// <summary>
	/// 下载中
	/// </summary>
	_INSTALL_PLAN_STATUS_Downloading,
	/// <summary>
	/// 下载完成
	/// </summary>
	_INSTALL_PLAN_STATUS_Downloaded,
	/// <summary>
	/// 运行中
	/// </summary>
	_INSTALL_PLAN_STATUS_Running,
	/// <summary>
	/// 完成
	/// </summary>
	_INSTALL_PLAN_STATUS_Completed,
	/// <summary>
	/// 停止
	/// </summary>
	_INSTALL_PLAN_STATUS_Stopped,
	/// <summary>
	/// 正在取消中
	/// </summary>
	_INSTALL_PLAN_STATUS_Canceling,
};

class NetInstallCaseObersever
{
public:
	virtual void NetInstallCaseStatusUpdated()=0;
};
//zjf待制作
class NetInstallPlanTaskManager
	: public BaseHandler<NotifyTaskItemMsg>
	//public TaskUIObserver
{
public:
	NetInstallPlanTaskManager();
	NetInstallPlanTaskManager(wstring deviceInfo, NetInstallsCaseData* installPlanItem);
	~NetInstallPlanTaskManager();

	// Summary  : 下载或者安装线程
	// Returns  : void
	// Parameter: bool bNeed     方案是否需要选中应用
	void Run( bool bNeed = false );
	void RunInstall( bool bNeed = false );
	CButtonUI* btnInstall;

	void SetObersever(NetInstallCaseObersever* pObersever);

	/// <summary>
	/// 当前下载步骤
	/// </summary>
	UINT CurrentDownStep() const { return mCurrentDownStep; }
	void CurrentDownStep(UINT val);

	/// <summary>
	/// 当前安装步骤
	/// </summary>
	UINT CurrentInstallStep() const { return mCurrentInstallStep; }
	void CurrentInstallStep(UINT val);

	/// <summary>
	/// 当前状态
	/// </summary>
	_INSTALL_PLAN_STATUS Status() const { return mStatus; }
	void Status(_INSTALL_PLAN_STATUS val);

	/// <summary>
	/// 设置网络装机方案“下载阶段”的状态，Status状态分为：初始、下载中、下载完成
	/// </summary>
	_NET_INSTALL_PLAN_STATUS PlanStatus() const 
	{
		return mNetInstallPlanItem->NetInstallStatus; 
	}

	void PlanStatus(_NET_INSTALL_PLAN_STATUS val);

	void InstallDownloadedSoft(InstallsAppInfo* appinfo);
	InstallsAppInfo* JoinAppItemsCollection(CTaskItem* item);
	void StatusChanged();

	virtual void TaskFinishedNotify(CTaskItem* item, wstring deviceId);
	virtual void TaskFailNotify(CTaskItem* item, wstring deviceId);
	virtual void TaskRemoveNotify(CTaskItem* item, wstring deviceId);
	virtual void TaskRemoveNotify(int taskid, wstring deviceId);
	virtual void TaskUpdatedSpeedNotify(CTaskItem* item ,wstring deviceId);//任务进度变化

	virtual void TaskNotify(CTaskItem* item, wstring deviceId , TASK_KIND taskKind , TASK_STATUS taskStatus, UINT nTaskId = -1);

	virtual bool Handle(const NotifyTaskItemMsg* pMsg);

private:
	//virtual bool Action(int nAction, BaseParam* pParam); 
	void RunInternal(bool bNeed);

	bool FindTaskId(vector<int>& tasVec, int taskId);

	void EreaseTaskId(vector<int>& tasVec, int taskId);

	void RefreshInstallProgress();

	void RefreshDownloadProgress();

public:
	wstring mDeviceId;
	DeviceData* mDeviceInfo;
	NetInstallsCaseData* mNetInstallPlanItem;

	vector<int> mDownloadComletedTasks;
	vector<int> mDownloadFailTasks;
	vector<int> mDownloadDelTasks;
	vector<int> mDownloadTasks;

	map<int,int> mDownloadPercents;

	UINT mCurrentDownStep;
	UINT mTotalDownStep;

	vector<int> mInstallComletedPlanTasks;
	vector<int> mInstallFailPlanTasks;
	vector<int> mInstallDelPlanTasks;
	vector<int> mInstallPlanTasks;

	map<int,int> mInstallPercents;

	UINT mCurrentInstallStep;
	UINT mTotalInstallStep;
	_INSTALL_PLAN_STATUS mStatus;

	double  m_curpercent;

	NetInstallCaseObersever* _pObersever;

	double  lastpercent;

	int     _skipinstallcnt;
};

#endif
