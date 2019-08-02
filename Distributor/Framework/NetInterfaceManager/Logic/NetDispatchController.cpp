#include "StdAfx.h"
#include "NetDispatchController.h"
#include "NetDownloadFile.h"

//���лر� ���ȵ���������
UINT g_DownloadFileTaskID = 0;

NetDispatchController* NetDispatchController::_pNetDispatchController =  NULL;
NetDispatchController::NetDispatchController(void)
{
}

NetDispatchController::~NetDispatchController(void)
{
}

NetDispatchController* NetDispatchController::GetInstance()
{
	if (NULL == _pNetDispatchController)
	{
		_pNetDispatchController = new NetDispatchController;
	}

	return _pNetDispatchController;
}

void NetDispatchController::Init()
{

}

void NetDispatchController::GetPcSuiteFunc(vector<FunctionItem>& And_STPcSuitFunc, vector<FunctionItem>& Ios_STPcSuitFunc)
{
	//��ȡ���ܴ�ȫ
	_PcSuiteFunction.GetPcSuiteFunc(And_STPcSuitFunc, Ios_STPcSuitFunc);
}

void NetDispatchController::DownloadFileReportProgress(string wsDownPath, wstring wsSavePath, UINT unTaskID)
{
	CNetDownloadFile* pNetDownloadFile = new CNetDownloadFile;
	pNetDownloadFile->SetDownloadInfo(wsDownPath, wsSavePath, unTaskID);
	pNetDownloadFile->Run();
}

UINT NetDispatchController::GetDownloadFileTaskID()
{
	return g_DownloadFileTaskID++;
}