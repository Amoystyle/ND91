#include "stdafx.h"
#include "DeamonInstallThread.h"
#include "Logic/DeamonInstall.h"
#include "../DeamonDownlaodManager.h"
#include "../DeamonInstallManager.h"
#include "Common/UserCenter/UserCenterMsg.h"

DeamonInstallThread::DeamonInstallThread( const string id, CDeamonInstall* pInstall, DeviceData* pData, bool bNeedChange, bool bAppSync )
:BaseThread(id)
{
	_pInstall	= pInstall;
	_pData		= pData;
	_bNeedChange= bNeedChange;
	_bAppSync	= bAppSync;
}

DeamonInstallThread::~DeamonInstallThread()
{

}

unsigned long DeamonInstallThread::Main()
{
	unsigned long nResult = 0;

 	bool bInstall = DeamonInstallManager::GetInstance()->HasInstall(_pData->GetId()); 
	
	if ( !bInstall )
	{
		if ( !_pInstall->NeedDeamonInstall() )
		{
			CDeamonInstall* pDeamon = DeamonDownloadManager::GetInstance()->GetTaskType(true, _pData->GetDevInfo()->_deviceType == Ios, _pData->GetDevInfo()->_ios_bJailBreaked, _pData->GetDevInfo()->IsPad() );

			while( true )
			{
				DeviceData* pData = DEVICE_MANAGER->FindDevice(_pData->GetId());
				RETURN_FALSE_IF ( !pData );

				if ( pDeamon->_bDownLoadFinish )
				{
					nResult = _pInstall->InstallDeamon() == INSTALLSTATE_COMPLETED ? 0 : 1;

					if ( !nResult )
					{	
						DeamonInstallManager::GetInstance()->RecordTask( _pData->GetId(), true );

						if ( _bAppSync && _pData->GetDevInfo()->_deviceType == Ios )
						{
							RequestInstallDeamonOverMsg msg;
							msg.deviceID = _pData->GetId();
							msg.SendToUI();
						}
					}
					break;
				}
				Sleep(200);
			}
		}
		else
		{
			if ( _bAppSync && _pData->GetDevInfo()->_deviceType == Ios )
			{
				RequestInstallDeamonOverMsg msg;
				msg.deviceID = _pData->GetId();
				msg.SendToUI();
			}
		}
	}
	else
	{
		if ( _bAppSync && _pData->GetDevInfo()->_deviceType == Ios )
		{
			RequestInstallDeamonOverMsg msg;
			msg.deviceID = _pData->GetId();
			msg.SendToUI();
		}
	}

	if ( !_pInstall->NeedAssistInstall() )
	{
	
		CDeamonInstall* pAssist = DeamonDownloadManager::GetInstance()->GetTaskType(false, _pData->GetDevInfo()->_deviceType == Ios, _pData->GetDevInfo()->_ios_bJailBreaked, _pData->GetDevInfo()->IsPad(), _bNeedChange );

		while( true )
		{
			DeviceData* pData = DEVICE_MANAGER->FindDevice(_pData->GetId());
			RETURN_FALSE_IF ( !pData );

			if ( pAssist->_bDownLoadFinish )
			{
				nResult = _pInstall->InstallAssistant() == INSTALLSTATE_COMPLETED ? 0 : 1;

				if ( !nResult )
					DeamonInstallManager::GetInstance()->_bInstallFinish = true;				 
				break;
			}
			Sleep(200);
		}
	}

	return nResult;
}