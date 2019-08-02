#include "stdafx.h"
#include "SettingCfg.h"

SettingCfg* SettingCfg::_pInstance = NULL;
SettingCfg* SettingCfg::GetInstance()
{INTERFUN;
	if (_pInstance)
		return _pInstance;
	_pInstance = new SettingCfg();
	return _pInstance;
}

void SettingCfg::Release()
{INTERFUN;
	if (_pInstance)
		delete _pInstance;
	_pInstance = NULL;
}

SettingCfg::SettingCfg()
{INTERFUN;

}

SettingCfg::~SettingCfg()
{INTERFUN;
	
}

void SettingCfg::WriteIni()
{INTERFUN;
	wstring start = CONFIG(_bAppStart) ? L"1" : L"0";
	wstring close = CONFIG(_bAppClose) ? L"1" : L"0";
	wstring downLoadMsg = CONFIG(_bDownLoadMsg) ? L"1" : L"0";
	wstring softInstallMsg = CONFIG(_bSoftInstallMsg) ? L"1" : L"0";
	wstring autoDeleteTask = CONFIG(_bAutoDeleteTask) ? L"1" : L"0";
	wstring autoDelUpdatedApp = CONFIG(_bAutoDelUpdatedApp) ? L"1" : L"0";
	WritePrivateProfileString(_T("Setting"),_T("AppStart"),start.c_str(),CONFIG(_strUserSettingIni).c_str());
	WritePrivateProfileString(_T("Setting"),_T("AppClose"),close.c_str(),CONFIG(_strUserSettingIni).c_str());
	WritePrivateProfileString(_T("Setting"),_T("DownLoadMsg"),downLoadMsg.c_str(),CONFIG(_strUserSettingIni).c_str());
	WritePrivateProfileString(_T("Setting"),_T("SoftInstallMsg"),softInstallMsg.c_str(),CONFIG(_strUserSettingIni).c_str());
	WritePrivateProfileString(_T("Setting"),_T("AutoDeleteTask"),autoDeleteTask.c_str(),CONFIG(_strUserSettingIni).c_str());
	WritePrivateProfileString(_T("Setting"),_T("IOSDownPath"),CONFIG(_strIosDownPath).c_str(),CONFIG(_strUserSettingIni).c_str());
	WritePrivateProfileString(_T("Setting"),_T("AndroidDownPath"),CONFIG(_strAndroidDownPath).c_str(),CONFIG(_strUserSettingIni).c_str());
	WritePrivateProfileString(_T("Setting"),_T("AutoDelUpdatedApp"),autoDelUpdatedApp.c_str(),CONFIG(_strUserSettingIni).c_str());
}
