#include "stdafx.h"
#include <shlobj.h>
#include "WebResourceModuleConfig.h"
WebResourceModuleCConfig* WebResourceModuleCConfig::_pInstance = NULL;
WebResourceModuleCConfig* WebResourceModuleCConfig::GetInstance()
{
	if (_pInstance)
		return _pInstance;
	_pInstance = new WebResourceModuleCConfig();
	return _pInstance;
}

void WebResourceModuleCConfig::Release()
{
	if (_pInstance)
		delete _pInstance;
	_pInstance = NULL;
}

WebResourceModuleCConfig::WebResourceModuleCConfig()
{
    LoadDefault();
}

WebResourceModuleCConfig::~WebResourceModuleCConfig()
{

}

void WebResourceModuleCConfig::LoadDefault()
{
	TCHAR szPath[MAX_PATH];
	HRESULT ret = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, szPath);
	if ( !SUCCEEDED(ret) )
		return;
		
    _strUserDataPath = szPath;
    _strUserDataPath += L"\\"; //MyFileOperation::GetMyDocumentPath() + L"\\" + APP_NAME + L"\\"; 
   
	_strWebLegalSoft = _strUserDataPath + L"WebLegalSoft\\";
	_strUserSettingIni = _strUserDataPath + L"UserSetting.ini";
	

	LoadUserSetting();
}

void WebResourceModuleCConfig::LoadUserSetting()
{
	wstring group = L"Setting";
	wstring ioskey = L"IOSDownPath";
	wstring androidkey = L"AndroidDownPath";

	wstring iosdownpath = MyFileOperation::GetIniFileValue(_strUserSettingIni.c_str(), group, ioskey);
	wstring androiddownpath = MyFileOperation::GetIniFileValue(_strUserSettingIni.c_str(), group, androidkey);
	_strIosDownPath = iosdownpath;
	if( L""== _strIosDownPath )
		_strIosDownPath = _strUserDataPath + L"DownloadCenter\\Software\\iOS\\";

	_strAndroidDownPath = androiddownpath;
	if( L""== _strAndroidDownPath )
		_strAndroidDownPath = _strUserDataPath + L"DownloadCenter\\Software\\Android\\";


	if(!CFileOperation::IsDirExist(_strIosDownPath.c_str()))
		CFileOperation::MakeLocalDir(_strIosDownPath);
	if(!CFileOperation::IsDirExist(_strAndroidDownPath.c_str()))
		CFileOperation::MakeLocalDir(_strAndroidDownPath);

	_strbatchdownloadPath = _strUserDataPath + L"NetDragon\\91Distributor\\DownloadCenter\\XML\\";
}
