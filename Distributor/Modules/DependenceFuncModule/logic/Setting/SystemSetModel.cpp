#include "StdAfx.h"
#include "SystemSetModel.h"
#include "Common/SettingCfg.h"

SystemSetModel::SystemSetModel(SystemSetInterface *pInterface)
:_pInterface(pInterface)
{INTERFUN;

}

SystemSetModel::~SystemSetModel()
{INTERFUN;

}

bool SystemSetModel::Init()
{INTERFUN;
	_pInterface->SetCheck(_T("ContactPerson"),CONFIG(_bAppStart));
	
	if (CONFIG(_bAppClose))
		_pInterface->SetRadio(_T("ExitApp"));
	else
		_pInterface->SetRadio(_T("Minimize_to_tray"));

	_pInterface->SetCheck(_T("DownLoadMsg"),CONFIG(_bDownLoadMsg));

	_pInterface->SetCheck(_T("SoftInstallMsg"),CONFIG(_bSoftInstallMsg));

	_pInterface->SetCheck(_T("AutoDeleteTask"),CONFIG(_bAutoDeleteTask));
	return true;
}

void SystemSetModel::Start(bool flag)
{INTERFUN;
	HKEY hKey; 
	//找到系统的启动项 
	LPCTSTR lpRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"); 
	//打开启动项Key 
	long lRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey); 
	if(lRet != ERROR_SUCCESS) 
		return;
	
	if (flag)
	{
		//写入注册表,开机自启动 
			TCHAR pFileName[MAX_PATH] = {0}; 
			//得到程序自身的全路径 
			DWORD dwRet = GetModuleFileNameW(NULL, pFileName, MAX_PATH); 
			//添加一个子Key,并设置值 // 下面的"getip"是应用程序名字（不加后缀.exe）
			//转成byte之后无法识别字符个数，所以字符数需要 *2
			lRet = RegSetValueExW(hKey, _T("91Distributor"), 0, REG_SZ, (BYTE *)pFileName, dwRet*2); 
	}
	else
	{
		RegSetValueExW(hKey, _T("91Distributor"), 0, REG_SZ, (BYTE *)_T(""), 0); 
	}
	//关闭注册表 
	RegCloseKey(hKey); 

}

void SystemSetModel::SaveSetting()
{INTERFUN;
	CONFIG(_bAppStart) = _pInterface->GetCheck(_T("ContactPerson"));
	Start(_pInterface->GetCheck(_T("ContactPerson")));

	CONFIG(_bAppClose) = _pInterface->GetRadio(_T("ExitApp"));

	CONFIG(_bDownLoadMsg) = _pInterface->GetCheck(_T("DownLoadMsg"));

	CONFIG(_bSoftInstallMsg) = _pInterface->GetCheck(_T("SoftInstallMsg"));

	CONFIG(_bAutoDeleteTask) = _pInterface->GetCheck(_T("AutoDeleteTask"));
}
