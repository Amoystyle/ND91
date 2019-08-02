#include "stdafx.h"
#include "BaseThread.h"

#include "../Manager/ThreadManager.h"
#include "../Manager/EventManager.h"
#include <dbghelp.h>
#include <excpt.h>


BaseThread::BaseThread( const string id )
:_id(id)
, _exit(false)
, _autoDelete(true)
{
     _hThread = NULL;

	 strPrintThreadID = "";
    // 复制系统退出的Event
    _hSystemExitEvent = Singleton::GetEventManager()->SystemExitEvent();
    _pSystemExit= Singleton::GetEventManager()->SystemExitFlag();

    // 没传入id时，自动分配一个id
	if ( _id.empty())
        _id =  CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());

    if ( ! _id.empty() )
    {
        Singleton::GetThreadManager()->InsertThread(this, id);
    }

    _hExitEvent = CreateEvent(NULL, true, false, NULL);
}

BaseThread::~BaseThread()
{
    if ( _hExitEvent )
        CloseHandle(_hExitEvent);

    if ( ! _id.empty() )
    {
        Singleton::GetThreadManager()->RemoveThread(this);
    }

    if ( _hThread )
        CloseHandle(_hThread);
}

// 使用动态load DLL的方式，减少依赖
void CreateMiniDump(EXCEPTION_POINTERS* pExcPtrs) 
{   
	LOG->WriteDebug(L"Begin--CreateMiniDump()");

    // Load dbghelp.dll
    HMODULE hDbgHelp = LoadLibrary(_T("dbghelp.dll"));
    if(hDbgHelp==NULL)  return;   // Error - couldn't load dbghelp.dll

	LOG->WriteDebug(L"LoadLibrary(_T(dbghelp.dll))--success");

    // make the dump file name
    SYSTEMTIME systime = {0};
    GetLocalTime(&systime);
    TCHAR buffer[1024] = {0};
    //_stprintf_s(buffer, _T("Dump\\%d-%d-%d %d-%d-%d(%u).dmp"),
    //    systime.wYear, systime.wMonth, systime.wDay,
    //    systime.wHour, systime.wMinute, systime.wSecond, GetCurrentThreadId());

	_stprintf_s(buffer, L"%s%d-%d-%d %d-%d-%d(%u).dmp"
		, CONFIG(_strDumpPath).c_str(),
		systime.wYear, systime.wMonth, systime.wDay,
		systime.wHour, systime.wMinute, systime.wSecond, GetCurrentThreadId());

	LOG->WriteDebug(L"获得路径--success");

	wstring strFolder = CFileOperation::GetFileDir(buffer);
	if (!CFileOperation::IsDirExist(strFolder.c_str()))
	{
		CFileOperation::MakeLocalDir(strFolder); 
	}

	LOG->WriteDebug(L"MakeLocalDir(strFolder)--success");

    // Create the minidump file
    HANDLE hFile = CreateFile(buffer, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile==INVALID_HANDLE_VALUE)   return;   // Couldn't create file

	LOG->WriteDebug(L"创建文件--success");

    typedef BOOL (WINAPI *LPMINIDUMPWRITEDUMP)(HANDLE hProcess,
        DWORD ProcessId, HANDLE hFile, MINIDUMP_TYPE DumpType, 
        CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, 
        CONST PMINIDUMP_USER_STREAM_INFORMATION UserEncoderParam, 
        CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

    LPMINIDUMPWRITEDUMP pfnMiniDumpWriteDump = (LPMINIDUMPWRITEDUMP)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
    if(!pfnMiniDumpWriteDump) return;  // Bad MiniDumpWriteDump function

	LOG->WriteDebug(L"GetProcAddress(hDbgHelp, MiniDumpWriteDump)--success");

    MINIDUMP_EXCEPTION_INFORMATION mei;
    mei.ThreadId = GetCurrentThreadId();
    mei.ExceptionPointers = pExcPtrs;
    mei.ClientPointers = FALSE;

    MINIDUMP_CALLBACK_INFORMATION mci;
    mci.CallbackRoutine = NULL;
    mci.CallbackParam = NULL;

	LOG->WriteDebug(L"Begin--MiniDumpWriteDump");

    // Write minidump to the file
    BOOL bWriteDump = pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &mei, NULL, &mci);
    
	LOG->WriteDebug(L"End--MiniDumpWriteDump");

	if(!bWriteDump) return; // Error writing dump.
    CloseHandle(hFile);     // Close file
    FreeLibrary(hDbgHelp);  // Unload dbghelp.dll

	LOG->WriteDebug(L"End--CreateMiniDump()");

}

// struct ThreadFail
// {
//     unsigned int u;
//     PEXCEPTION_POINTERS pExp;
// };
// 
// void my_trans_func(unsigned int u, PEXCEPTION_POINTERS pExp )
// {
//     ThreadFail tf;
//     tf.u = u;
//     tf.pExp = pExp;
//     throw tf;
// }
// 

int filter(unsigned int code, struct _EXCEPTION_POINTERS *p)
{
    CreateMiniDump(p);
    switch(code)
    {
    case EXCEPTION_ACCESS_VIOLATION:
        return EXCEPTION_EXECUTE_HANDLER;
    default:
        return EXCEPTION_CONTINUE_SEARCH;
    };
}

void BaseThread::InitPrintThreadID()
{
	if ( 0 != atoi(this->_id.c_str()) )
	{
		strPrintThreadID = typeid(*this).name();
		strPrintThreadID += ": ";
	}
	strPrintThreadID += this->_id;
}

DWORD WINAPI BaseThread::ThreadMain( LPVOID lpParam )
{
    RETURN_NULL_IF(! lpParam);

    BaseThread* pThread = (BaseThread*) lpParam;

// DEBUG时激活异常
#ifdef _DEBUG

    LOG->Write(Log::LOG_DEBUG, L"thread begin : %S !",
        CCodeOperation::GB2312ToUnicode(pThread->strPrintThreadID).c_str());

    pThread->Main();

    LOG->Write(Log::LOG_DEBUG, L"thread end   : %S !",
        CCodeOperation::GB2312ToUnicode(pThread->strPrintThreadID).c_str());    

    if ( pThread->_autoDelete )
	{
		Singleton::GetThreadManager()->RemoveThread(pThread);
        delete pThread;
        pThread = NULL;
    }

#else
    __try
    {
        __try
        {
			LOG->Write(Log::LOG_DEBUG, L"Begin--BaseThread::Main() ID=%S", pThread->strPrintThreadID.c_str());

	        pThread->Main();

			LOG->Write(Log::LOG_DEBUG, L"End--BaseThread::Main() ID=%S", pThread->strPrintThreadID.c_str());
        }
        __finally
        {  

            LOG->Write(Log::LOG_DEBUG, L"thread %S exit %s!", pThread->strPrintThreadID.c_str(), 
                AbnormalTermination() ? L"\tabnormal" : L"\tnormal");
            if ( !AbnormalTermination() )
            {
                if ( pThread->_autoDelete )
                {
                    delete pThread;
                    pThread = NULL;
                }
            }
            ExitThread(0);
        }
    }
    __except(filter(GetExceptionCode(), GetExceptionInformation()))
    {
        LOG->Write(Log::LOG_DEBUG, L"Thread Crash !");
        ExitThread(-1);
    }
#endif

    return 0L;
}

bool BaseThread::Run()
{
    unsigned long   dwThreadId;

	this->InitPrintThreadID();

    _hThread = CreateThread( 
        NULL,           // default security attributes
        0,              // use default stack size  
        ThreadMain,     // thread function 
        LPVOID(this),   // argument to thread function 
        0,              // use default creation flags 
        &dwThreadId);   // returns the thread identifier 

    LOG->Write(Log::LOG_DEBUG, L"Start thead : %d", _hThread);
    return _hThread != NULL;        
}

void BaseThread::Exit()
{
    _exit = true;
    SetEvent(_hExitEvent);
}

bool BaseThread::WaitExit(DWORD dwMilliseconds)
{
    HANDLE events[2];
    events[0] = _hSystemExitEvent;
    events[1] = _hExitEvent;

    return WaitForMultipleObjects(2, events, false, dwMilliseconds) != WAIT_TIMEOUT;
}

bool BaseThread::CheckExit()
{
    return (_exit || *_pSystemExit);
}

void BaseThread::SetAutoDelete( bool value )
{
    _autoDelete = value;
}
