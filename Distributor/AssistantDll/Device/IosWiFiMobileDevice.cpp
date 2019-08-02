#include "stdafx.h"

#include "IosWiFiMobileDevice.h"

#define U2UTF8 MyCodeOperation::Unicode2UTF8

Connect CIosWiFiMobileDevice::m_Connect = NULL;
ConnectionState CIosWiFiMobileDevice::m_ConnectionState = NULL;
CopyToPhone CIosWiFiMobileDevice::m_CopyToPhone = NULL;
DownFromPhone CIosWiFiMobileDevice::m_DownFromPhone = NULL;
CopyToPhoneAsync CIosWiFiMobileDevice::m_CopyToPhoneAsync = NULL;
DownFromPhoneAsync CIosWiFiMobileDevice::m_DownFromPhoneAsync = NULL;
GetTransProgress CIosWiFiMobileDevice::m_GetTransProgress = NULL;
CloseTransHandle CIosWiFiMobileDevice::m_CloseTransHandle = NULL;
Exists CIosWiFiMobileDevice::m_Exists = NULL;
IsDirectory CIosWiFiMobileDevice::m_IsDirectory = NULL;
HasDirectories CIosWiFiMobileDevice::m_HasDirectories = NULL;
CreatePhoneDirectory CIosWiFiMobileDevice::m_CreatePhoneDirectory = NULL;
FileSize CIosWiFiMobileDevice::m_FileSize = NULL;
Rename CIosWiFiMobileDevice::m_Rename = NULL;
DeletePhoneFile CIosWiFiMobileDevice::m_DeletePhoneFile = NULL;
DeleteDirectory CIosWiFiMobileDevice::m_DeleteDirectory = NULL;
GetFiles CIosWiFiMobileDevice::m_GetFiles = NULL;
GetDirectories CIosWiFiMobileDevice::m_GetDirectories = NULL;

SetTickCallBack CIosWiFiMobileDevice::m_SetTickCallBack = NULL;
RequestConnect CIosWiFiMobileDevice::m_RequestConnect = NULL;
ConnectConfig CIosWiFiMobileDevice::m_ConnectConfig = NULL;

#ifdef WIN32
    HINSTANCE CIosWiFiMobileDevice::m_ndiPhoneNet = NULL;
#else
    void* CIosWiFiMobileDevice::m_ndiPhoneNet = NULL;
#endif

CIosWiFiMobileDevice::CIosWiFiMobileDevice(void)
{
}

CIosWiFiMobileDevice::~CIosWiFiMobileDevice(void)
{
}
#ifdef WIN32
    #define GET_FUNCTION(x) 	if (NULL == (m_##x = (x)GetProcAddress(m_ndiPhoneNet, ""#x""))) return false;
#else
#define GET_FUNCTION(x) 	if (NULL == (m_##x = (x)dlsym(m_ndiPhoneNet, ""#x""))) { LOG_ALERT(L" ******* Load function "#x" fail, PLEASE CHECK DLL VERSION "; return false; }
#endif

bool CIosWiFiMobileDevice::MyLoadLibrary()
{
	if (m_ndiPhoneNet)
		return true;
    
#ifdef WIN32
    m_ndiPhoneNet = ::LoadLibrary(DLLNDIPHONENET);
#else
    m_ndiPhoneNet = dlopen(CCodeOperation::UnicodeToUTF_8(DLLNDIPHONENET).c_str(), RTLD_LAZY);
#endif
	if (!m_ndiPhoneNet)
	{
#ifdef WIN32
		TCHAR buffer[1024];
		swprintf(buffer, L"Load %s fail, error code = %d\n", DLLNDIPHONENET, GetLastError());
		LOG_ERROR(buffer);
#endif
		return false;
	}

    GET_FUNCTION(Connect);
    GET_FUNCTION(ConnectionState);
    GET_FUNCTION(CopyToPhone);
    GET_FUNCTION(DownFromPhone);
    GET_FUNCTION(CopyToPhoneAsync);
    GET_FUNCTION(DownFromPhoneAsync);
    GET_FUNCTION(GetTransProgress);
    GET_FUNCTION(CloseTransHandle);
    GET_FUNCTION(Exists);
    GET_FUNCTION(IsDirectory);
    GET_FUNCTION(HasDirectories);
    GET_FUNCTION(CreatePhoneDirectory);
    GET_FUNCTION(FileSize);
    GET_FUNCTION(Rename);
    GET_FUNCTION(DeletePhoneFile);
    GET_FUNCTION(DeleteDirectory);
    GET_FUNCTION(GetFiles);
    GET_FUNCTION(GetDirectories);
    GET_FUNCTION(SetTickCallBack);
    GET_FUNCTION(RequestConnect);
    GET_FUNCTION(ConnectConfig);

	return true;
}

void CIosWiFiMobileDevice::MyFreeLibrary()
{
	if (m_ndiPhoneNet)
	{
#ifdef WIN32
		::FreeLibrary(m_ndiPhoneNet);
#else
        dlclose( m_ndiPhoneNet );
#endif
		m_ndiPhoneNet = NULL;
	}
}

int CIosWiFiMobileDevice::WiFiConnect(wstring ip, wstring psw, int& lastErrorCode)
{
	string strIp = CCodeOperation::UnicodeToGB2312(ip);
	string strPsw = CCodeOperation::UnicodeToGB2312(psw);
	if (MyLoadLibrary() && m_Connect)
		return m_Connect((char*)strIp.c_str(), (char*)strPsw.c_str(), lastErrorCode);

	return -1;
}

void CIosWiFiMobileDevice::WiFiConnectionState(int handle, bool& connected, char* ip, char* phoneversion)
{
	if (MyLoadLibrary() && m_ConnectionState)
		m_ConnectionState(handle, connected, ip, phoneversion);
}

bool CIosWiFiMobileDevice::WiFiCopyToPhone(int handle, wstring source, wstring dest)
{
	if (MyLoadLibrary() && m_CopyToPhone)
		return m_CopyToPhone(handle, U2UTF8(source), U2UTF8(dest));
	return false;
}

bool CIosWiFiMobileDevice::WiFiDownFromPhone(int handle, wstring source, wstring dest)
{
	if (MyLoadLibrary() && m_DownFromPhone)
		return m_DownFromPhone(handle, U2UTF8(source), U2UTF8(dest));
	return false;
}

int CIosWiFiMobileDevice::WiFiCopyToPhoneAsync(int handle, wstring source, wstring dest)
{
	if (MyLoadLibrary() && m_CopyToPhoneAsync)
		return m_CopyToPhoneAsync(handle, (char*)CCodeOperation::UnicodeToGB2312(source).c_str(), U2UTF8(dest));
	return -1;
}

int CIosWiFiMobileDevice::WiFiDownFromPhoneAsync(int handle, wstring source, wstring dest)
{
	if (MyLoadLibrary() && m_DownFromPhoneAsync)
		return m_DownFromPhoneAsync(handle, U2UTF8(source), (char*)CCodeOperation::UnicodeToGB2312(dest).c_str());
	return -1;
}

void CIosWiFiMobileDevice::WiFiGetTransProgress(int handle, int& pFileSize, int& pCompleteSize)
{
	if (MyLoadLibrary() && m_GetTransProgress)
		m_GetTransProgress(handle, pFileSize, pCompleteSize);
}

void CIosWiFiMobileDevice::WiFiCloseTransHandle(int handle)
{
	if (MyLoadLibrary() && m_CloseTransHandle)
		m_CloseTransHandle(handle);
}

int CIosWiFiMobileDevice::WiFiExists(int handle, wstring path)
{
	if (m_ndiPhoneNet && m_Exists)
		return m_Exists(handle, U2UTF8(path));
	return -1;
}

int CIosWiFiMobileDevice::WiFiIsDirectory(int handle, wstring path)
{
	if (MyLoadLibrary() && m_IsDirectory)
		return m_IsDirectory(handle, U2UTF8(path));
	return -1;
}

int CIosWiFiMobileDevice::WiFiHasDirectories(int handle, wstring path)
{
	if (MyLoadLibrary() && m_HasDirectories)
		return m_HasDirectories(handle, U2UTF8(path));
	return -1;
}

bool CIosWiFiMobileDevice::WiFiCreatePhoneDirectory(int handle, wstring path)
{
	if (MyLoadLibrary() && m_CreatePhoneDirectory)
		return m_CreatePhoneDirectory(handle, U2UTF8(path));
	return false;
}

int CIosWiFiMobileDevice::WiFiFileSize(int handle, wstring path)
{
	if (MyLoadLibrary() && m_FileSize)
		return m_FileSize(handle, U2UTF8(path));
	return -1;
}

bool CIosWiFiMobileDevice::WiFiRename(int handle, wstring source, wstring dest)
{
	if (MyLoadLibrary() && m_Rename)
		return m_Rename(handle, U2UTF8(source), U2UTF8(dest));
	return false;
}

bool CIosWiFiMobileDevice::WiFiDeletePhoneFile(int handle, wstring path)
{
	if (MyLoadLibrary() && m_DeletePhoneFile)
		return m_DeletePhoneFile(handle, U2UTF8(path));
	return false;
}

bool CIosWiFiMobileDevice::WiFiDeleteDirectory(int handle, wstring path, bool recursive)
{
	if (MyLoadLibrary() && m_DeleteDirectory)
		return m_DeleteDirectory(handle, U2UTF8(path), recursive);
	return false;
}

int CIosWiFiMobileDevice::WiFiGetFiles(int handle, wstring path, char* buff)
{
	if (MyLoadLibrary() && m_GetFiles)
		return m_GetFiles(handle, U2UTF8(path), buff);
	return -1;
}

int CIosWiFiMobileDevice::WiFiGetDirectories(int handle, wstring path, char* buff)
{
	if (MyLoadLibrary() && m_GetDirectories)
		return m_GetDirectories(handle, U2UTF8(path), buff);
	return -1;
}

bool CIosWiFiMobileDevice::WiFiSetTickCallBack( int handle, TICKCALLBACK pCallBack, void* pVoid )
{
    if (MyLoadLibrary() && m_SetTickCallBack)
        return m_SetTickCallBack(handle, pCallBack, pVoid);
    return false;
}

int CIosWiFiMobileDevice::WiFiRequestConnect( wstring ip, wstring pasw, wstring showtext, unsigned int timeout )
{
    if (MyLoadLibrary() && m_RequestConnect)
        return m_RequestConnect(U2UTF8(ip), U2UTF8(pasw), U2UTF8(showtext), timeout);
    return 0;
}

int CIosWiFiMobileDevice::WiFiConnectConfig( wstring ip, wstring pasw, unsigned int timeout )
{
    if (MyLoadLibrary() && m_ConnectConfig)
        return m_ConnectConfig(U2UTF8(ip), U2UTF8(pasw), timeout);
    return 0;
}
