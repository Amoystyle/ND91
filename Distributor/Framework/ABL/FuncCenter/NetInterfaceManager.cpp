#include "StdAfx.h"
#include "NetInterfaceManager.h"
#include "logic/NetDispatchController.h"

vector<FunctionItem> NetInterfaceManager::_Android_PcSuitFunction;
vector<FunctionItem> NetInterfaceManager::_IOS_PcSuitFunction;

NetInterfaceManager::NetInterfaceManager(void)
{
}

NetInterfaceManager::~NetInterfaceManager(void)
{
}

void NetInterfaceManager::Init()
{
	//创建线程获取第三方功能配置
	CreateThread(0,0,(LPTHREAD_START_ROUTINE)GetPCSuitFunc, NULL, 0, NULL);
}


void NetInterfaceManager::GetPCSuitFunc()
{
	try
	{
		NetDispatchController::GetInstance()->GetPcSuiteFunc(_Android_PcSuitFunction, _IOS_PcSuitFunction);

		ReponsePcSuitFunctionMsg ReponseFunctionMsg;
		ReponseFunctionMsg.wsPlatform = L"android";
		ReponseFunctionMsg.vec_Function = _Android_PcSuitFunction;
		ReponseFunctionMsg.SendToUI();

		ReponseFunctionMsg.wsPlatform = L"iphone";
		ReponseFunctionMsg.vec_Function = _IOS_PcSuitFunction;
		ReponseFunctionMsg.SendToUI();
	}
	catch (AssistantException e)
	{
		LOG->WriteDebug(_T("方法GetPCSuitFunc出错"));
	}
	catch (...)
	{
		LOG->WriteDebug(_T("方法GetPCSuitFunc出错"));
	}

}

bool NetInterfaceManager::Handle(GetDownloadFileReportTaskIDMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	pMsg->unTaskID = NetDispatchController::GetDownloadFileTaskID();

	return true;
}

bool NetInterfaceManager::Handle(const DownloadFileReportMsg* pMsg)
{
	if (NULL == pMsg)
	{
		return false;
	}

	NetDispatchController::DownloadFileReportProgress(CCodeOperation::UnicodeToGB2312(pMsg->wsDownPath), pMsg->wsSavePath, pMsg->unTaskID);

	return true;
}

bool NetInterfaceManager::Handle(GetPcSuitFunctionMsg* pMsg)
{
	if ( NULL == pMsg)
	{
		return false;
	}

	if ( L"android" == pMsg->wsPlatform)
	{
		pMsg->vec_Function = _Android_PcSuitFunction;
	}
	else
	{
		pMsg->vec_Function = _IOS_PcSuitFunction;
	}

	return true;
}