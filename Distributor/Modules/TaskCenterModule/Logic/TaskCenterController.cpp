#include "stdafx.h"
#include "TaskCenterController.h"
#include "UI/MainView.h"
#include "UI/LeftView.h"

#include "TaskControl.h"
#include "Manager/ThreadManager.h"
#include "Logic/TaskThread.h"
#include "TaskModuleInnerMsg.h"
#include "TaskInstallObserver.h"
#include "UI/CommonUIFunc.h"
#include "Common/MyFileOperation.h"
#include "Common/SystemInfo.h"
#include "Module/Application/AppManager.h"
#include "UI/TaskCenterControlUI.h"

TaskCenterController::TaskCenterController()
{
	_pLeftView = new LeftView();
	_pMainView = new TaskCenterControlUI(_pLeftView);

	_curDeviceId =L"";
	_curTaskList =NULL;
	_unfinishedTaskCount = 0;
	_IconLoaded=false;
	_bExit = false;
}

TaskCenterController::~TaskCenterController()
{

}

bool TaskCenterController::Init(CContainerUI* pLeft,  CContainerUI* pContainer )
{
	TaskControl::Init();
	TaskControl::SetObserver(this);
	_pLeftView->Init(pLeft);
	_pMainView->Init(pContainer);
	return true;
}

void TaskCenterController::Show()
{
	_pLeftView->Show();
	_pMainView->Show();
}

void TaskCenterController::Hide()
{
	_pLeftView->Hide();
	_pMainView->Hide();
}



bool TaskCenterController::StartDownLoad( wstring url, wstring savepath,wstring appname/*=L""*/,wstring version/*=L""*/, wstring platform/*=L""*/,int fid/*=0*/, bool bUpdategrade/*=false*/,bool needInstall/*=false*/,wstring oldappfullpath/*=L""*/,bool isLegalSoft/*=false*/, TASK_UPGRADE_TYPE taskupgradetype/*=NOTUPGRADE*/, wstring appID/*=L""*/, void* pCpaInfo /*=NULL*/ )
{
	CTaskItem* finitem=TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.findSameItem(url.c_str(),savepath.c_str(),TASK_KIND::URL_DOWNLAOD,bUpdategrade);
	if(finitem)
	{
		finitem->m_fid=fid;
		finitem->m_isLegalSoft=isLegalSoft;
		finitem->m_appid=appID;
		if(NOTUPGRADE==finitem->m_taskupgradetype)
			finitem->m_taskupgradetype=taskupgradetype;
		else if(PIRACY2PIRACY==finitem->m_taskupgradetype && LEGAL2PIRACY==taskupgradetype 
			|| LEGAL2PIRACY==finitem->m_taskupgradetype && PIRACY2PIRACY==taskupgradetype)
		{
			finitem->m_taskupgradetype=BOTH2PIRACY;
		}
		else if(LEGAL2LEGAL==finitem->m_taskupgradetype && PIRACY2LEGAL==taskupgradetype 
			|| PIRACY2LEGAL==finitem->m_taskupgradetype && LEGAL2LEGAL==taskupgradetype )
		{
			finitem->m_taskupgradetype=BOTH2LEGAL;
		}
		if(bUpdategrade&&finitem->m_uStatus==FINISHED)
		{
			int taskId=finitem->m_uID;
			TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.DeleteTaskByID(finitem->m_uID);
			TaskControl::obServer->TaskDeleteNotify(taskId,L"");
		}
		else 
		{
			if(bUpdategrade)
				finitem->bUpadateGrade=bUpdategrade;
			StartTask(finitem->m_uID,L"");
			if (finitem->m_uID == 0)
			{
				return false;
			}
			else
			{
				return true;

			}
			//return finitem->m_uID;
		}
	}

	int index= TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.AddItem(appname.c_str(), url.c_str(),savepath.c_str(),TASK_KIND::URL_DOWNLAOD,0,NULL,version.c_str(),NULL,true,bUpdategrade, L"", pCpaInfo);
	if(index>-1)
	{
		TaskControl::g_gtiGlobalTaskInfo->StartThread();
		CTaskItem* litem =TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.GetTaskByID(index);
		litem->m_platform=platform;
		litem->m_needInstall=needInstall;
		litem->m_oldappfullpath=oldappfullpath;
		litem->m_fid=fid;
		litem->m_appid=appID;
		litem->m_taskupgradetype=taskupgradetype;
		litem->m_isLegalSoft=isLegalSoft;
		_unfinishedTaskCount++;
		UIInsertTask(litem);
		if (index == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
		//return index;
	}
	if (index == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
	//return (bool)index;
}

int TaskCenterController::DownLoadSoft( wstring url, wstring savepath,wstring appname/*=L""*/,wstring version/*=L""*/, wstring platform/*=L""*/,int fid/*=0*/,bool bUpdategrade/*=false*/,bool needInstall/*=false*/,wstring oldappfullpath/*=L""*/,bool isLegalSoft/*=false*/, TASK_UPGRADE_TYPE taskupgradetype/*=NOTUPGRADE*/, wstring appID/*=L""*/, void* pCpaInfo /*= NULL*/ )
{
	CTaskItem* finitem=TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.findSameItem(url.c_str(),savepath.c_str(),TASK_KIND::URL_DOWNLAOD,bUpdategrade);
	if(finitem)
	{
		finitem->m_fid=fid;
		finitem->m_isLegalSoft=isLegalSoft;
		finitem->m_appid=appID;
		if(NOTUPGRADE==finitem->m_taskupgradetype)
			finitem->m_taskupgradetype=taskupgradetype;
		else if(PIRACY2PIRACY==finitem->m_taskupgradetype && LEGAL2PIRACY==taskupgradetype 
			|| LEGAL2PIRACY==finitem->m_taskupgradetype && PIRACY2PIRACY==taskupgradetype)
		{
			finitem->m_taskupgradetype=BOTH2PIRACY;
		}
		else if(LEGAL2LEGAL==finitem->m_taskupgradetype && PIRACY2LEGAL==taskupgradetype 
			|| PIRACY2LEGAL==finitem->m_taskupgradetype && LEGAL2LEGAL==taskupgradetype )
		{
			finitem->m_taskupgradetype=BOTH2LEGAL;
		}
		if(bUpdategrade&&finitem->m_uStatus==FINISHED)
		{
			int taskId=finitem->m_uID;
			TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.DeleteTaskByID(finitem->m_uID);
			TaskControl::obServer->TaskDeleteNotify(taskId,L"");
		}
		else 
		{
			if(bUpdategrade)
				finitem->bUpadateGrade=bUpdategrade;
			StartTask(finitem->m_uID,L"");
			return finitem->m_uID;
		}
	}

	int index= TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.AddItem(appname.c_str(), url.c_str(),savepath.c_str(),TASK_KIND::URL_DOWNLAOD,0,NULL,version.c_str(),NULL,true,bUpdategrade, L"", pCpaInfo);
	if(index>-1)
	{
		TaskControl::g_gtiGlobalTaskInfo->StartThread();
		CTaskItem* litem =TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.GetTaskByID(index);
		litem->m_platform=platform;
		litem->m_needInstall=needInstall;
		litem->m_oldappfullpath=oldappfullpath;
		litem->m_fid=fid;
		litem->m_appid=appID;
		litem->m_taskupgradetype=taskupgradetype;
		litem->m_isLegalSoft=isLegalSoft;
		_unfinishedTaskCount++;
		UIInsertTask(litem);
		return index;
	}
	return index;
}

int  TaskCenterController::InstallApp( wstring appname, wstring appfilepath,wstring version, long filesize, wstring deviceId, bool onSDcard,wstring plat/*=L""*/,int fid/*=0*/,wstring casename/*=L""*/,wstring icon/*=L""*/, bool isLegalSoft/*=false*/ )
{
	if(deviceId.empty())return -1;
	DeviceData* device=DEVICE_MANAGER->FindDevice(deviceId);
	if(!device)return -1;
	//cpa软件则向服务端验证是否已安装过
	wstring wsIos_path = CONFIG(_strIos_CpaDownPath);
	wstring wsAndroid_path = CONFIG(_strAndroid_CpaDownPath);
	if (CFileOperation::GetFileDir(appfilepath)==wsIos_path
		||CFileOperation::GetFileDir(appfilepath)==wsAndroid_path)
	{
		DeviceInfo* pInfo = device->GetMutableDevInfo();
		if (pInfo)
		{
			//通过路径获得哈希值
			//string strHash = CFileOperation::GetFileMD5(appfilepath);
			//if (!strHash.empty())
			//{
			//	//向服务端验证是否已经装过cpa软件
			//	CheckCpaInstalledMsg ChkMsg;
			//	ChkMsg._strHash = strHash;
			//	if (L"Android" == plat)
			//	{
			//		wstring wsTemp = pInfo->_deviceIMEI.empty() ? pInfo->_deviceWiFiMac :  pInfo->_deviceIMEI;

			//		if ( wsTemp.empty() )
			//			wsTemp = pInfo->_strSerialNumber;

			//		ChkMsg._strImei = CCodeOperation::UnicodeToGB2312(wsTemp);
			//	}
			//	else
			//	{
			//		wstring wsTemp = device->GetId().empty() ? pInfo->_deviceIMEI : device->GetId();
			//		ChkMsg._strImei = CCodeOperation::UnicodeToGB2312(wsTemp);
			//	}

			//	ChkMsg.SendAndGetReturn();
			//	if (!ChkMsg._strReturn.empty())
			//	{//吐司
			//		CommonUIFunc::NoticeDialog(CCodeOperation::GB2312ToUnicode(ChkMsg._strReturn),ICON_WARNING);
			//	}
			//}
		}
	}

	wstring ext=CFileOperation::GetFileExt(appfilepath);
	wstring platform=L"";
	if(ext==L"ipa"||ext==L"deb"||ext==L"pxl")
	{
		platform=L"iPhone";
		if(device&&device->GetDevInfo()->_deviceType!=Ios)
		{
			DescCaseCount(deviceId,casename);
			return -1;
		}

		// 统计功能
		CStaticsticsMessage staticcticsMsg(Statistic_ID_InstallIosApp);
		staticcticsMsg.Send();
		//Statistic_StatFunc(Statistic_ID_InstallIosApp);
	}
	else if(ext==L"apk"||ext==L"npk")
	{
		platform=L"Android";
		if(device&&device->GetDevInfo()->_deviceType!=Android)
		{
			DescCaseCount(deviceId,casename);
			return -1;
		}

		// 统计功能
		CStaticsticsMessage staticcticsMsg2(Statistic_ID_InstallAndApp);
		staticcticsMsg2.Send();
		//Statistic_StatFunc(Statistic_ID_InstallAndApp);
	}
	if(!plat.empty())platform=plat;

	GlobalTaskInfo * gtask=TaskControl::GetInstallTaskIndexByDevice(deviceId);
	if (!gtask)
	{
		TaskControl::OnDeviceConnect(DEVICE_MANAGER->FindDevice(deviceId));
		gtask=TaskControl::GetInstallTaskIndexByDevice(deviceId);
	}
	if(gtask)
	{
		CTaskItem* finitem=gtask->m_tlTaskList.findSameItem(L"",appfilepath.c_str(),TASK_KIND::INSTALL,false,casename);
		if(finitem)
		{
			if(finitem->m_uStatus==FAILED)
			{
				gtask->m_tlTaskList.RetryItem(finitem->m_uID);
				gtask->StartThread();
			}
			return finitem->m_uID;
		}

		TaskInstallObserver* observer=new TaskInstallObserver(deviceId,this);
		int index=gtask->m_tlTaskList.AddItem(appname.c_str(),L"",appfilepath.c_str(),INSTALL,filesize,DEVICE_MANAGER->FindDevice(deviceId), version.c_str(),observer,onSDcard,false,casename);
		if(index>-1)
		{
			_unfinishedTaskCount++;
			observer->SetTaskId(index);
			CTaskItem* litem =gtask->m_tlTaskList.GetTaskByID(index);
			if(litem)
			{
				litem->m_platform=platform;
				litem->m_appiconpath=icon;
				litem->m_fid=fid;
				litem->m_isLegalSoft=isLegalSoft;
				_pInstallObservers.push_back(observer);
				UIInsertTask(litem);
				//if(litem->m_uStatus==WAITING)
				gtask->StartThread();
			}
			return index;
		}
		else
			SAFE_DELETE(observer);
	}
	DescCaseCount(deviceId,casename);
	return -1;
}

void TaskCenterController::InstallApp( int taskId,wstring device )
{
	CTaskItem* item=FindTaskItemById(taskId);
	RETURN_IF(!item||(item->m_uTaskType==URL_DOWNLAOD&&item->m_uStatus!=FINISHED));
		
	//Statistic_StatFunc("52010");
	CStaticsticsMessage stMsg("52010");
	stMsg.Send();
	//wstring strInstall = MyFileOperation::GetIniFileValue(CONFIG(_strOptionIni), _T("Android"), _T("installonphone"));

	RequestInstallPositionMsg requestInstallPositionMsg(device);
	requestInstallPositionMsg.SendAndGetReturn();

	bool bOnSDCard = requestInstallPositionMsg.bInstallOnPhone;//(strInstall == L"0" ? true : false);
	if(item->m_uTaskType==MOVEANDROIDAPP)
		MoveApp(item->appData,item->m_TaskFileInfo->TotalSize,device);
	else
	{
		int index=InstallApp(item->m_strName,item->m_TaskFileInfo->savePath,item->m_appVersion,item->m_TaskFileInfo->TotalSize,device,bOnSDCard,item->m_platform,item->m_fid,L"",item->m_appiconpath,item->m_isLegalSoft);
	}
}

void TaskCenterController::MoveApp( AppData* pApp,long filesize,wstring deviceId )
{
	RETURN_IF(deviceId.empty());
	DeviceData* device=DEVICE_MANAGER->FindDevice(deviceId);

	GlobalTaskInfo * gtask=TaskControl::GetInstallTaskIndexByDevice(deviceId);
	if (!gtask)
	{
		TaskControl::OnDeviceConnect(DEVICE_MANAGER->FindDevice(deviceId));
		gtask=TaskControl::GetInstallTaskIndexByDevice(deviceId);
	}
	if(gtask)
	{
		CTaskItem* finitem=gtask->m_tlTaskList.findSameItem(pApp->_packageName,L"",TASK_KIND::MOVEANDROIDAPP);
		if(finitem)
			return;
		TaskInstallObserver* observer=new TaskInstallObserver(deviceId,this);
		int index=gtask->m_tlTaskList.AddItem(pApp->_AppName.c_str(),pApp->_packageName,L"",MOVEANDROIDAPP,filesize,DEVICE_MANAGER->FindDevice(deviceId), pApp->_strVersion.c_str(),observer,!pApp->_bOnSDCard);
		if(index>-1)
		{
			_unfinishedTaskCount++;
			observer->SetTaskId(index);
			CTaskItem* litem =gtask->m_tlTaskList.GetTaskByID(index);
			if(litem)
			{
				litem->appData=new AppData(pApp);
				litem->m_appiconpath=pApp->_strIconPCPath;
				if(pApp->_eAppType<=APP_TYPE_DEB)
					litem->m_platform=pApp->_eAppType<=APP_TYPE_NPK?L"Android":L"iPhone";
				_pInstallObservers.push_back(observer);
				UIInsertTask(litem);
				gtask->StartThread();
			}
		}
		else
			SAFE_DELETE(observer);
	}
}

bool TaskCenterController::StartTask( int taskId,wstring deviceId )
{
	const string threadid = "CStartTaskThread" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	CTaskThread* pThread = new CTaskThread(threadid);
	pThread->SetType(Action_Start_Task);
	pThread->SetModule(this);
	pThread->SetTaskId(taskId);
	pThread->SetDeviceId(deviceId);
	pThread->Run();
	return true;
}

bool TaskCenterController::StartTaskAction( int taskId,wstring deviceId )
{
	if(!deviceId.empty())
		return RestartTask(taskId,deviceId);

	if(_curTaskList&&_curTaskList->m_tlTaskList.RetryItem(taskId))
	{
		TaskControl::g_gtiGlobalTaskInfo->StartThread();
		/*TaskModuleNotifyMsg msg;
		msg._curID = _curDeviceId;
		msg._param = (LPARAM)(&taskId);
		msg._notifyType = WM_TASK_UPDATESPEED;
		msg.SendToUI();*/
		Notify(WM_TASK_UPDATESPEED,_curDeviceId,LPARAM(&taskId));
		//SendMessage(GetOwnerHWnd(),WM_TASK_UPDATESPEED,(WPARAM)BaseOperation::AllocateBuffer(_curDeviceId),(LPARAM)(&taskId));
	}
	return true;
}

void TaskCenterController::Notify( UINT message, wstring curID, LPARAM lParam )
{
	AutoLock
		
	int taskid=lParam?*((int*)lParam):-1;
	GlobalTaskInfo* tmptasklist=NULL;
	if(curID.empty())
	{
		tmptasklist=TaskControl::g_gtiGlobalTaskInfo;
	}
	else
	{
		tmptasklist= TaskControl::GetInstallTaskIndexByDevice(curID);
	}
	CTaskItem* itemTmp=NULL;
	if(taskid>-1&&tmptasklist)
		itemTmp =  tmptasklist->m_tlTaskList.GetTaskByID( taskid);
	if(itemTmp&&itemTmp->m_uStatus==FINISHED)
		itemTmp->_delaydelete=true;

	CTaskItem* item = NULL;
	if (NULL != itemTmp)
	{
		item = new CTaskItem(*itemTmp);
	}
	
	/*for (size_t i=0;i<_taskUIObserver.size();i++)
	{*/
		switch(message)
		{
		case WM_TASK_FINISHED:
			{
				if(item)
				{
					//_taskUIObserver[i]->TaskFinishedNotify(item,curID);
					NotifyTaskItemMsg notifyTaskItemMsg;
					notifyTaskItemMsg.message = message;
					notifyTaskItemMsg.pTask = (void*)item;
					notifyTaskItemMsg.curID = curID;
					notifyTaskItemMsg.pOriginalTask = (void*)itemTmp;
					notifyTaskItemMsg.PostToUI();
				}
				break;
			}
		case WM_TASK_UPDATESPEED:
			{
				if(item)
				{
					//_taskUIObserver[i]->TaskUpdatedSpeedNotify(item,curID);
					NotifyTaskItemMsg notifyTaskItemMsg;
					notifyTaskItemMsg.message = message;
					notifyTaskItemMsg.pTask = (void*)item;
					notifyTaskItemMsg.curID = curID;
					notifyTaskItemMsg.pOriginalTask = (void*)itemTmp;
					notifyTaskItemMsg.PostToUI();
				}
				break;
			}
		case WM_TASK_FAIL:
			{
				if(item)
				{
					//_taskUIObserver[i]->TaskFailNotify(item,curID);
					NotifyTaskItemMsg notifyTaskItemMsg;
					notifyTaskItemMsg.message = message;
					notifyTaskItemMsg.pTask = (void*)item;
					notifyTaskItemMsg.curID = curID;
					notifyTaskItemMsg.pOriginalTask = (void*)itemTmp;
					notifyTaskItemMsg.PostToUI();
				}
				break;
			}
			
		case WM_TASK_ADDED:
			if(item)
			{
				//_taskUIObserver[i]->OneTaskAdded(item);
				NotifyTaskItemMsg notifyTaskItemMsg;
				notifyTaskItemMsg.message = message;
				notifyTaskItemMsg.pTask = (void*)item;
				notifyTaskItemMsg.pOriginalTask = (void*)itemTmp;
				notifyTaskItemMsg.PostToUI();
			}
			break;
		case WM_TASK_REMOVED:
			{
				//_taskUIObserver[i]->TaskRemoveNotify(taskid,curID);

				NotifyTaskItemMsg notifyTaskItemMsg;
				notifyTaskItemMsg.message = message;
				notifyTaskItemMsg.taskid = taskid;
				notifyTaskItemMsg.curID = curID;
				notifyTaskItemMsg.PostToUI();
			}
			break;
		case WM_TASK_COUNTCHANGE:
			{
				//_taskUIObserver[i]->TaskCountChangeNotify(curID);
				NotifyTaskItemMsg notifyTaskItemMsg;
				notifyTaskItemMsg.message = message;
				notifyTaskItemMsg.curID = curID;
				notifyTaskItemMsg.PostToUI();
			}
			
			break;
		}
	//}
	if(itemTmp&&itemTmp->m_uStatus==FINISHED)
		itemTmp->_delaydelete=false;

	

}

bool TaskCenterController::Handle( const TaskModuleNotifyMsg* pMsg )
{
	Notify(pMsg->_notifyType,pMsg->_curID,pMsg->_param);
	return true;
}


bool TaskCenterController::Handle( const CTaskModuleInnerMsg* pMsg )
{
	switch(pMsg->_type)
	{
	case TASK_UPDATESPEED:
		{
			CTaskItem* itemTmp = (CTaskItem*)pMsg->wParam;
			TaskUpdatedSpeedNotify(itemTmp,pMsg->strParam);
			break;
		}
	case REMOVE_DEAMON:
		{
			wstring sn = pMsg->strParam;
			RemoveInstalldeamon(sn);
			break;
		}
	case  DELETE_TASK:
		{
			int noticeTmp = pMsg->nUserData;
			bool notice = false;
			if (noticeTmp != 0)
			{
				notice = true;
			}
			DeleteTask(pMsg->nParam,pMsg->strParam,notice);
			break;
		}
	case LOAD_ICON:
		{
			LoadIcons();
			break;
		}
	case STOP_DOWN:
		{
			StopDown(pMsg->nParam);
			break;
		}
	case STAR_TASK:
		{
			StartTask(pMsg->nParam,pMsg->strParam);
			break;
		}
	case DELETE_ALLFINISHEDINSTALLTASK:
		{
			DeleteAllFinishedInstallTask();
			break;
		}
	case TASK_INSTALLAPP:
		{
			InstallApp(pMsg->nParam,pMsg->strParam);
			break;
		}
	case  ADD_TASKUIObserver:
		{
			TaskCenterControlUI* pTmp = (TaskCenterControlUI*)(pMsg->wParam);
			AddTaskUIObserver((TaskUIObserver*)pTmp);
			break;
		}

	case  DEAL_LASTUNFINISHEDTASK_ONDEVICE:
		{
			wstring sn = pMsg->strParam;
			DealLastUnfinishedTasksOnDevice(sn);
			break;
		}
	case  STARTDOWNLOAD:
		{
			StartDownloadParam* param = (StartDownloadParam*)pMsg->wParam;
			StartDownLoad(param->url,param->savepath,param->appname,param->version,param->platform,param->fid,param->bUpdategrade,
				param->needInstall,param->oldappfullpath,param->isLegalSoft,(TASK_UPGRADE_TYPE)(param->taskupgradetype),param->appID,param->pCpaInfo);
			break;
		}
	}
	return true;
}

bool TaskCenterController::Handle( CTaskModuleGetValueInnerMsg* pMsg )
{
	switch (pMsg->_type)
	{
	case FIND_TASKITEM_BYID:
		{
			int sn = pMsg->nParam;
			CTaskItem* item= FindTaskItemById(sn);
			pMsg->wParam = (WPARAM)item;
			//pMsg->nUserData = (int)item;
			break;
		}
	case GET_TASKLIST:
		{
 			const vector<CTaskItem*>* list = GetTaskList(pMsg->strParam,(TASK_LIST_TYPE)(pMsg->nParam));
			pMsg->wParam = (WPARAM) (list);
			break;
		}
	case FIND_TASKITEMID_BYURL:
		{
			pMsg->nParam = GetTaskIdByUrl(pMsg->strParam);
			break;
		}
	case GET_TASKCOUNT:
		{
			wstring strRecount = pMsg->strParam;
			bool bIsRecount = true;
			if (strRecount == L"0")
			{
				bIsRecount = false;
			}
			pMsg->nParam = GetTaskCount(bIsRecount);
			break;
		}
	case GET_TASKCOUNT_ONDEVICE:
		{
			pMsg->nUserData = GetTaskCountOnDevice(pMsg->strParam,pMsg->nParam);
			break;
		}
	}
	return true;
}

bool TaskCenterController::Handle( GetTaskByIDMsg* pMsg )
{
	if (NULL == pMsg)
	{
		return true;
	}

	pMsg->pTask = (void*)FindTaskItemById(pMsg->nTaskID);
	return true;
}

bool TaskCenterController::Handle( const SetCaseSizeMsg* pMsg )
{
	SetCaseCount(pMsg->deviceId,pMsg->casename,pMsg->count);
	return true;
}

bool TaskCenterController::Handle( GetInstallPlanIdMsg* pMsg )
{
	FindAllCaseInstallTask(pMsg->deviceId,pMsg->caseId,pMsg->PlanTasks);
	return true;
}

bool TaskCenterController::Handle( DownloadTaskItemMsg* pMsg )
{
	pMsg->taskid = this->DownLoadSoft(pMsg->url,pMsg->savepath,pMsg->appname,pMsg->version,pMsg->platform,pMsg->fid,
		pMsg->bUpdategrade,pMsg->needInstall,pMsg->oldappfullpath,pMsg->isLegalSoft,(TASK_UPGRADE_TYPE)(pMsg->taskupgradetype),pMsg->appID,pMsg->pCpaInfo);
	return true;
}

bool TaskCenterController::Handle( const MoveTaskItemMsg* pMsg )
{
	if (NULL == pMsg)
	{
		return true;
	}
	MoveApp((AppData*)( pMsg->pData),pMsg->staticDiskUsage,pMsg->curDeviceID);
	return true;
}

bool TaskCenterController::Handle( InstallTaskItemMsg* pMsg )
{
	if (NULL == pMsg)
	{
		return true;
	}
	pMsg->taskid = InstallApp(pMsg->appname,pMsg->appfilepath,pMsg->version,pMsg->filesize,pMsg->deviceId,pMsg->onSDcard,pMsg->plat,pMsg->fid,pMsg->casename,pMsg->icon,pMsg->isLegalSoft);
	return true;
}

bool TaskCenterController::RestartTask( int taskId,wstring deviceId )
{
	DeviceData* pdeviceData=DEVICE_MANAGER->FindDevice(deviceId);
	RETURN_FALSE_IF(!pdeviceData);
	GlobalTaskInfo * gtask= TaskControl::GetInstallTaskIndexByDevice(deviceId);
	RETURN_FALSE_IF(!gtask);
	CTaskItem* item=gtask->m_tlTaskList.GetTaskByID(taskId);
	RETURN_FALSE_IF(!item);
	RETURN_FALSE_IF(item->m_uTaskType!=INSTALL&&item->m_uTaskType!=MOVEANDROIDAPP);
	if(item->m_uStatus==WAITING&&!item->hasProcing)
		gtask->StartThread();
	RETURN_FALSE_IF(item->m_uTaskType==INSTALL&&item->m_uStatus!=FAILED&&item->m_uStatus!=FINISHED);
	RETURN_FALSE_IF(item->m_uTaskType==MOVEANDROIDAPP&&item->m_uStatus!=FAILED);
	if(item->m_uStatus==FAILED)
	{
		item->m_nProcess=0;
		item->_retryed=true;
		if(gtask) 
		{
			gtask->m_tlTaskList.RetryItem(taskId);
			gtask->StartThread();
		}
		if(deviceId==_curDeviceId)
		{
			/*TaskModuleNotifyMsg msg;
			msg._curID = _curDeviceId;
			msg._param = (LPARAM)(&taskId);
			msg._notifyType = WM_TASK_UPDATESPEED;
			msg.SendToUI();*/
			Notify(WM_TASK_UPDATESPEED,_curDeviceId,(LPARAM)(&taskId));
		}
		return true;
	}
	//SendMessage(GetOwnerHWnd(),WM_TASK_REMOVED,(WPARAM)BaseOperation::AllocateBuffer(_curDeviceId),(LPARAM)(&taskId));
	//wstring strInstall = MyFileOperation::GetIniFileValue(CONFIG(_strOptionIni), _T("Android"), _T("installonphone"));
	RequestInstallPositionMsg requestInstallPositionMsg(deviceId);
	requestInstallPositionMsg.SendAndGetReturn();

	bool bOnSDCard = requestInstallPositionMsg.bInstallOnPhone;//(strInstall == L"0" ? true : false);
	int index=InstallApp(item->m_strName,item->m_TaskFileInfo->savePath,item->m_appVersion,item->m_TaskFileInfo->TotalSize,_curDeviceId,bOnSDCard,item->m_platform,item->m_fid,item->m_casename,item->m_appiconpath,item->m_isLegalSoft);
	//InstallApp(taskId,_curDeviceId);
	CTaskItem* newitem=FindTaskItemById(index);
	if(newitem)
		newitem->_retryed=true;
	if(_curTaskList->m_tlTaskList.DeleteTaskByID(taskId)&&_taskUIObserver.size()>0)
	{
		/*TaskModuleNotifyMsg msg;
		msg._curID = _curDeviceId;
		msg._param = (LPARAM)(&taskId);
		msg._notifyType = WM_TASK_REMOVED;
		msg.SendToUI();*/
		Notify(WM_TASK_REMOVED,_curDeviceId,(LPARAM)(&taskId));
	}
	//DeleteTask(taskId,true);

	if(_listtype==FINISHED_TASK)
		_unfinishedTaskCount++;
	return true;
}

bool TaskCenterController::DeleteTask( int taskId,wstring deviceId,bool notice/*=true*/ )
{
	const string threadid = "CStartTaskThreadDelete" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	CTaskThread* pThread = new CTaskThread(threadid);
	pThread->SetModule(this);
	pThread->SetTaskId(taskId);
	pThread->SetDeviceId(deviceId);
	TaskThreadParam* pParam = new TaskThreadParam;
	pParam->bParam = notice;
	pThread->SetType(Action_Delete_Task);
	pThread->SetParam(pParam);
	pThread->Run();
	return true;
}

void TaskCenterController::StopDown( int taskId )
{
	if(!_curDeviceId.empty())
		return;
	TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.SuspendItem(taskId);
}

int TaskCenterController::GetTaskCount( bool recount/*=false*/ )
{
	if(recount)
	{
		_unfinishedTaskCount=0;
		GetTaskCountOnDevice(L"",_unfinishedTaskCount);
		vector<wstring> deviceIds=DEVICE_MANAGER->GetAllDevice();
		for (size_t i=0;i<deviceIds.size();i++)
		{
			GetTaskCountOnDevice(deviceIds[i],_unfinishedTaskCount);
		}
	}
	return _unfinishedTaskCount;
}

int TaskCenterController::GetTaskCountOnDevice( wstring deviceId, int& unfinished )
{
	int totalcount=0;
	TaskList* tmplist=NULL;
	GlobalTaskInfo * gtask=NULL;
	if(deviceId.empty())
		gtask=TaskControl::g_gtiGlobalTaskInfo;
	else
		gtask=TaskControl::GetInstallTaskIndexByDevice(deviceId);

	if(gtask)
		tmplist=gtask->m_tlTaskList.getTaskList();
	if(!tmplist)return totalcount;
	unfinished+=gtask->m_tlTaskList.GetUnfinishedTaskCount();
	totalcount=gtask->m_tlTaskList.getTaskList()->size();
	return totalcount;
}

const vector<CTaskItem*>* TaskCenterController::GetTaskList( wstring device, TASK_LIST_TYPE listtype )
{
	vector<CTaskItem*>* tasks = new vector<CTaskItem*>;
	if(device.empty())
		_curTaskList=TaskControl::g_gtiGlobalTaskInfo;
	else
	{
		GlobalTaskInfo * gtask= TaskControl::GetInstallTaskIndexByDevice(device);
		if(!gtask)
			return tasks;
		_curTaskList = gtask;
	}
	_curDeviceId = device;
	//_isFinishedList=finished;
	_listtype = listtype;

	TaskList* tmplist = _curTaskList->m_tlTaskList.getTaskList();
	list<CTaskItem*>::iterator it=tmplist->begin();
	while (it!=tmplist->end())
	{
		CTaskItem* item= *it;
		if(listtype!=UNFINISHED_TASK && item->m_uStatus==FINISHED)
		{
			if(device.empty()&&!CFileOperation::IsFileExist(item->m_TaskFileInfo->savePath.c_str()))
			{
				it++;
				_curTaskList->m_tlTaskList.DeleteTaskByID(item->m_uID);
				continue;
			}
			else
				tasks->push_back(item);
		}
		else if(listtype!=FINISHED_TASK && item->m_uStatus!=FINISHED)
		{
			tasks->push_back(item);
		}
		it++;
	}
	return tasks;
}

CTaskItem* TaskCenterController::FindTaskItemById( int taskId )
{
	if(_curTaskList)
		return _curTaskList->m_tlTaskList.GetTaskByID(taskId);
	return NULL;
}

void TaskCenterController::SetCaseCount( wstring deviceId, wstring casename,int count )
{
	wstring name=deviceId+L"_"+casename;
	if(!casename.empty())
	{
		if(_unDealCountInCase.find(name)!=_unDealCountInCase.end())
			_unDealCountInCase.find(name)->second=count;
		else
			_unDealCountInCase[name]=count;
	}
}

void TaskCenterController::LoadIcons()
{
	if(_IconLoaded)
		return;
	_IconLoaded=true;
	const string threadid = "CStartTaskThreadLoadIcons" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	CTaskThread* pThread = new CTaskThread(threadid);
	pThread->SetModule(this);
	pThread->SetType(Action_Load_TaskList);
	pThread->Run();
}

void TaskCenterController::LoadIconsInternal()
{
	//RETURN_TRUE_IF(!_curDeviceId.empty()&&_listtype!=FINISHED_TASK);
	if (!_curDeviceId.empty()&&_listtype!=FINISHED_TASK)
	{
		return;
	}
	//TaskList* tmplist = _curTaskList->m_tlTaskList.getTaskList();
	vector<int> taskids;
	TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.GetTaskByStatus(&taskids,true);
	for (size_t i=0; i<taskids.size(); i++)
	{
		CTaskItem* item= TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.GetTaskByID(taskids[i]);
		if(item)
		{
			item->Lock();
			if(!item->m_appiconpath.empty()&&CFileOperation::IsFileExist(item->m_appiconpath.c_str()))
			{
				item->Unlock();
				continue;
			}
			if(LoadDownedAppIcon(item))
			{
				/*TaskModuleNotifyMsg msg;
				msg._notifyType = WM_TASK_UPDATESPEED;
				msg._curID = _curDeviceId;
				msg._param = (LPARAM)(&item->m_uID);
				msg.SendToUI();*/
				Notify(WM_TASK_UPDATESPEED,_curDeviceId,(LPARAM)(&item->m_uID));
			}
			item->Unlock();
		}
	}
}

void TaskCenterController::DeleteAllFinishedInstallTask()
{
	vector<wstring>  devices=DEVICE_MANAGER->GetAllDevice();
	for (size_t i=0;i<devices.size();i++)
	{
		GlobalTaskInfo * gtask= TaskControl::GetInstallTaskIndexByDevice(devices[i]);
		if(!gtask)continue;
		vector<int> taskIds;
		gtask->m_tlTaskList.GetTaskByStatus(&taskIds,true);
		for (size_t j=0;j<taskIds.size();j++)
		{
			DeleteTask(taskIds[j],devices[i]);
		}
	}
}

bool TaskCenterController::DeleteTaskAction( int taskId,wstring deviceId )
{
	wstring device=deviceId;
	CTaskItem* item=NULL;
	GlobalTaskInfo * gtask=NULL;
	if(device.empty())
		gtask=TaskControl::g_gtiGlobalTaskInfo;
	else
		gtask=TaskControl::GetInstallTaskIndexByDevice(deviceId);
	RETURN_FALSE_IF(!gtask);
	item=gtask->m_tlTaskList.GetTaskByID(taskId);
	RETURN_TRUE_IF(!item);
	TASK_STATUS itemstatus=item->m_uStatus;
	RETURN_FALSE_IF(!device.empty() && itemstatus==PROCEDING);

	if(item->m_uTaskType==URL_DOWNLAOD && item->bUpadateGrade)
	{
		AppUpdategradeCancelMsg msg;
		msg.item = (void*)item;
		msg.Send();
		//MainDlg::GetInstance()->GetInstallCaseUI()->Model.AppUpdategradeCancel(item);
	}
	wstring casename=item->m_casename;
	if(!deviceId.empty()&&item->_delaydelete)
	{
		while(item->_delaydelete)
		{
			Sleep(10);
		}
	}
	bool retryed=item->_retryed;
	if(gtask->m_tlTaskList.DeleteTaskByID(taskId))
	{
		if(itemstatus!=FINISHED&&_unfinishedTaskCount>0)
			_unfinishedTaskCount--;
		if(itemstatus==WAITING&&!retryed)
			DescCaseCount(device,casename);
		return true;
	}
	return false;
}

void TaskCenterController::dealDeamonInstall( wstring deviceId )
{
	if(_installdeamon.find(deviceId)==_installdeamon.end())
	{
		_installdeamon[deviceId]=false;
	}
}

void TaskCenterController::DealLastUnfinishedTasksOnDevice( wstring deviceId )
{
	TaskControl::OnDeviceConnect(DEVICE_MANAGER->FindDevice(deviceId));
	GlobalTaskInfo * gtask=TaskControl::GetInstallTaskIndexByDevice(deviceId);
	if(gtask&&gtask->m_tlTaskList.RestartAllUnfinishedTask()>0)
	{
		//SetCSLock(_csLock);
		AutoLock
		for(vector<TaskUIObserver*>::iterator it = _taskUIObserver.begin(); it != _taskUIObserver.end(); it++)
		{
			if(_bExit)
				return;

			if(*it != NULL)
				(*it)->OnDeviceHasUnfinishedTasks(deviceId);
		}
	}
}

int TaskCenterController::GetTaskIdByUrl( wstring wsUrl )
{
	if(_curTaskList)
		return _curTaskList->m_tlTaskList.GetIDByUrl(wsUrl);
	return -1;
}

void TaskCenterController::UIShowTaskList( wstring device, TASK_LIST_TYPE finished )
{
	const vector<CTaskItem*>* tasks=GetTaskList(device,finished);
	for (size_t i=0; i<tasks->size();i++)
	{
		UIInsertTask((*tasks)[i]);
	}
}

void TaskCenterController::UIInsertTask( CTaskItem* item )
{
	if (!item) return;
	wstring deviceid=item->_pDeviceData==NULL?L"": item->_pDeviceData->GetId();
	/*TaskModuleNotifyMsg msg;
	msg._notifyType = WM_TASK_ADDED;
	msg._curID = deviceid;
	msg._param = (LPARAM)(&(item->m_uID));
	msg.PostToUI();*/

	Notify(WM_TASK_ADDED,deviceid,(LPARAM)(&(item->m_uID)));
	//SendMessage(GetOwnerHWnd(),WM_TASK_ADDED,(WPARAM)BaseOperation::AllocateBuffer(deviceid),(LPARAM)(&item->m_uID));
}

void TaskCenterController::detach( TaskUIObserver* pObserver )
{
	AutoLock

	for (vector<TaskUIObserver*>::iterator it=_taskUIObserver.begin(); it != _taskUIObserver.end();)
	{
		TaskUIObserver *dev = *it;
		if(pObserver==(*it))
		{
			it = _taskUIObserver.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void TaskCenterController::TaskDeleteNotify( int taskId, wstring deviceId/*=L"" */ )
{
	if (_unfinishedTaskCount>0)
		_unfinishedTaskCount--;
	/*TaskModuleNotifyMsg msg;
	msg._notifyType = WM_TASK_REMOVED;
	msg._curID = _curDeviceId;
	msg._param = (int)(&taskId);
	msg.SendToUI();*/
	Notify(WM_TASK_REMOVED,_curDeviceId,(LPARAM)(&taskId));
}

void TaskCenterController::TaskUpdatedSpeedNotify( CTaskItem* item,wstring deviceId/*=L"" */ )
{
	/*TaskModuleNotifyMsg msg;
	msg._notifyType = WM_TASK_UPDATESPEED;
	msg._curID = deviceId;
	msg._param = LPARAM(&item->m_uID);
	msg.SendToUI();*/
	Notify(WM_TASK_UPDATESPEED,deviceId,LPARAM(&item->m_uID));
}

void TaskCenterController::TaskFinishedNotify( CTaskItem* item,wstring deviceId/*=L"" */ )
{
	int taskId=item->m_uID;
	if(deviceId.empty())
	{
		RETURN_IF(!TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.GetTaskByID(taskId));		
		item->Lock();
		LoadDownedAppIcon(item);   
		TASK_STATUS tstatus= item->m_uStatus;
		item->Unlock();
		RETURN_IF(tstatus==FAILED||!TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.GetTaskByID(taskId));
	}
	if(_unfinishedTaskCount>0)
		_unfinishedTaskCount--;
	DescCaseCount(deviceId,item->m_casename);
	/*TaskModuleNotifyMsg msg;
	msg._notifyType = WM_TASK_FINISHED;
	msg._curID = deviceId;
	msg._param =LPARAM(&taskId);
	msg.SendToUI();*/
	Notify(WM_TASK_FINISHED,deviceId,LPARAM(&taskId));
}

void TaskCenterController::TaskFailNotify( CTaskItem* item,wstring deviceId/*=L"" */ )
{
	DescCaseCount(deviceId,item->m_casename);
	//TaskModuleNotifyMsg msg;
	//msg._notifyType = WM_TASK_FAIL;
	//msg._curID = deviceId;
	//msg._param = LPARAM(&item->m_uID);
	//msg.SendToUI();
	Notify(WM_TASK_FAIL,deviceId,LPARAM(&item->m_uID));
}

void TaskCenterController::DescCaseCount( wstring deviceId, wstring casename )
{
	wstring name=deviceId+L"_"+casename;
	RETURN_IF(casename.empty()||deviceId.empty());
	RETURN_IF(_unDealCountInCase.find(name)==_unDealCountInCase.end());
	int count=_unDealCountInCase.find(name)->second;
	if(count>0)
	{
		_unDealCountInCase.find(name)->second--;
		if(count==1)
		{
			//方案安装完成
			EnterCriticalSection(&_csLock);
			for(vector<TaskUIObserver*>::iterator it = _taskUIObserver.begin(); it != _taskUIObserver.end(); it++)
			{
				(*it)->NotifyCaseInsatllFinished(_wtoi(casename.c_str()),deviceId);
			}
			LeaveCriticalSection(&_csLock);
		}
	}
}

bool TaskCenterController::LoadDownedAppIcon( CTaskItem* item )
{
	RETURN_FALSE_IF(!item);
	wstring ext=CFileOperation::GetFileExt(item->m_TaskFileInfo->savePath);
	RETURN_TRUE_IF((ext==L"ipa"||ext==L"pxl") && CSystemInfo::IsNeedInstalliTunes());
	int taskId=item->m_uID;
	//_strTemp
	/*AppData* data=AppManager::GetAppInfoByFile(item->m_TaskFileInfo->savePath,DataPath::GetTempDir()+L"\\",true);*/
	AppData* data=AppManager::GetAppInfoByFile(item->m_TaskFileInfo->savePath,CONFIG(_strTemp)+L"\\",true);
	if(!TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.GetTaskByID(taskId))
	{
		SAFE_DELETE(data);
		return false;
	}
	if(data)
	{
		item->m_appiconpath	= data->_strIconPCPath;
		item->m_appid		= data->_packageName;
		item->m_isLegalSoft	= data->_isLegalSoft!=0;
		//item->m_realName	= data->_AppName; 
		SAFE_DELETE(data);
		if(!item->m_appiconpath.empty())
		{
			return true;
		}
	}
	else
	{
		item->m_uStatus=FAILED;
		//CFileOperation::MoveFileW(item->m_TaskFileInfo->savePath,item->m_TaskFileInfo->savePath+L"_0");
		CFileOperation::DeleteFile(item->m_TaskFileInfo->savePath);
		//item->m_TaskFileInfo->savePath+=L"_0";
		if(_curDeviceId.empty()&&_listtype==FINISHED_TASK)
		{
			/*TaskModuleNotifyMsg msg;
			msg._notifyType = WM_TASK_REMOVED;
			msg._curID = _curDeviceId;
			msg._param =LPARAM(&item->m_uID);
			msg.SendToUI();*/
			Notify(WM_TASK_REMOVED,_curDeviceId,LPARAM(&item->m_uID));
			/*SendMessage(GetOwnerHWnd(),WM_TASK_REMOVED,(WPARAM)BaseOperation::AllocateBuffer(_curDeviceId),(LPARAM)(&item->m_uID));*/
		}
		else if(_curDeviceId.empty()&&_listtype==UNFINISHED_TASK)
		{
			/*TaskModuleNotifyMsg msg;
			msg._notifyType = WM_TASK_FAIL;
			msg._curID = _curDeviceId;
			msg._param =LPARAM(&item->m_uID);
			msg.SendToUI();*/
			Notify(WM_TASK_FAIL,_curDeviceId,(LPARAM)(&item->m_uID));
		}
			//SendMessage(GetOwnerHWnd(),WM_TASK_FAIL,(WPARAM)BaseOperation::AllocateBuffer(_curDeviceId),(LPARAM)(&item->m_uID));
	}
	return false;
}

void TaskCenterController::AddTaskUIObserver( TaskUIObserver* pObserver )
{
	RETURN_IF(!pObserver);
	AutoLock
	for(vector<TaskUIObserver*>::iterator it = _taskUIObserver.begin(); it != _taskUIObserver.end(); it++)
	{
		TaskUIObserver *dev = *it;
		if ( dev == pObserver )
			return;
	}
	_taskUIObserver.push_back(pObserver);
}

void TaskCenterController::RemoveTaskUIObsercer( TaskUIObserver* pObserver )
{
	AutoLock
	for (vector<TaskUIObserver*>::iterator it=_taskUIObserver.begin();it!=_taskUIObserver.end();)
	{
		if(pObserver==(*it))
		{
			it = _taskUIObserver.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void TaskCenterController::RemoveInstalldeamon( wstring sn )
{
	if (_installdeamon.find(sn) != _installdeamon.end())
	{
		_installdeamon.erase(sn);
	}
}

void TaskCenterController::FindAllCaseInstallTask(wstring deviceId, wstring caseId, vector<int>& installTaskIds )
{
	GlobalTaskInfo * gtask=TaskControl::GetInstallTaskIndexByDevice(deviceId);
	RETURN_IF(!gtask);
	gtask->m_tlTaskList.FindAllCaseInstallTask(caseId,installTaskIds);
}

void TaskCenterController::Exit()
{
	TaskControl::SaveDownloadTaskInfoToLocal();
}

