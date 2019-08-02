#include "stdafx.h"
#include "UserCenterModule.h"
#include "Logic/UserCenterController.h"

USERCENTERMODULE_API SourceModule* CreateUserCenterModule()
{
	return UserCenterModule::GetInstance();
}

UserCenterModule* UserCenterModule::_pInstance = NULL;

UserCenterModule::UserCenterModule()
{
	_pController = new UserCenterController();
}

UserCenterModule::~UserCenterModule()
{

}

tstring UserCenterModule::Id()
{
	return L"UserCenterModule";
}

tstring UserCenterModule::Name()
{
	return CommonUIFunc::LoadString(L"UserCenter_Text_ModuleText");
}

tstring UserCenterModule::Icon()
{
	return L"#UserCenter_Image_ModuleIcon";
}

int UserCenterModule::LayoutIndex()
{
	return 0;
}

void UserCenterModule::Show()
{
	this->_pController->Show();
}

void UserCenterModule::Hide()
{
	this->_pController->Hide();
}

bool UserCenterModule::Init(CContainerUI* pLeft,  CContainerUI* pContainer )
{
	this->_pController->Init(pLeft);
	return true;
}

UserCenterModule* UserCenterModule::GetInstance()
{
	if(_pInstance == NULL)
		_pInstance = new UserCenterModule();

	return _pInstance;
}
