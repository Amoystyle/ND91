#include "stdafx.h"
#include "RecentInstallCaseModule.h"
#include "Logic/RecentInstallCaseController.h"

RECENTINSTALLCASEMODULE_API DeviceModule* CreateRecentInstallCaseModule(wstring deviceID, void* phone)
{
	return new RecentInstallCaseModule(deviceID, phone);
}

RecentInstallCaseModule::RecentInstallCaseModule( wstring deviceID, void* phone ) : DeviceModule(deviceID, phone)
{
	_pController = new RecentInstallCaseController(deviceID);
}

RecentInstallCaseModule::~RecentInstallCaseModule()
{

}

tstring RecentInstallCaseModule::Id()
{
	return L"RecentInstallCase";
}

tstring RecentInstallCaseModule::Name()
{
	return CommonUIFunc::LoadString(L"RecentInstallCase_Text_ModuleText");
}

tstring RecentInstallCaseModule::Icon()
{
	return L"#RecentInstallCase_Image_ModuleIcon";
}

int RecentInstallCaseModule::LayoutIndex()
{
	return 0;
}

void RecentInstallCaseModule::Show()
{
	_pController->Show();
}

void RecentInstallCaseModule::Hide()
{
	_pController->Hide();
}

bool RecentInstallCaseModule::Init(CContainerUI* pContainer)
{
	_pController->Init(pContainer);
	return true;
}

void RecentInstallCaseModule::Release()
{
	SAFE_DELETE(this->_pController);
}
