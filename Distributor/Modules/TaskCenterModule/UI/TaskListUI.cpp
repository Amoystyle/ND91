#include "StdAfx.h"
#include "TaskListUI.h"
//#include "logic/InstallsCase/InstallCaseModelEx.h"
//#include "logic/InstallsCase/InstallsCaseData.h"

#include "Common/DealTime.h"
//#include "Base/AutoShowTipLabelUI.h"
#include "ui/SimpleListItem.h"
//#include "UI/InstallsCase/InstallCaseUI.h"
//#include "Logic/DeamonInstall/AppSyncInstall.h"

#include "UI/CommonUIFunc.h"//"CommonUI/UI/CommonUIFunc.h"
#include "TaskModuleInnerMsg.h"
#include "Core/DeviceManager.h"

#define MENU_ID_TASK_ALL_DOWN 20000 //任务中心
#define MENU_ID_TASK_ALL_DOWN_FINISHED 21000
#define MENU_ID_TASK_ALL_DOWN_UNFINISH 22000
#define MENU_ID_TASK_ALL_INSTALL 23000
#define MENU_ID_TASK_ALL_BOTTOM_INSTALL 24000
#define MENU_ID_TASK_ALL_BOTTOM_ADD 25000

#define MENU_ID_TASK_DELETE 71101//删除
#define MENU_ID_TASK_DELETE_ALL 71102//删除全部
#define MENU_ID_TASK_PAUSE 71103//暂停
#define MENU_ID_TASK_START 71104//开始

#define MENU_ID_TASK_INSTALL 71105//安装到
#define MENU_ID_TASK_ADD 71106//添加到

#define MENU_ID_TASK_SELECT_ALL 71107//全选
#define MENU_ID_TASK_SELECT_NONE 71108//不选	
#define MENU_ID_TASK_OPEN_DIR	71109//不选	
#define MENU_ID_TASK_CREATE_NEWCASE   71110//创建新方案

#define MENU_ID_INSTALL_DEVICE_START	1000
#define MENU_ID_INSTALL_CASE_START		2000

#define Finished_GroupId  L"finishedlist"
#define UNFinish_GroupId  L"unfinishlist"

bool __stdcall TaskListSort(CListItemUI* pA, CListItemUI* pB, int nCol, bool exchange)
{//INTERFUN;
	CListItemUI* pItemA = dynamic_cast<CListItemUI*>(pA);
	CListItemUI* pItemB = dynamic_cast<CListItemUI*>(pB);
	if(_tcscmp(pItemA->GetId(),UNFinish_GroupId)==0
					||_tcscmp(pItemA->GetId(),Finished_GroupId)==0
					||_tcscmp(pItemB->GetId(),UNFinish_GroupId)==0
					||_tcscmp(pItemB->GetId(),Finished_GroupId)==0)
					return (!exchange);
	int taskidA=_wtoi(pA->GetId()); int taskidB=_wtoi(pB->GetId());
	//CTaskItem* taskA=TaskCenterModel::GetInstance()->FindTaskItemById(taskidA);
	CTaskModuleGetValueInnerMsg msgTaskA; 
	msgTaskA._type  = FIND_TASKITEM_BYID;
	msgTaskA.nParam = taskidA;
	msgTaskA.SendAndGetReturn();
	CTaskItem* taskA = (CTaskItem*)(msgTaskA.wParam);

	//CTaskItem* taskB=TaskCenterModel::GetInstance()->FindTaskItemById(taskidB);
	CTaskModuleGetValueInnerMsg msgTaskB; 
	msgTaskB._type = FIND_TASKITEM_BYID;
	msgTaskB.nParam = taskidB;
	msgTaskB.SendAndGetReturn();
	CTaskItem* taskB = (CTaskItem*)(msgTaskB.wParam);

	if(taskA&&taskB)
	{
		if(taskA->m_uStatus==FINISHED&&taskB->m_uStatus!=FINISHED)return(!exchange);
		if(taskA->m_uStatus!=FINISHED&&taskB->m_uStatus==FINISHED)return(!exchange);
	}
	if (pItemA && pItemB)
	{
		switch (nCol)
		{
		case 0:
			{
				wstring tempA,tempB;
				tempA = CStrOperation::trimLeftW(pItemA->GetItem(nCol)->GetItem(1)->GetText(), L" ");
				tempB = CStrOperation::trimLeftW(pItemB->GetItem(nCol)->GetItem(1)->GetText(), L" ");
				return lstrcmpi(tempA.c_str(), tempB.c_str()) > 0;


			//	return lstrcmpiW(pItemA->GetItem(nCol)->GetItem(1)->GetText(),pItemB->GetItem(nCol)->GetItem(1)->GetText())>0;
			}
		case 1:
			{
				return _tcscmp(pItemA->GetItem(nCol)->GetItem(0)->GetText(),pItemB->GetItem(nCol)->GetItem(0)->GetText())>0;
			}
		case 2:
			{
				return pItemA->GetSubItemUserdata(nCol) > pItemB->GetSubItemUserdata(nCol);
			}
		case 3:
			{
				if(taskA->m_uTaskType==URL_DOWNLAOD&&taskA->m_uStatus==FINISHED)//下载完成已分平台，不再按平台排序
					return (!exchange);
				return _tcscmp(pItemA->GetItem(nCol)->GetText(),pItemB->GetItem(nCol)->GetText())>0;
			}
		case 4:
			{
				if(taskA&&taskB)
				{
					if(taskA->m_uTaskType==URL_DOWNLAOD&&taskA->m_uStatus==FINISHED)
						return (taskA->m_tEnd<taskB->m_tEnd);
					if(taskA->m_uStatus==FINISHED&&taskB->m_uStatus==FINISHED)return (!exchange);
					if(taskA->m_uStatus==PROCEDING&&taskB->m_uStatus==PROCEDING)
						return taskA->GetTaskPercent()>taskB->GetTaskPercent();
					else if(taskA->m_uStatus <= taskB->m_uStatus)
						return (!exchange);
					else
						return taskA->m_uStatus > taskB->m_uStatus;					
				}
			}
			break;
		default:
			return (!exchange);
			break;
		}
	}
	return (!exchange);
} 

TaskListUI::TaskListUI( wstring deviceId, TASK_LIST_TYPE isFinishedList)//_curdeviceId(deviceId)
{//INTERFUN;
	_curdeviceId=deviceId;
	_listtype = isFinishedList;
	_IsAutoDelFinishedTask = CONFIG(_bAutoDeleteTask);
	_platform=0;
	_type=0;
	_lastUnFinishTaskId=-1;
	_keydeleteid = _T("");
	_pMenu=NULL;
	InitializeCriticalSection(&_removelistitemlock);
}

TaskListUI::~TaskListUI(void)
{//INTERFUN;
	//DeviceNotifyCenter::GetInstance()->DetachReceiver(USER_KEY, this);
	DeleteCriticalSection(&_removelistitemlock);
}

void TaskListUI::OnCreate()
{//INTERFUN;
	CListUI* taskList=(CListUI*)GetItem(_T("downtasklist"));
	if(taskList)
	{
		CLabelUI* notasklabel=(CLabelUI*)GetItem(_T("no_tasks_layout"))->GetItem(1);
		if(_curdeviceId.empty()&&_listtype==FINISHED_TASK)
		{
			_finisheddowntaskplat=L"Android";
			GetItem(_T("tasklist_headertab"))->SetVisible(true);
			GetItem(_T("tasklist_headertabback"))->SetVisible(true);
			taskList->SetInitPos(0,39,1,32,8);
			
			notasklabel->SetText(CommonUIFunc::LoadString(_T("Cur_No_Downloading_Task")));
			GetItem(_T("taskcenter_right_bottom.btn_pause_task"))->SetVisible(false);
			GetItem(_T("taskcenter_right_bottom.btn_start_task"))->SetVisible(false);
			GetItem(_T("taskcenter_right_bottom.install_to"))->SetVisible(true);
			GetItem(_T("taskcenter_right_bottom.add_to"))->SetVisible(true);
			taskList->GetCol(3)->SetAttribute(L"sort.image",L"");//平台排序的箭头去掉
			taskList->GetCol(4)->SetAttribute(L"text",L"下载完成时间");
		}
		taskList->EnableScrollBar(true,false);
		taskList->SetSortCallBack(TaskListSort);
	}	
	//DeviceNotifyCenter::GetInstance()->AttachReceiver(USER_KEY, this);
}

tstring TaskListUI::GetTaskStatusIconId(TASK_STATUS ts)
{//INTERFUN;
	switch(ts)
	{
	case WAITING:			//等待
		return _T("#icon_Task_Waiting");
		break;
	case PROCEDING:			//正在执行
		return _T("#icon_Task_Running");
		break;
	case SUSPENDED:			//挂起/暂停
		return _T("#icon_Task_Pause");
		break;
	case FAILED:			//失败
		return _T("#icon_Task_Warning");
		break;
	case FINISHED:			//完成
		return _T("#icon_Task_Success");
		break;
	case DEALLOCK:
		return _T("#icon_Task_Running");
		break;
	}
	return _T("");
}

tstring TaskListUI::GetDownProgressText(CTaskItem* item)
{//INTERFUN;
	switch(item->m_uStatus)
	{
	case PROCEDING:
	case DEALLOCK:
		{
			wostringstream prowos;
			if(item->m_uTaskType==URL_DOWNLAOD)
				prowos<<item->GetTaskPercent();
			else
				prowos<<item->m_nProcess;
			prowos<<L"% ";
			if(item->m_uTaskType!=URL_DOWNLAOD)
			{
				//安装npk时m_installstateStr为空，导致显示（X/S）文本
				if(!item->m_installstateStr.empty())
					prowos<<item->m_installstateStr;
				else
					prowos<<L"正在安装";
			}
			else
				prowos<<L"("<<CStrOperation::FormatFileSize((int64_t)(item->m_nSpeed))<<L"/s)";
			tstring str=prowos.str();
			return str;
		}
		break;
	case FINISHED:
		{
			switch(item->m_uTaskType)
			{
			case INSTALL:
				{
					wostringstream installerror;
					installerror<<CommonUIFunc::LoadString(_T("install_finished"));//BaseUI::LoadString(_T("install_finished"));
					if(!item->m_installerror.empty())
					{
						installerror<<L"【";installerror<<item->m_installerror;installerror<<L"】";
					}
					return installerror.str();
				}
			case MOVEANDROIDAPP:
				return CommonUIFunc::LoadString(_T("Move_App_Success"));
			case URL_DOWNLAOD: 
				{
					if(item->m_uStatus==FINISHED)
					{
						CDealTime dtime(item->m_tEnd+8*3600);
						return CCodeOperation::UTF_8ToUnicode(dtime.TimeFormat()).c_str();
					}
					return CommonUIFunc::LoadString(_T("download_finished"));
				}
			default:
				break;
			}
		}
		break;
	case SUSPENDED:
		if(item->m_uTaskType==URL_DOWNLAOD)
			return CommonUIFunc::LoadString(_T("download_stoped"));
		else
			return item->m_installstateStr;
		break;
	case WAITING:
		{
			if(item->m_uTaskType==INSTALL)
				return CommonUIFunc::LoadString(_T("install_waiting"));//BaseUI::LoadString(_T("install_waiting"));
			else if(item->m_uTaskType==MOVEANDROIDAPP)
				return CommonUIFunc::LoadString(_T("wait_for_Move_App"));//BaseUI::LoadString(_T("wait_for_Move_App"));
			else
				return CommonUIFunc::LoadString(_T("download_waiting"));//BaseUI::LoadString(_T("download_waiting"));
		}
		break;
	case FAILED:
		{
			if(item->m_uTaskType==INSTALL||item->m_uTaskType==MOVEANDROIDAPP)
			{
				wostringstream installerror;
				if(item->m_uTaskType==INSTALL)
					installerror<<CommonUIFunc::LoadString(_T("install_fail"));//BaseUI::LoadString(_T("install_fail"));
				else if(item->m_uTaskType==MOVEANDROIDAPP)
					installerror<<CommonUIFunc::LoadString(_T("_Move_App_fail"));//BaseUI::LoadString(_T("_Move_App_fail"));
				if(!item->m_installerror.empty())
				{
					wstring errorstr=item->m_installerror;
					//if(item->m_installerror.find(L"INSTALL_FAILED_")!=wstring::npos||item->m_installerror.find(L"kAMD")==0)
					{
						LOG->WriteDebug(_T("安装错误") + item->m_installerror);
						const wchar_t* pcharerr= CommonUIFunc::LoadString(item->m_installerror.c_str());//BaseUI::LoadStringW(item->m_installerror.c_str());
						if(pcharerr)
							errorstr=pcharerr;
						if(L"iPhone" == item->m_platform)
						{
							if(	item->m_installerror == L"原因未知")
							{
								RequestNeedInstallAppSyncMsg appSyncMsg;
								appSyncMsg.deviceID = _curdeviceId;
								appSyncMsg.bSync = true;
								appSyncMsg.SendAndGetReturn();
								//AppSyncInstall appsync;
								/*if(appsync.CanInstall(_curdeviceId))
									errorstr=L"您需要到首页安装AppSync";*/
								if(appSyncMsg.bNeed)
									errorstr=L"您需要到首页安装AppSync";
							}
						}
					}
					installerror<<L"【";installerror<<errorstr;installerror<<L"】";
				}
				return  installerror.str();
			}
			else 
				return CommonUIFunc::LoadString(_T("download_fail"));//BaseUI::LoadString(_T("download_fail"));
		}
		break;
	}
	return _T("");
}


void TaskListUI::InsertTask( CTaskItem* item )
{//INTERFUN;
	RETURN_IF(!item);
	RETURN_IF(item->m_uTaskType==URL_DOWNLAOD&&item->m_uStatus==FINISHED&&!CFileOperation::IsFileExist(item->m_TaskFileInfo->savePath.c_str()));
	CControlUI* con= GetItem(_T("no_tasks_layout"));
	con->SetVisible(false);
	CControlUI* pItem=new CSimpleListItemUI();
	pItem->SetId(CStrOperation::IntToWString(item->m_uID).c_str());
	pItem->SetInitPos(0,0,0,45,4);


	TooltipData tooltipdata;

	//名称
	CControlUI* namelayout=new CLayoutUI();
	namelayout->SetInitPos(0,0,0,0,8);
	namelayout->SetStyle(_T("Layout"));
	CControlUI* conimg=new CControlUI();
	conimg->SetStyle(_T("Picture"));
	conimg->SetAttribute(_T("pos"),_T("9|5,-1,30,30"));
	//conimg->SetAttribute(_T("bk.image"),GetTaskStatusIconId(item->m_uStatus).c_str());
	CControlUI* labelappname=new CLabelUI();
	labelappname->SetAttribute(_T("pos"),_T("8|38,0,5,0"));
	wstring name=!item->m_strName.empty()?item->m_strName:CFileOperation::GetFileNameNoExt(item->m_TaskFileInfo->savePath);
	if(_curdeviceId.empty()&&item->m_uStatus!=FINISHED)
		conimg->SetAttribute(_T("bk.image"),L"#TaskCenter_down_default");
	else
	{
		wstring ext=CFileOperation::GetFileExt(item->m_TaskFileInfo->savePath);
		if(!ext.empty())
			name+=L"."+ext;
		if(!item->m_appiconpath.empty()&&CFileOperation::IsFileExist(item->m_appiconpath.c_str()))
			conimg->SetAttribute(_T("bk.image"),item->m_appiconpath.c_str());
		else
			conimg->SetAttribute(_T("bk.image"),L"#TaskCenter_Appicon_Default_32");
	}
	labelappname->SetAttribute(_T("text"),name.c_str());
	labelappname->SetAttribute(_T("txt.endellipsis"),_T("1"));
	namelayout->Add(conimg);
	namelayout->Add(labelappname);
	CControlUI* pLegalControl = new CControlUI();
	pLegalControl->SetId(L"LegalControl");
	pLegalControl->SetInitPos(25,0,41,16,0);
	pLegalControl->SetStyle(_T("Picture"));
	pLegalControl->SetAttribute(_T("bk.image"), _T("#TaskCenter_legal_SoftIcon"));        
	namelayout->Add(pLegalControl);
	pLegalControl->SetVisible(item->m_isLegalSoft);
	pItem->Add(namelayout);
	tooltipdata.name = name;
	//版本
	CControlUI* versionlay=new CLayoutUI();
	versionlay->SetStyle(_T("Layout"));
	versionlay->SetInitPos(0,0,100,0,7);
	pItem->Add(versionlay);

	CControlUI* labelversion=new CLabelUI();
	labelversion->SetInitPos(5,0,0,0,8);
	labelversion->SetStyle(_T("Label"));
	labelversion->SetText(item->m_appVersion.c_str());
	versionlay->Add(labelversion);
	if(_curdeviceId.empty()&&item->bUpadateGrade&&item->m_uStatus!=FINISHED)
	{
		labelversion->SetInitPos(5,0,30,0,8);
		CControlUI* uppic=new CControlUI();
		uppic->SetAttribute(L"pos",item->m_isLegalSoft?L"5|30,13,5,14":L"5|21,15,10,14");
		uppic->SetStyle(_T("Picture"));
		uppic->SetAttribute(L"bk.image",item->m_isLegalSoft?L"#TaskCenter_legal_upgrade":L"#TaskCenter_piracy_upgrade");
		versionlay->Add(uppic);
	}
	tooltipdata.version = item->m_appVersion;

	//大小
	CControlUI* labelsize=new CLabelUI();
	labelsize->SetInitPos(0,0,80,0,7);
	labelsize->SetStyle(_T("Label"));
	labelsize->SetAttribute(_T("txt.pos"),_T("8|5,0,0,0"));
	labelsize->SetText(CStrOperation::FormatFileSize((int64_t)(item->m_TaskFileInfo->TotalSize)).c_str());
	pItem->Add(labelsize);
	((CListItemUI*)pItem)->SetSubItemUserdata(2,item->m_TaskFileInfo->TotalSize);
	tooltipdata.size = CStrOperation::FormatFileSize((int64_t)(item->m_TaskFileInfo->TotalSize));

	//平台
	CControlUI* labelplat=new CLabelUI();
	labelplat->SetInitPos(0,0,70,0,7);
	labelplat->SetStyle(_T("Label"));
	labelplat->SetAttribute(_T("txt.pos"),_T("8|5,0,0,0"));
	labelplat->SetText(item->m_platform.c_str());
	labelplat->SetAttribute(_T("txt.halign"),_T("left"));
	pItem->Add(labelplat);

	tooltipdata.platform = item->m_platform;

	//进度
	CControlUI* progresslay=new CLayoutUI();
	progresslay->SetStyle(_T("Layout"));
	progresslay->SetInitPos(0,0,0,0,8);
	pItem->Add(progresslay);

	CProgressBarUI* progress=new CProgressBarUI();
	progress->SetInitPos(-1,-1,180,13,9);
	progress->SetAttribute(_T("bk.image"),_T("#TaskCenter_progressbar_bk"));
	progress->SetAttribute(_T("forebk.image"),_T("#TaskCenter_progressbar_green"));
	progress->SetRange(0,100);
	progress->SetPos(item->GetTaskPercent());
	bool progressvisible=(item->m_uStatus!=FINISHED&&item->m_uStatus!=FAILED);
	progress->SetVisible(progressvisible);
	progresslay->Add(progress);
	

	CControlUI* labelprogress=new CAutoShowTipLabelUI();
	labelprogress->SetInitPos(0,0,0,0,8);
	labelprogress->SetText(GetDownProgressText(item).c_str());
	labelprogress->SetAttribute(_T("txt.endellipsis"),_T("1"));
	if(item->m_uStatus==FAILED)
		labelprogress->SetAttribute(_T("txt.textcolor"),_T("ff0000"));
	labelprogress->SetAttribute(_T("txt.halign"),_T("center"));
	labelprogress->SetAttribute(_T("txt.font"),_T("font12"));
	progresslay->Add(labelprogress);
// 	labelprogress->SetToolTipShow(true);
// 	labelprogress->SetToolTip(toolTipText.c_str());
	
	//操作
	CControlUI* lay=new CLayoutUI();
	lay->SetInitPos(0,0,0,0,8);
	lay->SetStyle(_T("Layout"));
	lay->SetAttribute(_T("mode"),_T("lefttoright"));
	lay->SetAttribute(_T("startx"),_T("5"));
	lay->SetAttribute(_T("spacex"),_T("10"));
	pItem->Add(lay);

	CButtonUI* btnstart=new CButtonUI();
	btnstart->SetId(_T("item_btn_start"));
	btnstart->SetStyle(_T("ThreeBack_Button"));
	btnstart->SetAttribute(L"bk.image",GetButtonText(item->m_uStatus).c_str());
	btnstart->SetToolTip(GetButtonText(item->m_uStatus,true).c_str());
	btnstart->SetAttribute(_T("pos"),_T("0|0,16,12,30"));
	btnstart->SetAttribute(L"cursor",L"hand");
	btnstart->SetINotifyUI(this);
	lay->Add(btnstart);
	CButtonUI* btndel=new CButtonUI();
	btndel->SetId(_T("item_btn_del"));
	btndel->SetStyle(_T("ThreeBack_Button"));
	btndel->SetAttribute(L"bk.image",_T("#TaskCenter_task_delete"));
	btndel->SetAttribute(_T("pos"),_T("0|0,17,11,30"));
	btndel->SetAttribute(L"cursor",L"hand");
	btndel->SetToolTip(CommonUIFunc::LoadString(_T("Task_delete")));//(BaseUI::LoadString(_T("Task_delete")));
	btndel->SetINotifyUI(this);
	lay->Add(btndel);

	CControlUI* labelnstall=new CLabelUI();
	labelnstall->SetInitPos(5,0,0,0,8);
	labelnstall->SetStyle(_T("Label"));
	labelnstall->SetAttribute(_T("txt.halign"),_T("left"));
	if(item->m_uTaskType==INSTALL)
		labelnstall->SetText(CommonUIFunc::LoadString(_T("app_installing")));//BaseUI::LoadString(_T("app_installing")));
	else if(item->m_uTaskType==MOVEANDROIDAPP)
		labelnstall->SetText(CommonUIFunc::LoadString(_T("app_moving")));//BaseUI::LoadString(_T("app_moving")));
	lay->Add(labelnstall);
	btnstart->SetVisible(false);
	btndel->SetVisible(false);

	if((item->m_uTaskType==INSTALL||item->m_uTaskType==MOVEANDROIDAPP)
		&&item->m_uStatus==PROCEDING)
		labelnstall->SetVisible(true);
	else
		labelnstall->SetVisible(false);
	
	CListUI* taskList=(CListUI*)GetItem(_T("downtasklist"));
	if(taskList)
	{
		if(item->m_uStatus!=FINISHED)
		{
			if(taskList->GetItem(Finished_GroupId))
			{
				AddListGroupTittle(UNFinish_GroupId,CommonUIFunc::LoadString(_T("Task_UnfinishedTask")));//BaseUI::LoadString(_T("Task_UnfinishedTask")));
				taskList->Insert(pItem,taskList->GetItem(taskList->GetItem(_T("FinishedList"))->GetIndex()-1));
			}
			else
			{
				if(_listtype==ALL_TASK&&taskList->GetCount()<1)
					AddListGroupTittle(UNFinish_GroupId,CommonUIFunc::LoadString(_T("Task_UnfinishedTask")));//BaseUI::LoadString(_T("Task_UnfinishedTask")));
				taskList->Add(pItem);
			}
			_lastUnFinishTaskId=item->m_uID;
		}
		else
		{
			if(_listtype==ALL_TASK)
			{
				AddListGroupTittle(Finished_GroupId,CommonUIFunc::LoadString(_T("Task_finishedTask")));//BaseUI::LoadString(_T("Task_finishedTask")));
				
			}
			else
			{
				btndel->SetToolTip(CommonUIFunc::LoadString(_T("Task_deleteclear")));//BaseUI::LoadString(_T("Task_deleteclear")));
			}
			taskList->Add(pItem);
		}
		taskList->CheckItem(false);
		for (int i=0;i<taskList->GetCount();i++)
		{
			if(taskList->GetItem(i)->IsSelect())
				taskList->CheckItem(true,i);
		}
		taskList->Resize();
		taskList->Invalidate();
		pItem->RemoveEventDelegate();
		pItem->AddEventDelegate(&MakeDelegate(this, &TaskListUI::MoveEventOnList));
	}

	SetToolTipText(tooltipdata,pItem);
}


void TaskListUI::SetToolTipText(TooltipData pNode, CControlUI* item)
{
	wstring toolTipText;
	if (!item)
		return;
	toolTipText += _T("名称：");
	toolTipText += pNode.name;
	toolTipText += _T("\r\n");

	toolTipText += _T("版本：");
	toolTipText += pNode.version;
	toolTipText += _T("\r\n");

	toolTipText += _T("大小：");
	toolTipText += pNode.size;
	toolTipText += _T("\r\n");

	toolTipText += _T("平台：");
	toolTipText += pNode.platform;



	item->SetToolTipShow(true);
	item->SetToolTip(toolTipText.c_str());
}

bool TaskListUI::MoveEventOnList(TEventUI& event)
{//INTERFUN;
	CControlUI* pControl = event.pSender;
	RETURN_TRUE_IF(!pControl);

	switch(event.Type)
	{
	case UIEVENT_MOUSEENTER:
		ShowListItemButton(pControl, true);
		break;
	case UIEVENT_MOUSELEAVE:
		ShowListItemButton(pControl, false);
		break;
	}
	return true;
}

void TaskListUI::ShowListItemButton(CControlUI* pControl, bool show)
{//INTERFUN;
	CListContainerItemUI* listitem=(CListContainerItemUI*)pControl;
	int taskid=_wtoi(listitem->GetId());
	//CTaskItem* item=TaskCenterModel::GetInstance()->FindTaskItemById(taskid);

	CTaskModuleGetValueInnerMsg msgTask; 
	msgTask._type  = FIND_TASKITEM_BYID;
	msgTask.nParam = taskid;
	msgTask.SendAndGetReturn();
	CTaskItem* item = (CTaskItem*)(msgTask.wParam);

	RETURN_IF(!item);
	if(!_curdeviceId.empty()&&item->m_uStatus!=FAILED&&item->m_uStatus!=FINISHED&&item->m_uStatus!=WAITING)
	{
		//listitem->GetItem(5)->GetItem(0)->SetVisible(false);
		//listitem->GetItem(5)->GetItem(1)->SetVisible(false);
		//listitem->GetItem(5)->GetItem(2)->SetVisible(true);
	}
	else
	{
		if((_curdeviceId.empty()&&item->m_uStatus==FINISHED)
			||(!_curdeviceId.empty()&&item->m_uStatus==WAITING)
			||(item->m_uStatus==FINISHED&&item->m_uTaskType==MOVEANDROIDAPP))//移动成功不显示重试
			listitem->GetItem(5)->GetItem(0)->SetVisible(false);
		else
			listitem->GetItem(5)->GetItem(0)->SetVisible(show);
		listitem->GetItem(5)->GetItem(1)->SetVisible(show);
		listitem->ReCalcRect();
		listitem->Invalidate();
		//listitem->GetItem(5)->GetItem(2)->SetVisible(false);
	}

}

void TaskListUI::AddListGroupTittle(wstring Id, wstring text)
{//INTERFUN;
	CListUI* taskList=(CListUI*)GetItem(_T("downtasklist"));
	RETURN_IF(taskList->GetItem(Id.c_str()));
	CListContainerItemUI* unfinishitem=new CListContainerItemUI();
	unfinishitem->SetId(Id.c_str());
	CControlUI* unfinishctrl=new CControlUI();
	unfinishctrl->SetInitPos(0,0,0,0,8);
	unfinishctrl->SetStyle(_T("myIconText"));
	unfinishctrl->SetAttribute(_T("icon.pos"),_T("6|0,10,0,9"));
	unfinishctrl->SetAttribute(_T("txt.pos"),_T("8|6,0,5,10"));
	unfinishctrl->SetAttribute(_T("text"),text.c_str());
	unfinishctrl->SetAttribute(_T("txt.font"),_T("font12b"));
	unfinishctrl->SetAttribute(_T("icon.image"),_T("#listsplitline"));
	unfinishitem->Add(unfinishctrl);
	if(Id==UNFinish_GroupId)
		taskList->Insert(unfinishitem,NULL);
	else
		taskList->Add(unfinishitem);
	taskList->CheckItemVisible(false,unfinishitem->GetIndex());
	unfinishitem->SetEnabled(false);
}


tstring TaskListUI::GetButtonText(int taskstate,bool Istip)
{/*INTERFUN*/;
	switch(taskstate)
	{
	case WAITING:
	case PROCEDING:
	case DEALLOCK:
		return Istip?CommonUIFunc::LoadString(L"Task_pause"):_T("#TaskCenter_task_stop");//BaseUI::LoadString(L"Task_pause"):_T("#task_stop");
		break;
	case SUSPENDED:
		{
			return  Istip?CommonUIFunc::LoadString(L"Task_start"):_T("#TaskCenter_task_Go_On");//BaseUI::LoadString(L"Task_start"):_T("#task_Go_On");
		}
		break;
	case FAILED:
		{
			if(Istip)
				return _curdeviceId.empty()?CommonUIFunc::LoadString(L"Task_start"):CommonUIFunc::LoadString(L"Task_retry");//BaseUI::LoadString(L"Task_start"):BaseUI::LoadString(L"Task_retry");
			return _T("#TaskCenter_task_Go_On");
		}
		break;
	case FINISHED:
		{
			if(!_curdeviceId.empty())
				return Istip?CommonUIFunc::LoadString(L"Task_restart"):_T("#TaskCenter_task_Go_On");//BaseUI::LoadString(L"Task_restart"):_T("#task_Go_On");
		}
		break;
	default:
		break;
	}
	return _T("");
}

void TaskListUI::UpdateTaskUI(CTaskItem* item,bool finished)
{/*INTERFUN*/;
	RETURN_IF(!item);
	RETURN_IF(_curdeviceId.empty()&&_listtype==FINISHED_TASK&&item->m_platform!=_finisheddowntaskplat);
	CListUI* taskList=(CListUI*)GetItem(_T("downtasklist"));
	if(taskList)
	{
		CListContainerItemUI* listitem=(CListContainerItemUI*)taskList->GetItem(CStrOperation::IntToWString(item->m_uID).c_str());
		if(listitem&&!finished)
		{
			int  taskType=item->m_uTaskType;
			wstring apppIconath;
			unsigned long filesize=0;
			int TaskPercent=0;
			wstring ProgressText;
			int status=-1;

			if(taskType==URL_DOWNLAOD)
			{
				//RETURN_IF(!TaskCenterModel::GetInstance()->FindTaskItemById(item->m_uID));
				CTaskModuleGetValueInnerMsg msgTask; 
				msgTask._type  = FIND_TASKITEM_BYID;
				msgTask.nParam = item->m_uID;
				msgTask.SendAndGetReturn();
				CTaskItem* item = (CTaskItem*)(msgTask.wParam);
				RETURN_IF(!item);
				bool isLock = false;
				if (FAILED != item->m_uStatus)
				{
					isLock = true;
					item->Lock();
				}
				
				//listitem->GetItem(0)->GetItem(0)->SetAttribute(_T("bk.image"),GetTaskStatusIconId(item->m_uStatus).c_str());
				listitem->GetItem(0)->GetItem(1)->SetText(item->m_strName.c_str());
				filesize=item->m_TaskFileInfo->TotalSize;
				TaskPercent=item->GetTaskPercent();
				ProgressText=GetDownProgressText(item);
				status=item->m_uStatus;
				if (isLock)
				{
					item->Unlock();
				}
				
			}
			else
			{
				apppIconath=item->m_appiconpath;
				filesize=item->m_TaskFileInfo->TotalSize;
				TaskPercent=item->GetTaskPercent();
				ProgressText=GetDownProgressText(item);
				status=item->m_uStatus;
				if(!apppIconath.empty()&&CFileOperation::IsFileExist(apppIconath.c_str()))
				listitem->GetItem(0)->GetItem(0)->SetAttribute(_T("bk.image"),apppIconath.c_str());
			}

			//listitem->GetItem(1)->SetText(item->m_appVersion.c_str());
			listitem->GetItem(2)->SetText(CStrOperation::FormatFileSize((int64_t)(filesize)).c_str());
			listitem->SetSubItemUserdata(2,filesize);
			listitem->GetItem(4)->GetItem(1)->SetText(ProgressText.c_str());
			listitem->GetItem(5)->GetItem(0)->SetAttribute(L"bk.image",GetButtonText(status).c_str());
			listitem->GetItem(5)->GetItem(0)->SetToolTip(GetButtonText(status,true).c_str());
			listitem->GetItem(4)->GetItem(1)->SetAttribute(_T("txt.textcolor"),_T("000000"));
			if(status!=FINISHED && status!=FAILED)
			{
				((CProgressBarUI*)listitem->GetItem(4)->GetItem(0))->SetPos(item->GetTaskPercent());
				listitem->GetItem(4)->GetItem(0)->SetVisible(true);
				//listitem->GetItem(4)->GetItem(1)->SetAttribute(_T("txt.halign"),_T("center"));
				if(!_curdeviceId.empty())
				{
					if(status!=WAITING)
					{//显示文字隐藏按钮
						listitem->GetItem(5)->GetItem(2)->SetVisible(true);
						listitem->GetItem(5)->GetItem(1)->SetVisible(false);
					}
					listitem->GetItem(5)->GetItem(0)->SetVisible(false);
				}
				listitem->ReCalcRect();
				listitem->Invalidate();
			}
			else 
			{
				listitem->GetItem(5)->GetItem(2)->SetVisible(false);
				if(status==FAILED)
				{
					listitem->GetItem(4)->GetItem(1)->SetAttribute(_T("txt.textcolor"),_T("ff0000"));
				}
				listitem->GetItem(4)->GetItem(0)->SetVisible(false);
				listitem->ReCalcRect();
				listitem->Invalidate();
			}

		}
		if(finished)
		{
			if(CONFIG(_bAutoDeleteTask)&&!_curdeviceId.empty())
			{
				CTaskModuleInnerMsg deleteMsg;
				deleteMsg._type = DELETE_TASK;
				deleteMsg.nParam = item->m_uID;
				deleteMsg.strParam = _curdeviceId;
				deleteMsg.nUserData = 1;
				deleteMsg.Send();
				//TaskCenterModel::GetInstance()->DeleteTask(item->m_uID,_curdeviceId);
			}
			if(_listtype!=FINISHED_TASK)
				RemoveTaskItemUI(item->m_uID);
			if(_listtype!=UNFINISHED_TASK
				&&!taskList->GetItem(CStrOperation::IntToWString(item->m_uID).c_str())
				&&(!CONFIG(_bAutoDeleteTask)||_curdeviceId.empty()))
				InsertTask(item);
			taskList->Resize();
			taskList->Invalidate();
		}
	}
}

void TaskListUI::RefreshData()
{/*INTERFUN*/;
	EnterCriticalSection(&_removelistitemlock);
	CListUI* taskList=(CListUI*)GetItem(_T("downtasklist"));
	if(taskList && taskList->GetCount()>0)
	{
		taskList->RemoveAll();
	}
	LeaveCriticalSection(&_removelistitemlock);

	_curSelTaskId=-1;
	_IsSelectedAll=false;
	_lastUnFinishTaskId=-1;
	CTaskModuleGetValueInnerMsg msg;
	msg._type = GET_TASKLIST;
	msg.strParam = _curdeviceId;
	msg.nParam = (int)_listtype;
	msg.SendAndGetReturn();
	const vector<CTaskItem*>* tasks = (const vector<CTaskItem*> *)msg.wParam;//TaskCenterModel::GetInstance()->GetTaskList(_curdeviceId,_listtype);
	for (size_t i=0;i<tasks->size();i++)
	{
		if(_curdeviceId.empty()&&_listtype==FINISHED_TASK)
		{
			CTaskItem* item=(*tasks)[i];
			if(item->m_platform!=_finisheddowntaskplat)
				continue;
		}
		InsertTask((*tasks)[i]);
	}
	taskList->Resize();
	taskList->Invalidate();
	if(_listtype==FINISHED_TASK&&_curdeviceId.empty())
	{
		CTaskModuleInnerMsg msg;
		msg._type = LOAD_ICON;
		msg.Send();
		//TaskCenterModel::GetInstance()->LoadIcons();
	}
		

	CControlUI* con= GetItem(_T("no_tasks_layout"));
	if(taskList->GetCount()<1)
		con->SetVisible(true);
	else
		con->SetVisible(false);
	con->Resize();
	con= GetItem(_T("taskcenter_right_bottom.btn_pause_task"));
	CControlUI* btnstart=GetItem(_T("taskcenter_right_bottom.btn_start_task"));
	CButtonUI* btndel=(CButtonUI*)GetItem(_T("taskcenter_right_bottom.delete.btn_del_task"));
	btndel->SetStyle(_T("ThreeBack_Button"));
	if(!_curdeviceId.empty())
	{
		con->SetVisible(false);
		if(_listtype==UNFINISHED_TASK)
			btnstart->SetVisible(false);
		else
		{
			btnstart->SetText(CommonUIFunc::LoadString(_T("Task_restart")));//BaseUI::LoadString(_T("Task_restart")));
			btnstart->SetAttribute(_T("pos"),_T("0|0,4,68,28"));
		}
	}
	else
	{
		if(_listtype==FINISHED_TASK)
		{
			con->SetVisible(false);
			btnstart->SetVisible(false);
		}
		else
			con->SetVisible(true);
		btnstart->SetText(CommonUIFunc::LoadString(_T("Task_start")));//BaseUI::LoadString(_T("Task_start")));
		if(_listtype==FINISHED_TASK)
			btndel->SetText(CommonUIFunc::LoadString(_T("Task_deleteclear")));//BaseUI::LoadString(_T("Task_deleteclear")));
	}
	CControlUI* btndelright =GetItem(_T("taskcenter_right_bottom.delete.btn_del_task_option"));
	btndelright->SetVisible(false);
	GetItem(_T("taskcenter_right_bottom"))->Resize();
	GetItem(_T("taskcenter_right_bottom"))->Invalidate();
}

bool TaskListUI::OnBottomBtnCk( TNotifyUI* pNotify )
{/*INTERFUN*/;
	wstring id;
	if (pNotify)
		id=pNotify->pSender->GetId();
	else
		id = _keydeleteid;	//快捷键用

	CListUI* taskList=(CListUI*)GetItem(_T("downtasklist"));
	if(taskList)
	{
		vector<int> taskids;
		for (int i=0; i< taskList->GetCount();i++)
		{
			if(taskList->IsCheckItem(i))
			{	
				int taskid=_wtoi(taskList->GetItem(i)->GetId());
				taskids.push_back(taskid);
			}
		}
		if(taskids.empty())
		{
			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("Select_task_first")),ICON_WARNING);
				//NoticeDialog(BaseUI::LoadString(_T("Select_task_first")),ICON_WARNING);
		}
		else if(id==L"btn_del_task")
		{
			CButtonUI* btndel=(CButtonUI*)GetItem(_T("taskcenter_right_bottom.delete.btn_del_task"));
			RETURN_TRUE_IF(!CommonUIFunc::ConfirmDialog(CommonUIFunc::LoadString(_listtype==FINISHED_TASK?L"Sure_To_DeleteFinishedTask":L"Sure_To_DeleteTask"),btndel->GetText()));//BaseUI::LoadString(_listtype==FINISHED_TASK?L"Sure_To_DeleteFinishedTask":L"Sure_To_DeleteTask"),btndel->GetText()));
		}
		else
		{
			CStaticsticsMessage staticcticsMsg("51007");
			staticcticsMsg.Send();
			//Statistic_StatFunc("51007");
		}

		for (size_t i=0;i<taskids.size();i++)
		{
			int taskid=taskids[i];
			if(id==L"btn_pause_task")
			{
				//TaskCenterModel::GetInstance()->StopDown(taskid);
				CTaskModuleInnerMsg stopDownMsg;
				stopDownMsg._type = STOP_DOWN;
				stopDownMsg.nParam = taskid;
				stopDownMsg.Send();

				CTaskModuleGetValueInnerMsg getTaskMsg;
				getTaskMsg._type = FIND_TASKITEM_BYID;
				getTaskMsg.nParam = taskid;
				getTaskMsg.SendAndGetReturn();
				CTaskItem* itemTmp = (CTaskItem*)(getTaskMsg.wParam);

				UpdateTaskUI(itemTmp);//TaskCenterModel::GetInstance()->FindTaskItemById(taskid)
			}
			else if(id== L"btn_start_task")
			{
				//bool result=TaskCenterModel::GetInstance()->StartTask(taskid,_curdeviceId);
				CTaskModuleInnerMsg startMsg;
				startMsg._type = STAR_TASK;
				startMsg.nParam = taskid;
				startMsg.strParam = _curdeviceId;
				startMsg.Send();
			}
			else if(id==L"btn_del_task")
			{
				DeleteTask(taskid);
			}
		}
		taskList->Resize();
		taskList->Invalidate();
	}
	return true;
}

void TaskListUI::DeleteTask(int taskid)
{/*INTERFUN*/
	CTaskModuleGetValueInnerMsg taskMsg;
	taskMsg._type = FIND_TASKITEM_BYID;
	taskMsg.nParam = taskid;
	taskMsg.SendAndGetReturn();	
	CTaskItem* item=(CTaskItem*)(taskMsg.wParam);//TaskCenterModel::GetInstance()->FindTaskItemById(taskid);
	RETURN_IF(!item);
	RETURN_IF(_listtype==UNFINISHED_TASK&&item->m_uStatus==FINISHED);
	bool process=(item->m_uStatus==PROCEDING);
	//TaskCenterModel::GetInstance()->DeleteTask(taskid,_curdeviceId);
	CTaskModuleInnerMsg deleteTaskMsg;
	deleteTaskMsg._type = DELETE_TASK;
	deleteTaskMsg.nParam = taskid;
	deleteTaskMsg.strParam = _curdeviceId;
	deleteTaskMsg.Send();
	if(process&&!_curdeviceId.empty())
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("running_task_cannt_delete")),ICON_WARNING);
		/*NoticeDialog(BaseUI::LoadString(_T("running_task_cannt_delete")),ICON_WARNING);*/
		return;
	}
	CListUI* taskList=(CListUI*)GetItem(_T("downtasklist"));
	CListContainerItemUI* listitem =(CListContainerItemUI*)taskList->GetItem(CStrOperation::IntToWString(taskid).c_str());
	RETURN_IF(!listitem);
	listitem->GetItem(5)->GetItem(2)->SetText(CommonUIFunc::LoadString(_T("Task_Deleting")));//BaseUI::LoadString(_T("Task_Deleting")));
	listitem->GetItem(5)->GetItem(2)->SetVisible(true);
	listitem->GetItem(5)->GetItem(1)->SetVisible(false);
	listitem->GetItem(5)->GetItem(0)->SetVisible(false);
	
}

bool TaskListUI::OnListRightClick( TNotifyUI* pNotify )
{/*INTERFUN*/;
	CListUI* taskList=(CListUI*)GetItem(_T("downtasklist"));
	RETURN_TRUE_IF(taskList->GetCount()<1);
	CListContainerItemUI* listitem =(CListContainerItemUI*)(pNotify->lParam);
	_curSelTaskId=-1;
	if(listitem)
	{
		if(taskList->IsCheckItem(listitem->GetIndex()))
			_curSelTaskId=_wtoi(listitem->GetId());
		else
			listitem->Select(false,false);
	}
	if(IsMultiSelected())
	{
		for(int i=0;i<taskList->GetCount();i++)
		{
			CControlUI* pItem=taskList->GetItem(i);

			if(taskList->IsCheckItem(i))
				pItem->Select(true,false);
		}
	}
	taskList->Invalidate();
	InitMenu();
	return true;
}


bool TaskListUI::IsMultiSelected()
{/*INTERFUN*/;
	CListUI* taskList=(CListUI*)GetItem(_T("downtasklist"));
	int selectcount=0;
	_platform=0;_type=0;
	for (int i=0;i<taskList->GetCount();i++)
	{
		CListContainerItemUI* listitem =(CListContainerItemUI*)(taskList->GetItem(i));
		if(taskList->IsCheckItem(i))
		{
			selectcount++;
			wstring platstr=listitem->GetItem(3)->GetText();
			int taskid=_wtoi(listitem->GetId());
			if(_curSelTaskId==-1)_curSelTaskId=taskid;
			//CTaskItem* item=TaskCenterModel::GetInstance()->FindTaskItemById(taskid);
			CTaskModuleGetValueInnerMsg taskMsg;
			taskMsg._type = FIND_TASKITEM_BYID;
			taskMsg.nParam = taskid;
			taskMsg.SendAndGetReturn();
			CTaskItem* item = (CTaskItem*)(taskMsg.wParam);
			if(item&&item->m_uStatus==FINISHED)
				_type|=1;
			else
				_type|=2;
			if(platstr==L"Android")
				_platform|=1;
			else
				_platform|=2;
			if((_platform&1)&&(_platform&2)&&(_type&1)&&(_type&2))
				break;
		}
	}
	return selectcount>0;
}

void TaskListUI::InitMenu()
{/*INTERFUN*/;
	UINT mainmenuId=MENU_ID_TASK_ALL_DOWN;
	if(!_curdeviceId.empty())
		mainmenuId=MENU_ID_TASK_ALL_INSTALL;
	else if(_listtype==UNFINISHED_TASK||(_listtype==ALL_TASK&&_type==2))
		mainmenuId=MENU_ID_TASK_ALL_DOWN_UNFINISH;
	else if(_listtype==FINISHED_TASK||(_listtype==ALL_TASK&&_type==1))
		mainmenuId=MENU_ID_TASK_ALL_DOWN_FINISHED;

	CMenuUI* pMenu = CMenuUI::Load(mainmenuId);
	if(!pMenu)
		return;
	CMenuUI* itemInstall=pMenu->GetItem(MENU_ID_TASK_INSTALL,BY_COMMAND);
	CMenuUI* itemCase=pMenu->GetItem(MENU_ID_TASK_ADD,BY_COMMAND);
	_pMenu=itemInstall;
	InitInstallSubMenu(itemInstall);

	InitInstallCaseSubMenu(itemCase);
	if(_curSelTaskId<0)
	{
		for (int i=0;i<pMenu->GetCount();i++)
		{
			UINT itemmenuid=pMenu->GetItem(i)->GetUID();
			if(itemmenuid==MENU_ID_TASK_DELETE_ALL
				||itemmenuid==MENU_ID_TASK_SELECT_ALL
				||itemmenuid==MENU_ID_TASK_SELECT_NONE)continue;
			pMenu->GetItem(i)->EnableItem(false);
		}
	}
	else 
	{
		//CTaskItem* taskitem=TaskCenterModel::GetInstance()->FindTaskItemById(_curSelTaskId);
		CTaskModuleGetValueInnerMsg taskMsg;
		taskMsg._type = FIND_TASKITEM_BYID;
		taskMsg.nParam = _curSelTaskId;
		taskMsg.SendAndGetReturn();
		CTaskItem* taskitem = (CTaskItem*)(taskMsg.wParam);
		if(taskitem)
		{
			if(taskitem->m_uStatus!=FINISHED)
			{
				if(itemInstall)itemInstall->EnableItem(false);
				if(itemCase)itemCase->EnableItem(false);
			}
			CMenuUI* itemCase=pMenu->GetItem(MENU_ID_TASK_START,BY_COMMAND);
			if(itemCase&&!_curdeviceId.empty()&&taskitem->m_uStatus!=FAILED&&(taskitem->m_uStatus!=FINISHED||taskitem->m_uTaskType==MOVEANDROIDAPP))
				itemCase->EnableItem(false);
		}
	}
	pMenu->Show(GetWindow()->GetHWND(), NULL, this);
}

void TaskListUI::InitInstallSubMenu(CMenuUI* item)
{/*INTERFUN*/;
	RETURN_IF(!item);
	vector<wstring> deviceids=DEVICE_MANAGER->GetAllDevice();
	for (size_t i=0; i<deviceids.size();i++)
	{
		DeviceData* device=DEVICE_MANAGER->FindDevice(deviceids[i]);

		if(!device
			||(_platform==1&&device->GetDevInfo()->_deviceType==Ios)
			||(_platform==2&&device->GetDevInfo()->_deviceType==Android))
			continue;
		
		if(device->GetDevInfo()->_deviceType==Ios&&device->GetDevInfo()->_connectType != USB)
			continue;
		CMenuUI* menuitem=new CMenuUI();
		menuitem->SetUID(MENU_ID_INSTALL_DEVICE_START+i);
		menuitem->SetId(device->GetId().c_str());
		menuitem->SetText(device->GetDevInfo()->GetDeviceName().c_str());
		wstring imagepath=device->GetDevInfo()->_deviceType==Ios?L"#TaskCenter_icon_Apple":L"#TaskCenter_icon_Android";
		menuitem->SetAttribute(_T("image"),imagepath.c_str());
		menuitem->SetAttribute(_T("imagesize"),_T("16"));
		LOG->WriteDebug(L"InitInstallSubMenu Insert sn:"+deviceids[i]);
		item->Insert(menuitem);
	}
	if(item->GetCount()<1)
		item->EnableItem(false);
}

void TaskListUI::InitInstallCaseSubMenu(CMenuUI* item)
{/*INTERFUN*/;
	RETURN_IF(!item);
	CMenuUI* newcasemenu=new CMenuUI();
	newcasemenu->SetUID(MENU_ID_TASK_CREATE_NEWCASE);
	newcasemenu->SetText(CommonUIFunc::LoadString(L"CreateNewCase"));//(BaseUI::LoadStringW(L"CreateNewCase"));
	item->Insert(newcasemenu);

	GetSortByUseTimeCaseListMsg getCaseListMsg;
	getCaseListMsg.SendAndGetReturn();
	vector<InstallsCaseData*>* caselist = (vector<InstallsCaseData*>*)getCaseListMsg.pDataList;//MainDlg::GetInstance()->GetInstallCaseUI()->Model.GetCaseDataList();
	if (NULL == caselist)
	{
		return;
	}
	int size = caselist->size();
	for (int i=0; i<size;i++)
	{
		if(caselist->at(i)->PlanType() != _INSTALL_PLAN_TYPE_USER)
			continue;
		int casetype=caselist->at(i)->CaseType();
		if((_platform==1&&casetype==_INSTALLS_CASE_DATA_IOS)
			||(_platform==2&&casetype==_INSTALLS_CASE_DATA_ANDROID))
			continue;
		CMenuUI* menuitem=new CMenuUI();
		menuitem->SetUID(MENU_ID_INSTALL_CASE_START+caselist->at(i)->Id());
		menuitem->SetText(caselist->at(i)->CaseName().c_str());
		wstring imagepath=caselist->at(i)->CaseType()==_INSTALLS_CASE_DATA_IOS?L"#TaskCenter_icon_Apple":L"#TaskCenter_icon_Android";
		menuitem->SetAttribute(_T("image"),imagepath.c_str());
		menuitem->SetAttribute(_T("imagesize"),_T("15"));
		menuitem->SetAttribute(_T("txt.endellipsis"),_T("1"));
		item->Insert(menuitem);
	}
}

bool TaskListUI::OnMenuClick( TNotifyUI* pNotify )
{/*INTERFUN*/;
	CMenuUI* mainmenu=(CMenuUI*)(pNotify->pSender);
	UIMENUINFO* pInfo = (UIMENUINFO*)(pNotify->wParam);
	if(pInfo)
	{
		CListUI* taskList=(CListUI*)GetItem(_T("downtasklist"));
		switch(pInfo->uID)
		{
		case MENU_ID_TASK_INSTALL://安装到
		case MENU_ID_TASK_ADD://添加到
		case MENU_ID_TASK_ALL_BOTTOM_INSTALL:
		case MENU_ID_TASK_ALL_BOTTOM_ADD:
			break;
		case MENU_ID_TASK_SELECT_ALL: //全选
			{
				taskList->SelectItemAll();
				if(taskList->GetItem(Finished_GroupId))
					taskList->GetItem(Finished_GroupId)->Select(false);
				if(taskList->GetItem(UNFinish_GroupId))
					taskList->GetItem(UNFinish_GroupId)->Select(false);
				taskList->CheckItem(true);
				_IsSelectedAll = true;
			}
			break;
		case MENU_ID_TASK_SELECT_NONE: //不选	
			{
				taskList->SelectItem(NULL);
				taskList->CheckItem(false);
				_IsSelectedAll = false;
			}
			break;
		default:
			{
				int uid=pInfo->uID;
				if(pInfo->uID<70000)
					uid=pInfo->uID/MENU_ID_INSTALL_DEVICE_START;
				wstring deviceid=L"";
				if(uid==1)
				{
					CMenuUI* devicemenu= _pMenu->GetItem(pInfo->uID,BY_COMMAND);
					if(devicemenu)
						deviceid=devicemenu->GetId();
				}
				//wstring deviceid=uid==1?mainmenu->GetItem(pInfo->uID%MENU_ID_INSTALL_DEVICE_START)->GetId():L"";
				int caseid=uid==2?pInfo->uID%MENU_ID_INSTALL_CASE_START:-1;
				vector<int> taskids;
				for (int i=0; i< taskList->GetCount();i++)
				{
					if(pInfo->uID==MENU_ID_TASK_DELETE_ALL||taskList->IsCheckItem(i))
					{
						int taskid=_wtoi(taskList->GetItem(i)->GetId());
						taskids.push_back(taskid);
					}
				}
				if(!taskids.empty()&&(uid==MENU_ID_TASK_DELETE_ALL||uid==MENU_ID_TASK_DELETE))
				{
					wstring notetext = CommonUIFunc::LoadString(_listtype==FINISHED_TASK?L"Sure_To_DeleteFinishedTask":L"Sure_To_DeleteTask");//BaseUI::LoadString(_listtype==FINISHED_TASK?L"Sure_To_DeleteFinishedTask":L"Sure_To_DeleteTask");
					if(MENU_ID_TASK_DELETE_ALL==uid)notetext=CStrOperation::ReplaceW(notetext,L"选中的",L"当前全部");
					CButtonUI* btndel=(CButtonUI*)GetItem(_T("taskcenter_right_bottom.delete.btn_del_task"));
					RETURN_TRUE_IF(!CommonUIFunc::ConfirmDialog(notetext,btndel->GetText()));
				}
				vector<wstring> paths;
				for(size_t i=0;i<taskids.size();i++)
				{
					TaskOperation(uid,taskids[i],deviceid,caseid);
					if(uid==2||uid==MENU_ID_TASK_CREATE_NEWCASE)
					{
						//CTaskItem* item=TaskCenterModel::GetInstance()->FindTaskItemById(taskids[i]);
						CTaskModuleGetValueInnerMsg getTaskMsg;
						getTaskMsg._type = FIND_TASKITEM_BYID;
						getTaskMsg.nParam = taskids[i];
						getTaskMsg.SendAndGetReturn();
						CTaskItem* item = (CTaskItem* )(getTaskMsg.wParam);
						if(item)
						{
							InstallCaseInfoMsg installCaseInfoMsg;
							installCaseInfoMsg.SendAndGetReturn();
							if(uid==2&&installCaseInfoMsg.bIsExist)//MainDlg::GetInstance()->GetInstallCaseUI()->Model.GetCaseById(caseid))
							{
								bool isios=installCaseInfoMsg.nCaseType == 1;//MainDlg::GetInstance()->GetInstallCaseUI()->Model.GetCaseById(caseid)->CaseType()==_INSTALLS_CASE_DATA_IOS;
								wstring ext=CFileOperation::GetFileExt(item->m_TaskFileInfo->savePath);
								if((ext==L"ipa"||ext==L"pxl")==isios)
								{
									paths.push_back(item->m_TaskFileInfo->savePath);
								}
								continue;
							}
							paths.push_back(item->m_TaskFileInfo->savePath);
						}
					}
				}
				if(!paths.empty())
				{
					if(uid==2)
					{
						AddAppToCaseByPathMsg addAppToCaseByPathMsg;
						addAppToCaseByPathMsg.nCaseId = caseid;
						addAppToCaseByPathMsg.pPathes = &paths;
						addAppToCaseByPathMsg.SendToUI();
						//MainDlg::GetInstance()->GetInstallCaseUI()->Model.AddAppToCaseByPath(caseid, &paths);
					}
					if(uid==MENU_ID_TASK_CREATE_NEWCASE)
					{
						AddCaseMsg addCaseMsg;
						addCaseMsg.pPathList = (void*)(&paths);
						addCaseMsg.Send();
						//MainDlg::GetInstance()->GetInstallCaseUI()->Model.AddCase(&paths);
					}
						
				}
			}
			break;
		}
	}
	_curSelTaskId=-1;
	return true;
}

bool TaskListUI::TaskOperation(int eventcode, int taskid, wstring deviceid, int caseId)
{/*INTERFUN*/;
	switch(eventcode)
	{
	case 1:
		{
			//TaskCenterModel::GetInstance()->InstallApp(taskid,deviceid);
			CTaskModuleInnerMsg installTaskMsg;
			installTaskMsg._type = TASK_INSTALLAPP;
			installTaskMsg.nParam = taskid;
			installTaskMsg.strParam = deviceid;
			installTaskMsg.Send();
			break;
		}
	case 2:
		{
		}
		break;
	case MENU_ID_TASK_OPEN_DIR:
		{
			CTaskModuleGetValueInnerMsg findTaskMsg;
			findTaskMsg._type = FIND_TASKITEM_BYID;
			findTaskMsg.nParam = taskid;
			findTaskMsg.SendAndGetReturn();

			CTaskItem* item=(CTaskItem*)(findTaskMsg.wParam);//TaskCenterModel::GetInstance()->FindTaskItemById(taskid);
			RETURN_TRUE_IF(!item||item->m_uStatus!=FINISHED||!CFileOperation::IsFileExist(item->m_TaskFileInfo->savePath.c_str()));
			BaseOperation::FindTarget(item->m_TaskFileInfo->savePath.c_str());
		}
		break;
	case MENU_ID_TASK_DELETE_ALL: 
	case MENU_ID_TASK_DELETE://删除
		{
			DeleteTask(taskid);
			return true;
		}
		break;
	case MENU_ID_TASK_PAUSE: //暂停
		{
			//TaskCenterModel::GetInstance()->StopDown(taskid);
			CTaskModuleInnerMsg stopDownTaskMsg;
			stopDownTaskMsg._type = STOP_DOWN;
			stopDownTaskMsg.nParam = taskid;
			stopDownTaskMsg.Send();

			CTaskModuleGetValueInnerMsg findTaskMsg;
			findTaskMsg._type = FIND_TASKITEM_BYID;
			findTaskMsg.nParam = taskid;
			findTaskMsg.SendAndGetReturn();

			CTaskItem* item=(CTaskItem*)(findTaskMsg.wParam);
			UpdateTaskUI(item);//TaskCenterModel::GetInstance()->FindTaskItemById(taskid));
		}
		break;
	case MENU_ID_TASK_START: //开始
		{
			CTaskModuleInnerMsg startMsg;
			startMsg._type = STAR_TASK;
			startMsg.nParam = taskid;
			startMsg.strParam = _curdeviceId;
			startMsg.Send();

			//TaskCenterModel::GetInstance()->StartTask(taskid,_curdeviceId);
		}
		break;
	}
	return true;
}

bool TaskListUI::OnMenuAutoDelClick( TNotifyUI* pNotify )
{/*INTERFUN*/;
	RETURN_TRUE_IF (pNotify->wParam == NULL);
	UIMENUINFO* pInfo = (UIMENUINFO*)pNotify->wParam;
	if(pInfo->uID==81101)
	{
		_IsAutoDelFinishedTask=!CONFIG(_bAutoDeleteTask);
		CONFIG_HANDLE->SetAutoDeleteTask(_IsAutoDelFinishedTask);
		if(_IsAutoDelFinishedTask)
		{
			CStaticsticsMessage staticcticsMsg(Statistic_ID_InstallAnAppDel);
			staticcticsMsg.Send();
			//Statistic_StatFunc(Statistic_ID_InstallAnAppDel);
		}
			
		if(_IsAutoDelFinishedTask)
		{
			CTaskModuleInnerMsg msg;
			msg._type = DELETE_ALLFINISHEDINSTALLTASK;
			msg.Send();
		}
			//TaskCenterModel::GetInstance()->DeleteAllFinishedInstallTask();
	}
	return true;
}

void TaskListUI::RemoveTaskItemUI( int taskid )
{/*INTERFUN*/;
	EnterCriticalSection(&_removelistitemlock);
	CListUI* taskList=(CListUI*)GetItem(_T("downtasklist"));
	CListContainerItemUI* item= (CListContainerItemUI*)taskList->GetItem(CStrOperation::IntToWString(taskid).c_str());
	if(!item){
		LeaveCriticalSection(&_removelistitemlock);
		return;
	}

	taskList->Remove(item);
	int finisheindex=-1;
	if(taskList->GetItem(Finished_GroupId))
		finisheindex=taskList->GetItem(Finished_GroupId)->GetIndex();
	if(finisheindex==1||(_listtype==ALL_TASK&&taskList->GetCount()<2))
		taskList->Remove(taskList->GetItem(0));
	else if(finisheindex>-1&&finisheindex==taskList->GetCount()-1)
		taskList->Remove(taskList->GetItem(Finished_GroupId));
	taskList->Resize();
	taskList->Invalidate();
	if(taskList->GetCount()<1)
	{
		taskList->CheckItem(false);
		CControlUI* con= GetItem(_T("no_tasks_layout"));
		con->SetVisible(true);
		con->Resize();
		con->Invalidate();
	}
	LeaveCriticalSection(&_removelistitemlock);
}

bool TaskListUI::OnBottomBtnPause( TNotifyUI* pNotify )
{/*INTERFUN*/;
	return OnBottomBtnCk(pNotify);
}

bool TaskListUI::OnBottomBtnStart( TNotifyUI* pNotify )
{/*INTERFUN*/;
	//Statistic_StatFunc(_curdeviceId.empty()?Statistic_ID_DownloadBatBegin:Statistic_ID_InstallBatchRetry);
	CStaticsticsMessage staticcticsMsg(_curdeviceId.empty()?Statistic_ID_DownloadBatBegin:Statistic_ID_InstallBatchRetry);
	staticcticsMsg.Send();
	return OnBottomBtnCk(pNotify );
}

bool TaskListUI::OnBottomBtnDel( TNotifyUI* pNotify )
{/*INTERFUN*/;
	//Statistic_StatFunc(_curdeviceId.empty()?Statistic_ID_DownloadBatDelete:Statistic_ID_InstallBatchDel);
	CStaticsticsMessage staticcticsMsg(_curdeviceId.empty()?Statistic_ID_DownloadBatDelete:Statistic_ID_InstallBatchDel);
	staticcticsMsg.Send();
	CListUI* taskList=(CListUI*)GetItem(_T("downtasklist"));
	//RETURN_TRUE_IF(!taskList||taskList->GetCount()<1);
	if (!taskList || taskList->GetCount()<1)
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("Select_task_first")),ICON_WARNING);
		return true;
	}
	return OnBottomBtnCk(pNotify );
}

bool TaskListUI::OnBottomBtnDelOption( TNotifyUI* pNotify )
{/*INTERFUN*/;
	RETURN_TRUE_IF(_curdeviceId.empty());
    CMenuUI* pMenu = CMenuUI::Load(8000);
    pMenu->GetItem(0)->Check(CONFIG(_bAutoDeleteTask));
   // ShowMenu(pMenu, GetWindow()->GetHWND(), this, pNotify->pSender);
	CommonUIFunc::ShowMenu(pMenu, GetWindow()->GetHWND(), this, pNotify->pSender);
	return true;
}

bool TaskListUI::OnItemBtnStart( TNotifyUI* pNotify )
{//INTERFUN;
	wstring text= pNotify->pSender->GetText();
	wstring id=pNotify->pSender->GetParent()->GetParent()->GetId();
	CTaskModuleGetValueInnerMsg getTaskItemMsg;
	getTaskItemMsg._type = FIND_TASKITEM_BYID;
	getTaskItemMsg.nParam = _wtoi(id.c_str());
	getTaskItemMsg.SendAndGetReturn();

	CTaskItem* item=(CTaskItem*)(getTaskItemMsg.wParam);//TaskCenterModel::GetInstance()->FindTaskItemById(_wtoi(id.c_str()));
	RETURN_TRUE_IF(!item);
	TASK_STATUS state=item->m_uStatus;
	if(state==PROCEDING||state==WAITING||state==DEALLOCK)
	{
		CTaskModuleInnerMsg stopDownMsg;
		stopDownMsg._type = STOP_DOWN;
		stopDownMsg.nParam = _wtoi(id.c_str());
		stopDownMsg.Send();
		//TaskCenterModel::GetInstance()->StopDown(_wtoi(id.c_str()));
		UpdateTaskUI(item);
		//Statistic_StatFunc(Statistic_ID_DownloadStopBnt);
		CStaticsticsMessage staticcticsMsg(Statistic_ID_DownloadStopBnt);
		staticcticsMsg.Send();
	}
	else if(state==SUSPENDED||state==FAILED||state==FINISHED)
	{
		CTaskModuleInnerMsg startTaskMsg;
		startTaskMsg._type = STAR_TASK;
		startTaskMsg.nParam = _wtoi(id.c_str());
		startTaskMsg.strParam = _curdeviceId;
		startTaskMsg.Send();
		//TaskCenterModel::GetInstance()->StartTask(_wtoi(id.c_str()),_curdeviceId);
		pNotify->pSender->SetAttribute(L"bk.image",GetButtonText(PROCEDING).c_str());
		pNotify->pSender->SetToolTip(GetButtonText(PROCEDING,true).c_str());
		//Statistic_StatFunc(_curdeviceId.empty()?Statistic_ID_DownloadBeginBnt:Statistic_ID_InstallAnAppRetry);
		CStaticsticsMessage staticcticsMsg(_curdeviceId.empty()?Statistic_ID_DownloadBeginBnt:Statistic_ID_InstallAnAppRetry);
		staticcticsMsg.Send();
	}
	return true;
}

bool TaskListUI::OnItemBtnDel( TNotifyUI* pNotify )
{//INTERFUN;
	RETURN_TRUE_IF(!pNotify||!pNotify->pSender);
	tstring id=pNotify->pSender->GetParent()->GetParent()->GetId();
	RETURN_TRUE_IF(!CommonUIFunc::ConfirmDialog(CommonUIFunc::LoadString(_listtype==FINISHED_TASK?L"Sure_To_DeleteFinishedTask":L"Sure_To_DeleteTask"),pNotify->pSender->GetToolTip()));
	int taskid=_wtoi(id.c_str());
	DeleteTask(taskid);
	//Statistic_StatFunc(_curdeviceId.empty()?Statistic_ID_DownloadDeleteBnt:Statistic_ID_InstallAnAppDel);

	CStaticsticsMessage staticcticsMsg(_curdeviceId.empty()?Statistic_ID_DownloadDeleteBnt:Statistic_ID_InstallAnAppDel);
	staticcticsMsg.Send();
	return true;

}

bool TaskListUI::OnListClick( TNotifyUI* pNotify )
{//INTERFUN;
	return true;
}

bool TaskListUI::OnlistChecked( TNotifyUI* pNotify )
{//INTERFUN;
	return true;
}

bool TaskListUI::OnBottomInstall(TNotifyUI* pNotify)
{//INTERFUN;
	CMenuUI* pMenu = CMenuUI::Load(24000);
	_pMenu=pMenu;
	IsMultiSelected();
	if(_platform==0&&_type==0)
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("Select_finished_file_first")),ICON_WARNING);
		//NoticeDialog(BaseUI::LoadString(_T("Select_finished_file_first")),ICON_WARNING);
		return true;
	}
	InitInstallSubMenu(pMenu);
	if(pMenu->GetCount()<1)
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("No_fitable_device")),ICON_WARNING);
		//NoticeDialog(BaseUI::LoadStringW(L"No_fitable_device"),ICON_WARNING);
		return true;
	}
	pMenu->ShowEx(GetWindow()->GetHWND(), NULL, this,CMenuUI::UIMENU_TOPALIGN);
	return true;
}

bool TaskListUI::OnBottomAddToCase(TNotifyUI* pNotify)
{//INTERFUN;
	CMenuUI* pMenu = CMenuUI::Load(25000);
	IsMultiSelected();
	if(_platform==0)
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("Select_finished_file_first")),ICON_WARNING);
		//NoticeDialog(BaseUI::LoadString(_T("Select_finished_file_first")),ICON_WARNING);
		return true;
	}
	InitInstallCaseSubMenu(pMenu);
	if(pMenu->GetCount()<1)
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("No_fitable_install_case")),ICON_WARNING);
		//NoticeDialog(BaseUI::LoadStringW(L"No_fitable_install_case"),ICON_WARNING);
		return true;
	}
	pMenu->ShowEx(GetWindow()->GetHWND(), NULL, this,CMenuUI::UIMENU_TOPALIGN);
	return true;
}

bool TaskListUI::OnNoTaskClick(TNotifyUI* pNotify)
{/*INTERFUN*/;
	/*if(_curdeviceId.empty()&&_listtype==UNFINISHED_TASK)
		MainDlg::GetInstance()->SwitchTo(WEBSOURCE_CLASS_ID);*/
	return true;
}

//void TaskListUI::Receive( MESSAGE_TYPE msg, const BaseParam* pParm )
//{INTERFUN;
//	RETURN_IF(!ListFocused());
//	_keydeleteid = _T("btn_del_task");
//	switch(pParm->GetInt())
//	{
//	case VK_DELETE:
//	case VK_DECIMAL:
//		OnBottomBtnCk(NULL);
//		break;
//	}
//}

bool TaskListUI::ListFocused()
{/*INTERFUN*/;
	CListUI* taskList=(CListUI*)GetItem(_T("downtasklist"));
	if (!taskList)
		return false;

	CControlUI* pControl = this->GetWindow()->GetFocus();
	while ( pControl )
	{
		if ( pControl == taskList )
			return true;
		pControl = pControl->GetParent();
	}
	return false;
}

void TaskListUI::UpdateTaskImage( CTaskItem* item )
{/*INTERFUN*/;
	RETURN_IF(!_curdeviceId.empty()||!item||item->m_uTaskType!=URL_DOWNLAOD||item->m_uStatus!=FINISHED);
	RETURN_IF(item->m_platform!=_finisheddowntaskplat);
	CListUI* taskList=(CListUI*)GetItem(_T("downtasklist"));
	CListContainerItemUI* listitem= (CListContainerItemUI*)taskList->GetItem(CStrOperation::IntToWString(item->m_uID).c_str());
	RETURN_IF(!listitem);
	listitem->GetItem(0)->GetItem(0)->SetAttribute(_T("bk.image"),item->m_appiconpath.c_str());
	listitem->GetItem(0)->GetItem(0)->SetAttribute(_T("pos"),_T("9|5,-1,30,30"));
	listitem->GetItem(0)->GetItem(1)->SetAttribute(_T("pos"),_T("8|38,0,5,0"));
	if(item->m_isLegalSoft)
		listitem->GetItem(0)->GetItem(2)->SetVisible(true);
	listitem->ReCalcRect();
	listitem->Invalidate();
}

bool TaskListUI::OnPlatClick( TNotifyUI* pNotify )
{
	wstring btnId=pNotify->pSender->GetId();
	if(btnId==L"btn_Android")
	{
		_finisheddowntaskplat=L"Android";
	}
	else if(btnId==L"btn_iPhone")
	{
		_finisheddowntaskplat=L"iPhone";
	}
	else if(btnId==L"btn_iPad")
	{
		_finisheddowntaskplat=L"iPad";
	}
	RefreshData();
	return true;
}

bool TaskListUI::Handle(const DelTaskItemMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}
	CTaskModuleGetValueInnerMsg msg;
	msg._type  = FIND_TASKITEMID_BYURL;
	msg.strParam = pMsg->_strUrl;
	msg.SendAndGetReturn();
	int nTaskId = msg.nParam;//TaskCenterModel::GetInstance()->GetTaskIdByUrl(pMsg->_strUrl);
	this->DeleteTask(nTaskId);

	return true;
}