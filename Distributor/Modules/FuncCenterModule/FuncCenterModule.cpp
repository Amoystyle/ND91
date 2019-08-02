#include "stdafx.h"
#include "FuncCenterModule.h"
#include "Logic/FuncCenterController.h"

FUNCCENTER_API DeviceModule* CreateFuncCenterModule( wstring deviceID, void* phone )
{
	return new FuncCenterModule(deviceID, phone);
}

FuncCenterModule::FuncCenterModule(wstring deviceID, void* phone) : DeviceModule(deviceID, phone)
{
	_pController = new FuncCenterController(deviceID, phone);
}

FuncCenterModule::~FuncCenterModule()
{

}

tstring FuncCenterModule::Id()
{
	return L"FuncCenterModule";
}

tstring FuncCenterModule::Name()
{
	return CommonUIFunc::LoadString(L"FuncCenter_Text_ModuleText");
}

tstring FuncCenterModule::Icon()
{
	return L"#FuncCenter_Image_ModuleIcon";
}

int FuncCenterModule::LayoutIndex()
{
	return 0;
}

void FuncCenterModule::Show()
{
	this->_pController->Show();
}

void FuncCenterModule::Hide()
{
	this->_pController->Hide();
}

bool FuncCenterModule::Init( CContainerUI* pContainer )
{
	this->_pController->Init(pContainer);
	return true;
}

void FuncCenterModule::Release()
{
	SAFE_DELETE(this->_pController);
}
