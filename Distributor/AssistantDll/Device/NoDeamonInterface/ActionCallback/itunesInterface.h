/*
 *  itunesInterface.h
 *
 *  Created by lindaojiang on 2012-6-8
 *  Copyright 2012 NetDragon Websoft Inc. All rights reserved.
 *
 */

#ifndef _ITUNES_DLL_INTERFACE_H_
#define _ITUNES_DLL_INTERFACE_H_

#include <windows.h>
#include <tchar.h>

#ifdef __cplusplus
extern "C" {
#endif

// 参考 MobileDevice.h  http://theiphonewiki.com/wiki/index.php?title=MobileDevice_Library
//	https://bitbucket.org/tristero/mobiledeviceaccess/src
//	http://code.google.com/p/iphonetunnel-usbmuxconnectbyport/source/browse/

#pragma pack(push, 1)

//========================CoreFoundation===========================================

#if !defined(__MACTYPES__)
#if !defined(_OS_OSTYPES_H)
typedef unsigned char           Boolean;
typedef unsigned char           UInt8;
typedef signed char             SInt8;
typedef unsigned short          UInt16;
typedef signed short            SInt16;
typedef unsigned int            UInt32;
typedef signed int              SInt32;
typedef UINT64		    UInt64;
typedef INT64		    SInt64;
typedef SInt32                  OSStatus;
#endif
typedef float                   Float32;
typedef double                  Float64;
typedef unsigned short          UniChar;
typedef unsigned long           UniCharCount;
typedef unsigned char *         StringPtr;
typedef const unsigned char *   ConstStringPtr;
typedef unsigned char           Str255[256];
typedef const unsigned char *   ConstStr255Param;
typedef SInt16                  OSErr;
typedef SInt16                  RegionCode;
typedef SInt16                  LangCode;
typedef SInt16                  ScriptCode;
typedef UInt32                  FourCharCode;
typedef FourCharCode            OSType;
typedef UInt8                   Byte;
typedef SInt8                   SignedByte;
#endif
#if !defined(__MACTYPES__) || (defined(UNIVERSAL_INTERFACES_VERSION) && UNIVERSAL_INTERFACES_VERSION < 0x0340)
typedef UInt32                  UTF32Char;
typedef UInt16                  UTF16Char;
typedef UInt8                   UTF8Char;
#endif

typedef unsigned long CFTypeID;
typedef unsigned long CFOptionFlags;
typedef unsigned long CFHashCode;
typedef signed long CFIndex;
typedef void * CFTypeRef;
typedef CFTypeRef CFPropertyListRef;
typedef CFTypeRef CFDictionaryRef;
typedef CFTypeRef CFMutableDictionaryRef;
typedef CFTypeRef CFDataRef;
typedef CFTypeRef CFStringRef;
typedef CFTypeRef CFMutableStringRef;
typedef CFTypeRef CFArrayRef;
typedef CFTypeRef CFMutableArrayRef;
typedef CFTypeRef CFAllocatorRef;
typedef CFTypeRef CFReadStreamRef;
typedef CFTypeRef CFWriteStreamRef;
typedef CFTypeRef CFNumberRef;
typedef CFTypeRef CFBooleanRef;

typedef struct CFRange {
	CFIndex location;
	CFIndex length;
}CFRange;

/* Values returned from comparison functions */
enum {
	kCFCompareLessThan = -1,
	kCFCompareEqualTo = 0,
	kCFCompareGreaterThan = 1
};
typedef CFIndex CFComparisonResult;

/* Platform-independent built-in encodings; always available on all platforms.
   Call CFStringGetSystemEncoding() to get the default system encoding.
*/
typedef UINT32 CFStringEncoding;
#define kCFStringEncodingInvalidId (0xffffffffU)
enum {
    kCFStringEncodingMacRoman = 0,
    kCFStringEncodingWindowsLatin1 = 0x0500, /* ANSI codepage 1252 */
    kCFStringEncodingISOLatin1 = 0x0201, /* ISO 8859-1 */
    kCFStringEncodingNextStepLatin = 0x0B01, /* NextStep encoding*/
    kCFStringEncodingASCII = 0x0600, /* 0..127 (in creating CFString, values greater than 0x7F are treated as corresponding Unicode value) */
    kCFStringEncodingUnicode = 0x0100, /* kTextEncodingUnicodeDefault  + kTextEncodingDefaultFormat (aka kUnicode16BitFormat) */
    kCFStringEncodingUTF8 = 0x08000100, /* kTextEncodingUnicodeDefault + kUnicodeUTF8Format */
    kCFStringEncodingNonLossyASCII = 0x0BFF, /* 7bit Unicode variants used by Cocoa & Java */

    kCFStringEncodingUTF16 = 0x0100, /* kTextEncodingUnicodeDefault + kUnicodeUTF16Format (alias of kCFStringEncodingUnicode) */
    kCFStringEncodingUTF16BE = 0x10000100, /* kTextEncodingUnicodeDefault + kUnicodeUTF16BEFormat */
    kCFStringEncodingUTF16LE = 0x14000100, /* kTextEncodingUnicodeDefault + kUnicodeUTF16LEFormat */

    kCFStringEncodingUTF32 = 0x0c000100, /* kTextEncodingUnicodeDefault + kUnicodeUTF32Format */
    kCFStringEncodingUTF32BE = 0x18000100, /* kTextEncodingUnicodeDefault + kUnicodeUTF32BEFormat */
    kCFStringEncodingUTF32LE = 0x1c000100 /* kTextEncodingUnicodeDefault + kUnicodeUTF32LEFormat */
};
typedef CFStringEncoding CFStringBuiltInEncodings;

enum {	
	//default 0, 区分大小写
	kCFCompareCaseInsensitive = 1,	//不区分大小写
	kCFCompareBackwards = 4,		/* Starting from the end of the string */
	kCFCompareAnchored = 8,		/* Only at the specified starting point */
	kCFCompareNonliteral = 16,		/* If specified, loose equivalence is performed (o-umlaut == o, umlaut) */
	kCFCompareLocalized = 32,		/* User's default locale is used for the comparisons */
	kCFCompareNumerically = 64		/* Numeric comparison is used; that is, Foo2.txt < Foo7.txt < Foo25.txt */
};
typedef CFOptionFlags CFStringCompareFlags;


typedef const void *(*CFDictionaryRetainCallBack)(CFAllocatorRef allocator, const void *value);
typedef void		(*CFDictionaryReleaseCallBack)(CFAllocatorRef allocator, const void *value);
typedef CFStringRef	(*CFDictionaryCopyDescriptionCallBack)(const void *value);
typedef bool		(*CFDictionaryEqualCallBack)(const void *value1, const void *value2);
typedef CFHashCode	(*CFDictionaryHashCallBack)(const void *value);
typedef struct {
	CFIndex				version;
	CFDictionaryRetainCallBack		retain;
	CFDictionaryReleaseCallBack		release;
	CFDictionaryCopyDescriptionCallBack	copyDescription;
	CFDictionaryEqualCallBack		equal;
	CFDictionaryHashCallBack		hash;
} CFDictionaryKeyCallBacks;

typedef struct {
	CFIndex				version;
	CFDictionaryRetainCallBack		retain;
	CFDictionaryReleaseCallBack		release;
	CFDictionaryCopyDescriptionCallBack	copyDescription;
	CFDictionaryEqualCallBack		equal;
} CFDictionaryValueCallBacks;

enum {
	kCFPropertyListImmutable = 0,
	kCFPropertyListMutableContainers,
	kCFPropertyListMutableContainersAndLeaves
};
typedef CFOptionFlags CFPropertyListMutabilityOptions;

enum {
	kCFPropertyListOpenStepFormat = 1,
	kCFPropertyListXMLFormat_v1_0 = 100,
	kCFPropertyListBinaryFormat_v1_0 = 200
};
typedef CFIndex CFPropertyListFormat;

enum {
	/* Fixed-width types */
	kCFNumberSInt8Type = 1,
	kCFNumberSInt16Type = 2,
	kCFNumberSInt32Type = 3,
	kCFNumberSInt64Type = 4,
	kCFNumberFloat32Type = 5,
	kCFNumberFloat64Type = 6,	/* 64-bit IEEE 754 */
	/* Basic C types */
	kCFNumberCharType = 7,
	kCFNumberShortType = 8,
	kCFNumberIntType = 9,
	kCFNumberLongType = 10,
	kCFNumberLongLongType = 11,
	kCFNumberFloatType = 12,
	kCFNumberDoubleType = 13,
	/* Other */
	kCFNumberCFIndexType = 14,
	kCFNumberNSIntegerType = 15,
	kCFNumberCGFloatType = 16,
	kCFNumberMaxType = 16
};
typedef CFIndex CFNumberType;

//========================CoreFoundation===========================================

typedef void * _UNKNOWN;

typedef unsigned int afc_error_t;
typedef unsigned int usbmux_error_t;

/* mode 2 = read, mode 3 = write; unknown = 0 */ /* 1=read, 2=write, 3=read/write */
#define AFC_FILEMODE_READ			2
#define AFC_FILEMODE_WRITE			3

struct am_recovery_device;

struct am_device_notification_callback_info {
    struct am_device *dev;  /* 0    device */ 
    unsigned int msg;       /* 4    one of ADNCI_MSG_* */
};

/* The type of the device restore notification callback functions.
 * TODO: change to correct type. */
typedef void (*am_restore_device_notification_callback)(struct am_recovery_device *);

/* This is a CoreFoundation object of class AMRecoveryModeDevice. */
struct am_recovery_device {
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
};

/* A CoreFoundation object of class AMRestoreModeDevice. */
struct am_restore_device {
    unsigned char unknown[32];
    int port;
};

/* The type of the device notification callback function. */
typedef void(*am_device_notification_callback)(struct am_device_notification_callback_info *);

/* The type of the _AMDDeviceAttached function.
 * TODO: change to correct type. */
typedef void *amd_device_attached_callback;

/* The type of the device restore notification callback functions.
 * TODO: change to correct type. */
typedef void (*am_restore_device_notification_callback)(struct am_recovery_device *);
 
struct am_device {			
    unsigned char unknown0[16]; /* 0 - zero */
    unsigned int device_id;     /* 16 */
    unsigned int product_id;    /* 20 - set to AMD_IPHONE_PRODUCT_ID */
    char *serial;               /* 24 - set to AMD_IPHONE_SERIAL */
    unsigned int unknown1;      /* 28 */
    unsigned char unknown2[4];  /* 32 */
    unsigned int lockdown_conn; /* 36 */
    unsigned char unknown3[8];  /* 40 */
	unsigned int unknown4;      /* 48 - used to store CriticalSection Handle*/
	unsigned char unknown5[24];  /* 52 */
};


struct am_device_notification {
    unsigned int unknown0;                      /* 0 */
    unsigned int unknown1;                      /* 4 */
    unsigned int unknown2;                      /* 8 */
    am_device_notification_callback callback;   /* 12 */ 
    unsigned int unknown3;                      /* 16 */
};

struct afc_connection {
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
};

struct afc_device_info {
    unsigned char unknown[12];  /* 0 */
};

struct afc_directory {
    unsigned char unknown[1];   /* size unknown */
};

struct afc_dictionary {
    unsigned char unknown[1];   /* size unknown */
};

typedef unsigned long long afc_file_ref;

struct usbmux_listener_1 {                  /* offset   value in iTunes */
    unsigned int unknown0;                  /* 0        1 */
    unsigned char *unknown1;                /* 4        ptr, maybe device? */
    amd_device_attached_callback callback;  /* 8        _AMDDeviceAttached */
    unsigned int unknown3;                  /* 12 */
    unsigned int unknown4;                  /* 16 */
    unsigned int unknown5;                  /* 20 */
};

struct usbmux_listener_2 {
    unsigned char unknown0[4144];
};

struct am_bootloader_control_packet {
    unsigned char opcode;       /* 0 */
    unsigned char length;       /* 1 */
    unsigned char magic[2];     /* 2: 0x34, 0x12 */
    //unsigned char payload[0];   /* 4 */
};




//------------------------------------------------------------

typedef int (*PF_AMDeviceNotificationSubscribe)(
	am_device_notification_callback callback, 
	unsigned int unused0, 
	unsigned int unused1, 
	unsigned int dn_unknown3, 
	struct am_device_notification **notification
);

typedef int (*PF_AMDeviceGetConnectionID)(struct am_device *device);
typedef int (*PF_AMDeviceCopyDeviceIdentifier)(struct am_device *device);
typedef CFTypeRef (*PF_AMDeviceCopyValue)(struct am_device *device, CFStringRef domain, CFStringRef key);

typedef int (*PF_AMDeviceConnect)(struct am_device *device);

typedef int (*PF_AMDeviceIsPaired)(struct am_device *device);
typedef int (*PF_AMDeviceValidatePairing)(struct am_device *device);

typedef int (*PF_AMDeviceStartSession)(struct am_device *device);

typedef int (*PF_AMDeviceStartService)(struct am_device *device, CFStringRef service_name, int *socket_fd, unsigned int *unknown);

typedef int (*PF_AFCConnectionOpen)(int socket_fd, unsigned int io_timeout, struct afc_connection **conn);

typedef int (*PF_AFCFileRefOpen)(struct afc_connection *conn, char *path, unsigned int mode, int zero, afc_file_ref *ref);  
typedef int (*PF_AFCFileRefClose)(struct afc_connection *conn, afc_file_ref ref);
typedef int (*PF_AFCFileRefRead)(struct afc_connection *conn, afc_file_ref ref, void *buf, unsigned int *len);
typedef int (*PF_AFCFileRefWrite)(struct afc_connection *conn, afc_file_ref ref, void *buf, unsigned int len);
typedef int (*PF_AFCFileRefSeek)(struct afc_connection *conn, afc_file_ref ref, unsigned long long offset1, unsigned long long offset2);
//AFCFileRefLock(struct afc_connection *conn, afc_file_ref ref);
//AFCFileRefUnlock(struct afc_connection *conn, afc_file_ref ref);

typedef int (*PF_AFCFileInfoOpen)(struct afc_connection *conn, char *path, struct afc_dictionary **info);
typedef int (*PF_AFCGetFileInfo)(struct afc_connection *conn, char *path, struct afc_dictionary **info, unsigned int *size);
typedef int (*PF_AFCKeyValueRead)(struct afc_dictionary *dict, char **key, char **val);
typedef int (*PF_AFCKeyValueClose)(struct afc_dictionary *dict);

typedef int (*PF_AMDeviceGetInterfaceType)(struct am_device *device);
typedef int (*PF_AMDevicePair)(struct am_device *device);
typedef int (*PF_AMDPostNotification)(int socket, CFStringRef notification, CFStringRef userinfo); 
typedef int (*PF_AMDShutdownNotificationProxy)(int socket);
typedef int (*PF_AMDeviceSetWirelessBuddyFlags)(struct am_device *device, int achr);
typedef int (*PF_AMDeviceSecureStartService)(struct am_device *pDev, char * serviceName, void *option, void *retSock);
typedef int (*PF_AMDServiceConnectionGetSocket)(void *retSock);
typedef int (*PF_AMDServiceConnectionGetSecureIOContext)(void *retSock);
typedef int (*PF_AFCConnectionSetSecureContext)(void *newAFCCon, int ioContext);
typedef int (*PF_AMDServiceConnectionReceive)(int sock, char *buf, int len);
typedef int (*PF_AMDServiceConnectionSend)(int sock, char *buf, int len);
typedef int (*PF_AMDeviceCreateHouseArrestService)(struct am_device *pDev, char *CFBundleIdentifier, int zero, void *newAfc);
typedef int (*PF_AMDeviceLookupApplications)(struct am_device *pDev, int zero, void *app);
typedef int (*PF_AMDeviceSecureInstallApplication)(int service, struct am_device *pDev, char *ipa, int zero, void (*callbak)(int, int), int opt);

typedef int (*PF_AMDeviceRelease)(struct am_device *device);
typedef int (*PF_AMDeviceRetain)(struct am_device *device);
typedef int (*PF_AMDeviceDisconnect)(struct am_device *device);
typedef int (*PF_AMDeviceStopSession)(struct am_device *device);
typedef int (*PF_AFCDirectoryOpen)(struct afc_connection *conn, char *path, struct afc_directory **dir);
typedef int (*PF_AFCDirectoryRead)(struct afc_connection *conn, struct afc_directory *dir, char **dirent);
typedef int (*PF_AFCDirectoryClose)(struct afc_connection *conn, struct afc_directory *dir);
typedef int (*PF_AFCDirectoryCreate)(struct afc_connection *conn, char *dirname);
typedef int (*PF_AFCRemovePath)(struct afc_connection *conn, char *dirname);
typedef int (*PF_AFCRenamePath)(struct afc_connection *conn, char *oldpath, char *newpath);
typedef int (*PF_AFCConnectionClose)(struct afc_connection *conn);

typedef int (*PF_USBMuxConnectByPort)(int deviceConnectionID, unsigned short iphonePort, int *newSocket);			

enum
{
	ENUM_AMDeviceNotificationSubscribe,
	ENUM_AMDeviceGetConnectionID,
	ENUM_AMDeviceCopyDeviceIdentifier,
	ENUM_AMDeviceCopyValue,
	ENUM_AMDeviceConnect,
	ENUM_AMDeviceDisconnect,
	ENUM_AMDeviceIsPaired,
	ENUM_AMDeviceValidatePairing,
	ENUM_AMDeviceStartSession,
	ENUM_AMDeviceStopSession,
	ENUM_AMDeviceRelease,
	ENUM_AMDeviceRetain,
	ENUM_AMDeviceStartService,
	ENUM_AFCConnectionOpen,
	ENUM_AFCConnectionClose,
	ENUM_AFCFileRefOpen,
	ENUM_AFCFileRefClose,
	ENUM_AFCFileRefRead,
	ENUM_AFCFileRefWrite,
	ENUM_AFCFileRefSeek,
	ENUM_AFCFileInfoOpen,
	ENUM_AFCDirectoryOpen,
	ENUM_AFCDirectoryRead,
	ENUM_AFCDirectoryClose,
	ENUM_AFCDirectoryCreate,
	ENUM_AFCRemovePath,
	ENUM_AFCRenamePath,
	ENUM_AFCKeyValueRead,		
	ENUM_AFCKeyValueClose,		
	ENUM_AMDeviceGetInterfaceType,
	ENUM_AMDevicePair,
	ENUM_AMDPostNotification,
	ENUM_AMDShutdownNotificationProxy,
	ENUM_AMDeviceSetWirelessBuddyFlags,
	ENUM_AMDeviceSecureStartService,
	ENUM_AMDServiceConnectionGetSocket,
	ENUM_AMDServiceConnectionGetSecureIOContext,
	ENUM_AFCConnectionSetSecureContext,
	ENUM_AMDServiceConnectionReceive,
	ENUM_AMDServiceConnectionSend,
	ENUM_AMDeviceCreateHouseArrestService,
	ENUM_AMDeviceLookupApplications,
	ENUM_AMDeviceSecureInstallApplication,
	ENUM_USBMuxConnectByPort,

	ENUM_ITUNESMOBILEDEVICE_DLL_NUM
};


extern FARPROC __gpf_iTunesMobileDevice_dll_ri[ENUM_ITUNESMOBILEDEVICE_DLL_NUM];


#define AMDeviceNotificationSubscribe  		((PF_AMDeviceNotificationSubscribe)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceNotificationSubscribe]))
#define AMDeviceGetConnectionID  			((PF_AMDeviceGetConnectionID)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceGetConnectionID]))
#define AMDeviceCopyDeviceIdentifier  		((PF_AMDeviceCopyDeviceIdentifier)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceCopyDeviceIdentifier]))
#define AMDeviceCopyValue			  		((PF_AMDeviceCopyValue)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceCopyValue]))
#define AMDeviceConnect  					((PF_AMDeviceConnect)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceConnect]))
#define AMDeviceIsPaired  					((PF_AMDeviceIsPaired)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceIsPaired]))
#define AMDeviceValidatePairing  			((PF_AMDeviceValidatePairing)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceValidatePairing]))
#define AMDeviceStartSession  				((PF_AMDeviceStartSession)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceStartSession]))
#define AMDeviceStartService  				((PF_AMDeviceStartService)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceStartService]))
#define AFCConnectionOpen 					((PF_AFCConnectionOpen)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCConnectionOpen]))
#define AFCFileRefOpen 						((PF_AFCFileRefOpen)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCFileRefOpen]))
#define AFCFileRefClose 					((PF_AFCFileRefClose)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCFileRefClose]))
#define AFCFileRefRead 						((PF_AFCFileRefRead)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCFileRefRead]))
#define AFCFileRefWrite 					((PF_AFCFileRefWrite)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCFileRefWrite]))
#define AFCFileRefSeek						((PF_AFCFileRefSeek)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCFileRefSeek]))
#define AFCFileInfoOpen 					((PF_AFCFileInfoOpen)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCFileInfoOpen]))
#define AFCKeyValueRead 					((PF_AFCKeyValueRead)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCKeyValueRead]))
#define AFCKeyValueClose 					((PF_AFCKeyValueClose)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCKeyValueClose]))
#define AMDeviceGetInterfaceType 			((PF_AMDeviceGetInterfaceType)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceGetInterfaceType]))
#define AMDevicePair						((PF_AMDevicePair)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDevicePair]))
#define AMDPostNotification					((PF_AMDPostNotification)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDPostNotification]))
#define AMDShutdownNotificationProxy		((PF_AMDShutdownNotificationProxy)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDShutdownNotificationProxy]))
#define AMDeviceSetWirelessBuddyFlags		((PF_AMDeviceSetWirelessBuddyFlags)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceSetWirelessBuddyFlags]))
#define AMDeviceSecureStartService			((PF_AMDeviceSecureStartService)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceSecureStartService]))
#define AMDServiceConnectionGetSocket		((PF_AMDServiceConnectionGetSocket)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDServiceConnectionGetSocket]))
#define AMDServiceConnectionGetSecureIOContext	((PF_AMDServiceConnectionGetSecureIOContext)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDServiceConnectionGetSecureIOContext]))
#define AFCConnectionSetSecureContext		((PF_AFCConnectionSetSecureContext)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCConnectionSetSecureContext]))
#define AMDServiceConnectionReceive			((PF_AMDServiceConnectionReceive)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDServiceConnectionReceive]))
#define AMDServiceConnectionSend			((PF_AMDServiceConnectionSend)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDServiceConnectionSend]))
#define AMDeviceCreateHouseArrestService	((PF_AMDeviceCreateHouseArrestService)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceCreateHouseArrestService]))
#define AMDeviceLookupApplications			((PF_AMDeviceLookupApplications)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceLookupApplications]))
#define AMDeviceSecureInstallApplication	((PF_AMDeviceSecureInstallApplication)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceSecureInstallApplication]))

#define AMDeviceRelease						((PF_AMDeviceRelease)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceRelease]))
#define AMDeviceRetain						((PF_AMDeviceRetain)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceRetain]))
#define AMDeviceDisconnect					((PF_AMDeviceDisconnect)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceDisconnect]))
#define AMDeviceStopSession					((PF_AMDeviceStopSession)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AMDeviceStopSession]))
#define AFCDirectoryOpen					((PF_AFCDirectoryOpen)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCDirectoryOpen]))
#define AFCDirectoryRead					((PF_AFCDirectoryRead)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCDirectoryRead]))
#define AFCDirectoryClose					((PF_AFCDirectoryClose)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCDirectoryClose]))
#define AFCDirectoryCreate					((PF_AFCDirectoryCreate)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCDirectoryCreate]))
#define AFCRemovePath						((PF_AFCRemovePath)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCRemovePath]))
#define AFCRenamePath						((PF_AFCRenamePath)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCRenamePath]))
#define AFCConnectionClose					((PF_AFCConnectionClose)(__gpf_iTunesMobileDevice_dll_ri[ENUM_AFCConnectionClose]))

#define USBMuxConnectByPort					((PF_USBMuxConnectByPort)(__gpf_iTunesMobileDevice_dll_ri[ENUM_USBMuxConnectByPort]))

//####################################################################################################################################
//typedef signed int (__cdecl *PF_ATAddAppleSearchPathsToEnvironmentFromReg)();
//typedef int (__cdecl *PF_ATCFMessageCreate)(char a1, int a2, int a3);
//typedef int (__cdecl *PF_ATCFMessageGetName)(int a1);
//typedef int (__cdecl *PF_ATCFMessageGetParam)(int a1, int a2);
//typedef int (__cdecl *PF_ATCFMessageGetSessionNumber)(void *a1, int a2);
//typedef char (__cdecl *PF_ATCFMessageVerify)(int a1, int a2);
typedef signed int (__cdecl *PF_ATHostAdvertiserStartWakeupService)(int a1);
typedef void (__cdecl *PF_ATHostAdvertiserStopWakeupService)();
typedef int (__cdecl *PF_ATHostConnectionCreate)(int a1, int a2);
typedef int (__cdecl *PF_ATHostConnectionCreateWithLibrary)(int a1, int a2, int a3);
typedef int (__cdecl *PF_ATHostConnectionDestroy)( int a1, int a2 );
typedef int (__cdecl *PF_ATHostConnectionGetCurrentSessionNumber)(int a1);
typedef int (__cdecl *PF_ATHostConnectionGetGrappaSessionId)(int a1);
typedef int (__cdecl *PF_ATHostConnectionInvalidate)(int *a1);
typedef int (__cdecl *PF_ATHostConnectionReadMessage)(int a1);
typedef int (__cdecl *PF_ATHostConnectionRelease)(int a1);
typedef int (__cdecl *PF_ATHostConnectionRetain)(int a1);
typedef int (__cdecl *PF_ATHostConnectionSendAssetCompleted)(int a1, int a2, int a3, int a4);
typedef int (__cdecl *PF_ATHostConnectionSendAssetMetricsRequest)(int a1, int a2);
typedef int (__cdecl *PF_ATHostConnectionSendFileBegin)(int a1, int a2, int a3, char a4, int a5, char a6);
typedef int (__cdecl *PF_ATHostConnectionSendFileError)(int a1, int a2, int a3, char a4);
typedef int (__cdecl *PF_ATHostConnectionSendFileProgress)(int a1, int a2, int a3, char a4, int a5, char a6);
typedef int (__cdecl *PF_ATHostConnectionSendHostInfo)(int a1, int a2);
typedef int (__cdecl *PF_ATHostConnectionSendMessage)(int *a1, int a2);
typedef int (__cdecl *PF_ATHostConnectionSendMetadataSyncFinished)(int a1, int a2, int a3);
typedef int (__cdecl *PF_ATHostConnectionSendPing)(int a1);
typedef int (__cdecl *PF_ATHostConnectionSendPowerAssertion)(int a1, int a2);
typedef int (__cdecl *PF_ATHostConnectionSendSyncFailed)(int a1, char a2);
typedef int (__cdecl *PF_ATHostConnectionSendSyncRequest)(int a1, int a2, int a3, int a4);
//typedef int (__cdecl *PF_ATProcessLinkCopyMessageFromChild)(int a1);
//typedef void (__cdecl *PF_ATProcessLinkExitChild)();
//typedef void (__cdecl *PF_ATProcessLinkInvalidate)(int a1);
//typedef void (__cdecl *PF_ATProcessLinkRelease)(int Memory);
//typedef int (__cdecl *PF_ATProcessLinkRetain)(int a1);
//typedef int (__cdecl *PF_ATProcessLinkRunChild)(const CHAR *a1, const CHAR *a2, void (__cdecl *a3)(DWORD, DWORD), int a4);
//typedef int (__cdecl *PF_ATProcessLinkSendMessage)(int a1, int a2);
//typedef signed int (__cdecl *PF_ATProcessLinkSendMessageToParent)(int a1);
//typedef int (__cdecl *PF_ATProcessLinkSetupParent)(int a1, _UNKNOWN a2);

enum
{
	//ENUM_ATAddAppleSearchPathsToEnvironmentFromReg,
	//ENUM_ATCFMessageCreate,
	//ENUM_ATCFMessageGetName,
	//ENUM_ATCFMessageGetParam,
	//ENUM_ATCFMessageGetSessionNumber,
	//ENUM_ATCFMessageVerify,
	ENUM_ATHostAdvertiserStartWakeupService,
	ENUM_ATHostAdvertiserStopWakeupService,
	ENUM_ATHostConnectionCreate,
	ENUM_ATHostConnectionCreateWithLibrary,
	ENUM_ATHostConnectionDestroy,
	ENUM_ATHostConnectionGetCurrentSessionNumber,
	ENUM_ATHostConnectionGetGrappaSessionId,
	ENUM_ATHostConnectionInvalidate,
	ENUM_ATHostConnectionReadMessage,
	ENUM_ATHostConnectionRelease,
	ENUM_ATHostConnectionRetain,
	ENUM_ATHostConnectionSendAssetCompleted,
	ENUM_ATHostConnectionSendAssetMetricsRequest,
	ENUM_ATHostConnectionSendFileBegin,
	ENUM_ATHostConnectionSendFileError,
	ENUM_ATHostConnectionSendFileProgress,
	ENUM_ATHostConnectionSendHostInfo,
	ENUM_ATHostConnectionSendMessage,
	ENUM_ATHostConnectionSendMetadataSyncFinished,
	ENUM_ATHostConnectionSendPing,
	ENUM_ATHostConnectionSendPowerAssertion,
	ENUM_ATHostConnectionSendSyncFailed,
	ENUM_ATHostConnectionSendSyncRequest,
	//ENUM_ATProcessLinkCopyMessageFromChild,
	//ENUM_ATProcessLinkExitChild,
	//ENUM_ATProcessLinkInvalidate,
	//ENUM_ATProcessLinkRelease,
	//ENUM_ATProcessLinkRetain,
	//ENUM_ATProcessLinkRunChild,
	//ENUM_ATProcessLinkSendMessage,
	//ENUM_ATProcessLinkSendMessageToParent,
	//ENUM_ATProcessLinkSetupParent,

	ENUM_AIRTRAFFICHOST_DLL_NUM
};

extern FARPROC __gpf_AirTrafficHost_dll_ri[ENUM_AIRTRAFFICHOST_DLL_NUM];

//#define ATH_ATAddAppleSearchPathsToEnvironmentFromReg				((PF_ATAddAppleSearchPathsToEnvironmentFromReg)(__gpf_AirTrafficHost_dll_ri[ENUM_ATAddAppleSearchPathsToEnvironmentFromReg]))
//#define ATH_ATCFMessageCreate										((PF_ATCFMessageCreate)(__gpf_AirTrafficHost_dll_ri[ENUM_ATCFMessageCreate]))
//#define ATH_ATCFMessageGetName										((PF_ATCFMessageGetName)(__gpf_AirTrafficHost_dll_ri[ENUM_ATCFMessageGetName]))
//#define ATH_ATCFMessageGetSessionNumber								((PF_ATCFMessageGetSessionNumber)(__gpf_AirTrafficHost_dll_ri[ENUM_ATCFMessageGetSessionNumber]))
//#define ATH_ATCFMessageVerify										((PF_ATCFMessageVerify)(__gpf_AirTrafficHost_dll_ri[ENUM_ATCFMessageVerify]))
#define ATH_ATHostAdvertiserStartWakeupService						((PF_ATHostAdvertiserStartWakeupService)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostAdvertiserStartWakeupService]))
#define ATH_ATHostAdvertiserStopWakeupService						((PF_ATHostAdvertiserStopWakeupService)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostAdvertiserStopWakeupService]))
#define ATH_ATHostConnectionCreate									((PF_ATHostConnectionCreate)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionCreate]))
#define ATH_ATHostConnectionCreateWithLibrary						((PF_ATHostConnectionCreateWithLibrary)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionCreateWithLibrary]))
#define ATH_ATHostConnectionDestroy									((PF_ATHostConnectionDestroy)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionDestroy]))
#define ATH_ATHostConnectionGetCurrentSessionNumber					((PF_ATHostConnectionGetCurrentSessionNumber)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionGetCurrentSessionNumber]))
#define ATH_ATHostConnectionGetGrappaSessionId						((PF_ATHostConnectionGetGrappaSessionId)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionGetGrappaSessionId]))
#define ATH_ATHostConnectionInvalidate								((PF_ATHostConnectionInvalidate)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionInvalidate]))
#define ATH_ATHostConnectionReadMessage								((PF_ATHostConnectionReadMessage)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionReadMessage]))
#define ATH_ATHostConnectionRelease									((PF_ATHostConnectionRelease)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionRelease]))
#define ATH_ATHostConnectionRetain									((PF_ATHostConnectionRetain)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionRetain]))
#define ATH_ATHostConnectionSendAssetCompleted						((PF_ATHostConnectionSendAssetCompleted)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionSendAssetCompleted]))
#define ATH_ATHostConnectionSendAssetMetricsRequest					((PF_ATHostConnectionSendAssetMetricsRequest)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionSendAssetMetricsRequest]))
#define ATH_ATHostConnectionSendFileBegin							((PF_ATHostConnectionSendFileBegin)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionSendFileBegin]))
#define ATH_ATHostConnectionSendFileError							((PF_ATHostConnectionSendFileError)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionSendFileError]))
#define ATH_ATHostConnectionSendFileProgress						((PF_ATHostConnectionSendFileProgress)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionSendFileProgress]))
#define ATH_ATHostConnectionSendHostInfo							((PF_ATHostConnectionSendHostInfo)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionSendHostInfo]))
#define ATH_ATHostConnectionSendMessage								((PF_ATHostConnectionSendMessage)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionSendMessage]))
#define ATH_ATHostConnectionSendMetadataSyncFinished				((PF_ATHostConnectionSendMetadataSyncFinished)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionSendMetadataSyncFinished]))
#define ATH_ATHostConnectionSendPing								((PF_ATHostConnectionSendPing)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionSendPing]))
#define ATH_ATHostConnectionSendPowerAssertion						((PF_ATHostConnectionSendPowerAssertion)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionSendPowerAssertion]))
#define ATH_ATHostConnectionSendSyncFailed							((PF_ATHostConnectionSendSyncFailed)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionSendSyncFailed]))
#define ATH_ATHostConnectionSendSyncRequest							((PF_ATHostConnectionSendSyncRequest)(__gpf_AirTrafficHost_dll_ri[ENUM_ATHostConnectionSendSyncRequest]))
//#define ATH_ATProcessLinkCopyMessageFromChild						((PF_ATProcessLinkCopyMessageFromChild)(__gpf_AirTrafficHost_dll_ri[ENUM_ATProcessLinkCopyMessageFromChild]))
//#define ATH_ATProcessLinkExitChild									((PF_ATProcessLinkExitChild)(__gpf_AirTrafficHost_dll_ri[ENUM_ATProcessLinkExitChild]))
//#define ATH_ATProcessLinkInvalidate									((PF_ATProcessLinkInvalidate)(__gpf_AirTrafficHost_dll_ri[ENUM_ATProcessLinkInvalidate]))
//#define ATH_ATProcessLinkRelease									((PF_ATProcessLinkRelease)(__gpf_AirTrafficHost_dll_ri[ENUM_ATProcessLinkRelease]))
//#define ATH_ATProcessLinkRetain										((PF_ATProcessLinkRetain)(__gpf_AirTrafficHost_dll_ri[ENUM_ATProcessLinkRetain]))
//#define ATH_ATProcessLinkRunChild									((PF_ATProcessLinkRunChild)(__gpf_AirTrafficHost_dll_ri[ENUM_ATProcessLinkRunChild]))
//#define ATH_ATProcessLinkSendMessage								((PF_ATProcessLinkSendMessage)(__gpf_AirTrafficHost_dll_ri[ENUM_ATProcessLinkSendMessage]))
//#define ATH_ATProcessLinkSendMessageToParent						((PF_ATProcessLinkSendMessageToParent)(__gpf_AirTrafficHost_dll_ri[ENUM_ATProcessLinkSendMessageToParent]))
//#define ATH_ATProcessLinkSetupParent								((PF_ATProcessLinkSetupParent)(__gpf_AirTrafficHost_dll_ri[ENUM_ATProcessLinkSetupParent]))

//####################################################################################################################################

//####################################################################################################################################


//CFTypeRef
typedef CFTypeID (__cdecl *PF_CFGetTypeID)(CFTypeRef cf); //
typedef void (__cdecl *PF_CFRelease)(CFTypeRef cf);

//CFNumberRef
typedef CFNumberRef (__cdecl *PF_CFNumberCreate)(CFAllocatorRef allocator, CFNumberType theType, const void *valuePtr);
typedef Boolean (__cdecl *PF_CFNumberGetValue)(CFNumberRef number, CFNumberType theType, void *valuePtr); 
//CFBooleanRef
typedef Boolean (__cdecl *PF_CFBooleanGetValue)(CFBooleanRef cfboolean);

//CFDataRef
typedef CFDataRef (__cdecl *PF_CFDataCreate)(CFAllocatorRef allocator, const UInt8 *utf8, CFIndex length);	
typedef CFIndex (__cdecl *PF_CFDataGetLength)(CFDataRef theData);
typedef const char * (__cdecl *PF_CFDataGetBytePtr)(CFDataRef theData);

//CFPropertyListRef
typedef CFPropertyListRef (__cdecl *PF_CFPropertyListCreateFromXMLData)(CFAllocatorRef allocator, CFDataRef xmlData, CFOptionFlags mutabilityOption, CFStringRef *errorString); 
typedef CFDataRef (__cdecl *PF_CFPropertyListCreateXMLData)(CFAllocatorRef allocator, CFPropertyListRef propertyList);	
typedef CFIndex (__cdecl *PF_CFPropertyListWriteToStream)(CFPropertyListRef propertyList, CFWriteStreamRef stream, CFPropertyListFormat format, CFStringRef *errorString); //

//CFWriteStreamRef
typedef int (__cdecl *PF_CFWriteStreamCreateWithAllocatedBuffers)( int *a1, int a2); //
typedef char (__cdecl *PF_CFWriteStreamOpen)( int a1); //
typedef int (__cdecl *PF_CFWriteStreamClose)( int a1); //
typedef int (__cdecl *PF_CFWriteStreamCopyProperty)( int a1,int a2); //

//CFStringRef
typedef CFIndex (__cdecl *PF_CFStringGetLength)(CFStringRef str);
typedef CFIndex (__cdecl *PF_CFStringGetMaximumSizeForEncoding)(CFIndex lengthOfUnicodeChar, CFStringEncoding encoding);
typedef CFTypeID (__cdecl *PF_CFStringGetTypeID)(); //
typedef CFStringRef (__cdecl *PF___CFStringMakeConstantString)(const char *str);
typedef bool (__cdecl *PF_CFStringGetCString)(CFStringRef theString, char *buffer, CFIndex bufferSize, CFStringEncoding encoding);
typedef const char * (__cdecl *PF_CFStringGetCStringPtr)(CFStringRef str, CFStringEncoding encoding);
typedef CFComparisonResult (__cdecl *PF_CFStringCompare)(CFStringRef str1, CFStringRef str2, CFStringCompareFlags compareFlags);
typedef CFComparisonResult (__cdecl *PF_CFStringCompareWithOptions)(CFStringRef str1, CFStringRef str2, CFRange range, CFStringCompareFlags compareFlag); // CFRange(location=字符串str1开始位置, length=字符串str1结束的位置,跟str2的位置无关)
typedef CFStringRef (__cdecl *PF_CFStringCreateWithCString)(CFAllocatorRef allocator, const char *cStr, CFStringEncoding encoding);
typedef CFStringRef (__cdecl *PF_CFStringCreateWithCharacters)(CFAllocatorRef allocator, const UniChar *uniChar, CFIndex numChars);//
typedef CFStringRef (__cdecl *PF_CFStringCreateCopy)(CFAllocatorRef allocator, CFStringRef str); //
typedef CFStringRef (__cdecl *PF_CFStringCreateWithFormat)(CFAllocatorRef allocator, CFDictionaryRef formatOptions, CFStringRef format, ...);
typedef SInt32 (__cdecl *PF_CFStringGetIntValue)(CFStringRef str);

//CFRange
typedef CFRange (__cdecl *PF_CFRangeMake)(CFIndex loc, CFIndex len);

//CFDictionaryRef CFMutableDictionaryRef
typedef CFMutableDictionaryRef (__cdecl *PF_CFDictionaryCreateMutable)(CFAllocatorRef allocator, CFIndex capacity, const CFDictionaryKeyCallBacks *keyCallBacks, const CFDictionaryValueCallBacks *valueCallBacks);
typedef CFDictionaryRef (__cdecl *PF_CFDictionaryCreateCopy)(CFAllocatorRef allocator, CFDictionaryRef theDict);
typedef CFMutableDictionaryRef (__cdecl *PF_CFDictionaryCreateMutableCopy)(CFAllocatorRef allocator, CFIndex capacity, CFDictionaryRef theDict);
typedef CFTypeID (__cdecl *PF_CFDictionaryGetTypeID)(void); //
typedef CFTypeRef (__cdecl *PF_CFDictionaryGetValue)(CFDictionaryRef theDict, CFTypeRef key);
typedef void (__cdecl *PF_CFDictionarySetValue)(CFMutableDictionaryRef theDict, CFTypeRef key, CFTypeRef value);
typedef void (__cdecl *PF_CFDictionaryAddValue)(CFMutableDictionaryRef theDict, CFTypeRef key, CFTypeRef value); //
typedef void (__cdecl *PF_CFDictionaryGetKeysAndValues)(CFDictionaryRef theDict, const void **keys, const void **values); //
typedef CFIndex (__cdecl *PF_CFDictionaryGetCount)(CFDictionaryRef theDict); //

//CFArrayRef
typedef int (__cdecl *PF_CFArrayCreateMutable)( int a1, int a2, int a3 ); //
typedef void * (__cdecl *PF_CFArrayAppendValue)(int Src, int a2); //
typedef CFIndex (__cdecl *PF_CFArrayGetCount)(CFArrayRef theArray);
typedef CFArrayRef (__cdecl *PF_CFArrayCreateCopy)(CFAllocatorRef allocator, CFArrayRef theArray);
typedef CFTypeRef (__cdecl *PF_CFArrayGetValueAtIndex)(CFArrayRef theArray, CFIndex idx);

enum
{
//ENUM_CFAbsoluteTimeAddGregorianUnits,
//ENUM_CFAbsoluteTimeGetCurrent,
//ENUM_CFAbsoluteTimeGetDayOfWeek,
//ENUM_CFAbsoluteTimeGetDayOfYear,
//ENUM_CFAbsoluteTimeGetDifferenceAsGregorianUnits,
//ENUM_CFAbsoluteTimeGetGregorianDate,
//ENUM_CFAbsoluteTimeGetWeekOfYear,
//ENUM_CFAllocatorAllocate,
//ENUM_CFAllocatorCreate,
//ENUM_CFAllocatorDeallocate,
//ENUM_CFAllocatorGetContext,
//ENUM_CFAllocatorGetDefault,
//ENUM_CFAllocatorGetPreferredSizeForSize,
//ENUM_CFAllocatorGetTypeID,
//ENUM_CFAllocatorReallocate,
//ENUM_CFAllocatorSetDefault,
//ENUM_CFArrayAppendArray,
ENUM_CFArrayAppendValue,
//ENUM_CFArrayApplyFunction,
//ENUM_CFArrayBSearchValues,
//ENUM_CFArrayContainsValue,
//ENUM_CFArrayCreate,
ENUM_CFArrayCreateCopy,
ENUM_CFArrayCreateMutable,
//ENUM_CFArrayCreateMutableCopy,
//ENUM_CFArrayExchangeValuesAtIndices,
ENUM_CFArrayGetCount,
//ENUM_CFArrayGetCountOfValue,
//ENUM_CFArrayGetFirstIndexOfValue,
//ENUM_CFArrayGetLastIndexOfValue,
//ENUM_CFArrayGetTypeID,
ENUM_CFArrayGetValueAtIndex,
//ENUM_CFArrayGetValues,
//ENUM_CFArrayInsertValueAtIndex,
//ENUM_CFArrayRemoveAllValues,
//ENUM_CFArrayRemoveValueAtIndex,
//ENUM_CFArrayReplaceValues,
//ENUM_CFArraySetValueAtIndex,
//ENUM_CFArraySortValues,
//ENUM_CFAttributedStringBeginEditing,
//ENUM_CFAttributedStringCreate,
//ENUM_CFAttributedStringCreateCopy,
//ENUM_CFAttributedStringCreateMutable,
//ENUM_CFAttributedStringCreateMutableCopy,
//ENUM_CFAttributedStringCreateWithSubstring,
//ENUM_CFAttributedStringEndEditing,
//ENUM_CFAttributedStringGetAttribute,
//ENUM_CFAttributedStringGetAttributeAndLongestEffectiveRange,
//ENUM_CFAttributedStringGetAttributes,
//ENUM_CFAttributedStringGetAttributesAndLongestEffectiveRange,
//ENUM_CFAttributedStringGetLength,
//ENUM_CFAttributedStringGetMutableString,
//ENUM_CFAttributedStringGetString,
//ENUM_CFAttributedStringGetTypeID,
//ENUM_CFAttributedStringOpenUText,
//ENUM_CFAttributedStringRemoveAttribute,
//ENUM_CFAttributedStringReplaceAttributedString,
//ENUM_CFAttributedStringReplaceString,
//ENUM_CFAttributedStringSetAttribute,
//ENUM_CFAttributedStringSetAttributes,
//ENUM_CFBSearch,
//ENUM_CFBagAddValue,
//ENUM_CFBagApplyFunction,
//ENUM_CFBagContainsValue,
//ENUM_CFBagCreate,
//ENUM_CFBagCreateCopy,
//ENUM_CFBagCreateMutable,
//ENUM_CFBagCreateMutableCopy,
//ENUM_CFBagGetCount,
//ENUM_CFBagGetCountOfValue,
//ENUM_CFBagGetTypeID,
//ENUM_CFBagGetValue,
//ENUM_CFBagGetValueIfPresent,
//ENUM_CFBagGetValues,
//ENUM_CFBagRemoveAllValues,
//ENUM_CFBagRemoveValue,
//ENUM_CFBagReplaceValue,
//ENUM_CFBagSetValue,
//ENUM_CFBinaryHeapAddValue,
//ENUM_CFBinaryHeapApplyFunction,
//ENUM_CFBinaryHeapContainsValue,
//ENUM_CFBinaryHeapCreate,
//ENUM_CFBinaryHeapCreateCopy,
//ENUM_CFBinaryHeapGetCount,
//ENUM_CFBinaryHeapGetCountOfValue,
//ENUM_CFBinaryHeapGetMinimum,
//ENUM_CFBinaryHeapGetMinimumIfPresent,
//ENUM_CFBinaryHeapGetTypeID,
//ENUM_CFBinaryHeapGetValues,
//ENUM_CFBinaryHeapRemoveAllValues,
//ENUM_CFBinaryHeapRemoveMinimumValue,
//ENUM_CFBitVectorContainsBit,
//ENUM_CFBitVectorCreate,
//ENUM_CFBitVectorCreateCopy,
//ENUM_CFBitVectorCreateMutable,
//ENUM_CFBitVectorCreateMutableCopy,
//ENUM_CFBitVectorFlipBitAtIndex,
//ENUM_CFBitVectorFlipBits,
//ENUM_CFBitVectorGetBitAtIndex,
//ENUM_CFBitVectorGetBits,
//ENUM_CFBitVectorGetCount,
//ENUM_CFBitVectorGetCountOfBit,
//ENUM_CFBitVectorGetFirstIndexOfBit,
//ENUM_CFBitVectorGetLastIndexOfBit,
//ENUM_CFBitVectorGetTypeID,
//ENUM_CFBitVectorSetAllBits,
//ENUM_CFBitVectorSetBitAtIndex,
//ENUM_CFBitVectorSetBits,
//ENUM_CFBitVectorSetCount,
//ENUM_CFBooleanGetTypeID,
ENUM_CFBooleanGetValue,
//ENUM_CFBundleCopyAuxiliaryExecutableURL,
//ENUM_CFBundleCopyBuiltInPlugInsURL,
//ENUM_CFBundleCopyBundleLocalizations,
//ENUM_CFBundleCopyBundleURL,
//ENUM_CFBundleCopyExecutableArchitectures,
//ENUM_CFBundleCopyExecutableArchitecturesForURL,
//ENUM_CFBundleCopyExecutableURL,
//ENUM_CFBundleCopyInfoDictionaryForURL,
//ENUM_CFBundleCopyInfoDictionaryInDirectory,
//ENUM_CFBundleCopyLocalizationForLocalizationInfo,
//ENUM_CFBundleCopyLocalizationsForPreferences,
//ENUM_CFBundleCopyLocalizationsForURL,
//ENUM_CFBundleCopyLocalizedString,
//ENUM_CFBundleCopyPreferredLocalizationsFromArray,
//ENUM_CFBundleCopyPrivateFrameworksURL,
//ENUM_CFBundleCopyResourceURL,
//ENUM_CFBundleCopyResourceURLForLocalization,
//ENUM_CFBundleCopyResourceURLInDirectory,
//ENUM_CFBundleCopyResourceURLsOfType,
//ENUM_CFBundleCopyResourceURLsOfTypeForLocalization,
//ENUM_CFBundleCopyResourceURLsOfTypeInDirectory,
//ENUM_CFBundleCopyResourcesDirectoryURL,
//ENUM_CFBundleCopySharedFrameworksURL,
//ENUM_CFBundleCopySharedSupportURL,
//ENUM_CFBundleCopySupportFilesDirectoryURL,
//ENUM_CFBundleCreate,
//ENUM_CFBundleCreateBundlesFromDirectory,
//ENUM_CFBundleGetAllBundles,
//ENUM_CFBundleGetBundleWithIdentifier,
//ENUM_CFBundleGetDataPointerForName,
//ENUM_CFBundleGetDataPointersForNames,
//ENUM_CFBundleGetDevelopmentRegion,
//ENUM_CFBundleGetFunctionPointerForName,
//ENUM_CFBundleGetFunctionPointersForNames,
//ENUM_CFBundleGetIdentifier,
//ENUM_CFBundleGetInfoDictionary,
//ENUM_CFBundleGetLocalInfoDictionary,
//ENUM_CFBundleGetLocalizationInfoForLocalization,
//ENUM_CFBundleGetMainBundle,
//ENUM_CFBundleGetPackageInfo,
//ENUM_CFBundleGetPackageInfoInDirectory,
//ENUM_CFBundleGetPlugIn,
//ENUM_CFBundleGetTypeID,
//ENUM_CFBundleGetValueForInfoDictionaryKey,
//ENUM_CFBundleGetVersionNumber,
//ENUM_CFBundleIsExecutableLoaded,
//ENUM_CFBundleLoadExecutable,
//ENUM_CFBundleLoadExecutableAndReturnError,
//ENUM_CFBundlePreflightExecutable,
//ENUM_CFBundleUnloadExecutable,
//ENUM_CFBurstTrieAdd,
//ENUM_CFBurstTrieAddCharacters,
//ENUM_CFBurstTrieAddCharactersWithWeight,
//ENUM_CFBurstTrieAddUTF8String,
//ENUM_CFBurstTrieAddUTF8StringWithWeight,
//ENUM_CFBurstTrieAddWithWeight,
//ENUM_CFBurstTrieContains,
//ENUM_CFBurstTrieContainsCharacters,
//ENUM_CFBurstTrieContainsUTF8String,
//ENUM_CFBurstTrieCreate,
//ENUM_CFBurstTrieCreateFromFile,
//ENUM_CFBurstTrieCreateFromMapBytes,
//ENUM_CFBurstTrieFind,
//ENUM_CFBurstTrieFindCharacters,
//ENUM_CFBurstTrieFindUTF8String,
//ENUM_CFBurstTrieGetCount,
//ENUM_CFBurstTrieInsert,
//ENUM_CFBurstTrieInsertCharacters,
//ENUM_CFBurstTrieInsertCharactersWithWeight,
//ENUM_CFBurstTrieInsertUTF8String,
//ENUM_CFBurstTrieInsertUTF8StringWithWeight,
//ENUM_CFBurstTrieInsertWithWeight,
//ENUM_CFBurstTrieRelease,
//ENUM_CFBurstTrieRetain,
//ENUM_CFBurstTrieSerialize,
//ENUM_CFBurstTrieSerializeWithFileDescriptor,
//ENUM_CFBurstTrieTraverse,
//ENUM_CFCalendarAddComponents,
//ENUM_CFCalendarComposeAbsoluteTime,
//ENUM_CFCalendarCopyCurrent,
//ENUM_CFCalendarCopyGregorianStartDate,
//ENUM_CFCalendarCopyLocale,
//ENUM_CFCalendarCopyTimeZone,
//ENUM_CFCalendarCreateWithIdentifier,
//ENUM_CFCalendarDecomposeAbsoluteTime,
//ENUM_CFCalendarGetComponentDifference,
//ENUM_CFCalendarGetFirstWeekday,
//ENUM_CFCalendarGetIdentifier,
//ENUM_CFCalendarGetMaximumRangeOfUnit,
//ENUM_CFCalendarGetMinimumDaysInFirstWeek,
//ENUM_CFCalendarGetMinimumRangeOfUnit,
//ENUM_CFCalendarGetOrdinalityOfUnit,
//ENUM_CFCalendarGetRangeOfUnit,
//ENUM_CFCalendarGetTimeRangeOfUnit,
//ENUM_CFCalendarGetTypeID,
//ENUM_CFCalendarSetFirstWeekday,
//ENUM_CFCalendarSetGregorianStartDate,
//ENUM_CFCalendarSetLocale,
//ENUM_CFCalendarSetMinimumDaysInFirstWeek,
//ENUM_CFCalendarSetTimeZone,
//ENUM_CFCharacterSetAddCharactersInRange,
//ENUM_CFCharacterSetAddCharactersInString,
//ENUM_CFCharacterSetCompact,
//ENUM_CFCharacterSetCreateBitmapRepresentation,
//ENUM_CFCharacterSetCreateCopy,
//ENUM_CFCharacterSetCreateInvertedSet,
//ENUM_CFCharacterSetCreateMutable,
//ENUM_CFCharacterSetCreateMutableCopy,
//ENUM_CFCharacterSetCreateWithBitmapRepresentation,
//ENUM_CFCharacterSetCreateWithCharactersInRange,
//ENUM_CFCharacterSetCreateWithCharactersInString,
//ENUM_CFCharacterSetFast,
//ENUM_CFCharacterSetGetPredefined,
//ENUM_CFCharacterSetGetTypeID,
//ENUM_CFCharacterSetHasMemberInPlane,
//ENUM_CFCharacterSetInitInlineBuffer,
//ENUM_CFCharacterSetIntersect,
//ENUM_CFCharacterSetInvert,
//ENUM_CFCharacterSetIsCharacterMember,
//ENUM_CFCharacterSetIsLongCharacterMember,
//ENUM_CFCharacterSetIsSupersetOfSet,
//ENUM_CFCharacterSetIsSurrogatePairMember,
//ENUM_CFCharacterSetRemoveCharactersInRange,
//ENUM_CFCharacterSetRemoveCharactersInString,
//ENUM_CFCharacterSetUnion,
//ENUM_CFCopyDescription,
//ENUM_CFCopyHomeDirectoryURLForUser,
//ENUM_CFCopySearchPathForDirectoriesInDomains,
//ENUM_CFCopySystemVersionString,
//ENUM_CFCopyTypeIDDescription,
//ENUM_CFCopyUserName,
//ENUM_CFDataAppendBytes,
ENUM_CFDataCreate,
//ENUM_CFDataCreateCopy,
//ENUM_CFDataCreateMutable,
//ENUM_CFDataCreateMutableCopy,
//ENUM_CFDataCreateWithBytesNoCopy,
//ENUM_CFDataDeleteBytes,
//ENUM_CFDataFind,
ENUM_CFDataGetBytePtr,
//ENUM_CFDataGetBytes,
ENUM_CFDataGetLength,
//ENUM_CFDataGetMutableBytePtr,
//ENUM_CFDataGetTypeID,
//ENUM_CFDataIncreaseLength,
//ENUM_CFDataReplaceBytes,
//ENUM_CFDataSetLength,
//ENUM_CFDateCompare,
//ENUM_CFDateCreate,
//ENUM_CFDateFormatterCopyProperty,
//ENUM_CFDateFormatterCreate,
//ENUM_CFDateFormatterCreateDateFormatFromTemplate,
//ENUM_CFDateFormatterCreateDateFormatsFromTemplates,
//ENUM_CFDateFormatterCreateDateFromString,
//ENUM_CFDateFormatterCreateStringWithAbsoluteTime,
//ENUM_CFDateFormatterCreateStringWithDate,
//ENUM_CFDateFormatterGetAbsoluteTimeFromString,
//ENUM_CFDateFormatterGetDateStyle,
//ENUM_CFDateFormatterGetFormat,
//ENUM_CFDateFormatterGetLocale,
//ENUM_CFDateFormatterGetTimeStyle,
//ENUM_CFDateFormatterGetTypeID,
//ENUM_CFDateFormatterSetFormat,
//ENUM_CFDateFormatterSetProperty,
//ENUM_CFDateGetAbsoluteTime,
//ENUM_CFDateGetTimeIntervalSinceDate,
//ENUM_CFDateGetTypeID,
ENUM_CFDictionaryAddValue,
//ENUM_CFDictionaryApplyFunction,
//ENUM_CFDictionaryContainsKey,
//ENUM_CFDictionaryContainsValue,
//ENUM_CFDictionaryCreate,
ENUM_CFDictionaryCreateCopy,
ENUM_CFDictionaryCreateMutable,
ENUM_CFDictionaryCreateMutableCopy,
ENUM_CFDictionaryGetCount,
//ENUM_CFDictionaryGetCountOfKey,
//ENUM_CFDictionaryGetCountOfValue,
//ENUM_CFDictionaryGetKeyIfPresent,
ENUM_CFDictionaryGetKeysAndValues,
ENUM_CFDictionaryGetTypeID,
ENUM_CFDictionaryGetValue,
//ENUM_CFDictionaryGetValueIfPresent,
//ENUM_CFDictionaryRemoveAllValues,
//ENUM_CFDictionaryRemoveValue,
//ENUM_CFDictionaryReplaceValue,
ENUM_CFDictionarySetValue,
//ENUM_CFEqual,
//ENUM_CFErrorCopyDescription,
//ENUM_CFErrorCopyFailureReason,
//ENUM_CFErrorCopyRecoverySuggestion,
//ENUM_CFErrorCopyUserInfo,
//ENUM_CFErrorCreate,
//ENUM_CFErrorCreateWithUserInfoKeysAndValues,
//ENUM_CFErrorGetCallBackForDomain,
//ENUM_CFErrorGetCode,
//ENUM_CFErrorGetDomain,
//ENUM_CFErrorGetTypeID,
//ENUM_CFErrorSetCallBackForDomain,
//ENUM_CFGetAllocator,
//ENUM_CFGetRetainCount,
//ENUM_CFGetSystemUptime,
ENUM_CFGetTypeID,
//ENUM_CFGetUserName,
//ENUM_CFGregorianDateGetAbsoluteTime,
//ENUM_CFGregorianDateIsValid,
//ENUM_CFHash,
//ENUM_CFHashBytes,
//ENUM_CFLocaleCopyAvailableLocaleIdentifiers,
//ENUM_CFLocaleCopyCommonISOCurrencyCodes,
//ENUM_CFLocaleCopyCurrent,
//ENUM_CFLocaleCopyDisplayNameForPropertyValue,
//ENUM_CFLocaleCopyISOCountryCodes,
//ENUM_CFLocaleCopyISOCurrencyCodes,
//ENUM_CFLocaleCopyISOLanguageCodes,
//ENUM_CFLocaleCopyPreferredLanguages,
//ENUM_CFLocaleCreate,
//ENUM_CFLocaleCreateCanonicalLanguageIdentifierFromString,
//ENUM_CFLocaleCreateCanonicalLocaleIdentifierFromScriptManagerCodes,
//ENUM_CFLocaleCreateCanonicalLocaleIdentifierFromString,
//ENUM_CFLocaleCreateComponentsFromLocaleIdentifier,
//ENUM_CFLocaleCreateCopy,
//ENUM_CFLocaleCreateLocaleIdentifierFromComponents,
//ENUM_CFLocaleCreateLocaleIdentifierFromWindowsLocaleCode,
//ENUM_CFLocaleGetIdentifier,
//ENUM_CFLocaleGetLanguageCharacterDirection,
//ENUM_CFLocaleGetLanguageLineDirection,
//ENUM_CFLocaleGetLanguageRegionEncodingForLocaleIdentifier,
//ENUM_CFLocaleGetSystem,
//ENUM_CFLocaleGetTypeID,
//ENUM_CFLocaleGetValue,
//ENUM_CFLocaleGetWindowsLocaleCodeFromLocaleIdentifier,
//ENUM_CFLog,
//ENUM_CFMakeCollectable,
//ENUM_CFMergeSortArray,
//ENUM_CFMessagePortCreateLocal,
//ENUM_CFMessagePortCreateRemote,
//ENUM_CFMessagePortCreateRunLoopSource,
//ENUM_CFMessagePortCreateUber,
//ENUM_CFMessagePortGetContext,
//ENUM_CFMessagePortGetInvalidationCallBack,
//ENUM_CFMessagePortGetName,
//ENUM_CFMessagePortGetTypeID,
//ENUM_CFMessagePortInvalidate,
//ENUM_CFMessagePortIsRemote,
//ENUM_CFMessagePortIsValid,
//ENUM_CFMessagePortSendRequest,
//ENUM_CFMessagePortSetCloneCallout,
//ENUM_CFMessagePortSetInvalidationCallBack,
//ENUM_CFMessagePortSetName,
//ENUM_CFMutableAttributedStringOpenUText,
//ENUM_CFMutableStringOpenUText,
//ENUM_CFNotificationCenterAddObserver,
//ENUM_CFNotificationCenterGetDarwinNotifyCenter,
//ENUM_CFNotificationCenterGetDistributedCenter,
//ENUM_CFNotificationCenterGetLocalCenter,
//ENUM_CFNotificationCenterGetTypeID,
//ENUM_CFNotificationCenterPostNotification,
//ENUM_CFNotificationCenterPostNotificationWithOptions,
//ENUM_CFNotificationCenterRemoveEveryObserver,
//ENUM_CFNotificationCenterRemoveObserver,
//ENUM_CFNullGetTypeID,
//ENUM_CFNumberCompare,
ENUM_CFNumberCreate,
//ENUM_CFNumberFormatterCopyProperty,
//ENUM_CFNumberFormatterCreate,
//ENUM_CFNumberFormatterCreateNumberFromString,
//ENUM_CFNumberFormatterCreateStringWithNumber,
//ENUM_CFNumberFormatterCreateStringWithValue,
//ENUM_CFNumberFormatterGetDecimalInfoForCurrencyCode,
//ENUM_CFNumberFormatterGetFormat,
//ENUM_CFNumberFormatterGetLocale,
//ENUM_CFNumberFormatterGetStyle,
//ENUM_CFNumberFormatterGetTypeID,
//ENUM_CFNumberFormatterGetValueFromString,
//ENUM_CFNumberFormatterSetFormat,
//ENUM_CFNumberFormatterSetProperty,
//ENUM_CFNumberGetByteSize,
//ENUM_CFNumberGetType,
//ENUM_CFNumberGetTypeID,
ENUM_CFNumberGetValue,
//ENUM_CFNumberIsFloatType,
//ENUM_CFPlugInAddInstanceForFactory,
//ENUM_CFPlugInCreate,
//ENUM_CFPlugInFindFactoriesForPlugInType,
//ENUM_CFPlugInFindFactoriesForPlugInTypeInPlugIn,
//ENUM_CFPlugInGetBundle,
//ENUM_CFPlugInGetTypeID,
//ENUM_CFPlugInInstanceCreate,
//ENUM_CFPlugInInstanceCreateWithInstanceDataSize,
//ENUM_CFPlugInInstanceGetFactoryName,
//ENUM_CFPlugInInstanceGetInstanceData,
//ENUM_CFPlugInInstanceGetInterfaceFunctionTable,
//ENUM_CFPlugInInstanceGetTypeID,
//ENUM_CFPlugInIsLoadOnDemand,
//ENUM_CFPlugInRegisterFactoryFunction,
//ENUM_CFPlugInRegisterFactoryFunctionByName,
//ENUM_CFPlugInRegisterPlugInType,
//ENUM_CFPlugInRemoveInstanceForFactory,
//ENUM_CFPlugInSetLoadOnDemand,
//ENUM_CFPlugInUnregisterFactory,
//ENUM_CFPlugInUnregisterPlugInType,
//ENUM_CFPreferencesAddSuitePreferencesToApp,
//ENUM_CFPreferencesAppSynchronize,
//ENUM_CFPreferencesAppValueIsForced,
//ENUM_CFPreferencesCopyAppValue,
//ENUM_CFPreferencesCopyApplicationList,
//ENUM_CFPreferencesCopyKeyList,
//ENUM_CFPreferencesCopyMultiple,
//ENUM_CFPreferencesCopyValue,
//ENUM_CFPreferencesFlushCaches,
//ENUM_CFPreferencesGetAppBooleanValue,
//ENUM_CFPreferencesGetAppIntegerValue,
//ENUM_CFPreferencesRemoveSuitePreferencesFromApp,
//ENUM_CFPreferencesSetAppValue,
//ENUM_CFPreferencesSetMultiple,
//ENUM_CFPreferencesSetValue,
//ENUM_CFPreferencesSynchronize,
ENUM_CFPropertyListCreateData,
//ENUM_CFPropertyListCreateDeepCopy,
//ENUM_CFPropertyListCreateFromStream,
ENUM_CFPropertyListCreateFromXMLData,
//ENUM_CFPropertyListCreateWithData,
//ENUM_CFPropertyListCreateWithStream,
ENUM_CFPropertyListCreateXMLData,
//ENUM_CFPropertyListIsValid,
//ENUM_CFPropertyListWrite,
ENUM_CFPropertyListWriteToStream,
//ENUM_CFQSortArray,
//ENUM_CFReadStreamClose,
//ENUM_CFReadStreamCopyError,
//ENUM_CFReadStreamCopyProperty,
//ENUM_CFReadStreamCreate,
//ENUM_CFReadStreamCreateWithBytesNoCopy,
//ENUM_CFReadStreamCreateWithData,
//ENUM_CFReadStreamCreateWithFile,
//ENUM_CFReadStreamGetBuffer,
//ENUM_CFReadStreamGetError,
//ENUM_CFReadStreamGetInfoPointer,
//ENUM_CFReadStreamGetStatus,
//ENUM_CFReadStreamGetTypeID,
//ENUM_CFReadStreamHasBytesAvailable,
//ENUM_CFReadStreamOpen,
//ENUM_CFReadStreamRead,
//ENUM_CFReadStreamScheduleWithRunLoop,
//ENUM_CFReadStreamSetClient,
//ENUM_CFReadStreamSetProperty,
//ENUM_CFReadStreamSignalEvent,
//ENUM_CFReadStreamUnscheduleFromRunLoop,
ENUM_CFRelease,
//ENUM_CFRetain,
//ENUM_CFRunArrayCreate,
//ENUM_CFRunArrayDelete,
//ENUM_CFRunArrayGetCount,
//ENUM_CFRunArrayGetTypeID,
//ENUM_CFRunArrayGetValueAtIndex,
//ENUM_CFRunArrayGetValueAtRunArrayIndex,
//ENUM_CFRunArrayInsert,
//ENUM_CFRunArrayReplace,
//ENUM_CFRunLoopAddCommonMode,
//ENUM_CFRunLoopAddObserver,
//ENUM_CFRunLoopAddSource,
//ENUM_CFRunLoopAddTimer,
//ENUM_CFRunLoopContainsObserver,
//ENUM_CFRunLoopContainsSource,
//ENUM_CFRunLoopContainsTimer,
//ENUM_CFRunLoopCopyAllModes,
//ENUM_CFRunLoopCopyCurrentMode,
//ENUM_CFRunLoopGetCurrent,
//ENUM_CFRunLoopGetMain,
//ENUM_CFRunLoopGetNextTimerFireDate,
//ENUM_CFRunLoopGetTypeID,
//ENUM_CFRunLoopIsWaiting,
//ENUM_CFRunLoopObserverCreate,
//ENUM_CFRunLoopObserverCreateWithHandler,
//ENUM_CFRunLoopObserverDoesRepeat,
//ENUM_CFRunLoopObserverGetActivities,
//ENUM_CFRunLoopObserverGetContext,
//ENUM_CFRunLoopObserverGetOrder,
//ENUM_CFRunLoopObserverGetTypeID,
//ENUM_CFRunLoopObserverInvalidate,
//ENUM_CFRunLoopObserverIsValid,
//ENUM_CFRunLoopPerformBlock,
//ENUM_CFRunLoopRemoveObserver,
//ENUM_CFRunLoopRemoveSource,
//ENUM_CFRunLoopRemoveTimer,
//ENUM_CFRunLoopRun,
//ENUM_CFRunLoopRunInMode,
//ENUM_CFRunLoopSourceCreate,
//ENUM_CFRunLoopSourceGetContext,
//ENUM_CFRunLoopSourceGetOrder,
//ENUM_CFRunLoopSourceGetTypeID,
//ENUM_CFRunLoopSourceInvalidate,
//ENUM_CFRunLoopSourceIsValid,
//ENUM_CFRunLoopSourceSignal,
//ENUM_CFRunLoopStop,
//ENUM_CFRunLoopTimerCreate,
//ENUM_CFRunLoopTimerCreateWithHandler,
//ENUM_CFRunLoopTimerDoesRepeat,
//ENUM_CFRunLoopTimerGetContext,
//ENUM_CFRunLoopTimerGetInterval,
//ENUM_CFRunLoopTimerGetNextFireDate,
//ENUM_CFRunLoopTimerGetOrder,
//ENUM_CFRunLoopTimerGetTypeID,
//ENUM_CFRunLoopTimerInvalidate,
//ENUM_CFRunLoopTimerIsValid,
//ENUM_CFRunLoopTimerSetNextFireDate,
//ENUM_CFRunLoopWakeUp,
//ENUM_CFSetAddValue,
//ENUM_CFSetApplyFunction,
//ENUM_CFSetContainsValue,
//ENUM_CFSetCreate,
//ENUM_CFSetCreateCopy,
//ENUM_CFSetCreateMutable,
//ENUM_CFSetCreateMutableCopy,
//ENUM_CFSetGetCount,
//ENUM_CFSetGetCountOfValue,
//ENUM_CFSetGetTypeID,
//ENUM_CFSetGetValue,
//ENUM_CFSetGetValueIfPresent,
//ENUM_CFSetGetValues,
//ENUM_CFSetRemoveAllValues,
//ENUM_CFSetRemoveValue,
//ENUM_CFSetReplaceValue,
//ENUM_CFSetSetValue,
//ENUM_CFShow,
//ENUM_CFShowStr,
//ENUM_CFSocketConnectToAddress,
//ENUM_CFSocketCopyAddress,
//ENUM_CFSocketCopyPeerAddress,
//ENUM_CFSocketCopyRegisteredSocketSignature,
//ENUM_CFSocketCopyRegisteredValue,
//ENUM_CFSocketCreate,
//ENUM_CFSocketCreateConnectedToSocketSignature,
//ENUM_CFSocketCreateRunLoopSource,
//ENUM_CFSocketCreateWithNative,
//ENUM_CFSocketCreateWithSocketSignature,
//ENUM_CFSocketDisableCallBacks,
//ENUM_CFSocketEnableCallBacks,
//ENUM_CFSocketGetContext,
//ENUM_CFSocketGetDefaultNameRegistryPortNumber,
//ENUM_CFSocketGetNative,
//ENUM_CFSocketGetSocketFlags,
//ENUM_CFSocketGetTypeID,
//ENUM_CFSocketInvalidate,
//ENUM_CFSocketIsValid,
//ENUM_CFSocketRegisterSocketSignature,
//ENUM_CFSocketRegisterValue,
//ENUM_CFSocketSendData,
//ENUM_CFSocketSetAddress,
//ENUM_CFSocketSetDefaultNameRegistryPortNumber,
//ENUM_CFSocketSetSocketFlags,
//ENUM_CFSocketUnregister,
//ENUM_CFStorageApplyBlock,
//ENUM_CFStorageApplyFunction,
//ENUM_CFStorageCreate,
//ENUM_CFStorageCreateWithSubrange,
//ENUM_CFStorageDeleteValues,
//ENUM_CFStorageGetConstValueAtIndex,
//ENUM_CFStorageGetCount,
//ENUM_CFStorageGetTypeID,
//ENUM_CFStorageGetValueAtIndex,
//ENUM_CFStorageGetValues,
//ENUM_CFStorageInsertValues,
//ENUM_CFStorageReplaceValues,
//ENUM_CFStreamCreateBoundPair,
//ENUM_CFStreamCreatePairWithPeerSocketSignature,
//ENUM_CFStreamCreatePairWithSocket,
//ENUM_CFStreamCreatePairWithSocketToHost,
//ENUM_CFStringAppend,
//ENUM_CFStringAppendCString,
//ENUM_CFStringAppendCharacters,
//ENUM_CFStringAppendFormat,
//ENUM_CFStringAppendFormatAndArguments,
//ENUM_CFStringAppendPascalString,
//ENUM_CFStringCapitalize,
ENUM_CFStringCompare,
ENUM_CFStringCompareWithOptions,
//ENUM_CFStringCompareWithOptionsAndLocale,
//ENUM_CFStringConvertEncodingToIANACharSetName,
//ENUM_CFStringConvertEncodingToNSStringEncoding,
//ENUM_CFStringConvertEncodingToWindowsCodepage,
//ENUM_CFStringConvertIANACharSetNameToEncoding,
//ENUM_CFStringConvertNSStringEncodingToEncoding,
//ENUM_CFStringConvertWindowsCodepageToEncoding,
//ENUM_CFStringCreateArrayBySeparatingStrings,
//ENUM_CFStringCreateArrayWithFindResults,
//ENUM_CFStringCreateByCombiningStrings,
ENUM_CFStringCreateCopy,
//ENUM_CFStringCreateExternalRepresentation,
//ENUM_CFStringCreateFromExternalRepresentation,
//ENUM_CFStringCreateMutable,
//ENUM_CFStringCreateMutableCopy,
//ENUM_CFStringCreateMutableWithExternalCharactersNoCopy,
//ENUM_CFStringCreateWithBytes,
//ENUM_CFStringCreateWithBytesNoCopy,
ENUM_CFStringCreateWithCString,
//ENUM_CFStringCreateWithCStringNoCopy,
ENUM_CFStringCreateWithCharacters,
//ENUM_CFStringCreateWithCharactersNoCopy,
//ENUM_CFStringCreateWithFileSystemRepresentation,
ENUM_CFStringCreateWithFormat,
//ENUM_CFStringCreateWithFormatAndArguments,
//ENUM_CFStringCreateWithPascalString,
//ENUM_CFStringCreateWithPascalStringNoCopy,
//ENUM_CFStringCreateWithSubstring,
//ENUM_CFStringDelete,
//ENUM_CFStringEncodingByteLengthForCharacters,
//ENUM_CFStringEncodingBytesToUnicode,
//ENUM_CFStringEncodingCharLengthForBytes,
//ENUM_CFStringEncodingIsValidEncoding,
//ENUM_CFStringEncodingListOfAvailableEncodings,
//ENUM_CFStringEncodingRegisterFallbackProcedures,
//ENUM_CFStringEncodingUnicodeToBytes,
//ENUM_CFStringFileSystemEncoding,
//ENUM_CFStringFind,
//ENUM_CFStringFindAndReplace,
//ENUM_CFStringFindCharacterFromSet,
//ENUM_CFStringFindWithOptions,
//ENUM_CFStringFindWithOptionsAndLocale,
//ENUM_CFStringFold,
//ENUM_CFStringGetBytes,
ENUM_CFStringGetCString,
ENUM_CFStringGetCStringPtr,
//ENUM_CFStringGetCharacterAtIndex,
//ENUM_CFStringGetCharacters,
//ENUM_CFStringGetCharactersPtr,
//ENUM_CFStringGetDoubleValue,
//ENUM_CFStringGetFastestEncoding,
//ENUM_CFStringGetFileSystemRepresentation,
//ENUM_CFStringGetHyphenationLocationBeforeIndex,
ENUM_CFStringGetIntValue,
ENUM_CFStringGetLength,
//ENUM_CFStringGetLineBounds,
//ENUM_CFStringGetListOfAvailableEncodings,
ENUM_CFStringGetMaximumSizeForEncoding,
//ENUM_CFStringGetMaximumSizeOfFileSystemRepresentation,
//ENUM_CFStringGetMostCompatibleMacStringEncoding,
//ENUM_CFStringGetNameOfEncoding,
//ENUM_CFStringGetParagraphBounds,
//ENUM_CFStringGetPascalString,
//ENUM_CFStringGetPascalStringPtr,
//ENUM_CFStringGetRangeOfCharacterClusterAtIndex,
//ENUM_CFStringGetRangeOfComposedCharactersAtIndex,
//ENUM_CFStringGetSmallestEncoding,
//ENUM_CFStringGetSystemEncoding,
ENUM_CFStringGetTypeID,
//ENUM_CFStringHasPrefix,
//ENUM_CFStringHasSuffix,
//ENUM_CFStringHashCString,
//ENUM_CFStringHashCharacters,
//ENUM_CFStringHashISOLatin1CString,
//ENUM_CFStringHashNSString,
//ENUM_CFStringInsert,
//ENUM_CFStringIsEncodingAvailable,
//ENUM_CFStringIsHyphenationAvailableForLocale,
//ENUM_CFStringLowercase,
//ENUM_CFStringNormalize,
//ENUM_CFStringOpenUText,
//ENUM_CFStringPad,
//ENUM_CFStringReplace,
//ENUM_CFStringReplaceAll,
//ENUM_CFStringSetExternalCharactersNoCopy,
//ENUM_CFStringTransform,
//ENUM_CFStringTrim,
//ENUM_CFStringTrimWhitespace,
//ENUM_CFStringUppercase,
//ENUM_CFTimeZoneCopyAbbreviation,
//ENUM_CFTimeZoneCopyAbbreviationDictionary,
//ENUM_CFTimeZoneCopyDefault,
//ENUM_CFTimeZoneCopyKnownNames,
//ENUM_CFTimeZoneCopyLocalizedName,
//ENUM_CFTimeZoneCopySystem,
//ENUM_CFTimeZoneCreate,
//ENUM_CFTimeZoneCreateWithName,
//ENUM_CFTimeZoneCreateWithTimeIntervalFromGMT,
//ENUM_CFTimeZoneGetData,
//ENUM_CFTimeZoneGetDaylightSavingTimeOffset,
//ENUM_CFTimeZoneGetName,
//ENUM_CFTimeZoneGetNextDaylightSavingTimeTransition,
//ENUM_CFTimeZoneGetSecondsFromGMT,
//ENUM_CFTimeZoneGetTypeID,
//ENUM_CFTimeZoneIsDaylightSavingTime,
//ENUM_CFTimeZoneResetSystem,
//ENUM_CFTimeZoneSetAbbreviationDictionary,
//ENUM_CFTimeZoneSetDefault,
//ENUM_CFTreeAppendChild,
//ENUM_CFTreeApplyFunctionToChildren,
//ENUM_CFTreeCreate,
//ENUM_CFTreeFindRoot,
//ENUM_CFTreeGetChildAtIndex,
//ENUM_CFTreeGetChildCount,
//ENUM_CFTreeGetChildren,
//ENUM_CFTreeGetContext,
//ENUM_CFTreeGetFirstChild,
//ENUM_CFTreeGetNextSibling,
//ENUM_CFTreeGetParent,
//ENUM_CFTreeGetTypeID,
//ENUM_CFTreeInsertSibling,
//ENUM_CFTreePrependChild,
//ENUM_CFTreeRemove,
//ENUM_CFTreeRemoveAllChildren,
//ENUM_CFTreeSetContext,
//ENUM_CFTreeSortChildren,
//ENUM_CFTypeGetTypeID,
//ENUM_CFURLCanBeDecomposed,
//ENUM_CFURLCopyAbsoluteURL,
//ENUM_CFURLCopyFileSystemPath,
//ENUM_CFURLCopyFragment,
//ENUM_CFURLCopyHostName,
//ENUM_CFURLCopyLastPathComponent,
//ENUM_CFURLCopyNetLocation,
//ENUM_CFURLCopyParameterString,
//ENUM_CFURLCopyPassword,
//ENUM_CFURLCopyPath,
//ENUM_CFURLCopyPathExtension,
//ENUM_CFURLCopyQueryString,
//ENUM_CFURLCopyResourceSpecifier,
//ENUM_CFURLCopyScheme,
//ENUM_CFURLCopyStrictPath,
//ENUM_CFURLCopyUserName,
//ENUM_CFURLCreateAbsoluteURLWithBytes,
//ENUM_CFURLCreateCopyAppendingPathComponent,
//ENUM_CFURLCreateCopyAppendingPathExtension,
//ENUM_CFURLCreateCopyDeletingLastPathComponent,
//ENUM_CFURLCreateCopyDeletingPathExtension,
//ENUM_CFURLCreateData,
//ENUM_CFURLCreateDataAndPropertiesFromResource,
//ENUM_CFURLCreateFromFileSystemRepresentation,
//ENUM_CFURLCreateFromFileSystemRepresentationRelativeToBase,
//ENUM_CFURLCreatePropertyFromResource,
//ENUM_CFURLCreateStringByAddingPercentEscapes,
//ENUM_CFURLCreateStringByReplacingPercentEscapes,
//ENUM_CFURLCreateStringByReplacingPercentEscapesUsingEncoding,
//ENUM_CFURLCreateWithBytes,
//ENUM_CFURLCreateWithFileSystemPath,
//ENUM_CFURLCreateWithFileSystemPathRelativeToBase,
//ENUM_CFURLCreateWithString,
//ENUM_CFURLDestroyResource,
//ENUM_CFURLEnumeratorCreateForDirectoryURL,
//ENUM_CFURLEnumeratorCreateForMountedVolumes,
//ENUM_CFURLEnumeratorGetDescendentLevel,
//ENUM_CFURLEnumeratorGetNextURL,
//ENUM_CFURLEnumeratorGetSourceDidChange,
//ENUM_CFURLEnumeratorGetTypeID,
//ENUM_CFURLEnumeratorSkipDescendents,
//ENUM_CFURLGetBaseURL,
//ENUM_CFURLGetByteRangeForComponent,
//ENUM_CFURLGetBytes,
//ENUM_CFURLGetFileSystemRepresentation,
//ENUM_CFURLGetPortNumber,
//ENUM_CFURLGetString,
//ENUM_CFURLGetTypeID,
//ENUM_CFURLHasDirectoryPath,
//ENUM_CFURLWriteDataAndPropertiesToResource,
//ENUM_CFUUIDCreate,
//ENUM_CFUUIDCreateFromString,
//ENUM_CFUUIDCreateFromUUIDBytes,
//ENUM_CFUUIDCreateString,
//ENUM_CFUUIDCreateWithBytes,
//ENUM_CFUUIDGetConstantUUIDWithBytes,
//ENUM_CFUUIDGetTypeID,
//ENUM_CFUUIDGetUUIDBytes,
//ENUM_CFUniCharCompatibilityDecompose,
//ENUM_CFUniCharDecompose,
//ENUM_CFUniCharDecomposeCharacter,
//ENUM_CFUniCharFillDestinationBuffer,
//ENUM_CFUniCharGetBitmapForPlane,
//ENUM_CFUniCharGetBitmapPtrForPlane,
//ENUM_CFUniCharGetConditionalCaseMappingFlags,
//ENUM_CFUniCharGetMappingData,
//ENUM_CFUniCharGetNumberOfPlanes,
//ENUM_CFUniCharGetNumberOfPlanesForUnicodePropertyData,
//ENUM_CFUniCharGetUnicodeProperty,
//ENUM_CFUniCharGetUnicodePropertyDataForPlane,
//ENUM_CFUniCharIsMemberOf,
//ENUM_CFUniCharMapCaseTo,
//ENUM_CFUniCharPrecompose,
//ENUM_CFUniCharPrecomposeCharacter,
//ENUM_CFUniCharPrioritySort,
//ENUM_CFWindowsNamedPipeCreateConnectToName,
//ENUM_CFWindowsNamedPipeCreateRunLoopSource,
//ENUM_CFWindowsNamedPipeCreateWithName,
//ENUM_CFWindowsNamedPipeGetContext,
//ENUM_CFWindowsNamedPipeGetHandle,
//ENUM_CFWindowsNamedPipeGetName,
//ENUM_CFWindowsNamedPipeGetTypeID,
//ENUM_CFWindowsNamedPipeInvalidate,
//ENUM_CFWindowsNamedPipeIsValid,
//ENUM_CFWindowsNamedPipeSetInvalidationCallBack,
//ENUM_CFWriteStreamCanAcceptBytes,
ENUM_CFWriteStreamClose,
//ENUM_CFWriteStreamCopyError,
ENUM_CFWriteStreamCopyProperty,
//ENUM_CFWriteStreamCreate,
ENUM_CFWriteStreamCreateWithAllocatedBuffers,
//ENUM_CFWriteStreamCreateWithBuffer,
//ENUM_CFWriteStreamCreateWithFile,
//ENUM_CFWriteStreamGetError,
//ENUM_CFWriteStreamGetInfoPointer,
//ENUM_CFWriteStreamGetStatus,
//ENUM_CFWriteStreamGetTypeID,
ENUM_CFWriteStreamOpen,
//ENUM_CFWriteStreamScheduleWithRunLoop,
//ENUM_CFWriteStreamSetClient,
//ENUM_CFWriteStreamSetProperty,
//ENUM_CFWriteStreamSignalEvent,
//ENUM_CFWriteStreamUnscheduleFromRunLoop,
//ENUM_CFWriteStreamWrite,
//ENUM_CFXMLCreateStringByEscapingEntities,
//ENUM_CFXMLCreateStringByUnescapingEntities,
//ENUM_CFXMLNodeCreate,
//ENUM_CFXMLNodeCreateCopy,
//ENUM_CFXMLNodeGetInfoPtr,
//ENUM_CFXMLNodeGetString,
//ENUM_CFXMLNodeGetTypeCode,
//ENUM_CFXMLNodeGetTypeID,
//ENUM_CFXMLNodeGetVersion,
//ENUM_CFXMLParserAbort,
//ENUM_CFXMLParserCopyErrorDescription,
//ENUM_CFXMLParserCreate,
//ENUM_CFXMLParserCreateWithDataFromURL,
//ENUM_CFXMLParserGetCallBacks,
//ENUM_CFXMLParserGetContext,
//ENUM_CFXMLParserGetDocument,
//ENUM_CFXMLParserGetLineNumber,
//ENUM_CFXMLParserGetLocation,
//ENUM_CFXMLParserGetSourceURL,
//ENUM_CFXMLParserGetStatusCode,
//ENUM_CFXMLParserGetTypeID,
//ENUM_CFXMLParserParse,
//ENUM_CFXMLTreeCreateFromData,
//ENUM_CFXMLTreeCreateFromDataWithError,
//ENUM_CFXMLTreeCreateWithDataFromURL,
//ENUM_CFXMLTreeCreateWithNode,
//ENUM_CFXMLTreeCreateXMLData,
//ENUM_CFXMLTreeGetNode,
//ENUM_NSBuddhistCalendar,
//ENUM_NSCalendarIdentifierBuddhist,
//ENUM_NSCalendarIdentifierChinese,
//ENUM_NSCalendarIdentifierCoptic,
//ENUM_NSCalendarIdentifierEthiopicAmeteAlem,
//ENUM_NSCalendarIdentifierEthiopicAmeteMihret,
//ENUM_NSCalendarIdentifierGregorian,
//ENUM_NSCalendarIdentifierHebrew,
//ENUM_NSCalendarIdentifierISO8601,
//ENUM_NSCalendarIdentifierIndian,
//ENUM_NSCalendarIdentifierIslamic,
//ENUM_NSCalendarIdentifierIslamicCivil,
//ENUM_NSCalendarIdentifierJapanese,
//ENUM_NSCalendarIdentifierPersian,
//ENUM_NSCalendarIdentifierRepublicOfChina,
//ENUM_NSChineseCalendar,
//ENUM_NSCurrentLocaleDidChangeNotification,
//ENUM_NSDefaultRunLoopMode,
//ENUM_NSGenericException,
//ENUM_NSGetSizeAndAlignment,
//ENUM_NSGregorianCalendar,
//ENUM_NSHebrewCalendar,
//ENUM_NSISO8601Calendar,
//ENUM_NSIndianCalendar,
//ENUM_NSInternalInconsistencyException,
//ENUM_NSInvalidArgumentException,
//ENUM_NSIslamicCalendar,
//ENUM_NSIslamicCivilCalendar,
//ENUM_NSJapaneseCalendar,
//ENUM_NSLocaleAlternateQuotationBeginDelimiterKey,
//ENUM_NSLocaleAlternateQuotationEndDelimiterKey,
//ENUM_NSLocaleCalendar,
//ENUM_NSLocaleCalendarIdentifier,
//ENUM_NSLocaleCalendarIdentifierKey,
//ENUM_NSLocaleCalendarKey,
//ENUM_NSLocaleCollationIdentifier,
//ENUM_NSLocaleCollationIdentifierKey,
//ENUM_NSLocaleCollatorIdentifier,
//ENUM_NSLocaleCollatorIdentifierKey,
//ENUM_NSLocaleCountryCode,
//ENUM_NSLocaleCountryCodeKey,
//ENUM_NSLocaleCurrencyCode,
//ENUM_NSLocaleCurrencyCodeKey,
//ENUM_NSLocaleCurrencySymbol,
//ENUM_NSLocaleCurrencySymbolKey,
//ENUM_NSLocaleDecimalSeparator,
//ENUM_NSLocaleDecimalSeparatorKey,
//ENUM_NSLocaleExemplarCharacterSet,
//ENUM_NSLocaleExemplarCharacterSetKey,
//ENUM_NSLocaleGroupingSeparator,
//ENUM_NSLocaleGroupingSeparatorKey,
//ENUM_NSLocaleIdentifier,
//ENUM_NSLocaleIdentifierKey,
//ENUM_NSLocaleLanguageCode,
//ENUM_NSLocaleLanguageCodeKey,
//ENUM_NSLocaleMeasurementSystem,
//ENUM_NSLocaleMeasurementSystemKey,
//ENUM_NSLocaleQuotationBeginDelimiterKey,
//ENUM_NSLocaleQuotationEndDelimiterKey,
//ENUM_NSLocaleScriptCode,
//ENUM_NSLocaleScriptCodeKey,
//ENUM_NSLocaleUsesMetricSystem,
//ENUM_NSLocaleUsesMetricSystemKey,
//ENUM_NSLocaleVariantCode,
//ENUM_NSLocaleVariantCodeKey,
//ENUM_NSMallocException,
//ENUM_NSPersianCalendar,
//ENUM_NSRangeException,
//ENUM_NSRepublicOfChinaCalendar,
//ENUM_NSRunLoopCommonModes,
//ENUM_NSStreamDataWrittenToMemoryStreamKey,
//ENUM_NSStreamFileCurrentOffsetKey,
//ENUM_NSSystemClockDidChangeNotification,
//ENUM_NSSystemTimeZoneDidChangeNotification,
//ENUM_OSAtomicAdd32,
//ENUM_OSAtomicAdd32Barrier,
//ENUM_OSAtomicCompareAndSwap32Barrier,
//ENUM_OSAtomicCompareAndSwapLong,
//ENUM_OSAtomicCompareAndSwapPtr,
//ENUM_OSAtomicCompareAndSwapPtrBarrier,
//ENUM_OSAtomicDecrement32,
//ENUM_OSAtomicDecrement32Barrier,
//ENUM_OSAtomicIncrement32,
//ENUM_OSAtomicIncrement32Barrier,
//ENUM__Block_copy,
//ENUM__Block_object_assign,
//ENUM__Block_object_dispose,
//ENUM__Block_release,
//ENUM__CFAllocatorAllocateGC,
//ENUM__CFAllocatorDeallocateGC,
//ENUM__CFAllocatorReallocateGC,
//ENUM__CFAppVersionCheck,
//ENUM__CFAppVersionCheckLessThan,
//ENUM__CFAppWindowsVersionCheck,
//ENUM__CFAppWindowsVersionCheckLessThan,
//ENUM__CFAppendPathComponent,
//ENUM__CFAppendPathExtension,
//ENUM__CFArrayCheckAndGetValueAtIndex,
//ENUM__CFArrayFastEnumeration,
//ENUM__CFArrayIsMutable,
//ENUM__CFArrayReplaceValues,
//ENUM__CFArraySetCapacity,
//ENUM__CFAttributedStringCheckAndReplace,
//ENUM__CFAttributedStringCheckAndReplaceAttributed,
//ENUM__CFAttributedStringCheckAndSetAttribute,
//ENUM__CFAttributedStringCheckAndSetAttributes,
//ENUM__CFAttributedStringGetLength,
//ENUM__CFAutoreleasePoolAddObject,
//ENUM__CFAutoreleasePoolPop,
//ENUM__CFAutoreleasePoolPrintPools,
//ENUM__CFAutoreleasePoolPush,
//ENUM__CFBagFastEnumeration,
//ENUM__CFBagGetKVOBit,
//ENUM__CFBagIsMutable,
//ENUM__CFBagSetCapacity,
//ENUM__CFBagSetKVOBit,
//ENUM__CFBundleCopyAllBundles,
//ENUM__CFBundleCopyBuiltInPlugInsURL,
//ENUM__CFBundleCopyBundleRegionsArray,
//ENUM__CFBundleCopyBundleURLForExecutableURL,
//ENUM__CFBundleCopyExecutableURLInDirectory,
//ENUM__CFBundleCopyFileTypeForFileData,
//ENUM__CFBundleCopyFileTypeForFileURL,
//ENUM__CFBundleCopyFrameworkURLForExecutablePath,
//ENUM__CFBundleCopyInfoPlistURL,
//ENUM__CFBundleCopyLanguageSearchListInDirectory,
//ENUM__CFBundleCopyMainBundleExecutableURL,
//ENUM__CFBundleCopyOtherExecutableURLInDirectory,
//ENUM__CFBundleCopyPrivateFrameworksURL,
//ENUM__CFBundleCopyResourceForkURL,
//ENUM__CFBundleCopyResourceURLForLanguage,
//ENUM__CFBundleCopyResourceURLsOfTypeForLanguage,
//ENUM__CFBundleCopySharedFrameworksURL,
//ENUM__CFBundleCopySharedSupportURL,
//ENUM__CFBundleCreateError,
//ENUM__CFBundleCreateIfLooksLikeBundle,
//ENUM__CFBundleCreateIfMightBeBundle,
//ENUM__CFBundleCreateWithExecutableURLIfLooksLikeBundle,
//ENUM__CFBundleCreateWithExecutableURLIfMightBeBundle,
//ENUM__CFBundleFlushBundleCaches,
//ENUM__CFBundleFlushCaches,
//ENUM__CFBundleFlushCachesForURL,
//ENUM__CFBundleGetCFMFunctionPointerForName,
//ENUM__CFBundleGetCFMFunctionPointersForNames,
//ENUM__CFBundleGetCurrentPlatform,
//ENUM__CFBundleGetExistingBundleWithBundleURL,
//ENUM__CFBundleGetHasChanged,
//ENUM__CFBundleGetLanguageAndRegionCodes,
//ENUM__CFBundleGetLanguageSearchList,
//ENUM__CFBundleGetLocalInfoDictionary,
//ENUM__CFBundleGetMainBundleIfLooksLikeBundle,
//ENUM__CFBundleGetPackageInfoInDirectory,
//ENUM__CFBundleGetStringsFilesShared,
//ENUM__CFBundleGetSupportedPlatforms,
//ENUM__CFBundleGetValueForInfoKey,
//ENUM__CFBundleLayoutVersion,
//ENUM__CFBundleLoadExecutableAndReturnError,
//ENUM__CFBundleMainBundleInfoDictionaryComesFromResourceFork,
//ENUM__CFBundleSetCFMConnectionID,
//ENUM__CFBundleSetDefaultLocalization,
//ENUM__CFBundleSetStringsFilesShared,
//ENUM__CFBundleURLLooksLikeBundle,
//ENUM__CFCalendarAddComponentsV,
//ENUM__CFCalendarComposeAbsoluteTimeV,
//ENUM__CFCalendarDecomposeAbsoluteTimeV,
//ENUM__CFCalendarGetComponentDifferenceV,
//ENUM__CFCharacterSetCreateKeyedCodingString,
//ENUM__CFCharacterSetGetKeyedCodingBuiltinType,
//ENUM__CFCharacterSetGetKeyedCodingRange,
//ENUM__CFCharacterSetGetKeyedCodingType,
//ENUM__CFCharacterSetIsInverted,
//ENUM__CFCharacterSetIsMutable,
//ENUM__CFCharacterSetSetIsInverted,
//ENUM__CFContentsOfDirectory,
//ENUM__CFCopyExtensionForAbstractType,
//ENUM__CFCopyServerVersionDictionary,
//ENUM__CFCopySystemVersionDictionary,
//ENUM__CFCreateApplicationRepositoryPath,
//ENUM__CFCreateDirectory,
//ENUM__CFCreateDirectoryWide,
//ENUM__CFDNStart,
//ENUM__CFDataFindBytes,
//ENUM__CFDeleteFile,
//ENUM__CFDictionaryFastEnumeration,
//ENUM__CFDictionaryGetKVOBit,
//ENUM__CFDictionaryIsMutable,
//ENUM__CFDictionarySetCapacity,
//ENUM__CFDictionarySetKVOBit,
//ENUM__CFDoExceptionOperation,
//ENUM__CFErrorCreateDebugDescription,
//ENUM__CFErrorCreateLocalizedDescription,
//ENUM__CFErrorCreateLocalizedFailureReason,
//ENUM__CFErrorCreateLocalizedRecoverySuggestion,
//ENUM__CFExecutableLinkedOnOrAfter,
//ENUM__CFFindBundleResources,
//ENUM__CFGetCurrentDirectory,
//ENUM__CFGetFileProperties,
//ENUM__CFGetOutOfMemoryErrorCallBack,
//ENUM__CFGetPathProperties,
//ENUM__CFGetProcessPath,
//ENUM__CFGetProgname,
//ENUM__CFGetTSD,
//ENUM__CFGetWindowsAppleAppDataDirectory,
//ENUM__CFGetWindowsAppleSystemLibraryDirectory,
//ENUM__CFGetWindowsBinaryDirectories,
//ENUM__CFHyphenationGetAllPossibleHyphenationLocations,
//ENUM__CFHyphenationGetLinguisticDataPath,
//ENUM__CFHyphenationSetLinguisticDataPath,
//ENUM__CFIsAbsolutePath,
//ENUM__CFIsDeallocating,
//ENUM__CFKeyedArchiverUIDCreate,
//ENUM__CFKeyedArchiverUIDGetTypeID,
//ENUM__CFKeyedArchiverUIDGetValue,
//ENUM__CFLengthAfterDeletingLastPathComponent,
//ENUM__CFLengthAfterDeletingPathExtension,
//ENUM__CFLocaleGetNoteCount,
//ENUM__CFLogvEx,
//ENUM__CFMainPThread,
//ENUM__CFNotificationCenterSetRunLoop,
//ENUM__CFNumberGetType2,
//ENUM__CFPreferencesAlwaysUseVolatileUserDomains,
//ENUM__CFPreferencesCreatePathToUsersManagedPrefsDirectory,
//ENUM__CFPreferencesIsManaged,
//ENUM__CFPreferencesManagementStatusChangedForDomains,
//ENUM__CFPreferencesPostValuesChangedInDomains,
//ENUM__CFPreferencesWriteDomainDictionaryToPath,
//ENUM__CFPreferencesWriteManagedDomainForUser,
//ENUM__CFProcessNameString,
//ENUM__CFProcessPath,
//ENUM__CFPropertyListCreateFromXMLData,
//ENUM__CFPropertyListCreateFromXMLString,
//ENUM__CFPropertyListCreateSingleValue,
//ENUM__CFPropertyListCreateXMLDataWithExtras,
//ENUM__CFReadBytesFromFile,
//ENUM__CFReadStreamClearEvent,
//ENUM__CFReadStreamCopyRunLoopsAndModes,
//ENUM__CFReadStreamCreateFromFileDescriptor,
//ENUM__CFReadStreamGetClient,
//ENUM__CFReadStreamGetRunLoopsAndModes,
//ENUM__CFReadStreamSignalEventDelayed,
//ENUM__CFRemoveDirectory,
//ENUM__CFRunLoop01,
//ENUM__CFRunLoopFinished,
//ENUM__CFRunLoopGet0,
//ENUM__CFRunLoopGet0b,
//ENUM__CFRunLoopGet2,
//ENUM__CFRunLoopGet2b,
//ENUM__CFRunLoopGetWindowsMessageQueueHandler,
//ENUM__CFRunLoopGetWindowsMessageQueueMask,
//ENUM__CFRunLoopGetWindowsThreadID,
//ENUM__CFRunLoopModeContainsMode,
//ENUM__CFRunLoopSetWindowsMessageQueueHandler,
//ENUM__CFRunLoopSetWindowsMessageQueueMask,
//ENUM__CFRunLoopStopMode,
//ENUM__CFRuntimeCreateInstance,
//ENUM__CFRuntimeGetClassWithTypeID,
//ENUM__CFRuntimeInitStaticInstance,
//ENUM__CFRuntimeRegisterClass,
//ENUM__CFRuntimeSetCFMPresent,
//ENUM__CFRuntimeSetInstanceTypeID,
//ENUM__CFRuntimeUnregisterClassWithTypeID,
//ENUM__CFSetFastEnumeration,
//ENUM__CFSetGetKVOBit,
//ENUM__CFSetIsMutable,
//ENUM__CFSetOutOfMemoryErrorCallBack,
//ENUM__CFSetSetCapacity,
//ENUM__CFSetSetKVOBit,
//ENUM__CFSetTSD,
//ENUM__CFSocketStreamSetAuthenticatesServerCertificateDefault,
//ENUM__CFStartOfLastPathComponent,
//ENUM__CFStartOfPathExtension,
//ENUM__CFStreamCreateWithConstantCallbacks,
//ENUM__CFStreamInstanceSize,
//ENUM__CFStreamSetInfoPointer,
//ENUM__CFStreamSourceScheduleWithAllRunLoops,
//ENUM__CFStreamSourceScheduleWithRunLoop,
//ENUM__CFStreamSourceUncheduleFromAllRunLoops,
//ENUM__CFStreamSourceUnscheduleFromRunLoop,
//ENUM__CFStringAppendFormatAndArgumentsAux,
//ENUM__CFStringCheckAndGetCharacterAtIndex,
//ENUM__CFStringCheckAndGetCharacters,
//ENUM__CFStringCreateHostName,
//ENUM__CFStringCreateWithBytesNoCopy,
//ENUM__CFStringCreateWithFormatAndArgumentsAux,
//ENUM__CFStringEncodingSetForceASCIICompatibility,
//ENUM__CFStringGetFileSystemRepresentation,
//ENUM__CFStringGetLength2,
//ENUM__CFStringHyphenationCompileHyphenatorData,
//ENUM__CFStringIsLegalURLString,
//ENUM__CFStripTrailingPathSlashes,
//ENUM__CFTimeZoneGetNoteCount,
//ENUM__CFTransmutePathSlashes,
//ENUM__CFTryRetain,
//ENUM__CFURLAlloc,
//ENUM__CFURLCopyComponents,
//ENUM__CFURLCopyPropertyListRepresentation,
//ENUM__CFURLCreateCurrentDirectoryURL,
//ENUM__CFURLCreateFromComponents,
//ENUM__CFURLCreateFromPropertyListRepresentation,
//ENUM__CFURLCreateOnlyUTF8CompatibleURLs,
//ENUM__CFURLGetEncoding,
//ENUM__CFURLGetWideFileSystemRepresentation,
//ENUM__CFURLInitFSPath,
//ENUM__CFURLInitWithString,
//ENUM__CFWriteBytesToFile,
//ENUM__CFWriteStreamCopyRunLoopsAndModes,
//ENUM__CFWriteStreamCreateFromFileDescriptor,
//ENUM__CFWriteStreamGetClient,
//ENUM__CFWriteStreamGetRunLoopsAndModes,
//ENUM__CFWriteStreamSignalEventDelayed,
//ENUM__CFXNotificationCenterCreate,
//ENUM__CFXNotificationCenterIsEmpty,
//ENUM__CFXNotificationGetHostCenter,
//ENUM__CFXNotificationGetSuspended,
//ENUM__CFXNotificationGetTaskCenter,
//ENUM__CFXNotificationPost,
//ENUM__CFXNotificationRegisterObserver,
//ENUM__CFXNotificationRemoveObservers,
//ENUM__CFXNotificationSetSuspended,
//ENUM__CFXPreferencesCopyDictionaryForApplicationPreferences,
//ENUM__CFXPreferencesCopyDictionaryForNamedVolatileSource,
//ENUM__CFXPreferencesCopyDictionaryForSourceWithBundleID,
//ENUM__CFXPreferencesCopyLanguageList,
//ENUM__CFXPreferencesCopyVolatileSourceNames,
//ENUM__CFXPreferencesGetByHostIdentifierString,
//ENUM__CFXPreferencesRegisterDefaultValues,
//ENUM__CFXPreferencesRemoveNamedVolatileSource,
//ENUM__CFXPreferencesReplaceValuesInNamedVolatileSource,
//ENUM__CFXPreferencesReplaceValuesInSourceWithBundleID,
//ENUM__CFXPreferencesSetUserDefaultsPointer,
//ENUM__CF_forwarding_prep_0,
//ENUM__NSConcreteGlobalBlock,
//ENUM__NSConcreteStackBlock,
//ENUM__NSMessageBuilder,
//ENUM__NS_access,
//ENUM__NS_chdir,
//ENUM__NS_chmod,
//ENUM__NS_getcwd,
//ENUM__NS_getenv,
//ENUM__NS_mkdir,
//ENUM__NS_mkstemp,
//ENUM__NS_open,
//ENUM__NS_pthread_main_np,
//ENUM__NS_pthread_setname_np,
//ENUM__NS_rename,
//ENUM__NS_rmdir,
//ENUM__NS_stat,
//ENUM__NS_unlink,
//ENUM___CFArgStuff,
//ENUM___CFAttributedStringMtbl,
//ENUM___CFBinaryPlistCreateObject,
//ENUM___CFBinaryPlistCreateObject2,
//ENUM___CFBinaryPlistGetOffsetForValueFromArray2,
//ENUM___CFBinaryPlistGetOffsetForValueFromDictionary2,
//ENUM___CFBinaryPlistGetOffsetForValueFromDictionary3,
//ENUM___CFBinaryPlistGetTopLevelInfo,
//ENUM___CFBinaryPlistWrite,
//ENUM___CFBinaryPlistWriteToStream,
//ENUM___CFBinaryPlistWriteToStreamWithEstimate,
//ENUM___CFBinaryPlistWriteToStreamWithOptions,
//ENUM___CFCharToUniCharFunc,
//ENUM___CFConstantStringClassReference,
//ENUM___CFConstantStringClassReferencePtr,
//ENUM___CFDataMtbl,
//ENUM___CFDefaultEightBitStringEncoding,
//ENUM___CFDoExternRefOperation,
//ENUM___CFGetNextSearchPathEnumeration,
//ENUM___CFInitialize,
//ENUM___CFOASafe,
//ENUM___CFPreferencesSetPathHackEnabled,
ENUM___CFRangeMake,
//ENUM___CFSetLastAllocationEventName,
//ENUM___CFSocketInitializeWinSock,
//ENUM___CFStartSearchPathEnumeration,
//ENUM___CFStorageGetCapacity,
//ENUM___CFStorageGetValueSize,
//ENUM___CFStorageSetAlwaysFrozen,
//ENUM___CFStringAppendBytes,
//ENUM___CFStringCheckAndReplace,
//ENUM___CFStringComputeEightBitStringEncoding,
//ENUM___CFStringCreateImmutableFunnel2,
//ENUM___CFStringDecodeByteStream2,
//ENUM___CFStringDecodeByteStream3,
//ENUM___CFStringEncodeByteStream,
//ENUM___CFStringHash,
//ENUM___CFStringInitializeSystemEncoding,
//ENUM___CFStringIsEightBit,
ENUM___CFStringMakeConstantString,
//ENUM___CFStringMtbl,
//ENUM___CFStringNoteErrors,
//ENUM___CFURLReservedPtr,
//ENUM___CFURLResourceInfoPtr,
//ENUM___CFURLSetReservedPtr,
//ENUM___CFURLSetResourceInfoPtr,
//ENUM__kCFBundleAllowMixedLocalizationsKey,
//ENUM__kCFBundleDisplayNameKey,
//ENUM__kCFBundleDocumentTypesKey,
//ENUM__kCFBundleExecutablePathKey,
//ENUM__kCFBundleGetInfoHTMLKey,
//ENUM__kCFBundleGetInfoStringKey,
//ENUM__kCFBundleIconFileKey,
//ENUM__kCFBundleInfoPlistURLKey,
//ENUM__kCFBundleInitialPathKey,
//ENUM__kCFBundleNumericVersionKey,
//ENUM__kCFBundleOldDocumentTypesKey,
//ENUM__kCFBundleOldExecutableKey,
//ENUM__kCFBundleOldIconFileKey,
//ENUM__kCFBundleOldInfoDictionaryVersionKey,
//ENUM__kCFBundleOldNameKey,
//ENUM__kCFBundleOldShortVersionStringKey,
//ENUM__kCFBundleOldTypeExtensions1Key,
//ENUM__kCFBundleOldTypeExtensions2Key,
//ENUM__kCFBundleOldTypeIconFileKey,
//ENUM__kCFBundleOldTypeNameKey,
//ENUM__kCFBundleOldTypeOSTypesKey,
//ENUM__kCFBundleOldTypeRoleKey,
//ENUM__kCFBundlePackageTypeKey,
//ENUM__kCFBundlePrincipalClassKey,
//ENUM__kCFBundleRawInfoPlistURLKey,
//ENUM__kCFBundleResolvedPathKey,
//ENUM__kCFBundleResourceSpecificationKey,
//ENUM__kCFBundleResourcesFileMappedKey,
//ENUM__kCFBundleShortVersionStringKey,
//ENUM__kCFBundleSignatureKey,
//ENUM__kCFBundleSupportedPlatformsKey,
//ENUM__kCFBundleTypeExtensionsKey,
//ENUM__kCFBundleTypeIconFileKey,
//ENUM__kCFBundleTypeMIMETypesKey,
//ENUM__kCFBundleTypeNameKey,
//ENUM__kCFBundleTypeOSTypesKey,
//ENUM__kCFBundleTypeRoleKey,
//ENUM__kCFBundleURLIconFileKey,
//ENUM__kCFBundleURLNameKey,
//ENUM__kCFBundleURLSchemesKey,
//ENUM__kCFBundleURLTypesKey,
//ENUM__kCFSystemVersionBuildStringKey,
//ENUM__kCFSystemVersionBuildVersionKey,
//ENUM__kCFSystemVersionProductCopyrightKey,
//ENUM__kCFSystemVersionProductNameKey,
//ENUM__kCFSystemVersionProductUserVisibleVersionKey,
//ENUM__kCFSystemVersionProductVersionExtraKey,
//ENUM__kCFSystemVersionProductVersionKey,
//ENUM__kCFSystemVersionProductVersionStringKey,
//ENUM_kCFAbsoluteTimeIntervalSince1904,
//ENUM_kCFAbsoluteTimeIntervalSince1970,
//ENUM_kCFAllocatorDefault,
//ENUM_kCFAllocatorMalloc,
//ENUM_kCFAllocatorMallocZone,
//ENUM_kCFAllocatorNull,
//ENUM_kCFAllocatorSystemDefault,
//ENUM_kCFAllocatorUseContext,
ENUM_kCFBooleanFalse,
ENUM_kCFBooleanTrue,
//ENUM_kCFBuddhistCalendar,
//ENUM_kCFBundleDevelopmentRegionKey,
//ENUM_kCFBundleExecutableKey,
//ENUM_kCFBundleIdentifierKey,
//ENUM_kCFBundleInfoDictionaryVersionKey,
//ENUM_kCFBundleLocalizationsKey,
//ENUM_kCFBundleNameKey,
//ENUM_kCFBundleVersionKey,
//ENUM_kCFCalendarIdentifierBuddhist,
//ENUM_kCFCalendarIdentifierChinese,
//ENUM_kCFCalendarIdentifierCoptic,
//ENUM_kCFCalendarIdentifierEthiopicAmeteAlem,
//ENUM_kCFCalendarIdentifierEthiopicAmeteMihret,
//ENUM_kCFCalendarIdentifierGregorian,
//ENUM_kCFCalendarIdentifierHebrew,
//ENUM_kCFCalendarIdentifierISO8601,
//ENUM_kCFCalendarIdentifierIndian,
//ENUM_kCFCalendarIdentifierIslamic,
//ENUM_kCFCalendarIdentifierIslamicCivil,
//ENUM_kCFCalendarIdentifierJapanese,
//ENUM_kCFCalendarIdentifierPersian,
//ENUM_kCFCalendarIdentifierRepublicOfChina,
//ENUM_kCFChineseCalendar,
//ENUM_kCFCopyStringBagCallBacks,
//ENUM_kCFCopyStringDictionaryKeyCallBacks,
//ENUM_kCFCopyStringSetCallBacks,
//ENUM_kCFDateFormatterAMSymbol,
//ENUM_kCFDateFormatterAMSymbolKey,
//ENUM_kCFDateFormatterCalendar,
//ENUM_kCFDateFormatterCalendarIdentifier,
//ENUM_kCFDateFormatterCalendarIdentifierKey,
//ENUM_kCFDateFormatterCalendarKey,
//ENUM_kCFDateFormatterCalendarName,
//ENUM_kCFDateFormatterDefaultDate,
//ENUM_kCFDateFormatterDefaultDateKey,
//ENUM_kCFDateFormatterDefaultFormat,
//ENUM_kCFDateFormatterDefaultFormatKey,
//ENUM_kCFDateFormatterDoesRelativeDateFormattingKey,
//ENUM_kCFDateFormatterEraSymbols,
//ENUM_kCFDateFormatterEraSymbolsKey,
//ENUM_kCFDateFormatterGregorianStartDate,
//ENUM_kCFDateFormatterGregorianStartDateKey,
//ENUM_kCFDateFormatterIsLenient,
//ENUM_kCFDateFormatterIsLenientKey,
//ENUM_kCFDateFormatterLongEraSymbols,
//ENUM_kCFDateFormatterLongEraSymbolsKey,
//ENUM_kCFDateFormatterMonthSymbols,
//ENUM_kCFDateFormatterMonthSymbolsKey,
//ENUM_kCFDateFormatterPMSymbol,
//ENUM_kCFDateFormatterPMSymbolKey,
//ENUM_kCFDateFormatterQuarterSymbols,
//ENUM_kCFDateFormatterQuarterSymbolsKey,
//ENUM_kCFDateFormatterShortMonthSymbols,
//ENUM_kCFDateFormatterShortMonthSymbolsKey,
//ENUM_kCFDateFormatterShortQuarterSymbols,
//ENUM_kCFDateFormatterShortQuarterSymbolsKey,
//ENUM_kCFDateFormatterShortStandaloneMonthSymbols,
//ENUM_kCFDateFormatterShortStandaloneMonthSymbolsKey,
//ENUM_kCFDateFormatterShortStandaloneQuarterSymbols,
//ENUM_kCFDateFormatterShortStandaloneQuarterSymbolsKey,
//ENUM_kCFDateFormatterShortStandaloneWeekdaySymbols,
//ENUM_kCFDateFormatterShortStandaloneWeekdaySymbolsKey,
//ENUM_kCFDateFormatterShortWeekdaySymbols,
//ENUM_kCFDateFormatterShortWeekdaySymbolsKey,
//ENUM_kCFDateFormatterStandaloneMonthSymbols,
//ENUM_kCFDateFormatterStandaloneMonthSymbolsKey,
//ENUM_kCFDateFormatterStandaloneQuarterSymbols,
//ENUM_kCFDateFormatterStandaloneQuarterSymbolsKey,
//ENUM_kCFDateFormatterStandaloneWeekdaySymbols,
//ENUM_kCFDateFormatterStandaloneWeekdaySymbolsKey,
//ENUM_kCFDateFormatterTimeZone,
//ENUM_kCFDateFormatterTimeZoneKey,
//ENUM_kCFDateFormatterTwoDigitStartDate,
//ENUM_kCFDateFormatterTwoDigitStartDateKey,
//ENUM_kCFDateFormatterVeryShortMonthSymbols,
//ENUM_kCFDateFormatterVeryShortMonthSymbolsKey,
//ENUM_kCFDateFormatterVeryShortStandaloneMonthSymbols,
//ENUM_kCFDateFormatterVeryShortStandaloneMonthSymbolsKey,
//ENUM_kCFDateFormatterVeryShortStandaloneWeekdaySymbols,
//ENUM_kCFDateFormatterVeryShortStandaloneWeekdaySymbolsKey,
//ENUM_kCFDateFormatterVeryShortWeekdaySymbols,
//ENUM_kCFDateFormatterVeryShortWeekdaySymbolsKey,
//ENUM_kCFDateFormatterWeekdaySymbols,
//ENUM_kCFDateFormatterWeekdaySymbolsKey,
//ENUM_kCFErrorDebugDescriptionKey,
//ENUM_kCFErrorDescriptionKey,
//ENUM_kCFErrorDomainCocoa,
//ENUM_kCFErrorDomainCoreFoundation,
//ENUM_kCFErrorDomainMach,
//ENUM_kCFErrorDomainOSStatus,
//ENUM_kCFErrorDomainPOSIX,
//ENUM_kCFErrorFilePathKey,
//ENUM_kCFErrorLocalizedDescriptionKey,
//ENUM_kCFErrorLocalizedFailureReasonKey,
//ENUM_kCFErrorLocalizedRecoverySuggestionKey,
//ENUM_kCFErrorURLKey,
//ENUM_kCFErrorUnderlyingErrorKey,
//ENUM_kCFFileURLDirectoryContents,
//ENUM_kCFFileURLExists,
//ENUM_kCFFileURLLastModificationTime,
//ENUM_kCFFileURLPOSIXMode,
//ENUM_kCFFileURLSize,
//ENUM_kCFGregorianCalendar,
//ENUM_kCFHTTPURLStatusCode,
//ENUM_kCFHTTPURLStatusLine,
//ENUM_kCFHebrewCalendar,
//ENUM_kCFISO8601Calendar,
//ENUM_kCFIndianCalendar,
//ENUM_kCFIslamicCalendar,
//ENUM_kCFIslamicCivilCalendar,
//ENUM_kCFJapaneseCalendar,
//ENUM_kCFLocaleAlternateQuotationBeginDelimiterKey,
//ENUM_kCFLocaleAlternateQuotationEndDelimiterKey,
//ENUM_kCFLocaleCalendar,
//ENUM_kCFLocaleCalendarIdentifier,
//ENUM_kCFLocaleCalendarIdentifierKey,
//ENUM_kCFLocaleCalendarKey,
//ENUM_kCFLocaleCollationIdentifier,
//ENUM_kCFLocaleCollationIdentifierKey,
//ENUM_kCFLocaleCollatorIdentifier,
//ENUM_kCFLocaleCollatorIdentifierKey,
//ENUM_kCFLocaleCountryCode,
//ENUM_kCFLocaleCountryCodeKey,
//ENUM_kCFLocaleCurrencyCode,
//ENUM_kCFLocaleCurrencyCodeKey,
//ENUM_kCFLocaleCurrencySymbol,
//ENUM_kCFLocaleCurrencySymbolKey,
//ENUM_kCFLocaleCurrentLocaleDidChangeNotification,
//ENUM_kCFLocaleDecimalSeparator,
//ENUM_kCFLocaleDecimalSeparatorKey,
//ENUM_kCFLocaleExemplarCharacterSet,
//ENUM_kCFLocaleExemplarCharacterSetKey,
//ENUM_kCFLocaleGroupingSeparator,
//ENUM_kCFLocaleGroupingSeparatorKey,
//ENUM_kCFLocaleIdentifier,
//ENUM_kCFLocaleIdentifierKey,
//ENUM_kCFLocaleLanguageCode,
//ENUM_kCFLocaleLanguageCodeKey,
//ENUM_kCFLocaleMeasurementSystem,
//ENUM_kCFLocaleMeasurementSystemKey,
//ENUM_kCFLocaleQuotationBeginDelimiterKey,
//ENUM_kCFLocaleQuotationEndDelimiterKey,
//ENUM_kCFLocaleScriptCode,
//ENUM_kCFLocaleScriptCodeKey,
//ENUM_kCFLocaleUsesMetricSystem,
//ENUM_kCFLocaleUsesMetricSystemKey,
//ENUM_kCFLocaleVariantCode,
//ENUM_kCFLocaleVariantCodeKey,
//ENUM_kCFManagedPreferencesMCXNotificationName,
//ENUM_kCFManagedPreferencesMCXObjectName,
//ENUM_kCFNotificationAnyName,
//ENUM_kCFNotificationAnyObject,
//ENUM_kCFNotificationAnyObserver,
//ENUM_kCFNull,
//ENUM_kCFNumberFormatterAlwaysShowDecimalSeparator,
//ENUM_kCFNumberFormatterAlwaysShowDecimalSeparatorKey,
//ENUM_kCFNumberFormatterCurrencyCode,
//ENUM_kCFNumberFormatterCurrencyCodeKey,
//ENUM_kCFNumberFormatterCurrencyDecimalSeparator,
//ENUM_kCFNumberFormatterCurrencyDecimalSeparatorKey,
//ENUM_kCFNumberFormatterCurrencyGroupingSeparator,
//ENUM_kCFNumberFormatterCurrencyGroupingSeparatorKey,
//ENUM_kCFNumberFormatterCurrencySymbol,
//ENUM_kCFNumberFormatterCurrencySymbolKey,
//ENUM_kCFNumberFormatterDecimalSeparator,
//ENUM_kCFNumberFormatterDecimalSeparatorKey,
//ENUM_kCFNumberFormatterDefaultFormat,
//ENUM_kCFNumberFormatterDefaultFormatKey,
//ENUM_kCFNumberFormatterExponentSymbol,
//ENUM_kCFNumberFormatterExponentSymbolKey,
//ENUM_kCFNumberFormatterFormatWidth,
//ENUM_kCFNumberFormatterFormatWidthKey,
//ENUM_kCFNumberFormatterGroupingSeparator,
//ENUM_kCFNumberFormatterGroupingSeparatorKey,
//ENUM_kCFNumberFormatterGroupingSize,
//ENUM_kCFNumberFormatterGroupingSizeKey,
//ENUM_kCFNumberFormatterInfinitySymbol,
//ENUM_kCFNumberFormatterInfinitySymbolKey,
//ENUM_kCFNumberFormatterInternationalCurrencySymbol,
//ENUM_kCFNumberFormatterInternationalCurrencySymbolKey,
//ENUM_kCFNumberFormatterIsLenient,
//ENUM_kCFNumberFormatterIsLenientKey,
//ENUM_kCFNumberFormatterMaxFractionDigits,
//ENUM_kCFNumberFormatterMaxFractionDigitsKey,
//ENUM_kCFNumberFormatterMaxIntegerDigits,
//ENUM_kCFNumberFormatterMaxIntegerDigitsKey,
//ENUM_kCFNumberFormatterMaxSignificantDigits,
//ENUM_kCFNumberFormatterMaxSignificantDigitsKey,
//ENUM_kCFNumberFormatterMinFractionDigits,
//ENUM_kCFNumberFormatterMinFractionDigitsKey,
//ENUM_kCFNumberFormatterMinIntegerDigits,
//ENUM_kCFNumberFormatterMinIntegerDigitsKey,
//ENUM_kCFNumberFormatterMinSignificantDigits,
//ENUM_kCFNumberFormatterMinSignificantDigitsKey,
//ENUM_kCFNumberFormatterMinusSign,
//ENUM_kCFNumberFormatterMinusSignKey,
//ENUM_kCFNumberFormatterMultiplier,
//ENUM_kCFNumberFormatterMultiplierKey,
//ENUM_kCFNumberFormatterNaNSymbol,
//ENUM_kCFNumberFormatterNaNSymbolKey,
//ENUM_kCFNumberFormatterNegativePrefix,
//ENUM_kCFNumberFormatterNegativePrefixKey,
//ENUM_kCFNumberFormatterNegativeSuffix,
//ENUM_kCFNumberFormatterNegativeSuffixKey,
//ENUM_kCFNumberFormatterPaddingCharacter,
//ENUM_kCFNumberFormatterPaddingCharacterKey,
//ENUM_kCFNumberFormatterPaddingPosition,
//ENUM_kCFNumberFormatterPaddingPositionKey,
//ENUM_kCFNumberFormatterPerMillSymbol,
//ENUM_kCFNumberFormatterPerMillSymbolKey,
//ENUM_kCFNumberFormatterPercentSymbol,
//ENUM_kCFNumberFormatterPercentSymbolKey,
//ENUM_kCFNumberFormatterPlusSign,
//ENUM_kCFNumberFormatterPlusSignKey,
//ENUM_kCFNumberFormatterPositivePrefix,
//ENUM_kCFNumberFormatterPositivePrefixKey,
//ENUM_kCFNumberFormatterPositiveSuffix,
//ENUM_kCFNumberFormatterPositiveSuffixKey,
//ENUM_kCFNumberFormatterRoundingIncrement,
//ENUM_kCFNumberFormatterRoundingIncrementKey,
//ENUM_kCFNumberFormatterRoundingMode,
//ENUM_kCFNumberFormatterRoundingModeKey,
//ENUM_kCFNumberFormatterSecondaryGroupingSize,
//ENUM_kCFNumberFormatterSecondaryGroupingSizeKey,
//ENUM_kCFNumberFormatterUseGroupingSeparator,
//ENUM_kCFNumberFormatterUseGroupingSeparatorKey,
//ENUM_kCFNumberFormatterUseSignificantDigits,
//ENUM_kCFNumberFormatterUseSignificantDigitsKey,
//ENUM_kCFNumberFormatterZeroSymbol,
//ENUM_kCFNumberFormatterZeroSymbolKey,
//ENUM_kCFNumberNaN,
//ENUM_kCFNumberNegativeInfinity,
//ENUM_kCFNumberPositiveInfinity,
//ENUM_kCFPersianCalendar,
//ENUM_kCFPlugInDynamicRegisterFunctionKey,
//ENUM_kCFPlugInDynamicRegistrationKey,
//ENUM_kCFPlugInFactoriesKey,
//ENUM_kCFPlugInTypesKey,
//ENUM_kCFPlugInUnloadFunctionKey,
//ENUM_kCFPreferencesAnyApplication,
//ENUM_kCFPreferencesAnyHost,
//ENUM_kCFPreferencesAnyUser,
//ENUM_kCFPreferencesCurrentApplication,
//ENUM_kCFPreferencesCurrentHost,
//ENUM_kCFPreferencesCurrentUser,
//ENUM_kCFPreferencesDidChangeNotificationName,
//ENUM_kCFPreferencesDidChangeObject,
//ENUM_kCFRepublicOfChinaCalendar,
//ENUM_kCFRunLoopCommonModes,
//ENUM_kCFRunLoopDefaultMode,
//ENUM_kCFSocketCommandKey,
//ENUM_kCFSocketErrorKey,
//ENUM_kCFSocketNameKey,
//ENUM_kCFSocketRegisterCommand,
//ENUM_kCFSocketResultKey,
//ENUM_kCFSocketRetrieveCommand,
//ENUM_kCFSocketValueKey,
//ENUM_kCFStreamPropertyAppendToFile,
ENUM_kCFStreamPropertyDataWritten,
//ENUM_kCFStreamPropertyFileCurrentOffset,
//ENUM_kCFStreamPropertySocketNativeHandle,
//ENUM_kCFStreamPropertySocketRemoteHostName,
//ENUM_kCFStreamPropertySocketRemotePortNumber,
//ENUM_kCFStringBinaryHeapCallBacks,
//ENUM_kCFStringFormatLocaleKey,
//ENUM_kCFStringFormatSpecTypeKey,
//ENUM_kCFStringFormatValueTypeKey,
//ENUM_kCFStringGenderRuleType,
//ENUM_kCFStringLocalizedFormatKey,
//ENUM_kCFStringPluralRuleType,
//ENUM_kCFStringTransformFullwidthHalfwidth,
//ENUM_kCFStringTransformHiraganaKatakana,
//ENUM_kCFStringTransformLatinArabic,
//ENUM_kCFStringTransformLatinCyrillic,
//ENUM_kCFStringTransformLatinGreek,
//ENUM_kCFStringTransformLatinHangul,
//ENUM_kCFStringTransformLatinHebrew,
//ENUM_kCFStringTransformLatinHiragana,
//ENUM_kCFStringTransformLatinKatakana,
//ENUM_kCFStringTransformLatinThai,
//ENUM_kCFStringTransformMandarinLatin,
//ENUM_kCFStringTransformStripCombiningMarks,
//ENUM_kCFStringTransformStripDiacritics,
//ENUM_kCFStringTransformToLatin,
//ENUM_kCFStringTransformToUnicodeName,
//ENUM_kCFStringTransformToXMLHex,
//ENUM_kCFTimeZoneSystemTimeZoneDidChangeNotification,
ENUM_kCFTypeArrayCallBacks,
//ENUM_kCFTypeBagCallBacks,
ENUM_kCFTypeDictionaryKeyCallBacks,
ENUM_kCFTypeDictionaryValueCallBacks,
//ENUM_kCFTypeSetCallBacks,
//ENUM_kCFURLFileDirectoryContents,
//ENUM_kCFURLFileExists,
//ENUM_kCFURLFileLastModificationTime,
//ENUM_kCFURLFileLength,
//ENUM_kCFURLFileOwnerID,
//ENUM_kCFURLFilePOSIXMode,
//ENUM_kCFURLHTTPStatusCode,
//ENUM_kCFURLHTTPStatusLine,
//ENUM_kCFUseCollectableAllocator,
//ENUM_kCFXMLTreeErrorDescription,
//ENUM_kCFXMLTreeErrorLineNumber,
//ENUM_kCFXMLTreeErrorLocation,
//ENUM_kCFXMLTreeErrorStatusCode,

	ENUM_COREFOUNDATION_DLL_NUM
};

extern FARPROC __gpf_CoreFoundation_dll_ri[ENUM_COREFOUNDATION_DLL_NUM];

//#define CoreF_CFAbsoluteTimeAddGregorianUnits									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAbsoluteTimeAddGregorianUnits				]))
//#define CoreF_CFAbsoluteTimeGetCurrent										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAbsoluteTimeGetCurrent					]))
//#define CoreF_CFAbsoluteTimeGetDayOfWeek										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAbsoluteTimeGetDayOfWeek					]))
//#define CoreF_CFAbsoluteTimeGetDayOfYear										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAbsoluteTimeGetDayOfYear					]))
//#define CoreF_CFAbsoluteTimeGetDifferenceAsGregorianUnits						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAbsoluteTimeGetDifferenceAsGregorianUnits	]))
//#define CoreF_CFAbsoluteTimeGetGregorianDate									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAbsoluteTimeGetGregorianDate				]))
//#define CoreF_CFAbsoluteTimeGetWeekOfYear										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAbsoluteTimeGetWeekOfYear					]))
//#define CoreF_CFAllocatorAllocate												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAllocatorAllocate							]))
//#define CoreF_CFAllocatorCreate												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAllocatorCreate							]))
//#define CoreF_CFAllocatorDeallocate											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAllocatorDeallocate						]))
//#define CoreF_CFAllocatorGetContext											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAllocatorGetContext						]))
//#define CoreF_CFAllocatorGetDefault											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAllocatorGetDefault						]))
//#define CoreF_CFAllocatorGetPreferredSizeForSize								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAllocatorGetPreferredSizeForSize			]))
//#define CoreF_CFAllocatorGetTypeID											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAllocatorGetTypeID						]))
//#define CoreF_CFAllocatorReallocate											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAllocatorReallocate						]))
//#define CoreF_CFAllocatorSetDefault											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAllocatorSetDefault						]))
//#define CoreF_CFArrayAppendArray												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayAppendArray							]))
#define CFArrayAppendValue													((PF_CFArrayAppendValue)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayAppendValue				]))
//#define CoreF_CFArrayApplyFunction											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayApplyFunction				]))
//#define CoreF_CFArrayBSearchValues											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayBSearchValues			]))
//#define CoreF_CFArrayContainsValue											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayContainsValue			]))
//#define CoreF_CFArrayCreate													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayCreate			]))
#define CFArrayCreateCopy													((PF_CFArrayCreateCopy)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayCreateCopy					]))
#define CFArrayCreateMutable												((PF_CFArrayCreateMutable)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayCreateMutable				]))
//#define CoreF_CFArrayCreateMutableCopy										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayCreateMutableCopy			]))
//#define CoreF_CFArrayExchangeValuesAtIndices									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayExchangeValuesAtIndices		]))
#define CFArrayGetCount														((PF_CFArrayGetCount)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayGetCount	]))
//#define CoreF_CFArrayGetCountOfValue											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayGetCountOfValue					]))
//#define CoreF_CFArrayGetFirstIndexOfValue										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayGetFirstIndexOfValue			]))
//#define CoreF_CFArrayGetLastIndexOfValue										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayGetLastIndexOfValue		]))
//#define CoreF_CFArrayGetTypeID												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayGetTypeID		]))
#define CFArrayGetValueAtIndex												((PF_CFArrayGetValueAtIndex)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayGetValueAtIndex				]))
//#define CoreF_CFArrayGetValues												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayGetValues			]))
//#define CoreF_CFArrayInsertValueAtIndex										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayInsertValueAtIndex				]))
//#define CoreF_CFArrayRemoveAllValues											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayRemoveAllValues		]))
//#define CoreF_CFArrayRemoveValueAtIndex										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayRemoveValueAtIndex			]))
//#define CoreF_CFArrayReplaceValues											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArrayReplaceValues		]))
//#define CoreF_CFArraySetValueAtIndex											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArraySetValueAtIndex			]))
//#define CoreF_CFArraySortValues												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFArraySortValues										]))
//#define CoreF_CFAttributedStringBeginEditing									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringBeginEditing							]))
//#define CoreF_CFAttributedStringCreate										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringCreate								]))
//#define CoreF_CFAttributedStringCreateCopy									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringCreateCopy							]))
//#define CoreF_CFAttributedStringCreateMutable									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringCreateMutable							]))
//#define CoreF_CFAttributedStringCreateMutableCopy								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringCreateMutableCopy						]))
//#define CoreF_CFAttributedStringCreateWithSubstring							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringCreateWithSubstring					]))
//#define CoreF_CFAttributedStringEndEditing									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringEndEditing							]))
//#define CoreF_CFAttributedStringGetAttribute									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringGetAttribute							]))
//#define CoreF_CFAttributedStringGetAttributeAndLongestEffectiveRange			((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringGetAttributeAndLongestEffectiveRange	]))
//#define CoreF_CFAttributedStringGetAttributes									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringGetAttributes							]))
//#define CoreF_CFAttributedStringGetAttributesAndLongestEffectiveRange			((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringGetAttributesAndLongestEffectiveRange	]))
//#define CoreF_CFAttributedStringGetLength										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringGetLength								]))
//#define CoreF_CFAttributedStringGetMutableString								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringGetMutableString						]))
//#define CoreF_CFAttributedStringGetString										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringGetString								]))
//#define CoreF_CFAttributedStringGetTypeID										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringGetTypeID								]))
//#define CoreF_CFAttributedStringOpenUText										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringOpenUText								]))
//#define CoreF_CFAttributedStringRemoveAttribute								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringRemoveAttribute						]))
//#define CoreF_CFAttributedStringReplaceAttributedString						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringReplaceAttributedString				]))
//#define CoreF_CFAttributedStringReplaceString									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringReplaceString							]))
//#define CoreF_CFAttributedStringSetAttribute									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringSetAttribute							]))
//#define CoreF_CFAttributedStringSetAttributes									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFAttributedStringSetAttributes							]))
//#define CoreF_CFBSearch														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBSearch					]))
//#define CoreF_CFBagAddValue													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagAddValue				]))
//#define CoreF_CFBagApplyFunction												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagApplyFunction			]))
//#define CoreF_CFBagContainsValue												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagContainsValue			]))
//#define CoreF_CFBagCreate														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagCreate					]))
//#define CoreF_CFBagCreateCopy													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagCreateCopy				]))
//#define CoreF_CFBagCreateMutable												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagCreateMutable			]))
//#define CoreF_CFBagCreateMutableCopy											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagCreateMutableCopy		]))
//#define CoreF_CFBagGetCount													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagGetCount				]))
//#define CoreF_CFBagGetCountOfValue											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagGetCountOfValue		]))
//#define CoreF_CFBagGetTypeID													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagGetTypeID				]))
//#define CoreF_CFBagGetValue													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagGetValue				]))
//#define CoreF_CFBagGetValueIfPresent											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagGetValueIfPresent		]))
//#define CoreF_CFBagGetValues													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagGetValues				]))
//#define CoreF_CFBagRemoveAllValues											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagRemoveAllValues		]))
//#define CoreF_CFBagRemoveValue												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagRemoveValue			]))
//#define CoreF_CFBagReplaceValue												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagReplaceValue			]))
//#define CoreF_CFBagSetValue													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBagSetValue				]))
//#define CoreF_CFBinaryHeapAddValue											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBinaryHeapAddValue		]))
//#define CoreF_CFBinaryHeapApplyFunction										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBinaryHeapApplyFunction	]))
//#define CoreF_CFBinaryHeapContainsValue										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBinaryHeapContainsValue	]))
//#define CoreF_CFBinaryHeapCreate												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBinaryHeapCreate			]))
//#define CoreF_CFBinaryHeapCreateCopy											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBinaryHeapCreateCopy		]))
//#define CoreF_CFBinaryHeapGetCount											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBinaryHeapGetCount			]))		
//#define CoreF_CFBinaryHeapGetCountOfValue										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBinaryHeapGetCountOfValue		]))
//#define CoreF_CFBinaryHeapGetMinimum											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBinaryHeapGetMinimum			]))
//#define CoreF_CFBinaryHeapGetMinimumIfPresent									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBinaryHeapGetMinimumIfPresent	]))
//#define CoreF_CFBinaryHeapGetTypeID											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBinaryHeapGetTypeID			]))
//#define CoreF_CFBinaryHeapGetValues											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBinaryHeapGetValues			]))
//#define CoreF_CFBinaryHeapRemoveAllValues										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBinaryHeapRemoveAllValues		]))
//#define CoreF_CFBinaryHeapRemoveMinimumValue									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBinaryHeapRemoveMinimumValue	]))
//#define CoreF_CFBitVectorContainsBit											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorContainsBit			]))
//#define CoreF_CFBitVectorCreate												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorCreate				]))
//#define CoreF_CFBitVectorCreateCopy											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorCreateCopy			]))
//#define CoreF_CFBitVectorCreateMutable										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorCreateMutable		]))
//#define CoreF_CFBitVectorCreateMutableCopy									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorCreateMutableCopy	]))
//#define CoreF_CFBitVectorFlipBitAtIndex										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorFlipBitAtIndex		]))
//#define CoreF_CFBitVectorFlipBits												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorFlipBits				]))
//#define CoreF_CFBitVectorGetBitAtIndex										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorGetBitAtIndex		]))
//#define CoreF_CFBitVectorGetBits												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorGetBits				]))
//#define CoreF_CFBitVectorGetCount												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorGetCount				]))
//#define CoreF_CFBitVectorGetCountOfBit										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorGetCountOfBit		]))
//#define CoreF_CFBitVectorGetFirstIndexOfBit									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorGetFirstIndexOfBit	]))
//#define CoreF_CFBitVectorGetLastIndexOfBit									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorGetLastIndexOfBit	]))
//#define CoreF_CFBitVectorGetTypeID											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorGetTypeID						]))
//#define CoreF_CFBitVectorSetAllBits											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorSetAllBits						]))
//#define CoreF_CFBitVectorSetBitAtIndex										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorSetBitAtIndex					]))
//#define CoreF_CFBitVectorSetBits												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorSetBits							]))
//#define CoreF_CFBitVectorSetCount												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBitVectorSetCount							]))
//#define CoreF_CFBooleanGetTypeID												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBooleanGetTypeID							]))
#define CFBooleanGetValue													((PF_CFBooleanGetValue)(__gpf_CoreFoundation_dll_ri[ENUM_CFBooleanGetValue							]))
//#define CoreF_CFBundleCopyAuxiliaryExecutableURL								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyAuxiliaryExecutableURL			]))
//#define CoreF_CFBundleCopyBuiltInPlugInsURL									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyBuiltInPlugInsURL				]))
//#define CoreF_CFBundleCopyBundleLocalizations									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyBundleLocalizations				]))
//#define CoreF_CFBundleCopyBundleURL											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyBundleURL						]))
//#define CoreF_CFBundleCopyExecutableArchitectures								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyExecutableArchitectures			]))
//#define CoreF_CFBundleCopyExecutableArchitecturesForURL						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyExecutableArchitecturesForURL	]))
//#define CoreF_CFBundleCopyExecutableURL										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyExecutableURL					]))
//#define CoreF_CFBundleCopyInfoDictionaryForURL								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyInfoDictionaryForURL			]))
//#define CoreF_CFBundleCopyInfoDictionaryInDirectory							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyInfoDictionaryInDirectory		]))
//#define CoreF_CFBundleCopyLocalizationForLocalizationInfo						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyLocalizationForLocalizationInfo	]))
//#define CoreF_CFBundleCopyLocalizationsForPreferences							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyLocalizationsForPreferences		]))
//#define CoreF_CFBundleCopyLocalizationsForURL									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyLocalizationsForURL				]))
//#define CoreF_CFBundleCopyLocalizedString										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyLocalizedString					]))
//#define CoreF_CFBundleCopyPreferredLocalizationsFromArray						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyPreferredLocalizationsFromArray	]))
//#define CoreF_CFBundleCopyPrivateFrameworksURL								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyPrivateFrameworksURL			]))
//#define CoreF_CFBundleCopyResourceURL											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyResourceURL						]))
//#define CoreF_CFBundleCopyResourceURLForLocalization							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyResourceURLForLocalization		]))
//#define CoreF_CFBundleCopyResourceURLInDirectory								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyResourceURLInDirectory			]))
//#define CoreF_CFBundleCopyResourceURLsOfType									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyResourceURLsOfType				]))
//#define CoreF_CFBundleCopyResourceURLsOfTypeForLocalization					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyResourceURLsOfTypeForLocalization	]))
//#define CoreF_CFBundleCopyResourceURLsOfTypeInDirectory						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyResourceURLsOfTypeInDirectory		]))
//#define CoreF_CFBundleCopyResourcesDirectoryURL								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopyResourcesDirectoryURL				]))
//#define CoreF_CFBundleCopySharedFrameworksURL									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopySharedFrameworksURL					]))
//#define CoreF_CFBundleCopySharedSupportURL									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopySharedSupportURL					]))
//#define CoreF_CFBundleCopySupportFilesDirectoryURL							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCopySupportFilesDirectoryURL			]))
//#define CoreF_CFBundleCreate													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCreate									]))
//#define CoreF_CFBundleCreateBundlesFromDirectory								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleCreateBundlesFromDirectory				]))
//#define CoreF_CFBundleGetAllBundles											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetAllBundles							]))
//#define CoreF_CFBundleGetBundleWithIdentifier									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetBundleWithIdentifier					]))
//#define CoreF_CFBundleGetDataPointerForName									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetDataPointerForName					]))
//#define CoreF_CFBundleGetDataPointersForNames									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetDataPointersForNames					]))
//#define CoreF_CFBundleGetDevelopmentRegion									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetDevelopmentRegion					]))
//#define CoreF_CFBundleGetFunctionPointerForName								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetFunctionPointerForName				]))
//#define CoreF_CFBundleGetFunctionPointersForNames								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetFunctionPointersForNames				]))
//#define CoreF_CFBundleGetIdentifier											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetIdentifier							]))
//#define CoreF_CFBundleGetInfoDictionary										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetInfoDictionary						]))
//#define CoreF_CFBundleGetLocalInfoDictionary									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetLocalInfoDictionary					]))
//#define CoreF_CFBundleGetLocalizationInfoForLocalization						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetLocalizationInfoForLocalization		]))
//#define CoreF_CFBundleGetMainBundle											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetMainBundle							]))
//#define CoreF_CFBundleGetPackageInfo											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetPackageInfo							]))
//#define CoreF_CFBundleGetPackageInfoInDirectory								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetPackageInfoInDirectory				]))
//#define CoreF_CFBundleGetPlugIn												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetPlugIn								]))
//#define CoreF_CFBundleGetTypeID												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetTypeID								]))
//#define CoreF_CFBundleGetValueForInfoDictionaryKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetValueForInfoDictionaryKey			]))
//#define CoreF_CFBundleGetVersionNumber										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleGetVersionNumber						]))
//#define CoreF_CFBundleIsExecutableLoaded										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleIsExecutableLoaded						]))
//#define CoreF_CFBundleLoadExecutable											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleLoadExecutable							]))
//#define CoreF_CFBundleLoadExecutableAndReturnError							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleLoadExecutableAndReturnError			]))
//#define CoreF_CFBundlePreflightExecutable										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundlePreflightExecutable						]))
//#define CoreF_CFBundleUnloadExecutable										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBundleUnloadExecutable						]))
//#define CoreF_CFBurstTrieAdd													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieAdd									]))
//#define CoreF_CFBurstTrieAddCharacters										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieAddCharacters				]))
//#define CoreF_CFBurstTrieAddCharactersWithWeight								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieAddCharactersWithWeight		]))
//#define CoreF_CFBurstTrieAddUTF8String										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieAddUTF8String				]))
//#define CoreF_CFBurstTrieAddUTF8StringWithWeight								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieAddUTF8StringWithWeight		]))
//#define CoreF_CFBurstTrieAddWithWeight										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieAddWithWeight				]))
//#define CoreF_CFBurstTrieContains												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieContains						]))
//#define CoreF_CFBurstTrieContainsCharacters									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieContainsCharacters			]))
//#define CoreF_CFBurstTrieContainsUTF8String									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieContainsUTF8String			]))
//#define CoreF_CFBurstTrieCreate												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieCreate						]))
//#define CoreF_CFBurstTrieCreateFromFile										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieCreateFromFile				]))
//#define CoreF_CFBurstTrieCreateFromMapBytes									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieCreateFromMapBytes			]))
//#define CoreF_CFBurstTrieFind													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieFind							]))
//#define CoreF_CFBurstTrieFindCharacters										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieFindCharacters				]))
//#define CoreF_CFBurstTrieFindUTF8String										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieFindUTF8String				]))
//#define CoreF_CFBurstTrieGetCount												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieGetCount						]))
//#define CoreF_CFBurstTrieInsert												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieInsert						]))
//#define CoreF_CFBurstTrieInsertCharacters										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieInsertCharacters				]))
//#define CoreF_CFBurstTrieInsertCharactersWithWeight							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieInsertCharactersWithWeight]))
//#define CoreF_CFBurstTrieInsertUTF8String										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieInsertUTF8String				]))
//#define CoreF_CFBurstTrieInsertUTF8StringWithWeight							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieInsertUTF8StringWithWeight]))
//#define CoreF_CFBurstTrieInsertWithWeight										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieInsertWithWeight				]))
//#define CoreF_CFBurstTrieRelease												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieRelease						]))
//#define CoreF_CFBurstTrieRetain												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieRetain						]))
//#define CoreF_CFBurstTrieSerialize											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieSerialize					]))
//#define CoreF_CFBurstTrieSerializeWithFileDescriptor							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieSerializeWithFileDescriptor]))
//#define CoreF_CFBurstTrieTraverse												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFBurstTrieTraverse					]))
//#define CoreF_CFCalendarAddComponents											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarAddComponents				]))
//#define CoreF_CFCalendarComposeAbsoluteTime									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarComposeAbsoluteTime		]))
//#define CoreF_CFCalendarCopyCurrent											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarCopyCurrent				]))
//#define CoreF_CFCalendarCopyGregorianStartDate								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarCopyGregorianStartDate	]))
//#define CoreF_CFCalendarCopyLocale											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarCopyLocale				]))
//#define CoreF_CFCalendarCopyTimeZone											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarCopyTimeZone				]))
//#define CoreF_CFCalendarCreateWithIdentifier									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarCreateWithIdentifier		]))
//#define CoreF_CFCalendarDecomposeAbsoluteTime									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarDecomposeAbsoluteTime		]))
//#define CoreF_CFCalendarGetComponentDifference								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarGetComponentDifference	]))
//#define CoreF_CFCalendarGetFirstWeekday										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarGetFirstWeekday			]))
//#define CoreF_CFCalendarGetIdentifier											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarGetIdentifier				]))
//#define CoreF_CFCalendarGetMaximumRangeOfUnit									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarGetMaximumRangeOfUnit		]))
//#define CoreF_CFCalendarGetMinimumDaysInFirstWeek								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarGetMinimumDaysInFirstWeek	]))
//#define CoreF_CFCalendarGetMinimumRangeOfUnit									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarGetMinimumRangeOfUnit		]))
//#define CoreF_CFCalendarGetOrdinalityOfUnit									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarGetOrdinalityOfUnit		]))
//#define CoreF_CFCalendarGetRangeOfUnit										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarGetRangeOfUnit			]))
//#define CoreF_CFCalendarGetTimeRangeOfUnit									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarGetTimeRangeOfUnit		]))
//#define CoreF_CFCalendarGetTypeID												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarGetTypeID					]))
//#define CoreF_CFCalendarSetFirstWeekday										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarSetFirstWeekday			]))
//#define CoreF_CFCalendarSetGregorianStartDate									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarSetGregorianStartDate		]))
//#define CoreF_CFCalendarSetLocale												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarSetLocale					]))
//#define CoreF_CFCalendarSetMinimumDaysInFirstWeek								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarSetMinimumDaysInFirstWeek	]))
//#define CoreF_CFCalendarSetTimeZone											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCalendarSetTimeZone				]))
//#define CoreF_CFCharacterSetAddCharactersInRange								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetAddCharactersInRange	]))
//#define CoreF_CFCharacterSetAddCharactersInString								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetAddCharactersInString	]))
//#define CoreF_CFCharacterSetCompact											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetCompact						]))
//#define CoreF_CFCharacterSetCreateBitmapRepresentation						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetCreateBitmapRepresentation	]))
//#define CoreF_CFCharacterSetCreateCopy										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetCreateCopy					]))
//#define CoreF_CFCharacterSetCreateInvertedSet									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetCreateInvertedSet				]))
//#define CoreF_CFCharacterSetCreateMutable										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetCreateMutable					]))
//#define CoreF_CFCharacterSetCreateMutableCopy									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetCreateMutableCopy				]))
//#define CoreF_CFCharacterSetCreateWithBitmapRepresentation					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetCreateWithBitmapRepresentation]))
//#define CoreF_CFCharacterSetCreateWithCharactersInRange						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetCreateWithCharactersInRange	]))
//#define CoreF_CFCharacterSetCreateWithCharactersInString						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetCreateWithCharactersInString	]))
//#define CoreF_CFCharacterSetFast												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetFast							]))
//#define CoreF_CFCharacterSetGetPredefined										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetGetPredefined					]))
//#define CoreF_CFCharacterSetGetTypeID											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetGetTypeID						]))
//#define CoreF_CFCharacterSetHasMemberInPlane									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetHasMemberInPlane				]))
//#define CoreF_CFCharacterSetInitInlineBuffer									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetInitInlineBuffer				]))
//#define CoreF_CFCharacterSetIntersect											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetIntersect						]))
//#define CoreF_CFCharacterSetInvert											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetInvert						]))
//#define CoreF_CFCharacterSetIsCharacterMember									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetIsCharacterMember				]))
//#define CoreF_CFCharacterSetIsLongCharacterMember								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetIsLongCharacterMember			]))
//#define CoreF_CFCharacterSetIsSupersetOfSet									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetIsSupersetOfSet				]))
//#define CoreF_CFCharacterSetIsSurrogatePairMember								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetIsSurrogatePairMember			]))
//#define CoreF_CFCharacterSetRemoveCharactersInRange							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetRemoveCharactersInRange		]))
//#define CoreF_CFCharacterSetRemoveCharactersInString							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetRemoveCharactersInString		]))
//#define CoreF_CFCharacterSetUnion												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCharacterSetUnion							]))
//#define CoreF_CFCopyDescription												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCopyDescription							]))
//#define CoreF_CFCopyHomeDirectoryURLForUser									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCopyHomeDirectoryURLForUser				]))
//#define CoreF_CFCopySearchPathForDirectoriesInDomains							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCopySearchPathForDirectoriesInDomains		]))
//#define CoreF_CFCopySystemVersionString										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCopySystemVersionString					]))
//#define CoreF_CFCopyTypeIDDescription											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCopyTypeIDDescription						]))
//#define CoreF_CFCopyUserName													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFCopyUserName								]))
//#define CoreF_CFDataAppendBytes												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataAppendBytes							]))
#define CFDataCreate														((PF_CFDataCreate)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataCreate								]))
//#define CoreF_CFDataCreateCopy												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataCreateCopy							]))
//#define CoreF_CFDataCreateMutable												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataCreateMutable							]))
//#define CoreF_CFDataCreateMutableCopy											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataCreateMutableCopy						]))
//#define CoreF_CFDataCreateWithBytesNoCopy										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataCreateWithBytesNoCopy					]))
//#define CoreF_CFDataDeleteBytes												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataDeleteBytes							]))
//#define CoreF_CFDataFind														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataFind									]))
#define CFDataGetBytePtr													((PF_CFDataGetBytePtr)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataGetBytePtr							]))
//#define CoreF_CFDataGetBytes													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataGetBytes								]))
#define CFDataGetLength														((PF_CFDataGetLength)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataGetLength								]))
//#define CoreF_CFDataGetMutableBytePtr											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataGetMutableBytePtr						]))
//#define CoreF_CFDataGetTypeID													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataGetTypeID								]))
//#define CoreF_CFDataIncreaseLength											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataIncreaseLength						]))
//#define CoreF_CFDataReplaceBytes												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataReplaceBytes							]))
//#define CoreF_CFDataSetLength													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDataSetLength								]))
//#define CoreF_CFDateCompare													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateCompare								]))
//#define CoreF_CFDateCreate													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateCreate								]))
//#define CoreF_CFDateFormatterCopyProperty										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateFormatterCopyProperty					]))
//#define CoreF_CFDateFormatterCreate											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateFormatterCreate						]))
//#define CoreF_CFDateFormatterCreateDateFormatFromTemplate						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateFormatterCreateDateFormatFromTemplate	]))
//#define CoreF_CFDateFormatterCreateDateFormatsFromTemplates					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateFormatterCreateDateFormatsFromTemplates]))
//#define CoreF_CFDateFormatterCreateDateFromString								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateFormatterCreateDateFromString			]))
//#define CoreF_CFDateFormatterCreateStringWithAbsoluteTime						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateFormatterCreateStringWithAbsoluteTime	]))
//#define CoreF_CFDateFormatterCreateStringWithDate								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateFormatterCreateStringWithDate			]))
//#define CoreF_CFDateFormatterGetAbsoluteTimeFromString						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateFormatterGetAbsoluteTimeFromString	]))
//#define CoreF_CFDateFormatterGetDateStyle										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateFormatterGetDateStyle					]))
//#define CoreF_CFDateFormatterGetFormat										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateFormatterGetFormat					]))
//#define CoreF_CFDateFormatterGetLocale										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateFormatterGetLocale					]))
//#define CoreF_CFDateFormatterGetTimeStyle										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateFormatterGetTimeStyle					]))
//#define CoreF_CFDateFormatterGetTypeID										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateFormatterGetTypeID					]))
//#define CoreF_CFDateFormatterSetFormat										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateFormatterSetFormat					]))
//#define CoreF_CFDateFormatterSetProperty										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateFormatterSetProperty					]))
//#define CoreF_CFDateGetAbsoluteTime											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateGetAbsoluteTime						]))
//#define CoreF_CFDateGetTimeIntervalSinceDate									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateGetTimeIntervalSinceDate				]))
//#define CoreF_CFDateGetTypeID													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDateGetTypeID								]))
#define CFDictionaryAddValue												((PF_CFDictionaryAddValue)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryAddValue						]))
//#define CoreF_CFDictionaryApplyFunction										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryApplyFunction					]))
//#define CoreF_CFDictionaryContainsKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryContainsKey						]))
//#define CoreF_CFDictionaryContainsValue										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryContainsValue					]))
//#define CoreF_CFDictionaryCreate												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryCreate							]))
#define CFDictionaryCreateCopy												((PF_CFDictionaryCreateCopy)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryCreateCopy						]))
#define CFDictionaryCreateMutable											((PF_CFDictionaryCreateMutable)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryCreateMutable					]))
#define CFDictionaryCreateMutableCopy										((PF_CFDictionaryCreateMutableCopy)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryCreateMutableCopy				]))
#define CFDictionaryGetCount												((PF_CFDictionaryGetCount)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryGetCount						]))
//#define CoreF_CFDictionaryGetCountOfKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryGetCountOfKey					]))
//#define CoreF_CFDictionaryGetCountOfValue										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryGetCountOfValue					]))
//#define CoreF_CFDictionaryGetKeyIfPresent										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryGetKeyIfPresent					]))
#define CFDictionaryGetKeysAndValues										((PF_CFDictionaryGetKeysAndValues)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryGetKeysAndValues				]))
#define CFDictionaryGetTypeID												((PF_CFDictionaryGetTypeID)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryGetTypeID						]))
#define CFDictionaryGetValue												((PF_CFDictionaryGetValue)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryGetValue						]))
//#define CoreF_CFDictionaryGetValueIfPresent									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryGetValueIfPresent				]))
//#define CoreF_CFDictionaryRemoveAllValues										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryRemoveAllValues					]))
//#define CoreF_CFDictionaryRemoveValue											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryRemoveValue						]))
//#define CoreF_CFDictionaryReplaceValue										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionaryReplaceValue					]))
#define CFDictionarySetValue												((PF_CFDictionarySetValue)(__gpf_CoreFoundation_dll_ri[ENUM_CFDictionarySetValue						]))
//#define CoreF_CFEqual															((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFEqual									]))
//#define CoreF_CFErrorCopyDescription											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFErrorCopyDescription					]))
//#define CoreF_CFErrorCopyFailureReason										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFErrorCopyFailureReason				]))
//#define CoreF_CFErrorCopyRecoverySuggestion									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFErrorCopyRecoverySuggestion			]))
//#define CoreF_CFErrorCopyUserInfo												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFErrorCopyUserInfo						]))
//#define CoreF_CFErrorCreate													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFErrorCreate							]))
//#define CoreF_CFErrorCreateWithUserInfoKeysAndValues							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFErrorCreateWithUserInfoKeysAndValues	]))
//#define CoreF_CFErrorGetCallBackForDomain										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFErrorGetCallBackForDomain				]))
//#define CoreF_CFErrorGetCode													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFErrorGetCode							]))
//#define CoreF_CFErrorGetDomain												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFErrorGetDomain						]))
//#define CoreF_CFErrorGetTypeID												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFErrorGetTypeID						]))
//#define CoreF_CFErrorSetCallBackForDomain										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFErrorSetCallBackForDomain				]))
//#define CoreF_CFGetAllocator													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFGetAllocator							]))
//#define CoreF_CFGetRetainCount												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFGetRetainCount						]))
//#define CoreF_CFGetSystemUptime												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFGetSystemUptime						]))
#define CFGetTypeID														((PF_CFGetTypeID)(__gpf_CoreFoundation_dll_ri[ENUM_CFGetTypeID								]))
//#define CoreF_CFGetUserName													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFGetUserName							]))
//#define CoreF_CFGregorianDateGetAbsoluteTime									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFGregorianDateGetAbsoluteTime			]))
//#define CoreF_CFGregorianDateIsValid											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFGregorianDateIsValid					]))
//#define CoreF_CFHash															((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFHash									]))
//#define CoreF_CFHashBytes														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFHashBytes								]))
//#define CoreF_CFLocaleCopyAvailableLocaleIdentifiers							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCopyAvailableLocaleIdentifiers	]))
//#define CoreF_CFLocaleCopyCommonISOCurrencyCodes								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCopyCommonISOCurrencyCodes		]))
//#define CoreF_CFLocaleCopyCurrent												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCopyCurrent						]))
//#define CoreF_CFLocaleCopyDisplayNameForPropertyValue							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCopyDisplayNameForPropertyValue	]))
//#define CoreF_CFLocaleCopyISOCountryCodes										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCopyISOCountryCodes				]))
//#define CoreF_CFLocaleCopyISOCurrencyCodes									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCopyISOCurrencyCodes			]))
//#define CoreF_CFLocaleCopyISOLanguageCodes									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCopyISOLanguageCodes			]))
//#define CoreF_CFLocaleCopyPreferredLanguages									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCopyPreferredLanguages			]))
//#define CoreF_CFLocaleCreate													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCreate													]))
//#define CoreF_CFLocaleCreateCanonicalLanguageIdentifierFromString				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCreateCanonicalLanguageIdentifierFromString				]))
//#define CoreF_CFLocaleCreateCanonicalLocaleIdentifierFromScriptManagerCodes	((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCreateCanonicalLocaleIdentifierFromScriptManagerCodes	]))
//#define CoreF_CFLocaleCreateCanonicalLocaleIdentifierFromString				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCreateCanonicalLocaleIdentifierFromString				]))
//#define CoreF_CFLocaleCreateComponentsFromLocaleIdentifier					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCreateComponentsFromLocaleIdentifier					]))
//#define CoreF_CFLocaleCreateCopy												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCreateCopy												]))
//#define CoreF_CFLocaleCreateLocaleIdentifierFromComponents					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCreateLocaleIdentifierFromComponents					]))
//#define CoreF_CFLocaleCreateLocaleIdentifierFromWindowsLocaleCode				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleCreateLocaleIdentifierFromWindowsLocaleCode				]))
//#define CoreF_CFLocaleGetIdentifier											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleGetIdentifier											]))
//#define CoreF_CFLocaleGetLanguageCharacterDirection							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleGetLanguageCharacterDirection							]))
//#define CoreF_CFLocaleGetLanguageLineDirection								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleGetLanguageLineDirection								]))
//#define CoreF_CFLocaleGetLanguageRegionEncodingForLocaleIdentifier			((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleGetLanguageRegionEncodingForLocaleIdentifier			]))
//#define CoreF_CFLocaleGetSystem												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleGetSystem												]))
//#define CoreF_CFLocaleGetTypeID												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleGetTypeID												]))
//#define CoreF_CFLocaleGetValue												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleGetValue												]))
//#define CoreF_CFLocaleGetWindowsLocaleCodeFromLocaleIdentifier				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLocaleGetWindowsLocaleCodeFromLocaleIdentifier				]))
//#define CoreF_CFLog															((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFLog								]))
//#define CoreF_CFMakeCollectable												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMakeCollectable					]))
//#define CoreF_CFMergeSortArray												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMergeSortArray					]))
//#define CoreF_CFMessagePortCreateLocal										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMessagePortCreateLocal			]))
//#define CoreF_CFMessagePortCreateRemote										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMessagePortCreateRemote			]))
//#define CoreF_CFMessagePortCreateRunLoopSource								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMessagePortCreateRunLoopSource	]))
//#define CoreF_CFMessagePortCreateUber											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMessagePortCreateUber				]))
//#define CoreF_CFMessagePortGetContext											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMessagePortGetContext				]))
//#define CoreF_CFMessagePortGetInvalidationCallBack							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMessagePortGetInvalidationCallBack]))
//#define CoreF_CFMessagePortGetName											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMessagePortGetName				]))
//#define CoreF_CFMessagePortGetTypeID											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMessagePortGetTypeID				]))
//#define CoreF_CFMessagePortInvalidate											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMessagePortInvalidate				]))
//#define CoreF_CFMessagePortIsRemote											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMessagePortIsRemote				]))
//#define CoreF_CFMessagePortIsValid											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMessagePortIsValid				]))
//#define CoreF_CFMessagePortSendRequest										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMessagePortSendRequest			]))
//#define CoreF_CFMessagePortSetCloneCallout									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMessagePortSetCloneCallout		]))
//#define CoreF_CFMessagePortSetInvalidationCallBack							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMessagePortSetInvalidationCallBack]))
//#define CoreF_CFMessagePortSetName											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMessagePortSetName							]))
//#define CoreF_CFMutableAttributedStringOpenUText								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMutableAttributedStringOpenUText				]))
//#define CoreF_CFMutableStringOpenUText										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFMutableStringOpenUText						]))
//#define CoreF_CFNotificationCenterAddObserver									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNotificationCenterAddObserver					]))
//#define CoreF_CFNotificationCenterGetDarwinNotifyCenter						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNotificationCenterGetDarwinNotifyCenter		]))
//#define CoreF_CFNotificationCenterGetDistributedCenter						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNotificationCenterGetDistributedCenter		]))
//#define CoreF_CFNotificationCenterGetLocalCenter								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNotificationCenterGetLocalCenter				]))
//#define CoreF_CFNotificationCenterGetTypeID									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNotificationCenterGetTypeID					]))
//#define CoreF_CFNotificationCenterPostNotification							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNotificationCenterPostNotification			]))
//#define CoreF_CFNotificationCenterPostNotificationWithOptions					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNotificationCenterPostNotificationWithOptions	]))
//#define CoreF_CFNotificationCenterRemoveEveryObserver							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNotificationCenterRemoveEveryObserver			]))
//#define CoreF_CFNotificationCenterRemoveObserver								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNotificationCenterRemoveObserver				]))
//#define CoreF_CFNullGetTypeID													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNullGetTypeID									]))
//#define CoreF_CFNumberCompare													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberCompare									]))
#define CFNumberCreate													((PF_CFNumberCreate)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberCreate									]))
//#define CoreF_CFNumberFormatterCopyProperty									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberFormatterCopyProperty					]))
//#define CoreF_CFNumberFormatterCreate											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberFormatterCreate							]))
//#define CoreF_CFNumberFormatterCreateNumberFromString							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberFormatterCreateNumberFromString			]))
//#define CoreF_CFNumberFormatterCreateStringWithNumber							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberFormatterCreateStringWithNumber			]))
//#define CoreF_CFNumberFormatterCreateStringWithValue							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberFormatterCreateStringWithValue			]))
//#define CoreF_CFNumberFormatterGetDecimalInfoForCurrencyCode					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberFormatterGetDecimalInfoForCurrencyCode	]))
//#define CoreF_CFNumberFormatterGetFormat										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberFormatterGetFormat						]))
//#define CoreF_CFNumberFormatterGetLocale										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberFormatterGetLocale						]))
//#define CoreF_CFNumberFormatterGetStyle										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberFormatterGetStyle						]))
//#define CoreF_CFNumberFormatterGetTypeID										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberFormatterGetTypeID						]))
//#define CoreF_CFNumberFormatterGetValueFromString								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberFormatterGetValueFromString				]))
//#define CoreF_CFNumberFormatterSetFormat										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberFormatterSetFormat					]))
//#define CoreF_CFNumberFormatterSetProperty									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberFormatterSetProperty				]))
//#define CoreF_CFNumberGetByteSize												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberGetByteSize							]))
//#define CoreF_CFNumberGetType													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberGetType								]))
//#define CoreF_CFNumberGetTypeID												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberGetTypeID							]))
#define CFNumberGetValue													((PF_CFNumberGetValue)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberGetValue							]))
//#define CoreF_CFNumberIsFloatType												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFNumberIsFloatType							]))
//#define CoreF_CFPlugInAddInstanceForFactory									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInAddInstanceForFactory				]))
//#define CoreF_CFPlugInCreate													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInCreate								]))
//#define CoreF_CFPlugInFindFactoriesForPlugInType								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInFindFactoriesForPlugInType			]))
//#define CoreF_CFPlugInFindFactoriesForPlugInTypeInPlugIn						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInFindFactoriesForPlugInTypeInPlugIn	]))
//#define CoreF_CFPlugInGetBundle												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInGetBundle							]))
//#define CoreF_CFPlugInGetTypeID												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInGetTypeID							]))
//#define CoreF_CFPlugInInstanceCreate											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInInstanceCreate						]))
//#define CoreF_CFPlugInInstanceCreateWithInstanceDataSize						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInInstanceCreateWithInstanceDataSize	]))
//#define CoreF_CFPlugInInstanceGetFactoryName									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInInstanceGetFactoryName				]))
//#define CoreF_CFPlugInInstanceGetInstanceData									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInInstanceGetInstanceData				]))
//#define CoreF_CFPlugInInstanceGetInterfaceFunctionTable						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInInstanceGetInterfaceFunctionTable	]))
//#define CoreF_CFPlugInInstanceGetTypeID										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInInstanceGetTypeID					]))
//#define CoreF_CFPlugInIsLoadOnDemand											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInIsLoadOnDemand						]))
//#define CoreF_CFPlugInRegisterFactoryFunction									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInRegisterFactoryFunction				]))
//#define CoreF_CFPlugInRegisterFactoryFunctionByName							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInRegisterFactoryFunctionByName		]))
//#define CoreF_CFPlugInRegisterPlugInType										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInRegisterPlugInType					]))
//#define CoreF_CFPlugInRemoveInstanceForFactory								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInRemoveInstanceForFactory			]))
//#define CoreF_CFPlugInSetLoadOnDemand											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInSetLoadOnDemand						]))
//#define CoreF_CFPlugInUnregisterFactory										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInUnregisterFactory					]))
//#define CoreF_CFPlugInUnregisterPlugInType									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPlugInUnregisterPlugInType				]))
//#define CoreF_CFPreferencesAddSuitePreferencesToApp							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesAddSuitePreferencesToApp		]))
//#define CoreF_CFPreferencesAppSynchronize										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesAppSynchronize					]))
//#define CoreF_CFPreferencesAppValueIsForced									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesAppValueIsForced				]))
//#define CoreF_CFPreferencesCopyAppValue										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesCopyAppValue					]))
//#define CoreF_CFPreferencesCopyApplicationList								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesCopyApplicationList			]))
//#define CoreF_CFPreferencesCopyKeyList										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesCopyKeyList					]))
//#define CoreF_CFPreferencesCopyMultiple										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesCopyMultiple					]))
//#define CoreF_CFPreferencesCopyValue											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesCopyValue						]))
//#define CoreF_CFPreferencesFlushCaches										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesFlushCaches					]))
//#define CoreF_CFPreferencesGetAppBooleanValue									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesGetAppBooleanValue				]))
//#define CoreF_CFPreferencesGetAppIntegerValue									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesGetAppIntegerValue				]))
//#define CoreF_CFPreferencesRemoveSuitePreferencesFromApp						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesRemoveSuitePreferencesFromApp	]))
//#define CoreF_CFPreferencesSetAppValue										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesSetAppValue			]))
//#define CoreF_CFPreferencesSetMultiple										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesSetMultiple			]))
//#define CoreF_CFPreferencesSetValue											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesSetValue				]))
//#define CoreF_CFPreferencesSynchronize										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPreferencesSynchronize			]))
#define CFPropertyListCreateData											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPropertyListCreateData			]))
//#define CoreF_CFPropertyListCreateDeepCopy									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPropertyListCreateDeepCopy		]))
//#define CoreF_CFPropertyListCreateFromStream									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPropertyListCreateFromStream		]))
#define CFPropertyListCreateFromXMLData										((PF_CFPropertyListCreateFromXMLData)(__gpf_CoreFoundation_dll_ri[ENUM_CFPropertyListCreateFromXMLData		]))
//#define CoreF_CFPropertyListCreateWithData									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPropertyListCreateWithData		]))
//#define CoreF_CFPropertyListCreateWithStream									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPropertyListCreateWithStream		]))
#define CFPropertyListCreateXMLData											((PF_CFPropertyListCreateXMLData)(__gpf_CoreFoundation_dll_ri[ENUM_CFPropertyListCreateXMLData			]))
//#define CoreF_CFPropertyListIsValid											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPropertyListIsValid				]))
//#define CoreF_CFPropertyListWrite												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFPropertyListWrite					]))
#define CFPropertyListWriteToStream											((PF_CFPropertyListWriteToStream)(__gpf_CoreFoundation_dll_ri[ENUM_CFPropertyListWriteToStream			]))
//#define CoreF_CFQSortArray													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFQSortArray						]))
//#define CoreF_CFReadStreamClose												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamClose					]))
//#define CoreF_CFReadStreamCopyError											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamCopyError				]))
//#define CoreF_CFReadStreamCopyProperty										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamCopyProperty			]))
//#define CoreF_CFReadStreamCreate												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamCreate					]))
//#define CoreF_CFReadStreamCreateWithBytesNoCopy								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamCreateWithBytesNoCopy	]))
//#define CoreF_CFReadStreamCreateWithData										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamCreateWithData			]))
//#define CoreF_CFReadStreamCreateWithFile										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamCreateWithFile			]))
//#define CoreF_CFReadStreamGetBuffer											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamGetBuffer				]))
//#define CoreF_CFReadStreamGetError											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamGetError				]))
//#define CoreF_CFReadStreamGetInfoPointer										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamGetInfoPointer			]))
//#define CoreF_CFReadStreamGetStatus											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamGetStatus				]))
//#define CoreF_CFReadStreamGetTypeID											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamGetTypeID				]))
//#define CoreF_CFReadStreamHasBytesAvailable									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamHasBytesAvailable		]))
//#define CoreF_CFReadStreamOpen												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamOpen					]))
//#define CoreF_CFReadStreamRead												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamRead					]))
//#define CoreF_CFReadStreamScheduleWithRunLoop									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamScheduleWithRunLoop		]))
//#define CoreF_CFReadStreamSetClient											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamSetClient				]))
//#define CoreF_CFReadStreamSetProperty											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamSetProperty				]))
//#define CoreF_CFReadStreamSignalEvent											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamSignalEvent				]))
//#define CoreF_CFReadStreamUnscheduleFromRunLoop								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFReadStreamUnscheduleFromRunLoop	]))
#define CFRelease															((PF_CFRelease)(__gpf_CoreFoundation_dll_ri[ENUM_CFRelease							]))
//#define CoreF_CFRetain														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRetain							]))
//#define CoreF_CFRunArrayCreate												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunArrayCreate					]))
//#define CoreF_CFRunArrayDelete												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunArrayDelete					]))
//#define CoreF_CFRunArrayGetCount												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunArrayGetCount					]))
//#define CoreF_CFRunArrayGetTypeID												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunArrayGetTypeID					]))
//#define CoreF_CFRunArrayGetValueAtIndex										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunArrayGetValueAtIndex			]))
//#define CoreF_CFRunArrayGetValueAtRunArrayIndex								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunArrayGetValueAtRunArrayIndex	]))
//#define CoreF_CFRunArrayInsert												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunArrayInsert					]))
//#define CoreF_CFRunArrayReplace												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunArrayReplace					]))
//#define CoreF_CFRunLoopAddCommonMode											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopAddCommonMode				]))
//#define CoreF_CFRunLoopAddObserver											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopAddObserver				]))
//#define CoreF_CFRunLoopAddSource												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopAddSource					]))
//#define CoreF_CFRunLoopAddTimer												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopAddTimer					]))
//#define CoreF_CFRunLoopContainsObserver										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopContainsObserver			]))
//#define CoreF_CFRunLoopContainsSource											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopContainsSource				]))
//#define CoreF_CFRunLoopContainsTimer											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopContainsTimer				]))
//#define CoreF_CFRunLoopCopyAllModes											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopCopyAllModes				]))
//#define CoreF_CFRunLoopCopyCurrentMode										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopCopyCurrentMode			]))
//#define CoreF_CFRunLoopGetCurrent												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopGetCurrent					]))
//#define CoreF_CFRunLoopGetMain												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopGetMain					]))
//#define CoreF_CFRunLoopGetNextTimerFireDate									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopGetNextTimerFireDate		]))
//#define CoreF_CFRunLoopGetTypeID												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopGetTypeID					]))
//#define CoreF_CFRunLoopIsWaiting												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopIsWaiting					]))
//#define CoreF_CFRunLoopObserverCreate											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopObserverCreate				]))
//#define CoreF_CFRunLoopObserverCreateWithHandler								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopObserverCreateWithHandler	]))
//#define CoreF_CFRunLoopObserverDoesRepeat										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopObserverDoesRepeat			]))
//#define CoreF_CFRunLoopObserverGetActivities									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopObserverGetActivities		]))
//#define CoreF_CFRunLoopObserverGetContext										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopObserverGetContext			]))
//#define CoreF_CFRunLoopObserverGetOrder										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopObserverGetOrder			]))
//#define CoreF_CFRunLoopObserverGetTypeID										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopObserverGetTypeID			]))
//#define CoreF_CFRunLoopObserverInvalidate										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopObserverInvalidate			]))
//#define CoreF_CFRunLoopObserverIsValid										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopObserverIsValid			]))
//#define CoreF_CFRunLoopPerformBlock											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopPerformBlock				]))
//#define CoreF_CFRunLoopRemoveObserver											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopRemoveObserver				]))
//#define CoreF_CFRunLoopRemoveSource											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopRemoveSource				]))
//#define CoreF_CFRunLoopRemoveTimer											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopRemoveTimer				]))
//#define CoreF_CFRunLoopRun													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopRun						]))
//#define CoreF_CFRunLoopRunInMode												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopRunInMode					]))
//#define CoreF_CFRunLoopSourceCreate											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopSourceCreate				]))
//#define CoreF_CFRunLoopSourceGetContext										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopSourceGetContext			]))
//#define CoreF_CFRunLoopSourceGetOrder											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopSourceGetOrder				]))
//#define CoreF_CFRunLoopSourceGetTypeID										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopSourceGetTypeID			]))
//#define CoreF_CFRunLoopSourceInvalidate										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopSourceInvalidate			]))
//#define CoreF_CFRunLoopSourceIsValid											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopSourceIsValid				]))
//#define CoreF_CFRunLoopSourceSignal											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopSourceSignal					]))
//#define CoreF_CFRunLoopStop													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopStop							]))
//#define CoreF_CFRunLoopTimerCreate											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopTimerCreate					]))
//#define CoreF_CFRunLoopTimerCreateWithHandler									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopTimerCreateWithHandler			]))
//#define CoreF_CFRunLoopTimerDoesRepeat										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopTimerDoesRepeat				]))
//#define CoreF_CFRunLoopTimerGetContext										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopTimerGetContext				]))
//#define CoreF_CFRunLoopTimerGetInterval										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopTimerGetInterval				]))
//#define CoreF_CFRunLoopTimerGetNextFireDate									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopTimerGetNextFireDate			]))
//#define CoreF_CFRunLoopTimerGetOrder											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopTimerGetOrder					]))
//#define CoreF_CFRunLoopTimerGetTypeID											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopTimerGetTypeID					]))
//#define CoreF_CFRunLoopTimerInvalidate										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopTimerInvalidate				]))
//#define CoreF_CFRunLoopTimerIsValid											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopTimerIsValid					]))
//#define CoreF_CFRunLoopTimerSetNextFireDate									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopTimerSetNextFireDate			]))
//#define CoreF_CFRunLoopWakeUp													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFRunLoopWakeUp							]))
//#define CoreF_CFSetAddValue													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetAddValue							]))
//#define CoreF_CFSetApplyFunction												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetApplyFunction						]))
//#define CoreF_CFSetContainsValue												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetContainsValue						]))
//#define CoreF_CFSetCreate														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetCreate								]))
//#define CoreF_CFSetCreateCopy													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetCreateCopy							]))
//#define CoreF_CFSetCreateMutable												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetCreateMutable						]))
//#define CoreF_CFSetCreateMutableCopy											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetCreateMutableCopy					]))
//#define CoreF_CFSetGetCount													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetGetCount							]))
//#define CoreF_CFSetGetCountOfValue											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetGetCountOfValue					]))
//#define CoreF_CFSetGetTypeID													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetGetTypeID							]))
//#define CoreF_CFSetGetValue													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetGetValue							]))
//#define CoreF_CFSetGetValueIfPresent											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetGetValueIfPresent					]))
//#define CoreF_CFSetGetValues													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetGetValues							]))
//#define CoreF_CFSetRemoveAllValues											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetRemoveAllValues					]))
//#define CoreF_CFSetRemoveValue												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetRemoveValue						]))
//#define CoreF_CFSetReplaceValue												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetReplaceValue						]))
//#define CoreF_CFSetSetValue													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSetSetValue							]))
//#define CoreF_CFShow															((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFShow									]))
//#define CoreF_CFShowStr														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFShowStr								]))
//#define CoreF_CFSocketConnectToAddress										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketConnectToAddress				]))
//#define CoreF_CFSocketCopyAddress												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketCopyAddress						]))
//#define CoreF_CFSocketCopyPeerAddress											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketCopyPeerAddress					]))
//#define CoreF_CFSocketCopyRegisteredSocketSignature							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketCopyRegisteredSocketSignature	]))
//#define CoreF_CFSocketCopyRegisteredValue										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketCopyRegisteredValue				]))
//#define CoreF_CFSocketCreate													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketCreate							]))
//#define CoreF_CFSocketCreateConnectedToSocketSignature						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketCreateConnectedToSocketSignature]))
//#define CoreF_CFSocketCreateRunLoopSource										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketCreateRunLoopSource					]))
//#define CoreF_CFSocketCreateWithNative										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketCreateWithNative					]))
//#define CoreF_CFSocketCreateWithSocketSignature								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketCreateWithSocketSignature			]))
//#define CoreF_CFSocketDisableCallBacks										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketDisableCallBacks					]))
//#define CoreF_CFSocketEnableCallBacks											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketEnableCallBacks						]))
//#define CoreF_CFSocketGetContext												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketGetContext							]))
//#define CoreF_CFSocketGetDefaultNameRegistryPortNumber						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketGetDefaultNameRegistryPortNumber	]))
//#define CoreF_CFSocketGetNative												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketGetNative							]))
//#define CoreF_CFSocketGetSocketFlags											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketGetSocketFlags						]))
//#define CoreF_CFSocketGetTypeID												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketGetTypeID							]))
//#define CoreF_CFSocketInvalidate												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketInvalidate							]))
//#define CoreF_CFSocketIsValid													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketIsValid								]))
//#define CoreF_CFSocketRegisterSocketSignature									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketRegisterSocketSignature				]))
//#define CoreF_CFSocketRegisterValue											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketRegisterValue						]))
//#define CoreF_CFSocketSendData												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketSendData							]))
//#define CoreF_CFSocketSetAddress												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketSetAddress							]))
//#define CoreF_CFSocketSetDefaultNameRegistryPortNumber						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketSetDefaultNameRegistryPortNumber	]))
//#define CoreF_CFSocketSetSocketFlags											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketSetSocketFlags						]))
//#define CoreF_CFSocketUnregister												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFSocketUnregister							]))
//#define CoreF_CFStorageApplyBlock												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStorageApplyBlock							]))
//#define CoreF_CFStorageApplyFunction											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStorageApplyFunction						]))
//#define CoreF_CFStorageCreate													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStorageCreate								]))
//#define CoreF_CFStorageCreateWithSubrange										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStorageCreateWithSubrange					]))
//#define CoreF_CFStorageDeleteValues											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStorageDeleteValues						]))
//#define CoreF_CFStorageGetConstValueAtIndex									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStorageGetConstValueAtIndex				]))
//#define CoreF_CFStorageGetCount												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStorageGetCount							]))
//#define CoreF_CFStorageGetTypeID												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStorageGetTypeID							]))
//#define CoreF_CFStorageGetValueAtIndex										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStorageGetValueAtIndex					]))
//#define CoreF_CFStorageGetValues												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStorageGetValues							]))
//#define CoreF_CFStorageInsertValues											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStorageInsertValues						]))
//#define CoreF_CFStorageReplaceValues											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStorageReplaceValues						]))
//#define CoreF_CFStreamCreateBoundPair											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStreamCreateBoundPair						]))
//#define CoreF_CFStreamCreatePairWithPeerSocketSignature						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStreamCreatePairWithPeerSocketSignature	]))
//#define CoreF_CFStreamCreatePairWithSocket									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStreamCreatePairWithSocket				]))
//#define CoreF_CFStreamCreatePairWithSocketToHost								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStreamCreatePairWithSocketToHost			]))
//#define CoreF_CFStringAppend													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringAppend										]))
//#define CoreF_CFStringAppendCString											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringAppendCString								]))
//#define CoreF_CFStringAppendCharacters										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringAppendCharacters							]))
//#define CoreF_CFStringAppendFormat											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringAppendFormat								]))
//#define CoreF_CFStringAppendFormatAndArguments								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringAppendFormatAndArguments					]))
//#define CoreF_CFStringAppendPascalString										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringAppendPascalString							]))
//#define CoreF_CFStringCapitalize												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCapitalize									]))
#define CFStringCompare														((PF_CFStringCompare)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCompare										]))
#define CFStringCompareWithOptions											((PF_CFStringCompareWithOptions)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCompareWithOptions							]))
//#define CoreF_CFStringCompareWithOptionsAndLocale								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCompareWithOptionsAndLocale					]))
//#define CoreF_CFStringConvertEncodingToIANACharSetName						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringConvertEncodingToIANACharSetName			]))
//#define CoreF_CFStringConvertEncodingToNSStringEncoding						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringConvertEncodingToNSStringEncoding			]))
//#define CoreF_CFStringConvertEncodingToWindowsCodepage						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringConvertEncodingToWindowsCodepage			]))
//#define CoreF_CFStringConvertIANACharSetNameToEncoding						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringConvertIANACharSetNameToEncoding			]))
//#define CoreF_CFStringConvertNSStringEncodingToEncoding						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringConvertNSStringEncodingToEncoding			]))
//#define CoreF_CFStringConvertWindowsCodepageToEncoding						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringConvertWindowsCodepageToEncoding			]))
//#define CoreF_CFStringCreateArrayBySeparatingStrings							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateArrayBySeparatingStrings				]))
//#define CoreF_CFStringCreateArrayWithFindResults								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateArrayWithFindResults					]))
//#define CoreF_CFStringCreateByCombiningStrings								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateByCombiningStrings					]))
#define CFStringCreateCopy													((PF_CFStringCreateCopy)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateCopy									]))
//#define CoreF_CFStringCreateExternalRepresentation							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateExternalRepresentation				]))
//#define CoreF_CFStringCreateFromExternalRepresentation						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateFromExternalRepresentation			]))
//#define CoreF_CFStringCreateMutable											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateMutable								]))
//#define CoreF_CFStringCreateMutableCopy										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateMutableCopy							]))
//#define CoreF_CFStringCreateMutableWithExternalCharactersNoCopy				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateMutableWithExternalCharactersNoCopy	]))
//#define CoreF_CFStringCreateWithBytes											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateWithBytes								]))
//#define CoreF_CFStringCreateWithBytesNoCopy									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateWithBytesNoCopy						]))
#define CFStringCreateWithCString											((PF_CFStringCreateWithCString)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateWithCString							]))
//#define CoreF_CFStringCreateWithCStringNoCopy									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateWithCStringNoCopy						]))
#define CFStringCreateWithCharacters										((PF_CFStringCreateWithCharacters)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateWithCharacters						]))
//#define CoreF_CFStringCreateWithCharactersNoCopy								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateWithCharactersNoCopy					]))
//#define CoreF_CFStringCreateWithFileSystemRepresentation						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateWithFileSystemRepresentation			]))
#define CFStringCreateWithFormat											((PF_CFStringCreateWithFormat)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateWithFormat					]))
//#define CoreF_CFStringCreateWithFormatAndArguments							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateWithFormatAndArguments		]))
//#define CoreF_CFStringCreateWithPascalString									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateWithPascalString				]))
//#define CoreF_CFStringCreateWithPascalStringNoCopy							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateWithPascalStringNoCopy		]))
//#define CoreF_CFStringCreateWithSubstring										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringCreateWithSubstring					]))
//#define CoreF_CFStringDelete													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringDelete								]))
//#define CoreF_CFStringEncodingByteLengthForCharacters							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringEncodingByteLengthForCharacters		]))
//#define CoreF_CFStringEncodingBytesToUnicode									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringEncodingBytesToUnicode				]))
//#define CoreF_CFStringEncodingCharLengthForBytes								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringEncodingCharLengthForBytes			]))
//#define CoreF_CFStringEncodingIsValidEncoding									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringEncodingIsValidEncoding				]))
//#define CoreF_CFStringEncodingListOfAvailableEncodings						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringEncodingListOfAvailableEncodings	]))
//#define CoreF_CFStringEncodingRegisterFallbackProcedures						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringEncodingRegisterFallbackProcedures	]))
//#define CoreF_CFStringEncodingUnicodeToBytes									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringEncodingUnicodeToBytes				]))
//#define CoreF_CFStringFileSystemEncoding										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringFileSystemEncoding					]))
//#define CoreF_CFStringFind													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringFind								]))
//#define CoreF_CFStringFindAndReplace											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringFindAndReplace						]))
//#define CoreF_CFStringFindCharacterFromSet									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringFindCharacterFromSet				]))
//#define CoreF_CFStringFindWithOptions											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringFindWithOptions						]))
//#define CoreF_CFStringFindWithOptionsAndLocale								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringFindWithOptionsAndLocale			]))
//#define CoreF_CFStringFold													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringFold								]))
//#define CoreF_CFStringGetBytes												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetBytes							]))
#define CFStringGetCString													((PF_CFStringGetCString)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetCString							]))
#define CFStringGetCStringPtr												((PF_CFStringGetCStringPtr)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetCStringPtr						]))
//#define CoreF_CFStringGetCharacterAtIndex										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetCharacterAtIndex					]))
//#define CoreF_CFStringGetCharacters											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetCharacters						]))
//#define CoreF_CFStringGetCharactersPtr										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetCharactersPtr					]))
//#define CoreF_CFStringGetDoubleValue											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetDoubleValue						]))
//#define CoreF_CFStringGetFastestEncoding										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetFastestEncoding					]))
//#define CoreF_CFStringGetFileSystemRepresentation								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetFileSystemRepresentation			]))
//#define CoreF_CFStringGetHyphenationLocationBeforeIndex						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetHyphenationLocationBeforeIndex	]))
#define CFStringGetIntValue													((PF_CFStringGetIntValue)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetIntValue								]))
#define CFStringGetLength													((PF_CFStringGetLength)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetLength								]))
//#define CoreF_CFStringGetLineBounds											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetLineBounds							]))
//#define CoreF_CFStringGetListOfAvailableEncodings								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetListOfAvailableEncodings				]))
#define CFStringGetMaximumSizeForEncoding									((PF_CFStringGetMaximumSizeForEncoding)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetMaximumSizeForEncoding				]))
//#define CoreF_CFStringGetMaximumSizeOfFileSystemRepresentation				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetMaximumSizeOfFileSystemRepresentation]))
//#define CoreF_CFStringGetMostCompatibleMacStringEncoding						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetMostCompatibleMacStringEncoding		]))
//#define CoreF_CFStringGetNameOfEncoding										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetNameOfEncoding						]))
//#define CoreF_CFStringGetParagraphBounds										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetParagraphBounds						]))
//#define CoreF_CFStringGetPascalString											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetPascalString							]))
//#define CoreF_CFStringGetPascalStringPtr										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetPascalStringPtr						]))
//#define CoreF_CFStringGetRangeOfCharacterClusterAtIndex						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetRangeOfCharacterClusterAtIndex		]))
//#define CoreF_CFStringGetRangeOfComposedCharactersAtIndex						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetRangeOfComposedCharactersAtIndex		]))
//#define CoreF_CFStringGetSmallestEncoding										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetSmallestEncoding						]))
//#define CoreF_CFStringGetSystemEncoding										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetSystemEncoding						]))
#define CFStringGetTypeID													((PF_CFStringGetTypeID)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringGetTypeID								]))
//#define CoreF_CFStringHasPrefix												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringHasPrefix								]))
//#define CoreF_CFStringHasSuffix												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringHasSuffix								]))
//#define CoreF_CFStringHashCString												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringHashCString								]))
//#define CoreF_CFStringHashCharacters											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringHashCharacters							]))
//#define CoreF_CFStringHashISOLatin1CString									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringHashISOLatin1CString					]))
//#define CoreF_CFStringHashNSString											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringHashNSString							]))
//#define CoreF_CFStringInsert													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringInsert									]))
//#define CoreF_CFStringIsEncodingAvailable										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringIsEncodingAvailable						]))
//#define CoreF_CFStringIsHyphenationAvailableForLocale							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringIsHyphenationAvailableForLocale			]))
//#define CoreF_CFStringLowercase												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringLowercase								]))
//#define CoreF_CFStringNormalize												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringNormalize								]))
//#define CoreF_CFStringOpenUText												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringOpenUText								]))
//#define CoreF_CFStringPad														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringPad										]))
//#define CoreF_CFStringReplace													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringReplace									]))
//#define CoreF_CFStringReplaceAll												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringReplaceAll								]))
//#define CoreF_CFStringSetExternalCharactersNoCopy								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringSetExternalCharactersNoCopy				]))
//#define CoreF_CFStringTransform												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringTransform								]))
//#define CoreF_CFStringTrim													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringTrim									]))
//#define CoreF_CFStringTrimWhitespace											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringTrimWhitespace							]))
//#define CoreF_CFStringUppercase												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFStringUppercase								]))
//#define CoreF_CFTimeZoneCopyAbbreviation										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneCopyAbbreviation						]))
//#define CoreF_CFTimeZoneCopyAbbreviationDictionary							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneCopyAbbreviationDictionary			]))
//#define CoreF_CFTimeZoneCopyDefault											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneCopyDefault							]))
//#define CoreF_CFTimeZoneCopyKnownNames										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneCopyKnownNames						]))
//#define CoreF_CFTimeZoneCopyLocalizedName										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneCopyLocalizedName						]))
//#define CoreF_CFTimeZoneCopySystem											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneCopySystem							]))
//#define CoreF_CFTimeZoneCreate												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneCreate								]))
//#define CoreF_CFTimeZoneCreateWithName										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneCreateWithName						]))
//#define CoreF_CFTimeZoneCreateWithTimeIntervalFromGMT							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneCreateWithTimeIntervalFromGMT			]))
//#define CoreF_CFTimeZoneGetData												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneGetData								]))
//#define CoreF_CFTimeZoneGetDaylightSavingTimeOffset							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneGetDaylightSavingTimeOffset			]))
//#define CoreF_CFTimeZoneGetName												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneGetName								]))
//#define CoreF_CFTimeZoneGetNextDaylightSavingTimeTransition					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneGetNextDaylightSavingTimeTransition	]))
//#define CoreF_CFTimeZoneGetSecondsFromGMT										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneGetSecondsFromGMT						]))
//#define CoreF_CFTimeZoneGetTypeID												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneGetTypeID								]))
//#define CoreF_CFTimeZoneIsDaylightSavingTime									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneIsDaylightSavingTime					]))
//#define CoreF_CFTimeZoneResetSystem											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneResetSystem							]))
//#define CoreF_CFTimeZoneSetAbbreviationDictionary								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneSetAbbreviationDictionary				]))
//#define CoreF_CFTimeZoneSetDefault											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTimeZoneSetDefault							]))
//#define CoreF_CFTreeAppendChild												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeAppendChild								]))
//#define CoreF_CFTreeApplyFunctionToChildren									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeApplyFunctionToChildren					]))
//#define CoreF_CFTreeCreate													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeCreate									]))
//#define CoreF_CFTreeFindRoot													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeFindRoot									]))
//#define CoreF_CFTreeGetChildAtIndex											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeGetChildAtIndex							]))
//#define CoreF_CFTreeGetChildCount												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeGetChildCount								]))
//#define CoreF_CFTreeGetChildren												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeGetChildren								]))
//#define CoreF_CFTreeGetContext												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeGetContext								]))
//#define CoreF_CFTreeGetFirstChild												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeGetFirstChild								]))
//#define CoreF_CFTreeGetNextSibling											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeGetNextSibling							]))
//#define CoreF_CFTreeGetParent													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeGetParent									]))
//#define CoreF_CFTreeGetTypeID													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeGetTypeID									]))
//#define CoreF_CFTreeInsertSibling												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeInsertSibling								]))
//#define CoreF_CFTreePrependChild												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreePrependChild								]))
//#define CoreF_CFTreeRemove													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeRemove									]))
//#define CoreF_CFTreeRemoveAllChildren											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeRemoveAllChildren							]))
//#define CoreF_CFTreeSetContext												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeSetContext								]))
//#define CoreF_CFTreeSortChildren												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTreeSortChildren								]))
//#define CoreF_CFTypeGetTypeID													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFTypeGetTypeID									]))
//#define CoreF_CFURLCanBeDecomposed											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCanBeDecomposed							]))
//#define CoreF_CFURLCopyAbsoluteURL											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCopyFileSystemPath									]))
//#define CoreF_CFURLCopyFileSystemPath											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCopyFragment										]))
//#define CoreF_CFURLCopyFragment												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCopyHostName										]))
//#define CoreF_CFURLCopyHostName												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCopyLastPathComponent								]))
//#define CoreF_CFURLCopyLastPathComponent										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCopyNetLocation									]))
//#define CoreF_CFURLCopyNetLocation											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCopyParameterString								]))
//#define CoreF_CFURLCopyParameterString										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCopyPassword										]))
//#define CoreF_CFURLCopyPassword												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCopyPath											]))
//#define CoreF_CFURLCopyPath													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCopyPathExtension									]))
//#define CoreF_CFURLCopyPathExtension											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCopyQueryString									]))
//#define CoreF_CFURLCopyQueryString											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCopyResourceSpecifier								]))
//#define CoreF_CFURLCopyResourceSpecifier										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCopyScheme											]))
//#define CoreF_CFURLCopyScheme													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCopyStrictPath										]))
//#define CoreF_CFURLCopyStrictPath												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCopyUserName										]))
//#define CoreF_CFURLCopyUserName												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateAbsoluteURLWithBytes							]))
//#define CoreF_CFURLCreateAbsoluteURLWithBytes									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateCopyAppendingPathComponent					]))
//#define CoreF_CFURLCreateCopyAppendingPathComponent							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateCopyAppendingPathExtension					]))
//#define CoreF_CFURLCreateCopyAppendingPathExtension							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateCopyDeletingLastPathComponent				]))
//#define CoreF_CFURLCreateCopyDeletingLastPathComponent						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateCopyDeletingPathExtension					]))
//#define CoreF_CFURLCreateCopyDeletingPathExtension							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateData											]))
//#define CoreF_CFURLCreateData													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateDataAndPropertiesFromResource				]))
//#define CoreF_CFURLCreateDataAndPropertiesFromResource						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateFromFileSystemRepresentation					]))
//#define CoreF_CFURLCreateFromFileSystemRepresentation							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateFromFileSystemRepresentationRelativeToBase	]))
//#define CoreF_CFURLCreateFromFileSystemRepresentationRelativeToBase			((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreatePropertyFromResource							]))
//#define CoreF_CFURLCreatePropertyFromResource									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateStringByAddingPercentEscapes					]))
//#define CoreF_CFURLCreateStringByAddingPercentEscapes							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateStringByReplacingPercentEscapes				]))
//#define CoreF_CFURLCreateStringByReplacingPercentEscapes						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateStringByReplacingPercentEscapesUsingEncoding	]))
//#define CoreF_CFURLCreateStringByReplacingPercentEscapesUsingEncoding			((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateWithBytes									]))
//#define CoreF_CFURLCreateWithBytes											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateWithFileSystemPath							]))
//#define CoreF_CFURLCreateWithFileSystemPath									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateWithFileSystemPathRelativeToBase				]))
//#define CoreF_CFURLCreateWithFileSystemPathRelativeToBase						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLCreateWithString									]))
//#define CoreF_CFURLCreateWithString											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLDestroyResource									]))
//#define CoreF_CFURLDestroyResource											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLEnumeratorCreateForDirectoryURL					]))
//#define CoreF_CFURLEnumeratorCreateForDirectoryURL							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLEnumeratorCreateForMountedVolumes					]))
//#define CoreF_CFURLEnumeratorCreateForMountedVolumes							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLEnumeratorCreateForMountedVolumes			]))
//#define CoreF_CFURLEnumeratorGetDescendentLevel								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLEnumeratorGetDescendentLevel				]))
//#define CoreF_CFURLEnumeratorGetNextURL										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLEnumeratorGetNextURL						]))
//#define CoreF_CFURLEnumeratorGetSourceDidChange								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLEnumeratorGetSourceDidChange				]))
//#define CoreF_CFURLEnumeratorGetTypeID										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLEnumeratorGetTypeID						]))
//#define CoreF_CFURLEnumeratorSkipDescendents									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLEnumeratorSkipDescendents					]))
//#define CoreF_CFURLGetBaseURL													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLGetBaseURL									]))
//#define CoreF_CFURLGetByteRangeForComponent									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLGetByteRangeForComponent					]))
//#define CoreF_CFURLGetBytes													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLGetBytes									]))
//#define CoreF_CFURLGetFileSystemRepresentation								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLGetFileSystemRepresentation				]))
//#define CoreF_CFURLGetPortNumber												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLGetPortNumber								]))
//#define CoreF_CFURLGetString													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLGetString									]))
//#define CoreF_CFURLGetTypeID													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLGetTypeID									]))
//#define CoreF_CFURLHasDirectoryPath											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLHasDirectoryPath							]))
//#define CoreF_CFURLWriteDataAndPropertiesToResource							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFURLWriteDataAndPropertiesToResource			]))
//#define CoreF_CFUUIDCreate													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUUIDCreate									]))
//#define CoreF_CFUUIDCreateFromString											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUUIDCreateFromString							]))
//#define CoreF_CFUUIDCreateFromUUIDBytes										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUUIDCreateFromUUIDBytes						]))
//#define CoreF_CFUUIDCreateString												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUUIDCreateString								]))
//#define CoreF_CFUUIDCreateWithBytes											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUUIDCreateWithBytes							]))
//#define CoreF_CFUUIDGetConstantUUIDWithBytes									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUUIDGetConstantUUIDWithBytes					]))
//#define CoreF_CFUUIDGetTypeID													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUUIDGetTypeID									]))
//#define CoreF_CFUUIDGetUUIDBytes												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUUIDGetUUIDBytes								]))
//#define CoreF_CFUniCharCompatibilityDecompose									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharCompatibilityDecompose					]))
//#define CoreF_CFUniCharDecompose												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharDecompose								]))
//#define CoreF_CFUniCharDecomposeCharacter										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharDecomposeCharacter						]))
//#define CoreF_CFUniCharFillDestinationBuffer									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharFillDestinationBuffer					]))
//#define CoreF_CFUniCharGetBitmapForPlane										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharGetBitmapForPlane						]))
//#define CoreF_CFUniCharGetBitmapPtrForPlane									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharGetBitmapPtrForPlane					]))
//#define CoreF_CFUniCharGetConditionalCaseMappingFlags							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharGetConditionalCaseMappingFlags			]))
//#define CoreF_CFUniCharGetMappingData											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharGetMappingData							]))
//#define CoreF_CFUniCharGetNumberOfPlanes										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharGetNumberOfPlanes						]))
//#define CoreF_CFUniCharGetNumberOfPlanesForUnicodePropertyData				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharGetNumberOfPlanesForUnicodePropertyData]))
//#define CoreF_CFUniCharGetUnicodeProperty										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharGetUnicodeProperty					]))
//#define CoreF_CFUniCharGetUnicodePropertyDataForPlane							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharGetUnicodePropertyDataForPlane		]))
//#define CoreF_CFUniCharIsMemberOf												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharIsMemberOf							]))
//#define CoreF_CFUniCharMapCaseTo												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharMapCaseTo							]))
//#define CoreF_CFUniCharPrecompose												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharPrecompose							]))
//#define CoreF_CFUniCharPrecomposeCharacter									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharPrecomposeCharacter				]))
//#define CoreF_CFUniCharPrioritySort											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFUniCharPrioritySort						]))
//#define CoreF_CFWindowsNamedPipeCreateConnectToName							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWindowsNamedPipeCreateConnectToName		]))
//#define CoreF_CFWindowsNamedPipeCreateRunLoopSource							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWindowsNamedPipeCreateRunLoopSource		]))
//#define CoreF_CFWindowsNamedPipeCreateWithName								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWindowsNamedPipeCreateWithName			]))
//#define CoreF_CFWindowsNamedPipeGetContext									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWindowsNamedPipeGetContext				]))
//#define CoreF_CFWindowsNamedPipeGetHandle										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWindowsNamedPipeGetHandle					]))
//#define CoreF_CFWindowsNamedPipeGetName										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWindowsNamedPipeGetName					]))
//#define CoreF_CFWindowsNamedPipeGetTypeID										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWindowsNamedPipeGetTypeID					]))
//#define CoreF_CFWindowsNamedPipeInvalidate									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWindowsNamedPipeInvalidate				]))
//#define CoreF_CFWindowsNamedPipeIsValid										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWindowsNamedPipeIsValid					]))
//#define CoreF_CFWindowsNamedPipeSetInvalidationCallBack						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWindowsNamedPipeSetInvalidationCallBack	]))
//#define CoreF_CFWriteStreamCanAcceptBytes										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamCanAcceptBytes					]))
#define CFWriteStreamClose													((PF_CFWriteStreamClose)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamClose							]))
//#define CoreF_CFWriteStreamCopyError											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamCopyError						]))
#define CFWriteStreamCopyProperty											((PF_CFWriteStreamCopyProperty)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamCopyProperty					]))
//#define CoreF_CFWriteStreamCreate												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamCreate							]))
#define CFWriteStreamCreateWithAllocatedBuffers								((PF_CFWriteStreamCreateWithAllocatedBuffers)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamCreateWithAllocatedBuffers		]))
//#define CoreF_CFWriteStreamCreateWithBuffer									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamCreateWithBuffer				]))
//#define CoreF_CFWriteStreamCreateWithFile										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamCreateWithFile					]))
//#define CoreF_CFWriteStreamGetError											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamGetError						]))
//#define CoreF_CFWriteStreamGetInfoPointer										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamGetInfoPointer					]))
//#define CoreF_CFWriteStreamGetStatus											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamGetStatus						]))
//#define CoreF_CFWriteStreamGetTypeID											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamGetTypeID						]))
#define CFWriteStreamOpen													((PF_CFWriteStreamOpen)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamOpen							]))
//#define CoreF_CFWriteStreamScheduleWithRunLoop								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamScheduleWithRunLoop			]))
//#define CoreF_CFWriteStreamSetClient											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamSetClient					]))
//#define CoreF_CFWriteStreamSetProperty										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamSetProperty				]))
//#define CoreF_CFWriteStreamSignalEvent										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamSignalEvent				]))
//#define CoreF_CFWriteStreamUnscheduleFromRunLoop								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamUnscheduleFromRunLoop		]))
//#define CoreF_CFWriteStreamWrite												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFWriteStreamWrite						]))
//#define CoreF_CFXMLCreateStringByEscapingEntities								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLCreateStringByEscapingEntities		]))
//#define CoreF_CFXMLCreateStringByUnescapingEntities							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLCreateStringByUnescapingEntities	]))
//#define CoreF_CFXMLNodeCreate													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLNodeCreate							]))
//#define CoreF_CFXMLNodeCreateCopy												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLNodeCreateCopy						]))
//#define CoreF_CFXMLNodeGetInfoPtr												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLNodeGetInfoPtr						]))
//#define CoreF_CFXMLNodeGetString												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLNodeGetString						]))
//#define CoreF_CFXMLNodeGetTypeCode											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLNodeGetTypeCode					]))
//#define CoreF_CFXMLNodeGetTypeID												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLNodeGetTypeID						]))
//#define CoreF_CFXMLNodeGetVersion												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLNodeGetVersion						]))
//#define CoreF_CFXMLParserAbort												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLParserAbort						]))
//#define CoreF_CFXMLParserCopyErrorDescription									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLParserCopyErrorDescription			]))
//#define CoreF_CFXMLParserCreate												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLParserCreate						]))
//#define CoreF_CFXMLParserCreateWithDataFromURL								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLParserCreateWithDataFromURL		]))
//#define CoreF_CFXMLParserGetCallBacks											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLParserGetCallBacks					]))
//#define CoreF_CFXMLParserGetContext											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLParserGetContext					]))
//#define CoreF_CFXMLParserGetDocument											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLParserGetDocument					]))
//#define CoreF_CFXMLParserGetLineNumber										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLParserGetLineNumber				]))
//#define CoreF_CFXMLParserGetLocation											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLParserGetLocation					]))
//#define CoreF_CFXMLParserGetSourceURL											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLParserGetSourceURL					]))
//#define CoreF_CFXMLParserGetStatusCode										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLParserGetStatusCode				]))
//#define CoreF_CFXMLParserGetTypeID											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLParserGetTypeID					]))
//#define CoreF_CFXMLParserParse												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLParserParse						]))
//#define CoreF_CFXMLTreeCreateFromData											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLTreeCreateFromData					]))
//#define CoreF_CFXMLTreeCreateFromDataWithError								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLTreeCreateFromDataWithError		]))
//#define CoreF_CFXMLTreeCreateWithDataFromURL									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLTreeCreateWithDataFromURL			]))
//#define CoreF_CFXMLTreeCreateWithNode											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLTreeCreateWithNode						]))
//#define CoreF_CFXMLTreeCreateXMLData											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLTreeCreateXMLData						]))
//#define CoreF_CFXMLTreeGetNode												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_CFXMLTreeGetNode							]))
//#define CoreF_NSBuddhistCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSBuddhistCalendar							]))
//#define CoreF_NSCalendarIdentifierBuddhist									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSCalendarIdentifierBuddhist				]))
//#define CoreF_NSCalendarIdentifierChinese										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSCalendarIdentifierChinese					]))
//#define CoreF_NSCalendarIdentifierCoptic										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSCalendarIdentifierCoptic					]))
//#define CoreF_NSCalendarIdentifierEthiopicAmeteAlem							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSCalendarIdentifierEthiopicAmeteAlem		]))
//#define CoreF_NSCalendarIdentifierEthiopicAmeteMihret							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSCalendarIdentifierEthiopicAmeteMihret		]))
//#define CoreF_NSCalendarIdentifierGregorian									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSCalendarIdentifierGregorian				]))
//#define CoreF_NSCalendarIdentifierHebrew										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSCalendarIdentifierHebrew					]))
//#define CoreF_NSCalendarIdentifierISO8601										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSCalendarIdentifierISO8601					]))
//#define CoreF_NSCalendarIdentifierIndian										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSCalendarIdentifierIndian					]))
//#define CoreF_NSCalendarIdentifierIslamic										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSCalendarIdentifierIslamic					]))
//#define CoreF_NSCalendarIdentifierIslamicCivil								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSCalendarIdentifierIslamicCivil			]))
//#define CoreF_NSCalendarIdentifierJapanese									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSCalendarIdentifierJapanese				]))
//#define CoreF_NSCalendarIdentifierPersian										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSCalendarIdentifierPersian					]))
//#define CoreF_NSCalendarIdentifierRepublicOfChina								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSCalendarIdentifierRepublicOfChina			]))
//#define CoreF_NSChineseCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSChineseCalendar							]))
//#define CoreF_NSCurrentLocaleDidChangeNotification							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSCurrentLocaleDidChangeNotification		]))
//#define CoreF_NSDefaultRunLoopMode											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSDefaultRunLoopMode						]))
//#define CoreF_NSGenericException												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSGenericException							]))
//#define CoreF_NSGetSizeAndAlignment											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSGetSizeAndAlignment						]))
//#define CoreF_NSGregorianCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSGregorianCalendar							]))
//#define CoreF_NSHebrewCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSHebrewCalendar							]))
//#define CoreF_NSISO8601Calendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSISO8601Calendar							]))
//#define CoreF_NSIndianCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSIndianCalendar							]))
//#define CoreF_NSInternalInconsistencyException								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSInternalInconsistencyException			]))
//#define CoreF_NSInvalidArgumentException										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSInvalidArgumentException					]))
//#define CoreF_NSIslamicCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSIslamicCalendar							]))
//#define CoreF_NSIslamicCivilCalendar											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSIslamicCivilCalendar						]))
//#define CoreF_NSJapaneseCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSJapaneseCalendar							]))
//#define CoreF_NSLocaleAlternateQuotationBeginDelimiterKey						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleAlternateQuotationBeginDelimiterKey	]))
//#define CoreF_NSLocaleAlternateQuotationEndDelimiterKey						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleAlternateQuotationEndDelimiterKey	]))
//#define CoreF_NSLocaleCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleCalendar							]))
//#define CoreF_NSLocaleCalendarIdentifier										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleCalendarIdentifier					]))
//#define CoreF_NSLocaleCalendarIdentifierKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleCalendarIdentifierKey				]))
//#define CoreF_NSLocaleCalendarKey												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleCalendarKey							]))
//#define CoreF_NSLocaleCollationIdentifier										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleCollationIdentifier					]))
//#define CoreF_NSLocaleCollationIdentifierKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleCollationIdentifierKey				]))
//#define CoreF_NSLocaleCollatorIdentifier										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleCollatorIdentifier				]))
//#define CoreF_NSLocaleCollatorIdentifierKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleCollatorIdentifierKey			]))
//#define CoreF_NSLocaleCountryCode												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleCountryCode						]))
//#define CoreF_NSLocaleCountryCodeKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleCountryCodeKey					]))
//#define CoreF_NSLocaleCurrencyCode											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleCurrencyCode					]))
//#define CoreF_NSLocaleCurrencyCodeKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleCurrencyCodeKey					]))
//#define CoreF_NSLocaleCurrencySymbol											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleCurrencySymbol					]))
//#define CoreF_NSLocaleCurrencySymbolKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleCurrencySymbolKey				]))
//#define CoreF_NSLocaleDecimalSeparator										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleDecimalSeparator				]))
//#define CoreF_NSLocaleDecimalSeparatorKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleDecimalSeparatorKey				]))
//#define CoreF_NSLocaleExemplarCharacterSet									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleExemplarCharacterSet			]))
//#define CoreF_NSLocaleExemplarCharacterSetKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleExemplarCharacterSetKey			]))
//#define CoreF_NSLocaleGroupingSeparator										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleGroupingSeparator				]))
//#define CoreF_NSLocaleGroupingSeparatorKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleGroupingSeparatorKey			]))
//#define CoreF_NSLocaleIdentifier												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleIdentifier						]))
//#define CoreF_NSLocaleIdentifierKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleIdentifierKey					]))
//#define CoreF_NSLocaleLanguageCode											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleLanguageCode					]))
//#define CoreF_NSLocaleLanguageCodeKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleLanguageCodeKey					]))
//#define CoreF_NSLocaleMeasurementSystem										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleMeasurementSystem				]))
//#define CoreF_NSLocaleMeasurementSystemKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleMeasurementSystemKey			]))
//#define CoreF_NSLocaleQuotationBeginDelimiterKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleQuotationBeginDelimiterKey		]))
//#define CoreF_NSLocaleQuotationEndDelimiterKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleQuotationEndDelimiterKey		]))
//#define CoreF_NSLocaleScriptCode												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleScriptCode						]))
//#define CoreF_NSLocaleScriptCodeKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleScriptCodeKey					]))
//#define CoreF_NSLocaleUsesMetricSystem										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleUsesMetricSystem				]))
//#define CoreF_NSLocaleUsesMetricSystemKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleUsesMetricSystemKey				]))
//#define CoreF_NSLocaleVariantCode												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleVariantCode						]))
//#define CoreF_NSLocaleVariantCodeKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSLocaleVariantCodeKey					]))
//#define CoreF_NSMallocException												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSMallocException						]))
//#define CoreF_NSPersianCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSPersianCalendar						]))
//#define CoreF_NSRangeException												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSRangeException						]))
//#define CoreF_NSRepublicOfChinaCalendar										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSRepublicOfChinaCalendar				]))
//#define CoreF_NSRunLoopCommonModes											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSRunLoopCommonModes					]))
//#define CoreF_NSStreamDataWrittenToMemoryStreamKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSStreamDataWrittenToMemoryStreamKey	]))
//#define CoreF_NSStreamFileCurrentOffsetKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSStreamFileCurrentOffsetKey			]))
//#define CoreF_NSSystemClockDidChangeNotification								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSSystemClockDidChangeNotification		]))
//#define CoreF_NSSystemTimeZoneDidChangeNotificatio							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_NSSystemTimeZoneDidChangeNotificatio	]))
//#define CoreF_OSAtomicAdd32													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_OSAtomicAdd32							]))
//#define CoreF_OSAtomicAdd32Barrier											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_OSAtomicAdd32Barrier					]))
//#define CoreF_OSAtomicCompareAndSwap32Barrier									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_OSAtomicCompareAndSwap32Barrier			]))
//#define CoreF_OSAtomicCompareAndSwapLong										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_OSAtomicCompareAndSwapLong				]))
//#define CoreF_OSAtomicCompareAndSwapPtr										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_OSAtomicCompareAndSwapPtr				]))
//#define CoreF_OSAtomicCompareAndSwapPtrBarrier								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_OSAtomicCompareAndSwapPtrBarrier		]))
//#define CoreF_OSAtomicDecrement32												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_OSAtomicDecrement32						]))
//#define CoreF_OSAtomicDecrement32Barrier										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_OSAtomicDecrement32Barrier				]))
//#define CoreF_OSAtomicIncrement32												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_OSAtomicIncrement32						]))
//#define CoreF_OSAtomicIncrement32Barrier										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_OSAtomicIncrement32Barrier						]))
//#define CoreF__Block_copy														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__Block_copy										]))
//#define CoreF__Block_object_assign											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__Block_object_assign							]))
//#define CoreF__Block_object_dispose											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__Block_object_dispose							]))
//#define CoreF__Block_release													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__Block_release									]))
//#define CoreF__CFAllocatorAllocateGC											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAllocatorAllocateGC							]))
//#define CoreF__CFAllocatorDeallocateGC										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAllocatorDeallocateGC						]))
//#define CoreF__CFAllocatorReallocateGC										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAllocatorReallocateGC						]))
//#define CoreF__CFAppVersionCheck												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAppVersionCheck								]))
//#define CoreF__CFAppVersionCheckLessThan										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAppVersionCheckLessThan						]))
//#define CoreF__CFAppWindowsVersionCheck										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAppWindowsVersionCheck						]))
//#define CoreF__CFAppWindowsVersionCheckLessThan								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAppWindowsVersionCheckLessThan				]))
//#define CoreF__CFAppendPathComponent											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAppendPathComponent							]))
//#define CoreF__CFAppendPathExtension											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAppendPathExtension							]))
//#define CoreF__CFArrayCheckAndGetValueAtIndex									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFArrayCheckAndGetValueAtIndex					]))
//#define CoreF__CFArrayFastEnumeration											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFArrayFastEnumeration							]))
//#define CoreF__CFArrayIsMutable												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFArrayIsMutable								]))
//#define CoreF__CFArrayReplaceValues											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFArrayReplaceValues							]))
//#define CoreF__CFArraySetCapacity												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFArraySetCapacity								]))
//#define CoreF__CFAttributedStringCheckAndReplace								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAttributedStringCheckAndReplace				]))
//#define CoreF__CFAttributedStringCheckAndReplaceAttributed					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAttributedStringCheckAndReplaceAttributed	]))
//#define CoreF__CFAttributedStringCheckAndSetAttribute							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAttributedStringCheckAndSetAttribute			]))
//#define CoreF__CFAttributedStringCheckAndSetAttributes						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAttributedStringCheckAndSetAttributes		]))
//#define CoreF__CFAttributedStringGetLength									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAttributedStringGetLength					]))
//#define CoreF__CFAutoreleasePoolAddObject										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAutoreleasePoolAddObject						]))
//#define CoreF__CFAutoreleasePoolPop											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAutoreleasePoolPop							]))
//#define CoreF__CFAutoreleasePoolPrintPools									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAutoreleasePoolPrintPools					]))
//#define CoreF__CFAutoreleasePoolPush											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFAutoreleasePoolPush							]))
//#define CoreF__CFBagFastEnumeration											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBagFastEnumeration							]))
//#define CoreF__CFBagGetKVOBit													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBagGetKVOBit									]))
//#define CoreF__CFBagIsMutable													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBagIsMutable									]))
//#define CoreF__CFBagSetCapacity												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBagSetCapacity								]))
//#define CoreF__CFBagSetKVOBit													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBagSetKVOBit									]))
//#define CoreF__CFBundleCopyAllBundles											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyAllBundles							]))
//#define CoreF__CFBundleCopyBuiltInPlugInsURL									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyBuiltInPlugInsURL					]))
//#define CoreF__CFBundleCopyBundleRegionsArray									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyBundleRegionsArray					]))
//#define CoreF__CFBundleCopyBundleURLForExecutableURL							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyBundleURLForExecutableURL			]))
//#define CoreF__CFBundleCopyExecutableURLInDirectory							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyExecutableURLInDirectory			]))
//#define CoreF__CFBundleCopyFileTypeForFileData								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyFileTypeForFileData				]))
//#define CoreF__CFBundleCopyFileTypeForFileURL									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyFileTypeForFileURL					]))
//#define CoreF__CFBundleCopyFrameworkURLForExecutablePath						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyFrameworkURLForExecutablePath		]))
//#define CoreF__CFBundleCopyInfoPlistURL										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyInfoPlistURL						]))
//#define CoreF__CFBundleCopyLanguageSearchListInDirectory						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyLanguageSearchListInDirectory		]))
//#define CoreF__CFBundleCopyMainBundleExecutableURL							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyMainBundleExecutableURL			]))
//#define CoreF__CFBundleCopyOtherExecutableURLInDirectory						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyOtherExecutableURLInDirectory		]))
//#define CoreF__CFBundleCopyPrivateFrameworksURL								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyPrivateFrameworksURL						]))
//#define CoreF__CFBundleCopyResourceForkURL									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyResourceForkURL							]))
//#define CoreF__CFBundleCopyResourceURLForLanguage								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyResourceURLForLanguage						]))
//#define CoreF__CFBundleCopyResourceURLsOfTypeForLanguage						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopyResourceURLsOfTypeForLanguage				]))
//#define CoreF__CFBundleCopySharedFrameworksURL								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopySharedFrameworksURL						]))
//#define CoreF__CFBundleCopySharedSupportURL									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCopySharedSupportURL							]))
//#define CoreF__CFBundleCreateError											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCreateError									]))
//#define CoreF__CFBundleCreateIfLooksLikeBundle								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCreateIfLooksLikeBundle						]))
//#define CoreF__CFBundleCreateIfMightBeBundle									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCreateIfMightBeBundle							]))
//#define CoreF__CFBundleCreateWithExecutableURLIfLooksLikeBundle				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCreateWithExecutableURLIfLooksLikeBundle		]))
//#define CoreF__CFBundleCreateWithExecutableURLIfMightBeBundle					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleCreateWithExecutableURLIfMightBeBundle			]))
//#define CoreF__CFBundleFlushBundleCaches										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleFlushBundleCaches								]))
//#define CoreF__CFBundleFlushCaches											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleFlushCaches									]))
//#define CoreF__CFBundleFlushCachesForURL										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleFlushCachesForURL								]))
//#define CoreF__CFBundleGetCFMFunctionPointerForName							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleGetCFMFunctionPointerForName					]))
//#define CoreF__CFBundleGetCFMFunctionPointersForNames							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleGetCFMFunctionPointersForNames					]))
//#define CoreF__CFBundleGetCurrentPlatform										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleGetCurrentPlatform								]))
//#define CoreF__CFBundleGetExistingBundleWithBundleURL							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleGetExistingBundleWithBundleURL					]))
//#define CoreF__CFBundleGetHasChanged											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleGetHasChanged									]))
//#define CoreF__CFBundleGetLanguageAndRegionCodes								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleGetLanguageAndRegionCodes						]))
//#define CoreF__CFBundleGetLanguageSearchList									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleGetLanguageSearchList							]))
//#define CoreF__CFBundleGetLocalInfoDictionary									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleGetLocalInfoDictionary							]))
//#define CoreF__CFBundleGetMainBundleIfLooksLikeBundle							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleGetMainBundleIfLooksLikeBundle					]))
//#define CoreF__CFBundleGetPackageInfoInDirectory								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleGetPackageInfoInDirectory						]))
//#define CoreF__CFBundleGetStringsFilesShared									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleGetStringsFilesShared							]))
//#define CoreF__CFBundleGetSupportedPlatforms									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleGetSupportedPlatforms							]))
//#define CoreF__CFBundleGetValueForInfoKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleGetValueForInfoKey								]))
//#define CoreF__CFBundleLayoutVersion											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleLayoutVersion									]))
//#define CoreF__CFBundleLoadExecutableAndReturnError							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleLoadExecutableAndReturnError					]))
//#define CoreF__CFBundleMainBundleInfoDictionaryComesFromResourceFork			((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleMainBundleInfoDictionaryComesFromResourceFork	]))
//#define CoreF__CFBundleSetCFMConnectionID										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleSetCFMConnectionID								]))
//#define CoreF__CFBundleSetDefaultLocalization									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleSetDefaultLocalization							]))
//#define CoreF__CFBundleSetStringsFilesShared									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleSetStringsFilesShared							]))
//#define CoreF__CFBundleURLLooksLikeBundle										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFBundleURLLooksLikeBundle								]))
//#define CoreF__CFCalendarAddComponentsV										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCalendarAddComponentsV								]))
//#define CoreF__CFCalendarComposeAbsoluteTimeV									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCalendarComposeAbsoluteTimeV							]))
//#define CoreF__CFCalendarDecomposeAbsoluteTimeV								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCalendarDecomposeAbsoluteTimeV						]))
//#define CoreF__CFCalendarGetComponentDifferenceV								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCalendarGetComponentDifferenceV						]))
//#define CoreF__CFCharacterSetCreateKeyedCodingString							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCharacterSetCreateKeyedCodingString					]))
//#define CoreF__CFCharacterSetGetKeyedCodingBuiltinType						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCharacterSetGetKeyedCodingBuiltinType				]))
//#define CoreF__CFCharacterSetGetKeyedCodingRange								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCharacterSetGetKeyedCodingRange						]))
//#define CoreF__CFCharacterSetGetKeyedCodingType								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCharacterSetGetKeyedCodingType						]))
//#define CoreF__CFCharacterSetIsInverted										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCharacterSetIsInverted								]))
//#define CoreF__CFCharacterSetIsMutable										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCharacterSetIsMutable								]))
//#define CoreF__CFCharacterSetSetIsInverted									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCharacterSetSetIsInverted							]))
//#define CoreF__CFContentsOfDirectory											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFContentsOfDirectory									]))
//#define CoreF__CFCopyExtensionForAbstractType									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCopyExtensionForAbstractType							]))
//#define CoreF__CFCopyServerVersionDictionary									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCopyServerVersionDictionary							]))
//#define CoreF__CFCopySystemVersionDictionary									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCopySystemVersionDictionary							]))
//#define CoreF__CFCreateApplicationRepositoryPath								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCreateApplicationRepositoryPath						]))
//#define CoreF__CFCreateDirectory												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCreateDirectory										]))
//#define CoreF__CFCreateDirectoryWide											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFCreateDirectoryWide									]))
//#define CoreF__CFDNStart														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFDNStart												]))
//#define CoreF__CFDataFindBytes												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFDataFindBytes										]))
//#define CoreF__CFDeleteFile													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFDeleteFile											]))
//#define CoreF__CFDictionaryFastEnumeration									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFDictionaryFastEnumeration							]))
//#define CoreF__CFDictionaryGetKVOBit											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFDictionaryGetKVOBit									]))
//#define CoreF__CFDictionaryIsMutable											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFDictionaryIsMutable									]))
//#define CoreF__CFDictionarySetCapacity										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFDictionarySetCapacity								]))
//#define CoreF__CFDictionarySetKVOBit											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFDictionarySetKVOBit									]))
//#define CoreF__CFDoExceptionOperation											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFDoExceptionOperation									]))
//#define CoreF__CFErrorCreateDebugDescription									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFErrorCreateDebugDescription							]))
//#define CoreF__CFErrorCreateLocalizedDescription								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFErrorCreateLocalizedDescription						]))
//#define CoreF__CFErrorCreateLocalizedFailureReason							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFErrorCreateLocalizedFailureReason					]))
//#define CoreF__CFErrorCreateLocalizedRecoverySuggestion						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFErrorCreateLocalizedRecoverySuggestion				]))
//#define CoreF__CFExecutableLinkedOnOrAfter									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFExecutableLinkedOnOrAfter						]))
//#define CoreF__CFFindBundleResources											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFFindBundleResources								]))
//#define CoreF__CFGetCurrentDirectory											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFGetCurrentDirectory								]))
//#define CoreF__CFGetFileProperties											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFGetFileProperties								]))
//#define CoreF__CFGetOutOfMemoryErrorCallBack									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFGetOutOfMemoryErrorCallBack						]))
//#define CoreF__CFGetPathProperties											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFGetPathProperties								]))
//#define CoreF__CFGetProcessPath												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFGetProcessPath									]))
//#define CoreF__CFGetProgname													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFGetProgname										]))
//#define CoreF__CFGetTSD														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFGetTSD											]))
//#define CoreF__CFGetWindowsAppleAppDataDirectory								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFGetWindowsAppleAppDataDirectory					]))
//#define CoreF__CFGetWindowsAppleSystemLibraryDirectory						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFGetWindowsAppleSystemLibraryDirectory			]))
//#define CoreF__CFGetWindowsBinaryDirectories									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFGetWindowsBinaryDirectories						]))
//#define CoreF__CFHyphenationGetAllPossibleHyphenationLocations				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFHyphenationGetAllPossibleHyphenationLocations	]))
//#define CoreF__CFHyphenationGetLinguisticDataPath								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFHyphenationGetLinguisticDataPath					]))
//#define CoreF__CFHyphenationSetLinguisticDataPath								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFHyphenationSetLinguisticDataPath					]))
//#define CoreF__CFIsAbsolutePath												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFIsAbsolutePath									]))
//#define CoreF__CFIsDeallocating												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFIsDeallocating									]))
//#define CoreF__CFKeyedArchiverUIDCreate										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFKeyedArchiverUIDCreate							]))
//#define CoreF__CFKeyedArchiverUIDGetTypeID									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFKeyedArchiverUIDGetTypeID						]))
//#define CoreF__CFKeyedArchiverUIDGetValue										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFKeyedArchiverUIDGetValue							]))
//#define CoreF__CFLengthAfterDeletingLastPathComponent							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFLengthAfterDeletingLastPathComponent				]))
//#define CoreF__CFLengthAfterDeletingPathExtension								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFLengthAfterDeletingPathExtension					]))
//#define CoreF__CFLocaleGetNoteCount											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFLocaleGetNoteCount								]))
//#define CoreF__CFLogvEx														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFLogvEx											]))
//#define CoreF__CFMainPThread													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFMainPThread										]))
//#define CoreF__CFNotificationCenterSetRunLoop									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFNotificationCenterSetRunLoop						]))
//#define CoreF__CFNumberGetType2												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFNumberGetType2									]))
//#define CoreF__CFPreferencesAlwaysUseVolatileUserDomains						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFPreferencesAlwaysUseVolatileUserDomains			]))
//#define CoreF__CFPreferencesCreatePathToUsersManagedPrefsDirectory			((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFPreferencesCreatePathToUsersManagedPrefsDirectory]))
//#define CoreF__CFPreferencesIsManaged											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFPreferencesIsManaged								]))
//#define CoreF__CFPreferencesManagementStatusChangedForDomains					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFPreferencesManagementStatusChangedForDomains		]))
//#define CoreF__CFPreferencesPostValuesChangedInDomains						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFPreferencesPostValuesChangedInDomains			]))
//#define CoreF__CFPreferencesWriteDomainDictionaryToPath						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFPreferencesWriteDomainDictionaryToPath			]))
//#define CoreF__CFPreferencesWriteManagedDomainForUser							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFPreferencesWriteManagedDomainForUser				]))
//#define CoreF__CFProcessNameString											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFProcessNameString								]))
//#define CoreF__CFProcessPath													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFProcessPath										]))
//#define CoreF__CFPropertyListCreateFromXMLData								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFPropertyListCreateFromXMLData					]))
//#define CoreF__CFPropertyListCreateFromXMLString								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFPropertyListCreateFromXMLString					]))
//#define CoreF__CFPropertyListCreateSingleValue								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFPropertyListCreateSingleValue					]))
//#define CoreF__CFPropertyListCreateXMLDataWithExtras							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFPropertyListCreateXMLDataWithExtras				]))
//#define CoreF__CFReadBytesFromFile											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFReadBytesFromFile									]))
//#define CoreF__CFReadStreamClearEvent											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFReadStreamClearEvent									]))
//#define CoreF__CFReadStreamCopyRunLoopsAndModes								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFReadStreamCopyRunLoopsAndModes						]))
//#define CoreF__CFReadStreamCreateFromFileDescriptor							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFReadStreamCreateFromFileDescriptor					]))
//#define CoreF__CFReadStreamGetClient											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFReadStreamGetClient									]))
//#define CoreF__CFReadStreamGetRunLoopsAndModes								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFReadStreamGetRunLoopsAndModes						]))
//#define CoreF__CFReadStreamSignalEventDelayed									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFReadStreamSignalEventDelayed							]))
//#define CoreF__CFRemoveDirectory												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRemoveDirectory										]))
//#define CoreF__CFRunLoop01													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRunLoop01											]))
//#define CoreF__CFRunLoopFinished												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRunLoopFinished										]))
//#define CoreF__CFRunLoopGet0													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRunLoopGet0											]))
//#define CoreF__CFRunLoopGet0b													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRunLoopGet0b											]))
//#define CoreF__CFRunLoopGet2													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRunLoopGet2											]))
//#define CoreF__CFRunLoopGet2b													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRunLoopGet2b											]))
//#define CoreF__CFRunLoopGetWindowsMessageQueueHandler							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRunLoopGetWindowsMessageQueueHandler					]))
//#define CoreF__CFRunLoopGetWindowsMessageQueueMask							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRunLoopGetWindowsMessageQueueMask					]))
//#define CoreF__CFRunLoopGetWindowsThreadID									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRunLoopGetWindowsThreadID							]))
//#define CoreF__CFRunLoopModeContainsMode										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRunLoopModeContainsMode								]))
//#define CoreF__CFRunLoopSetWindowsMessageQueueHandler							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRunLoopSetWindowsMessageQueueHandler					]))
//#define CoreF__CFRunLoopSetWindowsMessageQueueMask							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRunLoopSetWindowsMessageQueueMask					]))
//#define CoreF__CFRunLoopStopMode												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRunLoopStopMode										]))
//#define CoreF__CFRuntimeCreateInstance										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRuntimeCreateInstance								]))
//#define CoreF__CFRuntimeGetClassWithTypeID									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRuntimeGetClassWithTypeID							]))
//#define CoreF__CFRuntimeInitStaticInstance									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRuntimeInitStaticInstance							]))
//#define CoreF__CFRuntimeRegisterClass											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRuntimeRegisterClass									]))
//#define CoreF__CFRuntimeSetCFMPresent											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRuntimeSetCFMPresent									]))
//#define CoreF__CFRuntimeSetInstanceTypeID										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRuntimeSetInstanceTypeID								]))
//#define CoreF__CFRuntimeUnregisterClassWithTypeID								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFRuntimeUnregisterClassWithTypeID						]))
//#define CoreF__CFSetFastEnumeration											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFSetFastEnumeration									]))
//#define CoreF__CFSetGetKVOBit													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFSetGetKVOBit											]))
//#define CoreF__CFSetIsMutable													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFSetIsMutable											]))
//#define CoreF__CFSetOutOfMemoryErrorCallBack									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFSetOutOfMemoryErrorCallBack							]))
//#define CoreF__CFSetSetCapacity												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFSetSetCapacity										]))
//#define CoreF__CFSetSetKVOBit													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFSetSetKVOBit											]))
//#define CoreF__CFSetTSD														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFSetTSD												]))
//#define CoreF__CFSocketStreamSetAuthenticatesServerCertificateDefault			((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFSocketStreamSetAuthenticatesServerCertificateDefault	]))
//#define CoreF__CFStartOfLastPathComponent										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStartOfLastPathComponent					]))
//#define CoreF__CFStartOfPathExtension											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStartOfPathExtension						]))
//#define CoreF__CFStreamCreateWithConstantCallbacks							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStreamCreateWithConstantCallbacks		]))
//#define CoreF__CFStreamInstanceSize											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStreamInstanceSize						]))
//#define CoreF__CFStreamSetInfoPointer											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStreamSetInfoPointer						]))
//#define CoreF__CFStreamSourceScheduleWithAllRunLoops							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStreamSourceScheduleWithAllRunLoops		]))
//#define CoreF__CFStreamSourceScheduleWithRunLoop								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStreamSourceScheduleWithRunLoop			]))
//#define CoreF__CFStreamSourceUncheduleFromAllRunLoops							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStreamSourceUncheduleFromAllRunLoops		]))
//#define CoreF__CFStreamSourceUnscheduleFromRunLoop							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStreamSourceUnscheduleFromRunLoop		]))
//#define CoreF__CFStringAppendFormatAndArgumentsAux							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStringAppendFormatAndArgumentsAux		]))
//#define CoreF__CFStringCheckAndGetCharacterAtIndex							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStringCheckAndGetCharacterAtIndex		]))
//#define CoreF__CFStringCheckAndGetCharacters									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStringCheckAndGetCharacters				]))
//#define CoreF__CFStringCreateHostName											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStringCreateHostName						]))
//#define CoreF__CFStringCreateWithBytesNoCopy									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStringCreateWithBytesNoCopy				]))
//#define CoreF__CFStringCreateWithFormatAndArgumentsAux						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStringCreateWithFormatAndArgumentsAux	]))
//#define CoreF__CFStringEncodingSetForceASCIICompatibility						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStringEncodingSetForceASCIICompatibility	]))
//#define CoreF__CFStringGetFileSystemRepresentation							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStringGetFileSystemRepresentation		]))
//#define CoreF__CFStringGetLength2												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStringGetLength2							]))
//#define CoreF__CFStringHyphenationCompileHyphenatorData						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStringHyphenationCompileHyphenatorData	]))
//#define CoreF__CFStringIsLegalURLString										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStringIsLegalURLString					]))
//#define CoreF__CFStripTrailingPathSlashes										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFStripTrailingPathSlashes					]))
//#define CoreF__CFTimeZoneGetNoteCount											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFTimeZoneGetNoteCount						]))
//#define CoreF__CFTransmutePathSlashes											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFTransmutePathSlashes						]))
//#define CoreF__CFTryRetain													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFTryRetain								]))
//#define CoreF__CFURLAlloc														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFURLAlloc									]))
//#define CoreF__CFURLCopyComponents											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFURLCopyComponents						]))
//#define CoreF__CFURLCopyPropertyListRepresentation							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFURLCopyPropertyListRepresentation		]))
//#define CoreF__CFURLCreateCurrentDirectoryURL									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFURLCreateCurrentDirectoryURL				]))
//#define CoreF__CFURLCreateFromComponents										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFURLCreateFromComponents					]))
//#define CoreF__CFURLCreateFromPropertyListRepresentation						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFURLCreateFromPropertyListRepresentation	]))
//#define CoreF__CFURLCreateOnlyUTF8CompatibleURLs								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFURLCreateOnlyUTF8CompatibleURLs			]))
//#define CoreF__CFURLGetEncoding												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFURLGetEncoding							]))
//#define CoreF__CFURLGetWideFileSystemRepresentation							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFURLGetWideFileSystemRepresentation		]))
//#define CoreF__CFURLInitFSPath												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFURLInitFSPath							]))
//#define CoreF__CFURLInitWithString											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFURLInitWithString						]))
//#define CoreF__CFWriteBytesToFile												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFWriteBytesToFile							]))
//#define CoreF__CFWriteStreamCopyRunLoopsAndModes								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFWriteStreamCopyRunLoopsAndModes			]))
//#define CoreF__CFWriteStreamCreateFromFileDescriptor							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFWriteStreamCreateFromFileDescriptor					]))
//#define CoreF__CFWriteStreamGetClient											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFWriteStreamGetClient									]))
//#define CoreF__CFWriteStreamGetRunLoopsAndModes								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFWriteStreamGetRunLoopsAndModes						]))
//#define CoreF__CFWriteStreamSignalEventDelayed								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFWriteStreamSignalEventDelayed						]))
//#define CoreF__CFXNotificationCenterCreate									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXNotificationCenterCreate							]))
//#define CoreF__CFXNotificationCenterIsEmpty									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXNotificationCenterIsEmpty							]))
//#define CoreF__CFXNotificationGetHostCenter									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXNotificationGetHostCenter							]))
//#define CoreF__CFXNotificationGetSuspended									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXNotificationGetSuspended							]))
//#define CoreF__CFXNotificationGetTaskCenter									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXNotificationGetTaskCenter							]))
//#define CoreF__CFXNotificationPost											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXNotificationPost									]))
//#define CoreF__CFXNotificationRegisterObserver								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXNotificationRegisterObserver						]))
//#define CoreF__CFXNotificationRemoveObservers									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXNotificationRemoveObservers							]))
//#define CoreF__CFXNotificationSetSuspended									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXNotificationSetSuspended							]))
//#define CoreF__CFXPreferencesCopyDictionaryForApplicationPreferences			((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXPreferencesCopyDictionaryForApplicationPreferences	]))
//#define CoreF__CFXPreferencesCopyDictionaryForNamedVolatileSource				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXPreferencesCopyDictionaryForNamedVolatileSource		]))
//#define CoreF__CFXPreferencesCopyDictionaryForSourceWithBundleID				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXPreferencesCopyDictionaryForSourceWithBundleID		]))
//#define CoreF__CFXPreferencesCopyLanguageList									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXPreferencesCopyLanguageList							]))
//#define CoreF__CFXPreferencesCopyVolatileSourceNames							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXPreferencesCopyVolatileSourceNames					]))
//#define CoreF__CFXPreferencesGetByHostIdentifierString						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXPreferencesGetByHostIdentifierString				]))
//#define CoreF__CFXPreferencesRegisterDefaultValues							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXPreferencesRegisterDefaultValues					]))
//#define CoreF__CFXPreferencesRemoveNamedVolatileSource						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXPreferencesRemoveNamedVolatileSource				]))
//#define CoreF__CFXPreferencesReplaceValuesInNamedVolatileSource				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXPreferencesReplaceValuesInNamedVolatileSource		]))
//#define CoreF__CFXPreferencesReplaceValuesInSourceWithBundleID				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXPreferencesReplaceValuesInSourceWithBundleID		]))
//#define CoreF__CFXPreferencesSetUserDefaultsPointer							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CFXPreferencesSetUserDefaultsPointer					]))
//#define CoreF__CF_forwarding_prep_0											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__CF_forwarding_prep_0									]))
//#define CoreF__NSConcreteGlobalBlock											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NSConcreteGlobalBlock									]))
//#define CoreF__NSConcreteStackBlock											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NSConcreteStackBlock									]))
//#define CoreF__NSMessageBuilder												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NSMessageBuilder										]))
//#define CoreF__NS_access														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NS_access												]))
//#define CoreF__NS_chdir														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NS_chdir												]))
//#define CoreF__NS_chmod														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NS_chmod												]))
//#define CoreF__NS_getcwd														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NS_getcwd												]))
//#define CoreF__NS_getenv														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NS_getenv												]))
//#define CoreF__NS_mkdir														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NS_mkdir												]))
//#define CoreF__NS_mkstemp														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NS_mkstemp												]))
//#define CoreF__NS_open														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NS_open												]))
//#define CoreF__NS_pthread_main_np												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NS_pthread_main_np								]))
//#define CoreF__NS_pthread_setname_np											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NS_pthread_setname_np							]))
//#define CoreF__NS_rename														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NS_rename										]))
//#define CoreF__NS_rmdir														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NS_rmdir										]))
//#define CoreF__NS_stat														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NS_stat										]))
//#define CoreF__NS_unlink														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__NS_unlink										]))
//#define CoreF___CFArgStuff													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFArgStuff									]))
//#define CoreF___CFAttributedStringMtbl										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFAttributedStringMtbl						]))
//#define CoreF___CFBinaryPlistCreateObject										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFBinaryPlistCreateObject						]))
//#define CoreF___CFBinaryPlistCreateObject2									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFBinaryPlistCreateObject2					]))
//#define CoreF___CFBinaryPlistGetOffsetForValueFromArray2						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFBinaryPlistGetOffsetForValueFromArray2		]))
//#define CoreF___CFBinaryPlistGetOffsetForValueFromDictionary2					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFBinaryPlistGetOffsetForValueFromDictionary2	]))
//#define CoreF___CFBinaryPlistGetOffsetForValueFromDictionary3					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFBinaryPlistGetOffsetForValueFromDictionary3	]))
//#define CoreF___CFBinaryPlistGetTopLevelInfo									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFBinaryPlistGetTopLevelInfo					]))
//#define CoreF___CFBinaryPlistWrite											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFBinaryPlistWrite							]))
//#define CoreF___CFBinaryPlistWriteToStream									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFBinaryPlistWriteToStream					]))
//#define CoreF___CFBinaryPlistWriteToStreamWithEstimate						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFBinaryPlistWriteToStreamWithEstimate		]))
//#define CoreF___CFBinaryPlistWriteToStreamWithOptions							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFBinaryPlistWriteToStreamWithOptions			]))
//#define CoreF___CFCharToUniCharFunc											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFCharToUniCharFunc							]))
//#define CoreF___CFConstantStringClassReference								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFConstantStringClassReference				]))
//#define CoreF___CFConstantStringClassReferencePtr								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFConstantStringClassReferencePtr				]))
//#define CoreF___CFDataMtbl													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFDataMtbl									]))
//#define CoreF___CFDefaultEightBitStringEncoding								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFDefaultEightBitStringEncoding				]))
//#define CoreF___CFDoExternRefOperation										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFDoExternRefOperation						]))
//#define CoreF___CFGetNextSearchPathEnumeration								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFGetNextSearchPathEnumeration				]))
//#define CoreF___CFInitialize													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFInitialize									]))
//#define CoreF___CFOASafe														((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFOASafe										]))
//#define CoreF___CFPreferencesSetPathHackEnabled								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFPreferencesSetPathHackEnabled				]))
#define CFRangeMake															((PF_CFRangeMake)(__gpf_CoreFoundation_dll_ri[ENUM___CFRangeMake									]))
//#define CoreF___CFSetLastAllocationEventName									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFSetLastAllocationEventName				]))
//#define CoreF___CFSocketInitializeWinSock										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFSocketInitializeWinSock					]))
//#define CoreF___CFStartSearchPathEnumeration									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStartSearchPathEnumeration				]))
//#define CoreF___CFStorageGetCapacity											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStorageGetCapacity						]))
//#define CoreF___CFStorageGetValueSize											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStorageGetValueSize						]))
//#define CoreF___CFStorageSetAlwaysFrozen										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStorageSetAlwaysFrozen					]))
//#define CoreF___CFStringAppendBytes											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStringAppendBytes						]))
//#define CoreF___CFStringCheckAndReplace										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStringCheckAndReplace					]))
//#define CoreF___CFStringComputeEightBitStringEncoding							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStringComputeEightBitStringEncoding		]))
//#define CoreF___CFStringCreateImmutableFunnel2								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStringCreateImmutableFunnel2			]))
//#define CoreF___CFStringDecodeByteStream2										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStringDecodeByteStream2					]))
//#define CoreF___CFStringDecodeByteStream3										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStringDecodeByteStream3					]))
//#define CoreF___CFStringEncodeByteStream										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStringEncodeByteStream					]))
//#define CoreF___CFStringHash													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStringHash								]))
//#define CoreF___CFStringInitializeSystemEncoding								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStringInitializeSystemEncoding			]))
//#define CoreF___CFStringIsEightBit											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStringIsEightBit						]))
#define __CFStringMakeConstantString										((PF___CFStringMakeConstantString)(__gpf_CoreFoundation_dll_ri[ENUM___CFStringMakeConstantString				]))
//#define CoreF___CFStringMtbl													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStringMtbl								]))
//#define CoreF___CFStringNoteErrors											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFStringNoteErrors						]))
//#define CoreF___CFURLReservedPtr												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFURLReservedPtr							]))
//#define CoreF___CFURLResourceInfoPtr											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFURLResourceInfoPtr						]))
//#define CoreF___CFURLSetReservedPtr											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFURLSetReservedPtr						]))
//#define CoreF___CFURLSetResourceInfoPtr										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM___CFURLSetResourceInfoPtr					]))
//#define CoreF__kCFBundleAllowMixedLocalizationsKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleAllowMixedLocalizationsKey		]))
//#define CoreF__kCFBundleDisplayNameKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleDisplayNameKey					]))
//#define CoreF__kCFBundleDocumentTypesKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleDocumentTypesKey					]))
//#define CoreF__kCFBundleExecutablePathKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleExecutablePathKey					]))
//#define CoreF__kCFBundleGetInfoHTMLKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleGetInfoHTMLKey					]))
//#define CoreF__kCFBundleGetInfoStringKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleGetInfoStringKey					]))
//#define CoreF__kCFBundleIconFileKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleIconFileKey						]))
//#define CoreF__kCFBundleInfoPlistURLKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleInfoPlistURLKey					]))
//#define CoreF__kCFBundleInitialPathKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleInitialPathKey					]))
//#define CoreF__kCFBundleNumericVersionKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleNumericVersionKey					]))
//#define CoreF__kCFBundleOldDocumentTypesKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleOldDocumentTypesKey				]))
//#define CoreF__kCFBundleOldExecutableKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleOldExecutableKey					]))
//#define CoreF__kCFBundleOldIconFileKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleOldIconFileKey					]))
//#define CoreF__kCFBundleOldInfoDictionaryVersionKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleOldInfoDictionaryVersionKey		]))
//#define CoreF__kCFBundleOldNameKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleOldNameKey						]))
//#define CoreF__kCFBundleOldShortVersionStringKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleOldShortVersionStringKey			]))
//#define CoreF__kCFBundleOldTypeExtensions1Key									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleOldTypeExtensions1Key				]))
//#define CoreF__kCFBundleOldTypeExtensions2Key									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleOldTypeExtensions2Key				]))
//#define CoreF__kCFBundleOldTypeIconFileKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleOldTypeIconFileKey				]))
//#define CoreF__kCFBundleOldTypeNameKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleOldTypeNameKey					]))
//#define CoreF__kCFBundleOldTypeOSTypesKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleOldTypeOSTypesKey					]))
//#define CoreF__kCFBundleOldTypeRoleKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleOldTypeRoleKey					]))
//#define CoreF__kCFBundlePackageTypeKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundlePackageTypeKey					]))
//#define CoreF__kCFBundlePrincipalClassKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundlePrincipalClassKey					]))
//#define CoreF__kCFBundleRawInfoPlistURLKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleRawInfoPlistURLKey				]))
//#define CoreF__kCFBundleResolvedPathKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleResolvedPathKey					]))
//#define CoreF__kCFBundleResourceSpecificationKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleResourceSpecificationKey			]))
//#define CoreF__kCFBundleResourcesFileMappedKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleResourcesFileMappedKey			]))
//#define CoreF__kCFBundleShortVersionStringKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleShortVersionStringKey				]))
//#define CoreF__kCFBundleSignatureKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleSignatureKey						]))
//#define CoreF__kCFBundleSupportedPlatformsKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleSupportedPlatformsKey				]))
//#define CoreF__kCFBundleTypeExtensionsKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleTypeExtensionsKey					]))
//#define CoreF__kCFBundleTypeIconFileKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleTypeIconFileKey					]))
//#define CoreF__kCFBundleTypeMIMETypesKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleTypeMIMETypesKey					]))
//#define CoreF__kCFBundleTypeNameKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleTypeNameKey						]))
//#define CoreF__kCFBundleTypeOSTypesKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleTypeOSTypesKey					]))
//#define CoreF__kCFBundleTypeRoleKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleTypeRoleKey						]))
//#define CoreF__kCFBundleURLIconFileKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleURLIconFileKey					]))
//#define CoreF__kCFBundleURLNameKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleURLNameKey							]))
//#define CoreF__kCFBundleURLSchemesKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleURLSchemesKey							]))
//#define CoreF__kCFBundleURLTypesKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFBundleURLTypesKey							]))
//#define CoreF__kCFSystemVersionBuildStringKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFSystemVersionBuildStringKey					]))
//#define CoreF__kCFSystemVersionBuildVersionKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFSystemVersionBuildVersionKey				]))
//#define CoreF__kCFSystemVersionProductCopyrightKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFSystemVersionProductCopyrightKey			]))
//#define CoreF__kCFSystemVersionProductNameKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFSystemVersionProductNameKey					]))
//#define CoreF__kCFSystemVersionProductUserVisibleVersionKey					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFSystemVersionProductUserVisibleVersionKey	]))
//#define CoreF__kCFSystemVersionProductVersionExtraKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFSystemVersionProductVersionExtraKey			]))
//#define CoreF__kCFSystemVersionProductVersionKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFSystemVersionProductVersionKey				]))
//#define CoreF__kCFSystemVersionProductVersionStringKey						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM__kCFSystemVersionProductVersionStringKey		]))
//#define CoreF_kCFAbsoluteTimeIntervalSince1904								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFAbsoluteTimeIntervalSince1904				]))
//#define CoreF_kCFAbsoluteTimeIntervalSince1970								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFAbsoluteTimeIntervalSince1970				]))
//#define CoreF_kCFAllocatorDefault												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFAllocatorDefault								]))
//#define CoreF_kCFAllocatorMalloc												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFAllocatorMalloc								]))
//#define CoreF_kCFAllocatorMallocZone											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFAllocatorMallocZone							]))
//#define CoreF_kCFAllocatorNull												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFAllocatorNull								]))
//#define CoreF_kCFAllocatorSystemDefault										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFAllocatorSystemDefault						]))
//#define CoreF_kCFAllocatorUseContext											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFAllocatorUseContext							]))
#define kCFBooleanFalse													(*(CFBooleanRef*)(__gpf_CoreFoundation_dll_ri[ENUM_kCFBooleanFalse									]))
#define kCFBooleanTrue													(*(CFBooleanRef*)(__gpf_CoreFoundation_dll_ri[ENUM_kCFBooleanTrue									]))
//#define CoreF_kCFBuddhistCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFBuddhistCalendar								]))
//#define CoreF_kCFBundleDevelopmentRegionKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFBundleDevelopmentRegionKey					]))
//#define CoreF_kCFBundleExecutableKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFBundleExecutableKey							]))
//#define CoreF_kCFBundleIdentifierKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFBundleIdentifierKey							]))
//#define CoreF_kCFBundleInfoDictionaryVersionKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFBundleInfoDictionaryVersionKey				]))
//#define CoreF_kCFBundleLocalizationsKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFBundleLocalizationsKey						]))
//#define CoreF_kCFBundleNameKey												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFBundleNameKey								]))
//#define CoreF_kCFBundleVersionKey												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFBundleVersionKey								]))
//#define CoreF_kCFCalendarIdentifierBuddhist									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCalendarIdentifierBuddhist					]))
//#define CoreF_kCFCalendarIdentifierChinese									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCalendarIdentifierChinese					]))
//#define CoreF_kCFCalendarIdentifierCoptic										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCalendarIdentifierCoptic						]))
//#define CoreF_kCFCalendarIdentifierEthiopicAmeteAlem							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCalendarIdentifierEthiopicAmeteAlem			]))
//#define CoreF_kCFCalendarIdentifierEthiopicAmeteMihret						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCalendarIdentifierEthiopicAmeteMihret		]))
//#define CoreF_kCFCalendarIdentifierGregorian									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCalendarIdentifierGregorian					]))
//#define CoreF_kCFCalendarIdentifierHebrew										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCalendarIdentifierHebrew						]))
//#define CoreF_kCFCalendarIdentifierISO8601									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCalendarIdentifierISO8601					]))
//#define CoreF_kCFCalendarIdentifierIndian										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCalendarIdentifierIndian						]))
//#define CoreF_kCFCalendarIdentifierIslamic									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCalendarIdentifierIslamic					]))
//#define CoreF_kCFCalendarIdentifierIslamicCivil								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCalendarIdentifierIslamicCivil				]))
//#define CoreF_kCFCalendarIdentifierJapanese									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCalendarIdentifierJapanese					]))
//#define CoreF_kCFCalendarIdentifierPersian									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCalendarIdentifierPersian					]))
//#define CoreF_kCFCalendarIdentifierRepublicOfChina							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCalendarIdentifierRepublicOfChina			]))
//#define CoreF_kCFChineseCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFChineseCalendar								]))
//#define CoreF_kCFCopyStringBagCallBacks										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCopyStringBagCallBacks						]))
//#define CoreF_kCFCopyStringDictionaryKeyCallBacks								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCopyStringDictionaryKeyCallBacks				]))
//#define CoreF_kCFCopyStringSetCallBacks										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFCopyStringSetCallBacks						]))
//#define CoreF_kCFDateFormatterAMSymbol										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterAMSymbol						]))
//#define CoreF_kCFDateFormatterAMSymbolKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterAMSymbolKey						]))
//#define CoreF_kCFDateFormatterCalendar										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterCalendar						]))
//#define CoreF_kCFDateFormatterCalendarIdentifier								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterCalendarIdentifier				]))
//#define CoreF_kCFDateFormatterCalendarIdentifierKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterCalendarIdentifierKey			]))
//#define CoreF_kCFDateFormatterCalendarKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterCalendarKey						]))
//#define CoreF_kCFDateFormatterCalendarName									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterCalendarName					]))
//#define CoreF_kCFDateFormatterDefaultDate										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterDefaultDate						]))
//#define CoreF_kCFDateFormatterDefaultDateKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterDefaultDateKey					]))
//#define CoreF_kCFDateFormatterDefaultFormat									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterDefaultFormat					]))
//#define CoreF_kCFDateFormatterDefaultFormatKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterDefaultFormatKey				]))
//#define CoreF_kCFDateFormatterDoesRelativeDateFormattingKey					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterDoesRelativeDateFormattingKey	]))
//#define CoreF_kCFDateFormatterEraSymbols										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterEraSymbols						]))
//#define CoreF_kCFDateFormatterEraSymbolsKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterEraSymbolsKey					]))
//#define CoreF_kCFDateFormatterGregorianStartDate								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterGregorianStartDate				]))
//#define CoreF_kCFDateFormatterGregorianStartDateKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterGregorianStartDateKey			]))
//#define CoreF_kCFDateFormatterIsLenient										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterIsLenient						]))
//#define CoreF_kCFDateFormatterIsLenientKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterIsLenientKey					]))
//#define CoreF_kCFDateFormatterLongEraSymbols									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterLongEraSymbols					]))
//#define CoreF_kCFDateFormatterLongEraSymbolsKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterLongEraSymbolsKey				]))
//#define CoreF_kCFDateFormatterMonthSymbols									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterMonthSymbols					]))
//#define CoreF_kCFDateFormatterMonthSymbolsKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterMonthSymbolsKey					]))
//#define CoreF_kCFDateFormatterPMSymbol										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterPMSymbol						]))
//#define CoreF_kCFDateFormatterPMSymbolKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterPMSymbolKey						]))
//#define CoreF_kCFDateFormatterQuarterSymbols									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterQuarterSymbols					]))
//#define CoreF_kCFDateFormatterQuarterSymbolsKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterQuarterSymbolsKey				]))
//#define CoreF_kCFDateFormatterShortMonthSymbols								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterShortMonthSymbols				]))
//#define CoreF_kCFDateFormatterShortMonthSymbolsKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterShortMonthSymbolsKey			]))
//#define CoreF_kCFDateFormatterShortQuarterSymbols								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterShortQuarterSymbols				]))
//#define CoreF_kCFDateFormatterShortQuarterSymbolsKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterShortQuarterSymbolsKey			]))
//#define CoreF_kCFDateFormatterShortStandaloneMonthSymbols						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterShortStandaloneMonthSymbols		]))
//#define CoreF_kCFDateFormatterShortStandaloneMonthSymbolsKey					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterShortStandaloneMonthSymbolsKey	]))
//#define CoreF_kCFDateFormatterShortStandaloneQuarterSymbols					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterShortStandaloneQuarterSymbols	]))
//#define CoreF_kCFDateFormatterShortStandaloneQuarterSymbolsKey				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterShortStandaloneQuarterSymbolsKey]))
//#define CoreF_kCFDateFormatterShortStandaloneWeekdaySymbols					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterShortStandaloneWeekdaySymbols	]))
//#define CoreF_kCFDateFormatterShortStandaloneWeekdaySymbolsKey				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterShortStandaloneWeekdaySymbolsKey]))
//#define CoreF_kCFDateFormatterShortWeekdaySymbols								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterShortWeekdaySymbols				]))
//#define CoreF_kCFDateFormatterShortWeekdaySymbolsKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterShortWeekdaySymbolsKey			]))
//#define CoreF_kCFDateFormatterStandaloneMonthSymbols							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterStandaloneMonthSymbols			]))
//#define CoreF_kCFDateFormatterStandaloneMonthSymbolsKey						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterStandaloneMonthSymbolsKey		]))
//#define CoreF_kCFDateFormatterStandaloneQuarterSymbols						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterStandaloneQuarterSymbols		]))
//#define CoreF_kCFDateFormatterStandaloneQuarterSymbolsKey						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterStandaloneQuarterSymbolsKey		]))
//#define CoreF_kCFDateFormatterStandaloneWeekdaySymbols						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterStandaloneWeekdaySymbols		]))
//#define CoreF_kCFDateFormatterStandaloneWeekdaySymbolsKey						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterStandaloneWeekdaySymbolsKey		]))
//#define CoreF_kCFDateFormatterTimeZone										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterTimeZone						]))
//#define CoreF_kCFDateFormatterTimeZoneKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterTimeZoneKey						]))
//#define CoreF_kCFDateFormatterTwoDigitStartDate								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterTwoDigitStartDate				]))
//#define CoreF_kCFDateFormatterTwoDigitStartDateKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterTwoDigitStartDateKey			]))
//#define CoreF_kCFDateFormatterVeryShortMonthSymbols							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterVeryShortMonthSymbols			]))
//#define CoreF_kCFDateFormatterVeryShortMonthSymbolsKey						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterVeryShortMonthSymbolsKey		]))
//#define CoreF_kCFDateFormatterVeryShortStandaloneMonthSymbols					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterVeryShortStandaloneMonthSymbols		]))
//#define CoreF_kCFDateFormatterVeryShortStandaloneMonthSymbolsKey				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterVeryShortStandaloneMonthSymbolsKey	]))
//#define CoreF_kCFDateFormatterVeryShortStandaloneWeekdaySymbols				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterVeryShortStandaloneWeekdaySymbols	]))
//#define CoreF_kCFDateFormatterVeryShortStandaloneWeekdaySymbolsKey			((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterVeryShortStandaloneWeekdaySymbolsKey]))
//#define CoreF_kCFDateFormatterVeryShortWeekdaySymbols							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterVeryShortWeekdaySymbols				]))
//#define CoreF_kCFDateFormatterVeryShortWeekdaySymbolsKey						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterVeryShortWeekdaySymbolsKey			]))
//#define CoreF_kCFDateFormatterWeekdaySymbols									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterWeekdaySymbols						]))
//#define CoreF_kCFDateFormatterWeekdaySymbolsKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFDateFormatterWeekdaySymbolsKey					]))
//#define CoreF_kCFErrorDebugDescriptionKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFErrorDebugDescriptionKey							]))
//#define CoreF_kCFErrorDescriptionKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFErrorDescriptionKey								]))
//#define CoreF_kCFErrorDomainCocoa												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFErrorDomainCocoa									]))
//#define CoreF_kCFErrorDomainCoreFoundation									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFErrorDomainCoreFoundation						]))
//#define CoreF_kCFErrorDomainMach												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFErrorDomainMach									]))
//#define CoreF_kCFErrorDomainOSStatus											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFErrorDomainOSStatus								]))
//#define CoreF_kCFErrorDomainPOSIX												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFErrorDomainPOSIX									]))
//#define CoreF_kCFErrorFilePathKey												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFErrorFilePathKey									]))
//#define CoreF_kCFErrorLocalizedDescriptionKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFErrorLocalizedDescriptionKey						]))
//#define CoreF_kCFErrorLocalizedFailureReasonKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFErrorLocalizedFailureReasonKey					]))
//#define CoreF_kCFErrorLocalizedRecoverySuggestionKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFErrorLocalizedRecoverySuggestionKey				]))
//#define CoreF_kCFErrorURLKey													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFErrorURLKey										]))
//#define CoreF_kCFErrorUnderlyingErrorKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFErrorUnderlyingErrorKey							]))
//#define CoreF_kCFFileURLDirectoryContents										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFFileURLDirectoryContents							]))
//#define CoreF_kCFFileURLExists												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFFileURLExists									]))
//#define CoreF_kCFFileURLLastModificationTime									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFFileURLLastModificationTime						]))
//#define CoreF_kCFFileURLPOSIXMode												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFFileURLPOSIXMode									]))
//#define CoreF_kCFFileURLSize													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFFileURLSize										]))
//#define CoreF_kCFGregorianCalendar											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFGregorianCalendar								]))
//#define CoreF_kCFHTTPURLStatusCode											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFHTTPURLStatusCode								]))
//#define CoreF_kCFHTTPURLStatusLine											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFHTTPURLStatusLine								]))
//#define CoreF_kCFHebrewCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFHebrewCalendar									]))
//#define CoreF_kCFISO8601Calendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFISO8601Calendar									]))
//#define CoreF_kCFIndianCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFIndianCalendar									]))
//#define CoreF_kCFIslamicCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFIslamicCalendar									]))
//#define CoreF_kCFIslamicCivilCalendar											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFIslamicCivilCalendar								]))
//#define CoreF_kCFJapaneseCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFJapaneseCalendar									]))
//#define CoreF_kCFLocaleAlternateQuotationBeginDelimiterKey					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleAlternateQuotationBeginDelimiterKey		]))
//#define CoreF_kCFLocaleAlternateQuotationEndDelimiterKey						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleAlternateQuotationEndDelimiterKey			]))
//#define CoreF_kCFLocaleCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleCalendar									]))
//#define CoreF_kCFLocaleCalendarIdentifier										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleCalendarIdentifier							]))
//#define CoreF_kCFLocaleCalendarIdentifierKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleCalendarIdentifierKey					]))
//#define CoreF_kCFLocaleCalendarKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleCalendarKey							]))
//#define CoreF_kCFLocaleCollationIdentifier									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleCollationIdentifier					]))
//#define CoreF_kCFLocaleCollationIdentifierKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleCollationIdentifierKey					]))
//#define CoreF_kCFLocaleCollatorIdentifier										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleCollatorIdentifier						]))
//#define CoreF_kCFLocaleCollatorIdentifierKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleCollatorIdentifierKey					]))
//#define CoreF_kCFLocaleCountryCode											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleCountryCode							]))
//#define CoreF_kCFLocaleCountryCodeKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleCountryCodeKey							]))
//#define CoreF_kCFLocaleCurrencyCode											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleCurrencyCode							]))
//#define CoreF_kCFLocaleCurrencyCodeKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleCurrencyCodeKey						]))
//#define CoreF_kCFLocaleCurrencySymbol											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleCurrencySymbol							]))
//#define CoreF_kCFLocaleCurrencySymbolKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleCurrencySymbolKey						]))
//#define CoreF_kCFLocaleCurrentLocaleDidChangeNotification						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleCurrentLocaleDidChangeNotification		]))
//#define CoreF_kCFLocaleDecimalSeparator										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleDecimalSeparator						]))
//#define CoreF_kCFLocaleDecimalSeparatorKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleDecimalSeparatorKey					]))
//#define CoreF_kCFLocaleExemplarCharacterSet									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleExemplarCharacterSet					]))
//#define CoreF_kCFLocaleExemplarCharacterSetKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleExemplarCharacterSetKey				]))
//#define CoreF_kCFLocaleGroupingSeparator										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleGroupingSeparator						]))
//#define CoreF_kCFLocaleGroupingSeparatorKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleGroupingSeparatorKey					]))
//#define CoreF_kCFLocaleIdentifier												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleIdentifier								]))
//#define CoreF_kCFLocaleIdentifierKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleIdentifierKey							]))
//#define CoreF_kCFLocaleLanguageCode											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleLanguageCode							]))
//#define CoreF_kCFLocaleLanguageCodeKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleLanguageCodeKey						]))
//#define CoreF_kCFLocaleMeasurementSystem										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleMeasurementSystem						]))
//#define CoreF_kCFLocaleMeasurementSystemKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleMeasurementSystemKey					]))
//#define CoreF_kCFLocaleQuotationBeginDelimiterKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleQuotationBeginDelimiterKey				]))
//#define CoreF_kCFLocaleQuotationEndDelimiterKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleQuotationEndDelimiterKey				]))
//#define CoreF_kCFLocaleScriptCode												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleScriptCode								]))
//#define CoreF_kCFLocaleScriptCodeKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleScriptCodeKey							]))
//#define CoreF_kCFLocaleUsesMetricSystem										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleUsesMetricSystem						]))
//#define CoreF_kCFLocaleUsesMetricSystemKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleUsesMetricSystemKey					]))
//#define CoreF_kCFLocaleVariantCode											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleVariantCode							]))
//#define CoreF_kCFLocaleVariantCodeKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFLocaleVariantCodeKey							]))
//#define CoreF_kCFManagedPreferencesMCXNotificationName						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFManagedPreferencesMCXNotificationName		]))
//#define CoreF_kCFManagedPreferencesMCXObjectName								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFManagedPreferencesMCXObjectName				]))
//#define CoreF_kCFNotificationAnyName											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNotificationAnyName							]))
//#define CoreF_kCFNotificationAnyObject										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNotificationAnyObject						]))
//#define CoreF_kCFNotificationAnyObserver										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNotificationAnyObserver						]))
//#define CoreF_kCFNull															((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNull											]))
//#define CoreF_kCFNumberFormatterAlwaysShowDecimalSeparator					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterAlwaysShowDecimalSeparator	]))
//#define CoreF_kCFNumberFormatterAlwaysShowDecimalSeparatorKey					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterAlwaysShowDecimalSeparatorKey	]))
//#define CoreF_kCFNumberFormatterCurrencyCode									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterCurrencyCode					]))
//#define CoreF_kCFNumberFormatterCurrencyCodeKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterCurrencyCodeKey				]))
//#define CoreF_kCFNumberFormatterCurrencyDecimalSeparator						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterCurrencyDecimalSeparator		]))
//#define CoreF_kCFNumberFormatterCurrencyDecimalSeparatorKey					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterCurrencyDecimalSeparatorKey	]))
//#define CoreF_kCFNumberFormatterCurrencyGroupingSeparator						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterCurrencyGroupingSeparator		]))
//#define CoreF_kCFNumberFormatterCurrencyGroupingSeparatorKey					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterCurrencyGroupingSeparatorKey	]))
//#define CoreF_kCFNumberFormatterCurrencySymbol								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterCurrencySymbol				]))
//#define CoreF_kCFNumberFormatterCurrencySymbolKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterCurrencySymbolKey				]))
//#define CoreF_kCFNumberFormatterDecimalSeparator								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterDecimalSeparator				]))
//#define CoreF_kCFNumberFormatterDecimalSeparatorKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterDecimalSeparatorKey			]))
//#define CoreF_kCFNumberFormatterDefaultFormat									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterDefaultFormat					]))
//#define CoreF_kCFNumberFormatterDefaultFormatKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterDefaultFormatKey				]))
//#define CoreF_kCFNumberFormatterExponentSymbol								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterExponentSymbol				]))
//#define CoreF_kCFNumberFormatterExponentSymbolKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterExponentSymbolKey				]))
//#define CoreF_kCFNumberFormatterFormatWidth									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterFormatWidth					]))
//#define CoreF_kCFNumberFormatterFormatWidthKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterFormatWidthKey				]))
//#define CoreF_kCFNumberFormatterGroupingSeparator								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterGroupingSeparator				]))
//#define CoreF_kCFNumberFormatterGroupingSeparatorKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterGroupingSeparatorKey			]))
//#define CoreF_kCFNumberFormatterGroupingSize									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterGroupingSize					]))
//#define CoreF_kCFNumberFormatterGroupingSizeKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterGroupingSizeKey				]))
//#define CoreF_kCFNumberFormatterInfinitySymbol								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterInfinitySymbol				]))
//#define CoreF_kCFNumberFormatterInfinitySymbolKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterInfinitySymbolKey				]))
//#define CoreF_kCFNumberFormatterInternationalCurrencySymbol					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterInternationalCurrencySymbol	]))
//#define CoreF_kCFNumberFormatterInternationalCurrencySymbolKey				((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterInternationalCurrencySymbolKey]))
//#define CoreF_kCFNumberFormatterIsLenient										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterIsLenient						]))
//#define CoreF_kCFNumberFormatterIsLenientKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterIsLenientKey					]))
//#define CoreF_kCFNumberFormatterMaxFractionDigits								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMaxFractionDigits				]))
//#define CoreF_kCFNumberFormatterMaxFractionDigitsKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMaxFractionDigitsKey			]))
//#define CoreF_kCFNumberFormatterMaxIntegerDigits								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMaxIntegerDigits				]))
//#define CoreF_kCFNumberFormatterMaxIntegerDigitsKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMaxIntegerDigitsKey			]))
//#define CoreF_kCFNumberFormatterMaxSignificantDigits							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMaxSignificantDigits			]))
//#define CoreF_kCFNumberFormatterMaxSignificantDigitsKey						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMaxSignificantDigitsKey		]))
//#define CoreF_kCFNumberFormatterMinFractionDigits								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMinFractionDigits				]))
//#define CoreF_kCFNumberFormatterMinFractionDigitsKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMinFractionDigitsKey			]))
//#define CoreF_kCFNumberFormatterMinIntegerDigits								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMinIntegerDigits				]))
//#define CoreF_kCFNumberFormatterMinIntegerDigitsKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMinIntegerDigitsKey			]))
//#define CoreF_kCFNumberFormatterMinSignificantDigits							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMinSignificantDigits			]))
//#define CoreF_kCFNumberFormatterMinSignificantDigitsKey						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMinSignificantDigitsKey		]))
//#define CoreF_kCFNumberFormatterMinusSign										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMinusSign						]))
//#define CoreF_kCFNumberFormatterMinusSignKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMinusSignKey					]))
//#define CoreF_kCFNumberFormatterMultiplier									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMultiplier					]))
//#define CoreF_kCFNumberFormatterMultiplierKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterMultiplierKey					]))
//#define CoreF_kCFNumberFormatterNaNSymbol										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterNaNSymbol						]))
//#define CoreF_kCFNumberFormatterNaNSymbolKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterNaNSymbolKey					]))
//#define CoreF_kCFNumberFormatterNegativePrefix								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterNegativePrefix				]))
//#define CoreF_kCFNumberFormatterNegativePrefixKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterNegativePrefixKey				]))
//#define CoreF_kCFNumberFormatterNegativeSuffix								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterNegativeSuffix				]))
//#define CoreF_kCFNumberFormatterNegativeSuffixKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterNegativeSuffixKey				]))
//#define CoreF_kCFNumberFormatterPaddingCharacter								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterPaddingCharacter				]))
//#define CoreF_kCFNumberFormatterPaddingCharacterKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterPaddingCharacterKey			]))
//#define CoreF_kCFNumberFormatterPaddingPosition								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterPaddingPosition				]))
//#define CoreF_kCFNumberFormatterPaddingPositionKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterPaddingPositionKey			]))
//#define CoreF_kCFNumberFormatterPerMillSymbol									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterPerMillSymbol					]))
//#define CoreF_kCFNumberFormatterPerMillSymbolKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterPerMillSymbolKey				]))
//#define CoreF_kCFNumberFormatterPercentSymbol									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterPercentSymbol					]))
//#define CoreF_kCFNumberFormatterPercentSymbolKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterPercentSymbolKey				]))
//#define CoreF_kCFNumberFormatterPlusSign										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterPlusSign					]))
//#define CoreF_kCFNumberFormatterPlusSignKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterPlusSignKey				]))
//#define CoreF_kCFNumberFormatterPositivePrefix								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterPositivePrefix			]))
//#define CoreF_kCFNumberFormatterPositivePrefixKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterPositivePrefixKey			]))
//#define CoreF_kCFNumberFormatterPositiveSuffix								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterPositiveSuffix			]))
//#define CoreF_kCFNumberFormatterPositiveSuffixKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterPositiveSuffixKey			]))
//#define CoreF_kCFNumberFormatterRoundingIncrement								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterRoundingIncrement			]))
//#define CoreF_kCFNumberFormatterRoundingIncrementKey							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterRoundingIncrementKey		]))
//#define CoreF_kCFNumberFormatterRoundingMode									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterRoundingMode				]))
//#define CoreF_kCFNumberFormatterRoundingModeKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterRoundingModeKey			]))
//#define CoreF_kCFNumberFormatterSecondaryGroupingSize							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterSecondaryGroupingSize		]))
//#define CoreF_kCFNumberFormatterSecondaryGroupingSizeKey						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterSecondaryGroupingSizeKey	]))
//#define CoreF_kCFNumberFormatterUseGroupingSeparator							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterUseGroupingSeparator		]))
//#define CoreF_kCFNumberFormatterUseGroupingSeparatorKey						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterUseGroupingSeparatorKey	]))
//#define CoreF_kCFNumberFormatterUseSignificantDigits							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterUseSignificantDigits		]))
//#define CoreF_kCFNumberFormatterUseSignificantDigitsKey						((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterUseSignificantDigitsKey	]))
//#define CoreF_kCFNumberFormatterZeroSymbol									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterZeroSymbol				]))
//#define CoreF_kCFNumberFormatterZeroSymbolKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberFormatterZeroSymbolKey				]))
//#define CoreF_kCFNumberNaN													((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberNaN								]))
//#define CoreF_kCFNumberNegativeInfinity										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberNegativeInfinity					]))
//#define CoreF_kCFNumberPositiveInfinity										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFNumberPositiveInfinity					]))
//#define CoreF_kCFPersianCalendar												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFPersianCalendar							]))
//#define CoreF_kCFPlugInDynamicRegisterFunctionKey								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFPlugInDynamicRegisterFunctionKey			]))
//#define CoreF_kCFPlugInDynamicRegistrationKey									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFPlugInDynamicRegistrationKey				]))
//#define CoreF_kCFPlugInFactoriesKey											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFPlugInFactoriesKey						]))
//#define CoreF_kCFPlugInTypesKey												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFPlugInTypesKey							]))
//#define CoreF_kCFPlugInUnloadFunctionKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFPlugInUnloadFunctionKey					]))
//#define CoreF_kCFPreferencesAnyApplication									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFPreferencesAnyApplication				]))
//#define CoreF_kCFPreferencesAnyHost											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFPreferencesAnyHost						]))
//#define CoreF_kCFPreferencesAnyUser											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFPreferencesAnyUser						]))
//#define CoreF_kCFPreferencesCurrentApplication								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFPreferencesCurrentApplication			]))
//#define CoreF_kCFPreferencesCurrentHost										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFPreferencesCurrentHost					]))
//#define CoreF_kCFPreferencesCurrentUser										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFPreferencesCurrentUser					]))
//#define CoreF_kCFPreferencesDidChangeNotificationName							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFPreferencesDidChangeNotificationName		]))
//#define CoreF_kCFPreferencesDidChangeObject									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFPreferencesDidChangeObject				]))
//#define CoreF_kCFRepublicOfChinaCalendar										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFRepublicOfChinaCalendar					]))
//#define CoreF_kCFRunLoopCommonModes											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFRunLoopCommonModes						]))
//#define CoreF_kCFRunLoopDefaultMode											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFRunLoopDefaultMode						]))
//#define CoreF_kCFSocketCommandKey												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFSocketCommandKey							]))
//#define CoreF_kCFSocketErrorKey												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFSocketErrorKey							]))
//#define CoreF_kCFSocketNameKey												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFSocketNameKey							]))
//#define CoreF_kCFSocketRegisterCommand										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFSocketRegisterCommand					]))
//#define CoreF_kCFSocketResultKey												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFSocketResultKey							]))
//#define CoreF_kCFSocketRetrieveCommand										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFSocketRetrieveCommand					]))
//#define CoreF_kCFSocketValueKey												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFSocketValueKey							]))
//#define CoreF_kCFStreamPropertyAppendToFile									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStreamPropertyAppendToFile				]))
#define kCFStreamPropertyDataWritten									(*(DWORD*)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStreamPropertyDataWritten				]))
//#define CoreF_kCFStreamPropertyFileCurrentOffset								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStreamPropertyFileCurrentOffset			]))
//#define CoreF_kCFStreamPropertySocketNativeHandle								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStreamPropertySocketNativeHandle			]))
//#define CoreF_kCFStreamPropertySocketRemoteHostName							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStreamPropertySocketRemoteHostName		]))
//#define CoreF_kCFStreamPropertySocketRemotePortNumber							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStreamPropertySocketRemotePortNumber			]))
//#define CoreF_kCFStringBinaryHeapCallBacks									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringBinaryHeapCallBacks					]))
//#define CoreF_kCFStringFormatLocaleKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringFormatLocaleKey						]))
//#define CoreF_kCFStringFormatSpecTypeKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringFormatSpecTypeKey						]))
//#define CoreF_kCFStringFormatValueTypeKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringFormatValueTypeKey						]))
//#define CoreF_kCFStringGenderRuleType											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringGenderRuleType							]))
//#define CoreF_kCFStringLocalizedFormatKey										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringLocalizedFormatKey						]))
//#define CoreF_kCFStringPluralRuleType											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringPluralRuleType							]))
//#define CoreF_kCFStringTransformFullwidthHalfwidth							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformFullwidthHalfwidth			]))
//#define CoreF_kCFStringTransformHiraganaKatakana								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformHiraganaKatakana				]))
//#define CoreF_kCFStringTransformLatinArabic									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformLatinArabic					]))
//#define CoreF_kCFStringTransformLatinCyrillic									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformLatinCyrillic					]))
//#define CoreF_kCFStringTransformLatinGreek									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformLatinGreek					]))
//#define CoreF_kCFStringTransformLatinHangul									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformLatinHangul					]))
//#define CoreF_kCFStringTransformLatinHebrew									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformLatinHebrew					]))
//#define CoreF_kCFStringTransformLatinHiragana									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformLatinHiragana					]))
//#define CoreF_kCFStringTransformLatinKatakana									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformLatinKatakana					]))
//#define CoreF_kCFStringTransformLatinThai										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformLatinThai						]))
//#define CoreF_kCFStringTransformMandarinLatin									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformMandarinLatin					]))
//#define CoreF_kCFStringTransformStripCombiningMarks							((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformStripCombiningMarks			]))
//#define CoreF_kCFStringTransformStripDiacritics								((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformStripDiacritics				]))
//#define CoreF_kCFStringTransformToLatin										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformToLatin						]))
//#define CoreF_kCFStringTransformToUnicodeName									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformToUnicodeName					]))
//#define CoreF_kCFStringTransformToXMLHex										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFStringTransformToXMLHex						]))
//#define CoreF_kCFTimeZoneSystemTimeZoneDidChangeNotification					((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFTimeZoneSystemTimeZoneDidChangeNotification	]))
#define kCFTypeArrayCallBacks												(*(DWORD*)(__gpf_CoreFoundation_dll_ri[ENUM_kCFTypeArrayCallBacks							]))
//#define CoreF_kCFTypeBagCallBacks												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFTypeBagCallBacks								]))
#define kCFTypeDictionaryKeyCallBacks										(*(CFDictionaryKeyCallBacks*)(__gpf_CoreFoundation_dll_ri[ENUM_kCFTypeDictionaryKeyCallBacks					]))
#define kCFTypeDictionaryValueCallBacks										(*(CFDictionaryValueCallBacks*)(__gpf_CoreFoundation_dll_ri[ENUM_kCFTypeDictionaryValueCallBacks					]))
//#define CoreF_kCFTypeSetCallBacks												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFTypeSetCallBacks								]))
//#define CoreF_kCFURLFileDirectoryContents										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFURLFileDirectoryContents						]))
//#define CoreF_kCFURLFileExists												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFURLFileExists								]))
//#define CoreF_kCFURLFileLastModificationTime									((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFURLFileLastModificationTime					]))
//#define CoreF_kCFURLFileLength												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFURLFileLength								]))
//#define CoreF_kCFURLFileOwnerID												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFURLFileOwnerID								]))
//#define CoreF_kCFURLFilePOSIXMode												((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFURLFilePOSIXMode								]))
//#define CoreF_kCFURLHTTPStatusCode											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFURLHTTPStatusCode							]))
//#define CoreF_kCFURLHTTPStatusLine											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFURLHTTPStatusLine							]))
//#define CoreF_kCFUseCollectableAllocator										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFUseCollectableAllocator						]))
//#define CoreF_kCFXMLTreeErrorDescription										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFXMLTreeErrorDescription						]))
//#define CoreF_kCFXMLTreeErrorLineNumber										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFXMLTreeErrorLineNumber						]))
//#define CoreF_kCFXMLTreeErrorLocation											((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFXMLTreeErrorLocation							]))
//#define CoreF_kCFXMLTreeErrorStatusCode										((DWORD)(__gpf_CoreFoundation_dll_ri[ENUM_kCFXMLTreeErrorStatusCode						]))


#define CFSTR(cstr)			__CFStringMakeConstantString(cstr)	//只能处理英文ascii字符，中文需要用CFStringCreateWithCString

//####################################################################################################################################


//####################################################################################################################################
//初始化ATH_INTERFACE
int ATH_Interface_Initialize( void );

//释放ATH_INTERFACE
int ATH_Interface_Uninitialize( void );

//初始化CoreF_INTERFACE"),
int CoreF_Interface_Initialize(void);

//释放CoreF_INTERFACE"),
int CoreF_Interface_Uninitialize( void );

//初始化CoreF_INTERFACE"),
int ITMD_Interface_Initialize(void);

//释放CoreF_INTERFACE"),
int ITMD_Interface_Uninitialize( void );

int getItunesErrorString(int errcode);

#ifdef __cplusplus
}
#endif

#endif
