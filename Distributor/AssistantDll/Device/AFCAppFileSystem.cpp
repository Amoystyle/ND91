#include "stdafx.h"
#include "AFCAppFileSystem.h"
#include "Device/IosUsbConnection.h"

AFCAppFileSystem* AFCAppFileSystem::StartAPPHouseArrestService(wstring strIdentifier, DeviceInfo* devInfo)
{
	AFCAppFileSystem* pFileSystem = NULL;

	IosUsbConnection conn(devInfo);
	if (conn.Connect2AppSharingDirectory(strIdentifier))
		pFileSystem = new AFCAppFileSystem(devInfo->_ios_house_arrest_connect_handle, devInfo);

	return pFileSystem;
}

string AFCAppFileSystem::Ios_GetRootDirectory(eFileSystemType eType)
{
	return "/Documents";
}

AFCAppFileSystem::AFCAppFileSystem(void* pAfcHandle, const DeviceInfo* pDevInfo)
:AFCFileSysemBase(pAfcHandle, pDevInfo)
{
}

AFCAppFileSystem::~AFCAppFileSystem()
{
}
