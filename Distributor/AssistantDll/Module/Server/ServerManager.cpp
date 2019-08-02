#include "stdafx.h"

#ifdef MODULE_SERVER

#include "Module/Server/ServerManager.h"
#include "Core/DeviceData.h"


#include "Module/Server/GetAppUpdateInfo.h"		
#include "Module/Server/GetDeviceImageInfo.h"		
#include "Module/Server/GetIPhoneSellInfo.h"	
#include "Module/Server/GetPackageInfoforAndroid.h"
#include "Module/Server/GetPackageInfoforDeamon.h"	
#include "Module/Server/GetPackageInfoforIos.h"


ServerManager::ServerManager(const DeviceData* pDevice)
: IManager (pDevice)
{
	
	_pGetAppUpdateInfo			= NULL;
	_pGetDeviceImageInfo		= NULL;
	_pGetIPhoneSellInfo			= NULL;
	_pGetPackageInfoforAndroid	= NULL;
	_pGetPackageInfoforDeamon	= NULL;
	_pGetPackageInfoforIos		= NULL;

}

ServerManager::~ServerManager()
{
	SAFE_DELETE(_pGetAppUpdateInfo);
	SAFE_DELETE(_pGetDeviceImageInfo);
	SAFE_DELETE(_pGetIPhoneSellInfo);
	SAFE_DELETE(_pGetPackageInfoforAndroid);
	SAFE_DELETE(_pGetPackageInfoforDeamon);
	SAFE_DELETE(_pGetPackageInfoforIos);
}


CGetAppUpdateInfo* ServerManager::GetAppUpdateInfoManager()
{
	if ( !_pGetAppUpdateInfo )
		_pGetAppUpdateInfo = new CGetAppUpdateInfo((DeviceData*)_pDevice);
	
	return _pGetAppUpdateInfo;
}

CGetDeviceImageInfo* ServerManager::GetDeviceImageInfoManager()
{
	if ( !_pGetDeviceImageInfo )
		_pGetDeviceImageInfo = new CGetDeviceImageInfo((DeviceData*)_pDevice);

	return _pGetDeviceImageInfo;
}

CGetIPhoneSellInfo* ServerManager::GetIPhoneSellInfoManager()
{
	if ( !_pGetIPhoneSellInfo )
		_pGetIPhoneSellInfo = new CGetIPhoneSellInfo((DeviceData*)_pDevice);

	return _pGetIPhoneSellInfo;
}

CGetPackageInfoforAndroid* ServerManager::GetPackageInfoforAndroidManager()
{
	if ( !_pGetPackageInfoforAndroid )
		_pGetPackageInfoforAndroid = new CGetPackageInfoforAndroid((DeviceData*)_pDevice);

	return _pGetPackageInfoforAndroid;
}

CGetPackageInfoforDeamon* ServerManager::GetPackageInfoforDeamonManager()
{
	if ( !_pGetPackageInfoforDeamon )
		_pGetPackageInfoforDeamon = new CGetPackageInfoforDeamon((DeviceData*)_pDevice);

	return _pGetPackageInfoforDeamon;
}

CGetPackageInfoforIos* ServerManager::GetPackageInfoforIosManager()
{
	if ( !_pGetPackageInfoforIos )
		_pGetPackageInfoforIos = new CGetPackageInfoforIos((DeviceData*)_pDevice);

	return _pGetPackageInfoforIos;
}

#endif