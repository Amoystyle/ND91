#include "stdafx.h"
#include "CheckCanInstallAppsync.h"

CheckCanInstallAppsync::CheckCanInstallAppsync()
{

}

CheckCanInstallAppsync::~CheckCanInstallAppsync()
{

}

bool CheckCanInstallAppsync::Install(wstring sn, wstring& errorMsg)
{
	DeviceData* pDevice = DeviceManager::GetInstance()->FindDevice(sn);
	if(pDevice == NULL)
		return false;

	wstring com_saurik_substrate_safemode_Path = CONFIG(_strWorkPath) + L"\\appsync\\ndapp1.deb";
	wstring mobilesubstrate_Path = CONFIG(_strWorkPath) + L"\\appsync\\ndapp2.deb";
	wstring appsync50plus_Path = CONFIG(_strWorkPath) + L"\\appsync\\ndapp3.deb";

	if (!FileHelper::Upload(pDevice, com_saurik_substrate_safemode_Path, L"/var/root/Media/ndDaemon/appsync", NULL) ||
		!FileHelper::Upload(pDevice, mobilesubstrate_Path, L"/var/root/Media/ndDaemon/appsync", NULL) ||
		!FileHelper::Upload(pDevice, appsync50plus_Path, L"/var/root/Media/ndDaemon/appsync", NULL))
	{
		return false;
	}
	
	return pDevice->GetAppManager()->InstallAppsyncService();

}


bool CheckCanInstallAppsync::CanInstall( wstring sn )
{
	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(sn);
	RETURN_FALSE_IF(pDevice == NULL);

	wstring proVer = pDevice->GetDevInfo()->_deviceProductVersion;
	RETURN_FALSE_IF(proVer.empty());

	vector<wstring> verList = CStrOperation::parseStringsW(proVer, L".", false);

	RETURN_FALSE_IF(verList.size() <= 0);

	RETURN_FALSE_IF(atoi(CCodeOperation::ws2s(verList[0]).c_str()) < 6);

	RETURN_FALSE_IF(!pDevice->GetDevInfo()->_ios_bJailBreaked);

	wstring strAppSync = L"/Library/MobileSubstrate/DynamicLibraries/AppSync.dylib";
	return !CheckNodeExist(sn, strAppSync);
}

bool CheckCanInstallAppsync::CheckNodeExist( wstring sn, wstring path )
{
	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(sn);
	if(pDevice == NULL)
		return false;

	//判断是否安装过AppSync
	if (pDevice->GetRootData() != NULL && pDevice->GetRootData()->GetDir() != NULL)
	{
		NodeData* pNode = FileHelper::GetNode(pDevice->GetRootData()->GetDir(), path, false);
		return pNode != NULL;
	}

	return NULL;
}
