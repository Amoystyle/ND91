#include "StdAfx.h"
#include "LocalCpaCheckThread.h"
#include "CpaDataHelper.h"
#include "Logic/WebResourceInnerMsg.h"

CLocalCpaCheckThread::CLocalCpaCheckThread(const string threadId)
:BaseThread(threadId)
{
}

CLocalCpaCheckThread::~CLocalCpaCheckThread(void)
{
}

unsigned long CLocalCpaCheckThread::Main()
{
	DeleteLocalCpa msg;
	msg._map = CCpaDataHelper::GetInstance()->GetAllDataCopy();
	msg.Send();
	return 0;
}
