#include "StdAfx.h"
#include "../Statistics.h"
#include "StaticsticsDeviceInfoThread.h"
#include "Ufslib.h"

StaticsticsDeviceInfoThread::StaticsticsDeviceInfoThread(const string threadId, CStatistics* pHelper)
:BaseThread(threadId)
{
	_pHelper = pHelper;
}

StaticsticsDeviceInfoThread::~StaticsticsDeviceInfoThread(void)
{
}

unsigned long StaticsticsDeviceInfoThread::Main()
{
	::SendDevInfo(_pHelper->GetHandle(),_pHelper->GetDeviceData());
	return 0;
}