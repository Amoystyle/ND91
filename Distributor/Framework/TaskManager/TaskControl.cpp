#include "stdafx.h"
#include "TaskControl.h"
#include "HttpDownload.h"
#include "TaskHistroy.h"
#include "ThreadTree/ThreadTreeControl.h"
#include "Monitor/ProgressObserver.h"
#include "Module/Application/AppManager.h"
#include "Common/FileOperation.h"
#include "Common/AssistantException.h"
#pragma warning( disable: 4996 )
GlobalTaskInfo * TaskControl::g_gtiGlobalTaskInfo = NULL;
ThreadControl * TaskControl::g_tcTaskThreadControl = NULL;
//UINT GlobalTaskInfo::DealThreadNum = 0;
vector<GlobalTaskInfo*> TaskControl::g_gtiGlobalInstallTaskInfo;//安装任务列表信息（每个设备一个）
vector<ThreadControl*> TaskControl::g_tcInstallTaskThreadControl;//线程管理
vector<TaskDealParm*> TaskControl::ParmList;
vector<TaskDealParm*> TaskControl::ParmListDownload;
vector<TaskDealParm*> TaskControl::_parmInstall;
TaskCenterNotifyObserver*  TaskControl::obServer = NULL;
vector<int> TaskControl::ThreadIndex;
map<wstring,vector<int>> TaskControl::_installThreadIndex;
CRITICAL_SECTION TaskControl::_m_csThreadIndexLock;
CRITICAL_SECTION TaskControl::_m_csSaveTaskLock;
int TaskControl::_TaskDeviceNum=0;

//UINT TaskControl::g_gtiGlobalTaskInfo->DealThreadNum = 0;
TaskControl::TaskControl(void)
{INTERFUN;
}

TaskControl::~TaskControl(void)
{INTERFUN;
}
int TaskControl::getDealTaskThreadNumByTaskID(int taskid)
{INTERFUN;
	return g_gtiGlobalTaskInfo->getDealTaskThreadNumByTaskID(taskid);
}
int TaskControl::GetFreeThreadIndex()
{INTERFUN;
	if(ThreadIndex.size()>MAX_DEALING_TASK_NUM)
		return -1;
	int i;
	int jfind=-1;
	for(i=0;i<MAX_DEALING_TASK_NUM;i++)
	{
		EnterCriticalSection(&_m_csThreadIndexLock);
		for(size_t j=0;j<ThreadIndex.size();j++)
		{
			jfind= ThreadIndex.at(j);
			if(jfind==i)
			{				
				break;
			}
			jfind =-1;
		}
		LeaveCriticalSection(&_m_csThreadIndexLock);
		if(jfind==-1)
			break;
	}
	return i;
}

int TaskControl::GetFreeInstallThreadIndex(UINT max,wstring deviceId)
{INTERFUN;
	if(_installThreadIndex.find(deviceId)==_installThreadIndex.end())
		return 0;
	vector<int> installthreadinx=_installThreadIndex[deviceId];
	if(installthreadinx.size()>max)
		return -1;
	int i;
	int jfind=-1;
	for(i=0;i<(int)max;i++)
	{
		for(size_t j=0;j<installthreadinx.size();j++)
		{
			jfind= installthreadinx.at(j);
			if(jfind==i)
			{				
				break;
			}
			jfind =-1;
		}
		if(jfind==-1)
			break;
	}
	return i;
}

void TaskControl::RemoveInstallThreadListIndex(int index,wstring deviceId)
{INTERFUN;
	if(_installThreadIndex.find(deviceId)==_installThreadIndex.end())
		return;
	vector<int> installthreadinx=_installThreadIndex[deviceId];
	for(size_t i=0;i<installthreadinx.size();i++)
	{
		if(index == installthreadinx.at(i))
		{
			_installThreadIndex[deviceId].erase(_installThreadIndex[deviceId].begin()+i);
			LOG_DEBUG(deviceId+L"installthreadfinishedindex安装索引移除"+CStrOperation::IntToWString(i));
			break;
		}
	}
}

void TaskControl::RemoveThreadListIndex(int index)
{INTERFUN;
	EnterCriticalSection(&_m_csThreadIndexLock);
	for(size_t i=0;i<ThreadIndex.size();i++)
	{
		if(index == ThreadIndex.at(i))
		{
			ThreadIndex.erase(ThreadIndex.begin()+i);
			break;
		}
	}
	LeaveCriticalSection(&_m_csThreadIndexLock);
}
DWORD TaskControl::TaskDealThread(LPVOID info)
{INTERFUN;
	try
	{
		while(g_gtiGlobalTaskInfo->m_bDealEnable)
		{
			//当需要继续执行时，无限循环
			g_gtiGlobalTaskInfo->WaitForStart();

			if(!g_gtiGlobalTaskInfo->m_bDealEnable)
			{
				//需要退出时
				return 0;
			}
			if(g_gtiGlobalTaskInfo->DealThreadNum > MAX_DEALING_TASK_NUM-1)
				continue;

			//任务处理开始
			while(true)
			{
				int NewIndex = GetFreeThreadIndex();
				if(NewIndex==-1)
					break;
				if(!g_gtiGlobalTaskInfo->m_tlTaskList.GetFirstWaitingItemAndSetProceding(&g_gtiGlobalTaskInfo->m_dtiDealingTaskInfo[NewIndex].m_tiTaskItem))
					break;
				//当获取第一个等待执行的任务成功后，执行任务，否则继续等待
				//执行任务，并根据返回值设置任务状态
				//UINT dealResult = TASK_DEAL_SUC;
				if(g_gtiGlobalTaskInfo->DealThreadNum > 4)
				{
					g_gtiGlobalTaskInfo->m_tlTaskList.SetTaskState((g_gtiGlobalTaskInfo->m_dtiDealingTaskInfo[NewIndex].m_tiTaskItem.m_uID),WAITING);
					break;
				}
				TaskDealParm* parm = new TaskDealParm();
				parm->ThreadNum = NewIndex;
				parm->DealingTaskitem = &g_gtiGlobalTaskInfo->m_dtiDealingTaskInfo[NewIndex].m_tiTaskItem;
				parm->Taskitem =   g_gtiGlobalTaskInfo->m_tlTaskList.GetTaskByID(parm->DealingTaskitem->m_uID);
				parm->taskId = parm->DealingTaskitem->m_uID;
				parm->userpause=false;
				parm->onfinish = ONFinish;
				ParmListDownload.push_back(parm);
				parm->_pDeviceData = g_gtiGlobalTaskInfo->m_tlTaskList.GetTaskDevice(parm->DealingTaskitem->m_uID);
				//保存线程位置
				g_gtiGlobalTaskInfo->m_dtiDealingTaskInfo[NewIndex].ThreadNum = NewIndex;
				EnterCriticalSection(&_m_csThreadIndexLock);
				ThreadIndex.push_back(NewIndex);
				LeaveCriticalSection(&_m_csThreadIndexLock);
				try
				{
					//dealResult = DealGlobalTask();					
					CreateThread(0,0,(LPTHREAD_START_ROUTINE)DealGlobalTask,(LPVOID)parm,0,NULL);								
					LOG_DEBUG(L"down number added "+CStrOperation::IntToWString(g_gtiGlobalTaskInfo->DealThreadNum));
					//LOG_DEBUG(L"down number added  taskid "+CStrOperation::IntToWString(parm->Taskitem->m_uID));
					g_gtiGlobalTaskInfo->DealThreadNum++;
					if(g_gtiGlobalTaskInfo->DealThreadNum > 4)
						break;
				}
				catch(...)
				{
					if(g_gtiGlobalTaskInfo->m_bDealEnable)
					{
						//出现执行异常设置状态为失败
						SET_DEALING_TASK_NOTE(_T("执行任务出现无法控制的异常，导致执行失败"),g_gtiGlobalTaskInfo->DealThreadNum);
						//dealResult = TASK_DEAL_FAIL;	
						if(parm->Result != TASK_DEAL_SUC)
						{						
							parm->Result = TASK_DEAL_FAIL;						
							onFinish((LPVOID)parm);
						}

					}
					else
					{
						return 0;
					}
				}

			}//end 处理开始
			
		}
	}
	catch(...)
	{
		if(g_gtiGlobalTaskInfo->m_bDealEnable)
		{
	
		}
	}

	return 0;
}
void TaskControl::ONFinish(LPVOID info)
{	INTERFUN;
	if(!g_gtiGlobalTaskInfo)
	{
		return;
	}
	LOG_DEBUG(L"on finish enter:"+CStrOperation::IntToWString(g_gtiGlobalTaskInfo->DealThreadNum));

	TaskDealParm *dealResult = (TaskDealParm *)info;
	wstring deviceId=dealResult->deviceID;

	//设置任务执行状态
	if(TASK_DEAL_SUC != dealResult->Result)
	{
		//任务失败，可能为用户终止也可能其他原因
		TASK_STATUS recState;
		//获取任务执行状态判断是否用户暂停
		CTaskItem * pitem =dealResult->Taskitem;
		if(pitem)
		{
			pitem->hasProcing=false;
		}
		dealResult->DealingTaskitem->hasProcing=false;

		if(!g_gtiGlobalTaskInfo->m_tlTaskList.GetTaskState(dealResult->taskId, recState))
		{
			LOG_DEBUG(L"on finished not find item "+dealResult->DealingTaskitem->m_strName);
			if(g_gtiGlobalTaskInfo->DealThreadNum>0)
			{
				RemoveThreadListIndex(dealResult->ThreadNum);
				g_gtiGlobalTaskInfo->DealThreadNum--;
			}
			return;
		}

		if(TASK_STATUS::SUSPENDED != recState && TASK_STATUS::WAITING != recState&& TASK_STATUS::FINISHED != recState)
		{
			//非用户暂停则任务失败	
			if(dealResult->userpause&&recState==PROCEDING)
				g_gtiGlobalTaskInfo->m_tlTaskList.SetTaskState(dealResult->taskId,WAITING);
			else
				g_gtiGlobalTaskInfo->m_tlTaskList.SetTaskState(dealResult->taskId,FAILED);
		}

		if(obServer)
		{
			if(pitem)
			{
				if(pitem->m_uStatus==FAILED)
					obServer->TaskFailNotify(pitem,deviceId);
				else if(pitem->m_uStatus==FINISHED)
					obServer->TaskFinishedNotify(pitem,deviceId);
				else
				{
					obServer->TaskUpdatedSpeedNotify(pitem,deviceId);
				}
			}
		}
	}
	else
	{
		dealResult->DealingTaskitem->hasProcing=false;
		g_gtiGlobalTaskInfo->m_tlTaskList.SetTaskState(dealResult->taskId, TASK_STATUS::FINISHED);
		CTaskItem * pitem = dealResult->Taskitem;
		if(pitem)
			pitem->hasProcing=false;
		if(obServer)
		{
			if(pitem)
			{
				pitem->SetEndTimer();
				EnterCriticalSection(&_m_csSaveTaskLock);
				g_gtiGlobalTaskInfo->m_tlTaskList.TASK_LOCK();
				TaskHistory::SaveTaskToProfile(g_gtiGlobalTaskInfo->m_tlTaskList.getTaskList());
				g_gtiGlobalTaskInfo->m_tlTaskList.TASK_UNLOCK();
				LeaveCriticalSection(&_m_csSaveTaskLock);
				obServer->TaskFinishedNotify(pitem, deviceId);
			}
		}	
	}
	dealResult->DealingTaskitem->hasProcing=false;
	if(g_gtiGlobalTaskInfo->DealThreadNum>0)
	{
		RemoveThreadListIndex(dealResult->ThreadNum);
		LOG_DEBUG(L"on finish end:"+CStrOperation::IntToWString(g_gtiGlobalTaskInfo->DealThreadNum));
		g_gtiGlobalTaskInfo->DealThreadNum--;
	}
	g_gtiGlobalTaskInfo->StartThread();
}

BOOL TaskControl::ExitTaskDealThread(LPVOID info)
{INTERFUN;
	g_gtiGlobalTaskInfo->SetThreadExitEnable();
	g_gtiGlobalTaskInfo->ClearTask();
	g_gtiGlobalTaskInfo->StartThread();
	return TRUE;
}

void TaskControl::Init(void)
{INTERFUN;
	wstring tempPath;
	DWORD bufferSize = 255;
	wchar_t buffer[255];
	DWORD ret = GetTempPath(bufferSize, buffer);
	tempPath.assign(buffer);
	tempPath.append(USERDATA_FOLDER);
	CFileOperation::MakeLocalDir(tempPath);

	InitializeCriticalSection(&_m_csThreadIndexLock);
	InitializeCriticalSection(&_m_csSaveTaskLock);
	ThreadTreeControl::InitThreadTreeControl(tempPath.c_str());
	g_gtiGlobalTaskInfo = new GlobalTaskInfo();

	if(NULL == g_tcTaskThreadControl)
	{
		g_tcTaskThreadControl = new ThreadControl;
		ThreadTreeControl::TCreateThread(g_tcTaskThreadControl, TaskControl::TaskDealThread, TaskControl::ExitTaskDealThread, NULL, "task thread");
	}

	stuTaskHistroy TaskHis;

	if(TaskHistory::GetProfileList(TaskHis))
	{
		for(size_t i=0;i<TaskHis.nTaskNum;i++)
		{
			CString csTasksMsg = TaskHis.TasksMsg.at(i).c_str();
			wstring wsTasksMsg = csTasksMsg.GetBuffer();
			if(wsTasksMsg.empty())
				continue;
			CTaskItem taskitem(0,TaskHis.TaskName.at(i).c_str(),(wchar_t *)wsTasksMsg.c_str(), (TASK_KIND)_wtoi(TaskHis.Kind.at(i).c_str()),NULL);
			taskitem.m_uStatus = (TASK_STATUS)_wtoi(TaskHis.TaskStatus.at(i).c_str());
			if(taskitem.m_uStatus == WAITING || taskitem.m_uStatus == PROCEDING )
			{
				taskitem.m_uStatus = TASK_STATUS::SUSPENDED;
			}
			taskitem.m_nProcess = _wtoi(TaskHis.percent.at(i).c_str());
			taskitem.m_TaskFileInfo->TotalSize = _wtoi(TaskHis.Totalsize.at(i).c_str());
			taskitem.m_strNote = TaskHis.Note.at(i).c_str();
			taskitem.m_TaskFileInfo->savePath = TaskHis.SaveFileName.at(i).c_str();
			taskitem.m_appVersion=TaskHis.Version.at(i).c_str();
			taskitem.m_platform=TaskHis.Platform.at(i).c_str();
			taskitem.m_fid=_wtoi(TaskHis.fids.at(i).c_str());
			taskitem.m_tEnd=_wtoi64(TaskHis.finishedtimes.at(i).c_str());
			taskitem.m_fileguid=TaskHis.fileguids.at(i).c_str();
			if (TaskHis.islegal.at(i)==L"TRUE")
			{
				taskitem.m_isLegalSoft=true;
			}
			else
				taskitem.m_isLegalSoft=false;
			
			if(taskitem.m_TaskFileInfo->savePath.empty()||taskitem.m_strName.empty())
				continue;

			if(g_gtiGlobalTaskInfo->m_tlTaskList.AddItem(taskitem) == -1)
			{
				//if(taskitem.m_uStatus  == PROCEDING)
				//	TaskControl::g_gtiGlobalTaskInfo->StartThread();				
			}			
		}
	}

	vector<wstring> deviceIds=DEVICE_MANAGER->GetAllDevice();
	for (size_t i=0;i<deviceIds.size();i++)
	{
		GlobalTaskInfo * gtask=TaskControl::GetInstallTaskIndexByDevice(deviceIds[i]);
		if (!gtask)
		{
			TaskControl::OnDeviceConnect(DEVICE_MANAGER->FindDevice(deviceIds[i]));
		}
	}
}

void TaskControl::Destroy(void)
{INTERFUN;
	TaskHistory::SaveTaskToProfile(g_gtiGlobalTaskInfo->m_tlTaskList.getTaskList());
	if(NULL != g_tcTaskThreadControl)
	{
		g_gtiGlobalTaskInfo->SetThreadExitEnable();
	//	g_gtiGlobalTaskInfo->ClearTask();

	//	g_tcTaskThreadControl->DeleteThread(TRUE);
	//	delete g_tcTaskThreadControl;
	//	g_tcTaskThreadControl = NULL;
	}
	//SAFE_DELETE(g_gtiGlobalTaskInfo);
	for(size_t i=0;i<g_tcInstallTaskThreadControl.size();i++)
	{
		GlobalTaskInfo* installTask = g_gtiGlobalInstallTaskInfo.at(i);
		installTask->SetThreadExitEnable();
		//installTask->ClearTask();

		ThreadControl* installthread = g_tcInstallTaskThreadControl.at(i);
	//	installthread->DeleteThread(TRUE);
	//	SAFE_DELETE(installthread);		
	//	SAFE_DELETE(installTask);
	}
	g_gtiGlobalInstallTaskInfo.clear();
	g_tcInstallTaskThreadControl.clear();
	DeleteCriticalSection(&_m_csThreadIndexLock);
	DeleteCriticalSection(&_m_csSaveTaskLock);
	//ThreadTreeControl::DestroyThreadTreeControl();
}
bool TaskControl::CheckDealingTaskEnable(TaskDealParm *taskParm)
{INTERFUN;
	if(!g_gtiGlobalTaskInfo)
		return true;
	//任务失败，可能为用户终止也可能其他原因
	TASK_STATUS recState;
	//获取任务执行状态判断是否用户暂停
	if(!g_gtiGlobalTaskInfo->m_tlTaskList.GetTaskState(taskParm->Taskitem->m_uID, recState))
	{
		//taskParm->userpause=true;
		//获取状态失败
		return true;
	}

	if(TASK_STATUS::PROCEDING != recState)
	{
		taskParm->userpause=true;
		return true;
	}

	return false;
}

UINT TaskControl::DealGlobalTask(LPVOID lparm)
{INTERFUN;
	TaskDealParm *dealParm = (TaskDealParm *)lparm;
	//设置任务执行信号量为无信号状态,执行完任务后将设置为有信号状态
	g_gtiGlobalTaskInfo->m_dtiDealingTaskInfo[dealParm->ThreadNum].m_iTotalCount = 0;
	g_gtiGlobalTaskInfo->m_dtiDealingTaskInfo[dealParm->ThreadNum].m_iDealedCount = 0;
	
	UINT ret = TASK_DEAL_SUC;
	if(TASK_KIND::URL_DOWNLAOD == dealParm->DealingTaskitem->m_uTaskType)
	{
		ret = TaskControl::DealDownLoad(dealParm);
	}
	else
	{
		//unknown task
		SET_DEALING_TASK_NOTE(_T("未知任务类型处理！"),dealParm->ThreadNum);
	}
	dealParm->Result = ret;
	
	if( dealParm->onfinish!=NULL)
	{
		dealParm->onfinish(dealParm);		
	}
	else
	{
		SET_DEALING_TASK_NOTE(_T("ERROR！"),dealParm->ThreadNum);
	}
	SAFE_DELETE(dealParm);
	return ret;

}
UINT TaskControl::ReportProcess(LPVOID parm)
{INTERFUN;
	if(obServer)
		obServer->TaskUpdatedSpeedNotify((CTaskItem *)parm,L"");
	return 0;
}

UINT TaskControl::DealDownLoad(TaskDealParm *taskParm)
{INTERFUN;
	//遍历需要上传的任务中的文件ID，对比数据库中该文件上传的时间，当文件修改过则进行上传
	bool getPathSuc = false;
	UINT finalResult = TASK_DEAL_SUC;		//默认设置任务执行完成

	//CTaskItem TaskItem;
	CHECK_TASK_AND_RETURN(taskParm);

	//TaskFileInfo *info = taskParm->DealingTaskitem->m_TaskFileInfo;
	CTaskItem * pitem = taskParm->Taskitem;
	RETURN_FALSE_IF(!pitem);
	wstring surl=pitem->m_TaskFileInfo->sStrMes;
	wstring pathstr=pitem->m_TaskFileInfo->savePath;
	//unsigned long *downloaded = &g_gtiGlobalTaskInfo->m_dtiDealingTaskInfo.m_tiTaskItem.m_nProcess;
	try
	{
		if(!fnHttpDownload(surl, 
			pathstr,
			pitem,taskParm,CheckDealingTaskEnable))
							return TASK_DEAL_FAIL;
	}
	catch (...)
	{
		int a=0;
		LOG_DEBUG(L"fnHttpDownload exception");
		return TASK_DEAL_FAIL;
	}
	return TASK_DEAL_SUC;
}

int filter(unsigned int code, struct _EXCEPTION_POINTERS *p)
{
	switch(code)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		return EXCEPTION_EXECUTE_HANDLER;
	default:
		return EXCEPTION_CONTINUE_SEARCH;
	};
}

void TaskControl::DealInstallTask(LPVOID lparm)
{INTERFUN;
	RETURN_IF(!lparm);
	TaskDealParm *dealParm = (TaskDealParm *)lparm;

	UINT ret = TASK_DEAL_FAIL;
	__try
	{
		ret = DealInstall(lparm);
	}
	__except(filter(GetExceptionCode(), GetExceptionInformation()))
	{
	}

	dealParm->Result = ret;

	if( dealParm->onfinish != NULL)
	{
		dealParm->onfinish(dealParm);		
	}
}

const wchar_t* IOS_INSTALLFAIL_ERRS[]={
L"固件版本不符合要求",
L"设备不适合正版授权(暂仅支持IOS4.0及以上版本)",
L"连接方式不是USB",
L"iTunes版本过低",
L"未安装iTunes",
L"简易版本iTunes",
L"未安装AppSync",
L"设备禁止安装应用",
L"应用程序包不完整",
L"应用程序包无法解压",
L"此软件仅适用于iPad",
L"非越狱设备暂不提供安装应用功能",
L"非越狱设备不支持安装非正版应用"
};


UINT TaskControl::DealInstall(LPVOID lparm)
{INTERFUN;
	RETURN_VALUE_IF(!lparm,TASK_DEAL_FAIL);
	TaskDealParm *dealParm = (TaskDealParm *)lparm;
	GlobalTaskInfo * gGlobalTaskInfo = GetInstallTaskIndexByDevice(dealParm->deviceID);// (GlobalTaskInfo *)g_gtiGlobalInstallTaskInfo.at(dealParm->ThreadNum);
	RETURN_VALUE_IF(!gGlobalTaskInfo,TASK_DEAL_FAIL);
	CTaskItem * pitem = dealParm->Taskitem;  //gGlobalTaskInfo->m_tlTaskList.GetTaskByID(gGlobalTaskInfo->m_dtiDealingTaskInfo[0].m_tiTaskItem.m_uID);
	RETURN_VALUE_IF(!pitem,TASK_DEAL_FAIL);

	wstring deviceid=dealParm->deviceID;
	wstring appname=pitem->m_strName;
	UINT apptaskId=pitem->m_uID;
	wstring appsavepath=pitem->m_TaskFileInfo->savePath;

	AppData* retappdata=NULL;
	try
	{
		CString bufformat;bufformat.Format(L"TaskControl::DealInstall begin install:%s on device:%s",appname.c_str(),deviceid.c_str());
		LOG_DEBUG(bufformat.GetBuffer());
		CommandExecuteResult result=COMMAND_EXECUTE_UNKNOWN;
		if(pitem->m_uTaskType == INSTALL)
		{
			if(pitem->m_TaskFileInfo&&!CFileOperation::IsFileExist(appsavepath.c_str()))
			{
				RETURN_VALUE_IF(!DEVICE_MANAGER->FindDevice(deviceid),TASK_DEAL_FAIL);
				pitem->m_installerror=L"安装文件不存在";
				pitem->m_uStatus=FAILED;
				if(obServer&&DEVICE_MANAGER->FindDevice(deviceid))
					obServer->TaskFailNotify(pitem, deviceid);
				pitem->hasProcing=false;
				return TASK_DEAL_FAIL;
			}
			if(pitem->m_strName.empty()||pitem->m_appVersion.empty())
			{
				wchar_t buff[MAX_PATH] = {0};
				::GetTempPath(MAX_PATH, buff);
				wstring tmppath=buff;
				RETURN_VALUE_IF(!DEVICE_MANAGER->FindDevice(deviceid),TASK_DEAL_FAIL);
				AppData* pdata=AppManager::GetAppInfoByFile(pitem->m_TaskFileInfo->savePath,tmppath,true);
				if(pdata)
				{
					RETURN_VALUE_IF(!DEVICE_MANAGER->FindDevice(deviceid),TASK_DEAL_FAIL);
					pitem->m_appVersion=pdata->_strVersion;
					pitem->m_strName=pdata->_AppName.empty()?CFileOperation::GetFileNameNoExt(pitem->m_TaskFileInfo->savePath).c_str():pdata->_AppName.c_str();
					pitem->m_appiconpath=pdata->_strIconPCPath;
					SAFE_DELETE(pdata);
				}
			}
			LOG_DEBUG(L"startinstall ");
			//LOG_DEBUG(pitem->m_strName.GetBuffer());
			RETURN_VALUE_IF(!DEVICE_MANAGER->FindDevice(deviceid),TASK_DEAL_FAIL);
			wstring installerr;
			pitem->m_installerror=L"";
			while (dealParm->_pDeviceData&&!dealParm->_pDeviceData->IsActivate())//等待设备激活
			{
				Sleep(100);
				if(!DEVICE_MANAGER->FindDevice(deviceid))
				{
					gGlobalTaskInfo->_beRemoved=true;
					return TASK_DEAL_FAIL;
				}
			}
			wstring ext=CFileOperation::GetFileExt(pitem->m_TaskFileInfo->savePath);
			if(ext==L"pxl"&&dealParm->_pDeviceData->GetDevInfo()->_ios_bJailBreaked)//pxl先等待安装守护
			{
				int nRetryTime = 0;
				while (!gGlobalTaskInfo->_iosdeamonInstalled)
				{
					DeviceData* pDeviceData = DEVICE_MANAGER->FindDevice(deviceid);
					if (pDeviceData)
					{
						const DeviceInfo* pInfo = pDeviceData->GetDevInfo();
						if (pInfo)
						{
							gGlobalTaskInfo->_iosdeamonInstalled = pInfo->_ios_daemonInstalled;
							if (gGlobalTaskInfo->_iosdeamonInstalled)
							{
								break;//如果守护已经安装while接下来的事情就不要做了～ by chen xiao yong
							}
						}
					}
					Sleep(50);
					if(!DEVICE_MANAGER->FindDevice(deviceid))
					{
						gGlobalTaskInfo->_beRemoved=true;
						return TASK_DEAL_FAIL;
					}
					wstring wsPath = CONFIG(_strAssistPath) +  L"IOS\\Deamon\\DeamonPackage.zip";
					if (CFileOperation::IsFileExist(wsPath.c_str()))
					{
						nRetryTime++;
						if (nRetryTime>600)//30s
						{
							pitem->m_uStatus = FAILED;
							pitem->m_installerror=L"您可试试重启一次设备";
							obServer->TaskFailNotify(pitem, deviceid);
							pitem->hasProcing=false;
							return TASK_DEAL_FAIL;
						}
					}
				}

				if(!dealParm->_pDeviceData->GetDevInfo()->_ios_daemonInstalled)//pxl没装上守护直接失败
				{
					pitem->m_uStatus = FAILED;
					pitem->m_installerror=L"您可试试重启一次设备";
					obServer->TaskFailNotify(pitem, deviceid);
					pitem->hasProcing=false;
					return TASK_DEAL_FAIL;
				}
			}

			result=dealParm->_pDeviceData->GetAppManager()->InstallApp( appsavepath.c_str(),installerr,retappdata,
																		pitem->_pObserver,false,true,
																		pitem->InstallToSdCard,false,NULL);
			LOG_DEBUG(L"DealInstall:InstallApp End");
			
			//LOG_DEBUG(L" is device moved and reInsert");
			RETURN_VALUE_IF(!gGlobalTaskInfo||gGlobalTaskInfo->_beRemoved,TASK_DEAL_FAIL);
			LOG_DEBUG(L"DealInstall device not be removed");
			if(retappdata)
			{
				pitem->m_installerror=installerr;
				LOG_DEBUG(L"DealInstall pitem->m_installerror=installerr");
				SAFE_DELETE(pitem->appData);
				LOG_DEBUG(L"DealInstall SAFE_DELETE(pitem->appData)");
				pitem->appData=new AppData(retappdata);
				LOG_DEBUG(L"DealInstall pitem->appData=new AppData(retappdata)");
			}
			LOG_DEBUG(L"DealInstall go end if(retappdata)");
		}
		else if(pitem->m_uTaskType == MOVEANDROIDAPP)
		{
			if(pitem->appData)
			{
				while (dealParm->_pDeviceData&&!dealParm->_pDeviceData->IsActivate())
				{
					Sleep(100);
					if(!DEVICE_MANAGER->FindDevice(deviceid))
					{
						gGlobalTaskInfo->_beRemoved=true;
						pitem->m_uStatus = WAITING;
						return TASK_DEAL_FAIL;
					}
				}

				wstring installerr;
				result=dealParm->_pDeviceData->GetAppManager()->MoveToApp(pitem->appData,installerr, retappdata, pitem->_pObserver);

				DeviceData* pDevice = DEVICE_MANAGER->FindDevice(dealParm->_pDeviceData->GetId());

				RETURN_VALUE_IF(!pDevice || !gGlobalTaskInfo||gGlobalTaskInfo->_beRemoved,TASK_DEAL_FAIL);
				pitem->m_installerror=installerr;
				if(retappdata&&result==COMMAND_EXECUTE_SUCCESS)
				{
					SAFE_DELETE(pitem->appData);
					pitem->appData=new AppData(retappdata);
				}
			}
		}	
		if(result==COMMAND_EXECUTE_SUCCESS||result==COMMAND_EXECUTE_SUCCESS_AuthorizorFaile)
		{
			RETURN_VALUE_IF(!DEVICE_MANAGER->FindDevice(deviceid),TASK_DEAL_SUC);
			pitem->m_uStatus=FINISHED;
			pitem->m_appid=retappdata->_packageName;
			if(result==COMMAND_EXECUTE_SUCCESS_AuthorizorFaile)
				pitem->m_installerror=L"若闪退，请闪退修复";//by12.11.30 11:30 修改bug25179
			if(obServer&&DEVICE_MANAGER->FindDevice(deviceid))
			{
				if(dealParm->_pDeviceData->GetDevInfo()->_deviceType==Android)
					pitem->m_installerror=L"";
				obServer->TaskFinishedNotify(pitem, deviceid);
			}
			pitem->hasProcing=false;
			CString bufformaterr;
			bufformaterr.Format(L"TaskControl::DealInstall install success:%s on device:%s",appname.c_str(),deviceid.c_str());
			LOG_DEBUG(bufformaterr.GetBuffer());

			return TASK_DEAL_SUC;
		}
		else
		{
			RETURN_VALUE_IF(!DEVICE_MANAGER->FindDevice(deviceid),TASK_DEAL_FAIL);
			pitem->m_uStatus=FAILED;
			if(retappdata)
			{
				pitem->m_appid=retappdata->_packageName;
				SAFE_DELETE(retappdata);
			}
			if(obServer&&DEVICE_MANAGER->FindDevice(deviceid))
			{
				if(pitem->m_installerror.empty())
				{
					switch(result)
					{
						case COMMAND_EXECUTE_BREAK:         // 执行失败
							pitem->m_installerror=L"执行失败";
							break;
						case COMMAND_EXECUTE_UNSUPPORT:      // 不支持
							pitem->m_installerror=L"固件版本不支持";
							break;
						case COMMAND_EXECUTE_NOFOUND:        // 找不到可操作的数据
							pitem->m_installerror=L"找不到可操作的数据";
							break;
						case COMMAND_EXECUTE_ARGUMENTERROR:  // 参数错误
							pitem->m_installerror=L"参数错误";
							break;
						case COMMAND_THREAD_OVER_LIMIT:      // 并发超过限制
							pitem->m_installerror=L"并发超过限制";
							break;
						case COMMAND_EXECUTE_PARSEPKGERROR:
							pitem->m_installerror=L"解析包出错";
							break;
						default:
							pitem->m_installerror=L"原因未知";
							break;
					}
				}
				obServer->TaskFailNotify(pitem, deviceid);
			}
			pitem->hasProcing=false;
			CString bufformaterr;
			bufformaterr.Format(L"TaskControl::DealInstall install fail:%s on device:%s",appname.c_str(), deviceid.c_str());
			LOG_DEBUG(bufformaterr.GetBuffer());
			return TASK_DEAL_FAIL;
		}	
	}
	catch(AssistantException e)
	{
		//pitem->Unlock();
		RETURN_VALUE_IF(!DEVICE_MANAGER->FindDevice(deviceid),TASK_DEAL_FAIL);
		if(retappdata)
		{
			pitem->m_appid=retappdata->_packageName;
			SAFE_DELETE(retappdata);
		}
		switch (e.GetErrorCode())
		{
			case AssistantException::FILE_NOT_FOUND:
					pitem->m_installerror=L"安装文件不存在";
					break;
			default:
				{
					if(e.GetErrorCode()>=AssistantException::NotValideToAuthorizeByDeviceVersion &&e.GetErrorCode()<=AssistantException::NotValideToAuthorizeByItunesIsSimple)
					{
						if(e.GetErrorCode()>AssistantException::NotValideToAuthorizeByDeviceVersion)
							pitem->m_installerror=L"授权失败，固件版本太低";
						else
						{
							pitem->m_installerror=L"授权失败，";
							pitem->m_installerror+=IOS_INSTALLFAIL_ERRS[e.GetErrorCode()-AssistantException::PRODUCT_VERSION_NOT_AGREE];
						}
						pitem->m_uStatus=FINISHED;
						if(obServer&&DEVICE_MANAGER->FindDevice(deviceid))
						{
							obServer->TaskFinishedNotify(pitem, deviceid);
						}
						pitem->hasProcing=false;
						return TASK_DEAL_SUC;
					}
					if(e.GetErrorCode()>=AssistantException::PRODUCT_VERSION_NOT_AGREE&&e.GetErrorCode()<=AssistantException::NoJailBreakedNoLegalSoft)
					{
						pitem->m_installerror=IOS_INSTALLFAIL_ERRS[e.GetErrorCode()-AssistantException::PRODUCT_VERSION_NOT_AGREE];
					}
					else
						pitem->m_installerror=L"原因未知";
				}
			break;
		}
		if(obServer&&DEVICE_MANAGER->FindDevice(deviceid))
		{
			pitem->m_uStatus=FAILED;
			obServer->TaskFailNotify(pitem, deviceid);
		}
		pitem->hasProcing=false;
		CString bufformat;
		bufformat.Format(L"TaskControl::DealInstall install AssistantException:%s on device:%s",appname.c_str(),deviceid.c_str());
		LOG_DEBUG(bufformat.GetBuffer());
		return TASK_DEAL_FAIL;
	}
    catch (...)
    {
		//pitem->Unlock();
		RETURN_VALUE_IF(!DEVICE_MANAGER->FindDevice(deviceid),TASK_DEAL_FAIL);
		/*if(retappdata)
		{
			pitem->m_appid=retappdata->_packageName;
			SAFE_DELETE(retappdata);
		}*/
		if(obServer&&DEVICE_MANAGER->FindDevice(deviceid))
		{
			pitem->m_uStatus=FAILED;
			obServer->TaskFailNotify(pitem, dealParm->_pDeviceData->GetId());
		}
		pitem->hasProcing=false;
		CString bufformat;
		bufformat.Format(L"TaskControl::DealInstall install other Exception:%s on device:%s",appname.c_str(),deviceid.c_str());
		LOG_DEBUG(bufformat.GetBuffer());
        return TASK_DEAL_FAIL;
    }
	return TASK_DEAL_SUC;
}
//任务处理线程启动
DWORD TaskControl::InstallTaskDealThread(LPVOID info)
{INTERFUN;
	TaskDealParm *parm = (TaskDealParm *)info;
	RETURN_VALUE_IF(!parm||!parm->_pDeviceData, 0);
	GlobalTaskInfo * gGlobalTaskInfo =  (GlobalTaskInfo *)GetInstallTaskIndexByDevice(parm->deviceID);
	if(!gGlobalTaskInfo)
		return 0;
	try		
	{
		while(gGlobalTaskInfo->m_bDealEnable)
		{
			//当需要继续执行时，无限循环
			gGlobalTaskInfo->WaitForStart();

			if(!gGlobalTaskInfo->m_bDealEnable)
			{
				//需要退出时
				gGlobalTaskInfo->TaskEnd=true;
				return 0;
			}

			//if(gGlobalTaskInfo->DealThreadNum>maxnum)
			//	continue;
			UINT nMaxThreadNum =1;
			if(!DEVICE_MANAGER->FindDevice(gGlobalTaskInfo->deviceId))
				return 0;
			nMaxThreadNum=(UINT)(DEVICE_MANAGER->FindDevice(gGlobalTaskInfo->deviceId)->GetDevInfo()->_deviceType==Ios?3:3);
		
			while(gGlobalTaskInfo->DealThreadNum < nMaxThreadNum)
			{
				//UINT maxnum=DEVICE_MANAGER->FindDevice(gGlobalTaskInfo->deviceId)->GetDevInfo()->_deviceType==Ios?1:3;
				int NewIndex = GetFreeInstallThreadIndex(nMaxThreadNum,gGlobalTaskInfo->deviceId);
				LOG_DEBUG(gGlobalTaskInfo->deviceId+L"installthreadindex安装索引"+CStrOperation::IntToWString(NewIndex));
				if(NewIndex==-1)
					break;
				//任务处理开始
				if(!gGlobalTaskInfo->m_tlTaskList.GetFirstWaitingItemAndSetProceding(&gGlobalTaskInfo->m_dtiDealingTaskInfo[NewIndex].m_tiTaskItem))
					break;
				//if(gGlobalTaskInfo->DealThreadNum>maxnum)
				//	break;
				//当获取第一个等待执行的任务成功后，执行任务，否则继续等待
				//执行任务，并根据返回值设置任务状态
				UINT dealResult = TASK_DEAL_SUC;
				try
				{
					parm->onfinish = NULL;
					parm->Result = dealResult;
					TaskDealParm* newparm=new TaskDealParm();
					newparm->ThreadNum=NewIndex;
					newparm->_pDeviceData=DEVICE_MANAGER->FindDevice(gGlobalTaskInfo->deviceId);
					newparm->deviceID=gGlobalTaskInfo->deviceId;
					newparm->onfinish=OnFinishedInstall;
					newparm->DealingTaskitem=&gGlobalTaskInfo->m_dtiDealingTaskInfo[NewIndex].m_tiTaskItem;
					newparm->Taskitem=gGlobalTaskInfo->m_tlTaskList.GetTaskByID(newparm->DealingTaskitem->m_uID);
					newparm->taskId = newparm->DealingTaskitem->m_uID;
					gGlobalTaskInfo->m_dtiDealingTaskInfo[NewIndex].ThreadNum=NewIndex;
					_parmInstall.push_back(newparm);
					if(_installThreadIndex.find(gGlobalTaskInfo->deviceId)!=_installThreadIndex.end())
					{
						_installThreadIndex[gGlobalTaskInfo->deviceId].push_back(NewIndex);
					}
					else
					{
						vector<int> indexs;indexs.push_back(NewIndex);
						_installThreadIndex[gGlobalTaskInfo->deviceId]=indexs;
					}
					
					//dealResult = DealInstall((LPVOID)parm);
					CreateThread(0,0,(LPTHREAD_START_ROUTINE)DealInstallTask,(LPVOID)newparm,0,NULL);								
					gGlobalTaskInfo->DealThreadNum++;	
					if (gGlobalTaskInfo->DealThreadNum >= nMaxThreadNum)					
						break;
					//if(gGlobalTaskInfo->DealThreadNum>maxnum)
					//	break;
				}
				catch(...)
				{
					if(gGlobalTaskInfo->m_bDealEnable)
					{
						//出现执行异常设置状态为失败
						//SET_DEALING_TASK_NOTE(_T("执行任务出现无法控制的异常，导致执行失败"));
						dealResult = TASK_DEAL_FAIL;
					}
					else
					{
						gGlobalTaskInfo->TaskEnd=true;
						return 0;
					}
				}
			}
		}
	}
	catch(...)
	{
		if(!gGlobalTaskInfo->m_bDealEnable)
		{
			//程序需要退出时不提示
			gGlobalTaskInfo->TaskEnd=true;
			return 0;
		}
	}
	//SetEvent()
	gGlobalTaskInfo->TaskEnd=true;
	return 0;
}


//退出任务处理线程
BOOL TaskControl::ExitInstallTaskDealThread(LPVOID info)
{INTERFUN;
	TaskDealParm *parm = (TaskDealParm *)info;
	GlobalTaskInfo * gGlobalTaskInfo =  (GlobalTaskInfo *)g_gtiGlobalInstallTaskInfo.at(parm->ThreadNum);

	gGlobalTaskInfo->SetThreadExitEnable();
	gGlobalTaskInfo->ClearTask();
	gGlobalTaskInfo->StartThread();
	SAFE_DELETE(parm);
	return TRUE;
}

//设备接入需要启动安装任务处理线程
BOOL TaskControl::OnDeviceConnect(DeviceData* _pDeviceData)
{INTERFUN;
	if(!_pDeviceData)
		return FALSE;

	int findTasknum=-1;
	size_t i = 0;
	TaskDealParm *parm=NULL;
	for(i = 0; i < ParmList.size();i++)
	{
		TaskDealParm *findparm = (TaskDealParm *)ParmList.at(i);
		if(!findparm || !findparm->_pDeviceData)
			continue;
		if(findparm->deviceID == _pDeviceData->GetId())
		{
			findTasknum = findparm->ThreadNum;
			findparm->_pDeviceData=_pDeviceData;
			parm=findparm;
			break;
		}
	}
	if(findTasknum>-1&&g_gtiGlobalInstallTaskInfo.size()>i)
	{
		GlobalTaskInfo* gtask=g_gtiGlobalInstallTaskInfo.at(i);
		gtask->m_bDealEnable=true;
		if( _installThreadIndex.find(_pDeviceData->GetId()) != _installThreadIndex.end())
			_installThreadIndex[_pDeviceData->GetId()].clear();
		gtask->DealThreadNum=0;
		gtask->_beRemoved=false;
		return TRUE;
	}
	
	int TaskNum =_TaskDeviceNum++; 
	parm  = new TaskDealParm();
	parm->ThreadNum = TaskNum;
	parm->_pDeviceData = _pDeviceData;
	parm->deviceID = parm->_pDeviceData->GetId();

	CString buf;buf.Format(L"OnDeviceConnect (device %s, TaskNum %d)",parm->deviceID.c_str(),TaskNum);
	LOG_DEBUG(buf.GetBuffer());
	ParmList.push_back(parm);

	GlobalTaskInfo * gGlobalTaskInfo = new GlobalTaskInfo();
	gGlobalTaskInfo->TaskNum=TaskNum;
	gGlobalTaskInfo->deviceId=parm->deviceID;
	g_gtiGlobalInstallTaskInfo.push_back(gGlobalTaskInfo);

	ThreadControl * gThreadControl = new ThreadControl();
	string threadName = "install task thread ";
	char buff[4];
	_itoa(TaskNum,buff,10);
	threadName+=buff;
	gThreadControl->ThreadNum=TaskNum;
	g_tcInstallTaskThreadControl.push_back(gThreadControl);

	//_parmInstall.push_back(parm);
	ThreadTreeControl::TCreateThread(gThreadControl, TaskControl::InstallTaskDealThread, TaskControl::ExitInstallTaskDealThread, (LPVOID)parm,(char *)threadName.c_str());

	return TRUE;
}
//设备断开停止安装任务线程
BOOL TaskControl::OnDeviceDisconnect(const wchar_t* Devid)
{INTERFUN;
	int Tasknum=-1;
	for(size_t i = 0; i < ParmList.size();i++)
	{
		TaskDealParm *parm = (TaskDealParm *)ParmList.at(i);
		if(parm->deviceID == Devid)
		{
			Tasknum = parm->ThreadNum;
			//SAFE_DELETE(parm);
			//ParmList.erase(ParmList.begin()+i);
			break;
		}
	}    
	if(Tasknum<0)
		return FALSE;	

//查找2个task
	ThreadControl * gGlobalThreadControl = NULL;
	GlobalTaskInfo * gGlobalTaskInfo = NULL;
	vector<ThreadControl*>::iterator itThread;
	vector<GlobalTaskInfo*>::iterator itTask;
	for(itThread=g_tcInstallTaskThreadControl.begin();itThread!=g_tcInstallTaskThreadControl.end();itThread++)
	{
		ThreadControl *TempIt = *itThread;
		if(TempIt->ThreadNum == Tasknum)
		{
			gGlobalThreadControl=*itThread;
			break;
		}
	}
	for(itTask=g_gtiGlobalInstallTaskInfo.begin();itTask!=g_gtiGlobalInstallTaskInfo.end();itTask++)
	{
		GlobalTaskInfo *TempIt = *itTask;
		if(TempIt->TaskNum == Tasknum)
		{
			gGlobalTaskInfo=*itTask;
			break;
		}
	}
	//清除任务
	if(NULL != gGlobalThreadControl && NULL != gGlobalTaskInfo)
	{
		gGlobalTaskInfo->SetThreadExitEnable();
		gGlobalTaskInfo->_beRemoved=true;
		//gGlobalTaskInfo->ClearTask();
// 		int count=0;
// 		while(gGlobalTaskInfo->TaskEnd==false)
// 		{
// 			if(count>500)
// 				break;
// 			count++;
// 			Sleep(20);
// 		}

		//gGlobalThreadControl->DeleteThread(FALSE);
		//delete gGlobalThreadControl;
		//gGlobalThreadControl = NULL;
		//SAFE_DELETE(gGlobalTaskInfo);
		//g_gtiGlobalInstallTaskInfo.erase(itTask);
		//(*itThread)->DeleteThread(false);
		//g_tcInstallTaskThreadControl.erase(itThread);
		CString buf;buf.Format(L"OnDeviceDisconnect (device %s, TaskNum %d)",Devid,Tasknum);
		LOG_DEBUG(buf.GetBuffer());
	}
	if( _installThreadIndex.find(Devid) != _installThreadIndex.end())
		_installThreadIndex[Devid].clear();

	return TRUE;
}
GlobalTaskInfo * TaskControl::GetInstallTaskIndexByDevice(wstring deviceId)
{INTERFUN;
	int Tasknum=-1;
	size_t i = 0;
	for(i = 0; i < ParmList.size();i++)
	{
		TaskDealParm *parm = (TaskDealParm *)ParmList.at(i);
		if(!parm || parm->deviceID.empty())
			continue;
		if(parm->deviceID == deviceId)
		{
			Tasknum = parm->ThreadNum;
			break;
		}
	}
	if(Tasknum>=0&&g_gtiGlobalInstallTaskInfo.size()>i)
		return g_gtiGlobalInstallTaskInfo.at(i);

	return NULL;
}

void TaskControl::SetObserver(TaskCenterNotifyObserver *obj)
{INTERFUN;
	obServer = obj;
}

void TaskControl::OnFinishedInstall( LPVOID info )
{INTERFUN;
	TaskDealParm *dealpar = (TaskDealParm *)info;
	RETURN_IF(!dealpar);
	wstring deviceId=dealpar->deviceID;
	GlobalTaskInfo * gGlobalTaskInfo = GetInstallTaskIndexByDevice(deviceId);
	RETURN_IF(!gGlobalTaskInfo);
	UINT taskId= dealpar->taskId;
	if(gGlobalTaskInfo->DealThreadNum>0)
		gGlobalTaskInfo->DealThreadNum--;
	dealpar->DealingTaskitem->hasProcing=false;
	wstring ext= CFileOperation::GetFileExt(dealpar->DealingTaskitem->m_TaskFileInfo->savePath);
	if(CStrOperation::toLowerW(ext)==L"pxl")
		gGlobalTaskInfo->m_tlTaskList._bPxlRunning=false;
	CTaskItem* task=gGlobalTaskInfo->m_tlTaskList.GetTaskByID(taskId);
	RETURN_IF(!task);
	task->hasProcing=false;
	
	//设置任务执行状态
	if(TASK_DEAL_SUC != dealpar->Result)
	{
		if(task->m_uStatus==PROCEDING)
		{
			task->m_uStatus=FAILED;
			task->m_installerror=L"安装中断，可能设备被移除";
			//return;
		}
		if(!gGlobalTaskInfo->m_bDealEnable)
		{
			gGlobalTaskInfo->TaskEnd=true;
			return;
		}
	}
	else
	{
		//gGlobalTaskInfo->m_tlTaskList.SetTaskState(taskId, TASK_STATUS::FINISHED);
		if(!gGlobalTaskInfo->m_bDealEnable)
		{
			gGlobalTaskInfo->TaskEnd=true;
			return;
		}
	}
	if(DEVICE_MANAGER->FindDevice(deviceId))
	{
		RemoveInstallThreadListIndex(dealpar->ThreadNum,deviceId);
		gGlobalTaskInfo->StartThread();
	}
}

void TaskControl::SaveDownloadTaskInfoToLocal()
{INTERFUN;
	TaskHistory::SaveTaskToProfile(g_gtiGlobalTaskInfo->m_tlTaskList.getTaskList());
}

void GlobalTaskInfo::WaitForStart( void )
{INTERFUN;
	::WaitForSingleObject(_m_hDealEvent,INFINITE);
	::ResetEvent(_m_hDealEvent);
}
