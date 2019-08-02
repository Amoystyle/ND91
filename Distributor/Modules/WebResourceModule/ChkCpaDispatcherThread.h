#pragma once
#include "Base/BaseThread.h"
#include "CWebResourceMacros.h"
class CChkCpaDispatcher;
class CChkCpaDispatcherThread:
	public BaseThread
{
public:
	CChkCpaDispatcherThread(const string threadId);
	~CChkCpaDispatcherThread(void);
	PROPERTY(CChkCpaDispatcher*,_chkCpaDispatcher,ChkCpaDispatcher)
	
private:
	virtual unsigned long Main();
};
