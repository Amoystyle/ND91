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
	wstring _url;//????μ?URL
	wstring _savepath;//????±￡′?μ??·??
	wstring _appname;//3ìDò°ü??
	wstring _version;//°?±?o?
	wstring _platform;//??ì¨
	int     _fid;//èí?tID
	bool    _bIsLegal;//ê?·??y°?
	void*  _pCpaInfo;
};

enum WebCpaOperateType
{
	//summary//
	///webò3??????2ù×÷
	E_Single_Begin = 0,
	E_Single_Download = E_Single_Begin,
	E_Mutile_Download,
	E_Single_End
};

//ê×ò3cpa×°?ú°üwebò3?????￠
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


//?a?????tμ?URL
struct AnalyUrlMsg : BaseMessage<AnalyUrlMsg>
{
	wstring _strUrl;//·t????·￠μ?url
};

//é?3yè????óáDμ???
struct DelTaskItemMsg : BaseMessage<DelTaskItemMsg>
{
	wstring _strUrl;//???t????μ?url
};

//任务中心相关发送的消息
struct NotifyTaskItemMsg : BaseMessage<NotifyTaskItemMsg>
{
	NotifyTaskItemMsg()
	{
		pTask  = NULL;
		pOriginalTask = NULL;
		taskid = 0;
		message = 0;
	}
	void* pTask;//copy 的item
	void* pOriginalTask;//原始的item
	UINT message;
	wstring curID;
	int taskid;
};

//发送任务中心安装消息
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

//发送任务中心下载消息
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

//移动软件位置消息
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

//移动软件位置消息
struct InstallDeamonMsg : BaseMessage<InstallDeamonMsg>
{
	wstring wsDeviceId;
};

//获得任务id
struct GetInstallPlanIdMsg : BaseMessage<GetInstallPlanIdMsg>
{
	GetInstallPlanIdMsg()
	{
	}
	wstring caseId;
	wstring deviceId;
	vector<int> PlanTasks;
};

//设置方案大小
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

//通过任务ID获得任务项
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
	//nParam为任务个数
};