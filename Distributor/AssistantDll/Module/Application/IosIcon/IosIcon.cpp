#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "Module/Application/IosIcon/IosIcon.h"

IosIcon::IosIcon(void)
{
	_strDisPlayName = "";
	_iconModeDate = 0;
	_strBundleIdentifier = "";
	_strDisplayIndentifier = "";
	_strIconImgFile = L"";
}

IosIcon::~IosIcon(void)
{
	if (_strIconImgFile.length() > 0 && CFileOperation::IsFileExist(_strIconImgFile.c_str()))
        CFileOperation::DeleteFile(_strIconImgFile);
}

#endif