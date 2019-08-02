#include "stdafx.h"

#include "AndroidAdbHelper.h"
#include "Core/DeviceManager.h"
AndroidAdbHelper::AndroidAdbHelper()
{
    OPTION->Get(GlobalOption::STR_ADB_FILE_APTH, _strAdbExe);
}
 
AndroidAdbHelper::~AndroidAdbHelper()
{
}

void AndroidAdbHelper::ExecuteCmd(string strCmd, int nWaitTime)
{
    // 工作目录
    wstring strPath = CFileOperation::GetFileDir(_strAdbExe);
    if ( *strPath.rbegin() == '\\' || *strPath.rbegin() == '/' )
    {
        strPath.erase(strPath.size()-1);
    }

    // 环境变量
    SetEnvironmentVariable(L"ANDROID_SDK_HOME", strPath.c_str()); 

#ifdef WIN32
    PROCESS_INFORMATION pi;
    memset(&pi,0,sizeof(pi));   

	STARTUPINFO si;
	memset(&si,0,sizeof(si));   
	si.cb = sizeof(STARTUPINFO);   
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;   

    /*HANDLE*/void* houtfile = NULL;
	si.hStdError   = houtfile;
	si.hStdOutput  = houtfile;

    // 命令行参数
	wstring strCmd_w = CCodeOperation::GB2312ToUnicode(strCmd);
    int bufferSize = _strAdbExe.size() + strCmd_w.size() + 10;
    wchar_t* pBuffer = new wchar_t[bufferSize];
    swprintf_s(pBuffer, bufferSize, L"\"%s\" %s", _strAdbExe.c_str(), strCmd_w.c_str());

    // 启动 ADB server
    if (CreateProcessW(NULL, pBuffer, NULL, NULL, 
    	false,	// 不传递文件句柄  By QiuChang 2012.8.23
    	0, NULL, NULL, &si, &pi) )
        WaitForSingleObject(pi.hProcess, nWaitTime);

    delete[] pBuffer;
#else
	string adbPath = CCodeOperation::UnicodeToUTF_8(_strAdbExe);
	adbPath.insert(0, "\"");
	adbPath.push_back('"');
	adbPath += strCmd;

	system(adbPath.c_str());
	// timeout limit of nWaitTime : not implement
#endif
}

bool AndroidAdbHelper::isAdbWorking()
{
	if (CBaseSocket::isPortUsed(5037))
		return true;
	else
		return false;
}

bool AndroidAdbHelper::startServer()
{
	//多线程处理；防止同时几个命令进行startserver操作
	SetCSLock(DEVICE_MANAGER->GetInstance()->_csADBSSessionLock);
	if (isAdbWorking())
		return true;
	ExecuteCmd(ADB_CMD_START_SERVER);
	LOG->WriteError(L"adb common start server\r\n");
	Sleep(200);
	if (isAdbWorking())
		return true;
	Sleep(2000);
	if (isAdbWorking())
		return true;
	//log 等了2s还没启动成功
	Sleep(2000);
	if (isAdbWorking())
		return true;
	//log 等了4s还没启动成功
	//log ADB启动失败
	LOG->WriteError(L"adb start fail");
	return false;
}

bool AndroidAdbHelper::stopServer()
{
	ExecuteCmd(ADB_CMD_KILL_SERVER);
	return true;
/*
	Sleep(2000);
	if (!isAdbWorking())
		return true;
	//log 等了2s还在运行
	Sleep(2000);
	if (!isAdbWorking())
		return true;
	//log 等了4s还没启动成功
	//log ADB退出失败
	return false;
*/
}
