#include "StdAfx.h"
#include "ThreadLog.h"
#pragma warning( disable: 4996 )
CTCHAR * ThreadLog::_g_csFilePath;							//�ļ�·��
CRITICAL_SECTION ThreadLog::_g_csLogWritSection;			//LOG��¼д��ʱ����
fstream ThreadLog::_g_fsLogStream;

ThreadLog::ThreadLog(void)
{INTERFUN;
	_g_csFilePath = NULL;
}

ThreadLog::~ThreadLog(void)
{INTERFUN;
	if(NULL != _g_csFilePath)
	{
		delete [] _g_csFilePath;
		_g_csFilePath = NULL;
	}
}
	

void ThreadLog::InitThreadLog(const CTCHAR * logFilePath)
{INTERFUN;
	//���ļ�
	size_t len = _tcslen(logFilePath);
	_g_csFilePath = new CTCHAR[len + 1];
	_tcsncpy(_g_csFilePath, logFilePath, len);
	_g_csFilePath[len] = _T('\0');

	_g_fsLogStream.open(logFilePath, ios::out|ios::trunc);

	//�����������
	InitializeCriticalSection(&_g_csLogWritSection);			//�߳̽ڵ������

	//�����¼��ʼ
	AddLog("==========================");
	AddLog("Thread log start ...");
}
	

void ThreadLog::CloseThreadLog(void)
{INTERFUN;
	AddLog("Thread log end ...");
	AddLog("==========================");
	_g_fsLogStream.flush();
	_g_fsLogStream.close();

	if(NULL != _g_csFilePath)
	{
		delete [] _g_csFilePath;
		_g_csFilePath = NULL;
	}

	DeleteCriticalSection(&_g_csLogWritSection);
}
	

void ThreadLog::AddLog(ThreadInfo * thread, const char * mes)
{INTERFUN;
	_LockLog();

	//��ȡ��ǰʱ��
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	CStringA str;

	if(thread)
	{
		if(thread->_m_ttThreadTarget.isMainThread)
		{
			str.Format("%i-%i-%i %i:%i:%i -- mainthread --\n  --  %s"
				, systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute
				, systemTime.wSecond, mes);
		}
		else
		{
			str.Format("%i-%i-%i %i:%i:%i -- Thread name: \"%s\" id  : %i -- address: 0x%x\n  --  %s"
				, systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute
				, systemTime.wSecond, thread->_m_csThreadName, thread->_m_ttThreadTarget.threadId, thread, mes);
		}
	}
	else
	{
		str.Format("%i-%i-%i %i:%i:%i -- Thread NULL --\n  --  %s"
			, systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute
			, systemTime.wSecond, mes);
	}

	_g_fsLogStream.write(str, str.GetLength());
	_g_fsLogStream.write("\n", 1);

	_g_fsLogStream.flush();

	_UnLockLog();
}
	

void ThreadLog::AddLog(const char * mes)
{INTERFUN;
	_LockLog();

	//��ȡ��ǰʱ��
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	CStringA str;
	str.Format("%i-%i-%i %i:%i:%i -- message  --  %s", systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute
		, systemTime.wSecond, mes);

	_g_fsLogStream.write(str, str.GetLength());
	_g_fsLogStream.write("\n", 1);

	_g_fsLogStream.flush();

	_UnLockLog();
}
	


void ThreadLog::_LockLog(void)
{INTERFUN;
	EnterCriticalSection(&_g_csLogWritSection);
}
	

void ThreadLog::_UnLockLog(void)
{INTERFUN;
	LeaveCriticalSection(&_g_csLogWritSection);
}

