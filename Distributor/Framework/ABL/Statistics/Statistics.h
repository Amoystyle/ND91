#pragma once
#include "Manager/EventManager.h"
#include "Ufslib.h"

#define Statistic_StatFunc(id) LOG->WriteDebug(L"记录统计点");if (_pUfsHandle) FunctionCount(_pUfsHandle, id);
#define Statistic_StatFuncTab(id, tab, ret) LOG->WriteDebug(L"记录统计点");if (_pUfsHandle) ret = FunctionCount(_pUfsHandle, id, NULL, tab);
class CStatistics
	: public BaseHandler<CStaticsticsMessage>
	, public BaseHandler<CStaticsticsDevice>
	, public BaseGetData<CStaticsticsMessageWithTab>
	, public BaseGetData<RequestWindowSystemInfoMsg> //临时放在这里
	, public BaseHandler<ResponseUserLoginMsg>
	, public IExit
{
	friend class StatisticHelp;
public:
	CStatistics(void);
	~CStatistics(void);

	virtual bool Handle( const CStaticsticsMessage* msg );

	virtual bool Handle( const CStaticsticsDevice* msg );

	virtual bool Handle( CStaticsticsMessageWithTab* pMsg );

	virtual bool Handle( RequestWindowSystemInfoMsg* pMsg );

	virtual bool Handle( const ResponseUserLoginMsg* pMsg );

	virtual void Exit();

	UfslibHandle GetHandle() {return _pUfsHandle;}
	DEVICE_DATA*  GetDeviceData() { return &_pdata; }

protected:
	UfslibHandle _pUfsHandle;
	DEVICE_DATA	_pdata;

};
