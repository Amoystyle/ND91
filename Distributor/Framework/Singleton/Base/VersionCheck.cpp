#include "stdafx.h"
#include "VersionCheck.h"

wstring VersionCheck::getProductVersion(wstring strPath)
{
	int _ver[5];
	DWORD	verInfoSize;   
	DWORD	Handle;   
	BOOL	rt;   
	VS_FIXEDFILEINFO *pFixedFileInfo; 
	UINT size;   
	_ver[0] = -1;
	verInfoSize =::GetFileVersionInfoSizeW(strPath.c_str(), &Handle);
	if(verInfoSize)
	{   
		char *verInfo = new char[verInfoSize];
		rt = ::GetFileVersionInfoW(strPath.c_str(),NULL,verInfoSize,verInfo);     //strPath目标文件的绝对路径   
		if(rt)   
		{   
			size = sizeof(VS_FIXEDFILEINFO);   
			pFixedFileInfo = new VS_FIXEDFILEINFO;
			rt = ::VerQueryValueW(verInfo,_T("\\"),(LPVOID*)&pFixedFileInfo,&size);   
			if(rt)   
			{   
				//取版本数据   
				_ver[0]   =   HIWORD(pFixedFileInfo->dwProductVersionMS);   
				_ver[1]   =   LOWORD(pFixedFileInfo->dwProductVersionMS);   
				_ver[2]   =   HIWORD(pFixedFileInfo->dwProductVersionLS);   
				_ver[3]   =   LOWORD(pFixedFileInfo->dwProductVersionLS);   
			}   
		}   
		delete[] verInfo;   
	}

	TCHAR buf[128] = _T("");
	wsprintf(buf,_T("%d.%d.%d.%d"),_ver[0],_ver[1], _ver[2],_ver[3]);
	strPath = buf;
	return strPath;
}

wstring VersionCheck::GetIniVersion()
{
	TCHAR ini_version[128] = _T("");
	wstring new_version = _T("");
	GetPrivateProfileStringW (_T("General"), _T("ProductLastVersion"),NULL, ini_version, 128, (CFileOperation::GetCurrWorkingDir() + _T("ProductCfg.ini")).c_str()); 
	new_version = ini_version;
	if (new_version.length() == 0)
	{
	}
	return new_version;
}

wstring VersionCheck::GetNowVersion()
{
	wstring now_version;
	now_version = getProductVersion(CFileOperation::GetCurrWorkingDir() + APP_EXENAME);
	return now_version;
}

VersionCheck::VersionCheck()
{

}
