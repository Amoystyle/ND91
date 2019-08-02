
#pragma once

#include <sal.h>

HANDLE CreateTimerQueue();

typedef VOID (NTAPI * WAITORTIMERCALLBACKFUNC) (PVOID, BOOLEAN ); 
typedef WAITORTIMERCALLBACKFUNC WAITORTIMERCALLBACK ;

BOOL CreateTimerQueueTimer(
    PHANDLE phNewTimer,
    HANDLE TimerQueue,
    WAITORTIMERCALLBACK Callback,
    PVOID Parameter,
    DWORD DueTime,
    DWORD Period,
    ULONG Flags
    ) ;

BOOL DeleteTimerQueueTimer(
	__in_opt HANDLE TimerQueue,
	__in     HANDLE Timer,
	__in_opt HANDLE CompletionEvent
	);

BOOL DeleteTimerQueue(__in HANDLE TimerQueue);

#define APIENTRY    WINAPI
typedef __success(return==ERROR_SUCCESS) LONG LSTATUS;

LSTATUS RegDeleteKeyValueA (
					__in      HKEY     hKey,
					__in_opt  LPCSTR lpSubKey,
					__in_opt  LPCSTR lpValueName
					);

LSTATUS RegDeleteKeyValueW (
					__in      HKEY     hKey,
					__in_opt  LPCWSTR lpSubKey,
					__in_opt  LPCWSTR lpValueName
					);

#ifdef UNICODE
#define RegDeleteKeyValue  RegDeleteKeyValueW
#else
#define RegDeleteKeyValue  RegDeleteKeyValueA
#endif // !UNICODE

#define SERVICE_CONTROL_DEVICEEVENT          0x0000000B
#define DEVICE_NOTIFY_SERVICE_HANDLE         0x00000001
#define DEVICE_NOTIFY_ALL_INTERFACE_CLASSES  0x00000004

typedef DWORD (WINAPI *LPHANDLER_FUNCTION_EX)(
	  DWORD    dwControl,
	  DWORD    dwEventType,
	  LPVOID   lpEventData,
	  LPVOID   lpContext
    );

SERVICE_STATUS_HANDLE RegisterServiceCtrlHandlerExA(
	  __in    LPCSTR                    lpServiceName,
	  __in    __callback
	  LPHANDLER_FUNCTION_EX       lpHandlerProc,
	  __in_opt LPVOID                     lpContext
    );

#define RRF_RT_REG_SZ          0x00000002  // restrict type to REG_SZ        (other data types will not return ERROR_SUCCESS) (automatically converts REG_EXPAND_SZ to REG_SZ unless RRF_NOEXPAND is specified)

LSTATUS RegGetValueA (
	  __in HKEY    hkey,
	  __in_opt LPCSTR  lpSubKey,
	  __in_opt LPCSTR  lpValue,
	  __in_opt DWORD    dwFlags,
	  __out_opt LPDWORD pdwType,
	  __out_bcount_part_opt(*pcbData,*pcbData) PVOID   pvData,
	  __inout_opt LPDWORD pcbData
	  );

LSTATUS RegGetValueW (
	  __in HKEY    hkey,
	  __in_opt LPCWSTR  lpSubKey,
	  __in_opt LPCWSTR  lpValue,
	  __in_opt DWORD    dwFlags,
	  __out_opt LPDWORD pdwType,
	  __out_bcount_part_opt(*pcbData,*pcbData) PVOID   pvData,
	  __inout_opt LPDWORD pcbData
	  );

#ifdef UNICODE
#define RegGetValue  RegGetValueW
#else
#define RegGetValue  RegGetValueA
#endif // !UNICODE


typedef  PVOID           HDEVNOTIFY;
typedef  HDEVNOTIFY     *PHDEVNOTIFY;

BOOL UnregisterDeviceNotification(__in HDEVNOTIFY Handle);

HDEVNOTIFY RegisterDeviceNotificationA(
	IN HANDLE hRecipient,
	IN LPVOID NotificationFilter,
	IN DWORD Flags
    );