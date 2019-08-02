#include "stdafx.h"

#include "ThemePath.h"
#include "Common/Path.h"

ThemePath::ThemePath(const wstring workFolder)
: _workFolder(workFolder)
{
}

wstring ThemePath::GetPath() const
{
    //wstring strPath = _workFolder + L"Theme\\";
    wstring strPath = _workFolder + L"Theme" + WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}

wstring ThemePath::GetPreviewPath() const
{
	//wstring strPath = _workFolder + L"Theme\\Preview\\";
    wstring strPath = _workFolder + L"Theme" + WSTRING_PATH_SEPARATER + L"Preview" + WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}

wstring ThemePath::GetThemeTempPath() const
{
	//wstring strPath = _workFolder + L"Theme\\Temp\\";
    wstring strPath = _workFolder + L"Theme" + WSTRING_PATH_SEPARATER + L"Temp" + WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}

wstring ThemePath::GetAdpPath() const
{
	//wstring strPath = _workFolder + L"Theme\\AddApt\\";
    wstring strPath = _workFolder + L"Theme" + WSTRING_PATH_SEPARATER + L"AddApt" + WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}

std::wstring ND91Assistant::ThemePath::GetUploadPath() const
{
	//wstring strPath = _workFolder + L"Theme\\ThemeUpload\\";
    wstring strPath = _workFolder + L"Theme" + WSTRING_PATH_SEPARATER + L"ThemeUpload" + WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}
