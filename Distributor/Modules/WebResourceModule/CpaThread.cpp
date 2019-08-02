#include "StdAfx.h"
#include "CpaThread.h"
#include "CpaDataHelper.h"
CCpaThread::CCpaThread(const string threadId)
:BaseThread(threadId)
{
}

CCpaThread::~CCpaThread(void)
{
}

unsigned long CCpaThread::Main()
{
	CCpaDataHelper::GetInstance()->InitInternal();
	return 0;
}
