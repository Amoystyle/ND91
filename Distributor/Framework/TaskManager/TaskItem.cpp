#include "stdafx.h"
#include "TaskItem.h"
int TaskFileInfo::g_iTaskInfoID = 1;
CTaskItem::CTaskItem(void)
{INTERFUN;
	m_uID = 0;								//任务ID
	m_uStatus = TASK_STATUS::WAITING;		//当前任务状态
	m_uTaskType = URL_DOWNLAOD;			//任务种类:上传|下载
	m_uTimeTried = 0;						//已经尝试执行的次数
	m_strName = L"";
	m_csFileSavePath=L"";
	_m_uiCustimInfoType = 0;
	_m_pCustomInfo = NULL;//更多的相关信息
	m_TaskFileInfo = NULL;
	m_nProcess = 0;
	m_nSpeed =0;
	_pDeviceData = NULL;
	m_appVersion=_T("");
	_pObserver=NULL;
	InstallToSdCard=true;
	hasProcing=false;
	bUpadateGrade=false;
	appData=NULL;
	_retryed=false;
	m_needInstall=false;
	_delaydelete=false;
	m_fid=0;
	m_isLegalSoft=false;
	m_tEnd=0;
	_IsAndroidMoveStarted=false;
	pCpaInfo = NULL;
#ifndef _AFX
	InitializeCriticalSection(&_m_csItemsListLock);
#endif
}


CTaskItem::CTaskItem(int id, LPCTSTR szName,const wchar_t* URL, TASK_KIND taskType,DeviceData* pDeviceData, const wchar_t * fileSavePath )
{INTERFUN;
	m_uID = id;
#ifndef _AFX
	InitializeCriticalSection(&_m_csItemsListLock);
#endif
	Lock();
	m_TaskFileInfo = new TaskFileInfo(taskType, URL,fileSavePath);

	Unlock();

	m_csFileSavePath=fileSavePath?fileSavePath:L"";

	m_uStatus = WAITING;
	m_uTaskType = taskType;						//任务种类:上传|下载
	m_uTimeTried = 0;						//已经尝试执行的次数
	m_strName = szName;

	//////////////////////////////////////////////////////////////////////////
	_m_uiCustimInfoType = 0;
	_m_pCustomInfo = NULL;//更多的相关信息
	m_nProcess = 0;
	m_nSpeed =0;
	_pDeviceData = pDeviceData;
	_pObserver=NULL;
	InstallToSdCard=true;
	hasProcing=false;
	bUpadateGrade=false;
	appData=NULL;
	_retryed=false;
	m_needInstall=false;
	_delaydelete=false;
	m_fid=0;
	m_isLegalSoft=false;
	m_tEnd=0;
	_IsAndroidMoveStarted=false;
	pCpaInfo = NULL;
}

//拷贝构造函数
CTaskItem::CTaskItem(CTaskItem& modeobj)
{INTERFUN;

	m_uID = modeobj.m_uID;								//任务ID

	m_casename = modeobj.m_casename;
#ifndef _AFX
	InitializeCriticalSection(&_m_csItemsListLock);
#endif
	Lock();
	m_TaskFileInfo = new TaskFileInfo(modeobj.m_TaskFileInfo);
	Unlock();

	m_csFileSavePath=modeobj.m_csFileSavePath;
	m_uStatus = modeobj.m_uStatus;						//当前任务状态
	m_uTaskType = modeobj.m_uTaskType;					//任务种类:上传|下载
	m_uTimeTried = modeobj.m_uTimeTried;				//已经尝试执行的次数
	m_strName = modeobj.m_strName;
	m_appVersion = modeobj.m_appVersion;
	m_platform = modeobj.m_platform;

	//////////////////////////////////////////////////////////////////////////
	_m_uiCustimInfoType = modeobj._m_uiCustimInfoType;
	_m_pCustomInfo = modeobj._m_pCustomInfo;//更多的相关信息
	m_nProcess = modeobj.m_nProcess;
	m_nSpeed = modeobj.m_nSpeed;
	_pDeviceData = modeobj._pDeviceData;
	m_appVersion = modeobj.m_appVersion;
	_pObserver=modeobj._pObserver;
	InstallToSdCard=modeobj.InstallToSdCard;
	hasProcing=modeobj.hasProcing;
	bUpadateGrade=modeobj.bUpadateGrade;
	m_appiconpath = modeobj.m_appiconpath;

	_retryed=modeobj._retryed;
	m_needInstall=modeobj.m_needInstall;
	m_fid=modeobj.m_fid;
	m_isLegalSoft=modeobj.m_isLegalSoft;
	m_tEnd=modeobj.m_tEnd;
	m_fileguid=modeobj.m_fileguid;
	_IsAndroidMoveStarted=modeobj._IsAndroidMoveStarted;
	appData = NULL;
	pCpaInfo = modeobj.pCpaInfo;
	if(modeobj.appData)
		appData=new AppData(modeobj.appData);
}

CTaskItem::~CTaskItem(void)
{INTERFUN;
	//Lock();
	SAFE_DELETE(m_TaskFileInfo);
	//Unlock();
	//SAFE_DELETE(appData);
#ifndef _AFX
	DeleteCriticalSection(&_m_csItemsListLock);
#endif
}

int CTaskItem::GetTaskPercent(void)
{INTERFUN;
	if(FAILED == m_uStatus || FINISHED == m_uStatus)
	{
		return 100;
	}
	if(m_uTaskType!=URL_DOWNLAOD)
		return m_nProcess;
	
	if(m_TaskFileInfo->TotalSize > 0)
	{
		//if(m_nProcess>m_TaskFileInfo->TotalSize)
		//	return 99;
		//else
		double a=(double)m_nProcess/(double)m_TaskFileInfo->TotalSize;
		return (int)(a*100);
	}
	else
		return m_nProcess;
}

void CTaskItem::SetState(TASK_STATUS status)
{INTERFUN;
	m_uStatus = status;
}

void CTaskItem::AddTriedTime(void)
{INTERFUN;
	m_uTimeTried++;
}

void CTaskItem::SetStartTimer(void)
{INTERFUN;
	m_tStart.GetTime();
}

void CTaskItem::SetEndTimer(void)
{INTERFUN;
	_time64(&m_tEnd);
	// m_tEnd.GetTime();
}


void CTaskItem::CopyValue(CTaskItem * target)
{INTERFUN;
	m_uID = target->m_uID;								//任务ID

Lock();
	m_TaskFileInfo = new TaskFileInfo(target->m_TaskFileInfo);
Unlock();

	m_uStatus = target->m_uStatus;						//当前任务状态
	m_uTaskType = target->m_uTaskType;					//任务种类:上传|下载
	m_uTimeTried = target->m_uTimeTried;				//已经尝试执行的次数
	m_strName = target->m_strName;
	m_appVersion = target->m_appVersion;

	//////////////////////////////////////////////////////////////////////////
	_m_uiCustimInfoType = target->_m_uiCustimInfoType;
	_m_pCustomInfo = target->_m_pCustomInfo;//更多的相关信息
	m_appVersion=target->m_appVersion;
	_retryed=target->_retryed;
	m_fid=target->m_fid;
	m_isLegalSoft=target->m_isLegalSoft;
	appData = NULL;
	m_tEnd=target->m_tEnd;
	_IsAndroidMoveStarted=target->_IsAndroidMoveStarted;
	if(target->appData)
		appData=new AppData(target->appData);
}

void CTaskItem::Lock()
{INTERFUN;
#ifndef _AFX
	EnterCriticalSection(&_m_csItemsListLock);
#else
	_m_csItemsListLock.Lock();
#endif
}

void CTaskItem::Unlock()
{INTERFUN;
#ifndef _AFX
	LeaveCriticalSection(&_m_csItemsListLock);
#else
	_m_csItemsListLock.Unlock();
#endif
}
