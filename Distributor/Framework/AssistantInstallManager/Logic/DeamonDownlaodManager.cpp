#include "StdAfx.h"
#include "DeamonDownlaodManager.h"
#include "IOSDeamonInstall.h"
#include "AndroidDeamonInstall.h"
#include "Thread/DeamonDownlaodThread.h"



static DeamonDownloadManager* _pManager = NULL;

DeamonDownloadManager* DeamonDownloadManager::GetInstance()
{
	if (_pManager == NULL)
	_pManager = new DeamonDownloadManager;
	return _pManager;
}

DeamonDownloadManager::DeamonDownloadManager()
{
	_pIosDeamon	= NULL;
	_pLegalAssist= NULL;
	_pIosAssist	= NULL;
	_pIpadAssist= NULL;
	_pAndAssist	= NULL;
}

DeamonDownloadManager::~DeamonDownloadManager(void)
{
	SAFE_DELETE(_pIosDeamon);
	SAFE_DELETE(_pLegalAssist);
	SAFE_DELETE(_pIosAssist);
	SAFE_DELETE(_pIpadAssist);
	SAFE_DELETE(_pAndAssist);
}


bool DeamonDownloadManager::DownloadDeamons()
{
	bool success = false;

	_pIosDeamon = new CIosDeamonInstall(L"");
	success = StartDownload(_pIosDeamon, true);					// 下载IOS守护

	_pLegalAssist = new CIosDeamonInstall(L"");
	success = StartDownload(_pLegalAssist, false);				// 下载正版助手

	_pIosAssist = new CIosDeamonInstall(L"");
	success = StartDownload(_pIosAssist, false, true);			// 下载IOS助手

	_pIpadAssist = new CIosDeamonInstall(L"");
	success = StartDownload(_pIpadAssist, false, true, true);	// 下载Ipad助手


	_pAndAssist = new CAndroidDeamonInstall(L"");
	success = StartDownload(_pAndAssist, false);

	_pOfficialAssist = new CAndroidDeamonInstall(L"");
	success = StartDownload(_pOfficialAssist, false, true);
		
	return success;
}

bool DeamonDownloadManager::StartDownload( CDeamonInstall* pInstall, bool isDeamon, bool isCpa, bool isJailBreaked, bool isPad, int nDeviceVer )
{
	DeamonDownlaodThread* pThread = new DeamonDownlaodThread( "", pInstall, isDeamon, isCpa, isJailBreaked, isPad, nDeviceVer);

	return pThread->Run();
}

CDeamonInstall* DeamonDownloadManager::GetTaskType( bool isDeamon, bool isIos, bool isJailBreaked, bool isPad, bool isCpa )
{
	if ( isDeamon )
	{
		return _pIosDeamon;
	}
	else if ( isCpa )
	{
		return _pOfficialAssist;
	}
	else
	{
		return isIos ? !isJailBreaked ? _pLegalAssist : isPad ? _pIpadAssist : _pIosAssist : _pAndAssist ;
	}
}

bool DeamonDownloadManager::Handle( const BeginDownLoadDeamonMsg* pMsg )
{
	DownloadDeamons();
	return true;
}