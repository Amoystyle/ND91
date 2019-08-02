#include "StdAfx.h"
#include "TaskStatisticsThread.h"

CTaskStatisticsThread::CTaskStatisticsThread( const string id, TaskStatistcsAction action )
:BaseThread(id)
{
	_action =  action;
	_param	= NULL;
	_pModel	= NULL;
	_mapRunFuncs[Action_PostDownLoadInfo] = &CTaskStatisticsThread::SendDownLoadInfo;
	_mapRunFuncs[Action_PostInstallInfo] = &CTaskStatisticsThread::SendInstallInfo;
	_mapRunFuncs[Action_PostDataToService] = &CTaskStatisticsThread::SendData;
	_mapRunFuncs[Action_SendDownloadInfo] = &CTaskStatisticsThread::SendDownlosdSms;
	_mapRunFuncs[Action_SendInstallInfo] = &CTaskStatisticsThread::SendSuccessInfo;
	_mapRunFuncs[Action_SendFailInfo] = &CTaskStatisticsThread::SendFailInfo;
}

CTaskStatisticsThread::~CTaskStatisticsThread( void )
{
	
}

unsigned long CTaskStatisticsThread::Main()
{
	RunFunc_MAP::iterator it(_mapRunFuncs.find(_action));
	if (it != _mapRunFuncs.end())
	{
		RunFunc pFunc = it->second;
		(this->*pFunc)();
	}
	return 0;
}

void CTaskStatisticsThread::SetVoidParam( void* param )
{
	_param = param;
}

void CTaskStatisticsThread::SetStrParam( wstring strParam )
{
	_strParam = strParam;
}

void CTaskStatisticsThread::SendDownLoadInfo()
{
	if (NULL == _pModel)
	{
		return;
	}
	StaticsticsInnerMsg *msg = new StaticsticsInnerMsg(Action_PostDownLoadInfo);
	msg->voidParam = _param;
	_pModel->Action(Action_PostDownLoadInfo,msg);
}

void CTaskStatisticsThread::SendInstallInfo()
{
	if (NULL == _pModel)
	{
		return;
	}
	StaticsticsInnerMsg *msg = new StaticsticsInnerMsg(Action_PostInstallInfo);
	msg->voidParam = _param;
	_pModel->Action(Action_PostInstallInfo,msg);
}

void CTaskStatisticsThread::SendData()
{
	if (NULL == _pModel)
	{
		return;
	}
	StaticsticsInnerMsg *msg = new StaticsticsInnerMsg(Action_PostDataToService);
	msg->strParam = _strParam;
	_pModel->Action(Action_PostDataToService,msg);
}

void CTaskStatisticsThread::SendDownlosdSms()
{
	if (NULL == _pModel)
	{
		return;
	}
	StaticsticsInnerMsg *msg = new StaticsticsInnerMsg(Action_SendDownloadInfo);
	msg->strParam = _strParam;
	_pModel->Action(Action_SendDownloadInfo,msg);
}

void CTaskStatisticsThread::SendSuccessInfo()
{
	StaticsticsInnerMsg *msg = new StaticsticsInnerMsg(Action_SendInstallInfo);
	msg->strParam = _strParam;
	_pModel->Action(Action_SendInstallInfo,msg);
}

void CTaskStatisticsThread::SendFailInfo()
{
	StaticsticsInnerMsg *msg = new StaticsticsInnerMsg(Action_SendFailInfo);
	msg->voidParam = _param;
	_pModel->Action(Action_SendFailInfo,msg);
}

void CTaskStatisticsThread::SetModel( TaskHelpModel *pModel )
{
	this->_pModel = pModel;
}

