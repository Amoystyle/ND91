#include "stdafx.h"
#include "ModuleController.h"
#include "UI/MainView.h"
#include "Modules/UserCenterModule/UserCenterModule.h"
#include "Modules/DeviceCenterModule/DeviceCenterModule.h"
#include "Modules/InstallCaseModule/InstallCaseModule.h"
#include "Modules/WebResourceModule/WebResourceModule.h"
#include "Modules/TaskCenterModule/TaskCenterModule.h"
#include "Modules/DependenceFuncModule/DependenceFuncCenter.h"
#include "Framework/AssistantInstallManager/AssistantInstallManager.h"
#include "LoadResourceThread.h"
#include "InitModuleThread.h"

ModuleController::ModuleController()
{
	_pParent = NULL;
	_pMainView = new MainView();
	_pSelectModule = NULL;
}

ModuleController::~ModuleController()
{
	Cabl::Release();
}

bool ModuleController::Init(CContainerUI* pTitleToolBar,  CContainerUI* pMain )
{
	if(pMain == NULL)
		return false;

	_pParent = pMain;
	_pTitleToolBar = pTitleToolBar;

	RequestShowResourceLoadingMsg msg;
	msg.bShow = true;
	msg.SendToUI();

	LoadResourceThread* pThread = new LoadResourceThread("ModuleController:LoadResourceThread");
	pThread->Run();

	return true;
}

void ModuleController::LoadModules()
{	
	SourceModule* pDeviceModule = CreateDeviceCenterModule();
	this->_pMainView->AddModule(pDeviceModule);

	/*SourceModule* pInstallCaseModule = CreateInstallCaseModule();
	this->_pMainView->AddModule(pInstallCaseModule);

	SourceModule* pWebResourceModule = CreateWebResourceModule();
	this->_pMainView->AddModule(pWebResourceModule);*/

	SourceModule* pTaskCenterModule = CreateTaskCenterModule();
	this->_pMainView->AddModule(pTaskCenterModule);

	DependenceFuncCenter::GetInstance()->Init(_pTitleToolBar, this->_pMainView->GetStatusBar());
	
	this->_pMainView->SelectModuleButton(pDeviceModule);
	
	Cabl::GetInstance()->Init();
	AssistantInstallManager::Init();

	SourceModule* pUserModule = CreateUserCenterModule();
	this->_pMainView->AddUserModule(pUserModule);

	RequestShowResourceLoadingMsg msg;
	msg.bShow = false;
	msg.SendToUI();
}

bool ModuleController::Handle( const ModuleManagerInnerMsg* pMsg )
{
	switch (pMsg->nAction)
	{
	case ModuleManager_Action_LoadResourceCompleted:
		{
			if(this->_pParent != NULL)
			{
				_pMainView->Init(this->_pParent);
				_pMainView->Hide();
			}
		}
		break;
	case ModuleManager_Action_LoadModules:
		{
			InitModuleThread* pThread = new InitModuleThread("ModuleController:InitModuleThread", this);
			pThread->Run();
		}
		break;
	case ModuleManager_Action_SelectModule:
		{
			if(!pMsg->wParam)
				return true;

			this->SelectModule((SourceModule*)pMsg->wParam);
			_pMainView->Show();
		}
		break;
	}
	return true;
}

void ModuleController::SelectModule( SourceModule* pModule )
{
	if(pModule == NULL || _pSelectModule == pModule)
		return;

	if(_pSelectModule != NULL)
	{
		_pSelectModule->Hide();
		_pSelectModule = NULL;
	}

	pModule->Show();
	_pSelectModule = pModule;
}

void ModuleController::Show()
{
	this->_pMainView->Show();
}

void ModuleController::Hide()
{
	this->_pMainView->Hide();
}