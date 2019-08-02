#include "stdafx.h"
#include "FileManagerModule.h"
#include "Logic/FileManagerController.h"

FILEMANAGER_API DeviceModule* CreateFileManagerModule( wstring deviceID, void* phone )
{
	return new FileManagerModule(deviceID, phone);
}

FileManagerModule::FileManagerModule(wstring deviceID, void* phone) : DeviceModule(deviceID, phone)
{
	_pController = new FileManagerController(deviceID);
}

FileManagerModule::~FileManagerModule()
{

}

tstring FileManagerModule::Id()
{
	return L"FileManagerModule";
}

tstring FileManagerModule::Name()
{
	return CommonUIFunc::LoadString(L"FileManager_Text_ModuleText");
}

tstring FileManagerModule::Icon()
{
	return L"#FileManager_Image_ModuleIcon";
}

int FileManagerModule::LayoutIndex()
{
	return 0;
}

void FileManagerModule::Show()
{
	_pController->Show();
}

void FileManagerModule::Hide()
{
	_pController->Hide();
}

bool FileManagerModule::Init(CContainerUI* pContainer)
{
	_pController->Init(pContainer);
	return true;
}

void FileManagerModule::Release()
{
	SAFE_DELETE(this->_pController);
}
