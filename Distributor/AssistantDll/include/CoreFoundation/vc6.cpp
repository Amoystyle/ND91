#include <sal.h>
#include <windows.h>
#include "vc6.h"

HINSTANCE g_dll = NULL;	// handle to kernel32.dll 

inline void* Load(const char * funcName)
{
	if ( ! g_dll )
	{
		g_dll = LoadLibrary("kernel32.dll");
	}
	return GetProcAddress(g_dll, funcName);
}

#define LOAD_FUNC(x) 	static FUNC_##x func = (FUNC_##x)Load(""#x"");

/************************************************************************
   从KERNEL32.DLL导入函数: CreateTimerQueue
************************************************************************/
typedef HANDLE (*FUNC_CreateTimerQueue) ();

HANDLE CreateTimerQueue()
{
	LOAD_FUNC(CreateTimerQueue);
	return func ? func()
		: NULL;	// If the function fails, the return value is NULL
}

/************************************************************************
   从KERNEL32.DLL导入函数: CreateTimerQueueTimer
************************************************************************/
typedef BOOL (*FUNC_CreateTimerQueueTimer)(
	PHANDLE phNewTimer,
	HANDLE TimerQueue,
	WAITORTIMERCALLBACK Callback,
	PVOID Parameter,
	DWORD DueTime,
	DWORD Period,
	ULONG Flags);

BOOL CreateTimerQueueTimer(
	PHANDLE phNewTimer,
	HANDLE TimerQueue,
	WAITORTIMERCALLBACK Callback,
	PVOID Parameter,
	DWORD DueTime,
	DWORD Period,
	ULONG Flags
	)
{
	LOAD_FUNC(CreateTimerQueueTimer);
	return func ? func(phNewTimer, TimerQueue, Callback, Parameter, DueTime, Period, Flags)
		: false; // If the function fails, the return value is zero.
}

/************************************************************************
   从KERNEL32.DLL导入函数: DeleteTimerQueueTimer
************************************************************************/
typedef BOOL (*FUNC_DeleteTimerQueueTimer)(
	__in_opt HANDLE TimerQueue,
	__in     HANDLE Timer,
	__in_opt HANDLE CompletionEvent);

BOOL DeleteTimerQueueTimer(
	__in_opt HANDLE TimerQueue,
	__in     HANDLE Timer,
	__in_opt HANDLE CompletionEvent)
{
	LOAD_FUNC(DeleteTimerQueueTimer);
	return func ? func(TimerQueue, Timer, CompletionEvent) 
		: false;  // If the function fails, the return value is zero
}

/************************************************************************
   从KERNEL32.DLL导入函数: DeleteTimerQueue
************************************************************************/
typedef BOOL (*FUNC_DeleteTimerQueue)(__in HANDLE TimerQueue);

BOOL DeleteTimerQueue(__in HANDLE TimerQueue)
{
	LOAD_FUNC(DeleteTimerQueue);
	return func ? func(TimerQueue) 
		: false;  // If the function fails, the return value is zero
}

/************************************************************************
   从KERNEL32.DLL导入函数: RegDeleteKeyValue
************************************************************************/

typedef __success(return==ERROR_SUCCESS) LONG LSTATUS;

typedef LSTATUS (*FUNC_RegDeleteKeyValueA)(
	__in      HKEY     hKey,
	__in_opt  LPCSTR lpSubKey,
	__in_opt  LPCSTR lpValueName
	);

LSTATUS RegDeleteKeyValueA (
	__in      HKEY     hKey,
	__in_opt  LPCSTR lpSubKey,
	__in_opt  LPCSTR lpValueName
	)
{
	LOAD_FUNC(RegDeleteKeyValueA);
	return func ? func(hKey, lpSubKey, lpValueName) 
		: -1;  // If the function fails, the return value is a nonzero error code
}

typedef LSTATUS (*FUNC_RegDeleteKeyValueW)(
	__in      HKEY     hKey,
	__in_opt  LPCWSTR lpSubKey,
	__in_opt  LPCWSTR lpValueName
	);

LSTATUS RegDeleteKeyValueW (
	__in      HKEY     hKey,
	__in_opt  LPCWSTR lpSubKey,
	__in_opt  LPCWSTR lpValueName
	)
{
	LOAD_FUNC(RegDeleteKeyValueW);
	return func ? func(hKey, lpSubKey, lpValueName) 
		: -1;  // If the function fails, the return value is a nonzero error code
}

#ifdef UNICODE
#define RegDeleteKeyValue  RegDeleteKeyValueW
#else
#define RegDeleteKeyValue  RegDeleteKeyValueA
#endif // !UNICODE

/************************************************************************
   从KERNEL32.DLL导入函数: RegisterServiceCtrlHandlerExA
************************************************************************/

#define SERVICE_CONTROL_DEVICEEVENT          0x0000000B
#define DEVICE_NOTIFY_SERVICE_HANDLE         0x00000001
#define DEVICE_NOTIFY_ALL_INTERFACE_CLASSES  0x00000004

typedef DWORD (WINAPI *LPHANDLER_FUNCTION_EX)(
	  DWORD    dwControl,
	  DWORD    dwEventType,
	  LPVOID   lpEventData,
	  LPVOID   lpContext);

typedef SERVICE_STATUS_HANDLE (*FUNC_RegisterServiceCtrlHandlerExA)(
	  __in    LPCSTR		lpServiceName,
	  __in    __callback  LPHANDLER_FUNCTION_EX	lpHandlerProc,
	  __in_opt LPVOID		lpContext    );

SERVICE_STATUS_HANDLE RegisterServiceCtrlHandlerExA(
	__in    LPCSTR								lpServiceName,
	__in    __callback LPHANDLER_FUNCTION_EX    lpHandlerProc,
	__in_opt LPVOID								lpContext)
{
	LOAD_FUNC(RegisterServiceCtrlHandlerExA);
	return func ? func(lpServiceName, lpHandlerProc, lpContext) 
		: 0;  // If the function fails, the return value is zero
}

/************************************************************************
   从KERNEL32.DLL导入函数: RegGetValue
************************************************************************/

#define RRF_RT_REG_SZ          0x00000002  // restrict type to REG_SZ        (other data types will not return ERROR_SUCCESS) (automatically converts REG_EXPAND_SZ to REG_SZ unless RRF_NOEXPAND is specified)

typedef LSTATUS (*FUNC_RegGetValueA) (
	__in HKEY    hkey,
	__in_opt LPCSTR  lpSubKey,
	__in_opt LPCSTR  lpValue,
	__in_opt DWORD    dwFlags,
	__out_opt LPDWORD pdwType,
	__out_bcount_part_opt(*pcbData,*pcbData) PVOID   pvData,
	__inout_opt LPDWORD pcbData
	);

LSTATUS RegGetValueA (
	__in HKEY    hkey,
	__in_opt LPCSTR  lpSubKey,
	__in_opt LPCSTR  lpValue,
	__in_opt DWORD    dwFlags,
	__out_opt LPDWORD pdwType,
	__out_bcount_part_opt(*pcbData,*pcbData) PVOID   pvData,
	__inout_opt LPDWORD pcbData)
{
	LOAD_FUNC(RegGetValueA);
	return func ? func(hkey, lpSubKey, lpValue, dwFlags, pdwType, pvData, pcbData)
	: 0;  // If the function fails, the return value is zero
}

typedef LSTATUS (*FUNC_RegGetValueW) (
	__in HKEY    hkey,
	__in_opt LPCWSTR  lpSubKey,
	__in_opt LPCWSTR  lpValue,
	__in_opt DWORD    dwFlags,
	__out_opt LPDWORD pdwType,
	__out_bcount_part_opt(*pcbData,*pcbData) PVOID   pvData,
	__inout_opt LPDWORD pcbData);

LSTATUS RegGetValueW (
	__in HKEY    hkey,
	__in_opt LPCWSTR  lpSubKey,
	__in_opt LPCWSTR  lpValue,
	__in_opt DWORD    dwFlags,
	__out_opt LPDWORD pdwType,
	__out_bcount_part_opt(*pcbData,*pcbData) PVOID   pvData,
	__inout_opt LPDWORD pcbData)
{
	LOAD_FUNC(RegGetValueW);
	return func ? func(hkey, lpSubKey, lpValue, dwFlags, pdwType, pvData, pcbData)
		: 0;  // If the function fails, the return value is zero
}

#ifdef UNICODE
#define RegGetValue  RegGetValueW
#else
#define RegGetValue  RegGetValueA
#endif // !UNICODE

/************************************************************************
   从KERNEL32.DLL导入函数: UnregisterDeviceNotification
************************************************************************/

typedef BOOL (*FUNC_UnregisterDeviceNotification) (__in HDEVNOTIFY Handle);

BOOL UnregisterDeviceNotification(__in HDEVNOTIFY Handle)
{
	LOAD_FUNC(UnregisterDeviceNotification);
	return func ? func(Handle)
		: 0;  // If the function fails, the return value is zero
}

/************************************************************************
   从KERNEL32.DLL导入函数: RegisterDeviceNotificationA
************************************************************************/

typedef HDEVNOTIFY (*FUNC_RegisterDeviceNotificationA)(
	IN HANDLE hRecipient,
	IN LPVOID NotificationFilter,
	IN DWORD Flags);

HDEVNOTIFY RegisterDeviceNotificationA(
	IN HANDLE hRecipient,
	IN LPVOID NotificationFilter,
	IN DWORD Flags)
{
	LOAD_FUNC(RegisterDeviceNotificationA);
	return func ? func(hRecipient, NotificationFilter, Flags)
		: 0;  // If the function fails, the return value is NULL
}