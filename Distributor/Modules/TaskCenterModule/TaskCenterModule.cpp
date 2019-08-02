#include "stdafx.h"
#include "TaskCenterModule.h"
#include "Logic/TaskCenterController.h"

TASKCENTERMODULE_API SourceModule* CreateTaskCenterModule()
{
	return TaskCenterModule::GetInstance();
}

TaskCenterModule* TaskCenterModule::_pInstance = NULL;
TaskCenterModule::TaskCenterModule( void )
{
	_pController = new TaskCenterController();
}

TaskCenterModule::~TaskCenterModule()
{

}

TaskCenterModule* TaskCenterModule::GetInstance()
{
	if(_pInstance == NULL)
		_pInstance = new TaskCenterModule();

	return _pInstance;
}

tstring TaskCenterModule::Id()
{
	return L"TaskCenterModule";
}

tstring TaskCenterModule::Name()
{
	return CommonUIFunc::LoadString(L"TaskCenter_Text_ModuleText");
}

tstring TaskCenterModule::Icon()
{
	return L"#TaskCenter_Image_ModuleIcon";
}

int TaskCenterModule::LayoutIndex()
{
	return 0;
}

void TaskCenterModule::Show()
{
	_pController->Show();
}

void TaskCenterModule::Hide()
{
	_pController->Hide();
}

bool TaskCenterModule::Init(CContainerUI* pLeft, CContainerUI* pContainer )
{
	if(!_pController->Init(pLeft, pContainer))
		return false;

	return true;
}