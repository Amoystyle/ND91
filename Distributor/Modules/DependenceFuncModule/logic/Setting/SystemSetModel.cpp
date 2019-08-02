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
	//�ҵ�ϵͳ�������� 
	LPCTSTR lpRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"); 
	//��������Key 
	long lRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey); 
	if(lRet != ERROR_SUCCESS) 
		return;
	
	if (flag)
	{
		//д��ע���,���������� 
			TCHAR pFileName[MAX_PATH] = {0}; 
			//�õ����������ȫ·�� 
			DWORD dwRet = GetModuleFileNameW(NULL, pFileName, MAX_PATH); 
			//���һ����Key,������ֵ // �����"getip"��Ӧ�ó������֣����Ӻ�׺.exe��
			//ת��byte֮���޷�ʶ���ַ������������ַ�����Ҫ *2
			lRet = RegSetValueExW(hKey, _T("91Distributor"), 0, REG_SZ, (BYTE *)pFileName, dwRet*2); 
	}
	else
	{
		RegSetValueExW(hKey, _T("91Distributor"), 0, REG_SZ, (BYTE *)_T(""), 0); 
	}
	//�ر�ע��� 
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
