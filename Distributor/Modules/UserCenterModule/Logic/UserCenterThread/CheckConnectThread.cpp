#include "stdafx.h"
#include "CheckConnectThread.h"
#include "../UserCenterInfoMsg.h"


unsigned long CheckConnectThread::Main()
{
	BOOL bConnect=InternetCheckConnection(_T("http://zj.91.com/"),FLAG_ICC_FORCE_CONNECTION,0);

	CheckConnectInfoMsg msg;
	msg.bConnectOK = bConnect == TRUE;
	msg.SendToUI();

	return 0;
}

CheckConnectThread::CheckConnectThread( const string id )
:BaseThread(id)
{

}

CheckConnectThread::~CheckConnectThread()
{

}
