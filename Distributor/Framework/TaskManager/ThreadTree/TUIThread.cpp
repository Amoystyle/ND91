#include "StdAfx.h"
#include "TUIThread.h"
#include "ThreadTreeControl.h"

IMPLEMENT_DYNCREATE(TUIThread, CWinThread)
TUIThread::TUIThread(void)
{INTERFUN;
	//��ʼ����WND����
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

	//��ȡ�̲߳���������ȷ���߳̽ڵ�����д��ڱ�����
	ThreadTreeControl::_LockThreadControlMutex();

	//��ȡ��Ա�����е��߳̽ڵ�ֵ
	_m_tiThreadInfo = ThreadTreeControl::_FindRunningThread(this);

	if(NULL == _m_tiThreadInfo)
	{
		//��ȡ�����߳̽ڵ�
#ifdef _DEBUG
		DebugBreak();
#endif
		ThreadTreeControl::_UnLockThreadControlMutex();
		return FALSE;
	}

	//��ȡ������
	ThreadTreeControl::_SetThreadStart(_m_tiThreadInfo);

	//��ȡ�߳�Ψһ��ʶID
	_m_tiThreadInfo->_m_ttThreadTarget.threadId = ::GetCurrentThreadId();

	//��ȡ�̲߳���������ȷ���߳̽ڵ�����д��ڱ�����
	ThreadTreeControl::_UnLockThreadControlMutex();

	_m_wnd = this->CustomCreateWin();

	if(NULL == _m_wnd && NULL != _m_lpUIWndClass)
	{
		CObject * object = _m_lpUIWndClass->CreateObject();
		_m_wnd = (CWnd *)object;

		//�������ڶ��󣬲���Ҫɾ��
		if(NULL != _m_lpUIWndInfo)
		{
			if(_m_lpUIWndInfo->bCreateDialogByID && _m_lpUIWndClass->IsDerivedFrom(RUNTIME_CLASS(CDialog)))
			{
				//����CDialog����ʱ����Ҫʹ��CDialog������ʽ
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
	//ɾ�����ڵ����ӽڵ�
	this->DelWnd();

	//�ͷŽ�����
	HANDLE mutex = _m_tiThreadInfo->_m_hFinalMutex;

	//ɾ�����ڵ㣬��ɾ���ӽڵ㣬�������߳��й�
	ThreadTreeControl::_DisposeThread(_m_tiThreadInfo);

	_m_tiThreadInfo = NULL;

	ReleaseMutex(mutex);

	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}
	

/************************************************************************************
 * @brief
 * 
 * �Զ���Ĵ������ں�����Ĭ�Ϸ���NULL������NULL������Ĭ�ϵĴ��ڴ�����ʽ
 * @n<b>��������</b>: CustomCreateWin
 * @return           
 * @see              
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-7-22 14:22:39
 * @version	�޸���        ʱ��        ����@n
 * @n		alzq          2009-07-22 
 ************************************************************************************/
CWnd * TUIThread::CustomCreateWin(void)
{INTERFUN;
	return NULL;
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [���߳�����UI�̴߳�����������ǰ������ñ�����һ��]
 * @n<b>��������</b>: PreCreatedUIThread
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-3-3 14:13:42
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-03-03  [����]
 ******************************************************************************************************/
void TUIThread::PreCreatedUIThread(void)
{INTERFUN;
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�̴߳����������غ󽫻���ñ�����һ��]
 * @n<b>��������</b>: AfterCreatedUIThread
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-3-3 14:14:06
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-03-03  [����]
 ******************************************************************************************************/
void TUIThread::AfterCreatedUIThread(void)
{INTERFUN;
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [ɾ�����ں���������ֱ��DELETEʱ��Ҫ���ش˺���]
 * @n<b>��������</b>: DelWnd
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-3-3 15:35:23
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-03-03  [����]
 ******************************************************************************************************/
void TUIThread::DelWnd(void)
{INTERFUN;
	SAFE_DELETE(_m_wnd);
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [��ȡ�����Ķ���ָ��]
 * @n<b>��������</b>: GetWindow
 * @return            [����ֵ����]
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-3-3 10:30:42
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-03-03  [����]
 ******************************************************************************************************/
CWnd * TUIThread::GetWindow(void)
{INTERFUN;
	return _m_wnd;
}
	

/*******************************************************************************************************
 * @brief [������Ҫ����]
 * 
 * [�ȴ����ڴ������]
 * @n<b>��������</b>: WaitForWndCreate
 * @see               [�μ�����]
 * @n<b>����</b>    :alzq
 * @n<b>����ʱ��</b>: 09-3-3 10:38:30
 * @version    �޸���        ʱ��        ����@n
 * @n          alzq          2009-03-03  [����]
 ******************************************************************************************************/
bool TUIThread::WaitForWndCreate(void)
{INTERFUN;
	if(NULL == _m_sLoadFinishEvent)
	{
		return false;
	}
	
	//�ȴ�������󷵻�
	ThreadTreeControl::TWaitForSingle(_m_sLoadFinishEvent, INFINITE);
	return true;
}

BEGIN_MESSAGE_MAP(TUIThread, CWinThread)
END_MESSAGE_MAP()
