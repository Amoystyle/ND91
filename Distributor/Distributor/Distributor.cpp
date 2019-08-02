#include "stdafx.h" 
#include "Common/DebugHelper.h"

void LoadResource()
{
    TCHAR szPath[MAX_PATH];
    ::GetModuleFileName(NULL, szPath, MAX_PATH);
    TCHAR *pLastBias = NULL;
    if (pLastBias = _tcsrchr(szPath, '\\'))
        *(pLastBias + 1) = '\0';

	tstring frameworkPath = szPath;
	frameworkPath += _T("Resource\\Framework\\");
	GetUIRes()->InitResDir(frameworkPath.c_str());

    tstring resourcePath = szPath;
    resourcePath += _T("Resource\\MainSource\\");
	GetUIRes()->InitResDir(resourcePath.c_str(),true);//设置主资源

	/*
if (CSystemInfo::IsXP())
        GetUIRes()->SetDefaultFontType(UIFONT_GDIPLUS);
#ifdef _DEBUG
    else
        GetUIRes()->SetDefaultFontType(UIFONT_GDIPLUS);
#endif
		*/

}

void LoadOption()
{

	wstring strWorkingDir = CFileOperation::GetCurrWorkingDir();
	wstring _strOptionFile = strWorkingDir + _T("Option.ini");

	wstring str;
	// Adb文件地址
	OPTION->Set(GlobalOption::STR_ADB_FILE_APTH, CFileOperation::GetCurrWorkingDir() + _T("Adb.exe"));

	// 手机守护安装程序地址
	OPTION->Set(GlobalOption::STR_DEAMON_PATH,			CFileOperation::GetCurrWorkingDir() + _T("Android\\Assistance.apk"));
	// 手机助手安装程序地址
	OPTION->Set(GlobalOption::STR_PANDASPACE_PATH,		CFileOperation::GetCurrWorkingDir() + _T("Android\\pandaspaceweboneicon.apk"));
	// npk 临时文件目录
	OPTION->Set(GlobalOption::STR_ANDROID_NPK_TEMP_PATH, CONFIG(_strTemp) + _T("Android\\npktemp\\"));
	// 守护名称
	OPTION->Set(GlobalOption::STR_DEAMON_NAME,		CConfig::GetIniFileValue(_strOptionFile, _T("Android"), _T("deamon_name")));
	// 手机助手名称
	OPTION->Set(GlobalOption::STR_PANDASPACE_NAME,  CConfig::GetIniFileValue(_strOptionFile, _T("Android"), _T("space_name")));
	// 守护端口
	str = CConfig::GetIniFileValue(_strOptionFile, _T("Android"), _T("wifiport"));
	OPTION->Set(GlobalOption::INT_DEAMON_PORT, _wtoi(str.c_str()));
	// 安卓使用模式
	OPTION->Set(GlobalOption::INT_ANDROID_DEAMON_MODE, 1);//_wtoi(MyFileOperation::GetIniFileValue(_strOptionFile, _T("Android"), _T("deamonmode")).c_str()));
	// Ios使用模式
	OPTION->Set(GlobalOption::INT_IOS_DEAMON_MODE, _wtoi(CConfig::GetIniFileValue(_strOptionFile, _T("Ios"), _T("deamonmode")).c_str()));
	// 上传下载是否计算文件夹数量
	OPTION->Set(GlobalOption::INT_FILETASK_COUNTMODE, _wtoi(CConfig::GetIniFileValue(_strOptionFile, _T("Common"), _T("FileTaskCountMode")).c_str()));    

	// 安卓守护超时时间
	OPTION->Set(GlobalOption::INT_DEAMON_SILENCE_LIMIT, 10000);
}

// 禁止双开程序
void Instance()
{
	// 这里做单例检查
	HANDLE hMutex;
	hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, false, MUTEX_MAINAPP);
	if (hMutex)
	{
		LOG->WriteDebug(_T("检测到MUTEX 程序已运行，发送消息后退出"));
		//程序已经运行
		HWND pWndPrev = FindWindow( WNDTITLE_CLASSNAME ,NULL);
		if(pWndPrev) 
		{ 
			::PostMessage(pWndPrev, WM_ONLY,0,0);			
		}
		exit(0);
	}

	hMutex = ::CreateMutex(NULL, TRUE, MUTEX_MAINAPP);
	if (hMutex)
	{
		LOG->WriteDebug(_T("创建主程序MUTEX成功"));
	}
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

	OPTION->Set(GlobalOption::INT_LOG_SWITCH, 1);
	// Log文件地址
	OPTION->Set(GlobalOption::STR_LOG_FILE_PATH, CONFIG(_strLogPath) + APP_NAME);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_CHECK_EVERY_1024_DF); 

	Instance();	    // 禁止双开程序

	SetUnhandledExceptionFilter(UnhandledExceptionFilterNd);    //初始化Dump

    GetUIEngine()->InitSkin();

    LoadResource();
	
	LoadOption();

	CommonUIFunc::RegeditControl();
    
	MainDlg::GetInstance()->Init();

    GetUIEngine()->MessageLoop();
    GetUIEngine()->UnInitSkin();

    Singleton::Destory();

    return 0;
}
