#include "stdafx.h"
#include "NetInstallPlanTaskManager.h"
#include "Common/DataPath.h"
#include "Logic/InstallsCase/InstallCaseManagerEx.h"

INSTALLCASE_BEGIN
#define CONTINUE_IF(x)            if ( x ) { continue; }

NetInstallPlanTaskManager::NetInstallPlanTaskManager()
{INTERFUN;
	
}

NetInstallPlanTaskManager::NetInstallPlanTaskManager(wstring deviceId, NetInstallsCaseData* installPlanItem)
{INTERFUN;

	mDeviceId = deviceId;
	mDeviceInfo = DEVICE_MANAGER->FindDevice(mDeviceId);
	mNetInstallPlanItem = installPlanItem;

	mTotalDownStep = installPlanItem->NetAppItems->size();
	mTotalInstallStep = mTotalDownStep + installPlanItem->ListApp()->size();

	mCurrentInstallStep = 0;

	mStatus = _INSTALL_PLAN_STATUS_Waiting;

	if(mTotalDownStep>0)
		m_curpercent = installPlanItem->ListApp()->size()*100/mTotalInstallStep;
	else
		m_curpercent=0;

	lastpercent=m_curpercent;

	_pObersever=NULL;

	_skipinstallcnt = 0;

	//获得安装计划任务大小

	GetInstallPlanIdMsg msg;
	msg.caseId = CStrOperation::IntToWString(mNetInstallPlanItem->Id());
	msg.SendAndGetReturn();
	mInstallPlanTasks = msg.PlanTasks;

	if(mInstallPlanTasks.size()>0)
	{
		mNetInstallPlanItem->NetInstallStatus=_NET_INSTALL_PLAN_STATUS_DOWNLOADED;
		_skipinstallcnt = mTotalInstallStep-mInstallPlanTasks.size();
		m_curpercent = 0;
		lastpercent = 0;
		Status(_INSTALL_PLAN_STATUS_Running);
	}

}

NetInstallPlanTaskManager::~NetInstallPlanTaskManager()
{INTERFUN;

}

/// <summary>
/// 开始
/// </summary>
void NetInstallPlanTaskManager::Run( bool bNeed )
{INTERFUN;
	this->mTotalInstallStep = 0;
	for(InstallsAppList::iterator it = mNetInstallPlanItem->ListApp()->begin();it != mNetInstallPlanItem->ListApp()->end();it++)
	{
		InstallsAppInfo *appinfo = *it;
		if (appinfo->m_bSelected || bNeed)
		{
			this->mTotalInstallStep++;
		}

	}
	this->mTotalDownStep = 0;
	InstallsAppList::iterator netItemTmp = mNetInstallPlanItem->NetAppItems->begin();
	while(netItemTmp != mNetInstallPlanItem->NetAppItems->end())
	{
		InstallsAppInfo *soft = *netItemTmp;
		if (soft->m_bSelected || bNeed)
		{
			this->mTotalDownStep++;
			this->mTotalInstallStep++;
		}
		netItemTmp++;
	}
	RunInternal(bNeed);
}

/// <summary>
/// 开始安装
/// </summary>
void NetInstallPlanTaskManager::RunInstall( bool bNeed )
{INTERFUN;
	int appNumber = 0;
	for(InstallsAppList::iterator it = mNetInstallPlanItem->ListApp()->begin();it != mNetInstallPlanItem->ListApp()->end();it++)
	{
		InstallsAppInfo *appinfo = *it;
		if (appinfo->m_bSelected || bNeed)
		{
			appNumber++;
		}
		
	}

	if(appNumber == 0)
		return;
	m_curpercent=0;
	mInstallPercents.clear();
	mCurrentInstallStep = 0;
	mNetInstallPlanItem->NetInstallStatus=_NET_INSTALL_PLAN_STATUS_DOWNLOADED;
	Status(_INSTALL_PLAN_STATUS_Running);

	Statistic_StatFunc( "52012" );

	SetCaseSizeMsg CaseSizeMsg;
	CaseSizeMsg.casename = CStrOperation::IntToWString(mNetInstallPlanItem->Id());
	CaseSizeMsg.deviceId = mDeviceId;
	CaseSizeMsg.count    = appNumber;

	for(InstallsAppList::iterator it = mNetInstallPlanItem->ListApp()->begin();it != mNetInstallPlanItem->ListApp()->end();it++)
	{
		InstallsAppInfo *appinfo = *it;
		if (appinfo->m_bSelected || bNeed)
		{
			if(!CFileOperation::IsFileExist(appinfo->m_strSavePath.c_str()))
			{
				if (mTotalInstallStep>0)
				{
					m_curpercent+=100/mTotalInstallStep;
				}
				continue;
			}
			wstring strInstall = MyFileOperation::GetIniFileValue(CONFIG(_strOptionIni), _T("Android"), _T("installonphone"));
			bool bOnSDCard = (strInstall == L"0" ? true : false);
			int taskId;

			InstallTaskItemMsg Installmsg;
			Installmsg.appname = appinfo->m_strName;
			Installmsg.appfilepath = appinfo->m_strSavePath;
			Installmsg.version = appinfo->m_strVersion;
			Installmsg.filesize = appinfo->m_FileSize;
			Installmsg.deviceId = mDeviceId;
			Installmsg.onSDcard = bOnSDCard;
			Installmsg.plat = L"";
			Installmsg.fid  = 0;
			Installmsg.casename = CStrOperation::IntToWString(mNetInstallPlanItem->Id());
			Installmsg.icon = appinfo->m_Iconpath;
			Installmsg.isLegalSoft = appinfo->m_isLegalSoft==1;
			Installmsg.SendAndGetReturn();
			taskId = Installmsg.taskid;

			if(taskId > 0)
				mInstallPlanTasks.push_back(taskId);
		}
		
	}
	lastpercent = m_curpercent;

	if(mInstallPlanTasks.size() > 0)
	{
		mNetInstallPlanItem->AddinstallCount();

		time_t now;
		time(&now);
		mNetInstallPlanItem->lastUseTime = now;
	}
}

// 安装下载任务
void NetInstallPlanTaskManager::InstallDownloadedSoft(InstallsAppInfo* appinfo)
{INTERFUN;
	wstring strInstall = MyFileOperation::GetIniFileValue(CONFIG(_strOptionIni), _T("Android"), _T("installonphone"));
	bool bOnSDCard = (strInstall == L"0" ? true : false);
	int taskId;

	InstallTaskItemMsg Installmsg;
	Installmsg.appname = appinfo->m_strName;
	Installmsg.appfilepath = appinfo->m_strSavePath;
	Installmsg.version = appinfo->m_strVersion;
	Installmsg.filesize = appinfo->m_FileSize;
	Installmsg.deviceId = mDeviceId;
	Installmsg.onSDcard = bOnSDCard;
	Installmsg.plat = L"";
	Installmsg.fid  = 0;
	Installmsg.casename = CStrOperation::IntToWString(mNetInstallPlanItem->Id());
	Installmsg.icon = appinfo->m_Iconpath;
	Installmsg.isLegalSoft = appinfo->m_isLegalSoft==1;
	Installmsg.SendAndGetReturn();
	taskId = Installmsg.taskid;
	
	if(taskId>0&&!(FindTaskId(mInstallPlanTasks,taskId)))
		mInstallPlanTasks.push_back(taskId);
}

// 任务完成
void NetInstallPlanTaskManager::TaskFinishedNotify(CTaskItem* item, wstring deviceId)
{INTERFUN;
	if(item == NULL)
		return;

	TaskNotify(item,deviceId,item->m_uTaskType,FINISHED,item->m_uID);
}

// 任务失败
void NetInstallPlanTaskManager::TaskFailNotify(CTaskItem* item, wstring deviceId)
{INTERFUN;
	if(item == NULL)
		return;

	TaskNotify(item,deviceId,item->m_uTaskType,FAILED,item->m_uID);
}

// 任务删除
void NetInstallPlanTaskManager::TaskRemoveNotify(CTaskItem* item, wstring deviceId)
{INTERFUN;
	if(item == NULL)
		return;

	TaskNotify(item,deviceId,item->m_uTaskType,DEALLOCK,item->m_uID);
}

void NetInstallPlanTaskManager::TaskRemoveNotify(int taskid, wstring deviceId)
{INTERFUN;
	TaskNotify(NULL, deviceId, deviceId.empty()?URL_DOWNLAOD:INSTALL, DEALLOCK, taskid);
}

void NetInstallPlanTaskManager::TaskNotify(CTaskItem* item, wstring deviceId , TASK_KIND taskKind , TASK_STATUS taskStatus, UINT nTaskId)
{INTERFUN;
	// 下载
	if(taskKind==URL_DOWNLAOD)
	{
		// 是否存在本方案中
		RETURN_IF(!FindTaskId(mDownloadTasks,nTaskId));
		switch (taskStatus)
		{
		case FAILED:
			{
				if (!FindTaskId(mDownloadFailTasks,nTaskId))
				{
					mDownloadFailTasks.push_back(nTaskId);
				}
				EreaseTaskId(mDownloadComletedTasks,nTaskId);
				EreaseTaskId(mDownloadDelTasks,nTaskId);
				CurrentDownStep(mDownloadComletedTasks.size() + mDownloadDelTasks.size() + mDownloadFailTasks.size());
			}
			break;
		case FINISHED:
			{
				if (!FindTaskId(mDownloadComletedTasks,nTaskId))
				{
					mDownloadComletedTasks.push_back(nTaskId);
					if(PlanStatus()==_NET_INSTALL_PLAN_STATUS_DOWNLOADED&&_skipinstallcnt>0)
						_skipinstallcnt--;
					InstallsAppInfo* appinfo = JoinAppItemsCollection(item);
					if(mDeviceInfo && appinfo)
					{
						InstallDownloadedSoft(appinfo);
					}
				}
				EreaseTaskId(mDownloadDelTasks,nTaskId);
				EreaseTaskId(mDownloadFailTasks,nTaskId);
				mDownloadPercents[nTaskId]=0;
				CurrentDownStep(mDownloadComletedTasks.size() + mDownloadDelTasks.size() + mDownloadFailTasks.size());
			}
			break;
		default:
			{
				if (!FindTaskId(mDownloadDelTasks,nTaskId))
				{
					mDownloadDelTasks.push_back(nTaskId);
				}
				EreaseTaskId(mDownloadComletedTasks,nTaskId);
				EreaseTaskId(mDownloadFailTasks,nTaskId);
				mDownloadPercents[nTaskId]=0;
				CurrentDownStep(mDownloadComletedTasks.size() + mDownloadDelTasks.size() + mDownloadFailTasks.size());
			}
			break;
		}
	}
	// 安装
	else if(taskKind==INSTALL)
	{
		RETURN_IF(mDeviceId != deviceId);
		RETURN_IF(!FindTaskId(mInstallPlanTasks,nTaskId));
		EreaseTaskId(mInstallPlanTasks,nTaskId);
		mInstallPercents[nTaskId]=0;
		switch (taskStatus)
		{
		case FAILED:
			{
				if (!FindTaskId(mInstallFailPlanTasks,nTaskId))
				{
					mInstallFailPlanTasks.push_back(nTaskId);
				}
				EreaseTaskId(mInstallComletedPlanTasks,nTaskId);
				EreaseTaskId(mInstallDelPlanTasks,nTaskId);
				CurrentInstallStep(mInstallComletedPlanTasks.size() + mInstallDelPlanTasks.size() + mInstallFailPlanTasks.size());
			}
			break;
		case FINISHED:
			{
				// 是否在完成集合
				if (!FindTaskId(mInstallComletedPlanTasks,nTaskId))
				{
					mInstallComletedPlanTasks.push_back(nTaskId);
				}
				// 是否在失败集合
				EreaseTaskId(mInstallFailPlanTasks,nTaskId);
				// 是否在删除集合
				EreaseTaskId(mInstallDelPlanTasks,nTaskId);
				CurrentInstallStep(mInstallComletedPlanTasks.size() + mInstallDelPlanTasks.size() + mInstallFailPlanTasks.size());
			}
			break;
		default:
			{
				if (!FindTaskId(mInstallDelPlanTasks,nTaskId))
				{
					mInstallDelPlanTasks.push_back(nTaskId);
				}
				EreaseTaskId(mInstallComletedPlanTasks,nTaskId);
				EreaseTaskId(mInstallFailPlanTasks,nTaskId);
				CurrentInstallStep(mInstallComletedPlanTasks.size() + mInstallDelPlanTasks.size() + mInstallFailPlanTasks.size());
			}
			break;
		}
	}
}

bool NetInstallPlanTaskManager::FindTaskId(vector<int>& tasVec, int taskId)
{
	bool isExist = false;
	for ( vector<int>::iterator no = tasVec.begin(); no != tasVec.end(); ++no ) 
	{
		int cpTaskId = *no;
		if( cpTaskId == taskId)
		{
			isExist = true;
			break;
		}
	}
	return isExist;
}

void NetInstallPlanTaskManager::EreaseTaskId(vector<int>& tasVec, int taskId)
{
	for ( vector<int>::iterator no = tasVec.begin(); no != tasVec.end(); ++no ) 
	{
		int deTaskId = *no;
		if( deTaskId == taskId)
		{
			tasVec.erase(no);
			break;
		}
	}
}

/// <summary>
/// 将下载完的安装包按路径添加到 AppItems 集合，同时移除NetAppItem中的相应数据
/// </summary>
/// <param name="task"></param>
InstallsAppInfo* NetInstallPlanTaskManager::JoinAppItemsCollection(CTaskItem* task)
{INTERFUN;
	CString strSql;
	UINT planId = mNetInstallPlanItem->Id() - NET_CASE_START_INDEX;

	wstring savePath = task->m_TaskFileInfo->savePath;
	wstring downUrl = task->m_csFileDownUrl;
	if(downUrl.empty())
	{
		downUrl = task->m_TaskFileInfo->sStrMes;
	}
	InstallsAppInfo* appItem=NULL;
	// 向AppItems添加安装包
	bool canAdd = true;
	for(InstallsAppList::iterator no = mNetInstallPlanItem->ListApp()->begin();no != mNetInstallPlanItem->ListApp()->end();no++)
	{
		InstallsAppInfo* appinfo = *no;
		if (savePath == appinfo->m_strSavePath)
		{
			appItem = appinfo;
			canAdd = false;
			break;
		}
	}
	if (canAdd)
	{
		appItem = InstallCaseManagerEx::GetInstance()->AnalyzeApp(savePath);
		if (appItem)
		{
			mNetInstallPlanItem->AddAppToList(appItem);
			time_t InstallTime;
			time(&InstallTime);
			wstring phoneCategory = L"Android";
			if (appItem->m_strFileType == L"ipa" || appItem->m_strFileType == L"deb" ||appItem->m_strFileType == L"pxl")
			{
				phoneCategory = L"iOS";
			}
			appItem->m_fid=0;
			CppSQLite3Query q_Ret;
			strSql.Format(L"select * from NetInstallPlanSofts where planid='%d' and DownloadUrl='%s';" , planId , downUrl.c_str());
			if(CDBManager::DB_QUERY_DATA(strSql,q_Ret))
			{
				while(!q_Ret.Eof())
				{
					appItem->m_fid=q_Ret.GetIntField(1);
					q_Ret.NextRow();
				}
			}	
			strSql.Format(L"Insert into InstallPlanFile(Plan_ID,Name,Path,Size,Version,Format,SupportDevice,AppName,InstallTime,\
						   LastWriteTime,IsLegalSoft,Buyer,PhoneCategory,VersionCode,FBundleVersion,SDKVersion,FID,ItemID)\
						   values('%d','%s','%s','%d','%s','%s','%s','%s','%d','%d','%d','%s','%s','%s','%d','%s','%d','%d');",
						   planId,
						   appItem->m_strName.c_str(),
						   appItem->m_strSavePath.c_str(),
						   appItem->m_FileSize,
						   appItem->m_strVersion.c_str(),
						   appItem->m_strFileType.c_str(),
						   appItem->m_SupportDevice.c_str(),
						   appItem->m_strPackageID.c_str(),
						   1,
						   (long)InstallTime,
						   appItem->m_isLegalSoft,
						   appItem->m_Purchaser.c_str(),
						   phoneCategory.c_str(),
						   appItem->m_strVersion.c_str(),
						   appItem->m_VerCode.c_str(),
						   appItem->m_SDKVersion.c_str(),
						   appItem->m_fid,
						   appItem->m_itemID);
			CDBManager::DoExecSql(strSql.GetBuffer());
			appItem->m_id = CDBManager::m_db.LastRowId();
		}
	}

	// 向NetAppItems移除下载包
	for(InstallsAppList::iterator no = mNetInstallPlanItem->NetAppItems->begin();no != mNetInstallPlanItem->NetAppItems->end();no++)
	{
		InstallsAppInfo* item = *no;
		wstring itemurl=item->m_strSavePath;
		int legalindex = itemurl.find(L"itunesapp2");
		if(legalindex>0)
		{
			string content=CNetOperation::Get(CCodeOperation::UnicodeToGB2312(itemurl),false);
			int pos= content.find("href=");
			CONTINUE_IF(pos<0);
			content =content.substr(pos+6);
			int pos1=content.find("\"");
			CONTINUE_IF(pos1<0);
			content =content.substr(0,pos1);
			itemurl = CCodeOperation::GB2312ToUnicode(content);
		}
		if (CStrOperation::startsWith(downUrl,itemurl))
		{
			strSql.Format(L"delete from NetInstallPlanSofts where planid='%d' and DownloadUrl='%s';" , planId , item->m_strSavePath.c_str());
			CDBManager::DoExecSql(strSql.GetBuffer());
			mNetInstallPlanItem->NetAppItems->erase(no);
			break;
		}
	}
	return appItem;
}


void NetInstallPlanTaskManager::StatusChanged()
{INTERFUN;
	return;

	CButtonUI* btn= btnInstall;

	switch (Status())
	{
	case _INSTALL_PLAN_STATUS_Downloading:
		{
			btn->SetEnabled(false);
			btn->SetText(L"下载中");
		}
		break;
	case _INSTALL_PLAN_STATUS_Completed:
		{
			btn->SetEnabled(true);
			btn->SetText(L"已安装");
		}
		break;
	case _INSTALL_PLAN_STATUS_Stopped:
		{
			btn->SetEnabled(true);
			btn->SetText(L"安装");
		}
		break;
	case _INSTALL_PLAN_STATUS_Running:
		{
			btn->SetEnabled(false);
			btn->SetText(L"安装中");
		}
		break;
	}
	//btn->Resize();
	//btn->Invalidate();
}

void NetInstallPlanTaskManager::RunInternal(bool bNeed)
{
	_skipinstallcnt = 0;

	mDownloadPercents.clear();
	mDownloadComletedTasks.clear();
	mDownloadFailTasks.clear();
	mDownloadDelTasks.clear();
	mDownloadTasks.clear();


	mInstallComletedPlanTasks.clear();
	mInstallFailPlanTasks.clear();
	mInstallDelPlanTasks.clear();

	mInstallPlanTasks.clear();

	mCurrentDownStep = 0;

	mCurrentInstallStep = 0;

	Status(_INSTALL_PLAN_STATUS_Running);

	//根据数据文件是否被删除调整未下载和下载的LIST
	NetInstallsCaseDB::RefreshNetPlanCaseData(this->mNetInstallPlanItem);

	// 已下载的 执行安装
	RunInstall(bNeed);

	// 未下载 执行下载安装
	
	if (mNetInstallPlanItem->NetInstallStatus != _NET_INSTALL_PLAN_STATUS_DOWNLOADED || mNetInstallPlanItem->NetAppItems->size() > 0)
	{
		if (this->mTotalInstallStep == 0)//mNetInstallPlanItem->ListApp()->size() 
		{
			Status(_INSTALL_PLAN_STATUS_Downloading);
		}
		if ( this->mTotalDownStep > 0)//mNetInstallPlanItem->NetAppItems->size()
		{
			PlanStatus(_NET_INSTALL_PLAN_STATUS_DOWNLOADING);
		}
		if (mTotalInstallStep==0)
		{
			m_curpercent = 100;
		}
		else
		{
			if (mTotalInstallStep>0)
			{
				m_curpercent=mNetInstallPlanItem->ListApp()->size()*100/mTotalInstallStep;
			}
		}
		
		lastpercent=m_curpercent;

		Statistic_StatFunc( "51010" );

		wstring strPlat = L"Android";
		if(mNetInstallPlanItem->CaseType() != _INSTALLS_CASE_DATA_ANDROID)
			strPlat = L"iPhone";
		wstring strPath = L"";
		if (strPlat == L"Android")
			strPath = CONFIG(_strAndroidDownPath);
		else
			strPath = CONFIG(_strIosDownPath);

		if (mNetInstallPlanItem->IsbCpa())
		{
			if (strPlat == L"Android")
				strPath = CONFIG(_strAndroid_CpaDownPath);
			else
				strPath = CONFIG(_strIos_CpaDownPath);
		}

		CFileOperation::MakeLocalDir(strPath.substr(0,strPath.length()-1));
		InstallsAppList::iterator it = mNetInstallPlanItem->NetAppItems->begin();
		while(it != mNetInstallPlanItem->NetAppItems->end())
		{
			InstallsAppInfo *soft = *it;
			if (soft->m_bSelected || bNeed)
			{
				wstring downurl = soft->m_strSavePath;
				int legalindex = downurl.find(L"itunesapp2");
				if(legalindex>0)
				{
					string content=CNetOperation::Get(CCodeOperation::UnicodeToGB2312(downurl),false);
					int pos= content.find("href=");
					CONTINUE_IF(pos<0);
					content =content.substr(pos+6);
					int pos1=content.find("\"");
					CONTINUE_IF(pos1<0);
					content =content.substr(0,pos1);
					downurl = CCodeOperation::GB2312ToUnicode(content);
				}
				wstring wsSoftPath = strPath + soft->m_strName + soft->m_strVersion;
				if (_INSTALLS_CASE_DATA_ANDROID == mNetInstallPlanItem->CaseType())
				{
					wsSoftPath += L".apk";
				}
				else
				{
					wsSoftPath += L".ipa";
				}

				if (mNetInstallPlanItem->IsbCpa() && !CFileOperation::IsFileExist(wsSoftPath.c_str()))
				{
					DelTaskItemMsg DelMsg;
					DelMsg._strUrl = downurl;
					DelMsg.SendToUI();
				}
				int taskId;

				DownloadTaskItemMsg DownloadMsg;
				DownloadMsg.url = downurl;
				DownloadMsg.savepath = strPath;
				DownloadMsg.appname = soft->m_strName;
				DownloadMsg.version = soft->m_strVersion;
				DownloadMsg.platform = strPlat;
				DownloadMsg.fid = 0;
				DownloadMsg.bUpdategrade = false;
				DownloadMsg.needInstall = false;
				DownloadMsg.oldappfullpath = L"";
				DownloadMsg.isLegalSoft = soft->m_isLegalSoft>0?true:false; 
				DownloadMsg.taskupgradetype = (int)NOTUPGRADE;
				DownloadMsg.appID = L"";
				DownloadMsg.SendAndGetReturn();
				taskId = DownloadMsg.taskid;

				if (taskId > 0)
				{
					if(!FindTaskId(mDownloadTasks,taskId))
						mDownloadTasks.push_back(taskId);
					else
					{
						CString strSql;
						UINT planId = mNetInstallPlanItem->Id() - NET_CASE_START_INDEX;
						it=mNetInstallPlanItem->NetAppItems->erase(it);
						strSql.Format(L"delete from NetInstallPlanSofts where planid='%d' and DownloadUrl='%s';" , planId , downurl.c_str());
						CDBManager::DoExecSql(strSql.GetBuffer());
						mTotalDownStep--;
						continue;
					}
				}
				
			}
			it++;
			
		}


		for (size_t i=0;i<mDownloadTasks.size();i++)
		{
			int taskId=mDownloadTasks[i];
			CTaskItem* item;
			GetTaskByIDMsg GetTaskMsg;
			GetTaskMsg.nTaskID = taskId;
			GetTaskMsg.SendAndGetReturn();
			item = (CTaskItem*)GetTaskMsg.pTask;

			if(item)
			{
				item->Lock();
				if(item->m_uStatus==FINISHED)
				{
					if (mTotalInstallStep>0)
					{
						m_curpercent+=100/mTotalInstallStep;
					}
					TaskNotify(item,L"",URL_DOWNLAOD,FINISHED,taskId);
				}
				else
				{
					if (mTotalInstallStep>0)
					{
						m_curpercent+=item->GetTaskPercent()/mTotalInstallStep;
					}
					mDownloadPercents[taskId]=item->GetTaskPercent();
				}
				item->Unlock();
			}
		}
	}
	if(_pObersever)
		_pObersever->NetInstallCaseStatusUpdated();
}

void NetInstallPlanTaskManager::CurrentDownStep( UINT val )
{
	mCurrentDownStep = val;
	if (mCurrentDownStep >= mTotalDownStep)
	{
		Status(_INSTALL_PLAN_STATUS_Downloaded);
		m_curpercent=100;

		PlanStatus(_NET_INSTALL_PLAN_STATUS_DOWNLOADED);

		// 真正全部下载完成的动作
		if (mNetInstallPlanItem->NetAppItems->size()== 0)
		{
			if(!mDeviceId.empty())
			{
				Status(_INSTALL_PLAN_STATUS_Running);
				mTotalInstallStep = mNetInstallPlanItem->ListApp()->size();
				if (mTotalInstallStep>0)
				{
					m_curpercent=mCurrentInstallStep*100/mTotalInstallStep;
				}
				lastpercent = m_curpercent;
			}
			if(_pObersever)
				_pObersever->NetInstallCaseStatusUpdated();
		}
		//可能是手动删除或者失败的情况,总安装数要随着变化 
		else
		{
			if(!mDeviceId.empty())
			{
				Status(_INSTALL_PLAN_STATUS_Running);
				_skipinstallcnt = mNetInstallPlanItem->NetAppItems->size();
				lastpercent = 0;
			}
			//mTotalInstallStep = mNetInstallPlanItem->ListApp()->size();
			if (mCurrentInstallStep >= mTotalInstallStep)
			{
				m_curpercent=100;
				Status(_INSTALL_PLAN_STATUS_Completed);

			}
			if(_pObersever)
				_pObersever->NetInstallCaseStatusUpdated();
		}
	}
	else if(Status()==_INSTALL_PLAN_STATUS_Completed)
	{
		_skipinstallcnt = 0;
		PlanStatus(_NET_INSTALL_PLAN_STATUS_DOWNLOADING);
		Status(_INSTALL_PLAN_STATUS_Running);
		lastpercent = 0;
	}
}

void NetInstallPlanTaskManager::CurrentInstallStep( UINT val )
{
	if (mCurrentInstallStep != val)
	{
		mCurrentInstallStep = val;
		if (mCurrentInstallStep+_skipinstallcnt >= mTotalInstallStep)
		{
			if(mInstallPlanTasks.size()<1&&mCurrentDownStep < mTotalDownStep)//
			{
				_skipinstallcnt = 0;
				lastpercent = 0;
				PlanStatus(_NET_INSTALL_PLAN_STATUS_DOWNLOADING);
				Status(_INSTALL_PLAN_STATUS_Running);
				RefreshDownloadProgress();
				return;
			}
			m_curpercent = 100;
			Status(_INSTALL_PLAN_STATUS_Completed);
			if(_pObersever)
				_pObersever->NetInstallCaseStatusUpdated();
		}
	}
}

void NetInstallPlanTaskManager::Status( _INSTALL_PLAN_STATUS val )
{
	if (mStatus != val)
	{
		mStatus = val;
		StatusChanged();
	}
}

void NetInstallPlanTaskManager::PlanStatus( _NET_INSTALL_PLAN_STATUS val )
{
	if (mNetInstallPlanItem->NetInstallStatus != val)
	{
		if(PlanStatus()==_NET_INSTALL_PLAN_STATUS_DOWNLOADED&&mNetInstallPlanItem->NetAppItems->size()<=0)
			NetInstallsCaseDB::UpdateNetInstallPlanItemsStatus(mNetInstallPlanItem, val);
		mNetInstallPlanItem->NetInstallStatus = val;
	}
}

bool NetInstallPlanTaskManager::Handle(const NotifyTaskItemMsg* pMsg)
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
			this->TaskFinishedNotify(item, pMsg->curID);
		}
		break;
	case WM_TASK_UPDATESPEED:
		{
			this->TaskUpdatedSpeedNotify(item, pMsg->curID);
		}
		break;
	case WM_TASK_FAIL:
		{
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
void NetInstallPlanTaskManager::TaskUpdatedSpeedNotify( CTaskItem* item ,wstring deviceId )
{
	int taskId=item->m_uID;
	if(deviceId.empty())
	{
		RETURN_IF(!FindTaskId(mDownloadTasks,taskId));
		mDownloadPercents[taskId]=item->GetTaskPercent();
		if(item->m_uStatus==PROCEDING)
		{
			EreaseTaskId(mDownloadFailTasks,taskId);
			CurrentDownStep(mDownloadComletedTasks.size() + mDownloadDelTasks.size() + mDownloadFailTasks.size());
		}
		else if(item->m_uStatus==SUSPENDED)
		{
			if(!FindTaskId(mDownloadFailTasks,taskId))
			{
				mDownloadFailTasks.push_back(taskId);
				CurrentDownStep(mDownloadComletedTasks.size() + mDownloadDelTasks.size() + mDownloadFailTasks.size());
			}
		}

		if(mNetInstallPlanItem->NetInstallStatus==_NET_INSTALL_PLAN_STATUS_DOWNLOADING)
			RefreshDownloadProgress();
	}
	else
	{
		RETURN_IF(deviceId!=mDeviceId);
		RETURN_IF(!FindTaskId(mInstallPlanTasks,taskId));
		mInstallPercents[taskId]=item->m_nProcess;
		if(mNetInstallPlanItem->NetInstallStatus==_NET_INSTALL_PLAN_STATUS_DOWNLOADED)
			RefreshInstallProgress();
	}
}

void NetInstallPlanTaskManager::RefreshDownloadProgress()
{
	if(mTotalInstallStep>0)
	{
		m_curpercent=mNetInstallPlanItem->ListApp()->size()*100/mTotalInstallStep;
		for (map<int,int>::iterator it=mDownloadPercents.begin();it!=mDownloadPercents.end();it++)
		{
			m_curpercent+=it->second/mTotalInstallStep;
		}
	}
	if(m_curpercent>(lastpercent+1)&&_pObersever)
	{
		lastpercent=m_curpercent;
		_pObersever->NetInstallCaseStatusUpdated();
	}
}

void NetInstallPlanTaskManager::RefreshInstallProgress()
{
	if(mTotalInstallStep>0)
	{
		int percent=(mTotalInstallStep- mInstallPlanTasks.size())*100/mTotalInstallStep;
		for (map<int,int>::iterator it=mInstallPercents.begin();it!=mInstallPercents.end();it++)
		{
			percent+=it->second/mTotalInstallStep;
		}
		m_curpercent=percent;
	}
	if(m_curpercent>(lastpercent+1)&&_pObersever)
	{
		lastpercent=m_curpercent;
		_pObersever->NetInstallCaseStatusUpdated();
	}
}

void NetInstallPlanTaskManager::SetObersever( NetInstallCaseObersever* pObersever )
{
	_pObersever = pObersever;
}


INSTALLCASE_END