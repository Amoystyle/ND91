#pragma once


struct CWebResourceDownload :
	BaseMessage<CWebResourceDownload>
{
	CWebResourceDownload()
	{	
		_url=L"";
		_savepath=L"";
		_appname=L"";
		_version=L"";
		_platform=L"";
		_fid=0;
		_bIsLegal = false;
		_pCpaInfo = NULL;
	}
	wstring _url;//????��?URL
	wstring _savepath;//????�����?��??��??
	wstring _appname;//3��D���㨹??
	wstring _version;//��?��?o?
	wstring _platform;//??����
	int     _fid;//����?tID
	bool    _bIsLegal;//��?��??y��?
	void*  _pCpaInfo;
};

enum WebCpaOperateType
{
	//summary//
	///web��3??????2������
	E_Single_Begin = 0,
	E_Single_Download = E_Single_Begin,
	E_Mutile_Download,
	E_Single_End
};

//������3cpa����?���㨹web��3?????��
struct CpaPackageMsg : BaseMessage<CpaPackageMsg>
{
	CpaPackageMsg():_nPlatForm(-1)
	{
		_lparam = NULL;
	}
	WebCpaOperateType _eType;
	string _strUrl;
	int    _nPlatForm;
	void* _lparam;
};


//?a?????t��?URL
struct AnalyUrlMsg : BaseMessage<AnalyUrlMsg>
{
	wstring _strUrl;//��t????�����?url
};

//��?3y��????����D��???
struct DelTaskItemMsg : BaseMessage<DelTaskItemMsg>
{
	wstring _strUrl;//???t????��?url
};

//����������ط��͵���Ϣ
struct NotifyTaskItemMsg : BaseMessage<NotifyTaskItemMsg>
{
	NotifyTaskItemMsg()
	{
		pTask  = NULL;
		pOriginalTask = NULL;
		taskid = 0;
		message = 0;
	}
	void* pTask;//copy ��item
	void* pOriginalTask;//ԭʼ��item
	UINT message;
	wstring curID;
	int taskid;
};

//�����������İ�װ��Ϣ
struct InstallTaskItemMsg : BaseMessage<InstallTaskItemMsg>
{
	InstallTaskItemMsg()
	{
		filesize = 0;
		onSDcard = false;
		fid = 0;
		isLegalSoft = false;
		taskid = 0;
	}
	wstring appname;
	wstring appfilepath;
	wstring version;
	long filesize;
	wstring deviceId;
	bool onSDcard;
	wstring plat;
	int fid;
	wstring casename;
	wstring icon;
	bool isLegalSoft;
	int  taskid;
};

//������������������Ϣ
struct DownloadTaskItemMsg : BaseMessage<DownloadTaskItemMsg>
{
	DownloadTaskItemMsg()
	{
		fid = 0;
		bUpdategrade = false;
		needInstall = false;
		isLegalSoft = false;
		pCpaInfo = NULL;
		taskid = 0;
		taskupgradetype = 0;
		appID = L"";
	}
	wstring url;
	wstring savepath;
	wstring appname;
	wstring version;
	wstring platform;
	int fid;
	bool bUpdategrade;
	bool needInstall;
	wstring oldappfullpath;
	bool isLegalSoft;
	int  taskupgradetype;
	wstring appID;
	void* pCpaInfo;
	int taskid;
};

//�ƶ����λ����Ϣ
struct MoveTaskItemMsg : BaseMessage<MoveTaskItemMsg>
{
	MoveTaskItemMsg()
	{
		pData = NULL;
		staticDiskUsage = 0;
	}
	void* pData;
	long  staticDiskUsage;
	wstring curDeviceID;
};

//�ƶ����λ����Ϣ
struct InstallDeamonMsg : BaseMessage<InstallDeamonMsg>
{
	wstring wsDeviceId;
};

//�������id
struct GetInstallPlanIdMsg : BaseMessage<GetInstallPlanIdMsg>
{
	GetInstallPlanIdMsg()
	{
	}
	wstring caseId;
	wstring deviceId;
	vector<int> PlanTasks;
};

//���÷�����С
struct SetCaseSizeMsg : BaseMessage<SetCaseSizeMsg>
{
	SetCaseSizeMsg()
	{
		count = 0;
	}
	wstring deviceId;
	wstring casename;
	int count;
};

//ͨ������ID���������
struct GetTaskByIDMsg : BaseMessage<GetTaskByIDMsg>
{
	GetTaskByIDMsg()
	{
		nTaskID = 0;
		pTask = NULL;
	}
	int nTaskID;
	void* pTask;
};

struct TaskModuleNotifyMsg : BaseMessage<TaskModuleNotifyMsg>
{
	int _notifyType;
	wstring _curID;
	LPARAM _param;
};

struct TopTaskCountMsg :  BaseMessage<TopTaskCountMsg>
{
	//nParamΪ�������
};