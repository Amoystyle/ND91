#include "stdafx.h"
#include "InstallCaseController.h"

InstallCaseController::InstallCaseController()
{
	_pLeftView      = new LeftView;
	_pInstallCaseUI = new InstallCaseUI(_pLeftView);	
}

InstallCaseController::~InstallCaseController()
{

}

bool InstallCaseController::Init(CContainerUI* pLeft, CContainerUI* pContainer)
{
	_pLeftView->Init(pLeft);
	_pInstallCaseUI->Init(pContainer);
	return true;
}

void InstallCaseController::Show()
{
	_pInstallCaseUI->Show();
	_pLeftView->Show();
}

void InstallCaseController::Hide()
{
	_pInstallCaseUI->Hide();
	_pLeftView->Hide();
}

void InstallCaseController::Exit()
{
	vector<InstallsCaseData*> verInstalls = _pInstallCaseUI->Model.GetCaseDataList();
	//StatisticHelp stathelp;
	//stathelp.PostZJInstallPlanNum(verInstalls);
	PostZJInstallPlanNumMsg postZJInstallPlanNumMsg;
	postZJInstallPlanNumMsg.pInstallsCaseDataCollection = (void*)(&verInstalls);
	postZJInstallPlanNumMsg.Send();
}