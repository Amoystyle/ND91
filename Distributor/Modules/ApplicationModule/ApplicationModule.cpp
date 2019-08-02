#include "stdafx.h"
#include "ApplicationModule.h"
#include "Logic/ApplicationController.h"

APPLICATION_API DeviceModule* CreateApplicationModule( wstring deviceID, void* phone )
{
	return new ApplicationModule(deviceID, phone);
}

ApplicationModule::ApplicationModule(wstring deviceID, void* phone) : DeviceModule(deviceID, phone)
{
	_pController = new ApplicationController(deviceID, phone);
}

ApplicationModule::~ApplicationModule()
{

}

tstring ApplicationModule::Id()
{
	return L"ApplicationModule";
}

tstring ApplicationModule::Name()
{
	return CommonUIFunc::LoadString(L"Application_Text_ModuleText");
}

tstring ApplicationModule::Icon()
{
	return L"#Application_Image_ModuleIcon";
}

int ApplicationModule::LayoutIndex()
{
	return 0;
}

void ApplicationModule::Show()
{
	this->_pController->Show();
}

void ApplicationModule::Hide()
{
	this->_pController->Hide();
}

bool ApplicationModule::Init( CContainerUI* pContainer )
{
	this->_pController->Init(pContainer);
	return true;
}

void ApplicationModule::Release()
{
	SAFE_DELETE(this->_pController);
}

