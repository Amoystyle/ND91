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
	FIND_TASKITEM_BYID, //CTaskModuleGetValueInnerMsg nParam taskId,wparamΪ����ֵCTaskItem* 
	DELETE_TASK, //CTaskModuleInnerMsg nParam taskID, strParam deviceId nUserData ����noticeΪtrue
	GET_TASKLIST, //strParam device, nParam listtype, ����ֵ wparam vector<CTaskItem*>*
	LOAD_ICON,
	STOP_DOWN,//nParam taskId
	STAR_TASK,//nParam taskId, strParam deviceId
	FIND_TASKITEMID_BYURL,//CTaskModuleGetValueInnerMsg strParam url,����ֵΪnParam taskId
	DELETE_ALLFINISHEDINSTALLTASK,//CTaskModuleInnerMsg
	TASK_INSTALLAPP,//CTaskModuleInnerMsg nParam taskid, strParam deviceid
	ADD_TASKUIObserver,//CTaskModuleInnerMsg WPARAMΪ����TaskUIObserver*
	GET_TASKCOUNT,//CTaskModuleGetValueInnerMsg striParam Ϊ��0�� recountΪfalse ����Ϊtrue��nParam Ϊ����ֵ
	DEAL_LASTUNFINISHEDTASK_ONDEVICE,//CTaskModuleInnerMsg strParam ΪSN
	GET_TASKCOUNT_ONDEVICE,//CTaskModuleGetValueInnerMsg nParam Ϊunfinished�� strParam deviceId nUserDataΪ����ֵ
	STARTDOWNLOAD // CTaskModuleInnerMsg wParamΪStartDownloadParam*
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