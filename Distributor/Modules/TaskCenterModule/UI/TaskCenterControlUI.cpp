#include "stdafx.h"

#include "TaskListUI.h"
#include "TaskModuleInnerMsg.h"

#include "UI/TaskCenterControlUI.h"
#include "ui/MessageWnd.h"
#include "UI/CommonUIFunc.h"
#include "Common/InstallCase/InstallsCase/InstallsAppInfo.h"
#include "UI/MessageWnd.h"
#include "LeftView.h"

TaskCenterControlUI::TaskCenterControlUI(LeftView* pLeftView)
{//INTERFUN;
	_curdeviceId=L"";
	//_isFinishedList=false;
	_listtype = ALL_TASK;
	_IsAutoDelFinishedTask=false;
	_curTasklist=NULL;
    //TaskCenterModule::GetInstance(this);
	_usercancel=true;
	//_latConfirmEvent = NULL;
	_wndContinueInstall = NULL;
	_pLeftView = pLeftView;
	
}

TaskCenterControlUI::~TaskCenterControlUI()
{//INTERFUN;
	_tasklistUImap.clear();
	//TaskCenterModule::ReleaseInstance();
	if(_wndContinueInstall&&_wndContinueInstall->GetHWND())
		CloseHandle(_wndContinueInstall->GetHWND());
	SAFE_DELETE(_wndContinueInstall);
}


void TaskCenterControlUI::OnCreate()
{//INTERFUN;
    CCompositeTabUI::OnCreate();
	CTreeUI* _pLeftTree = _pLeftView->GetTree();
	if(_pLeftTree)
	{
		_pLeftTree->SetINotifyUI(this);
        _pLeftTree->SetGroupHeight(39);
		_pLeftTree->SetItemHeight(39);
		_pLeftTree->SetSelectedHeight(39);
		AddTreeNode(CommonUIFunc::LoadString(_T("DownLoad_Task")),_T("#icon_TreeNode_Download"),_T("download_task"));//(BaseUI::LoadString(_T("DownLoad_Task")),_T("#icon_TreeNode_Download"),_T("download_task"));
		_pLeftTree->SelectItem(0);
	}

	CTaskModuleInnerMsg addTaskUIObserverMsg;
	addTaskUIObserverMsg._type = ADD_TASKUIObserver;
	addTaskUIObserverMsg.wParam = (WPARAM)this;
	addTaskUIObserverMsg.Send();
	//TaskCenterModule::GetInstance()->AddTaskUIObserver(this);

	if(_tasklistUImap.find(L"download_task")!=_tasklistUImap.end())
	{
		_curTasklist=_tasklistUImap[L"download_task"];
		_tasklistUImap[L"download_task"]->SetVisible(true);
		_tasklistUImap[L"download_task"]->RefreshData();
		//TaskCenterModule::GetInstance()->GetTaskCount(true);
		CTaskModuleGetValueInnerMsg getTaskCountMsg;
		getTaskCountMsg._type = GET_TASKCOUNT;
		getTaskCountMsg.strParam = L"1";
		getTaskCountMsg.SendAndGetReturn();
		SetTaskCount(L"");
	}	
}

const wchar_t* typestr[]={L"_all",L"",L"_finished"};
void TaskCenterControlUI::AddContainer(wstring id, TASK_LIST_TYPE isfinished)
{//INTERFUN;
	wstring mapid=id+typestr[isfinished];
	RETURN_IF(_tasklistUImap.find(mapid)!=_tasklistUImap.end());
	TaskListUI* tasklist=new TaskListUI(id==L"download_task"?L"":id,isfinished);
	RegisterComponent(dynamic_cast<CContainerUI*>(GetItem(L"taskcenter_right_layout.TaskList")), L"TaskListComposite", (CTabItem*)tasklist);
    tasklist->CreateUI();
	//tasklist->CreateComponent(L"TaskListComposite",(CTabItem*)GetItem(L"taskcenter_right_layout.TaskList"));
	tasklist->SetVisible(false);
	_tasklistUImap[mapid]=tasklist;

}
void TaskCenterControlUI::AddTreeNode(wstring text, wstring image, wstring id)
{//INTERFUN;
	if(id==L"download_task")
	{
		AddContainer(id, UNFINISHED_TASK);
		AddContainer(id, FINISHED_TASK);
	}
	else
		AddContainer(id, ALL_TASK);
	 
	CTreeUI* _pLeftTree = _pLeftView->GetTree();
    // 临时
    if (!_pLeftTree) return;
	CTreeNodeUI* pGroup = NULL;
	if(id!=L"download_task")
	{
		pGroup = new CTreeNodeUI(true);
		pGroup->SetStyle(_T("Style_US_GroupTreeNodeEx"));
		pGroup->SetId(id.c_str());
		pGroup->SetImage(image.c_str());
		pGroup->SetText(text.c_str());
		pGroup->SetAttribute(_T("tooltip"),text.c_str());
		pGroup->SetAttribute(_T("bk.image"),_T(""));
		pGroup->SetAttribute(_T("DrawNode"),_T("1"));
		_pLeftTree->InsertItem(pGroup);
		pGroup->SetVisible(false);
		return;
	}
	//pGroup->SetEnabled(false);
	//pGroup->SetAttribute(_T("icon.pos"),_T("9|13,-1,16,16"));

	CTreeNodeUI* pSecondGroup = NULL;
	for (int j = 0; j < 2; j++)
	{
		pSecondGroup = new CTreeNodeUI();
		pSecondGroup->SetStyle(_T("Style_US_SingleTreeNode"));
		pSecondGroup->SetAttribute(_T("DrawNode"),_T("1"));
		switch(j)
		{
		case 0:
			{
				pSecondGroup->SetId(L"download_unfinished");
				pSecondGroup->SetText(CommonUIFunc::LoadString(_T("unFinished_Task")));//BaseUI::LoadString(_T("unFinished_Task")));
			}
			break;
		case 1:
			{
				pSecondGroup->SetId(L"download_finished");
				pSecondGroup->SetText(CommonUIFunc::LoadString(_T("Finished_Task")));//BaseUI::LoadString(_T("Finished_Task")));
			}
			break;
		}
		pSecondGroup->SetAttribute(_T("icon.image"),_T(""));
		pSecondGroup->SetAttribute(L"txt.pos",L"8|35,0,0,0");
		_pLeftTree->InsertItem(pSecondGroup, pGroup);
	}
	//pGroup->Expand(true);
}

void TaskCenterControlUI::OneTaskAdded( CTaskItem* item )
{//INTERFUN;
	RETURN_IF(!item);
	wstring deviceid=item->_pDeviceData==NULL?L"": item->_pDeviceData->GetId();
	SetTaskCount(deviceid);
	RETURN_IF(_curdeviceId!=deviceid);
	RETURN_IF(!item);
	if(_listtype!=FINISHED_TASK)
	{
		_curTasklist->InsertTask(item);
	}
}

void TaskCenterControlUI::TaskUpdatedSpeedNotify( CTaskItem* item ,wstring deviceId )
{//INTERFUN;
	RETURN_IF(!item);
	if(item->m_uStatus==FINISHED&&item->m_uTaskType==URL_DOWNLAOD)
	{
		_tasklistUImap[_T("download_task_finished")]->UpdateTaskImage(item);
		return;
	}
	RETURN_IF(_curdeviceId!=deviceId);
	_curTasklist->UpdateTaskUI(item);
}

void TaskCenterControlUI::TaskFinishedNotify( CTaskItem* item, wstring deviceId )
{//INTERFUN;
	SetTaskCount(deviceId);
	RETURN_IF(!item);
	if(item->m_uTaskType==URL_DOWNLAOD)
	{
		/*GetConfigMsg configMsg;
		configMsg.SendAndGetReturn();*/
		wstring wsDir = CFileOperation::GetFileDir(item->m_TaskFileInfo->savePath);
		if (wsDir != CONFIG(_strAndroid_CpaDownPath)&& wsDir != CONFIG(_strIos_CpaDownPath))
		{
			LOG_DEBUG(L"TaskCenterControlUI::TaskFinishedNotify, AddSingleAppFileToLib Begin");
			

			AddSingleAppFileToLibMsg addSigleAppToLibMsg;
			addSigleAppToLibMsg.filePath = item->m_TaskFileInfo->savePath;
			addSigleAppToLibMsg.fid = item->m_fid;
			addSigleAppToLibMsg.oldFilePath = item->m_oldappfullpath;
			addSigleAppToLibMsg.SendAndGetReturn();

			bool isReshUI = addSigleAppToLibMsg.isReshUI;
			InstallsAppInfo* appInfo  = (InstallsAppInfo*)(addSigleAppToLibMsg.result);
			//InstallsAppInfo* appInfo = MainDlg::GetInstance()->GetInstallCaseUI()->Model.AddSingleAppFileToLib(item->m_TaskFileInfo->savePath, isReshUI, item->m_fid ,item->m_oldappfullpath);
			LOG_DEBUG(L"TaskCenterControlUI::TaskFinishedNotify, AddSingleAppFileToLib End");
			if(appInfo)appInfo->m_fid=item->m_fid;
			if((appInfo == NULL && item->bUpadateGrade)||isReshUI)
			{
				AppUpdategradeCancelMsg appUpdategradeCancelMsg;
				appUpdategradeCancelMsg.item = (void*)item;
				appUpdategradeCancelMsg.SendToUI();
				//MainDlg::GetInstance()->GetInstallCaseUI()->Model.AppUpdategradeCancel(item);
			}
		}
		else
		{
			LocalCpaAddOrRemoveMsg localCpaAddOrRemoveMsg;
			if (wsDir == CONFIG(_strAndroid_CpaDownPath))
			{
				localCpaAddOrRemoveMsg.bIsAndroidPlatform = true;
				localCpaAddOrRemoveMsg.vecAddoveCpa.push_back(item->m_TaskFileInfo->savePath);
			}
			else
			{
				localCpaAddOrRemoveMsg.bIsAndroidPlatform = false;
				localCpaAddOrRemoveMsg.vecAddoveCpa.push_back(item->m_TaskFileInfo->savePath);
			}
			localCpaAddOrRemoveMsg.Send();
		}

		LOG_DEBUG(L"TaskCenterControlUI::TaskFinishedNotify, PostDownLoad Begin");
		CStaticsticsMessage stMsg("51009");
		stMsg.Send();
		PostDownloadInfoMsg postDownloadInfoMsg;
		postDownloadInfoMsg.m_appid = item->m_appid;
		postDownloadInfoMsg.m_platform = item->m_platform;
		postDownloadInfoMsg.m_csFileDownUrl = item->m_csFileDownUrl;
		postDownloadInfoMsg.m_strName = item->m_strName;
	    postDownloadInfoMsg.m_appVersion = item->m_appVersion;
		postDownloadInfoMsg.m_csFileSavePath = item->m_csFileSavePath;
		postDownloadInfoMsg.Send();

		PostDownloadFinishMsg postDonloadFinishMsg;
		postDonloadFinishMsg.m_fid = item->m_fid;
		postDonloadFinishMsg.m_uStatus = (int)item->m_uStatus;
		postDonloadFinishMsg.m_nSpeed = item->m_nSpeed;
		postDonloadFinishMsg.m_platform = item->m_platform;
		postDonloadFinishMsg.Send();

		LOG_DEBUG(L"TaskCenterControlUI::TaskFinishedNotify, PostDownLoad End");

		CpaReDownLoadAndInstall* pCpaInfo = (CpaReDownLoadAndInstall*)item->pCpaInfo;
		if (pCpaInfo&&pCpaInfo->st_bIsDownLoadAndInstal)
		{
			WebSoftInstallMsg msg;
			msg._st_Hash = pCpaInfo->st_vec_Hash;
			msg._st_Sn = pCpaInfo->st_vec_sn;
			msg.Send();
		}
		
	}
	else if(item->m_uTaskType==INSTALL||item->m_uTaskType==MOVEANDROIDAPP)
	{
		wstring text=item->m_uTaskType==INSTALL?CommonUIFunc::LoadString(L"Install_Success"):CommonUIFunc::LoadString(L"Move_App_Success");//BaseUI::LoadStringW(L"Install_Success"):BaseUI::LoadStringW(L"Move_App_Success");
		text+=item->m_strName;
		if( INSTALL==item->m_uTaskType )
		{
			 if(CONFIG(_bSoftInstallMsg))
				 CommonUIFunc::NoticeDialog(text,ICON_SUCCESS);
		}
		else
			CommonUIFunc::NoticeDialog(text,ICON_SUCCESS);

		if ( item->m_uTaskType==INSTALL )
		{
			CStaticsticsMessage stMsg("52011");
			stMsg.Send();

			PostInstallInfoMsg postInstallInfoMsg;
			postInstallInfoMsg.m_appid = item->m_appid;
			postInstallInfoMsg.m_csFileSavePath = item->m_csFileSavePath;
			postInstallInfoMsg.m_strName = item->m_strName;
			postInstallInfoMsg.m_appVersion = item->m_appVersion;
			postInstallInfoMsg.deviceId = deviceId;
			postInstallInfoMsg.Send();

			PostInstallResultInfoMsg postInstallResultInfoMsg;
			postInstallResultInfoMsg.m_platform = item->m_platform;
			postInstallResultInfoMsg.m_fid = item->m_fid;
			postInstallResultInfoMsg.m_appid = item->m_appid;
			postInstallResultInfoMsg.bSuccess = true;
			postInstallResultInfoMsg.Send();

			PostInstallAppMsg postInstallAppMsg;
			postInstallAppMsg.size = item->m_TaskFileInfo->TotalSize;
			postInstallAppMsg.duration = 0;
			postInstallAppMsg.Send();

			if(item->m_platform == L"Android")
			{
				bool suc = false;
				wstring strtab = CStrOperation::IntToWString(item->m_fid) + item->m_strName;
				CStaticsticsMessageWithTab staMsgTab("900006", CCodeOperation::UnicodeToUTF_8(strtab).c_str());
				staMsgTab.SendAndGetReturn();
				suc = staMsgTab.ret;
			}

		}
		if(_curdeviceId!=deviceId&&CONFIG(_bAutoDeleteTask))
		{
			TaskListUI* listui=_tasklistUImap[deviceId+typestr[ALL_TASK]];
			if(listui)
			{
				CTaskModuleInnerMsg deleteTaskMsg;
				deleteTaskMsg._type = DELETE_TASK;
				deleteTaskMsg.nParam = item->m_uID;
				deleteTaskMsg.strParam = deviceId;
				deleteTaskMsg.nUserData = 1;
				deleteTaskMsg.Send();
				//TaskCenterModule::GetInstance()->DeleteTask(item->m_uID,deviceId);
			}
		}	
	}
	RETURN_IF(_curdeviceId!=deviceId);
	RETURN_IF(!item);
	LOG_DEBUG(L"TaskCenterControlUI::TaskFinishedNotify, UpdateTaskUI Begin");
	if(_curTasklist)
		_curTasklist->UpdateTaskUI(item,true);
	LOG_DEBUG(L"TaskCenterControlUI::TaskFinishedNotify, UpdateTaskUI End");
}

void TaskCenterControlUI::TaskFailNotify( CTaskItem* item, wstring deviceId )
{//INTERFUN;
	LOG_DEBUG(L"TaskCenterControlUI::TaskFailNotify begin");

	RETURN_IF(!item);
	if(item->bUpadateGrade)
	{
		//MainDlg::GetInstance()->GetInstallCaseUI()->Model.AppUpdategradeCancel(item);

		AppUpdategradeCancelMsg appUpdategradeCancelMsg;
		appUpdategradeCancelMsg.item = (void*)item;
		appUpdategradeCancelMsg.Send();
	}

	if ( item->m_uTaskType == INSTALL )
	{
		bool isPost = true;//IOS设备的话非越狱设备且是盗版的则不上传
		if( CStrOperation::toLowerW(item->m_platform)  != L"android")
		{
			DeviceData* pDeviceData = DEVICE_MANAGER->FindDevice(deviceId);
			if (pDeviceData != NULL)
			{
				if (!pDeviceData->GetDevInfo()->_ios_bJailBreaked)
				{
					if(!item->m_isLegalSoft)
					{
						isPost = false;
					}
				}
			}
			
		}

		if ( isPost )
		{
			PostInstallFailInfoMsg postInstallFailInfoMsg;
			postInstallFailInfoMsg.filePath = item->m_TaskFileInfo != NULL ? item->m_TaskFileInfo->savePath : item->m_csFileSavePath;
			postInstallFailInfoMsg.m_fid = item->m_fid;
			postInstallFailInfoMsg.m_appid = item->m_appid;
			postInstallFailInfoMsg.m_appVersion = item->m_appVersion;
			postInstallFailInfoMsg.TotalSize = item->m_TaskFileInfo != NULL? item->m_TaskFileInfo->TotalSize:0;
			postInstallFailInfoMsg.m_strName = item->m_strName;
			postInstallFailInfoMsg.m_installerror = item->m_installerror;
			postInstallFailInfoMsg.deviceId = deviceId;
			postInstallFailInfoMsg.Send();

			PostInstallResultInfoMsg postInstallResultInfoMsg;
			postInstallResultInfoMsg.bSuccess = false;
			postInstallResultInfoMsg.m_platform = item->m_platform;
			postInstallResultInfoMsg.m_fid = item->m_fid;
			postInstallResultInfoMsg.m_appid = item->m_appid;
			postInstallResultInfoMsg.Send();

		}


	}
	else if ( item->m_uTaskType == URL_DOWNLAOD )
	{
		PostDownloadFinishMsg postDownloadFinishMsg;
		postDownloadFinishMsg.m_nSpeed = item->m_nSpeed;
		postDownloadFinishMsg.m_platform = item->m_platform;
		postDownloadFinishMsg.m_fid = item->m_fid;
		postDownloadFinishMsg.m_uStatus = (int)item->m_uStatus;
		postDownloadFinishMsg.Send();
	}
	LOG_DEBUG(L"TaskCenterControlUI::TaskFailNotify post 统计信息 end");
	TaskUpdatedSpeedNotify(item,deviceId);
	LOG_DEBUG(L"TaskCenterControlUI::TaskFailNotify post updatetaskitem end");
}

void TaskCenterControlUI::OnDeviceConnect( wstring sn )
{//INTERFUN;
	LOG_DEBUG(L"TaskCenterControlUI::OnDeviceConnect");
	DeviceData* device=DEVICE_MANAGER->FindDevice(sn);
	if(device)
	{
		wstring imagepath=device->GetDevInfo()->_deviceType==Ios?L"#TaskCenter_icon_Apple":L"#TaskCenter_icon_Android";
		wstring devicename=device->GetDevInfo()->GetDeviceName();
		if(devicename.empty())
			devicename = sn;
		AddTreeNode(devicename.c_str(),imagepath,sn);
		//TaskCenterModule::GetInstance()->DealLastUnfinishedTasksOnDevice(sn);
		CTaskModuleInnerMsg dealUnFinishTaskMsg;
		dealUnFinishTaskMsg._type = DEAL_LASTUNFINISHEDTASK_ONDEVICE;
		dealUnFinishTaskMsg.strParam = sn;
		dealUnFinishTaskMsg.PostToUI();
	}
	LOG_DEBUG(L"TaskCenterControlUI::OnDeviceConnect FINISH");

}

void TaskCenterControlUI::OnDeviceHasUnfinishedTasks(wstring deviceId)
{
	GlobalTaskInfo * gtask=TaskControl::GetInstallTaskIndexByDevice(deviceId);
	

	RETURN_IF(!gtask);
	if(_wndContinueInstall)
	{
		if (_wndContinueInstall->GetHWND())
		{
			_usercancel = false;
			_wndContinueInstall->EndDialog(IDABORT);
		}
		SAFE_DELETE(_wndContinueInstall);
	}
	if(!_wndContinueInstall)
	{
		_wndContinueInstall=new MessageWnd(L"您的设备还有上次未完成的任务，是否继续？", L"继续安装", L"", MB_OKCANCEL, 0);
	}
	_usercancel=true;
	_mapNeedContinuesTasks[deviceId] = gtask;
	
	int wndResult = _wndContinueInstall->Show(Singleton::GetMainWnd());
	if(!_wndContinueInstall||IDABORT == wndResult)
	{
		/*if(_usercancel)
		{
			gtask->ClearTask();*/
			return;
		//}
	}

	if (IDOK != wndResult)
	{
		for (map<wstring,GlobalTaskInfo *>::iterator iterTmp = _mapNeedContinuesTasks.begin(); iterTmp != _mapNeedContinuesTasks.end(); ++iterTmp)
		{
			iterTmp->second->ClearTask();
		}
		_mapNeedContinuesTasks.clear();
	}
	/*TaskCenterModule::GetInstance()->GetTaskCount(true);*/


	
	for (map<wstring,GlobalTaskInfo *>::iterator iterTmp = _mapNeedContinuesTasks.begin(); iterTmp != _mapNeedContinuesTasks.end(); ++iterTmp)
	{
		wstring snTmp = iterTmp->first;
		GlobalTaskInfo *taskTmp = iterTmp->second;
		if(DEVICE_MANAGER->FindDevice(snTmp) != NULL)
		{
			taskTmp->StartThread();
			CTreeUI* _pLeftTree = _pLeftView->GetTree();
			if(_pLeftTree->GetItem(snTmp.c_str()))
				_pLeftTree->SelectItem(_pLeftTree->GetItem(snTmp.c_str())->GetIndex());
			OnTreeSelectChange(NULL);
		}
	}
	_mapNeedContinuesTasks.clear();

	CTaskModuleGetValueInnerMsg getTaskCountMsg;
	getTaskCountMsg._type = GET_TASKCOUNT;
	getTaskCountMsg.strParam = L"1";
	getTaskCountMsg.SendAndGetReturn();
	
}


void TaskCenterControlUI::OnDeviceDisconnect( wstring sn )
{//INTERFUN;
	LOG_DEBUG(L"TaskCenterControlUI::OnDeviceDisconnect");
	CTreeUI* _pLeftTree = _pLeftView->GetTree();
	RETURN_IF(!_pLeftTree);
	_pLeftTree->RemoveItem((CTreeNodeUI*)(_pLeftTree->GetItem(sn.c_str())));
	if(_curdeviceId == sn)
	{
		_pLeftTree->SelectItem(0);
		OnTreeSelectChange(NULL);
	}
	_pLeftTree->Resize();
	_pLeftTree->Invalidate();
	//int count = TaskCenterModule::GetInstance()->GetTaskCount(true);
	CTaskModuleGetValueInnerMsg getTaskCountMsg;
	getTaskCountMsg._type = GET_TASKCOUNT;
	getTaskCountMsg.strParam = L"1";
	getTaskCountMsg.SendAndGetReturn();
	int count = getTaskCountMsg.nParam;

	/*CControlUI* btn= this->GetWindow()->GetItem(_T("Task_Count"));
	if(count>0)
	{
		btn->SetText(CStrOperation::IntToWString(count).c_str());
		btn->SetVisible(true);
	}
	else
		btn->SetVisible(false);*/
	TopTaskCountMsg topTaskCountMsg;
	topTaskCountMsg.nParam = count;
	topTaskCountMsg.Send();

	TaskControl::OnDeviceDisconnect(sn.c_str());

	if(_wndContinueInstall)
	{
		if(_wndContinueInstall->GetHWND())
		{
			_wndContinueInstall->EndDialog(IDCANCEL);
			_usercancel=false;
			if (_mapNeedContinuesTasks.end() != _mapNeedContinuesTasks.find(sn))
			{
				_mapNeedContinuesTasks.erase(_mapNeedContinuesTasks.find(sn));
			}
		}
		SAFE_DELETE(_wndContinueInstall);
	}

	LOG_DEBUG(L"TaskCenterControlUI::OnDeviceDisconnect end");
}

bool TaskCenterControlUI::OnTreeSelectChange( TNotifyUI* pNotify )
{//INTERFUN;
	LOG_DEBUG(L"TaskCenterControlUI::OnTreeSelectChange");
	CTreeUI* _pLeftTree = _pLeftView->GetTree();
	if(_pLeftTree)
	{
		wstring text=_pLeftTree->GetCurSel()->GetText();
		wstring id=L"";
		if(((CTreeNodeUI*)_pLeftTree->GetCurSel())->IsGroup())
		{
			id=_pLeftTree->GetCurSel()->GetId();
			_listtype= ALL_TASK;
		}
		else
		{//不知此处为何一个节点还是get_parent Id
			id=L"download_task";//((CTreeNodeUI*)taskTree->GetCurSel())->get_parent()->GetId();
			wstring nodetId=((CTreeNodeUI*)_pLeftTree->GetCurSel())->GetId();
			_listtype = nodetId==L"download_unfinished"?UNFINISHED_TASK:FINISHED_TASK;
		}
		_curdeviceId=id==L"download_task"?L"":id;
		if(_curTasklist)
			_curTasklist->SetVisible(false);
		SetTaskCount(_curdeviceId);
		if(!_curdeviceId.empty())_listtype=ALL_TASK;
		if(_tasklistUImap.find(id+typestr[_listtype])!=_tasklistUImap.end())
			_curTasklist=_tasklistUImap[id+typestr[_listtype]];
		_curTasklist->RefreshData();
		_curTasklist->SetVisible(true);
		_curTasklist->Resize();
		_curTasklist->Invalidate();
	}
	LOG_DEBUG(L"TaskCenterControlUI::OnTreeSelectChange FINISH");
	return true;
}

void TaskCenterControlUI::SetTaskCount(wstring deviceId)
{//INTERFUN;
	//int count = TaskCenterModule::GetInstance()->GetTaskCount();
	CTaskModuleGetValueInnerMsg getTaskCountMsg;
	getTaskCountMsg._type = GET_TASKCOUNT;
	getTaskCountMsg.strParam = L"1";
	getTaskCountMsg.SendAndGetReturn();
	int count = getTaskCountMsg.nParam;


	int unfinishedcount=0;
	//int total = TaskCenterModule::GetInstance()->GetTaskCountOnDevice(deviceId,unfinishedcount);
	CTaskModuleGetValueInnerMsg getTaskCountOnDeviceMsg;
	getTaskCountOnDeviceMsg._type = GET_TASKCOUNT_ONDEVICE;
	getTaskCountOnDeviceMsg.strParam = deviceId;
	getTaskCountOnDeviceMsg.nParam = 0;//unfinishedcount;
	getTaskCountOnDeviceMsg.SendAndGetReturn();
	int total = getTaskCountOnDeviceMsg.nUserData;
	unfinishedcount = getTaskCountOnDeviceMsg.nParam;
	
	CTreeUI* _pLeftTree = _pLeftView->GetTree();
	if(!deviceId.empty())
	{
		
		DeviceData* device=DEVICE_MANAGER->FindDevice(deviceId);
		RETURN_IF(!device);
		wostringstream prowos;
		bool visible = total > 0;
		if(CONFIG(_bAutoDeleteTask))
		{
			if(unfinishedcount > 0)
			{
				prowos<<L"("; 
				prowos<<unfinishedcount;
				prowos<<L")";
			}
			else
			{
				visible = false;
			}
		}
		else if(total > 0)
		{
			prowos<<L"("; 
			prowos<<total;
			prowos<<L")";
		}
		
		CTreeNodeUI* node=(CTreeNodeUI*)_pLeftTree->GetItem(deviceId.c_str());//设备发现了节点还未创建
		RETURN_IF(!node);
		wstring nodetext = node->GetText();
		wstring nodetextex = device->GetDevInfo()->GetDeviceName();

		node->SetAttribute(_T("tooltip"),(nodetextex+prowos.str()).c_str());
		node->SetAttribute(_T("txtright.text"),prowos.str().c_str());

		node->SetVisible(visible);
		if(!visible)
		{	
			node->SetAttribute(_T("txtright.text"), L"");
			node->SetAttribute(_T("tooltip"), nodetextex.c_str());
			_pLeftTree->SelectItem(1);
			OnTreeSelectChange(NULL);
			return;
		}
	}
	else
	{
		wostringstream unfinishwos,finishedwos;
		if(deviceId.empty())
			unfinishwos<<CommonUIFunc::LoadString(_T("Task_DownloadingTask"));//BaseUI::LoadString(_T("Task_DownloadingTask"));
		else
			unfinishwos<<CommonUIFunc::LoadString(_T("Task_UnfinishedTask"));//BaseUI::LoadString(_T("Task_UnfinishedTask"));
		if(unfinishedcount>0){unfinishwos<<L"("; unfinishwos<<unfinishedcount;unfinishwos<<L")";}
		if(deviceId.empty())
		{
			_pLeftTree->GetItem(L"download_unfinished") ->SetText(unfinishwos.str().c_str());
			if(deviceId.empty())
			{
				finishedwos<< CommonUIFunc::LoadString(_T("Task_finishedDownTask"));//BaseUI::LoadString(_T("Task_finishedDownTask"));
			}
			else
			{
				finishedwos<<  CommonUIFunc::LoadString(_T("Task_finishedTask"));//BaseUI::LoadString(_T("Task_finishedTask"));
			}
			if(total-unfinishedcount>0){finishedwos<<L"("; finishedwos<<total-unfinishedcount;finishedwos<<L")";}
			_pLeftTree->GetItem(L"download_finished")->SetText(finishedwos.str().c_str());
		}
	}
	_pLeftTree->Resize();
	_pLeftTree->Invalidate();
	/*CControlUI* btn= this->GetWindow()->GetItem(_T("Task_Count"));
	if(count>0)
	{
		wstring cnttext=CStrOperation::IntToWString(count);
		int x,y,r,b;
		btn->GetInitPos(x,y,r,b);
		btn->SetInitPos(r-(cnttext.length()*6+16),y,r,b);
		btn->SetText(cnttext.c_str());
		btn->SetVisible(true);
		btn->Resize();
		btn->Invalidate();
	}
	else
	{
		btn->SetVisible(false);
	}*/
	TopTaskCountMsg topTaskMsg;
	topTaskMsg.nParam = count;
	topTaskMsg.Send();

}

void TaskCenterControlUI::TaskCountChangeNotify()
{//INTERFUN;
}

void TaskCenterControlUI::TaskRemoveNotify( int taskid, wstring deviceId )
{//INTERFUN;
	SetTaskCount(deviceId);
	if(_curdeviceId!=deviceId)
	{
		TaskListUI* listui=_tasklistUImap[deviceId+typestr[ALL_TASK]];
		if(listui&&listui->IsAutoDelFinishedTask())
			listui->RemoveTaskItemUI(taskid);
	}	
	RETURN_IF(_curdeviceId!=deviceId);
	_curTasklist->RemoveTaskItemUI(taskid);
}

void TaskCenterControlUI::OnDeviceNameChanged( wstring sn )
{//INTERFUN;
	CTreeUI* _pLeftTree = _pLeftView->GetTree();
	RETURN_IF(!_pLeftTree);
	DeviceData* device=DEVICE_MANAGER->FindDevice(sn);
	CTreeNodeUI* node=((CTreeNodeUI*)(_pLeftTree->GetItem(sn.c_str())));
	if(node&&device)
	{
		wstring tip=node->GetToolTip();
        wstring strOld = node->GetText();
        if (strOld.length() == 0)
            return;
		wstring devicename = device->GetDevInfo()->GetDeviceName();
        tip=CStrOperation::ReplaceW(tip,node->GetText(),devicename);
		node->SetAttribute(_T("txt.text"),devicename.c_str());
		node->SetToolTip(tip.c_str());
	}
	_pLeftTree->Resize();
	_pLeftTree->Invalidate();
}

void TaskCenterControlUI::SelectTreeNode()
{
	vector<wstring>  devices=DEVICE_MANAGER->GetAllDevice();
	CTreeUI* _pLeftTree = _pLeftView->GetTree();
	for(size_t i=0;i<devices.size();i++)
	{
		wstring deviceId=devices[i];
		GlobalTaskInfo * gtask=TaskControl::GetInstallTaskIndexByDevice(deviceId);
		if(gtask&&!gtask->_beRemoved&&gtask->m_tlTaskList.GetUnfinishedTaskCount()>0)
		{
			if(_pLeftTree->GetItem(deviceId.c_str())&&_pLeftTree->GetItem(deviceId.c_str())!=_pLeftTree->GetCurSel())
			{
				_pLeftTree->SelectItem(_pLeftTree->GetItem(deviceId.c_str())->GetIndex());
				OnTreeSelectChange(NULL);
			}
			return;
		}
	}

	int selindex=TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.GetUnfinishedTaskCount()>0?0:1;
	if(_pLeftTree->GetCurSel()!=_pLeftTree->GetItem(selindex))
	{
		_pLeftTree->SelectItem(selindex);
		OnTreeSelectChange(NULL);
	}
}

void TaskCenterControlUI::OnDeviceLinkOut( wstring sn, void* pPhone )
{
	OnUsbDisconnect(sn);

}

void TaskCenterControlUI::OnUsbConnect( wstring sn )
{
	LOG_DEBUG(L"TaskCenterControlUI::OnUsbConnect");
	//PostMessage(GetOwnerHWnd(),WM_TASKUI_DEVICE_CONNECTED,(WPARAM)BaseOperation::AllocateBuffer(sn),NULL);
	DeviceConnectMsg msg; 
	msg.strParam = sn;
	msg.PostToUI();
	LOG_DEBUG(L"TaskCenterControlUI::OnUsbConnect end");
}

void TaskCenterControlUI::OnUsbDisconnect( wstring sn )
{
	OnDeviceDisconnect(sn);
	CTaskModuleInnerMsg msg;
	msg._type = REMOVE_DEAMON;
	msg.strParam = sn;
	msg.Send();
	//TaskCenterModel::GetInstance()->RemoveInstalldeamon(sn);
}

void TaskCenterControlUI::OnWifiConnect( wstring sn )
{
	OnUsbConnect(sn);
}

void TaskCenterControlUI::OnWifiDisconnect( wstring sn )
{
	OnUsbDisconnect(sn);
}

void TaskCenterControlUI::OnDeviceActived( wstring sn )
{
}

bool TaskCenterControlUI::Handle( const CWebResourceDownload* pMsg )
{
	if (NULL==pMsg)
	{
		return false;
	}
	if(!pMsg->_url.empty())
	{
		LOG->WriteDebug(pMsg->_url);
		CTaskItem* finitem=TaskControl::g_gtiGlobalTaskInfo->m_tlTaskList.findSameItem(pMsg->_url.c_str(),pMsg->_savepath.c_str(),TASK_KIND::URL_DOWNLAOD);
		if(!finitem)
		{
			StartDownloadParam* startDownloadParam = new StartDownloadParam;
			startDownloadParam->url = pMsg->_url;
			startDownloadParam->savepath = pMsg->_savepath;
			startDownloadParam->appname= pMsg->_appname;
			startDownloadParam->version = pMsg->_version;
			startDownloadParam->platform = pMsg->_platform;
			startDownloadParam->fid = pMsg->_fid;
			startDownloadParam->bUpdategrade = false;
			startDownloadParam->needInstall = false;
			startDownloadParam->oldappfullpath = L"";
			startDownloadParam->isLegalSoft = pMsg->_bIsLegal;
			startDownloadParam->taskupgradetype = 0;
			startDownloadParam->appID = L"";
			startDownloadParam->pCpaInfo = pMsg->_pCpaInfo;
			CTaskModuleInnerMsg startDownloadMsg;
			startDownloadMsg._type = STARTDOWNLOAD;
			startDownloadMsg.wParam = (WPARAM)startDownloadParam;
			startDownloadMsg.Send();
			delete startDownloadParam;
			startDownloadParam = NULL;
			//TaskCenterModel::GetInstance()->StartDownLoad(pMsg->_url, pMsg->_savepath, pMsg->_appname, pMsg->_version, pMsg->_platform, pMsg->_fid, false, false, L"", pMsg->_bIsLegal, NOTUPGRADE, L"", pMsg->_pCpaInfo);
			if(CONFIG(_bDownLoadMsg))
				CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"Task_Added_to_list"),ICON_SUCCESS);//BaseUI::LoadString(L"Task_Added_to_list"),ICON_SUCCESS);
		}
		else
		{
			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"Task_Exist_in_list"),ICON_WARNING);//BaseUI::LoadString(L"Task_Exist_in_list"),ICON_WARNING);
		}
	}

	return true;
}

bool TaskCenterControlUI::Handle( const DeviceConnectMsg* pMsg )
{
	if (NULL == pMsg)
	{
		return true;
	}
	OnDeviceConnect(pMsg->deviceID);
	return true;
}

bool TaskCenterControlUI::Handle( const DeviceLinkOutMsg* pMsg )
{
	if (NULL == pMsg)
	{
		return true;
	}
	OnDeviceLinkOut(pMsg->id,pMsg->pPhone);
	return true;
}

bool TaskCenterControlUI::Handle( const NotifyTaskItemMsg* pMsg )
{
	if (NULL == pMsg)
	{
		return true;
	}
	switch (pMsg->message)
	{
	case WM_TASK_FINISHED:
		{
			CTaskItem* item = (CTaskItem*)pMsg->pOriginalTask;
			if (item)
			{
				TaskFinishedNotify(item,pMsg->curID);
			}
			break;
		}
	case WM_TASK_UPDATESPEED:
		{
			CTaskItem* item = (CTaskItem*)pMsg->pTask;
			if (item)
			{
				TaskUpdatedSpeedNotify(item,pMsg->curID);
			}
			break;
		}
	case WM_TASK_FAIL:
		{
			CTaskItem* item = (CTaskItem*)pMsg->pTask;
			if (item)
			{
				TaskFailNotify(item,pMsg->curID);
			}
			break;
		}
	case WM_TASK_ADDED:
		{
			CTaskItem* item = (CTaskItem*)pMsg->pTask;
			if (item)
			{
				OneTaskAdded(item);
			}
			break;
		}
	case WM_TASK_REMOVED:
		{
			TaskRemoveNotify(pMsg->taskid,pMsg->curID);
			break;
		}
	case WM_TASK_COUNTCHANGE:
		{
			break;
		}
	}
	return true;
}

std::wstring TaskCenterControlUI::GetStyleID()
{
	return L"TaskCenterComposite";
}
