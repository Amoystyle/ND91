#include "stdafx.h"
#include "InstallAppsyncThread.h"
#include "../CheckCanInstallAppsync.h"

InstallAppsyncThread::InstallAppsyncThread(const string id)
:BaseThread(id)
{
}



InstallAppsyncThread::~InstallAppsyncThread()
{

}

unsigned long InstallAppsyncThread::Main()
{
	WaitForSingleObject(m_event_lock, INFINITE);
	CloseHandle(m_event_lock);


	wstring errorMsg = L"";
	if( CheckCanInstallAppsync::Install(m_deviceid, errorMsg) )
	{	//安装appsync成功
		RequestInstallAppsyncMsg  Returnmsg;
		Returnmsg.deviceID = m_deviceid;
		Returnmsg.installReturn = true;
		Returnmsg.SendToUI();

	}
	else
	{//安装appsync失败
		RequestInstallAppsyncMsg  Returnmsg;
		Returnmsg.deviceID =m_deviceid;
		Returnmsg.installReturn = false;
		Returnmsg.SendToUI();
	}
	return 0;
}

