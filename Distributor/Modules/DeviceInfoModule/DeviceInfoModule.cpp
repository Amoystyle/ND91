#include "stdafx.h"
#include "DeviceInfoModule.h"
#include "Logic/DeviceInfoController.h"

DEVICEINFO_API DeviceModule* CreateDeviceInfoModule(wstring deviceID, void* phone)
{
	return new DeviceInfoModule(deviceID, phone);
}

DeviceInfoModule::DeviceInfoModule( wstring deviceID, void* phone ) : DeviceModule(deviceID, phone)
{
	_pController = new DeviceInfoController(deviceID, phone);
}

DeviceInfoModule::~DeviceInfoModule()
{

}

tstring DeviceInfoModule::Id()
{
	return L"DeviceInfoModule";
}

tstring DeviceInfoModule::Name()
{
	return CommonUIFunc::LoadString(L"DeviceInfo_Text_ModuleText");
}

tstring DeviceInfoModule::Icon()
{
	return L"#DeviceInfo_Image_ModuleIcon";
}

int DeviceInfoModule::LayoutIndex()
{
	return 0;
}

void DeviceInfoModule::Show()
{
	_pController->Show();
}

void DeviceInfoModule::Hide()
{
	_pController->Hide();
}

bool DeviceInfoModule::Init(CContainerUI* pContainer)
{
	_pController->Init(pContainer);
	return true;
}

void DeviceInfoModule::Release()
{
	SAFE_DELETE(this->_pController);
}
