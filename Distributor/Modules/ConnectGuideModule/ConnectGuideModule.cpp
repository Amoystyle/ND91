#include "stdafx.h"
#include "ConnectGuideModule.h"
#include "Logic/ConnectGuideController.h"

CONNECTGUIDE_API DeviceModule* CreateConnectGuideModule( wstring deviceID, void* phone )
{
	return new ConnectGuideModule(deviceID, phone);
}

ConnectGuideModule::ConnectGuideModule(wstring deviceID, void* phone) : DeviceModule(deviceID, phone)
{
	pController = new ConnectGuideController(deviceID, phone);
}

ConnectGuideModule::~ConnectGuideModule()
{

}

tstring ConnectGuideModule::Id()
{
	return L"ConnectGuideModule";
}

tstring ConnectGuideModule::Name()
{
	return CommonUIFunc::LoadString(L"ConnectGuide_Text_ModuleText");
}

tstring ConnectGuideModule::Icon()
{
	return L"#ConnectGuide_Image_ModuleIcon";
}

int ConnectGuideModule::LayoutIndex()
{
	return 0;
}

void ConnectGuideModule::Show()
{
	pController->Show();
}

void ConnectGuideModule::Hide()
{
	pController->Hide();
}

bool ConnectGuideModule::Init(CContainerUI* pContainer)
{
	pController->Init(pContainer);
	return true;
}

void ConnectGuideModule::Release()
{
	SAFE_DELETE(this->pController);
}
