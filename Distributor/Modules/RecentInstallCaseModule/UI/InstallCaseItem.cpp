#include "StdAfx.h"
#include "InstallCaseItem.h"
#include "ui/InstallCaseTipWindow.h"
#include "ui/CaseListInfoUI.h"
#include "WebCpaView.h"

InstallCaseItem::InstallCaseItem(InstallsCaseData* pCaseData,wstring deviceId )
:_pCaseData(pCaseData),
_deviceId(deviceId)
{
	_bIsNetCase=((dynamic_cast<NetInstallsCaseData*>(_pCaseData))!=NULL);

	_pMainLayout = NULL;

	_pCaseImageCtrl = NULL;

	_pBtnClose =NULL;

	_pNetCaseFlag = NULL;

	_pLegalflag = NULL;

	_pCpaflag   = NULL;

	_pTotalSizeCtrl = NULL;

	_pProgBar = NULL;

	_pBtnStatus = NULL;

	_pCaseNameCtrl = NULL;

	_pBtnHover = NULL;

	mNetInstallPlanTaskManager=NULL;

	_totalInstallApp = 0;

	_curInstallpercent = 0;

	_lastpercent = 0;


	if(_bIsNetCase)
	{
		mNetInstallPlanTaskManager = new NetInstallPlanTaskManager(deviceId, static_cast<NetInstallsCaseData*>(_pCaseData));
		mNetInstallPlanTaskManager->SetObersever(this);
	}
	if(!_deviceId.empty()&&!_bIsNetCase)
	{
		GetInstallPlanIdMsg msg;
		msg.deviceId = _deviceId;
		msg.caseId = CStrOperation::IntToWString(_pCaseData->Id());
		msg.SendAndGetReturn();
		_InstallTaskIds = msg.PlanTasks;

		_totalInstallApp = _InstallTaskIds.size();
	}
	_bExit = false;
}

InstallCaseItem::~InstallCaseItem(void)
{
	SAFE_DELETE(mNetInstallPlanTaskManager);
	_bExit = true;
}

void InstallCaseItem::OnCreate()
{
	_pMainLayout=dynamic_cast<CLayoutUI*>(GetItem(_T("InstallCaseItemLayout")));

	_pCaseImageCtrl=dynamic_cast<CLayoutUI*>(GetItem(_T("InstallCaseItemLayout.itemImageLayout")));

	_pBtnClose=dynamic_cast<CButtonUI*>(GetItem(_T("closebtn")));

	_pNetCaseFlag=GetItem(_T("InstallCaseItemLayout.netcaseflag"));

	_pTotalSizeCtrl=dynamic_cast<CLabelUI*>(GetItem(_T("InstallCaseItemLayout.totalSize")));

	_pProgBar=dynamic_cast<CProgressBarUI*>(GetItem(_T("InstallCaseItemLayout.downProgress")));

	_pBtnStatus=dynamic_cast<CButtonUI*>(GetItem(_T("InstallCaseItemLayout.CaseStatusbtn")));

	_pCaseNameCtrl=dynamic_cast<CLabelUI*>(GetItem(_T("InstallCaseItemLayout.casename")));

	_pLegalflag=GetItem(_T("Legalflag"));

	_pCpaflag  = GetItem(_T("Cpaflag"));

	_pBtnHover=dynamic_cast<CButtonUI*>(GetItem(_T("BtnHover")));
	

	if(_pBtnHover)
		_pBtnHover->AddEventDelegate(&MakeDelegate(this, &InstallCaseItem::OnEventHover));

	InitCaseImage();

	SetCaseTotalSize();

	UpdateCaseName();

	if(_pBtnStatus)
	{
		_pBtnStatus->SetText(L"安装");
		_pBtnStatus->SetAttribute(L"icon.image",L"#RecentInstallCase_Image_case_install_btn");
		_pBtnStatus->SetAttribute(L"icon.pos",L"5|28,8,15,8");
	}

	if(_pLegalflag)
		_pLegalflag->SetVisible(_pCaseData->IsLegalPlan()==1);

	if (_pCpaflag)
	{
		NetInstallsCaseData* pNetData = dynamic_cast<NetInstallsCaseData*>(_pCaseData);
		if (pNetData)
			_pCpaflag->SetVisible(pNetData->IsbCpa());
	}
		
	if(_pProgBar)
		_pProgBar->SetVisible(false);
	if(_pTotalSizeCtrl)
		_pTotalSizeCtrl->SetVisible(true);

	if(_pBtnStatus)
	{
		_pBtnStatus->SetEnabled(true);
		if(!_bIsNetCase&&_deviceId.empty())
			_pBtnStatus->SetEnabled(false);
	}

	if(_pBtnStatus&&_bIsNetCase)
	{
		_pBtnStatus->SetText(_deviceId.empty()?L"下载":L"安装");
		_pNetCaseFlag->SetVisible(true);

		_pBtnStatus->SetAttribute(L"icon.image",_deviceId.empty()?L"#RecentInstallCase_Image_down_case":L"#RecentInstallCase_Image_case_install_btn");
		_pBtnStatus->SetAttribute(L"icon.pos",_deviceId.empty()?L"5|28,6,15,7":L"5|28,8,15,8");
	}
}

void InstallCaseItem::UpdateCaseName()
{
	RETURN_IF(!_pCaseNameCtrl||!_pCaseData);

	_pCaseNameCtrl->SetText(_pCaseData->CaseName().c_str());
}


void InstallCaseItem::UpdateNetCaseStatus()
{

	RETURN_IF(!_pCaseData||!_bIsNetCase);

	NetInstallsCaseData* pNetCase = dynamic_cast<NetInstallsCaseData*>(_pCaseData);
	switch(pNetCase->NetInstallStatus)
	{
	case _NET_INSTALL_PLAN_STATUS_INIT:
		{
			if(_pBtnStatus)
			{
				_pBtnStatus->SetText(_deviceId.empty()?L"下载":L"安装");
				_pBtnStatus->SetAttribute(L"icon.image",_deviceId.empty()?L"#RecentInstallCase_Image_down_case":L"#RecentInstallCase_Image_case_install_btn");
				_pBtnStatus->SetAttribute(L"icon.pos",_deviceId.empty()?L"5|28,6,15,7":L"5|28,8,15,8");
			}
			if(_pProgBar)
				_pProgBar->SetVisible(false);
			if(_pTotalSizeCtrl)
				_pTotalSizeCtrl->SetVisible(true);
			if(_pNetCaseFlag)
				_pNetCaseFlag->SetVisible(true);

		}
		break;
	case _NET_INSTALL_PLAN_STATUS_DOWNLOADING:
		{
			if(pNetCase->NetAppItems->size()==0)
			{
				mNetInstallPlanTaskManager->PlanStatus(_NET_INSTALL_PLAN_STATUS_DOWNLOADED);
				return UpdateNetCaseStatus();
			}
			if(_pBtnStatus)
			{
				_pBtnStatus->SetText(L"下载中");
				_pBtnStatus->SetEnabled(false);
				_pBtnStatus->SetAttribute(L"icon.image",L"#RecentInstallCase_Image_down_case");
				_pBtnStatus->SetAttribute(L"icon.pos",L"5|28,6,15,7");
			}
			if(_pProgBar)
			{
				_pProgBar->SetVisible(true);
				_pProgBar->SetPos((int64_t)mNetInstallPlanTaskManager->m_curpercent);
				_pProgBar->Resize();
				_pProgBar->Invalidate();
			}
			if(_pTotalSizeCtrl)
				_pTotalSizeCtrl->SetVisible(false);
			if(_pNetCaseFlag)
				_pNetCaseFlag->SetVisible(true);
		}
		break;
	case _NET_INSTALL_PLAN_STATUS_DOWNLOADED:
		{
			if(_pBtnStatus)
			{
				_pBtnStatus->SetText(_deviceId.empty()?(pNetCase->NetAppItems->size()>0?L"下载":L"已下载"):L"安装");
				_pBtnStatus->SetEnabled(!_deviceId.empty()||pNetCase->NetAppItems->size()>0);
				_pBtnStatus->SetAttribute(L"icon.image",_deviceId.empty()?L"#RecentInstallCase_Image_down_case":L"#RecentInstallCase_Image_case_install_btn");
				_pBtnStatus->SetAttribute(L"icon.pos",_deviceId.empty()?L"5|28,6,15,7":L"5|28,8,15,8");
			}
			if(_pProgBar)
				_pProgBar->SetVisible(false);
			if(_pTotalSizeCtrl)
				_pTotalSizeCtrl->SetVisible(true);

			//zhengjx20120301 为了不破坏原有设计的逻辑，这里做暂时能想到的最简单调整（只要有下载失败||安装失败||下载被删除||安装被删除且完成百分百的情况，均设置状态为“安装”状态）
			//是否需要重构后续可能需要考虑
			if(!_deviceId.empty() && mNetInstallPlanTaskManager->Status()==_INSTALL_PLAN_STATUS_Running 
				&& 100==(int64_t)mNetInstallPlanTaskManager->m_curpercent
				&& (mNetInstallPlanTaskManager->mDownloadFailTasks.size()>0
				|| mNetInstallPlanTaskManager->mInstallFailPlanTasks.size()>0
				|| mNetInstallPlanTaskManager->mDownloadDelTasks.size()>0
				|| mNetInstallPlanTaskManager->mInstallDelPlanTasks.size()>0))
			{
				
			}
			else
			{
				if(!_deviceId.empty()&&mNetInstallPlanTaskManager->Status()==_INSTALL_PLAN_STATUS_Running)
				{
					_pBtnStatus->SetText(L"安装中");
					_pBtnStatus->SetEnabled(false);
					if(_pProgBar)
					{
						_pProgBar->SetVisible(true);
						_pProgBar->SetPos((int64_t)mNetInstallPlanTaskManager->m_curpercent);
						//_pProgBar->Resize();
						//_pProgBar->Invalidate();
					}
					if(_pTotalSizeCtrl)
						_pTotalSizeCtrl->SetVisible(false);
				}
				else if(!_deviceId.empty()&&mNetInstallPlanTaskManager->Status()==_INSTALL_PLAN_STATUS_Completed)
				{
					_pBtnStatus->SetText(L"已安装");
				}
			}

			if(_pNetCaseFlag&&pNetCase->NetAppItems->size()==0)
				_pNetCaseFlag->SetVisible(false);
			this->Resize();
			this->Invalidate();
		}
		break;
	default:
		break;
	}
}

void InstallCaseItem::SetCaseTotalSize()
{
	if(_pTotalSizeCtrl&&_pCaseData)
	{
		_pTotalSizeCtrl->SetText(CStrOperation::FormatFileSize((int64_t)_pCaseData->GetSumFileSize()).c_str());
	}
}

void InstallCaseItem::InitCaseImage()
{
	RETURN_IF(!_pCaseData);
	vector<wstring> caseimages;
	NetInstallsCaseData* pNetCase = NULL;
	InstallsAppList * applist = new InstallsAppList();//_pCaseData->ListApp();

	InstallsAppList::iterator iterTmp=_pCaseData->ListApp()->begin();

	while(iterTmp!=_pCaseData->ListApp()->end()&&applist->m_list.size()<4)
	{
		applist->m_list.push_back(*iterTmp);
		iterTmp++;
	}

	if(_bIsNetCase)
	{
		pNetCase = dynamic_cast<NetInstallsCaseData*>(_pCaseData);
		RETURN_IF(!pNetCase);
		if(CFileOperation::IsFileExist(pNetCase->_wsPackegeIconpath.c_str()))
		{
			caseimages.push_back(pNetCase->_wsPackegeIconpath);
		}
		else
		{
			if(!applist||applist->size()<4&&pNetCase)
			{
				for (InstallsAppList::iterator it = pNetCase->NetAppItems->begin(); it != pNetCase->NetAppItems->end(); it++)
				{
					InstallsAppInfo* appInfo = *it;
					applist->m_list.push_back(appInfo); 
				}
			}
		}
	}
	if(NULL==pNetCase||(NULL!=pNetCase&&!CFileOperation::IsFileExist(pNetCase->_wsPackegeIconpath.c_str())))
	{
		RETURN_IF(!applist);

		InstallsAppList::iterator iter=applist->begin();

		while(iter!=applist->end()&&caseimages.size()<4)
		{
			InstallsAppInfo* info=(*iter);
			if(info&&!info->m_Iconpath.empty()&&CFileOperation::IsFileExist(info->m_Iconpath.c_str()))
				caseimages.push_back(info->m_Iconpath);
			iter++;
		}
	}

	if(_pCaseImageCtrl)
	{
		_pCaseImageCtrl->RemoveAll();
		if(!caseimages.empty()&&caseimages.size()<2)
		{
			CControlUI* pImageCtrl=new CControlUI();
			pImageCtrl->SetInitPos(0,0,0,0,8);
			pImageCtrl->SetStyle(L"Picture");
			pImageCtrl->SetAttribute(L"bk.image",caseimages[0].c_str());
			//pImageCtrl->SetAttribute(L"bk.paintmode",L"center");
			
			_pCaseImageCtrl->Add(pImageCtrl);
		}
		if(caseimages.size()>1)
		{
			for (size_t i=0;i<caseimages.size();i++)
			{
				CControlUI* pImageCtrl=new CControlUI();
				pImageCtrl->SetInitPos(0,0,25,25,0);
				pImageCtrl->SetStyle(L"Picture");
				pImageCtrl->SetAttribute(L"bk.image",caseimages[i].c_str());
				_pCaseImageCtrl->Add(pImageCtrl);
			}
		}
		_pCaseImageCtrl->Resize();
		_pCaseImageCtrl->Invalidate();
	}

	applist->m_list.clear();
	SAFE_DELETE(applist);
}

void InstallCaseItem::LockCase(bool bLock)
{
	if(_bIsNetCase)
		return;
	if(_pBtnClose)
		_pBtnClose->SetVisible(!bLock);
}

bool InstallCaseItem::OnColseBtnClick( TNotifyUI* pNotify )
{
	vector<int> ids;ids.push_back(_pCaseData->Id());
	RemoveCaseByCaseIdMsg RemoveMsg;
	RemoveMsg.idList = &ids;
	RemoveMsg.Send();

	return true;
}

bool InstallCaseItem::OnBtnClick( TNotifyUI* pNotify )
{
	if(NULL==_pCaseData)
		return false;

	wstring devID = _deviceId;


	GetInstallPathByCaseIdMsg PathMsg;
	PathMsg.caseId = _pCaseData->Id();
	PathMsg.SendAndGetReturn();
	int appnumber = PathMsg.nListSize;
	if( appnumber == 0)
	{
		CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"RecentInstallCase_Text_NoAppInCase"),ICON_WARNING);
		return true;
	}

	vector<wstring> pathList = PathMsg.InstallpathList;

	DeviceData *pDevData =	DEVICE_MANAGER->FindDevice(_deviceId);
	if(pDevData)
	{		
		//by chenxy 27389修改此BUG【任务中心】手机和助手已经失去通信，已经读取不到SD卡和手机内存了，而且刷新应用程序了，
		//再安装本地应用仍然提示安装成功，实际手机中安装成功。添加pDevData->IsActivate()判断
		if (pDevData->IsActivate())
		{
			if(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_NET)
			{
				// 统计功能

				CStaticsticsMessage StaticMsg(Statistic_ID_InstallNetPlan);
				StaticMsg.Send();

				mNetInstallPlanTaskManager->btnInstall = (CButtonUI* )pNotify->pSender;
				mNetInstallPlanTaskManager->Run(true);
				CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"RecentInstallCase_Text_HasbeenAddToTaskList"),ICON_SUCCESS);
			}
			else
			{
				// 统计功能

				CStaticsticsMessage StaticMsg(Statistic_ID_InstallPlan);
				StaticMsg.Send();

				SetCaseSizeMsg CaseSizeMsg;
				CaseSizeMsg.casename = CStrOperation::IntToWString(_pCaseData->Id());
				CaseSizeMsg.deviceId = devID;
				CaseSizeMsg.count    = appnumber;
				CaseSizeMsg.Send();

				InstallAppByCaseIdMsg InstallMsg;
				InstallMsg.sn = devID;
				InstallMsg.caseId = _pCaseData->Id();
				InstallMsg.pathList = &pathList;
				InstallMsg.Send();

				_InstallTaskIds.clear();
				_installtaskpercents.clear();
				_totalInstallApp=appnumber;
				_curInstallpercent = 0;
				_lastpercent=0;
				LocalInstalling();
			}
		}
		else
		{
			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"RecentInstallCase_Text_DeviceConnectedFail"),ICON_WARNING);
		}
	}
	else
	{
		if(_pCaseData->PlanType() == _INSTALL_PLAN_TYPE_NET)
		{

			for (InstallsAppList::iterator it = mNetInstallPlanTaskManager->mNetInstallPlanItem->NetAppItems->begin(); 
				it != mNetInstallPlanTaskManager->mNetInstallPlanItem->NetAppItems->end(); ++it)
			{
				(*it)->m_bSelected=true;
			}

			mNetInstallPlanTaskManager->Run();
			if(CONFIG(_bDownLoadMsg))
				CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"RecentInstallCase_Text_HasbeenAddToDownLoadTaskList"),ICON_SUCCESS);
		}
		else
			CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"RecentInstallCase_Text_makeSureDevicehaslinkPls"),CommonUIFunc::LoadString(L"RecentInstallCase_Text_OneKeyInstall"));
	}
	return true;
}

void InstallCaseItem::RefreshCase()
{
	RETURN_IF(!_pCaseData);
	InitCaseImage();

	SetCaseTotalSize();

	UpdateCaseName();

	if(_pLegalflag)
		_pLegalflag->SetVisible(_pCaseData->IsLegalPlan()==1);

	if (_pCpaflag)
	{
		NetInstallsCaseData* pNetData = dynamic_cast<NetInstallsCaseData*>(_pCaseData);
		if (pNetData)
			_pCpaflag->SetVisible(pNetData->IsbCpa());
	}
}


//鼠标HOVER事件
bool InstallCaseItem::OnEventHover( TEventUI& event )
{INTERFUN;

	if (_bExit)
	{
		return true;
	}

	switch(event.Type)
	{
	case UIEVENT_MOUSEENTER:
		{
			int num=0;
			InstallCaseTipWindow::GetInstance()->Show(Singleton::GetMainWnd(), 
														_pBtnHover->GetRect(),
														_pCaseData->CaseName(),
														CStrOperation::FormatFileSize((int64_t)_pCaseData->GetSumFileSize()),
														_pCaseData->GetAppNum(),
														UIStringFormat::InstallNeedTime(_pCaseData->GetAppNum(),_pCaseData->GetSumFileSize(),num));
		}
		break;
	case UIEVENT_MOUSELEAVE:
		InstallCaseTipWindow::GetInstance()->Hide();
		break;
	}
	return true;
}

bool InstallCaseItem::OnBtnShowDetailClick( TNotifyUI* pNotify )
{
	NetInstallsCaseData* pNetData = dynamic_cast<NetInstallsCaseData*>(_pCaseData);
	if (pNetData&&pNetData->IsbCpa())
	{
		CWebCpaView* infoDlg = new CWebCpaView(_deviceId, CCodeOperation::GB2312ToUnicode(pNetData->GetWebCpaUrl()));
		infoDlg->Create(Singleton::GetMainWnd(), _T("WebCpa_View"));
		infoDlg->CenterWindow(Singleton::GetMainWnd());
		int ret=infoDlg->DoModal();
		SAFE_DELETE(infoDlg);
		return true;
	}

	CaseListInfoUI* infoDlg = new CaseListInfoUI(_pCaseData->Id(), _deviceId, _pBtnStatus,mNetInstallPlanTaskManager);
	infoDlg->Create(Singleton::GetMainWnd(),_T("CaseInfoList"));
	infoDlg->CenterWindow(Singleton::GetMainWnd());
	int ret=infoDlg->DoModal();
	delete infoDlg;
	infoDlg = NULL;
	if(ret>=100)
	{
		if(DEVICE_MANAGER->FindDevice(_deviceId))
		{
			_InstallTaskIds.clear();
			_installtaskpercents.clear();
			_totalInstallApp=ret-100;
			_curInstallpercent =0;
			_lastpercent=0;
			LocalInstalling();
		}
		else
		{
			CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"RecentInstallCase_Text_makeSureDevicehaslinkPls"),CommonUIFunc::LoadString(L"RecentInstallCase_Text_OneKeyInstall"));
		}
	}
	return true;
}

void InstallCaseItem::InstallFinished()
{
	if(_pBtnStatus)
	{
		_pBtnStatus->SetEnabled(true);
		_pBtnStatus->SetText(CommonUIFunc::LoadString(L"RecentInstallCase_Text_Device_CaseInstalled"));
	}
	if(_pProgBar)
		_pProgBar->SetVisible(false);
	if(_pTotalSizeCtrl)
		_pTotalSizeCtrl->SetVisible(true);
}

void InstallCaseItem::ResetInstall()
{
	if(_pBtnStatus)
	{
		_pBtnStatus->SetEnabled(true);
		_pBtnStatus->SetText(CommonUIFunc::LoadString(L"安装"));
	}
	if(_pProgBar)
		_pProgBar->SetVisible(false);
	if(_pTotalSizeCtrl)
		_pTotalSizeCtrl->SetVisible(true);
}

void InstallCaseItem::NetInstallCaseStatusUpdated()
{
	UpdateNetCaseStatus();
}

void InstallCaseItem::TaskFinishedNotify( CTaskItem* item, wstring deviceId )
{
	RETURN_IF(_bIsNetCase||deviceId.empty()||deviceId!=_deviceId);
	RETURN_IF(item->m_casename.empty()||item->m_casename!=CStrOperation::IntToWString(_pCaseData->Id()));
	_installtaskpercents[item->m_uID]=100;
	UpdateLocalInstallCaseProgress();
}

bool InstallCaseItem::Handle(const NotifyTaskItemMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}
	CTaskItem* item = (CTaskItem*)pMsg->pTask;

	switch(pMsg->message)
	{
	case WM_TASK_FINISHED:
		{
			if (NULL == item)
			{
				return false;
			}
			this->TaskFinishedNotify(item, pMsg->curID);
		}
		break;
	case WM_TASK_UPDATESPEED:
		{
			if (NULL == item)
			{
				return false;
			}
			this->TaskUpdatedSpeedNotify(item, pMsg->curID);
		}
		break;
	case WM_TASK_FAIL:
		{
			if (NULL == item)
			{
				return false;
			}
			this->TaskFailNotify(item, pMsg->curID);
		}
		break;
	case WM_TASK_REMOVED:
		{
			this->TaskRemoveNotify(pMsg->taskid, pMsg->curID);
		}
		break;
	default:
		break;
	}

	return true;

}

void InstallCaseItem::TaskFailNotify( CTaskItem* item, wstring deviceId )
{
	RETURN_IF(_bIsNetCase||deviceId.empty()||deviceId!=_deviceId);
	RETURN_IF(item->m_casename.empty()||item->m_casename!=CStrOperation::IntToWString(_pCaseData->Id()));
	_installtaskpercents[item->m_uID]=100;
	UpdateLocalInstallCaseProgress();
}

void InstallCaseItem::TaskRemoveNotify( int taskid, wstring deviceId )
{
	RETURN_IF(_bIsNetCase||deviceId.empty()||deviceId!=_deviceId);

	_installtaskpercents[taskid]=100;
	UpdateLocalInstallCaseProgress();

}

void InstallCaseItem::TaskUpdatedSpeedNotify( CTaskItem* item ,wstring deviceId )
{
	RETURN_IF(_bIsNetCase||deviceId.empty()||deviceId!=_deviceId);
	RETURN_IF(item->m_casename.empty()||item->m_casename!=CStrOperation::IntToWString(_pCaseData->Id()));
	_installtaskpercents[item->m_uID]=item->m_nProcess;
	UpdateLocalInstallCaseProgress();
}


void InstallCaseItem::LocalInstalling()
{
	if(_pBtnStatus)
	{
		_pBtnStatus->SetEnabled(false);
		bool isUpdate = false;
		wstring text = _pBtnStatus->GetText();
		if ( text != L"安装中")
		{
			isUpdate = true;
		}
		_pBtnStatus->SetText(L"安装中");
		if (isUpdate)
		{
			_pBtnStatus->Resize();
			_pBtnStatus->Invalidate();
		}
		
	}
	if(_pProgBar)
	{
		_pProgBar->SetVisible(true);
		_pProgBar->SetPos(_curInstallpercent,true);
	}
	if(_pTotalSizeCtrl)
		_pTotalSizeCtrl->SetVisible(false);
}

void InstallCaseItem::UpdateLocalInstallCaseProgress()
{
	RETURN_IF(_totalInstallApp<1);
	if(_InstallTaskIds.empty())
	{
		GetInstallPlanIdMsg msg;
		msg.deviceId = _deviceId;
		msg.caseId = CStrOperation::IntToWString(_pCaseData->Id());
		msg.SendAndGetReturn();
		_InstallTaskIds = msg.PlanTasks;

	}
	_curInstallpercent=(_totalInstallApp-_InstallTaskIds.size())*100/_totalInstallApp;
	size_t finishedcnt=0;
	for (map<int,int>::iterator it=_installtaskpercents.begin();it!=_installtaskpercents.end();it++)
	{
		_curInstallpercent+=it->second/_totalInstallApp;
		if(it->second==100)
			finishedcnt++;
	}
	bool finished=(finishedcnt>=_InstallTaskIds.size());
	bool bAllfinished = false;
	InstallsAppList* pAppList = _pCaseData->ListApp();
	if (NULL!=pAppList)
	{
		bAllfinished=(finishedcnt>=pAppList->size());
	}
	
	if(finished)
	{
		if (bAllfinished)
		{
			InstallFinished();
		}
		else
		{
			ResetInstall();
		}

		_totalInstallApp=0;
		_InstallTaskIds.clear();
		_installtaskpercents.clear();
		_lastpercent =0;
		_curInstallpercent=0;
	}
	else 
	{
		if(_curInstallpercent>=(_lastpercent+1))
		{
			_lastpercent = _curInstallpercent;
			LocalInstalling();
		}
	}
}