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
	GetUIRes()->InitResDir(resourcePath.c_str(),true);//��������Դ

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
	// Adb�ļ���ַ
	OPTION->Set(GlobalOption::STR_ADB_FILE_APTH, CFileOperation::GetCurrWorkingDir() + _T("Adb.exe"));

	// �ֻ��ػ���װ�����ַ
	OPTION->Set(GlobalOption::STR_DEAMON_PATH,			CFileOperation::GetCurrWorkingDir() + _T("Android\\Assistance.apk"));
	// �ֻ����ְ�װ�����ַ
	OPTION->Set(GlobalOption::STR_PANDASPACE_PATH,		CFileOperation::GetCurrWorkingDir() + _T("Android\\pandaspaceweboneicon.apk"));
	// npk ��ʱ�ļ�Ŀ¼
	OPTION->Set(GlobalOption::STR_ANDROID_NPK_TEMP_PATH, CONFIG(_strTemp) + _T("Android\\npktemp\\"));
	// �ػ�����
	OPTION->Set(GlobalOption::STR_DEAMON_NAME,		CConfig::GetIniFileValue(_strOptionFile, _T("Android"), _T("deamon_name")));
	// �ֻ���������
	OPTION->Set(GlobalOption::STR_PANDASPACE_NAME,  CConfig::GetIniFileValue(_strOptionFile, _T("Android"), _T("space_name")));
	// �ػ��˿�
	str = CConfig::GetIniFileValue(_strOptionFile, _T("Android"), _T("wifiport"));
	OPTION->Set(GlobalOption::INT_DEAMON_PORT, _wtoi(str.c_str()));
	// ��׿ʹ��ģʽ
	OPTION->Set(GlobalOption::INT_ANDROID_DEAMON_MODE, 1);//_wtoi(MyFileOperation::GetIniFileValue(_strOptionFile, _T("Android"), _T("deamonmode")).c_str()));
	// Iosʹ��ģʽ
	OPTION->Set(GlobalOption::INT_IOS_DEAMON_MODE, _wtoi(CConfig::GetIniFileValue(_strOptionFile, _T("Ios"), _T("deamonmode")).c_str()));
	// �ϴ������Ƿ�����ļ�������
	OPTION->Set(GlobalOption::INT_FILETASK_COUNTMODE, _wtoi(CConfig::GetIniFileValue(_strOptionFile, _T("Common"), _T("FileTaskCountMode")).c_str()));    

	// ��׿�ػ���ʱʱ��
	OPTION->Set(GlobalOption::INT_DEAMON_SILENCE_LIMIT, 10000);
}

// ��ֹ˫������
void Instance()
{
	// �������������
	HANDLE hMutex;
	hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, false, MUTEX_MAINAPP);
	if (hMutex)
	{
		LOG->WriteDebug(_T("��⵽MUTEX ���������У�������Ϣ���˳�"));
		//�����Ѿ�����
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
		LOG->WriteDebug(_T("����������MUTEX�ɹ�"));
	}
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

	OPTION->Set(GlobalOption::INT_LOG_SWITCH, 1);
	// Log�ļ���ַ
	OPTION->Set(GlobalOption::STR_LOG_FILE_PATH, CONFIG(_strLogPath) + APP_NAME);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_CHECK_EVERY_1024_DF); 

	Instance();	    // ��ֹ˫������

	SetUnhandledExceptionFilter(UnhandledExceptionFilterNd);    //��ʼ��Dump

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
