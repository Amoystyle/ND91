#include "MobileDevice.h"
#include "CFWrapper.h"
#include "Common/CodeOperation.h"
#define U2UTF8 CCodeOperation::Unicode2UTF8

MobileDevice::MobileDevice()
{
}

MobileDevice::~MobileDevice()
{
}

mach_error_t MobileDevice::iTunes_AMDeviceRetain(void* device)
{
	try 
    {
	    return AMDeviceRetain(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceRelease(void* device)
{
	try 
    {
	    return AMDeviceRelease(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceConnect(void* device)
{
	try 
    {
	    return AMDeviceConnect(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceIsPaired(void* device)
{
	try 
    {
	    return AMDeviceIsPaired(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceValidatePairing(void* device)
{
	try 
    {
	    return AMDeviceValidatePairing(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDevicePair(void*  device)
{
	try 
    {
	    return AMDevicePair(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceUnpair(void*  device)
{

	try 
    {
	    return AMDeviceUnpair(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceDisconnect(void*  device)
{
	try 
    {
	    return AMDeviceDisconnect(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceStopSession(void*  device)
{
	try 
    {
	    return AMDeviceStopSession(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceStartSession(void*  device)
{
	try 
    {
	    return AMDeviceStartSession(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceGetConnectionID(void*  device)
{
	try 
    {
	    return AMDeviceGetConnectionID(device);
	}
	catch(...)
	{
		return -1;
	}
}

void* MobileDevice::iTunes_AMDeviceCopyDeviceIdentifier(void*  device)
{
	try 
    {
	    return (void*)AMDeviceCopyDeviceIdentifier(device);
	}
	catch(...)
	{
		return NULL;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceStartService(void *device, void* service_name, void **handle, UINT32 *unknown)
{
	try 
    {
	    return AMDeviceStartService(device, service_name, handle, unknown);
	}
	catch(...)
	{
		return -1;
	}
}

void* MobileDevice::iTunes_AMDeviceCopyValue(void* device, void* unknown, void* cfstring)
{
	try 
    {
	    return AMDeviceCopyValue(device, unknown, cfstring);
	}
	catch(...)
	{
		return NULL;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceSetValue(void* device, unsigned int unknown, void* cfkey, void* cfValue)
{
	try 
    {
	    return AMDeviceSetValue(device, unknown, cfkey, cfValue);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceNotificationSubscribe(am_device_notification_callback callback, UINT32 unused1, UINT32 unused2, UINT32 unused3, void **notification)
{
	try 
    {
	    return AMDeviceNotificationSubscribe(callback, unused1, unused2, unused3, (struct am_device_notification **)notification);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDPostNotification( void* socket, const char* notification, const char* userinfo)
{
    try 
    {
        void* pNotification = CFWrapper::StringtoCFString(string(notification));
        void* pUserInfo = CFWrapper::StringtoCFString(string(userinfo));

        mach_error_t ret = AMDPostNotification((service_conn_t)socket, (CFStringRef)pNotification, (CFStringRef)pUserInfo);

        CFWrapper::NDCFRelease(pNotification);
        CFWrapper::NDCFRelease(pUserInfo);
        return ret;
    }
    catch(...)
    {
        return -1;
    }
}

mach_error_t MobileDevice::iTunes_AMDeviceNotificationUnsubscribe(void* nitification)
{
	try 
    {
		return AMDeviceNotificationUnsubscribe((struct am_device_notification *)nitification);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceTransferApplication(void *sockAFC, void* cfStrPath, void* cfDictUnknow, PackDataTransferEventHandler callback, void* unknow)
{
	try 
    {
	    return AMDeviceTransferApplication(sockAFC, cfStrPath, cfDictUnknow, callback, unknow);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceInstallApplication(void *sockInstallProxy, void* cfStrPath, void* opts, PackInstallApplicationEventHandler callback, void* unknow)
{
	try 
    {
	    return AMDeviceInstallApplication(sockInstallProxy, cfStrPath, opts, callback, unknow);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceUninstallApplication(void *sock, void* cfAppName, void* opts, void* callback, void* ukn)
{
	try 
    {
	    return AMDeviceUninstallApplication(sock, cfAppName, opts, callback, ukn);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceLookupApplications(void* device, void* optsDict, void** outApps)
{
	try 
    {
	    return AMDeviceLookupApplications(device, optsDict, outApps);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceArchiveApplication(void *sockInstallProxy, void* cfIdentifier, void* optsDict, PackInstallApplicationEventHandler callback, void* unknow)
{
	try 
    {
	    return AMDeviceArchiveApplication(sockInstallProxy, cfIdentifier, optsDict, callback, unknow);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceRemoveApplicationArchive(void *sockInstallProxy, void* cfIdentifier, void* optsDict, PackInstallApplicationEventHandler callback, void* unknow)
{
	try 
    {
	    return AMDeviceRemoveApplicationArchive(sockInstallProxy, cfIdentifier, optsDict, callback, unknow);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceRestoreApplication(void *sockInstallProxy, void* cfIdentifier, void* opts, PackInstallApplicationEventHandler callback, void* unknow)
{
	try 
    {
	    return AMDeviceRestoreApplication(sockInstallProxy, cfIdentifier, opts, callback, unknow);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceUpgradeApplication(void *sockInstallProxy, void* cfPackagePath, void* opts, PackInstallApplicationEventHandler callback, void* unknow)
{
	try 
    {
	    return AMDeviceUpgradeApplication(sockInstallProxy, cfPackagePath, opts, callback, unknow);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t MobileDevice::iTunes_AMDeviceTransferPath(void* device, void* cfPackagePath, void* opts, PackInstallApplicationEventHandler callback, void* unknow)
{
	try 
    {
	    return AMDeviceTransferPath(device, cfPackagePath, opts, callback, unknow);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCDirectoryOpen(void *conn, const wchar_t* path, void **hAFDirHandle)
{
	RETURN_VALUE_IF(!path, -1);
	
	try 
    {
	    return AFCDirectoryOpen(conn, U2UTF8(path), (struct afc_directory **)hAFDirHandle);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCDirectoryRead(void *conn, void *dir, char **dirent)
{
	try 
    {
	    return AFCDirectoryRead(conn, (struct afc_directory *)dir, dirent);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCDirectoryClose(void *conn, void *dir)
{
	try 
    {
	    return AFCDirectoryClose(conn, (struct afc_directory *)dir);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCConnectionOpen(void *handle, UINT32 io_timeout, void **conn)
{
	try 
    {
	    return AFCConnectionOpen((service_conn_t)handle, io_timeout, conn);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCConnectionClose(void *conn)
{
	try 
    {
	    return AFCConnectionClose(conn);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCFileInfoOpen(void *conn, const wchar_t* path, void **info)
{
	RETURN_VALUE_IF(!path || !conn, -1);
	try 
    {
        return AFCFileInfoOpen(conn, U2UTF8(path), (struct afc_dictionary **)info);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCKeyValueRead(void *dict, char **key, char **value)
{
	try 
    {
	    return AFCKeyValueRead((struct afc_dictionary *)dict, key, value);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCKeyValueClose(void *dict)
{
	try 
    {
	    return AFCKeyValueClose((struct afc_dictionary *)dict);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCRemovePath(void *conn, const wchar_t* path)
{
	RETURN_VALUE_IF(!path, -1);
	try 
    {
        return AFCRemovePath(conn, U2UTF8(path));
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCRenamePath(void *conn, const wchar_t* oldPath, const wchar_t* newPath)
{
	RETURN_VALUE_IF(!oldPath || !newPath, -1);
	try 
    {
        return AFCRenamePath(conn, U2UTF8(oldPath), U2UTF8(newPath));
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCFileRefOpen(void *conn, const wchar_t* path, afc_file_ref mode, afc_file_ref *ref)
{
	RETURN_VALUE_IF(!path, -1);
	try 
    {
        return AFCFileRefOpen(conn, U2UTF8(path), mode, ref);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCFileRefLock( void *conn, afc_file_ref ref )
{
    try 
    {
        return AFCFileRefLock(conn, ref);
    }
    catch(...)
    {
        return -1;
    }
}

afc_error_t MobileDevice::iTunes_AFCFileRefUnlock( void *conn, afc_file_ref ref )
{
    try 
    {
        return AFCFileRefUnlock(conn, ref);
    }
    catch(...)
    {
        return -1;
    }
}

afc_error_t MobileDevice::iTunes_AFCFileRefClose(void *conn, afc_file_ref ref)
{
	try 
    {
	    return AFCFileRefClose(conn, ref);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCFileRefRead(void *conn, afc_file_ref ref, void* buffer, UINT32 *len)
{
	try 
    {
	    return AFCFileRefRead(conn, ref, buffer, len);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCFileRefWrite(void *conn, afc_file_ref ref, void* buffer, UINT32 len)
{
	try 
    {
	    return AFCFileRefWrite(conn, ref, buffer, len);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCFileRefSeek(void *conn, afc_file_ref ref, afc_file_ref pos, afc_file_ref origin)
{
	try 
    {
	    return AFCFileRefSeek(conn, ref, pos, origin);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCDirectoryCreate(void *conn, const wchar_t* path)
{
	RETURN_VALUE_IF(!path, -1);
	try 
    {
        return AFCDirectoryCreate(conn, U2UTF8(path));
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCGetFileInfo(void *conn, const wchar_t* path, void **info, UINT32 *length)
{
	RETURN_VALUE_IF(!path, -1);
	try 
    {
        return -1;/*return AFCGetFileInfo(conn, U2UTF8(path), info, length);*/
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCFlushData(void *conn, afc_file_ref handle)
{
	try 
    {
	    return -1;/*return AFCFlushData(conn, handle);*/
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCFileRefTell(void *conn, afc_file_ref handle, UINT32 *position)
{
	try 
    {
	    return AFCFileRefTell(conn, handle, position);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t MobileDevice::iTunes_AFCFileRefSetFileSize(void *conn, afc_file_ref handle, UINT32 size)
{
	try 
    {
	    return AFCFileRefSetFileSize(conn, handle, size);
	}
	catch(...)
	{
		return -1;
	}
}

UINT32 MobileDevice::iTunes_AMRestoreRegisterForDeviceNotifications(
	am_restore_device_notification_callback dfu_connect_callback,
	am_restore_device_notification_callback recovery_connect_callback,
	am_restore_device_notification_callback dfu_disconnect_callback,
	am_restore_device_notification_callback recovery_disconnect_callback,
	unsigned int unknown0,
	void *user_info)
{
	try 
    {
	    return AMRestoreRegisterForDeviceNotifications(dfu_connect_callback, recovery_connect_callback, 
		                                                 dfu_disconnect_callback, recovery_disconnect_callback, unknown0, user_info);
	}
	catch(...)
	{
		return -1;
	}
}

INT32 MobileDevice::iTunes_AMRestoreModeDeviceCreate(UINT32 unknown0, UINT32 connection_id, UINT32 unknown1)
{
	try 
    {
	    return AMRestoreModeDeviceCreate(unknown0, connection_id, unknown1);
	}
	catch(...)
	{
		return NULL;
	}
}

void* MobileDevice::iTunes_AMRecoveryModeDeviceCopyIMEI(void* device)
{
	try 
    {
	    return AMRecoveryModeDeviceCopyIMEI(device);
	}
	catch(...)
	{
		return NULL;
	}
}

void* MobileDevice::iTunes_AMRecoveryModeDeviceCopySerialNumber(void* device)
{
	try 
    {
	    return AMRecoveryModeDeviceCopySerialNumber(device);
	}
	catch(...)
	{
		return NULL;
	}
}

void* MobileDevice::iTunes_AMRecoveryModeDeviceGetLocationID(void* device)
{
	try 
    {
	    return AMRecoveryModeDeviceGetLocationID(device);
	}
	catch(...)
	{
		return NULL;
	}
}

void* MobileDevice::iTunes_AMRecoveryModeDeviceGetProductID(void* device)
{
	try 
    {
	    return AMRecoveryModeDeviceGetProductID(device);
	}
	catch(...)
	{
		return NULL;
	}
}

INT32 MobileDevice::iTunes_AMRecoveryModeDeviceGetProductType(void* device)
{
	try 
    {
	    return AMRecoveryModeDeviceGetProductType(device);
	}
	catch(...)
	{
		return -1;
	}
}

void* MobileDevice::iTunes_AMRecoveryModeGetSoftwareBuildVersion(void* device)
{
	try 
    {
	    return AMRecoveryModeGetSoftwareBuildVersion(device);
	}
	catch(...)
	{
		return NULL;
	}
}

INT32 MobileDevice::iTunes_AMRecoveryModeDeviceGetTypeID(void* device)
{
	try 
    {
	    return AMRecoveryModeDeviceGetTypeID(device);
	}
	catch(...)
	{
		return -1;
	}
}

INT32 MobileDevice::iTunes_AMRecoveryModeDeviceSetAutoBoot(void* device, char flag)
{
	try 
    {
	    return AMRecoveryModeDeviceSetAutoBoot(device, flag);
	}
	catch(...)
	{
		return -1;
	}
}

INT32 MobileDevice::iTunes_AMRecoveryModeDeviceReboot(void* device)
{
	try 
    {
		return AMRecoveryModeDeviceReboot(device);
	}
	catch(...)
	{
		return -1;
	}
}

void MobileDevice::CloseSocket(void* socket)
{
}