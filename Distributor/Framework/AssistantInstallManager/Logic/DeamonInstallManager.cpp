#include "StdAfx.h"
#include "DeamonInstallManager.h"
#include "IOSDeamonInstall.h"
#include "AndroidDeamonInstall.h"
#include "Thread/DeamonInstallThread.h"
#include "Common/UserCenter/UserCenterMsg.h"


static DeamonInstallManager* _pManager = NULL;

DeamonInstallManager* DeamonInstallManager::GetInstance()
{
	if (_pManager == NULL)
	_pManager = new DeamonInstallManager;
	return _pManager;
}


DeamonInstallManager::DeamonInstallManager()
{
	_bInstallFinish = false;
	_mapDeviceHasInstall.clear();
}

DeamonInstallManager::~DeamonInstallManager(void)
{
	_bInstallFinish = false;
}

bool DeamonInstallManager::InstallDeamon( wstring deviceId, bool bAppSync )
{
	DeviceData* pData = DEVICE_MANAGER->FindDevice(deviceId);
	RETURN_FALSE_IF ( !pData );
	CDeamonInstall* pInstall = NULL;

	GetUserLoginInfoMsg msg;
	msg.SendAndGetReturn();
		
	if ( pData->GetDevInfo()->_deviceType == Ios )
	{
		pInstall = new CIosDeamonInstall( pData->GetId() );
	}
	else
	{
		pInstall = new CAndroidDeamonInstall( pData->GetId() );	
	}
	pInstall->Init();
	pInstall->SetUserInfo(msg.bLongin, msg.bCpaUser, msg.channelID);

	DeamonInstallThread* pThread = new DeamonInstallThread("", pInstall, pData, msg.bLongin&&msg.bCpaUser, bAppSync);
	return pThread->Run();

}

void DeamonInstallManager::RecordTask( wstring strID, bool bsuccess )
{
	AutoLock;
	_mapDeviceHasInstall[strID] = bsuccess;
}

bool DeamonInstallManager::HasInstall( wstring strID )
{
	AutoLock;
	RETURN_FALSE_IF ( _mapDeviceHasInstall.find(strID) == _mapDeviceHasInstall.end() );

	return true;
}

bool DeamonInstallManager::Handle( const BeginInstallDeamonMsg* pMsg )
{
	RETURN_FALSE_IF ( pMsg->deviceID.empty() );
	InstallDeamon( pMsg->deviceID, pMsg->bAppSync );
	return true;
}