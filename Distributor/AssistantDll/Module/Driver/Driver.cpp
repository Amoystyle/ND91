#include "stdafx.h"

#ifdef MODULE_DRIVER

#include "Module/Driver/Driver.h"
#include "Module/Driver/HardwareInfo.h"
#include "HttpDriver.h"
#include "Common/SystemInfo.h"
#include "Module/Driver/DriverManager.h"

Driver::Driver(HardwareInfo* pHardware)
{
  _pHardware = pHardware;
	_bDefaultDriver = false;
}

Driver::Driver(Driver* pDriver)
{
  _pHardware = new HardwareInfo(pDriver->getHardwareInfo());
  _bDefaultDriver = pDriver->_bDefaultDriver;
}

Driver::~Driver()
{
  SAFE_DELETE(_pHardware);
}

bool ND91Assistant::Driver::InstallInf(wstring strInf)
{
#ifdef WIN32			
	if (_bDefaultDriver)
		CheckDefaultInfFile(strInf);
	LOG->WriteDebug(L"安装INF驱动：" + strInf);
	try 
	{
		bool bRec = (UpdateDriverForPlugAndPlayDevicesW(NULL,_pHardware->_strHardID.c_str(),strInf.c_str(),INSTALLFLAG_FORCE,NULL) == TRUE);
		return bRec;
	
	}
	catch(...)
	{
		THROW_ASSISTANT_EXCEPTION(INVALID_FILE_NAME);
	}
#endif		
	return false;
}

bool ND91Assistant::Driver::InstallExe( wstring strExe, wstring strParam)
{
	bool bRec = false;
#ifdef WIN32	
	LOG->WriteDebug(L"安装Exe驱动：" + strExe + L" " + strParam);
	try
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		memset(&si,0,sizeof(si)); 
		memset(&pi,0,sizeof(pi)); 
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW; 
		si.wShowWindow = SW_SHOW;//SW_HIDE; 
		SECURITY_ATTRIBUTES psa={sizeof(psa),NULL,TRUE}; 
		psa.bInheritHandle=TRUE; 
		HANDLE houtfile = NULL;
		//houtfile=CreateFile(m_pResultFile,GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE, &psa,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);	
		//if (houtfile==NULL)
		//	return ;
		si.hStdError = houtfile;
		si.hStdOutput  = houtfile;
		
		wchar_t pBuffer[256] = {0};
		memcpy(pBuffer, strParam.c_str(), strParam.length() > 255 ? 255 : strParam.length());
		if (CreateProcess(strExe.c_str(), pBuffer, NULL, NULL,TRUE,0,NULL,NULL,&si,&pi))
		{
			bRec = TRUE;
			WaitForSingleObject(pi.hProcess, INFINITE);	//死等
			if (houtfile != NULL)
			{
				CloseHandle(houtfile);
			}
		} 
	}
	catch(...)
	{
		THROW_ASSISTANT_EXCEPTION(INVALID_FILE_NAME);
	}
#endif	
	return bRec;
}	

bool ND91Assistant::Driver::InstallMsi(wstring strMsi, wstring strParam)
{
	bool bRec = false;
#ifdef WIN32	 
	LOG->WriteDebug(L"安装Msi驱动：" + strMsi + L" " + strParam);
	try
	{		
		SHELLEXECUTEINFO ShExecInfo; 
		ZeroMemory(&ShExecInfo, sizeof(SHELLEXECUTEINFO)); 
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); 
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 
		ShExecInfo.hwnd = NULL; 
		ShExecInfo.lpVerb = NULL; 
		ShExecInfo.lpFile = strMsi.c_str(); 
		ShExecInfo.lpParameters = strParam.c_str(); 
		ShExecInfo.lpDirectory = NULL; 
		ShExecInfo.nShow = SW_SHOW;//SW_HIDE; 
		ShExecInfo.hInstApp = NULL; 
		if (ShellExecuteEx(&ShExecInfo))
		{
			bRec = true;
			WaitForSingleObject(ShExecInfo.hProcess, INFINITE); //死等		
		}		
	}
	catch(...)
	{
		THROW_ASSISTANT_EXCEPTION(INVALID_FILE_NAME);
	}
#endif	
	return bRec;
}

std::wstring ND91Assistant::Driver::GetDriverUrl()
{
	HttpDriver hd(_pHardware);
	return hd.GetDriverUrl();
}

std::wstring ND91Assistant::Driver::GetDefaultAndroidDriverUrl()
{
	_bDefaultDriver = true;
	HttpDriver hd(_pHardware);
	return hd.GetDefaultAndroidDriverUrl();
}

bool ND91Assistant::Driver::CheckDefaultInfFile( wstring strInfFile )
{
#ifdef WIN32	
	LOG->WriteDebug(L"检查默认INF驱动：" + strInfFile);
	const HardwareInfo* pInfo = getHardwareInfo();
	wstring strID = GetInfHardID(pInfo);
	wstring strKey1 = strID + _T(".DeviceDescRelease");
	wstring strKey2 = _T("%") + strKey1 + _T("%");
	wstring strValue1 = _T("HTCAND32.Dev, ") + strID;
	wstring strValue2 = _T("\"Adb Device\"");

	// 所有平台都会有的值，以此来判断inf里面已有该设备的支持信息
	wstring strValue = MyFileOperation::GetIniFileValue(strInfFile, _T("Strings"), strKey1);
	if (strValue.empty()) // 不支持该型号
	{
		// 写入该设备的信息，以使默认驱动支持该设备
		LOG->WriteDebug(CCodeOperation::StringToWstring("检查默认INF驱动：添加支持"));
		if (CSystemInfo::IsXP())
			MyFileOperation::SetIniFileValue(strInfFile, _T("HTC"),	strKey2, strValue1);

		if (CSystemInfo::is64())
			MyFileOperation::SetIniFileValue(strInfFile, _T("HTC.NTAMD64"),strKey2, strValue1);
		else
			MyFileOperation::SetIniFileValue(strInfFile, _T("HTC.NTx86"),strKey2, strValue1);

		MyFileOperation::SetIniFileValue(strInfFile, _T("Strings"),	strKey1, strValue2);

		return false;
	}
	return true;
#else
	return false;
#endif		
}

wstring ND91Assistant::Driver::GetInfHardID( const HardwareInfo* pInfo )
{
	ostringstream ss;
	ss << "USB\\VID_" << pInfo->_strVID << "&PID_" << pInfo->_strPID;
	wstring strID = CCodeOperation::GB2312ToUnicode(ss.str());
	wstring strHardID = CStrOperation::toUpperW(pInfo->_strHardID);

	int nIndex = strHardID.find(L"&MI_");
	if (nIndex > 0 && nIndex + 6 <= (int)strHardID.length())
		strID += strHardID.substr(nIndex, 6);
	return CStrOperation::toUpperW(strID);
}

void ND91Assistant::Driver::SetDriverState( unsigned long nState )
{
	_pHardware->_nState = nState;
}

bool Driver::WakeupDevice()
{
	bool bRec = false;
#ifdef WIN32	
	string strID = CCodeOperation::UnicodeToGB2312(CStrOperation::toUpperW(GetInfHardID(getHardwareInfo())));
	wstring strDir = MyFileOperation::GetWindowsDir() + L"\\inf\\";
	wstring strFind = strDir + L"*";
	WIN32_FIND_DATA fd;
	HANDLE hFindFile = FindFirstFile(strFind.c_str(), &fd);
	vector<wstring> files;
	if (INVALID_HANDLE_VALUE != hFindFile)
	{
		bool bFinish = false;
		while (!bFinish)
		{
			bool bDir = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

			if (bDir || _tcscmp(fd.cFileName, L".")==0 || _tcscmp(fd.cFileName, L"..")==0 )
			{
				bFinish  = (FindNextFile(hFindFile, &fd) == false);
				continue;
			}

			if (CStrOperation::toUpperW(CFileOperation::GetFileExt(fd.cFileName)) == L"INF"
				&& (CStrOperation::toUpperW(fd.cFileName).find(L"USB.INF") == 0 || CStrOperation::toUpperW(fd.cFileName).find(L"OEM") == 0))
			{
				if ((int)CFileOperation::ReadFile(strDir + fd.cFileName).find(strID) > 0)
				{
					files.insert(files.begin(), strDir + fd.cFileName);
				}				
			}
			bFinish  = (FindNextFile(hFindFile, &fd) == false);
		}
	}
	FindClose(hFindFile);

	for (vector<wstring>::iterator it = files.begin(); it != files.end(); it++)
	{
		if (InstallInf(*it))
		{
			bRec = true;
			break;
		}
	}
    DRIVER_MANAGER->ScanForHardwareChange();
#endif	
	return bRec;
}
#endif