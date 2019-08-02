#include "stdafx.h"
#include "Manager/ThreadManager.h"
#include "InstallAppsyncController.h"
#include "Thread/NeedInstallThread.h"
#include "CheckCanInstallAppsync.h"
#include "Thread/InstallAppsyncThread.h"



InstallAppsyncController::InstallAppsyncController()
{
}



InstallAppsyncController::~InstallAppsyncController()
{

}



bool InstallAppsyncController::Handle( const RequestInstallAppSyncMsg* pMsg )
{


	HANDLE event_lock = CreateEvent(NULL, false, false, NULL);	

	BeginInstallDeamonMsg DeamonMsg;
	DeamonMsg.deviceID = pMsg->deviceID;
	DeamonMsg.bAppSync = true;
	DeamonMsg.Send();
	m_event[pMsg->deviceID] = event_lock;
	

	const string threadid = "InstallAppsyncThread" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	InstallAppsyncThread *appsyncThread= new InstallAppsyncThread(threadid);
	appsyncThread->m_deviceid = pMsg->deviceID;
	appsyncThread->m_event_lock = event_lock;
	appsyncThread->Run();

	return true;
}


bool InstallAppsyncController::Handle( RequestNeedInstallAppSyncMsg* pMsg )
{
	if(!pMsg->bSync)
	{
		string threadID = "RequestNeedInstallAppSyncMsg-" + CCodeOperation::UnicodeToGB2312(pMsg->deviceID);

		//具体操作未实现
		NeedInstallThread* pThread = new NeedInstallThread(threadID, pMsg->deviceID);
		pThread->Run();
	}
	else
	{
		pMsg->bNeed = CheckCanInstallAppsync::CanInstall(pMsg->deviceID);//获取是否安装appasync
	}

	return true;
}

bool InstallAppsyncController::Handle( const RequestInstallDeamonOverMsg* pMsg )
{
	
	map<wstring,HANDLE> :: iterator event_ite;

	event_ite = m_event.find(pMsg->deviceID);
	HANDLE event_lock = event_ite->second;
	if (event_lock)
	{
		SetEvent(event_lock);	
	}
	
	return true;
}
