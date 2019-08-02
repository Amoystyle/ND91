#include "stdafx.h"
#include "ApplicationManager.h"
#include "InstallAppThread.h"


ApplicationManager::ApplicationManager()
{

}

ApplicationManager::~ApplicationManager()
{

}

void ApplicationManager::Init()
{

}

bool ApplicationManager::Handle( RequestLoadAppMsg* pMsg )
{
	int requestID = 1; //ͨ��ThreadManager����

	/* new Thread ִ�����£�
	ResponseLoadAppMsg msg;
	msg.Begin();
	msg.nRequestID = requestID?
	for ()
	{
		copy msg.item
		msg.SendToUI();
	}
	msg.End();*/

	pMsg->nRequestID = requestID;
	return true;
}

bool ApplicationManager::Handle( const RequestInstallAppMsg* msg )
{
	int nID = Singleton::GetThreadManager()->GetGUID();
	string threadID = "RequestInstallAppThread-" + CStrOperation::Int2string(nID);

	InstallAppThread* pThread = new InstallAppThread(threadID, nID, msg->deviceID);
	pThread->Run();

	return true;
}
