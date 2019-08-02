#pragma once
#include "Base/BaseThread.h"
#include "StaticsticsInnerMsg.h"
#include "../TaskHelpModel.h"
#include <map>
using namespace std;
class CTaskStatisticsThread : public BaseThread
{
	typedef void (CTaskStatisticsThread::*RunFunc)();
	typedef map<TaskStatistcsAction,RunFunc> RunFunc_MAP;
public:
	CTaskStatisticsThread(const string id, TaskStatistcsAction action);
	~CTaskStatisticsThread(void);

	virtual unsigned long Main();

	void SetVoidParam(void* param);

	void SetStrParam(wstring strParam);

	void SetModel(TaskHelpModel *pModel);
private:
	TaskStatistcsAction _action;
	void* _param;
	wstring _strParam;
	RunFunc_MAP _mapRunFuncs;
	TaskHelpModel *_pModel;

	void SendDownLoadInfo();
	void SendInstallInfo();
	void SendData();
	void SendDownlosdSms();
	void SendSuccessInfo();
	void SendFailInfo();
};
