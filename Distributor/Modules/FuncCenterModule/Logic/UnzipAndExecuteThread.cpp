#include "StdAfx.h"
#include "UnzipAndExecuteThread.h"
#include "TlHelp32.h"

UnzipAndExecuteThread::UnzipAndExecuteThread(void)
{
}

UnzipAndExecuteThread::~UnzipAndExecuteThread(void)
{
}

void UnzipAndExecuteThread::SetDownPath(wstring wsPath, wstring wsExeBinPath, string strPlat, string strFunName)
{
	_wsPath = wsPath;
	_wsExeBinPath = wsExeBinPath;
	_strPlat = strPlat;
	_strFunName = strFunName;
}


unsigned long UnzipAndExecuteThread::Main()
{
	if (!this->IsSoftRunning())
	{
		wstring exePath = CFileOperation::GetFileDir(_wsPath) + CFileOperation::GetFileNameNoExt(_wsPath);
		if (!CFileOperation::IsDirExist(exePath.c_str()))
		{
			CFileOperation::MakeLocalDir(exePath);
		}
		bool bReturn = CFileOperation::UnZipFile(_wsPath, exePath);

		if (bReturn)
		{//待制作
			BaseOperation::Execute(exePath + _wsExeBinPath);
		}
		//检查是否在运行
		if (!this->IsSoftRunning())
		{
			string strTab = _strPlat + "_" + _strFunName + "_" + "运行失败";
			CStaticsticsMessageWithTab StaticsticsMsg("900011", CCodeOperation::GB2312ToUTF_8(strTab));
			StaticsticsMsg.SendAndGetReturn();

			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(L"FuniCenter_RunningFaield"), ICON_WARNING);
		}
		else
		{
			string strTab = _strPlat + "_" + _strFunName + "_" + "运行成功";
			CStaticsticsMessageWithTab StaticsticsMsg("900011", CCodeOperation::GB2312ToUTF_8(strTab));
			StaticsticsMsg.SendAndGetReturn();
		}
	}

	return 0;
}

BOOL UnzipAndExecuteThread::IsSoftRunning()
{
	 wstring wsFileName = CFileOperation::GetFileNameNoExt(_wsExeBinPath) + L".exe";

     PROCESSENTRY32 pe32;
	 pe32.dwSize = sizeof(pe32);
	 HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	 if (hProcessSnap == INVALID_HANDLE_VALUE)
	 { 
		 return FALSE; 
	 } 
	 BOOL bIsRunning = FALSE;
	 BOOL bMore = ::Process32First(hProcessSnap,&pe32);
	 while (bMore) 
	 {  
		if (0 == wcsicmp(pe32.szExeFile,wsFileName.c_str()))
		//你想检测的程序  
		{   
			bIsRunning = TRUE;
			break;  
	    } 
		bMore = ::Process32Next(hProcessSnap,&pe32); 
	 } 
	 ::CloseHandle(hProcessSnap);

	 return bIsRunning;
}