///////////////////////////////////////////////////////////
//  CIosWiFiMobileDevice.h
//  ndiPhoneNet.dll库的导出函数类
//  用于IOS设备的wifi通讯
///////////////////////////////////////////////////////////

#pragma once

#include "Common/FileOperation.h"
#ifdef WIN32
    #include <tchar.h>
#else
    #include <dlfcn.h>
#endif
#include "iPhoneInterfaceDef.h"

typedef int (*Connect)(char* ip, char* psw, int& lastErrorCode);
typedef void (*ConnectionState)(int handle, bool& connected, char* ip, char* phoneversion);
typedef bool (*CopyToPhone)(int handle, char* sourceOnComputer, char* destinationOnPhone);
typedef bool (*DownFromPhone)(int handle, char* sourceOnPhone, char* destinationOnComputer);
typedef int (*CopyToPhoneAsync)(int handle, char* sourceOnComputer, char* destinationOnPhone);
typedef int (*DownFromPhoneAsync)(int handle, char* sourceOnPhone, char* destinationOnComputer);
typedef void (*GetTransProgress)(int handle, int& pFileSize, int& pCompleteSize);
typedef void (*CloseTransHandle)(int handle);
typedef int (*Exists)(int handle, char* path);
typedef int (*IsDirectory)(int handle, char* path);
typedef int (*HasDirectories)(int handle, char* path);
typedef bool (*CreatePhoneDirectory)(int handle, char* path);
typedef int (*FileSize)(int handle, char* path);
typedef bool (*Rename)(int handle, char* sourceName, char* destName);
typedef bool (*DeletePhoneFile)(int handle, char* path);
typedef bool (*DeleteDirectory)(int handle, char* path, bool recursive);
typedef int (*GetFiles)(int handle, char* path, char* buff);
typedef int (*GetDirectories)(int handle, char* path, char* buff);
// 心跳回调函数
typedef void (*TICKCALLBACK)(int nNotify, void* pVoid);
// 设置回调函数
typedef bool (*SetTickCallBack)(int handle, TICKCALLBACK pCallBack, void* pVoid);
// 请求连接
typedef int (*RequestConnect)(char* ip, char* pasw, char* showtext, unsigned int timeout );
// 查询自动连接设置
typedef int (*ConnectConfig)(char* ip, char* pasw, unsigned int timeout);

class CIosWiFiMobileDevice
{
public:
	CIosWiFiMobileDevice(void);
	~CIosWiFiMobileDevice(void);

	static int WiFiConnect(wstring ip, wstring psw, int& lastErrorCode);
	static void WiFiConnectionState(int handle, bool& connected, char* ip, char* phoneversion);

	static bool WiFiCopyToPhone(int handle, wstring sourceOnComputer, wstring destinationOnPhone);
	static bool WiFiDownFromPhone(int handle, wstring sourceOnPhone, wstring destinationOnComputer);
	static int WiFiCopyToPhoneAsync(int handle, wstring sourceOnComputer, wstring destinationOnPhone);
	static int WiFiDownFromPhoneAsync(int handle, wstring sourceOnPhone, wstring destinationOnComputer);
	static void WiFiGetTransProgress(int handle, int& pFileSize, int& pCompleteSize);
	static void WiFiCloseTransHandle(int handle);
	static int WiFiExists(int handle, wstring path);
	static int WiFiIsDirectory(int handle, wstring path);
	static int WiFiHasDirectories(int handle, wstring path);
	static bool WiFiCreatePhoneDirectory(int handle, wstring path);
	static int WiFiFileSize(int handle, wstring path);
	static bool WiFiRename(int handle, wstring source, wstring dest);
	static bool WiFiDeletePhoneFile(int handle, wstring path);
	static bool WiFiDeleteDirectory(int handle, wstring path, bool recursive);
	static int WiFiGetFiles(int handle, wstring path, char* buff);
	static int WiFiGetDirectories(int handle, wstring path, char* buff);

    static bool WiFiSetTickCallBack(int handle, TICKCALLBACK pCallBack, void* pVoid);
    static int WiFiRequestConnect(wstring ip, wstring pasw, wstring showtext, unsigned int timeout );
    static int WiFiConnectConfig(wstring ip, wstring pasw, unsigned int timeout);

	static bool MyLoadLibrary();
	static void MyFreeLibrary();

private:
#ifdef WIN32
	static HINSTANCE m_ndiPhoneNet;
#else
    static void* m_ndiPhoneNet;
#endif
	static Connect m_Connect;
	static ConnectionState m_ConnectionState;
	static CopyToPhone m_CopyToPhone;
	static DownFromPhone m_DownFromPhone;
	static CopyToPhoneAsync m_CopyToPhoneAsync;
	static DownFromPhoneAsync m_DownFromPhoneAsync;
	static GetTransProgress m_GetTransProgress;
	static CloseTransHandle m_CloseTransHandle;
	static Exists m_Exists;
	static IsDirectory m_IsDirectory;
	static HasDirectories m_HasDirectories;
	static CreatePhoneDirectory m_CreatePhoneDirectory;
	static FileSize m_FileSize;
	static Rename m_Rename;
	static DeletePhoneFile m_DeletePhoneFile;
	static DeleteDirectory m_DeleteDirectory;
	static GetFiles m_GetFiles;
	static GetDirectories m_GetDirectories;
    static SetTickCallBack m_SetTickCallBack;
    static RequestConnect m_RequestConnect;
    static ConnectConfig m_ConnectConfig;
};
