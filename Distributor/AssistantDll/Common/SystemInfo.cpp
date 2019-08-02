#include <stdafx.h>

#include "Common/SystemInfo.h"
#include "Common/Version.h"
#include "Common/CodeOperation.h"
#define	IS_SOFT_ITUNES					_T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\iTunes.exe")
#define	IS_SOFT_ITUNES_PATH				_T("Path")
#define	IS_SOFT_APPLE_MOBILE_DEVICE		_T("Software\\Apple Inc.\\Apple Mobile Device Support")
#define	IS_SOFT_APPLE_MOBILE_VERSION	_T("Version")
#define ITUNES_VERSION					_T("7.5.0.20")
#define APPLE_DEVICE_VERSION			_T("3.1.0.62")
#define APPLE_MOBILE_DEVICE_SUPPORT		_T("Apple Mobile Device Support")
#define ITUNES_URL						_T("http://nav.sj.91.com/launch/Download.aspx?os=0&b=%d&lan=0&sn=%d")
CSystemInfo::CSystemInfo()
{
}

CSystemInfo::~CSystemInfo()
{
}

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
bool CSystemInfo::is64()
{
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandleA("kernel32"), "IsWow64Process");
	BOOL bIsWow64 = FALSE;
	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{
			return FALSE;
		}
	}
	return bIsWow64 == TRUE;
}

bool CSystemInfo::GetOSVer(CVersion* pVer)
{
	OSVERSIONINFO osinfo; 
	osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); 
	if (!GetVersionEx(&osinfo)) 
		return false;
	pVer->_nVer1 = osinfo.dwMajorVersion;
	pVer->_nVer2 = osinfo.dwMinorVersion;
	return true;
}

bool CSystemInfo::IsVista()
{
	CVersion ver;
	if (GetOSVer(&ver))
	{
		return ver._nVer1 == 6;
	}
	return false;
}

bool CSystemInfo::IsWindows7()
{
	CVersion ver;
	if (GetOSVer(&ver))
	{
		return ver._nVer1 == 7;
	}
	return false;
}

bool CSystemInfo::IsXP()
{
	CVersion ver;
	if (GetOSVer(&ver))
	{
		return ver._nVer1 == 5;
	}
	return false;
}

std::wstring CSystemInfo::GetSystemInfo()
{
    OSVERSIONINFO osinfo; 
    osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); 
    if (!GetVersionEx(&osinfo)) 
        return L"";

    unsigned long dwPlatformId   = osinfo.dwPlatformId; 
    unsigned long dwMinorVersion = osinfo.dwMinorVersion; 
    unsigned long dwMajorVersion = osinfo.dwMajorVersion; 
    unsigned long dwBuildNumber  = osinfo.dwBuildNumber & 0xFFFF; // Win 95 needs this 

    wostringstream woss;
    woss    << osinfo.dwPlatformId << L"." 
        << osinfo.dwMinorVersion << L"." 
        << osinfo.dwMajorVersion << L"." 
        << (osinfo.dwBuildNumber & 0xFFFF);
    return woss.str();
}

typedef void (WINAPI *PGetNativeSystemInfo)(LPSYSTEM_INFO);
std::string CSystemInfo::GetOs()
{
	PGetNativeSystemInfo pGNSI = NULL;
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	BOOL bOsVersionInfoEx;
	string strSystemName;

	ZeroMemory( &si, sizeof(SYSTEM_INFO) );
	ZeroMemory( &osvi, sizeof(OSVERSIONINFOEX) );
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *)&osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if( !GetVersionEx ( (OSVERSIONINFO *)&osvi ) ) 
			return "";
	}

	pGNSI = (PGetNativeSystemInfo)GetProcAddress( GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo" );
	if(NULL != pGNSI)
		pGNSI(&si);
	else
		::GetSystemInfo(&si);

	switch( osvi.dwPlatformId )
	{  
	case VER_PLATFORM_WIN32_NT: // Windows NT family.
		{
			if( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 && osvi.wProductType == VER_NT_WORKSTATION )
				strSystemName = "Microsoft Windows Vista";
			else if( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 && osvi.wProductType != VER_NT_WORKSTATION )
				strSystemName = "Microsoft Windows Server 2008";
			else if( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1 && osvi.wProductType == VER_NT_WORKSTATION )
				strSystemName = "Microsoft Windows7";
			else if( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1 && osvi.wProductType != VER_NT_WORKSTATION )
				strSystemName = "Microsoft Windows Server 2008 R2";
			else if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 )
					strSystemName = "Microsoft Windows XP Professional x64 Edition";
				else
					strSystemName = GetSystemMetrics(SM_SERVERR2) != 0 ? "Microsoft Windows Server 2003 R2" : "Microsoft Windows Server 2003";
			}
			else if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
				strSystemName = "Microsoft Windows XP";
			else if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
				strSystemName = "Microsoft Windows 2000";
			else if( osvi.dwMajorVersion <= 4 )
				strSystemName = "Microsoft Windows NT";
		}
		break;

	case VER_PLATFORM_WIN32_WINDOWS: // Windows 95 family.
		{
			if( osvi.dwMajorVersion == 4 &&  osvi.dwMinorVersion == 0 )
				strSystemName = "Microsoft Windows 95";
			else if( osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10 )
				strSystemName = osvi.szCSDVersion[1] == 'A' ? "Windows 98_SE" : "Windows 98";
			else if( osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90 )
				strSystemName = "Microsoft Windows Millennium Edition";
		}
		break;

	case VER_PLATFORM_WIN32s:
		{
			strSystemName = "Microsoft Win32s";
		}
		break;

	default:
		{
			strSystemName = "Unknown";
		}
	}

	return strSystemName;
}

#define NET_CARD_KEY _T("System\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}")

// 利用网卡名去注册表中查找对应的硬件信息。
// 若PnpInstanceID的值以PCI开头，说明是物理网卡
bool IsLocalAdapter(char *pAdapterName)
{
	bool ret_value = FALSE;

	char szDataBuf[MAX_PATH+1];
	DWORD dwDataLen = MAX_PATH;
	DWORD dwType = REG_SZ;
	HKEY hNetKey = NULL;
	HKEY hLocalNet = NULL;

	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, NET_CARD_KEY, 0, KEY_READ, &hNetKey))
		return false;

	wsprintfA(szDataBuf, "%s\\Connection", pAdapterName);

	if(ERROR_SUCCESS != RegOpenKeyExA(hNetKey ,szDataBuf ,0 ,KEY_READ, &hLocalNet))
	{
		RegCloseKey(hNetKey);
		return false;
	}

	dwDataLen = MAX_PATH;
	if (ERROR_SUCCESS != RegQueryValueExA(hLocalNet, "PnpInstanceID", 0, &dwType, (BYTE *)szDataBuf, &dwDataLen))
	{
		goto ret;
	}

	if (strncmp(szDataBuf, "PCI", strlen("PCI")))
		goto ret;

	ret_value = true;

ret:
	RegCloseKey(hLocalNet);
	RegCloseKey(hNetKey);
	return ret_value;
}

std::string CSystemInfo::GetMac()
{
	std::string out;
	PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	unsigned long ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS) {
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen);
	}

	DWORD dwRetVal = 0;
	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) != NO_ERROR) {
		//cpLogError("Call to GetAdaptersInfo failed", dwRetVal);
		GlobalFree (pAdapterInfo);
		return out;
	}

	DWORD minIndex = -1;
	UINT AddressLength = 0;
	BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];

	PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
	while (pAdapter) {
		if ( IsLocalAdapter(pAdapter->AdapterName) )
		{
			if ( pAdapter->Index < minIndex )
			{
				minIndex = pAdapter->Index;
				AddressLength = pAdapter->AddressLength;
				memcpy(Address, pAdapter->Address, AddressLength);
			}
		}
		pAdapter = pAdapter->Next;
	}

	GlobalFree(pAdapter);

	char buffer[16];
	for (UINT i = 0; i < AddressLength; i++)
	{
		//sprintf_s(buffer, 16, "%02x%c", Address[i], i == AddressLength - 1 ? '\0' : '-');
		sprintf_s(buffer, 16, "%02x", Address[i]);
		out.append(buffer);
	}
	return out;
}

bool CSystemInfo::IsNeedInstalliTunes(wstring minVer)
{
	bool bNeed = false;
#ifdef WIN32	
	//检查iTunes.exe版本
	wstring strItunesPath = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, IS_SOFT_ITUNES, IS_SOFT_ITUNES_PATH);
	//某些版本iTunes使用默认值为path值
	if(strItunesPath.empty())
		strItunesPath = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, IS_SOFT_ITUNES, _T(""));
	LOG->WriteDebug(L"取得iTunes完整版地址：" + strItunesPath);
	if (strItunesPath.length() > 0 && CFileOperation::IsDirExist(strItunesPath.c_str()))
	{//已安装iTunes完整版
		CVersion iTunesVer = CVersion::GetFileVersion(strItunesPath.c_str());
		LOG->WriteDebug(L"取得iTunes版本：" + iTunesVer.ToStringW());
		CVersion minVer(CCodeOperation::UnicodeToGB2312(minVer));
		if (iTunesVer.CompareTo(&minVer) < 0)//(CFileOperation::CompareFileVersion(&iTunesVer, &minVer) < 0)
		{//版本低于最小要求
			LOG->WriteDebug(L"版本低于最低要求：" + minVer.ToStringW());
			bNeed = true;
		}
	}
	else
	{//未安装iTunes完整版, 
		LOG->WriteDebug(L"未安装iTunes完整版,检查简化版信息");
		// 检查
		wstring strItunesVer = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, IS_SOFT_APPLE_MOBILE_DEVICE, IS_SOFT_APPLE_MOBILE_VERSION);
		LOG->WriteDebug(L"取得iTunes版本：" + strItunesVer);
		if (strItunesVer.length() > 0)
		{
			CVersion curVer(CCodeOperation::UnicodeToGB2312(strItunesVer));
			CVersion minVer(CCodeOperation::UnicodeToGB2312(APPLE_DEVICE_VERSION));
			if (curVer.CompareTo(&minVer) < 0)
			{
				LOG->WriteDebug(L"简化版Itunes小于最小版本要求：" + minVer.ToStringW());
				bNeed = true;
			}
		}
		else
		{// 没有找到值，说明没有安装
			LOG->WriteDebug(L"没有安装iTunes!");
			bNeed = true;
		}
	}
#endif	
	return bNeed;
}