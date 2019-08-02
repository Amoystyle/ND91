#include "StdAfx.h"
#include "DeamonInstall.h"
// #include "AndroidDeamonInstall.h"
// #include "IOSDeamonInstall.h"



CDeamonInstall::CDeamonInstall(wstring deviceId)
{

	_InstallState	= INSTALLSTATE_WAITTING;
	_deviceId		= deviceId;
	_pDevData		= NULL;
	_bHasInit		= false;
	_bDownLoadFinish= false;

	_bLogin			= false;
	_bCpaUser		= false;


	_strServiceVer.clear();
	_strServiceUrl.clear();


	if ( ! _deviceId.empty() )
	{
		_pDevData = DEVICE_MANAGER->FindDevice(deviceId);
	}

}

CDeamonInstall::~CDeamonInstall()
{

	_bHasInit		= false;
	_bDownLoadFinish= false;

	_bLogin			= false;
	_bCpaUser		= false;

}

void CDeamonInstall::Init()
{

	if (!_bHasInit)
	{
		_strIniPath = CONFIG(_strAssistPath) + L"config.ini";

		// IOS存放总路径
		_strIosDir = CONFIG(_strAssistPath) + L"IOS\\";
		if (!CFileOperation::IsDirExist(_strIosDir.c_str()))
		{
			CFileOperation::MakeLocalDir(_strIosDir);
		}
		
		// IOS守护存放总路径
		_strIosDeamonDir = CONFIG(_strAssistPath) + L"IOS\\Deamon\\";
		if (!CFileOperation::IsDirExist(_strIosDeamonDir.c_str()))
		{
			CFileOperation::MakeLocalDir(_strIosDeamonDir);
		}

		// IOS助手存放总路径
		_strIosAssistantDir = CONFIG(_strAssistPath) + L"IOS\\Assistant\\";
		if (!CFileOperation::IsDirExist(_strIosAssistantDir.c_str()))
		{
			CFileOperation::MakeLocalDir(_strIosAssistantDir);
		}
		
		_strDeamonPath			= _strIosDeamonDir + L"DeamonPackage.zip";	

		_strLegalAssistPath		= _strIosAssistantDir + L"91LegalPandaSpace.ipa";
		_strIphoneAssistPath	= _strIosAssistantDir + L"91IosPandaSpace.ipa";		
		_strIpadAssistantPath	= _strIosAssistantDir + L"91IpadPandaSpace.ipa";

		
		_strAndDir = CONFIG(_strAssistPath) + L"ANDROID\\";
		if (!CFileOperation::IsDirExist(_strAndDir.c_str()))
		{
			CFileOperation::MakeLocalDir(_strAndDir);
		}

		_strAndAssistantDir = CONFIG(_strAssistPath) + L"ANDROID\\Assistant\\";
		if (!CFileOperation::IsDirExist(_strAndAssistantDir.c_str()))
		{
			CFileOperation::MakeLocalDir(_strAndAssistantDir);
		}

		_strAssistantAndPath	= _strAndAssistantDir + L"91AndroidPandaSpace.apk" ;	
		_strAssOfficialAndPath	= _strAndAssistantDir + L"91AndroidOfficialPs.apk" ;

		_bHasInit = true;
	}

}

bool CDeamonInstall::CompareVersion(vector<wstring> LocalVersionList,vector<wstring> serviceVersionList,size_t pos)
{
	int v1 = 0 , v2 = 0;
	if (LocalVersionList.size() > pos)
	{
		v1 = _wtoi( LocalVersionList.at(pos).c_str());
		if (serviceVersionList.size() > pos)
		{
			v2 = _wtoi( serviceVersionList.at(pos).c_str());
		}

		if (v1 < v2)
		{
			return false;
		}
		else
		{
			pos = pos + 1;
			return CompareVersion(LocalVersionList,serviceVersionList,pos);
		}
	}
	return true;
}

void CDeamonInstall::SetUserInfo( bool bLogin, bool bCpaUser, std::string channelID )
{
	_bLogin			= bLogin;
	_bCpaUser		= bCpaUser;
	_strChannelID	= channelID;
}