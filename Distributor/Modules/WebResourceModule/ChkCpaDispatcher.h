#pragma once
#include "NetResourceStruct.h"
class CChkCpaTask;
struct CpaTaskStatueMsg;
class CChkCpaDispatcherThread;
class CChkCpaDispatcher:
	private BaseCSLock,
	public BaseHandler<CpaTaskStatueMsg>
{
public:
	CChkCpaDispatcher(void);
	virtual ~CChkCpaDispatcher(void);

	void AddTask(wstring strHashValue,ENUM_PLATFORM plat,wstring guid);

	void Run();
private:
	CChkCpaTask* _curTask;
	CChkCpaTask* _backgroundTask;
	void Swap();

	virtual bool Handle( const CpaTaskStatueMsg* pMsg );

	CChkCpaDispatcherThread* _pThread;
	HANDLE _signal;
};
