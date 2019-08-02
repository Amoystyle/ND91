#include "stdafx.h"
#include "InstallCaseModule.h"
#include "Logic/InstallCaseController.h"


INSTALLCASE_API SourceModule* CreateInstallCaseModule()
{
	return InstallCaseModule::GetInstance();
}

InstallCaseModule* InstallCaseModule::_pInstance = NULL;

InstallCaseModule::InstallCaseModule( void )
{
	_pController = new InstallCaseController();
}

InstallCaseModule::~InstallCaseModule()
{

}

InstallCaseModule* InstallCaseModule::GetInstance()
{
	if(_pInstance == NULL)
		_pInstance = new InstallCaseModule();

	return _pInstance;
}

tstring InstallCaseModule::Id()
{
	return L"InstallCaseModule";
}

tstring InstallCaseModule::Name()
{
	return CommonUIFunc::LoadString(L"InstallCase_Text_ModuleText");
}

tstring InstallCaseModule::Icon()
{
	return L"#InstallCase_Image_ModuleIcon";
}

int InstallCaseModule::LayoutIndex()
{
	return 0;
}

void InstallCaseModule::Show()
{
	//_pController->Show();
}

void InstallCaseModule::Hide()
{
	_pController->Hide();
}

bool InstallCaseModule::Init(CContainerUI* pLeft, CContainerUI* pContainer )
{
	_pController->Init(pLeft, pContainer);
	return true;
}