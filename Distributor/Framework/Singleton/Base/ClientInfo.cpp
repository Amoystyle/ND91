#include "StdAfx.h"
#include "ClientInfo.h"
#include "VersionCheck.h"

CClientInfo::CClientInfo(void)
{
	_version = new VersionCheck();
	_appId = 100571;
	_appKey = "3cdbd2244fb7d781cf4c8938a5d3eb5413e35d1b626c787b";
}

CClientInfo::~CClientInfo(void)
{
}

CClientInfo* CClientInfo::GetInstance()
{
	if (NULL == _instance)
	{
		CSLock_Ext lockCS(CClientInfo::_lock->GetLock());
		if (NULL == _instance)
		{
			_instance = new CClientInfo;
		}
	}
	return _instance;
}

string CClientInfo::GetChangleId()
{
	string strData = CFileOperation::ReadFile(CFileOperation::GetCurrWorkingDir() + L"Tqwavlib.dll");
	strData = CStrOperation::Replace(strData, "\t", "");
	strData = CStrOperation::Replace(strData, "\r", "");
	strData = CStrOperation::Replace(strData, "\n", "");
	strData = CStrOperation::Replace(strData, " ", "");
	return strData;
}

string CClientInfo::GetVersion()
{
	string verTmp = "";
	verTmp = CCodeOperation::ws2s(_version->GetNowVersion());
	return verTmp;
}

string CClientInfo::GetIniVersion()
{
	TCHAR ini_version[128] = _T("");
	wstring new_version = _T("");
	GetPrivateProfileStringW (_T("General"), _T("ProductLastVersion"),NULL, ini_version, 128, (CFileOperation::GetCurrWorkingDir() + _T("ProductCfg.ini")).c_str()); 
	new_version = ini_version;
	if (new_version.length() == 0)
	{
	}
	return CCodeOperation::UnicodeToGB2312(new_version);
}

int CClientInfo::GetAppId()
{
	return _appId;
}

string CClientInfo::GetAppKey()
{
	return _appKey;
}


BaseCSLock* CClientInfo::_lock = new BaseCSLock();

CClientInfo* CClientInfo::_instance = NULL;
