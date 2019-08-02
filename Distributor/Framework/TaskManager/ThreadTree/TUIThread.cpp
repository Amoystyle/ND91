#include "StdAfx.h"
#include "TUIThread.h"
#include "ThreadTreeControl.h"

IMPLEMENT_DYNCREATE(TUIThread, CWinThread)
TUIThread::TUIThread(void)
{INTERFUN;
	//开始创建WND对象
	if(NULL == ThreadTreeControl::_g_giGlobalInfo->_g_lpUIWndClass)
	{
		AfxMessageBox(_T("ui wnd class is null!!"));
	}

	_m_lpUIWndClass = ThreadTreeControl::_g_giGlobalInfo->_g_lpUIWndClass;
	_m_lpUIWndInfo = ThreadTreeControl::_g_giGlobalInfo->_g_lpUIWndInfo;
	_m_wnd = NULL;

	_m_tiThreadInfo = NULL;

	_m_sLoadFinishEvent = ThreadTreeControl::TCreateEvent();
}

TUIThread::~TUIThread(void)
{INTERFUN;
	if(NULL != _m_sLoadFinishEvent)
	{
		ThreadTreeControl::TCloseSingle(_m_sLoadFinishEvent);
		_m_sLoadFinishEvent = NULL;
	}
}

BOOL TUIThread::InitInstance()
{INTERFUN;
	CWinThread::InitInstance();

	//获取线程操作锁，以确保线程节点队列中存在本对象
	ThreadTreeControl::_LockThreadControlMutex();

	//获取成员变量中的线程节点值
	_m_tiThreadInfo = ThreadTreeControl::_FindRunningThread(this);

	if(NULL == _m_tiThreadInfo)
	{
		//获取不到线程节点
#ifdef _DEBUG
		DebugBreak();
#endif
		ThreadTreeControl::_UnLockThreadControlMutex();
		return FALSE;
	}

	//获取结束锁
	ThreadTreeControl::_SetThreadStart(_m_tiThreadInfo);

	//获取线程唯一标识ID
	_m_tiThreadInfo->_m_ttThreadTarget.threadId = ::GetCurrentThreadId();

	//获取线程操作锁，以确保线程节点队列中存在本对象
	ThreadTreeControl::_UnLockThreadControlMutex();

	_m_wnd = this->CustomCreateWin();

	if(NULL == _m_wnd && NULL != _m_lpUIWndClass)
	{
		CObject * object = _m_lpUIWndClass->CreateObject();
		_m_wnd = (CWnd *)object;

		//创建窗口对象，不需要删除
		if(NULL != _m_lpUIWndInfo)
		{
			if(_m_lpUIWndInfo->bCreateDialogByID && _m_lpUIWndClass->IsDerivedFrom(RUNTIME_CLASS(CDialog)))
			{
				//当是CDialog子类时，需要使用CDialog创建方式
				((CDialog *) _m_wnd)->Create(_m_lpUIWndInfo->nID, _m_lpUIWndInfo->lpParentWnd);
			}
			else
			{
				_m_wnd->Create(_m_lpUIWndInfo->lpszClassName
					, _m_lpUIWndInfo->lpszWindowName
					, _m_lpUIWndInfo->dwStyle
					, _m_lpUIWndInfo->rect
					, _m_lpUIWndInfo->lpParentWnd
					, _m_lpUIWndInfo->nID
					, _m_lpUIWndInfo->pContext);
			}
		}
	}

	this->PreCreatedUIThread();

	ThreadTreeControl::TSetEventSingle(_m_sLoadFinishEvent);

	this->AfterCreatedUIThread();

	return TRUE;
}
	

int TUIThread::ExitInstance()
{INTERFUN;
	LPSINGLE tmpSingle = _m_sLoadFinishEvent;
	_m_sLoadFinishEvent = NULL;
	if(NULL != tmpSingle)
	{
		ThreadTreeControl::TCloseSingle(tmpSingle);
	}
	//删除本节点与子节点
	this->DelWnd();

	//释放结束锁
	HANDLE mutex = _m_tiThreadInfo->_m_hFinalMutex;

	//删除本节点，不删除子节点，并将子线程托管
	ThreadTreeControl::_DisposeThread(_m_tiThreadInfo);

	_m_tiThreadInfo = NULL;

	ReleaseMutex(mutex);

	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}
	

/************************************************************************************
 * @brief
 * 
 * 自定义的创建窗口函数，默认返回NULL，返回NULL则套用默认的窗口创建方式
 * @n<b>函数名称</b>: CustomCreateWin
 * @return           
 * @see              
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-7-22 14:22:39
 * @version	修改者        时间        描述@n
 * @n		alzq          2009-07-22 
 ************************************************************************************/
CWnd * TUIThread::CustomCreateWin(void)
{INTERFUN;
	return NULL;
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [在线程树的UI线程创建函数返回前将会调用本函数一次]
 * @n<b>函数名称</b>: PreCreatedUIThread
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-3-3 14:13:42
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-03-03  [描述]
 ******************************************************************************************************/
void TUIThread::PreCreatedUIThread(void)
{INTERFUN;
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [线程创建函数返回后将会调用本函数一次]
 * @n<b>函数名称</b>: AfterCreatedUIThread
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-3-3 14:14:06
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-03-03  [描述]
 ******************************************************************************************************/
void TUIThread::AfterCreatedUIThread(void)
{INTERFUN;
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [删除窗口函数，当不直接DELETE时需要重载此函数]
 * @n<b>函数名称</b>: DelWnd
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-3-3 15:35:23
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-03-03  [描述]
 ******************************************************************************************************/
void TUIThread::DelWnd(void)
{INTERFUN;
	SAFE_DELETE(_m_wnd);
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [获取创建的对象指针]
 * @n<b>函数名称</b>: GetWindow
 * @return            [返回值描述]
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-3-3 10:30:42
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-03-03  [描述]
 ******************************************************************************************************/
CWnd * TUIThread::GetWindow(void)
{INTERFUN;
	return _m_wnd;
}
	

/*******************************************************************************************************
 * @brief [函数简要描述]
 * 
 * [等待窗口创建完成]
 * @n<b>函数名称</b>: WaitForWndCreate
 * @see               [参见函数]
 * @n<b>作者</b>    :alzq
 * @n<b>创建时间</b>: 09-3-3 10:38:30
 * @version    修改者        时间        描述@n
 * @n          alzq          2009-03-03  [描述]
 ******************************************************************************************************/
bool TUIThread::WaitForWndCreate(void)
{INTERFUN;
	if(NULL == _m_sLoadFinishEvent)
	{
		return false;
	}
	
	//等待加载完后返回
	ThreadTreeControl::TWaitForSingle(_m_sLoadFinishEvent, INFINITE);
	return true;
}

BEGIN_MESSAGE_MAP(TUIThread, CWinThread)
END_MESSAGE_MAP()
