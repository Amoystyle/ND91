#include "stdafx.h"
#include "ModuleManager.h"
#include "Logic/ModuleController.h"

ModuleManager* ModuleManager::_pInstance = NULL;
ModuleManager::ModuleManager()
{
	_pController = new ModuleController();
}

ModuleManager::~ModuleManager()
{
	SAFE_DELETE(_pController);
}

ModuleManager* ModuleManager::GetInstance()
{
	if(_pInstance == NULL)
		_pInstance = new ModuleManager();

	return _pInstance;
}

bool ModuleManager::Init( CContainerUI* pTitleToolBar, CContainerUI* pMain )
{
	return _pController->Init(pTitleToolBar, pMain);
}

void ModuleManager::Show()
{
	_pController->Show();
}

void ModuleManager::Hide()
{
	_pController->Hide();
}

void ModuleManager::Release()
{
	SAFE_DELETE(_pInstance);
}
