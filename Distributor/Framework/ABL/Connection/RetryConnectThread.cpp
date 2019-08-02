#include "stdafx.h"
#include "RetryConnectThread.h"

RetryConnectThread::RetryConnectThread( string threadID, void* pPhone )
						: BaseThread(threadID)
{
    _autoDelete = true;
	_pPhone = pPhone;
}

RetryConnectThread::~RetryConnectThread()
{
}

unsigned long RetryConnectThread::Main()
{
	Sleep(2000);

	LOG->WriteDebug(L"RetryConnectThread:  Run");

	RequestConnectMsg msg;
	msg.pPhone = _pPhone;
	msg.bRetry = true;
	msg.SendToUI();

	return 0;
}