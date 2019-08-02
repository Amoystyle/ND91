#include "stdafx.h"

#include "ITunesMobileDevice.h"
#include "Device/IosPublicFunc.h"
#include "Device/CFConverter.h"


#define U2UTF8 MyCodeOperation::Unicode2UTF8

HINSTANCE CITunesMobileDevice::m_ndiPhoneUSB = NULL;
AMDeviceDeactivate CITunesMobileDevice::m_AMDeviceDeactivate = NULL;
AMDeviceActivate CITunesMobileDevice::m_AMDeviceActivate = NULL;
AMDeviceRetain  CITunesMobileDevice::m_AMDeviceRetain = NULL;
AMDeviceRelease  CITunesMobileDevice::m_AMDeviceRelease = NULL;	
AMDeviceConnect CITunesMobileDevice::m_AMDeviceConnect = NULL;
AMDeviceIsPaired CITunesMobileDevice::m_AMDeviceIsPaired = NULL;	
AMDeviceValidatePairing CITunesMobileDevice::m_AMDeviceValidatePairing = NULL;	
AMDevicePair CITunesMobileDevice::m_AMDevicePair = NULL;	
AMDeviceUnpair CITunesMobileDevice::m_AMDeviceUnpair = NULL;	
AMDeviceDisconnect CITunesMobileDevice::m_AMDeviceDisconnect = NULL;	
AMDeviceStopSession CITunesMobileDevice::m_AMDeviceStopSession = NULL;	
AMDeviceStartSession CITunesMobileDevice::m_AMDeviceStartSession = NULL;	
AMDeviceGetConnectionID CITunesMobileDevice::m_AMDeviceGetConnectionID = NULL;	
AMDeviceCopyDeviceIdentifier CITunesMobileDevice::m_AMDeviceCopyDeviceIdentifier = NULL;	
AMDeviceStartService CITunesMobileDevice::m_AMDeviceStartService = NULL;	
AMDeviceSecureStartService CITunesMobileDevice::m_AMDeviceSecureStartService = NULL;	

AMDeviceCopyValue CITunesMobileDevice::m_AMDeviceCopyValue = NULL;	
AMDeviceSetValue CITunesMobileDevice::m_AMDeviceSetValue = NULL;	
AMDeviceTransferApplication CITunesMobileDevice::m_AMDeviceTransferApplication = NULL;	
AMDeviceInstallApplication CITunesMobileDevice::m_AMDeviceInstallApplication = NULL;	
AMDeviceUninstallApplication CITunesMobileDevice::m_AMDeviceUninstallApplication = NULL; 	
AMDeviceNotificationSubscribe CITunesMobileDevice::m_AMDeviceNotificationSubscribe = NULL;	
AMDeviceNotificationUnsubscribe CITunesMobileDevice::m_AMDeviceNotificationUnsubscribe = NULL;	
AMDeviceLookupApplications CITunesMobileDevice::m_AMDeviceLookupApplications = NULL;	
AMRestoreModeDeviceCreate CITunesMobileDevice::m_AMRestoreModeDeviceCreate = NULL;	
AMDPostNotification CITunesMobileDevice::m_AMDPostNotification = NULL;
AMDeviceEnterRecover CITunesMobileDevice::m_AMDeviceEnterRecover = NULL;

AFCDirectoryOpen CITunesMobileDevice::m_AFCDirectoryOpen = NULL;	
AFCDirectoryRead CITunesMobileDevice::m_AFCDirectoryRead = NULL;
AFCDirectoryClose CITunesMobileDevice::m_AFCDirectoryClose = NULL;	
AMRestoreRegisterForDeviceNotifications CITunesMobileDevice::m_AMRestoreRegisterForDeviceNotifications = NULL;	
AFCConnectionOpen CITunesMobileDevice::m_AFCConnectionOpen = NULL;	
// ¹Ø±ÕÁ¬½Ó - 20081231 added -
AFCConnectionClose CITunesMobileDevice::m_AFCConnectionClose = NULL;	
AFCGetFileInfo CITunesMobileDevice::m_AFCGetFileInfo = NULL;	
AFCFileInfoOpen CITunesMobileDevice::m_AFCFileInfoOpen = NULL;
AFCKeyValueRead CITunesMobileDevice::m_AFCKeyValueRead = NULL;	
AFCKeyValueClose CITunesMobileDevice::m_AFCKeyValueClose = NULL;	
AFCRemovePath CITunesMobileDevice::m_AFCRemovePath = NULL;	
AFCRenamePath CITunesMobileDevice::m_AFCRenamePath = NULL;
AFCLinkPath CITunesMobileDevice::m_AFCLinkPath = NULL;
AFCFileRefOpen CITunesMobileDevice::m_AFCFileRefOpen = NULL;	
AFCFileRefLock CITunesMobileDevice::m_AFCFileRefLock = NULL;	
AFCFileRefUnlock CITunesMobileDevice::m_AFCFileRefUnlock = NULL;	
AFCFileRefClose CITunesMobileDevice::m_AFCFileRefClose = NULL;	
AFCFileRefRead CITunesMobileDevice::m_AFCFileRefRead = NULL;	
AFCFileRefWrite CITunesMobileDevice::m_AFCFileRefWrite = NULL;
AFCFlushData CITunesMobileDevice::m_AFCFlushData = NULL;	
AFCFileRefSeek CITunesMobileDevice::m_AFCFileRefSeek = NULL;	
AFCFileRefTell CITunesMobileDevice::m_AFCFileRefTell = NULL;	
AFCFileRefSetFileSize CITunesMobileDevice::m_AFCFileRefSetFileSize = NULL;
AFCOperationCreateGetFileHash CITunesMobileDevice::m_AFCOperationCreateGetFileHash = NULL;
AFCOperationCreateGetFileHashWithRange CITunesMobileDevice::m_AFCOperationCreateGetFileHashWithRange = NULL;
AFCOperationGetContext CITunesMobileDevice::m_AFCOperationGetContext = NULL;
AFCConnectionGetIOTimeout CITunesMobileDevice::m_AFCConnectionGetIOTimeout = NULL;
AFCConnectionProcessOperation CITunesMobileDevice::m_AFCConnectionProcessOperation = NULL;
AFCOperationGetResultStatus CITunesMobileDevice::m_AFCOperationGetResultStatus = NULL;
AFCOperationGetResultObject CITunesMobileDevice::m_AFCOperationGetResultObject = NULL;
AFCConnectionCopyLastErrorInfo CITunesMobileDevice::m_AFCConnectionCopyLastErrorInfo = NULL;
AFCConnectionIsValid CITunesMobileDevice::m_AFCConnectionIsValid = NULL;
AFCDirectoryCreate CITunesMobileDevice::m_AFCDirectoryCreate = NULL;	
AMRecoveryModeDeviceCopyIMEI CITunesMobileDevice::m_AMRecoveryModeDeviceCopyIMEI = NULL;	
AMRecoveryModeDeviceCopySerialNumber CITunesMobileDevice::m_AMRecoveryModeDeviceCopySerialNumber = NULL;	
AMRecoveryModeDeviceGetLocationID CITunesMobileDevice::m_AMRecoveryModeDeviceGetLocationID = NULL;	
AMRecoveryModeDeviceGetProductID CITunesMobileDevice::m_AMRecoveryModeDeviceGetProductID = NULL;	
AMRecoveryModeDeviceGetProductType CITunesMobileDevice::m_AMRecoveryModeDeviceGetProductType = NULL;	
AMRecoveryModeGetSoftwareBuildVersion CITunesMobileDevice::m_AMRecoveryModeGetSoftwareBuildVersion = NULL;	
AMRecoveryModeDeviceGetTypeID CITunesMobileDevice::m_AMRecoveryModeDeviceGetTypeID = NULL;	
AMRecoveryModeDeviceSetAutoBoot CITunesMobileDevice::m_AMRecoveryModeDeviceSetAutoBoot = NULL;	
AMRecoveryModeDeviceReboot CITunesMobileDevice::m_AMRecoveryModeDeviceReboot = NULL;	
AMDeviceArchiveApplication CITunesMobileDevice::m_AMDeviceArchiveApplication = NULL;
AMDeviceRemoveApplicationArchive CITunesMobileDevice::m_AMDeviceRemoveApplicationArchive = NULL;	
AMDeviceRestoreApplication CITunesMobileDevice::m_AMDeviceRestoreApplication = NULL;	
AMDeviceUpgradeApplication CITunesMobileDevice::m_AMDeviceUpgradeApplication = NULL;	
AMDeviceTransferPath CITunesMobileDevice::m_AMDeviceTransferPath = NULL;	
AMDCopyErrorText CITunesMobileDevice::m_AMDCopyErrorText = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CITunesMobileDevice::CITunesMobileDevice()
{
	if (!MyLoadLibrary())
		return ;
}

CITunesMobileDevice::~CITunesMobileDevice()
{
	MyFreeLibrary();
}

bool CITunesMobileDevice::MyLoadLibrary()
{
    RETURN_TRUE_IF(m_ndiPhoneUSB);
	 wstring appleApplicationSupportzlib1 = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, DLLCoreFoundationReg, _T("InstallDir"));
	 wstring appleApplicationSupportsql3=appleApplicationSupportzlib1+L"sqlite3.dll";
	 ::LoadLibrary(appleApplicationSupportsql3.c_str());
	 appleApplicationSupportzlib1+=L"zlib1.dll";
	 ::LoadLibrary(appleApplicationSupportzlib1.c_str());

	wstring strPath = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, DLLiTunesMobileDeviceReg, L"InstallDir");
	wstring szPath = IosPublicFunc::GetItunesDllRealPath(strPath, DLLiTunesMobileDevice);
    LOG->WriteDebug(L"iTunesDir = " + strPath);
    LOG->WriteDebug(L"ItunesDllRealPath = " + szPath);
	TCHAR sziTunesMobileDeviceDll[MAX_PATH];
	ZeroMemory(sziTunesMobileDeviceDll, MAX_PATH*sizeof(TCHAR));
	_tcscat_s(sziTunesMobileDeviceDll, MAX_PATH, szPath.c_str());
	_tcscat_s(sziTunesMobileDeviceDll, MAX_PATH, DLLiTunesMobileDevice);

    m_ndiPhoneUSB = ::LoadLibrary(sziTunesMobileDeviceDll);
	return NULL != m_ndiPhoneUSB;
}

void CITunesMobileDevice::MyFreeLibrary()
{
	if (m_ndiPhoneUSB)
	{
        ::FreeLibrary(m_ndiPhoneUSB);
        m_ndiPhoneUSB = NULL;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceDeactivate(struct am_device *device)
{
	RETURN_VALUE_IF(!MyLoadLibrary(), -1);

	m_AMDeviceDeactivate = (AMDeviceDeactivate)GetProcAddress(m_ndiPhoneUSB, "AMDeviceDeactivate");
	if (!m_AMDeviceDeactivate)
		return -1;

	try 
	{
		return m_AMDeviceDeactivate(device);
	}
	catch(...)
	{
		return -1;
	}
}
mach_error_t CITunesMobileDevice::iTunes_AMDeviceActivate(struct am_device *device, void* dict)
{
	RETURN_VALUE_IF(!MyLoadLibrary(), -1);

	m_AMDeviceActivate = (AMDeviceActivate)GetProcAddress(m_ndiPhoneUSB, "AMDeviceActivate");
	if (!m_AMDeviceActivate)
		return -1;

	try 
	{
		return m_AMDeviceActivate(device,dict);
	}
	catch(...)
	{
		return -1;
	}
}
mach_error_t CITunesMobileDevice::iTunes_AMDeviceRetain(void* device)
{
    RETURN_VALUE_IF(!MyLoadLibrary(), -1);

	m_AMDeviceRetain = (AMDeviceRetain)GetProcAddress(m_ndiPhoneUSB, "AMDeviceRetain");
	if (!m_AMDeviceRetain)
		return -1;

	try 
    {
	    return m_AMDeviceRetain(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceRelease(void* device)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceRelease = (AMDeviceRelease)GetProcAddress(m_ndiPhoneUSB, "AMDeviceRelease");
	if (!m_AMDeviceRelease)
		return -1;

	try {
	return m_AMDeviceRelease(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceConnect(void* device)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceConnect = (AMDeviceConnect)GetProcAddress(m_ndiPhoneUSB, "AMDeviceConnect");
	if (!m_AMDeviceConnect)
		return -1;

	try {
	return m_AMDeviceConnect(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceIsPaired(void* device)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceIsPaired = (AMDeviceIsPaired)GetProcAddress(m_ndiPhoneUSB, "AMDeviceIsPaired");
	if (!m_AMDeviceIsPaired)
		return -1;

	try {
	return m_AMDeviceIsPaired(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceValidatePairing(void* device)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceValidatePairing = (AMDeviceValidatePairing)GetProcAddress(m_ndiPhoneUSB, "AMDeviceValidatePairing");
	if (!m_AMDeviceValidatePairing)
		return -1;

	try {
	return m_AMDeviceValidatePairing(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDevicePair(void*  device)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDevicePair = (AMDevicePair)GetProcAddress(m_ndiPhoneUSB, "AMDevicePair");
	if (!m_AMDevicePair)
		return -1;

	try {
	return m_AMDevicePair(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceUnpair(void*  device)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceUnpair = (AMDeviceUnpair)GetProcAddress(m_ndiPhoneUSB, "AMDeviceUnpair");
	if (!m_AMDeviceUnpair)
		return -1;

	try {
	return m_AMDeviceUnpair(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceDisconnect(void*  device)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceDisconnect = (AMDeviceDisconnect)GetProcAddress(m_ndiPhoneUSB, "AMDeviceDisconnect");
	if (!m_AMDeviceDisconnect)
		return -1;

	try {
	return m_AMDeviceDisconnect(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceStopSession(void*  device)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceStopSession = (AMDeviceStopSession)GetProcAddress(m_ndiPhoneUSB, "AMDeviceStopSession");
	if (!m_AMDeviceStopSession)
		return -1;

	try {
	return m_AMDeviceStopSession(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceStartSession(void*  device)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceStartSession = (AMDeviceStartSession)GetProcAddress(m_ndiPhoneUSB, "AMDeviceStartSession");
	if (!m_AMDeviceStartSession)
		return -1;

	try {
	return m_AMDeviceStartSession(device);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceGetConnectionID(void*  device)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceGetConnectionID = (AMDeviceGetConnectionID)GetProcAddress(m_ndiPhoneUSB, "AMDeviceGetConnectionID");
	if (!m_AMDeviceGetConnectionID)
		return -1;

	try {
	return m_AMDeviceGetConnectionID(device);
	}
	catch(...)
	{
		return -1;
	}
}

void* CITunesMobileDevice::iTunes_AMDeviceCopyDeviceIdentifier(void*  device)
{
	if (!MyLoadLibrary())
		return NULL;
    m_AMDeviceCopyDeviceIdentifier = (AMDeviceCopyDeviceIdentifier)GetProcAddress(m_ndiPhoneUSB, "AMDeviceCopyDeviceIdentifier");
	if (!m_AMDeviceCopyDeviceIdentifier)
		return NULL;

	try {
	return m_AMDeviceCopyDeviceIdentifier(device);
	}
	catch(...)
	{
		return NULL;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceStartService(void *device, void* service_name, void **handle, UINT32 *unknown)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceStartService = (AMDeviceStartService)GetProcAddress(m_ndiPhoneUSB, "AMDeviceStartService");
	if (!m_AMDeviceStartService)
		return -1;

	try {
	return m_AMDeviceStartService(device, service_name, handle, unknown);
	}
	catch(...)
	{
		return -1;
	}
}
mach_error_t CITunesMobileDevice::iTunes_AMDeviceSecureStartService(void *device, void* service_name, void **unknown, void **secureHandle)
{
    if (!MyLoadLibrary())
        return -1;
    m_AMDeviceSecureStartService = (AMDeviceSecureStartService)GetProcAddress(m_ndiPhoneUSB, "AMDeviceSecureStartService");
    if (!m_AMDeviceSecureStartService)
        return -1;

    try {
        return m_AMDeviceSecureStartService(device, service_name, unknown, secureHandle);
    }
    catch(...)
    {
        return -1;
    }
}

void* CITunesMobileDevice::iTunes_AMDeviceCopyValue(void* device, void* unknown, void * cfstring)
{
	if (!MyLoadLibrary())
		return NULL;
    m_AMDeviceCopyValue = (AMDeviceCopyValue)GetProcAddress(m_ndiPhoneUSB, "AMDeviceCopyValue");
	if (!m_AMDeviceCopyValue)
		return NULL;

	try {
	return m_AMDeviceCopyValue(device, unknown, cfstring);
	}
	catch(...)
	{
		return NULL;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceSetValue(void* device, unsigned int unknown, void* cfkey, void* cfValue)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceSetValue = (AMDeviceSetValue)GetProcAddress(m_ndiPhoneUSB, "AMDeviceSetValue");
	if (!m_AMDeviceSetValue)
		return -1;

	try {
	return m_AMDeviceSetValue(device, unknown, cfkey, cfValue);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceNotificationSubscribe(am_device_notification_callback callback, UINT32 unused1, UINT32 unused2, UINT32 unused3, void **notification)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceNotificationSubscribe = (AMDeviceNotificationSubscribe)GetProcAddress(m_ndiPhoneUSB, "AMDeviceNotificationSubscribe");
	if (!m_AMDeviceNotificationSubscribe)
		return -1;

	try {
	return m_AMDeviceNotificationSubscribe(callback, unused1, unused2, unused3, notification);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDPostNotification( void* socket, const char* notification, const char* userinfo)
{
    if (!MyLoadLibrary())
        return -1;
    m_AMDPostNotification = (AMDPostNotification)GetProcAddress(m_ndiPhoneUSB, "AMDPostNotification");
    if (!m_AMDPostNotification)
        return -1;

    try {
        void* pNotification = CCFConverter::StringtoCFString(string(notification));
        void* pUserInfo = CCFConverter::StringtoCFString(string(userinfo));

        mach_error_t ret = m_AMDPostNotification(socket, pNotification, pUserInfo);

        CCFConverter::NDCFRelease(pNotification);
        CCFConverter::NDCFRelease(pUserInfo);
        return ret;
    }
    catch(...)
    {
        return -1;
    }
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceNotificationUnsubscribe(void* nitification)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceNotificationUnsubscribe = (AMDeviceNotificationUnsubscribe)GetProcAddress(m_ndiPhoneUSB, "AMDeviceNotificationUnsubscribe");
	if (!m_AMDeviceNotificationUnsubscribe)
		return -1;

	try {
		return m_AMDeviceNotificationUnsubscribe(nitification);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceTransferApplication(void *sockAFC, void* cfStrPath, void* cfDictUnknow, PackDataTransferEventHandler callback, void* unknow)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceTransferApplication = (AMDeviceTransferApplication)GetProcAddress(m_ndiPhoneUSB, "AMDeviceTransferApplication");
	if (!m_AMDeviceTransferApplication)
		return -1;

	try {
	return m_AMDeviceTransferApplication(sockAFC, cfStrPath, cfDictUnknow, callback, unknow);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceInstallApplication(void *sockInstallProxy, void* cfStrPath, void* opts, PackInstallApplicationEventHandler callback, void* unknow)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceInstallApplication = (AMDeviceInstallApplication)GetProcAddress(m_ndiPhoneUSB, "AMDeviceInstallApplication");
	if (!m_AMDeviceInstallApplication)
		return -1;

	try {
	return m_AMDeviceInstallApplication(sockInstallProxy, cfStrPath, opts, callback, unknow);
	}
	catch(...)
	{
		return -1;
	}
}
mach_error_t CITunesMobileDevice::iTunes_AMDeviceEnterRecover(void *handle)
{
	if (!MyLoadLibrary())
		return -1;
	m_AMDeviceEnterRecover = (AMDeviceEnterRecover)GetProcAddress(m_ndiPhoneUSB, "AMDeviceEnterRecovery");
	if (!m_AMDeviceEnterRecover)
		return -1;

	try {
		return m_AMDeviceEnterRecover(handle);
	}
	catch(...)
	{
		return -1;
	}
}
mach_error_t CITunesMobileDevice::iTunes_AMDeviceUninstallApplication(void *sock, void* cfAppName, void* opts, void* callback, void* ukn)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceUninstallApplication = (AMDeviceUninstallApplication)GetProcAddress(m_ndiPhoneUSB, "AMDeviceUninstallApplication");
	if (!m_AMDeviceUninstallApplication)
		return -1;

	try {
	return m_AMDeviceUninstallApplication(sock, cfAppName, opts, callback, ukn);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceLookupApplications(void* device, void* optsDict, void** outApps)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceLookupApplications = (AMDeviceLookupApplications)GetProcAddress(m_ndiPhoneUSB, "AMDeviceLookupApplications");
	if (!m_AMDeviceLookupApplications)
		return -1;

	try {
	return m_AMDeviceLookupApplications(device, optsDict, outApps);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceArchiveApplication(void *sockInstallProxy, void* cfIdentifier, void* opts, PackInstallApplicationEventHandler callback, void* unknow)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceArchiveApplication = (AMDeviceArchiveApplication)GetProcAddress(m_ndiPhoneUSB, "AMDeviceArchiveApplication");
	if (!m_AMDeviceArchiveApplication)
		return -1;

	try {
	return m_AMDeviceArchiveApplication(sockInstallProxy, cfIdentifier, opts, callback, unknow);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceRemoveApplicationArchive(void *sockInstallProxy, void* cfIdentifier, void* opts, PackInstallApplicationEventHandler callback, void* unknow)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceRemoveApplicationArchive = (AMDeviceRemoveApplicationArchive)GetProcAddress(m_ndiPhoneUSB, "AMDeviceRemoveApplicationArchive");
	if (!m_AMDeviceRemoveApplicationArchive)
		return -1;

	try {
	return m_AMDeviceRemoveApplicationArchive(sockInstallProxy, cfIdentifier, opts, callback, unknow);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceRestoreApplication(void *sockInstallProxy, void* cfIdentifier, void* opts, PackInstallApplicationEventHandler callback, void* unknow)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceRestoreApplication = (AMDeviceRestoreApplication)GetProcAddress(m_ndiPhoneUSB, "AMDeviceRestoreApplication");
	if (!m_AMDeviceRestoreApplication)
		return -1;

	try {
	return m_AMDeviceRestoreApplication(sockInstallProxy, cfIdentifier, opts, callback, unknow);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceUpgradeApplication(void *sockInstallProxy, void* cfPackagePath, void* opts, PackInstallApplicationEventHandler callback, void* unknow)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceUpgradeApplication = (AMDeviceUpgradeApplication)GetProcAddress(m_ndiPhoneUSB, "AMDeviceUpgradeApplication");
	if (!m_AMDeviceUpgradeApplication)
		return -1;

	try {
	return m_AMDeviceUpgradeApplication(sockInstallProxy, cfPackagePath, opts, callback, unknow);
	}
	catch(...)
	{
		return -1;
	}
}

mach_error_t CITunesMobileDevice::iTunes_AMDeviceTransferPath(void* device, void* cfPackagePath, void* opts, PackInstallApplicationEventHandler callback, void* unknow)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMDeviceTransferPath = (AMDeviceTransferPath)GetProcAddress(m_ndiPhoneUSB, "AMDeviceTransferPath");
	if (!m_AMDeviceTransferPath)
		return -1;

	try {
	return m_AMDeviceTransferPath(device, cfPackagePath, opts, callback, unknow);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCDirectoryOpen(void *conn, const wchar_t* path, void **hAFDirHandle)
{
    RETURN_VALUE_IF(!path, -1);

	if (!MyLoadLibrary())
		return -1;
    m_AFCDirectoryOpen = (AFCDirectoryOpen)GetProcAddress(m_ndiPhoneUSB, "AFCDirectoryOpen");
	if (!m_AFCDirectoryOpen)
		return -1;

	try 
    {
	    return m_AFCDirectoryOpen(conn, U2UTF8(path), hAFDirHandle);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCDirectoryRead(void *conn, void *dir, char **dirent)
{
	if (!MyLoadLibrary())
		return -1;
    m_AFCDirectoryRead = (AFCDirectoryRead)GetProcAddress(m_ndiPhoneUSB, "AFCDirectoryRead");
	if (!m_AFCDirectoryRead)
		return -1;

	try {
	return m_AFCDirectoryRead(conn, dir, dirent);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCDirectoryClose(void *conn, void *dir)
{
	if (!MyLoadLibrary())
		return -1;
    m_AFCDirectoryClose = (AFCDirectoryClose)GetProcAddress(m_ndiPhoneUSB, "AFCDirectoryClose");
	if (!m_AFCDirectoryClose)
		return -1;

	try {
	return m_AFCDirectoryClose(conn, dir);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCConnectionOpen(void *handle, UINT32 io_timeout, void **conn)
{
	if (!MyLoadLibrary())
		return -1;
    m_AFCConnectionOpen = (AFCConnectionOpen)GetProcAddress(m_ndiPhoneUSB, "AFCConnectionOpen");
	if (!m_AFCConnectionOpen)
		return -1;

	try {
	return m_AFCConnectionOpen(handle, io_timeout, conn);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCConnectionClose(void *conn)
{
	if (!MyLoadLibrary())
		return -1;
    m_AFCConnectionClose = (AFCConnectionClose)GetProcAddress(m_ndiPhoneUSB, "AFCConnectionClose");
	if (!m_AFCConnectionClose)
		return -1;

	try {
	return m_AFCConnectionClose(conn);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCFileInfoOpen(void *conn, const wchar_t* path, void **info)
{
    RETURN_VALUE_IF(!path || !conn, -1);

	if (!MyLoadLibrary())
		return -1;
    m_AFCFileInfoOpen = (AFCFileInfoOpen)GetProcAddress(m_ndiPhoneUSB, "AFCFileInfoOpen");
	if (!m_AFCFileInfoOpen)
		return -1;

	try {
    return m_AFCFileInfoOpen(conn, U2UTF8(path), info);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCKeyValueRead(void *dict, char **key, char **value)
{
	if (!MyLoadLibrary())
		return -1;
    m_AFCKeyValueRead = (AFCKeyValueRead)GetProcAddress(m_ndiPhoneUSB, "AFCKeyValueRead");
	if (!m_AFCKeyValueRead)
		return -1;

	try {
	return m_AFCKeyValueRead(dict, key, value);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCKeyValueClose(void *dict)
{
	if (!MyLoadLibrary())
		return -1;
    m_AFCKeyValueClose = (AFCKeyValueClose)GetProcAddress(m_ndiPhoneUSB, "AFCKeyValueClose");
	if (!m_AFCKeyValueClose)
		return -1;

	try {
	return m_AFCKeyValueClose(dict);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCRemovePath(void *conn, const wchar_t* path)
{
    RETURN_VALUE_IF(!path, -1);

	if (!MyLoadLibrary())
		return -1;
    m_AFCRemovePath = (AFCRemovePath)GetProcAddress(m_ndiPhoneUSB, "AFCRemovePath");
	if (!m_AFCRemovePath)
		return -1;

	try {
    return m_AFCRemovePath(conn, U2UTF8(path));
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCRenamePath(void *conn, const wchar_t* oldPath, const wchar_t* newPath)
{
    RETURN_VALUE_IF(!oldPath || !newPath, -1);

	if (!MyLoadLibrary())
		return -1;
    m_AFCRenamePath = (AFCRenamePath)GetProcAddress(m_ndiPhoneUSB, "AFCRenamePath");
	if (!m_AFCRenamePath)
		return -1;

	try {
    return m_AFCRenamePath(conn, U2UTF8(oldPath), U2UTF8(newPath));
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCLinkPath(void *conn, int mode, int unknow, const wchar_t* target, const wchar_t* link)
{
    RETURN_VALUE_IF(!target || !link, -1);
    RETURN_VALUE_IF(!MyLoadLibrary(), -1)

    m_AFCLinkPath = (AFCLinkPath)GetProcAddress(m_ndiPhoneUSB, "AFCLinkPath");
    RETURN_VALUE_IF(!m_AFCLinkPath, -1);
    try 
    {
        return m_AFCLinkPath(conn, mode, unknow, U2UTF8(target), U2UTF8(link));
    }
    catch(...)
    {
        return -1;
    }
}

afc_error_t CITunesMobileDevice::iTunes_AFCFileRefOpen(void *conn, const wchar_t* path, afc_file_ref mode, afc_file_ref *ref)
{
    RETURN_VALUE_IF(!path, -1);

	if (!MyLoadLibrary())
		return -1;

    m_AFCFileRefOpen = (AFCFileRefOpen)GetProcAddress(m_ndiPhoneUSB, "AFCFileRefOpen");
	if (!m_AFCFileRefOpen)
		return -1;

	try {
    return m_AFCFileRefOpen(conn, U2UTF8(path), mode, ref);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCFileRefLock( void *conn, afc_file_ref ref )
{
    if (!MyLoadLibrary())
        return -1;

    m_AFCFileRefLock = (AFCFileRefLock)GetProcAddress(m_ndiPhoneUSB, "AFCFileRefLock");
    if (!m_AFCFileRefLock)
        return -1;

    try {
        return m_AFCFileRefLock(conn, ref);
    }
    catch(...)
    {
        return -1;
    }
}

afc_error_t CITunesMobileDevice::iTunes_AFCFileRefUnlock( void *conn, afc_file_ref ref )
{
    if (!MyLoadLibrary())
        return -1;

    m_AFCFileRefUnlock = (AFCFileRefUnlock)GetProcAddress(m_ndiPhoneUSB, "AFCFileRefUnlock");
    if (!m_AFCFileRefUnlock)
        return -1;

    try {
        return m_AFCFileRefUnlock(conn, ref);
    }
    catch(...)
    {
        return -1;
    }
}

afc_error_t CITunesMobileDevice::iTunes_AFCFileRefClose(void *conn, afc_file_ref ref)
{
	if (!MyLoadLibrary())
		return -1;
    m_AFCFileRefClose = (AFCFileRefClose)GetProcAddress(m_ndiPhoneUSB, "AFCFileRefClose");
	if (!m_AFCFileRefClose)
		return -1;

	try {
	return m_AFCFileRefClose(conn, ref);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCFileRefRead(void *conn, afc_file_ref ref, void* buffer, UINT32 *len)
{
	if (!MyLoadLibrary())
		return -1;
    m_AFCFileRefRead = (AFCFileRefRead)GetProcAddress(m_ndiPhoneUSB, "AFCFileRefRead");
	if (!m_AFCFileRefRead)
		return -1;

	try {
	return m_AFCFileRefRead(conn, ref, buffer, len);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCFileRefWrite(void *conn, afc_file_ref ref, void* buffer, UINT32 len)
{
	if (!MyLoadLibrary())
		return -1;
    m_AFCFileRefWrite = (AFCFileRefWrite)GetProcAddress(m_ndiPhoneUSB, "AFCFileRefWrite");
	if (!m_AFCFileRefWrite)
		return -1;

	try {
	return m_AFCFileRefWrite(conn, ref, buffer, len);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCFileRefSeek(void *conn, afc_file_ref ref, afc_file_ref pos, afc_file_ref origin)
{
	if (!MyLoadLibrary())
		return -1;
    m_AFCFileRefSeek = (AFCFileRefSeek)GetProcAddress(m_ndiPhoneUSB, "AFCFileRefSeek");
	if (!m_AFCFileRefSeek)
		return -1;

	try {
	return m_AFCFileRefSeek(conn, ref, pos, origin);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCDirectoryCreate(void *conn, const wchar_t* path)
{
    RETURN_VALUE_IF(!path, -1);

	if (!MyLoadLibrary())
		return -1;
    m_AFCDirectoryCreate = (AFCDirectoryCreate)GetProcAddress(m_ndiPhoneUSB, "AFCDirectoryCreate");
    if (!m_AFCDirectoryCreate)
        return -1;

	try {
    return m_AFCDirectoryCreate(conn, U2UTF8(path));
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCGetFileInfo(void *conn, const wchar_t* path, void **info, UINT32 *length)
{
    RETURN_VALUE_IF(!path, -1);

	if (!MyLoadLibrary())
		return -1;
    m_AFCGetFileInfo = (AFCGetFileInfo)GetProcAddress(m_ndiPhoneUSB, "AFCGetFileInfo");
	if (!m_AFCGetFileInfo)
		return -1;

	try {
    return m_AFCGetFileInfo(conn, U2UTF8(path), info, length);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCFlushData(void *conn, afc_file_ref handle)
{
	if (!MyLoadLibrary())
		return -1;
    m_AFCFlushData = (AFCFlushData)GetProcAddress(m_ndiPhoneUSB, "AFCFlushData");
	if (!m_AFCFlushData)
		return -1;
	try {
	return m_AFCFlushData(conn, handle);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCFileRefTell(void *conn, afc_file_ref handle, UINT32 *position)
{
	if (!MyLoadLibrary())
		return -1;
    m_AFCFileRefTell = (AFCFileRefTell)GetProcAddress(m_ndiPhoneUSB, "AFCFileRefTell");
	if (!m_AFCFileRefTell)
		return -1;
	try {
	return m_AFCFileRefTell(conn, handle, position);
	}
	catch(...)
	{
		return -1;
	}
}

afc_error_t CITunesMobileDevice::iTunes_AFCFileRefSetFileSize(void *conn, afc_file_ref handle, UINT32 size)
{
	if (!MyLoadLibrary())
		return -1;
    m_AFCFileRefSetFileSize = (AFCFileRefSetFileSize)GetProcAddress(m_ndiPhoneUSB, "AFCFileRefSetFileSize");
	if (!m_AFCFileRefSetFileSize)
		return -1;
	try {
	return m_AFCFileRefSetFileSize(conn, handle, size);
	}
	catch(...)
	{
		return -1;
	}
}

void* CITunesMobileDevice::iTunes_AFCOperationCreateGetFileHash(void* allocator, void* cfstringFileName, void* afcContext)
{
    RETURN_NULL_IF(!MyLoadLibrary());

    m_AFCOperationCreateGetFileHash = (AFCOperationCreateGetFileHash)GetProcAddress(m_ndiPhoneUSB, "AFCOperationCreateGetFileHash");
    RETURN_NULL_IF(!m_AFCOperationCreateGetFileHash);
    try 
    {
        return m_AFCOperationCreateGetFileHash(allocator, cfstringFileName, afcContext);
    }
    catch(...)
    {
        return NULL;
    }
}

void* CITunesMobileDevice::iTunes_AFCOperationCreateGetFileHashWithRange(void* allocator, void* cfstringFileName, void* unknow1,
                                                           void* unknow2, long endPosition, void* unknow3, void* unknow4)
{
    RETURN_NULL_IF(!MyLoadLibrary());

    m_AFCOperationCreateGetFileHashWithRange = (AFCOperationCreateGetFileHashWithRange)GetProcAddress(m_ndiPhoneUSB, "AFCOperationCreateGetFileHashWithRange");
    RETURN_NULL_IF(!m_AFCOperationCreateGetFileHashWithRange);
    try 
    {
        return m_AFCOperationCreateGetFileHashWithRange(allocator, cfstringFileName, unknow1,
                                                        unknow2, endPosition, unknow3, unknow4);
    }
    catch(...)
    {
        return NULL;
    }
}

void* CITunesMobileDevice::iTunes_AFCOperationGetContext(void* afcHandle)
{
    RETURN_NULL_IF(!MyLoadLibrary());

    m_AFCOperationGetContext = (AFCOperationGetContext)GetProcAddress(m_ndiPhoneUSB, "AFCOperationGetContext");
    RETURN_NULL_IF(!m_AFCOperationGetContext);
    try 
    {
        return m_AFCOperationGetContext(afcHandle);
    }
    catch(...)
    {
        return NULL;
    }
}

afc_error_t CITunesMobileDevice::iTunes_AFCConnectionGetIOTimeout(void* afcHandle)
{
    RETURN_VALUE_IF(!MyLoadLibrary(), -1);

    m_AFCConnectionGetIOTimeout = (AFCConnectionGetIOTimeout)GetProcAddress(m_ndiPhoneUSB, "AFCConnectionGetIOTimeout");
    RETURN_VALUE_IF(!m_AFCConnectionGetIOTimeout, -1);
    try 
    {
        return m_AFCConnectionGetIOTimeout(afcHandle);
    }
    catch(...)
    {
        return -1;
    }
}

afc_error_t CITunesMobileDevice::iTunes_AFCConnectionProcessOperation(void* afcHandle, void* op, int timeout)
{
    RETURN_VALUE_IF(!MyLoadLibrary(), -1);

    m_AFCConnectionProcessOperation = (AFCConnectionProcessOperation)GetProcAddress(m_ndiPhoneUSB, "AFCConnectionProcessOperation");
    RETURN_VALUE_IF(!m_AFCConnectionProcessOperation, -1);
    try 
    {
        return m_AFCConnectionProcessOperation(afcHandle, op, timeout);
    }
    catch(...)
    {
        return -1;
    }
}

afc_error_t CITunesMobileDevice::iTunes_AFCOperationGetResultStatus(void* op)
{
    RETURN_VALUE_IF(!MyLoadLibrary(), -1);

    m_AFCOperationGetResultStatus = (AFCOperationGetResultStatus)GetProcAddress(m_ndiPhoneUSB, "AFCOperationGetResultStatus");
    RETURN_VALUE_IF(!m_AFCOperationGetResultStatus, -1);
    try 
    {
        return m_AFCOperationGetResultStatus(op);
    }
    catch(...)
    {
        return -1;
    }
}

void* CITunesMobileDevice::iTunes_AFCOperationGetResultObject(void* op)
{
    RETURN_NULL_IF(!MyLoadLibrary());

    m_AFCOperationGetResultObject = (AFCOperationGetResultObject)GetProcAddress(m_ndiPhoneUSB, "AFCOperationGetResultObject");
    RETURN_NULL_IF(!m_AFCOperationGetResultObject);
    try 
    {
        return m_AFCOperationGetResultObject(op);
    }
    catch(...)
    {
        return NULL;
    }
}

void* CITunesMobileDevice::iTunes_AFCConnectionCopyLastErrorInfo(void* afcHandle)
{
    RETURN_NULL_IF(!MyLoadLibrary());

    m_AFCConnectionCopyLastErrorInfo = (AFCConnectionCopyLastErrorInfo)GetProcAddress(m_ndiPhoneUSB, "AFCConnectionCopyLastErrorInfo");
    RETURN_NULL_IF(!m_AFCConnectionCopyLastErrorInfo);
    try 
    {
        return m_AFCConnectionCopyLastErrorInfo(afcHandle);
    }
    catch(...)
    {
        return NULL;
    }
}

bool CITunesMobileDevice::iTunes_AFCConnectionIsValid(void* afcHandle)
{
    RETURN_FALSE_IF(!MyLoadLibrary());

    m_AFCConnectionIsValid = (AFCConnectionIsValid)GetProcAddress(m_ndiPhoneUSB, "AFCConnectionIsValid");
    RETURN_FALSE_IF(!m_AFCConnectionIsValid);
    try 
    {
        return m_AFCConnectionIsValid(afcHandle);
    }
    catch(...)
    {
        return false;
    }
}

UINT32 CITunesMobileDevice::iTunes_AMRestoreRegisterForDeviceNotifications(
	am_restore_device_notification_callback dfu_connect_callback,
	am_restore_device_notification_callback recovery_connect_callback,
	am_restore_device_notification_callback dfu_disconnect_callback,
	am_restore_device_notification_callback recovery_disconnect_callback,
	unsigned int unknown0,
	void *user_info)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMRestoreRegisterForDeviceNotifications = (AMRestoreRegisterForDeviceNotifications)GetProcAddress(m_ndiPhoneUSB, "AMRestoreRegisterForDeviceNotifications");
	if (!m_AMRestoreRegisterForDeviceNotifications)
		return -1;
	try {
	return m_AMRestoreRegisterForDeviceNotifications(dfu_connect_callback, recovery_connect_callback, 
		dfu_disconnect_callback, recovery_disconnect_callback, unknown0, user_info);
	}
	catch(...)
	{
		return -1;
	}
}

INT32 CITunesMobileDevice::iTunes_AMRestoreModeDeviceCreate(UINT32 unknown0, UINT32 connection_id, UINT32 unknown1)
{
	if (!MyLoadLibrary())
		return NULL;
    m_AMRestoreModeDeviceCreate = (AMRestoreModeDeviceCreate)GetProcAddress(m_ndiPhoneUSB, "AMRestoreModeDeviceCreate");
	if (!m_AMRestoreModeDeviceCreate)
		return NULL;
	try {
	return m_AMRestoreModeDeviceCreate(unknown0, connection_id, unknown1);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CITunesMobileDevice::iTunes_AMRecoveryModeDeviceCopyIMEI(void* device)
{
	if (!MyLoadLibrary())
		return NULL;
    m_AMRecoveryModeDeviceCopyIMEI = (AMRecoveryModeDeviceCopyIMEI)GetProcAddress(m_ndiPhoneUSB, "AMRecoveryModeDeviceCopyIMEI");
	if (!m_AMRecoveryModeDeviceCopyIMEI)
		return NULL;
	try {
	return m_AMRecoveryModeDeviceCopyIMEI(device);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CITunesMobileDevice::iTunes_AMRecoveryModeDeviceCopySerialNumber(void* device)
{
	if (!MyLoadLibrary())
		return NULL;
    m_AMRecoveryModeDeviceCopySerialNumber = (AMRecoveryModeDeviceCopySerialNumber)GetProcAddress(m_ndiPhoneUSB, "AMRecoveryModeDeviceCopySerialNumber");
	if (!m_AMRecoveryModeDeviceCopySerialNumber)
		return NULL;
	try {
	return m_AMRecoveryModeDeviceCopySerialNumber(device);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CITunesMobileDevice::iTunes_AMRecoveryModeDeviceGetLocationID(void* device)
{
	if (!MyLoadLibrary())
		return NULL;
    m_AMRecoveryModeDeviceGetLocationID = (AMRecoveryModeDeviceGetLocationID)GetProcAddress(m_ndiPhoneUSB, "AMRecoveryModeDeviceGetLocationID");
	if (!m_AMRecoveryModeDeviceGetLocationID)
		return NULL;
	try {
	return m_AMRecoveryModeDeviceGetLocationID(device);
	}
	catch(...)
	{
		return NULL;
	}
}

void* CITunesMobileDevice::iTunes_AMRecoveryModeDeviceGetProductID(void* device)
{
	if (!MyLoadLibrary())
		return NULL;
    m_AMRecoveryModeDeviceGetProductID = (AMRecoveryModeDeviceGetProductID)GetProcAddress(m_ndiPhoneUSB, "AMRecoveryModeDeviceGetProductID");
	if (!m_AMRecoveryModeDeviceGetProductID)
		return NULL;
	try {
	return m_AMRecoveryModeDeviceGetProductID(device);
	}
	catch(...)
	{
		return NULL;
	}
}

INT32 CITunesMobileDevice::iTunes_AMRecoveryModeDeviceGetProductType(void* device)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMRecoveryModeDeviceGetProductType = (AMRecoveryModeDeviceGetProductType)GetProcAddress(m_ndiPhoneUSB, "AMRecoveryModeDeviceGetProductType");
	if (!m_AMRecoveryModeDeviceGetProductType)
		return -1;
	try {
	return m_AMRecoveryModeDeviceGetProductType(device);
	}
	catch(...)
	{
		return -1;
	}
}

void* CITunesMobileDevice::iTunes_AMRecoveryModeGetSoftwareBuildVersion(void* device)
{
	if (!MyLoadLibrary())
		return NULL;
    m_AMRecoveryModeGetSoftwareBuildVersion = (AMRecoveryModeGetSoftwareBuildVersion)GetProcAddress(m_ndiPhoneUSB, "AMRecoveryModeGetSoftwareBuildVersion");
	if (!m_AMRecoveryModeGetSoftwareBuildVersion)
		return NULL;
	try {
	return m_AMRecoveryModeGetSoftwareBuildVersion(device);
	}
	catch(...)
	{
		return NULL;
	}
}

INT32 CITunesMobileDevice::iTunes_AMRecoveryModeDeviceGetTypeID(void* device)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMRecoveryModeDeviceGetTypeID = (AMRecoveryModeDeviceGetTypeID)GetProcAddress(m_ndiPhoneUSB, "AMRecoveryModeDeviceGetTypeID");
	if (!m_AMRecoveryModeDeviceGetTypeID)
		return -1;
	try {
	return m_AMRecoveryModeDeviceGetTypeID(device);
	}
	catch(...)
	{
		return -1;
	}
}

INT32 CITunesMobileDevice::iTunes_AMRecoveryModeDeviceSetAutoBoot(void* device, char flag)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMRecoveryModeDeviceSetAutoBoot = (AMRecoveryModeDeviceSetAutoBoot)GetProcAddress(m_ndiPhoneUSB, "AMRecoveryModeDeviceSetAutoBoot");
	if (!m_AMRecoveryModeDeviceSetAutoBoot)
		return -1;
	try {
	return m_AMRecoveryModeDeviceSetAutoBoot(device, flag);
	}
	catch(...)
	{
		return -1;
	}
}

INT32 CITunesMobileDevice::iTunes_AMRecoveryModeDeviceReboot(void* device)
{
	if (!MyLoadLibrary())
		return -1;
    m_AMRecoveryModeDeviceReboot = (AMRecoveryModeDeviceReboot)GetProcAddress(m_ndiPhoneUSB, "AMRecoveryModeDeviceReboot");
	if (!m_AMRecoveryModeDeviceReboot)
		return -1;
	try {
		return m_AMRecoveryModeDeviceReboot(device);
	}
	catch(...)
	{
		return -1;
	}
}

void CITunesMobileDevice::CloseSocket(void* socket)
{
	if (socket)
		closesocket((SOCKET)socket);
}

void* CITunesMobileDevice::iTunes_AMDCopyErrorText(int errorCode)
{
    RETURN_NULL_IF(!MyLoadLibrary());

    m_AMDCopyErrorText = (AMDCopyErrorText)GetProcAddress(m_ndiPhoneUSB, "AMDCopyErrorText");
    RETURN_NULL_IF(!m_AMDCopyErrorText);
    try 
    {
        return m_AMDCopyErrorText(errorCode);
    }
    catch(...)
    {
        return NULL;
    }
}

string CITunesMobileDevice::GetAMDErrorText(int errorCode)
{
    string error;
    void* errorPtr = iTunes_AMDCopyErrorText(errorCode);
    if (errorPtr != NULL)
    {
        CCFConverter::GetIosData(errorPtr, error);
        CCFConverter::NDCFRelease(errorPtr);
        errorPtr = NULL;
    }

    return error;
}

string CITunesMobileDevice::iTunes_GetDeviceErrorCodeString(int errorCode)
{
    string result = "UnknowError For 91 GetDeviceErrorCodeString";
    RETURN_VALUE_IF(errorCode > 0, result);

    result = GetAMDErrorText(errorCode);

    RETURN_VALUE_IF(!result.empty(), result);

    switch (errorCode)
    {
    case -402653183:
        result = "kAMDUndefinedError";
        break;
    case -402653182:
        result = "kAMDBadHeaderError";
        break;
    case -402653181:
        result = "kAMDNoResourcesError";
        break;
    case -402653180:
        result = "kAMDReadError";
        break;
    case -402653179:
        result = "kAMDWriteError";
        break;
    case -402653178:
        result = "kAMDUnknownPacketError";
        break;
    case -402653177:
        result = "kAMDInvalidArgumentError";
        break;
    case -402653176:
        result = "kAMDNotFoundError";
        break;
    case -402653175:
        result = "kAMDIsDirectoryError";
        break;
    case -402653174:
        result = "kAMDPermissionError";
        break;
    case -402653173:
        result = "kAMDNotConnectedError";
        break;
    case -402653172:
        result = "kAMDTimeOutError";
        break;
    case -402653171:
        result = "kAMDOverrunError";
        break;
    case -402653170:
        result = "kAMDEOFError";
        break;
    case -402653169:
        result = "kAMDUnsupportedError";
        break;
    case -402653168:
        result = "kAMDFileExistsError";
        break;
    case -402653167:
        result = "kAMDBusyError";
        break;
    case -402653166:
        result = "kAMDCryptoError";
        break;
    case -402653165:
        result = "kAMDInvalidResponseError";
        break;
    case -402653164:
        result = "kAMDMissingKeyError";
        break;
    case -402653163:
        result = "kAMDMissingValueError";
        break;
    case -402653162:
        result = "kAMDGetProhibitedError";
        break;
    case -402653161:
        result = "kAMDSetProhibitedError";
        break;
    case -402653160:
        result = "kAMDRemoveProhibitedError";
        break;
    case -402653159:
        result = "kAMDImmutableValueError";
        break;
    case -402653158:
        result = "kAMDPasswordProtectedError";
        break;
    case -402653157:
        result = "kAMDMissingHostIDError";
        break;
    case -402653156:
        result = "kAMDInvalidHostIDError";
        break;
    case -402653155:
        result = "kAMDSessionActiveError";
        break;
    case -402653154:
        result = "kAMDSessionInactiveError";
        break;
    case -402653153:
        result = "kAMDMissingSessionIDError";
        break;
    case -402653152:
        result = "kAMDInvalidSessionIDError";
        break;
    case -402653151:
        result = "kAMDMissingServiceError";
        break;
    case -402653150:
        result = "kAMDInvalidServiceError";
        break;
    case -402653093:
        result = "kAMDServiceLimitError";
        break;
    case -402653090:
        result = "kAMDCheckinSetupFailedError";
        break;
    case -402653149:
        result = "kAMDInvalidCheckinError";
        break;
    case -402653148:
        result = "kAMDCheckinTimeoutError";
        break;
    case -402653089:
        result = "kAMDCheckinConnectionFailedError";
        break;
    case -402653088:
        result = "kAMDCheckinReceiveFailedError";
        break;
    case -402653087:
        result = "kAMDCheckinResponseFailedError";
        break;
    case -402653079:
        result = "kAMDCheckinOutOfMemoryError";
        break;
    case -402653086:
        result = "kAMDCheckinSendFailedError";
        break;
    case -402653147:
        result = "kAMDMissingPairRecordError";
        break;
    case -402653092:
        result = "kAMDInvalidPairRecordError";
        break;
    case -402653080:
        result = "kAMDSavePairRecordFailedError";
        break;
    case -402653146:
        result = "kAMDInvalidActivationRecordError";
        break;
    case -402653145:
        result = "kAMDMissingActivationRecordError";
        break;
    case -402653091:
        result = "kAMDServiceProhibitedError";
        break;
    case -402653055:
        result = "kAMDEscrowLockedError";
        break;
    case -402653054:
        result = "kAMDPairingProhibitedError";
        break;
    case -402653053:
        result = "kAMDProhibitedBySupervision";
        break;
    case -402653144:
        result = "kAMDWrongDroidError";
        break;
    case -402653143:
        result = "kAMDSUVerificationError";
        break;
    case -402653142:
        result = "kAMDSUPatchError";
        break;
    case -402653141:
        result = "kAMDSUFirmwareError";
        break;
    case -402653140:
        result = "kAMDProvisioningProfileNotValid";
        break;
    case -402653139:
        result = "kAMDSendMessageError";
        break;
    case -402653138:
        result = "kAMDReceiveMessageError";
        break;
    case -402653137:
        result = "kAMDMissingOptionsError";
        break;
    case -402653136:
        result = "kAMDMissingImageTypeError";
        break;
    case -402653135:
        result = "kAMDDigestFailedError";
        break;
    case -402653134:
        result = "kAMDStartServiceError";
        break;
    case -402653133:
        result = "kAMDInvalidDiskImageError";
        break;
    case -402653132:
        result = "kAMDMissingDigestError";
        break;
    case -402653131:
        result = "kAMDMuxError";
        break;
    case -402653130:
        result = "kAMDApplicationAlreadyInstalledError";
        break;
    case -402653129:
        result = "kAMDApplicationMoveFailedError";
        break;
    case -402653128:
        result = "kAMDApplicationSINFCaptureFailedError";
        break;
    case -402653127:
        result = "kAMDApplicationSandboxFailedError";
        break;
    case -402653126:
        result = "kAMDApplicationVerificationFailedError";
        break;
    case -402653125:
        result = "kAMDArchiveDestructionFailedError";
        break;
    case -402653124:
        result = "kAMDBundleVerificationFailedError";
        break;
    case -402653123:
        result = "kAMDCarrierBundleCopyFailedError";
        break;
    case -402653122:
        result = "kAMDCarrierBundleDirectoryCreationFailedError";
        break;
    case -402653121:
        result = "kAMDCarrierBundleMissingSupportedSIMsError";
        break;
    case -402653120:
        result = "kAMDCommCenterNotificationFailedError";
        break;
    case -402653119:
        result = "kAMDContainerCreationFailedError";
        break;
    case -402653118:
        result = "kAMDContainerP0wnFailedError";
        break;
    case -402653117:
        result = "kAMDContainerRemovalFailedError";
        break;
    case -402653116:
        result = "kAMDEmbeddedProfileInstallFailedError";
        break;
    case -402653115:
        result = "kAMDErrorError";
        break;
    case -402653114:
        result = "kAMDExecutableTwiddleFailedError";
        break;
    case -402653113:
        result = "kAMDExistenceCheckFailedError";
        break;
    case -402653112:
        result = "kAMDInstallMapUpdateFailedError";
        break;
    case -402653111:
        result = "kAMDManifestCaptureFailedError";
        break;
    case -402653110:
        result = "kAMDMapGenerationFailedError";
        break;
    case -402653109:
        result = "kAMDMissingBundleExecutableError";
        break;
    case -402653108:
        result = "kAMDMissingBundleIdentifierError";
        break;
    case -402653107:
        result = "kAMDMissingBundlePathError";
        break;
    case -402653106:
        result = "kAMDMissingContainerError";
        break;
    case -402653105:
        result = "kAMDNotificationFailedError";
        break;
    case -402653104:
        result = "kAMDPackageExtractionFailedError";
        break;
    case -402653103:
        result = "kAMDPackageInspectionFailedError";
        break;
    case -402653102:
        result = "kAMDPackageMoveFailedError";
        break;
    case -402653101:
        result = "kAMDPathConversionFailedError";
        break;
    case -402653100:
        result = "kAMDRestoreContainerFailedError";
        break;
    case -402653099:
        result = "kAMDSeatbeltProfileRemovalFailedError";
        break;
    case -402653098:
        result = "kAMDStageCreationFailedError";
        break;
    case -402653097:
        result = "kAMDSymlinkFailedError";
        break;
    case -402653096:
        result = "kAMDiTunesArtworkCaptureFailedError";
        break;
    case -402653095:
        result = "kAMDiTunesMetadataCaptureFailedError";
        break;
    case -402653094:
        result = "kAMDAlreadyArchivedError";
        break;
    case -402653082:
        result = "kAMDUnknownCommandError";
        break;
    case -402653081:
        result = "kAMDAPIInternalError";
        break;
    case -402653085:
        result = "kAMDMuxCreateListenerError";
        break;
    case -402653084:
        result = "kAMDMuxGetListenerError";
        break;
    case -402653083:
        result = "kAMDMuxConnectError";
        break;
    case -402653078:
        result = "kAMDDeviceTooNewError";
        break;
    case -402653077:
        result = "kAMDDeviceRefNoGood";
        break;
    case -402653076:
        result = "kAMDCannotTranslateError";
        break;
    case -402653075:
        result = "kAMDMobileImageMounterMissingImageSignature";
        break;
    case -402653074:
        result = "kAMDMobileImageMounterResponseCreationFailed";
        break;
    case -402653073:
        result = "kAMDMobileImageMounterMissingImageType";
        break;
    case -402653072:
        result = "kAMDMobileImageMounterMissingImagePath";
        break;
    case -402653071:
        result = "kAMDMobileImageMounterImageMapLoadFailed";
        break;
    case -402653070:
        result = "kAMDMobileImageMounterAlreadyMounted";
        break;
    case -402653069:
        result = "kAMDMobileImageMounterImageMoveFailed";
        break;
    case -402653068:
        result = "kAMDMobileImageMounterMountPathMissing";
        break;
    case -402653067:
        result = "kAMDMobileImageMounterMountPathNotEmpty";
        break;
    case -402653066:
        result = "kAMDMobileImageMounterImageMountFailed";
        break;
    case -402653065:
        result = "kAMDMobileImageMounterTrustCacheLoadFailed";
        break;
    case -402653064:
        result = "kAMDMobileImageMounterDigestFailed";
        break;
    case -402653063:
        result = "kAMDMobileImageMounterDigestCreationFailed";
        break;
    case -402653062:
        result = "kAMDMobileImageMounterImageVerificationFailed";
        break;
    case -402653061:
        result = "kAMDMobileImageMounterImageInfoCreationFailed";
        break;
    case -402653060:
        result = "kAMDMobileImageMounterImageMapStoreFailed";
        break;
    case -402653059:
        result = "kAMDBonjourSetupError";
        break;
    case -402653057:
        result = "kAMDNoWifiSyncSupportError";
        break;
    case -402653058:
        result = "kAMDDeviceOSVersionTooLow";
        break;
    case -402653056:
        result = "kAMDDeviceFamilyNotSupported";
        break;
    }

    return result;
}
