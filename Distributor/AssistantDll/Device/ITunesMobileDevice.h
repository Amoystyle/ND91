///////////////////////////////////////////////////////////
//  ITunesMobileDevice.h
//  iTunesMobileDevice.dll库的导出函数类
//  用于IOS设备的USB通讯
//  需要安装ITUNES，ITUNES版本不同，导出函数可能有增减
///////////////////////////////////////////////////////////

#pragma once
#pragma warning(disable:4200)
#include <Windows.h>
#include "iPhoneInterfaceDef.h"

typedef ULONGLONG afc_file_ref;
typedef unsigned int mach_error_t;
typedef unsigned int afc_error_t;

typedef enum eConnectNotificationMessage
{	
	///<summary>未知的状况</summary>
	ADNCI_MSG_UNKNOWN = 0,
	///<summary>iPhone 已经连接到计算机</summary> 
	ADNCI_MSG_CONNECTED = 1,
	///<summary>iPhone 已经从计算机中断开</summary> 
	ADNCI_MSG_DISCONNECTED = 2,
	///<summary>iTunes 环境卸载后收到的消息</summary> 
	ADNCI_MSG_UninstallItunes = 3,
};

 typedef struct am_device_notification_callback_info 
 {
 	void *dev;                              /* 0    device */ 
 	eConnectNotificationMessage msg;        /* 4    one of ADNCI_MSG_* */
 }am_device_notification_callback_info;

typedef void (*am_device_notification_callback)(struct am_device_notification_callback_info *);//, void* arg);
typedef void (*am_restore_device_notification_callback)(void* );

//iphoneEvents
typedef void (*PackDataTransferEventHandler)(void* ref);
typedef void (*PackInstallApplicationEventHandler)(void* sender, void* e);

typedef mach_error_t (*AMDeviceDeactivate)(struct am_device *device);
typedef mach_error_t (*AMDeviceActivate)(struct am_device *device, void* dict);

typedef mach_error_t (*AMDeviceRetain)(void* device);
typedef mach_error_t (*AMDeviceRelease)(void* device);
typedef mach_error_t (*AMDeviceConnect)(void* device);
typedef mach_error_t (*AMDeviceIsPaired)(void* device);
typedef mach_error_t (*AMDeviceValidatePairing)(void* device);
typedef mach_error_t (*AMDevicePair)(void*  hDevice);
typedef mach_error_t (*AMDeviceUnpair)(void*  hDevice);
typedef mach_error_t (*AMDeviceDisconnect)(void*  device);
typedef mach_error_t (*AMDeviceStopSession)(void*  device);
typedef mach_error_t (*AMDeviceStartSession)(void*  device);
typedef mach_error_t (*AMDeviceGetConnectionID)(void*  device);
typedef void* (*AMDeviceCopyDeviceIdentifier)(void*  device);
typedef mach_error_t (*AMDeviceStartService)(void *device, void * service_name, void **handle, UINT32 *unknown);
typedef mach_error_t (*AMDeviceSecureStartService)(void *device, void * service_name, void **unknown, void** secureHandle);  // add by qiuchang 2012.7.5

typedef void* (*AMDeviceCopyValue)(void* device, void * unknown, void * cfstring);
typedef mach_error_t (*AMDeviceSetValue)(void* device, unsigned int unknown, void* cfkey, void* cfValue);
typedef mach_error_t (*AMDeviceNotificationSubscribe)(am_device_notification_callback callback, UINT32 unused1, UINT32 unused2, UINT32 unused3, void **notification);
typedef mach_error_t (*AMDeviceTransferApplication)(void *sockAFC, void* cfStrPath, void* cfDictUnknow, PackDataTransferEventHandler callback, void* unknow);
typedef mach_error_t (*AMDeviceInstallApplication)(void *sockInstallProxy, void* cfStrPath, void* opts, PackInstallApplicationEventHandler callback, void* unknow);
typedef mach_error_t (*AMDeviceUninstallApplication)(void *sock, void* cfAppName, void* opts, void* callback, void* ukn);
typedef mach_error_t (*AMDeviceLookupApplications)(void* device, void* optsDict, void** outApps);
typedef mach_error_t (*AMDeviceArchiveApplication)(void *sockInstallProxy, void* cfIdentifier, void* opts, PackInstallApplicationEventHandler callback, void* unknow);
typedef mach_error_t (*AMDeviceRemoveApplicationArchive)(void *sockInstallProxy, void* cfIdentifier, void* opts, PackInstallApplicationEventHandler callback, void* unknow);
typedef mach_error_t (*AMDeviceRestoreApplication)(void *sockInstallProxy, void* cfIdentifier, void* opts, PackInstallApplicationEventHandler callback, void* unknow);
typedef mach_error_t (*AMDeviceUpgradeApplication)(void *sockInstallProxy, void* cfPackagePath, void* opts, PackInstallApplicationEventHandler callback, void* unknow);
typedef mach_error_t (*AMDeviceTransferPath)(void* device, void* cfPackagePath, void* opts, PackInstallApplicationEventHandler callback, void* unknow);
typedef mach_error_t (*AMDeviceNotificationUnsubscribe)(void* notification);
typedef mach_error_t (*AMDPostNotification)(void* socket, void* notification, void* userinfo);
typedef mach_error_t (*AMDeviceEnterRecover)(void* device);
typedef void* (*AMDCopyErrorText)(int errorCode);

typedef afc_error_t (*AFCDirectoryOpen)(void *conn, char* utf8_path, void **hAFDirHandle);
typedef afc_error_t (*AFCDirectoryRead)(void *conn, void *dir, char **dirent);
typedef afc_error_t (*AFCDirectoryClose)(void *conn, void *dir);
typedef afc_error_t (*AFCConnectionOpen)(void *handle, UINT32 io_timeout, void **conn);
typedef afc_error_t (*AFCConnectionClose)(void *conn);// 关闭连接 - 20081231 added -
typedef afc_error_t (*AFCFileInfoOpen)(void *conn, char* utf8_path, void **info);
typedef afc_error_t (*AFCKeyValueRead)(void *dict, char **key, char **value);
typedef afc_error_t (*AFCKeyValueClose)(void *dict);
typedef afc_error_t (*AFCRemovePath)(void *conn, char* utf8_path);
typedef afc_error_t (*AFCRenamePath)(void *conn, char* old_utf8_path, char* new_utf8_path);
typedef afc_error_t (*AFCLinkPath)(void *conn, int mode, int unknow, char* target, char* link);
typedef afc_error_t (*AFCFileRefOpen)(void *conn, char* utf8_path, afc_file_ref mode, afc_file_ref *ref);
typedef afc_error_t (*AFCFileRefLock)(void *conn, afc_file_ref ref);
typedef afc_error_t (*AFCFileRefUnlock)(void *conn, afc_file_ref ref);
typedef afc_error_t (*AFCFileRefClose)(void *conn, afc_file_ref ref);
typedef afc_error_t (*AFCFileRefRead)(void *conn, afc_file_ref ref, void* buffer, UINT32 *len);
typedef afc_error_t (*AFCFileRefWrite)(void *conn, afc_file_ref ref, void* buffer, UINT32 len);
typedef afc_error_t (*AFCFileRefSeek)(void *conn, afc_file_ref ref, afc_file_ref pos, afc_file_ref origin);
typedef afc_error_t (*AFCDirectoryCreate)(void *conn, char *dirname);
typedef afc_error_t (*AFCGetFileInfo)(void *conn, char* utf8_path, void **info, UINT32 *length);
typedef afc_error_t (*AFCFlushData)(void *conn, afc_file_ref handle);
typedef afc_error_t (*AFCFileRefTell)(void *conn, afc_file_ref handle, UINT32 *position);
typedef afc_error_t (*AFCFileRefSetFileSize)(void *conn, afc_file_ref handle, UINT32 size);
typedef void* (*AFCOperationCreateGetFileHash)(void* allocator, void* cfstringFileName, void* afcContext);
typedef void* (*AFCOperationCreateGetFileHashWithRange)(void* allocator, void* cfstringFileName, void* unknow1,
                                                        void* unknow2, long endPosition, void* unknow3, void* unknow4);
typedef void* (*AFCOperationGetContext)(void* afcHandle);
typedef afc_error_t (*AFCConnectionGetIOTimeout)(void* afcHandle);
typedef afc_error_t (*AFCConnectionProcessOperation)(void* afcHandle, void* op, UINT32 timeout);
typedef afc_error_t (*AFCOperationGetResultStatus)(void* op);
typedef void* (*AFCOperationGetResultObject)(void* op);
typedef void* (*AFCConnectionCopyLastErrorInfo)(void* afcHandle);

typedef bool (*AFCConnectionIsValid)(void* afcHandle);

typedef UINT32 (*AMRestoreRegisterForDeviceNotifications)(
	am_restore_device_notification_callback dfu_connect_callback,
	am_restore_device_notification_callback recovery_connect_callback,
	am_restore_device_notification_callback dfu_disconnect_callback,
	am_restore_device_notification_callback recovery_disconnect_callback,
	unsigned int unknown0,
	void *user_info);
typedef INT32 (*AMRestoreModeDeviceCreate)(UINT32 unknown0, UINT32 connection_id, UINT32 unknown1);
typedef void* (*AMRecoveryModeDeviceCopyIMEI)(void* device);
typedef void* (*AMRecoveryModeDeviceCopySerialNumber)(void* device);
typedef void* (*AMRecoveryModeDeviceGetLocationID)(void* device);
typedef void* (*AMRecoveryModeDeviceGetProductID)(void* device);
typedef INT32 (*AMRecoveryModeDeviceGetProductType)(void* device);
typedef void* (*AMRecoveryModeGetSoftwareBuildVersion)(void* device);
typedef INT32 (*AMRecoveryModeDeviceGetTypeID)(void* device);
typedef INT32 (*AMRecoveryModeDeviceSetAutoBoot)(void* device, char flag);
typedef INT32 (*AMRecoveryModeDeviceReboot)(void* device);

class CITunesMobileDevice  
{
public:
	CITunesMobileDevice();
	virtual ~CITunesMobileDevice();

	static bool MyLoadLibrary();
	static void MyFreeLibrary();

	static mach_error_t iTunes_AMDeviceDeactivate(struct am_device *device);
	static mach_error_t iTunes_AMDeviceActivate(struct am_device *device, void* dict);

	static mach_error_t iTunes_AMDeviceRetain(void* device);
	static mach_error_t iTunes_AMDeviceRelease(void* device);
	static mach_error_t iTunes_AMDeviceConnect(void* device);
	static mach_error_t iTunes_AMDeviceIsPaired(void* device);
	static mach_error_t iTunes_AMDeviceValidatePairing(void* device);
	static mach_error_t iTunes_AMDevicePair(void*  hDevice);
	static mach_error_t iTunes_AMDeviceUnpair(void*  hDevice);
	static mach_error_t iTunes_AMDeviceDisconnect(void*  device);
	static mach_error_t iTunes_AMDeviceStopSession(void*  device);
	static mach_error_t iTunes_AMDeviceStartSession(void*  device);
	static mach_error_t iTunes_AMDeviceGetConnectionID(void*  device);
	static void* iTunes_AMDeviceCopyDeviceIdentifier(void*  device);
	static mach_error_t iTunes_AMDeviceStartService(void *device, void * service_name, void **handle, UINT32 *unknown);
    static mach_error_t iTunes_AMDeviceSecureStartService(void *device, void * service_name, void **handle, void** secureHandle);

	static void*	 iTunes_AMDeviceCopyValue(void* device, void * unknown, void * cfstring);
	static mach_error_t iTunes_AMDeviceSetValue(void* device, unsigned int unknown, void* cfkey, void* cfValue);	
	static mach_error_t iTunes_AMDeviceNotificationSubscribe(am_device_notification_callback callback, UINT32 unused1, UINT32 unused2, UINT32 unused3, void **notification);
	static mach_error_t iTunes_AMDeviceNotificationUnsubscribe(void* nitification);
	static mach_error_t iTunes_AMDeviceTransferApplication(void *sockAFC, void* cfStrPath, void* cfDictUnknow, PackDataTransferEventHandler callback, void* unknow);
	static mach_error_t iTunes_AMDeviceInstallApplication(void *sockInstallProxy, void* cfStrPath, void* opts, PackInstallApplicationEventHandler callback, void* unknow);
	static mach_error_t iTunes_AMDeviceUninstallApplication(void *sock, void* cfAppName, void* opts, void* callback, void* ukn);
	static mach_error_t iTunes_AMDeviceLookupApplications(void* device, void* optsDict, void** outApps);
	static mach_error_t iTunes_AMDeviceArchiveApplication(void *sockInstallProxy, void* cfIdentifier, void* opts, PackInstallApplicationEventHandler callback, void* unknow);
	static mach_error_t iTunes_AMDeviceRemoveApplicationArchive(void *sockInstallProxy, void* cfIdentifier, void* opts, PackInstallApplicationEventHandler callback, void* unknow);
	static mach_error_t iTunes_AMDeviceRestoreApplication(void *sockInstallProxy, void* cfIdentifier, void* opts, PackInstallApplicationEventHandler callback, void* unknow);
	static mach_error_t iTunes_AMDeviceUpgradeApplication(void *sockInstallProxy, void* cfPackagePath, void* opts, PackInstallApplicationEventHandler callback, void* unknow);
	static mach_error_t iTunes_AMDeviceTransferPath(void* device, void* cfPackagePath, void* opts, PackInstallApplicationEventHandler callback, void* unknow);
    static mach_error_t iTunes_AMDPostNotification(void* socket, const char* notification, const char* userinfo);
	static mach_error_t iTunes_AMDeviceEnterRecover(void *handle);
    static void* iTunes_AMDCopyErrorText(int errorCode);

	static afc_error_t iTunes_AFCDirectoryOpen(void *conn, const wchar_t* path, void **hAFDirHandle);
	static afc_error_t iTunes_AFCDirectoryRead(void *conn, void *dir, char **dirent);
	static afc_error_t iTunes_AFCDirectoryClose(void *conn, void *dir);
	static afc_error_t iTunes_AFCConnectionOpen(void *handle, UINT32 io_timeout, void **conn);
	static afc_error_t iTunes_AFCConnectionClose(void *conn);// 关闭连接 - 20081231 added -
	static afc_error_t iTunes_AFCFileInfoOpen(void *conn, const wchar_t* path, void **info);
	static afc_error_t iTunes_AFCKeyValueRead(void *dict, char **key, char **value);
	static afc_error_t iTunes_AFCKeyValueClose(void *dict);
	static afc_error_t iTunes_AFCRemovePath(void *conn, const wchar_t* path);
	static afc_error_t iTunes_AFCRenamePath(void *conn, const wchar_t* oldPath, const wchar_t* newPath);
    static afc_error_t iTunes_AFCLinkPath(void *conn, int mode, int unknow, const wchar_t* target, const wchar_t* link);

    // Summary  : Opens file for reading or writing without locking it in any way. afc_file_ref should not be shared between threads - * opening file in one thread and closing it in another will lead to possible crash.
    // Returns  : 
    // Parameter: void * conn 
    // Parameter: path : absolute path to file (convert to UTF-8 encoded)
    // Parameter: mode : mode  2 = read, mode 3 = write; unknown = 0 
    // RDONLY   = 0x00000001,       // 只读,  r   O_RDONLY 
    // RD       = 0x00000002,       // 读写权限,  r+  O_RDWR   | O_CREAT
    // WRONLY   = 0x00000003,       // 只写权限同时截断文件, w   O_WRONLY | O_CREAT  | O_TRUNC
    // WR       = 0x00000004,       // 又读又写同时截断,w+  O_RDWR   | O_CREAT  | O_TRUNC 
    // APPEND   = 0x00000005,       // 追加只写,a   O_WRONLY | O_APPEND | O_CREAT
    // RDAPPEND = 0x00000006        // 追加读写,a+  O_RDWR   | O_APPEND | O_CREAT
    static afc_error_t iTunes_AFCFileRefOpen(void *conn, const wchar_t* path,  afc_file_ref mode, afc_file_ref *ref);
    static afc_error_t iTunes_AFCFileRefLock(void *conn, afc_file_ref ref);
    static afc_error_t iTunes_AFCFileRefUnlock(void *conn, afc_file_ref ref);
	static afc_error_t iTunes_AFCFileRefClose(void *conn, afc_file_ref ref);
	static afc_error_t iTunes_AFCFileRefRead(void *conn, afc_file_ref ref, void* buffer, UINT32 *len);
	static afc_error_t iTunes_AFCFileRefWrite(void *conn, afc_file_ref ref, void* buffer, UINT32 len);
	static afc_error_t iTunes_AFCFileRefSeek(void *conn, afc_file_ref ref, afc_file_ref pos, afc_file_ref origin);
	static afc_error_t iTunes_AFCDirectoryCreate(void *conn, const wchar_t* folder);
	static afc_error_t iTunes_AFCGetFileInfo(void *conn, const wchar_t* path, void **info, UINT32 *length);
	static afc_error_t iTunes_AFCFlushData(void *conn, afc_file_ref handle);
	static afc_error_t iTunes_AFCFileRefTell(void *conn, afc_file_ref handle, UINT32 *position);
	static afc_error_t iTunes_AFCFileRefSetFileSize(void *conn, afc_file_ref handle, UINT32 size);
    // 创建获取文件sha1的afcOperation
    static void* iTunes_AFCOperationCreateGetFileHash(void* allocator, void* cfstringFileName, void* afcContext);
    // 创建获取文件块sha1的afcOperation
    static void* iTunes_AFCOperationCreateGetFileHashWithRange(void* allocator, void* cfstringFileName, void* unknow1,
                                                                void* unknow2, long endPosition, void* unknow3, void* unknow4);

    static void* iTunes_AFCOperationGetContext(void* afcHandle);
    static afc_error_t iTunes_AFCConnectionGetIOTimeout(void* afcHandle);
    static afc_error_t iTunes_AFCConnectionProcessOperation(void* afcHandle, void* op, int timeout);
    static afc_error_t iTunes_AFCOperationGetResultStatus(void* op);
    static void* iTunes_AFCOperationGetResultObject(void* op);
    static void* iTunes_AFCConnectionCopyLastErrorInfo(void* afcHandle);

    static bool iTunes_AFCConnectionIsValid(void* afcHandle);

	static UINT32 iTunes_AMRestoreRegisterForDeviceNotifications(
		am_restore_device_notification_callback dfu_connect_callback,
		am_restore_device_notification_callback recovery_connect_callback,
		am_restore_device_notification_callback dfu_disconnect_callback,
		am_restore_device_notification_callback recovery_disconnect_callback,
		unsigned int unknown0,
		void *user_info);
	static INT32 iTunes_AMRestoreModeDeviceCreate(UINT32 unknown0, UINT32 connection_id, UINT32 unknown1);
	static void* iTunes_AMRecoveryModeDeviceCopyIMEI(void* device);
	static void* iTunes_AMRecoveryModeDeviceCopySerialNumber(void* device);
	static void* iTunes_AMRecoveryModeDeviceGetLocationID(void* device);
	static void* iTunes_AMRecoveryModeDeviceGetProductID(void* device);
	static INT32 iTunes_AMRecoveryModeDeviceGetProductType(void* device);
	static void* iTunes_AMRecoveryModeGetSoftwareBuildVersion(void* device);
	static INT32 iTunes_AMRecoveryModeDeviceGetTypeID(void* device);
	static INT32 iTunes_AMRecoveryModeDeviceSetAutoBoot(void* device, char flag);
	static INT32 iTunes_AMRecoveryModeDeviceReboot(void* device);		

	static void CloseSocket(void* socket);

    // 获取类似-402653125 错误码对应的文本描述
    static string iTunes_GetDeviceErrorCodeString(int errorCode);

    // 获取和设备交互过程中的错误码对应的错误文本(查询不到的时候,返回空)
    static string GetAMDErrorText(int errorCode);

private:

	static HINSTANCE m_ndiPhoneUSB;
	static AMDeviceDeactivate m_AMDeviceDeactivate;
	static AMDeviceActivate m_AMDeviceActivate;
	static AMDeviceRetain  m_AMDeviceRetain;
	static AMDeviceRelease  m_AMDeviceRelease;	
	static AMDeviceConnect m_AMDeviceConnect;
	static AMDeviceIsPaired m_AMDeviceIsPaired;	
	static AMDeviceValidatePairing m_AMDeviceValidatePairing;	
	static AMDevicePair m_AMDevicePair;	
	static AMDeviceUnpair m_AMDeviceUnpair;	
	static AMDeviceDisconnect m_AMDeviceDisconnect;	
	static AMDeviceStopSession m_AMDeviceStopSession;	
	static AMDeviceStartSession m_AMDeviceStartSession;	
	static AMDeviceGetConnectionID m_AMDeviceGetConnectionID;	
	static AMDeviceCopyDeviceIdentifier m_AMDeviceCopyDeviceIdentifier;	
	static AMDeviceStartService m_AMDeviceStartService;	
    static AMDeviceSecureStartService m_AMDeviceSecureStartService;	

	static AMDeviceCopyValue m_AMDeviceCopyValue;	
	static AMDeviceSetValue m_AMDeviceSetValue;	
	static AMDeviceTransferApplication m_AMDeviceTransferApplication;	
	static AMDeviceInstallApplication m_AMDeviceInstallApplication;	
	static AMDeviceUninstallApplication m_AMDeviceUninstallApplication; 	
	static AMDeviceNotificationSubscribe m_AMDeviceNotificationSubscribe;	
	static AMDeviceNotificationUnsubscribe m_AMDeviceNotificationUnsubscribe;
	static AMDeviceLookupApplications m_AMDeviceLookupApplications;	
	static AMRestoreModeDeviceCreate m_AMRestoreModeDeviceCreate;	
    static AMDPostNotification m_AMDPostNotification;
	static AMDeviceEnterRecover m_AMDeviceEnterRecover;

	static AFCDirectoryOpen m_AFCDirectoryOpen;	
	static AFCDirectoryRead m_AFCDirectoryRead;
	static AFCDirectoryClose m_AFCDirectoryClose;	
	static AMRestoreRegisterForDeviceNotifications m_AMRestoreRegisterForDeviceNotifications;	
	static AFCConnectionOpen m_AFCConnectionOpen;	
	// 关闭连接 - 20081231 added -
	static AFCConnectionClose m_AFCConnectionClose;	
	static AFCGetFileInfo m_AFCGetFileInfo;	
	static AFCFileInfoOpen m_AFCFileInfoOpen;
	static AFCKeyValueRead m_AFCKeyValueRead;	
	static AFCKeyValueClose m_AFCKeyValueClose;	
	static AFCRemovePath m_AFCRemovePath;	
	static AFCRenamePath m_AFCRenamePath;
    static AFCLinkPath m_AFCLinkPath;
	static AFCFileRefOpen m_AFCFileRefOpen;	
    static AFCFileRefLock m_AFCFileRefLock;	
    static AFCFileRefUnlock m_AFCFileRefUnlock;	
	static AFCFileRefClose m_AFCFileRefClose;	
	static AFCFileRefRead m_AFCFileRefRead;	
	static AFCFileRefWrite m_AFCFileRefWrite;
	static AFCFlushData m_AFCFlushData;	
	static AFCFileRefSeek m_AFCFileRefSeek;	
	static AFCFileRefTell m_AFCFileRefTell;	
	static AFCFileRefSetFileSize m_AFCFileRefSetFileSize;
    static AFCOperationCreateGetFileHash m_AFCOperationCreateGetFileHash;
    static AFCOperationCreateGetFileHashWithRange m_AFCOperationCreateGetFileHashWithRange;
    static AFCOperationGetContext m_AFCOperationGetContext;
    static AFCConnectionGetIOTimeout m_AFCConnectionGetIOTimeout;
    static AFCConnectionProcessOperation m_AFCConnectionProcessOperation;
    static AFCOperationGetResultStatus m_AFCOperationGetResultStatus;
    static AFCOperationGetResultObject m_AFCOperationGetResultObject;
    static AFCConnectionCopyLastErrorInfo m_AFCConnectionCopyLastErrorInfo;
    static AFCConnectionIsValid m_AFCConnectionIsValid;

	static AFCDirectoryCreate m_AFCDirectoryCreate;	
	static AMRecoveryModeDeviceCopyIMEI m_AMRecoveryModeDeviceCopyIMEI;	
	static AMRecoveryModeDeviceCopySerialNumber m_AMRecoveryModeDeviceCopySerialNumber;	
	static AMRecoveryModeDeviceGetLocationID m_AMRecoveryModeDeviceGetLocationID;	
	static AMRecoveryModeDeviceGetProductID m_AMRecoveryModeDeviceGetProductID;	
	static AMRecoveryModeDeviceGetProductType m_AMRecoveryModeDeviceGetProductType;	
	static AMRecoveryModeGetSoftwareBuildVersion m_AMRecoveryModeGetSoftwareBuildVersion;	
	static AMRecoveryModeDeviceGetTypeID m_AMRecoveryModeDeviceGetTypeID;	
	static AMRecoveryModeDeviceSetAutoBoot m_AMRecoveryModeDeviceSetAutoBoot;	
	static AMRecoveryModeDeviceReboot m_AMRecoveryModeDeviceReboot;	
	static AMDeviceArchiveApplication m_AMDeviceArchiveApplication;
	static AMDeviceRemoveApplicationArchive m_AMDeviceRemoveApplicationArchive;	
	static AMDeviceRestoreApplication m_AMDeviceRestoreApplication;	
	static AMDeviceUpgradeApplication m_AMDeviceUpgradeApplication;	
	static AMDeviceTransferPath m_AMDeviceTransferPath;
    static AMDCopyErrorText m_AMDCopyErrorText;
};
