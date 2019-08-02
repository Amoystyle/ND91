#include "stdafx.h"
#include "Config.h"
#include "DataPath.h"
CConfig* CConfig::_pInstance = NULL;
CConfig* CConfig::GetInstance()
{
	if (_pInstance)
		return _pInstance;
	_pInstance = new CConfig();
	return _pInstance;
}

void CConfig::Release()
{
	if (_pInstance)
		delete _pInstance;
	_pInstance = NULL;
}

CConfig::CConfig()
{
    LoadDefault();
}

CConfig::~CConfig()
{

}

void CConfig::LoadDefault()
{
    _strUserDataPath = DataPath::GetDataDir() + L"\\"; 
    _strWorkPath = CFileOperation::GetCurrWorkingDir();
    _strTemp = DataPath::GetTempDir() + L"\\"; 
    _strDumpPath = _strUserDataPath + L"Dump\\"; 
    _strStatisticPath = _strUserDataPath + L"Statistic\\"; 
    _strOptionIni = _strWorkPath + L"Option.ini";
    _strDriverPath = GetIniFileValue(_strOptionIni, _T("Common"), _T("DriverPath"));
    if (_strDriverPath.empty())
        _strDriverPath = _strUserDataPath + L"Driver\\";
    _strInfoFile = _strUserDataPath + L"info.ini";
    _strDownTempPath = _strUserDataPath + L"91DownloadTemp\\";
    _strBackupPath = _strUserDataPath + L"Backup\\";
    _strDownloadPath = _strUserDataPath + L"Download\\";
    _strDevicePhotoPath = _strUserDataPath + L"DevicePhoto\\";
    _strSoftwarePath = _strUserDataPath + L"Software\\";
    _strAssistPath = _strDownloadPath + L"Assistant\\";
    _strUpdatePath = _strUserDataPath + L"Update\\";
    _strIconPath = _strUserDataPath + L"AppIcons\\";
	_strWebLegalSoft = _strUserDataPath + L"WebLegalSoft\\";
	_strCachePath = _strUserDataPath + L"Cache\\";
    _strOptionIni = _strWorkPath + L"Option.ini";
    _strUserSettingIni = _strUserDataPath + L"UserSetting.ini";
    _strLogPath = _strUserDataPath + L"Logs\\";

	_strDriverInstallIni = _strWorkPath + L"DriverInstallList.ini";
	_strDriverInstallXML = _strWorkPath + L"DriverInstallList.xml";
	_strTrayIconPath = _strWorkPath + L"Resource\\DependenceFuncCenter\\res\\Images\\MsgIcon.png";

	_strBizDictionaryIni = _strUserDataPath + L"UserSetting.ini";
	_strSHSHPath = _strUserDataPath + L"DownloadCenter\\SHSH";

	_strIos_CpaDownPath = _strUserDataPath+ + L"DownloadCenter\\Software\\CPA\\iOS\\";
	_strAndroid_CpaDownPath = _strUserDataPath+ + L"DownloadCenter\\Software\\CPA\\Android\\";

    LoadOption(_strOptionIni);
	LoadUserSetting();
}

void CConfig::LoadOption( wstring strIniFilePath )
{
    _bEnableAndroid = GetIniFileValue(strIniFilePath, L"Android", L"enable") == L"0" ? false : true;
    _bEnableIos = true;
	_bFirstTimeRun = GetIniFileValue(_strUserSettingIni, L"Default", L"FirstTimeRun") == L"1" ? false:true;
	wstring webnewstr=GetIniFileValue(_strUserSettingIni, L"Default", L"ShowWebNew");
	_bShowWebNew = (webnewstr.empty()||webnewstr == L"1") ? true:false;
}
void CConfig::SetFirstTimeRun( bool val )
{
	wstring value=(val==true?L"1":L"0");
	SetIniFileValue(_strUserSettingIni,L"Default", L"FirstTimeRun",value);
}

bool CConfig::GetFirstTimeRun()
{
	wstring firstTime = GetIniFileValue(_strUserSettingIni,L"Default", L"FirstTimeRun");

	return _wtoi(firstTime.c_str()) == 1;
}

bool CConfig::GetShowWebNew()
{
	wstring showwebnew = GetIniFileValue(_strUserSettingIni,L"Default", L"ShowWebNew");
	_bShowWebNew = (showwebnew.empty()||showwebnew == L"1") ? true:false;
	return _bShowWebNew;
}

void CConfig::SetShowWebNew( bool val )
{
	_bShowWebNew = val;
	SetIniFileValue(_strUserSettingIni,L"Default", L"ShowWebNew",val?L"1":L"0");
}

//设置下载路径
void CConfig::SetDownLoadPath( int platform, wstring downpath )
{
	if( 0==platform )
	{
		_strIosDownPath = downpath;
		
	}
	else
	{
		_strAndroidDownPath = downpath;
	}
} 

//设置是否自动删除已经完成任务
void CConfig::SetAutoDeleteTask( bool del )
{
	wstring value=(del==true?L"1":L"0");
	_bAutoDeleteTask = del;
	SetIniFileValue(_strUserSettingIni,L"Default", L"AutoDeleteTask",value);
}

//加载用户设置属性
void CConfig::LoadUserSetting()
{
	wstring group = L"Setting";
	wstring ioskey = L"IOSDownPath";
	wstring androidkey = L"AndroidDownPath";
	wstring appstartkey = L"AppStart";
	wstring appclosekey = L"AppClose";
	wstring downloadkey = L"DownLoadMsg";
	wstring softinstallkey = L"SoftInstallMsg";
	wstring autodeletetaskkey = L"AutoDeleteTask";
	wstring autodelupdatedappkey = L"AutoDelUpdatedApp";
	_bAppStart  = _wtoi(GetIniFileValue(_strUserSettingIni,group, appstartkey).c_str()) == 1;
	_bAppClose  = _wtoi(GetIniFileValue(_strUserSettingIni,group, appclosekey).c_str()) == 1;
	_bDownLoadMsg = _wtoi(GetIniFileValue(_strUserSettingIni,group, downloadkey).c_str()) == 1;
	_bSoftInstallMsg = _wtoi(GetIniFileValue(_strUserSettingIni,group, softinstallkey).c_str()) == 1;
	wstring autodeletetask = GetIniFileValue(_strUserSettingIni,group, autodeletetaskkey);
	wstring autodelupdatedapp = GetIniFileValue(_strUserSettingIni,group, autodelupdatedappkey);
	_bAutoDeleteTask = autodeletetask.length()==0 ? true:_wtoi(autodeletetask.c_str()) == 1;
	_bAutoDelUpdatedApp = autodelupdatedapp.length()==0 ? true:_wtoi(autodelupdatedapp.c_str()) == 1;

	wstring iosdownpath = GetIniFileValue(_strUserSettingIni.c_str(), group, ioskey);
	wstring androiddownpath = GetIniFileValue(_strUserSettingIni.c_str(), group, androidkey);
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
	if(!CFileOperation::IsDirExist(_strIos_CpaDownPath.c_str()))
		CFileOperation::MakeLocalDir(_strIos_CpaDownPath);
	if(!CFileOperation::IsDirExist(_strAndroid_CpaDownPath.c_str()))
		CFileOperation::MakeLocalDir(_strAndroid_CpaDownPath);
}


std::wstring CConfig::GetIniFileValue( wstring strIniFile, wstring strGroup, wstring strKey )
{
	TCHAR buf[1048];
	GetPrivateProfileStringW(strGroup.c_str(), strKey.c_str(), _T(""), buf, 1048, strIniFile.c_str());
	wstring strValue(buf);
	return strValue;
}

bool CConfig::SetIniFileValue( wstring strIniFile, wstring strGroup, wstring strKey, wstring strValue )
{
	return (WritePrivateProfileStringW(strGroup.c_str(), strKey.c_str(), strValue.c_str(), strIniFile.c_str()) == TRUE);
}