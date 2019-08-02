#ifndef NetInstallPlanTaskManager_h__
#define NetInstallPlanTaskManager_h__

#define NET_CASE_START_INDEX   4

#include "Logic/NetInstallsCase/NetInstallsCaseData.h"
/// <summary>
/// װ������״̬
/// </summary>
enum _INSTALL_PLAN_STATUS
{
	/// <summary>
	/// �ȴ���
	/// </summary>
	_INSTALL_PLAN_STATUS_Waiting,
	/// <summary>
	/// ������
	/// </summary>
	_INSTALL_PLAN_STATUS_Downloading,
	/// <summary>
	/// �������
	/// </summary>
	_INSTALL_PLAN_STATUS_Downloaded,
	/// <summary>
	/// ������
	/// </summary>
	_INSTALL_PLAN_STATUS_Running,
	/// <summary>
	/// ���
	/// </summary>
	_INSTALL_PLAN_STATUS_Completed,
	/// <summary>
	/// ֹͣ
	/// </summary>
	_INSTALL_PLAN_STATUS_Stopped,
	/// <summary>
	/// ����ȡ����
	/// </summary>
	_INSTALL_PLAN_STATUS_Canceling,
};

class NetInstallCaseObersever
{
public:
	virtual void NetInstallCaseStatusUpdated()=0;
};
//zjf������
class NetInstallPlanTaskManager
	: public BaseHandler<NotifyTaskItemMsg>
	//public TaskUIObserver
{
public:
	NetInstallPlanTaskManager();
	NetInstallPlanTaskManager(wstring deviceInfo, NetInstallsCaseData* installPlanItem);
	~NetInstallPlanTaskManager();

	// Summary  : ���ػ��߰�װ�߳�
	// Returns  : void
	// Parameter: bool bNeed     �����Ƿ���Ҫѡ��Ӧ��
	void Run( bool bNeed = false );
	void RunInstall( bool bNeed = false );
	CButtonUI* btnInstall;

	void SetObersever(NetInstallCaseObersever* pObersever);

	/// <summary>
	/// ��ǰ���ز���
	/// </summary>
	UINT CurrentDownStep() const { return mCurrentDownStep; }
	void CurrentDownStep(UINT val);

	/// <summary>
	/// ��ǰ��װ����
	/// </summary>
	UINT CurrentInstallStep() const { return mCurrentInstallStep; }
	void CurrentInstallStep(UINT val);

	/// <summary>
	/// ��ǰ״̬
	/// </summary>
	_INSTALL_PLAN_STATUS Status() const { return mStatus; }
	void Status(_INSTALL_PLAN_STATUS val);

	/// <summary>
	/// ��������װ�����������ؽ׶Ρ���״̬��Status״̬��Ϊ����ʼ�������С��������
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
	virtual void TaskUpdatedSpeedNotify(CTaskItem* item ,wstring deviceId);//������ȱ仯

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
