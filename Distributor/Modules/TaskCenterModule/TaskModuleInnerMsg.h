#pragma once

struct StartDownloadParam
{
	wstring url;
	wstring savepath;
	wstring appname/*=L""*/;
	wstring version/*=L""*/;
	wstring platform/*=L""*/;
	int fid/*=0*/;
	bool bUpdategrade/*=false*/;
	bool needInstall/*=false*/;
	wstring oldappfullpath/*=L""*/;
	bool isLegalSoft/*=false*/;
	int taskupgradetype/*=NOTUPGRADE TASK_UPGRADE_TYPE*/;
	wstring appID/*=L""*/;
	void* pCpaInfo;

	StartDownloadParam()
	{
		appname = L"";
		version = L"";
		platform = L"";
		fid = 0;
		bUpdategrade = false;
		needInstall = false;
		oldappfullpath = L"";
		isLegalSoft = false;
		taskupgradetype = 0;
		appID = L"";
		pCpaInfo = NULL;
	}
};

enum TaskModuleMsgType
{
	TASK_UPDATESPEED,//wParam CTaskItem* 
	REMOVE_DEAMON, //strParam sn
	FIND_TASKITEM_BYID, //CTaskModuleGetValueInnerMsg nParam taskId,wparam为返回值CTaskItem* 
	DELETE_TASK, //CTaskModuleInnerMsg nParam taskID, strParam deviceId nUserData 非零notice为true
	GET_TASKLIST, //strParam device, nParam listtype, 返回值 wparam vector<CTaskItem*>*
	LOAD_ICON,
	STOP_DOWN,//nParam taskId
	STAR_TASK,//nParam taskId, strParam deviceId
	FIND_TASKITEMID_BYURL,//CTaskModuleGetValueInnerMsg strParam url,返回值为nParam taskId
	DELETE_ALLFINISHEDINSTALLTASK,//CTaskModuleInnerMsg
	TASK_INSTALLAPP,//CTaskModuleInnerMsg nParam taskid, strParam deviceid
	ADD_TASKUIObserver,//CTaskModuleInnerMsg WPARAM为参数TaskUIObserver*
	GET_TASKCOUNT,//CTaskModuleGetValueInnerMsg striParam 为“0” recount为false 否则为true，nParam 为返回值
	DEAL_LASTUNFINISHEDTASK_ONDEVICE,//CTaskModuleInnerMsg strParam 为SN
	GET_TASKCOUNT_ONDEVICE,//CTaskModuleGetValueInnerMsg nParam 为unfinished， strParam deviceId nUserData为返回值
	STARTDOWNLOAD // CTaskModuleInnerMsg wParam为StartDownloadParam*
};
struct CTaskModuleInnerMsg : BaseMessage<CTaskModuleInnerMsg>
{
	TaskModuleMsgType _type;
	WPARAM wParam;
	LPARAM lParam;
};


struct CTaskModuleGetValueInnerMsg : BaseMessage<CTaskModuleGetValueInnerMsg>
{
	TaskModuleMsgType _type;
	WPARAM wParam;
	LPARAM lParam;
};