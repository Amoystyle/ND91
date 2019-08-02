#include "StdAfx.h"
#include "DeamonDownlaodThread.h"
#include "../DeamonInstall.h"
#include "Base/BaseCSLock.h"

DeamonDownlaodThread::DeamonDownlaodThread(const string threadId, CDeamonInstall* pInstall, bool isDeamon, bool isCpa, bool isJailBreaked, bool isPad, int nDeviceVer)
:BaseThread(threadId)
{
	_pInstall	= pInstall;
	_isDeamon	= isDeamon;	
	_isPad		= isPad;
	_nDeviceVer	= nDeviceVer;
	_isJailBreaked = isJailBreaked;
	_isCpa		= isCpa;
}

DeamonDownlaodThread::~DeamonDownlaodThread(void)
{

}

unsigned long DeamonDownlaodThread::Main()
{
	unsigned long nResult = 0;

	RETURN_VALUE_IF ( !_pInstall, nResult );

	_pInstall->Init();

	if ( _isDeamon )
	{
		/*if ( !_pInstall->CheckDeamonFilesPerfect(_nDeviceVer) )
		{
			nResult = _pInstall->DownloadDeamon() ? 0 : -1;
		}*/
	}
	/*else
	{
		if ( !_pInstall->CheckAssistFilesPerfect(_nDeviceVer,_isJailBreaked,_isPad,_isCpa) )
		{
			nResult = _pInstall->DownloadAssistant( _isJailBreaked, _isPad, _isCpa ) ? 0 : -1;
		}	
	}*/

	return 0;
}
