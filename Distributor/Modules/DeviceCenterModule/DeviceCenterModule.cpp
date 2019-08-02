// Device.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"
#include "DeviceCenterModule.h"
#include "Logic/DeviceController.h"

DEVICECENTERM_API SourceModule* CreateDeviceCenterModule()
{
	return DeviceCenterModule::GetInstance();
}

DeviceCenterModule* DeviceCenterModule::_pInstance = NULL;
DeviceCenterModule::DeviceCenterModule()
{
	_pController = new DeviceController();
}

DeviceCenterModule* DeviceCenterModule::GetInstance()
{
	if(_pInstance == NULL)
		_pInstance = new DeviceCenterModule();

	return _pInstance;
}

DeviceCenterModule::~DeviceCenterModule()
{
	SAFE_DELETE(_pController);
}

tstring DeviceCenterModule::Id()
{
	return L"DeviceModule";
}

tstring DeviceCenterModule::Name()
{
	return CommonUIFunc::LoadString(L"Device_Text_ModuleText");
}

tstring DeviceCenterModule::Icon()
{
	return L"#Device_Image_ModuleIcon";
}

int DeviceCenterModule::LayoutIndex()
{
	return 0;
}

void DeviceCenterModule::Show()
{
	this->_pController->Show();
}

void DeviceCenterModule::Hide()
{
	this->_pController->Hide();
}

bool DeviceCenterModule::Init(CContainerUI* pLeft, CContainerUI* pContainer )
{
	if(!_pController->Init(pLeft, pContainer))
	{
		return false;
	}

	return true;
}
