#include "stdafx.h"
#include "InitModuleThread.h"
#include "Logic/ModuleController.h"

InitModuleThread::InitModuleThread(string threadId, ModuleController* pController) : BaseThread(threadId)
{
	_pModuleController = pController;
}

InitModuleThread::~InitModuleThread()
{
	
}

unsigned long InitModuleThread::Main()
{
	_pModuleController->LoadModules();
	return 0;
}
