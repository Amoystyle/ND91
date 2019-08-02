#pragma once

//#include "VersionCheck.h"
#include <DbgHelp.h>
#include "Base/ClientInfo.h"
#pragma auto_inline (off)
#pragma comment(lib, "DbgHelp.lib")
//#include "mpp_util.h"
//#include "Main.h"

//#include <ShellAPI.h>
/*LONG WINAPI UnhandledExceptionFilterNd(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	static int nCount = 0;
	if (nCount != 0)
		return EXCEPTION_EXECUTE_HANDLER;
	nCount++;

	SYSTEMTIME systime = {0};
	TCHAR  pDataBuffer[MAX_PATH] = {0};
	GetLocalTime(&systime);
	

	_stprintf_s(pDataBuffer, _T("Dump\\%s.dmp"), g_main.GetCoreLogFileNamePart());

	HANDLE lhDumpFile = CreateFile(pDataBuffer, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL ,NULL);
	MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
	loExceptionInfo.ExceptionPointers = ExceptionInfo;
	loExceptionInfo.ThreadId = GetCurrentThreadId();
	loExceptionInfo.ClientPointers = TRUE;
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);
	CloseHandle(lhDumpFile);
	std::wstring wstrPath = L"";
	{
		WCHAR szModulePath[MAX_PATH]={0}; 
		GetModuleFileName( NULL, szModulePath, MAX_PATH );
		WCHAR *pLastBias = NULL;
		if (pLastBias = wcsrchr(szModulePath, '\\'))
			*(pLastBias + 1) = '\0';
		wstrPath = szModulePath;
	}

	std::wstring wstrMailMsgExe = wstrPath + L"\\SendDump.exe";
	std::wstring wstrParam = L"\"" + wstrPath + pDataBuffer + L"\" \"";
	wstrParam += g_main.GetCoreLogFileName();
	WCHAR szModulePath[MAX_PATH]={0}; 
	GetModuleFileName( NULL, szModulePath, MAX_PATH );
	wstrParam += L"\" \"";
	wstrParam += szModulePath;
	wstrParam += L"\"";
	::ShellExecute(NULL, L"open", wstrMailMsgExe.c_str(), wstrParam.c_str(), NULL, SW_SHOWNORMAL);//在此可以提示是否发送dump给开发人员
	return EXCEPTION_EXECUTE_HANDLER;
}*/
void StarDumpColApp(TCHAR  pDataBuffer[MAX_PATH], wstring strOthers);

LONG WINAPI UnhandledExceptionFilterNd(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	
    LOG->WriteDebug(L"程序出现错误,进入Dump处理过程");

	static int nCount = 0;
	if (nCount != 0)
		return EXCEPTION_EXECUTE_HANDLER;
	nCount++;
	SYSTEMTIME systime = {0};
	TCHAR  pDataBuffer[MAX_PATH] = {0};
    TCHAR  pTimeBuffer[MAX_PATH] = {0};
	GetLocalTime(&systime);

	//std::wstring strFileName = L"12345";    
	//_stprintf_s(pDataBuffer, L"Dump\\%s.dmp", strFileName.c_str());//pDataBuffer这个就是要存放dump文件路径
        
    //pDataBuffer这个就是要存放dump文件路径
    _stprintf_s(pTimeBuffer, L"%d-%d-%d %d-%d-%d"
        , systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);

    _stprintf_s(pDataBuffer, L"%s%s.dmp"
        , CONFIG(_strDumpPath).c_str()
        , pTimeBuffer);   
	
    wstring strFolder = CFileOperation::GetFileDir(pDataBuffer);
    CFileOperation::MakeLocalDir(strFolder); 
    wstring strDumpFile = pDataBuffer;
    LOG->WriteDebug(L"开始进行Dump处理 " + strDumpFile);
	HANDLE lhDumpFile = CreateFile(pDataBuffer, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL ,NULL);
	MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
	loExceptionInfo.ExceptionPointers = ExceptionInfo;
	loExceptionInfo.ThreadId = GetCurrentThreadId();
	loExceptionInfo.ClientPointers = TRUE;
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);
	CloseHandle(lhDumpFile); 
	
    LOG->WriteDebug(L"完成Dump处理,开始准备提交反馈接口");
	StarDumpColApp(pDataBuffer,L"");
#ifdef IS_UPLOAD_DUMP
    // 上传
    sFeedBackInfo* pInfo = new sFeedBackInfo();
    pInfo->itemCode = ITEM_CODE;
    pInfo->strProblemType = BaseUI::LoadString(_T("App_Collapse_text"));     //问题类型
    pInfo->strAddAccount = APP_TITLE;        //提交帐号
	/*VersionCheck version_check;*/
	pInfo->strSoftVersion = CClientInfo::GetInstance()->GetVersion();//version_check.GetIniVersion();         //软件版本
    pInfo->strPhoneModelName = MyFileOperation::GetIniFileValue(CONFIG(_strInfoFile), L"CurPhoneInfo", L"DeviceType");      //手机型号
    pInfo->strPhoneSystem = pInfo->strPhoneModelName;
    pInfo->strFirmWare = MyFileOperation::GetIniFileValue(CONFIG(_strInfoFile), L"CurPhoneInfo", L"DeviceVer");;            //固件版本
    pInfo->strConnectMode = L"USB";       //连接方式
    pInfo->strSystemInfo = L"";           //系统信息
    pInfo->strContactDescription = L"";   //联系方式
    pInfo->strProblemDescription = L"";   //问题描述
    wstring strZip = pDataBuffer; 
    strZip += L".zip";
    CFileOperation::ZipFile(pDataBuffer, strZip);
    pInfo->strFilePath = strZip;             //附件路径  
    LOG->WriteDebug(L"提交反馈接口");
    FeedbackInterface fdb;
    int nRec = fdb.Feedback(pInfo);    
    LOG->WriteDebug(L"反馈接口返回 " + CStrOperation::IntToWString(nRec));
    MyFileOperation::SetIniFileValue(CONFIG(_strInfoFile), L"FeedBack", L"FeedBackTime", pTimeBuffer);
    MyFileOperation::SetIniFileValue(CONFIG(_strInfoFile), L"FeedBack", L"FeedBackResult", CStrOperation::IntToWString(nRec));
#endif

    //MessageBox(NULL, BaseUI::LoadString(_T("Sorry_AppError_text")), APP_TITLE, MB_ICONERROR | MB_OK);
    LOG->WriteDebug(L"Dump处理完成!");
	return EXCEPTION_EXECUTE_HANDLER;
}


//param pDataBuffer 需要上传的dump的路径
//param strOthers 需要上传的附加信息
void StarDumpColApp(TCHAR  pDataBuffer[MAX_PATH], wstring strOthers)
{
	wstring strExePath = CFileOperation::GetCurrWorkingDir() + APP_EXENAME;
	wstring dumpColExePath = CFileOperation::GetCurrWorkingDir() + L"dpa.exe";
	wstring params = pDataBuffer;

	wstring logPath = CONFIG(_strLogPath);
	vector<wstring> logFiles;
	if ( CFileOperation::IsDirExist(logPath.c_str()) )
	{
		CFileOperation::FindAllFileInDir(logPath,logFiles,_T("log"));
	}
	
	
	params += L";" + strExePath + L";";
	//VersionCheck version_check;
	wstring strVersion = CCodeOperation::s2ws(CClientInfo::GetInstance()->GetVersion());//version_check.GetNowVersion();
	params += strVersion + L";" + strOthers;
	if ( logFiles.size()>0 )
	{
		params += L";" + logFiles[0];
	}
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = dumpColExePath.c_str();//执行程序路径
	ShExecInfo.lpParameters = params.c_str();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
}

