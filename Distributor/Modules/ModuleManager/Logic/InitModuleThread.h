#pragma once

class ModuleController;

class InitModuleThread
	: public BaseThread
{
public:
	InitModuleThread(string threadId, ModuleController* pController);
	~InitModuleThread();

	virtual unsigned long Main();

private:
	ModuleController* _pModuleController;
};