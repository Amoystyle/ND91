/* ----------------------------------------------------------------------------
 *   MobileDevice.h - interface to MobileDevice.framework 
 *   $LastChangedDate: 2007-07-09 18:59:29 -0700 (Mon, 09 Jul 2007) $
 *
 * Copied from http://iphonesvn.halifrag.com/svn/iPhone/
 * With modifications from Allen Porter and Scott Turner
 *
 * ------------------------------------------------------------------------- */
#ifndef MOBILEDEVICE_H
#define MOBILEDEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32)
#include <CoreFoundation.h>
typedef unsigned int mach_error_t;
#elif defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#include <mach/error.h>
#endif	

/* Error codes */
#define MDERR_APPLE_MOBILE  (err_system(0x3a))
#define MDERR_IPHONE        (err_sub(0))

/* Apple Mobile (AM*) errors */
#define MDERR_OK                ERR_SUCCESS
#define MDERR_SYSCALL           (ERR_MOBILE_DEVICE | 0x01)
#define MDERR_OUT_OF_MEMORY     (ERR_MOBILE_DEVICE | 0x03)
#define MDERR_QUERY_FAILED      (ERR_MOBILE_DEVICE | 0x04) 
#define MDERR_INVALID_ARGUMENT  (ERR_MOBILE_DEVICE | 0x0b)
#define MDERR_DICT_NOT_LOADED   (ERR_MOBILE_DEVICE | 0x25)

/* Apple File Connection (AFC*) errors */
#define MDERR_AFC_OUT_OF_MEMORY 0x03

/* USBMux errors */
#define MDERR_USBMUX_ARG_NULL   0x16
#define MDERR_USBMUX_FAILED     0xffffffff

/* Messages passed to device notification callbacks: passed as part of
 * am_device_notification_callback_info. */
#define ADNCI_MSG_UNKNOWN       				0 
#define ADNCI_MSG_CONNECTED     				1
#define ADNCI_MSG_DISCONNECTED  				2
#define ADNCI_MSG_UninstallItunes       3

#define AMD_IPHONE_PRODUCT_ID   0x1290
#define AMD_IPHONE_SERIAL       "3391002d9c804d105e2c8c7d94fc35b6f3d214a3"

/* Services, found in /System/Library/Lockdown/Services.plist */
#define AMSVC_AFC                   CFSTR("com.apple.afc")
#define AMSVC_BACKUP                CFSTR("com.apple.mobilebackup")
#define AMSVC_CRASH_REPORT_COPY     CFSTR("com.apple.crashreportcopy")
#define AMSVC_DEBUG_IMAGE_MOUNT     CFSTR("com.apple.mobile.debug_image_mount")
#define AMSVC_NOTIFICATION_PROXY    CFSTR("com.apple.mobile.notification_proxy")
#define AMSVC_PURPLE_TEST           CFSTR("com.apple.purpletestr")
#define AMSVC_SOFTWARE_UPDATE       CFSTR("com.apple.mobile.software_update")
#define AMSVC_SYNC                  CFSTR("com.apple.mobilesync")
#define AMSVC_SCREENSHOT            CFSTR("com.apple.screenshotr")
#define AMSVC_SYSLOG_RELAY          CFSTR("com.apple.syslog_relay")
#define AMSVC_SYSTEM_PROFILER       CFSTR("com.apple.mobile.system_profiler")

typedef unsigned int afc_error_t;
typedef unsigned int usbmux_error_t;
//typedef unsigned int service_conn_t;
typedef void* service_conn_t;

struct am_recovery_device;

typedef struct am_device_notification_callback_info {
    /*struct am_device*/void *dev;  /* 0    device */ 
    unsigned int msg;       /* 4    one of ADNCI_MSG_* */
} __attribute__ ((packed)) am_device_notification_callback_info;

/* The type of the device restore notification callback functions.
 * TODO: change to correct type. */
//typedef void (*am_restore_device_notification_callback)(struct am_recovery_device *);
typedef void (*am_restore_device_notification_callback)(void*);

/* This is a CoreFoundation object of class AMRecoveryModeDevice. */
typedef struct am_recovery_device {
    unsigned char unknown0[8];                          /* 0 */
    am_restore_device_notification_callback callback;   /* 8 */
    void *user_info;                                    /* 12 */
    unsigned char unknown1[12];                         /* 16 */
    unsigned int readwrite_pipe;                        /* 28 */
    unsigned char read_pipe;                            /* 32 */
    unsigned char write_ctrl_pipe;                      /* 33 */
    unsigned char read_unknown_pipe;                    /* 34 */
    unsigned char write_file_pipe;                      /* 35 */
    unsigned char write_input_pipe;                     /* 36 */
} __attribute__ ((packed)) am_recovery_device;

/* A CoreFoundation object of class AMRestoreModeDevice. */
typedef struct am_restore_device {
    unsigned char unknown[32];
    int port;
} __attribute__ ((packed)) am_restore_device;

/* The type of the device notification callback function. */
typedef void(*am_device_notification_callback)(struct am_device_notification_callback_info*);//, void* arg);

/* The type of the _AMDDeviceAttached function.
 * TODO: change to correct type. */
typedef void *amd_device_attached_callback;

 
typedef struct am_device {
    unsigned char unknown0[16]; /* 0 - zero */
    unsigned int device_id;     /* 16 */
    unsigned int product_id;    /* 20 - set to AMD_IPHONE_PRODUCT_ID */
    char *serial;               /* 24 - set to AMD_IPHONE_SERIAL */
    unsigned int unknown1;      /* 28 */
    unsigned char unknown2[4];  /* 32 */
    unsigned int lockdown_conn; /* 36 */
    unsigned char unknown3[8];  /* 40 */
} __attribute__ ((packed)) am_device;

typedef struct am_device_notification {
    unsigned int unknown0;                      /* 0 */
    unsigned int unknown1;                      /* 4 */
    unsigned int unknown2;                      /* 8 */
    am_device_notification_callback callback;   /* 12 */ 
    unsigned int unknown3;                      /* 16 */
} __attribute__ ((packed)) am_device_notification;

typedef struct afc_connection {
    unsigned int handle;            /* 0 */
    unsigned int unknown0;          /* 4 */
    unsigned char unknown1;         /* 8 */
    unsigned char padding[3];       /* 9 */
    unsigned int unknown2;          /* 12 */
    unsigned int unknown3;          /* 16 */
    unsigned int unknown4;          /* 20 */
    unsigned int fs_block_size;     /* 24 */
    unsigned int sock_block_size;   /* 28: always 0x3c */
    unsigned int io_timeout;        /* 32: from AFCConnectionOpen, usu. 0 */
    void *afc_lock;                 /* 36 */
    unsigned int context;           /* 40 */
} __attribute__ ((packed)) afc_connection;

typedef struct afc_directory {
    unsigned char unknown[0];   /* size unknown */
} __attribute__ ((packed)) afc_directory;

typedef struct afc_dictionary {
    unsigned char unknown[0];   /* size unknown */
} __attribute__ ((packed)) afc_dictionary;

typedef unsigned long afc_file_ref;

typedef struct usbmux_listener_1 {                  /* offset   value in iTunes */
    unsigned int unknown0;                  /* 0        1 */
    unsigned char *unknown1;                /* 4        ptr, maybe device? */
    amd_device_attached_callback callback;  /* 8        _AMDDeviceAttached */
    unsigned int unknown3;                  /* 12 */
    unsigned int unknown4;                  /* 16 */
    unsigned int unknown5;                  /* 20 */
} __attribute__ ((packed)) usbmux_listener_1;

typedef struct usbmux_listener_2 {
    unsigned char unknown0[4144];
} __attribute__ ((packed)) usbmux_listener_2;

typedef struct am_bootloader_control_packet {
    unsigned char opcode;       /* 0 */
    unsigned char length;       /* 1 */
    unsigned char magic[2];     /* 2: 0x34, 0x12 */
    unsigned char payload[0];   /* 4 */
} __attribute__ ((packed)) am_bootloader_control_packet;

/* ----------------------------------------------------------------------------
 *   Public routines
 * ------------------------------------------------------------------------- */

void AMDSetLogLevel(int level);

/*  Registers a notification with the current run loop. The callback gets
 *  copied into the notification struct, as well as being registered with the
 *  current run loop. dn_unknown3 gets copied into unknown3 in the same.
 *  (Maybe dn_unknown3 is a user info parameter that gets passed as an arg to
 *  the callback?) unused0 and unused1 are both 0 when iTunes calls this.
 *  In iTunes the callback is located from $3db78e-$3dbbaf.
 *
 *  Returns:
 *      MDERR_OK            if successful
 *      MDERR_SYSCALL       if CFRunLoopAddSource() failed
 *      MDERR_OUT_OF_MEMORY if we ran out of memory
 */

mach_error_t AMDeviceNotificationSubscribe(am_device_notification_callback
    callback, unsigned int unused0, unsigned int unused1, unsigned int //void*
    dn_unknown3, struct am_device_notification **notification);

//  Unregisters notification
mach_error_t AMDeviceNotificationUnsubscribe(struct am_device_notification *notification);

/*  Connects to the iPhone. Pass in the am_device structure that the
 *  notification callback will give to you.
 *
 *  Returns:
 *      MDERR_OK                if successfully connected
 *      MDERR_SYSCALL           if setsockopt() failed
 *      MDERR_QUERY_FAILED      if the daemon query failed
 *      MDERR_INVALID_ARGUMENT  if USBMuxConnectByPort returned 0xffffffff
 */

mach_error_t AMDeviceConnect(/*struct am_device*/void *device);

/*  Calls PairingRecordPath() on the given device, than tests whether the path
 *  which that function returns exists. During the initial connect, the path
 *  returned by that function is '/', and so this returns 1.
 *
 *  Returns:
 *      0   if the path did not exist
 *      1   if it did
 */

int AMDeviceIsPaired(/*struct am_device*/void *device);

/*  iTunes calls this function immediately after testing whether the device is
 *  paired. It creates a pairing file and establishes a Lockdown connection.
 *
 *  Returns:
 *      MDERR_OK                if successful
 *      MDERR_INVALID_ARGUMENT  if the supplied device is null
 *      MDERR_DICT_NOT_LOADED   if the load_dict() call failed
 */

mach_error_t AMDeviceValidatePairing(/*struct am_device*/void *device);

/*  Creates a Lockdown session and adjusts the device structure appropriately
 *  to indicate that the session has been started. iTunes calls this function
 *  after validating pairing.
 *
 *  Returns:
 *      MDERR_OK                if successful
 *      MDERR_INVALID_ARGUMENT  if the Lockdown conn has not been established
 *      MDERR_DICT_NOT_LOADED   if the load_dict() call failed
 */

mach_error_t AMDeviceStartSession(/*struct am_device*/void *device);

/* Starts a service and returns a handle that can be used in order to further
 * access the service. You should stop the session and disconnect before using
 * the service. iTunes calls this function after starting a session. It starts 
 * the service and the SSL connection. unknown may safely be
 * NULL (it is when iTunes calls this), but if it is not, then it will be
 * filled upon function exit. service_name should be one of the AMSVC_*
 * constants. If the service is AFC (AMSVC_AFC), then the handle is the handle
 * that will be used for further AFC* calls.
 *
 * Returns:
 *      MDERR_OK                if successful
 *      MDERR_SYSCALL           if the setsockopt() call failed
 *      MDERR_INVALID_ARGUMENT  if the Lockdown conn has not been established
 */

mach_error_t AMDeviceStartService(/*struct am_device*/void *device, /*CFStringRef*/void* service_name, /*service_conn_t*/void* *handle, unsigned int *unknown);

mach_error_t AMDeviceStartHouseArrestService(/*struct am_device*/void *device, CFStringRef identifier, void *unknown, service_conn_t *handle, unsigned int *what);

/* Stops a session. You should do this before accessing services.
 *
 * Returns:
 *      MDERR_OK                if successful
 *      MDERR_INVALID_ARGUMENT  if the Lockdown conn has not been established
 */

mach_error_t AMDeviceStopSession(/*struct am_device*/void *device);

/* Opens an Apple File Connection. You must start the appropriate service
 * first with AMDeviceStartService(). In iTunes, io_timeout is 0.
 *
 * Returns:
 *      MDERR_OK                if successful
 *      MDERR_AFC_OUT_OF_MEMORY if malloc() failed
 */

afc_error_t AFCConnectionOpen(service_conn_t handle, unsigned int io_timeout, /*struct afc_connection*/void **conn);

/* Pass in a pointer to an afc_device_info structure. It will be filled. */
afc_error_t AFCDeviceInfoOpen(afc_connection *conn, struct afc_dictionary **info);

/* Turns debug mode on if the environment variable AFCDEBUG is set to a numeric
 * value, or if the file '/AFCDEBUG' is present and contains a value. */
void AFCPlatformInit();

/* Opens a directory on the iPhone. Pass in a pointer in dir to be filled in.
 * Note that this normally only accesses the iTunes sandbox/partition as the
 * root, which is /var/root/Media. Pathnames are specified with '/' delimiters
 * as in Unix style.
 *
 * Returns:
 *      MDERR_OK                if successful
 */

//afc_error_t AFCDirectoryOpen(afc_connection *conn, const char *path,struct afc_directory **dir);
afc_error_t AFCDirectoryOpen(void *conn, const char* path, struct afc_directory** dir);
/* Acquires the next entry in a directory previously opened with
 * AFCDirectoryOpen(). When dirent is filled with a NULL value, then the end
 * of the directory has been reached. '.' and '..' will be returned as the
 * first two entries in each directory except the root; you may want to skip
 * over them.
 *
 * Returns:
 *      MDERR_OK                if successful, even if no entries remain
 */

afc_error_t AFCDirectoryRead(/*afc_connection*/void *conn/*unsigned int unused*/, struct afc_directory *dir,char **dirent);
afc_error_t AFCDirectoryClose(/*afc_connection*/void *conn, struct afc_directory *dir);
//afc_error_t AFCDirectoryCreate(afc_connection *conn, const char *dirname);
afc_error_t AFCDirectoryCreate(void *conn, const char *dirname);
afc_error_t AFCRemovePath(/*afc_connection*/void *conn, const char *dirname);
//afc_error_t AFCRenamePath(afc_connection *conn, const char *from, const char *to);
afc_error_t AFCRenamePath(void *conn, const char *from, const char *to);
afc_error_t AFCLinkPath(/*afc_connection*/void *conn, long long int linktype, const char *target, const char *linkname);

/* Returns the context field of the given AFC connection. */
unsigned int AFCConnectionGetContext(/*afc_connection*/void *conn);

/* Returns the fs_block_size field of the given AFC connection. */
unsigned int AFCConnectionGetFSBlockSize(/*afc_connection*/void *conn);

/* Returns the io_timeout field of the given AFC connection. In iTunes this is
 * 0. */
unsigned int AFCConnectionGetIOTimeout(/*afc_connection*/void *conn);

/* Returns the sock_block_size field of the given AFC connection. */
unsigned int AFCConnectionGetSocketBlockSize(/*afc_connection*/void *conn);

/* Closes the given AFC connection. */
afc_error_t AFCConnectionClose(/*afc_connection*/void *conn);

/* Registers for device notifications related to the restore process. unknown0
 * is zero when iTunes calls this. In iTunes,
 * the callbacks are located at:
 *      1: $3ac68e-$3ac6b1, calls $3ac542(unknown1, arg, 0)
 *      2: $3ac66a-$3ac68d, calls $3ac542(unknown1, 0, arg)
 *      3: $3ac762-$3ac785, calls $3ac6b2(unknown1, arg, 0)
 *      4: $3ac73e-$3ac761, calls $3ac6b2(unknown1, 0, arg)
 */

unsigned int AMRestoreRegisterForDeviceNotifications(
    am_restore_device_notification_callback dfu_connect_callback,
    am_restore_device_notification_callback recovery_connect_callback,
    am_restore_device_notification_callback dfu_disconnect_callback,
    am_restore_device_notification_callback recovery_disconnect_callback,
    unsigned int unknown0,
    void *user_info);

/* Causes the restore functions to spit out (unhelpful) progress messages to
 * the file specified by the given path. iTunes always calls this right before
 * restoring with a path of
 * "$HOME/Library/Logs/iPhone Updater Logs/iPhoneUpdater X.log", where X is an
 * unused number.
 */

unsigned int AMRestoreEnableFileLogging(char *path);

/* Initializes a new option dictionary to default values. Pass the constant
 * kCFAllocatorDefault as the allocator. The option dictionary looks as
 * follows:
 * {
 *      NORImageType => 'production',
 *      AutoBootDelay => 0,
 *      KernelCacheType => 'Release',
 *      UpdateBaseband => true,
 *      DFUFileType => 'RELEASE',
 *      SystemImageType => 'User',
 *      CreateFilesystemPartitions => true,
 *      FlashNOR => true,
 *      RestoreBootArgs => 'rd=md0 nand-enable-reformat=1 -progress'
 *      BootImageType => 'User'
 *  }
 *
 * Returns:
 *      the option dictionary   if successful
 *      NULL                    if out of memory
 */ 

CFMutableDictionaryRef AMRestoreCreateDefaultOptions(CFAllocatorRef allocator);

/* ----------------------------------------------------------------------------
 *   Less-documented public routines
 * ------------------------------------------------------------------------- */

/* mode 2 = read, mode 3 = write */
//afc_error_t AFCFileRefOpen(afc_connection *conn, const char *path,unsigned long long mode, afc_file_ref *ref);
afc_error_t AFCFileRefOpen(void *conn, const char *path,unsigned long long mode, afc_file_ref *ref);
afc_error_t AFCFileRefSeek(/*afc_connection*/void *conn, afc_file_ref ref,unsigned long long offset1, unsigned long long offset2);
afc_error_t AFCFileRefRead(/*afc_connection*/void *conn, afc_file_ref ref,void *buf, unsigned int *len);
afc_error_t AFCFileRefSetFileSize(/*afc_connection*/void *conn, afc_file_ref ref,unsigned long long offset);
afc_error_t AFCFileRefWrite(/*afc_connection*/void *conn, afc_file_ref ref,const void *buf, unsigned int len);
afc_error_t AFCFileRefClose(/*afc_connection*/void *conn, afc_file_ref ref);

//afc_error_t AFCFileInfoOpen(afc_connection *conn, const char *path, struct afc_dictionary **info);
afc_error_t AFCFileInfoOpen(/*afc_connection*/void *conn, const char *path, struct afc_dictionary **info);

afc_error_t AFCKeyValueRead(struct afc_dictionary *dict, char **key, char **val);

afc_error_t AFCKeyValueClose(struct afc_dictionary *dict);

unsigned int AMRestorePerformRecoveryModeRestore(struct am_recovery_device *
    rdev, CFDictionaryRef opts, void *callback, void *user_info);
unsigned int AMRestorePerformRestoreModeRestore(struct am_restore_device *
    rdev, CFDictionaryRef opts, void *callback, void *user_info);

unsigned int AMRestoreCreatePathsForBundle(CFStringRef restore_bundle_path,
    CFStringRef kernel_cache_type, CFStringRef boot_image_type, unsigned int
    unknown0, CFStringRef *firmware_dir_path, CFStringRef *
    kernelcache_restore_path, unsigned int unknown1, CFStringRef *
    ramdisk_path);

unsigned int AMDeviceGetConnectionID(/*struct am_device*/void *device);
mach_error_t AMDeviceEnterRecovery(/*struct am_device*/void *device);
mach_error_t AMDeviceDisconnect(/*struct am_device*/void *device);
mach_error_t AMDeviceRetain(/*struct am_device*/void *device);
mach_error_t AMDeviceRelease(/*struct am_device*/void *device);
//CFStringRef AMDeviceCopyValue(/*struct am_device*/void *device, unsigned int, CFStringRef cfstring);
void* AMDeviceCopyValue(/*struct am_device*/void *device, void* unknow, void* cfstring);
CFStringRef AMDeviceCopyDeviceIdentifier(/*struct am_device*/void *device);

typedef void (*notify_callback)(CFStringRef notification, void *data);

mach_error_t AMDPostNotification(service_conn_t socket, CFStringRef  notification, CFStringRef userinfo);
mach_error_t AMDObserveNotification(void *socket, CFStringRef notification);
mach_error_t AMDListenForNotifications(void *socket, notify_callback cb, void *data);
mach_error_t AMDShutdownNotificationProxy(void *socket);
                    
/*edits by geohot*/
mach_error_t AMDeviceDeactivate(/*struct am_device*/void *device);
mach_error_t AMDeviceActivate(/*struct am_device*/void*, CFMutableDictionaryRef);
/*end*/

void *AMDeviceSerialize(/*struct am_device*/void *device);
void AMDAddLogFileDescriptor(int fd);
//kern_return_t AMDeviceSendMessage(service_conn_t socket, void *unused, CFPropertyListRef plist);
//kern_return_t AMDeviceReceiveMessage(service_conn_t socket, CFDictionaryRef options, CFPropertyListRef * result);

/* ----------------------------------------------------------------------------
 *   Semi-private routines
 * ------------------------------------------------------------------------- */

/*  Pass in a usbmux_listener_1 structure and a usbmux_listener_2 structure
 *  pointer, which will be filled with the resulting usbmux_listener_2.
 *
 *  Returns:
 *      MDERR_OK                if completed successfully
 *      MDERR_USBMUX_ARG_NULL   if one of the arguments was NULL
 *      MDERR_USBMUX_FAILED     if the listener was not created successfully
 */

usbmux_error_t USBMuxListenerCreate(struct usbmux_listener_1 *esi_fp8, struct
    usbmux_listener_2 **eax_fp12);

/* ----------------------------------------------------------------------------
 *   Less-documented semi-private routines
 * ------------------------------------------------------------------------- */

usbmux_error_t USBMuxListenerHandleData(void *);

/* ----------------------------------------------------------------------------
 *   Private routines - here be dragons
 * ------------------------------------------------------------------------- */

/* AMRestorePerformRestoreModeRestore() calls this function with a dictionary
 * in order to perform certain special restore operations
 * (RESTORED_OPERATION_*). It is thought that this function might enable
 * significant access to the phone. */

typedef unsigned int (*t_performOperation)(struct am_restore_device *rdev,
    CFDictionaryRef op); // __attribute__ ((regparm(2)));

#include <stdint.h>
typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef int64_t             INT64, *PINT64;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef uint64_t            UINT64, *PUINT64;

//iphoneEvents
typedef void (*PackDataTransferEventHandler)(void* ref);
typedef void (*PackInstallApplicationEventHandler)(void* sender, void* e);

mach_error_t AMDevicePair(/*struct am_device*/void *device);
mach_error_t AMDeviceUnpair(/*struct am_device*/void *device);
mach_error_t AMDeviceSetValue(/*struct am_device*/void *device, unsigned int unknown, void* cfkey, void* cfValue);
mach_error_t AMDeviceTransferApplication(void* sockAFC, void* cfStrPath, void* cfDictUnknow, PackDataTransferEventHandler callback, void* unknow);
mach_error_t AMDeviceInstallApplication(void* sockInstallProxy, void* cfStrPath, void* opts, PackInstallApplicationEventHandler callback, void* unknow);
mach_error_t AMDeviceUninstallApplication(void* sock, void* cfAppName, void* opts, void* callback, void* ukn);
mach_error_t AMDeviceLookupApplications(/*struct am_device*/void* device, void* optsDict, void** outApps);
mach_error_t AMDeviceArchiveApplication(void* sockInstallProxy, void* cfIdentifier, void* optsDict, PackInstallApplicationEventHandler callback, void* unknow);
mach_error_t AMDeviceRemoveApplicationArchive(void *sockInstallProxy, void* cfIdentifier, void* optsDict, PackInstallApplicationEventHandler callback, void* unknow);
mach_error_t AMDeviceRestoreApplication(void *sockInstallProxy, void* cfIdentifier, void* opts, PackInstallApplicationEventHandler callback, void* unknow);
mach_error_t AMDeviceUpgradeApplication(void *sockInstallProxy, void* cfPackagePath, void* opts, PackInstallApplicationEventHandler callback, void* unknow);
mach_error_t AMDeviceTransferPath(void* device, void* cfPackagePath, void* opts, PackInstallApplicationEventHandler callback, void* unknow);

afc_error_t AFCFileRefLock( /*afc_connection*/void *conn, afc_file_ref ref );
afc_error_t AFCFileRefUnlock( /*afc_connection*/void *conn, afc_file_ref ref );
//afc_error_t AFCGetFileInfo(/*afc_connection*/void *conn, const char* path, void **info, UINT32 *length);
//afc_error_t AFCFlushData(/*afc_connection*/void *conn, afc_file_ref handle);
afc_error_t AFCFileRefTell(/*afc_connection*/void *conn, afc_file_ref handle, UINT32 *position);
void* AMRecoveryModeDeviceCopyIMEI(/*struct am_device*/void* device);
void* AMRecoveryModeDeviceCopySerialNumber(/*struct am_device*/void *device);
void* AMRecoveryModeDeviceGetLocationID(/*struct am_device*/void *device);
void* AMRecoveryModeDeviceGetProductID(/*struct am_device*/void *device);
void* AMRecoveryModeGetSoftwareBuildVersion(/*struct am_device*/void *device);
INT32 AMRecoveryModeDeviceGetProductType(/*struct am_device*/void *device);
INT32 AMRecoveryModeDeviceGetTypeID(/*struct am_device*/void* device);
INT32 AMRecoveryModeDeviceSetAutoBoot(/*struct am_device*/void *device, char flag);
INT32 AMRecoveryModeDeviceReboot(/*struct am_device*/void *device);
//struct am_restore_device *AMRestoreModeDeviceCreate(unsigned int unknown0,unsigned int connection_id, unsigned int unknown1);
INT32 AMRestoreModeDeviceCreate(UINT32 unknown0, UINT32 connection_id, UINT32 unknown1);

class MobileDevice
{

public:
    MobileDevice();
    virtual ~MobileDevice();

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
		static void*	 iTunes_AMDeviceCopyValue(void* device, void* unknown, void* cfstring);
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

		static afc_error_t iTunes_AFCDirectoryOpen(void *conn, const wchar_t* path, void **hAFDirHandle);
    static afc_error_t iTunes_AFCDirectoryRead(void *conn, void *dir, char **dirent);
    static afc_error_t iTunes_AFCDirectoryClose(void *conn, void *dir);
    static afc_error_t iTunes_AFCConnectionOpen(void *handle, UINT32 io_timeout, void **conn);
    static afc_error_t iTunes_AFCConnectionClose(void *conn);// πÿ±’¡¨Ω” - 20081231 added -
		static afc_error_t iTunes_AFCFileInfoOpen(void *conn, const wchar_t* path, void **info);
		static afc_error_t iTunes_AFCKeyValueRead(void *dict, char **key, char **value);
    static afc_error_t iTunes_AFCKeyValueClose(void *dict);
		static afc_error_t iTunes_AFCRemovePath(void *conn, const wchar_t* path);
		static afc_error_t iTunes_AFCRenamePath(void *conn, const wchar_t* oldPath, const wchar_t* newPath);

    // Summary  : Opens file for reading or writing without locking it in any way. afc_file_ref should not be shared between threads - * opening file in one thread and closing it in another will lead to possible crash.
    // Returns  : 
    // Parameter: void * conn 
    // Parameter: path : absolute path to file (convert to UTF-8 encoded)
    // Parameter: mode : mode  2 = read, mode 3 = write; unknown = 0 
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
};

#ifdef __cplusplus
}
#endif

#endif
