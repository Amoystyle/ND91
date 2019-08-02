/*****************************************************************************
 *  @file 
 *  @brief [�߳����Ŀ����࣬�ṩ�߳����е�ȫ�ֿ��Ʒ�����ͬʱ�������߳��������ڵ㣬�źŽڵ����ɾ]
 * 
 *  <b>�ļ���</b>      : ThreadTreeControl.h
 *  @n@n<b>��Ȩ����</b>: �����������Ӧ�����������
 *  @n@n<b>��  ��</b>  : alzq
 *  @n@n<b>����ʱ��</b>: 09-2-5 14:38:51
 *  @n@n<b>�ļ�����</b>: 
 *  @version  �汾        �޸���        ʱ��        ����@n
 *  @n        [�汾��]    alzq          2009-02-05  [����]
 * 
*******************************************************************************/
#ifndef __THREAD_TREE_CONTROL__
#define __THREAD_TREE_CONTROL__

#include "ThreadControl.h"
//#include "TUIThread.h"

#pragma once

typedef list<ThreadInfo *>	WAITINGTHREADLIST,RUNNINGTHREADLIST;
typedef SINGLE *			LPSINGLE;
typedef ThreadControl *		LPTHREADCONTROL;
typedef ThreadInfo *		LPTHREADINFO;

//�����Զ�����߳��еĲ����ṹ�壬�������û�����ָ��������̲߳���
struct CUSTOMTHREADINFO
{
	USER_FUNCTION userFunc;			//�û�����ָ��
	ThreadInfo * threadInfo;		//�߳�ָ��
};

//���붨ʱ�̵߳�����EVENT��HANDLE
struct TimeControlEvents
{
	HANDLE timeEvent;
	HANDLE finishEvent;
};

//�����߳�ɾ�����߳��еĽṹ��
struct DeleteThreadInfo
{
public:
	HANDLE timeEvent;					//��ʱʹ�õ��ź���
	HANDLE finishEvent;					//�����ı�ʾ�ź���
	bool exitThread;					//�Ƿ���Ҫ�˳�ɾ���߳�
	THREADLIST threadList;				//�ȴ�ɾ�����̵߳�ַ����

	DeleteThreadInfo()
	{
		timeEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		finishEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		threadList.clear();
		exitThread = false;
	}
	~DeleteThreadInfo()
	{
		exitThread = true;
		if(NULL != timeEvent)
		{
			CloseHandle(timeEvent);
			timeEvent = NULL;
		}
		if(NULL != finishEvent)
		{
			CloseHandle(finishEvent);
			finishEvent = NULL;
		}
		threadList.clear();
	}
};

struct ThreadTreeGlobalInfo
{
	/**
	 *	--�˱�����ʱ��Ч
	 **/
	int _g_iThreadExitTimeLimit;						//�߳��˳�ʱ��ʱ������

	//static HANDLE _g_hThreadControlMutex;					//�߳̽ڵ������
	//static HANDLE _g_hSingleControlMutex;					//�ź����������в�����
	/**
	 *	--����SECTION��֤��ͬһʱ��ֻ��һ���߳��ڶ�SINGLE���̶߳��н��в���
	 **/
	//CRITICAL_SECTION _g_csThreadControlSection;		//�߳̽ڵ������
	//CRITICAL_SECTION _g_csSingleControlSection;		//�ź����������в�����
	HANDLE _g_csThreadControlMutex;		//�߳̽ڵ������
	HANDLE _g_csSingleControlMutex;		//�ź����������в�����

	/**
	 *	--���ж�ʱ����ʱ������ź�����WINDOWS API���ź�����
	 **/
	HANDLE _g_hTimeThreadEvent;						//��ʱѭ���ź�
	HANDLE _g_hTimeThreadFinishEvent;				//����˳���ʱ�߳�ʱ���źŵ��ź���

	/**
	 *	--�洢�Ѿ�������SINGLE�Ķ��У���������������Ϊ�˷��㴴��SINGLEʱ��ѯͬ��SINGLEʱ������
	 **/
	EventList _g_elNamedEventList;					//�Ѿ����������ź����ڵ��б�
	MutexList _g_mlNamedMutexList;					//�Ѿ������������ڵ��б�
	SINGLELIST _g_slSingleList;						//�����߳�Ϊ�յ�SINGLE����

	/**
	 *	--�洢�߳���ض��У���һ��Ϊ�˱��������̶߳������ģ���������Ҫ�����߳����Ϳ��Բ�ѯ
	 *	--����ʱ��ȴ��������ڶ�ʱ�̹߳����߳��У�������ʱ��ȴ��̵߳Ĳ�������
	 *	--��Ҫɾ�����̶߳��У��Ƕ��Ѿ��رյ��߳̽ڵ���л���ɾ���Ķ��У�����ȫ�˳��̺߳�ŶԽڵ����ɾ��
	 **/
	RUNNINGTHREADLIST _g_rtlThreadList;				//�����˵��߳̽ڵ�洢����
	WAITINGTHREADLIST _g_wtlTimeLimitWaitingThread;	//����ʱ��ȴ��̶߳���
	DeleteThreadInfo * _g_dtiDeleteThreadInfo;		//��Ҫɾ�����߳̽ڵ��ַ

	/**
	 *	--�߳������ڵ�
	 **/
	TreeItem_t * _g_treThreadTree;					//�߳����ĸ��ڵ�

	/**
	 *	--���ڴ���UI�߳�ʱ����ָ��洢����ֻ������ʱ�Դ洢��ֹ��ͬ�̷߳���ʱ��ͻ
	 **/
//	CRuntimeClass * _g_lpUIWndClass;
	//CreateUIWndInfo * _g_lpUIWndInfo;

	/**
	 *	--��¼��ǰ���ڵ��̵߳�LOG�ļ�·��
	 **/
	CTCHAR * _g_sAliveThreadFile;

	ThreadTreeGlobalInfo()
	{
		/**
		 *	--�˱�����ʱ��Ч
		 **/
		_g_iThreadExitTimeLimit = 0;						//�߳��˳�ʱ��ʱ������

		//static HANDLE _g_hThreadControlMutex;					//�߳̽ڵ������
		//static HANDLE _g_hSingleControlMutex;					//�ź����������в�����
		/**
		 *	--����SECTION��֤��ͬһʱ��ֻ��һ���߳��ڶ�SINGLE���̶߳��н��в���
		 **/
		//CRITICAL_SECTION _g_csThreadControlSection;		//�߳̽ڵ������
		//CRITICAL_SECTION _g_csSingleControlSection;		//�ź����������в�����

		/**
		 *	--���ж�ʱ����ʱ������ź�����WINDOWS API���ź�����
		 **/
		_g_hTimeThreadEvent = NULL;						//��ʱѭ���ź�
		_g_hTimeThreadFinishEvent = NULL;				//����˳���ʱ�߳�ʱ���źŵ��ź���

		/**
		 *	--�洢�Ѿ�������SINGLE�Ķ��У���������������Ϊ�˷��㴴��SINGLEʱ��ѯͬ��SINGLEʱ������
		 **/
		//EventList _g_elNamedEventList;					//�Ѿ����������ź����ڵ��б�
		//MutexList _g_mlNamedMutexList;					//�Ѿ������������ڵ��б�
		//SINGLELIST _g_slSingleList;						//�����߳�Ϊ�յ�SINGLE����

		/**
		 *	--�洢�߳���ض��У���һ��Ϊ�˱��������̶߳������ģ���������Ҫ�����߳����Ϳ��Բ�ѯ
		 *	--����ʱ��ȴ��������ڶ�ʱ�̹߳����߳��У�������ʱ��ȴ��̵߳Ĳ�������
		 *	--��Ҫɾ�����̶߳��У��Ƕ��Ѿ��رյ��߳̽ڵ���л���ɾ���Ķ��У�����ȫ�˳��̺߳�ŶԽڵ����ɾ��
		 **/
		//RUNNINGTHREADLIST _g_rtlThreadList;				//�����˵��߳̽ڵ�洢����
		//WAITINGTHREADLIST _g_wtlTimeLimitWaitingThread;	//����ʱ��ȴ��̶߳���
		_g_dtiDeleteThreadInfo = NULL;		//��Ҫɾ�����߳̽ڵ��ַ

		/**
		 *	--�߳������ڵ�
		 **/
		_g_treThreadTree = NULL;					//�߳����ĸ��ڵ�

		/**
		 *	--���ڴ���UI�߳�ʱ����ָ��洢����ֻ������ʱ�Դ洢��ֹ��ͬ�̷߳���ʱ��ͻ
		 **/
	//	_g_lpUIWndClass = NULL;
	//	_g_lpUIWndInfo = NULL;

		/**
		 *	--��¼��ǰ���ڵ��̵߳�LOG�ļ�·��
		 **/
		_g_sAliveThreadFile = NULL;
	}

	~ThreadTreeGlobalInfo()
	{
		/**
		 *	--�˱�����ʱ��Ч
		 **/
		_g_iThreadExitTimeLimit = 0;						//�߳��˳�ʱ��ʱ������

		//static HANDLE _g_hThreadControlMutex;					//�߳̽ڵ������
		//static HANDLE _g_hSingleControlMutex;					//�ź����������в�����
		/**
		 *	--����SECTION��֤��ͬһʱ��ֻ��һ���߳��ڶ�SINGLE���̶߳��н��в���
		 **/
		//CRITICAL_SECTION _g_csThreadControlSection;		//�߳̽ڵ������
		//CRITICAL_SECTION _g_csSingleControlSection;		//�ź����������в�����

		/**
		 *	--���ж�ʱ����ʱ������ź�����WINDOWS API���ź�����
		 **/
		_g_hTimeThreadEvent = NULL;						//��ʱѭ���ź�
		_g_hTimeThreadFinishEvent = NULL;				//����˳���ʱ�߳�ʱ���źŵ��ź���

		/**
		 *	--�洢�Ѿ�������SINGLE�Ķ��У���������������Ϊ�˷��㴴��SINGLEʱ��ѯͬ��SINGLEʱ������
		 **/
		_g_elNamedEventList.clear();					//�Ѿ����������ź����ڵ��б�
		_g_mlNamedMutexList.clear();					//�Ѿ������������ڵ��б�
		_g_slSingleList.clear();						//�����߳�Ϊ�յ�SINGLE����

		/**
		 *	--�洢�߳���ض��У���һ��Ϊ�˱��������̶߳������ģ���������Ҫ�����߳����Ϳ��Բ�ѯ
		 *	--����ʱ��ȴ��������ڶ�ʱ�̹߳����߳��У�������ʱ��ȴ��̵߳Ĳ�������
		 *	--��Ҫɾ�����̶߳��У��Ƕ��Ѿ��رյ��߳̽ڵ���л���ɾ���Ķ��У�����ȫ�˳��̺߳�ŶԽڵ����ɾ��
		 **/
		_g_rtlThreadList.clear();				//�����˵��߳̽ڵ�洢����
		_g_wtlTimeLimitWaitingThread.clear();	//����ʱ��ȴ��̶߳���
		SAFE_DELETE(_g_dtiDeleteThreadInfo);		//��Ҫɾ�����߳̽ڵ��ַ

		/**
		 *	--�߳������ڵ�
		 **/
		SAFE_DELETE(_g_treThreadTree);					//�߳����ĸ��ڵ�

		/**
		 *	--���ڴ���UI�߳�ʱ����ָ��洢����ֻ������ʱ�Դ洢��ֹ��ͬ�̷߳���ʱ��ͻ
		 **/
//		_g_lpUIWndClass = NULL;
//		_g_lpUIWndInfo = NULL;

		if(NULL != _g_sAliveThreadFile)
		{
			delete [] _g_sAliveThreadFile;
			_g_sAliveThreadFile = NULL;
		}
	}
};

#ifdef _LANG_TRANSLATE                  //                  //
#define LANG_EXPORT __declspec(dllexport)                   //                  //
#else                                   //                  //
#define LANG_EXPORT                     //                  //
#endif  

class LANG_EXPORT ThreadTreeControl
{
	friend class ThreadInfo;
	friend class TCWinThread;
	friend class ThreadControl;


public:
	/**
	 *
	 *	---���������������Ƿ���DEBUGģʽ������̲߳�����Ϣ
	 *
	 **/
	static bool _g_bLogSingleCreate;					//�Ƿ����Single������¼
	static bool _g_bLogSetSingle;						//�Ƿ���������ź�����¼
	static bool _g_bLogWaitObj;							//�Ƿ�����ȴ���¼

protected:
	/**
	 *	--ȫ�ֱ����ṹ��
	 **/
	static ThreadTreeGlobalInfo * _g_giGlobalInfo;
public:
	ThreadTreeControl(void);
	~ThreadTreeControl(void);

public:
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�߳����������ĳ�ʼ������]
	 * @n<b>��������</b>: InitThreadTreeControl
	 * @see               [�μ�����]
	 * @n<b>����</b>    :alzq
	 * @n<b>����ʱ��</b>: 09-2-11 10:26:39
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-11  [����]
	 ******************************************************************************************************/
	static void InitThreadTreeControl(const CTCHAR * logPath);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�߳�������������������]
	 * @n<b>��������</b>: DestroyThreadTreeControl
	 * @see               [�μ�����]
	 * @n<b>����</b>    :alzq
	 * @n<b>����ʱ��</b>: 09-2-11 10:38:23
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-11  [����]
	 ******************************************************************************************************/
	static void DestroyThreadTreeControl(void);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�ͷ��ź�����������]
	 * @n<b>��������</b>        : TCloseSingle
	 * @n@param LPSINGLE single : [��������]
	 * @return                    [����ֵ����]
	 * @see                       [�μ�����]
	 * @n<b>����</b>            :alzq
	 * @n<b>����ʱ��</b>        : 09-2-17 14:29:10
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-17  [����]
	 ******************************************************************************************************/
	static BOOL TCloseSingle(LPSINGLE single);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [ʹ�ø��ڵ��ͷ���]
	 * @n<b>��������</b>        : TReleaseMutex
	 * @n@param SINGLE * single : [��������]
	 * @see                       [�μ�����]
	 * @n<b>����</b>            :alzq
	 * @n<b>����ʱ��</b>        : 09-2-13 16:13:23
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-13  [����]
	 ******************************************************************************************************/
	static UINT TReleaseMutex(SINGLE * single);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [ʹ�ø��ڵ�ȴ��ź�������]
	 * @n<b>��������</b>         : WaitForSingle
	 * @n@param SINGLE * single  : [��������]
	 * @param   DWORD    millSec : [��������]
	 * @return                     [����ֵ����]
	 * @see                        [�μ�����]
	 * @n<b>����</b>             :alzq
	 * @n<b>����ʱ��</b>         : 09-2-13 16:16:50
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-13  [����]
	 ******************************************************************************************************/
	static UINT TWaitForSingle(SINGLE * single, DWORD millSec = INFINITE);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�����߳̽ڵ㣬�����ڵ�����������]
	 * @n<b>��������</b>                              : CreateThread
	 * @n@param ThreadControl *       control         : [���Դ洢�߳̽ڵ���Ϣ���߳̿��ƶ���]
	 * @param   LPSECURITY_ATTRIBUTES lpsa            : [�ο�API]
	 * @param   DWORD                 dwStackSize     : [�ο�API]
	 * @param   USER_FUNCTION         pfnThreadProc   : [�û������壬��API��ͬ��������������߳̽ڵ�Ŀ�����]
	 * @param   LPVOID                lParam          : [�û���Ҫ�����߳��еĲ���]
	 * @param   DWORD                 dwCreationFlags : [�ο�API]
	 * @param   DWORD *               pdwThreadId     : [�ο�API]
	 * @return                                          [����ֵ����]
	 * @see                                             [�μ�����]
	 * @n<b>����</b>                                  :alzq
	 * @n<b>����ʱ��</b>                              : 09-2-5 14:53:07
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-05  [����]
	 ******************************************************************************************************/
	static bool TCreateThread(ThreadControl * control, LPSECURITY_ATTRIBUTES lpsa, DWORD dwStackSize, USER_FUNCTION pfnThreadProc
		, DELETE_THREAD_FUNCTION exitThread, LPVOID lParam, DWORD dwCreationFlags, DWORD * pdwThreadId, char * name = NULL);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�����߳̽ڵ�ļ򵥷�ʽ]
	 * @n<b>��������</b>                      : CreateThread
	 * @n@param ThreadControl * control       : [���Դ洢�߳̽ڵ���Ϣ���߳̿��ƶ���]
	 * @param   USER_FUNCTION   pfnThreadProc : [������ָ��]
	 * @param   LPVOID          lParam        : [�������]
	 * @see                                     [�μ�����]
	 * @n<b>����</b>                          :alzq
	 * @n<b>����ʱ��</b>                      : 09-2-5 14:58:52
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-05  [����]
	 ******************************************************************************************************/
	static bool TCreateThread(ThreadControl * control, USER_FUNCTION pfnThreadProc, DELETE_THREAD_FUNCTION exitThread = NULL, LPVOID lParam = NULL, char * name = NULL);
	
	
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�������ڵ�]
	 * @n<b>��������</b>                : TCreateMutex
	 * @n@param ThreadInfo *  thread    : [�����ýڵ���߳���Ϣ�ڵ��ַ]
	 * @param   BOOL          initState : [��ʼ����״̬��TRUEΪ���źţ�FALSEΪ���ź�]
	 * @param   const WCHAR * name      : [�����ƣ�Ĭ��Ϊ��]
	 * @return                            [����ֵ����]
	 * @see                               [�μ�����]
	 * @n<b>����</b>                    :alzq
	 * @n<b>����ʱ��</b>                : 09-2-6 9:35:35
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-06  [����]
	 ******************************************************************************************************/
	static LPSINGLE TCreateMutex(BOOL initOwner = FALSE, const CTCHAR * name = NULL);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�����źŽڵ�]
	 * @n<b>��������</b>                 : TCreateEvent
	 * @n@param ThreadInfo *  threadInfo : [�����ýڵ���߳���Ϣ�ڵ��ַ]
	 * @param   BOOL          autoReSet  : [�Ƿ��Զ�ת���ź�״̬]
	 * @param   BOOL          initState  : [��ʼ״̬]
	 * @param   const WCHAR * name       : [�ź������ƣ�Ĭ��Ϊ��]
	 * @return                             [����ֵ����]
	 * @see                                [�μ�����]
	 * @n<b>����</b>                     :alzq
	 * @n<b>����ʱ��</b>                 : 09-2-6 9:44:15
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-06  [����]
	 ******************************************************************************************************/
	static LPSINGLE TCreateEvent(BOOL autoReSet = FALSE, BOOL initState = FALSE, const CTCHAR * name = NULL);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�����ź���Ϊ���ź�״̬]
	 * @n<b>��������</b>                : TSetEventSingle
	 * @param   SINGLE *     single     : [�ź����Ľڵ��ַ]
	 * @return                            [����ֵ����]
	 * @see                               [�μ�����]
	 * @n<b>����</b>                    :alzq
	 * @n<b>����ʱ��</b>                : 09-2-6 15:14:36
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-06  [����]
	 ******************************************************************************************************/
	static UINT TSetEventSingle(SINGLE * single);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�����ź���Ϊ���ź�״̬]
	 * @n<b>��������</b>                : TSetEventNoSingle
	 * @param   SINGLE *     single     : [�ź����Ľڵ��ַ]
	 * @return                            [����ֵ����]
	 * @see                               [�μ�����]
	 * @n<b>����</b>                    :alzq
	 * @n<b>����ʱ��</b>                : 09-2-6 15:15:05
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-06  [����]
	 ******************************************************************************************************/
	static UINT TSetEventNoSingle(SINGLE * single);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [ɾ�����ڵ㣬�Լ����ӽڵ�]
	 * @n<b>��������</b>                     : DeleteThread
	 * @n@param TreeItem_t * treeItem        : [��������]
	 * @param BOOL delChild                  : [�Ƿ�ɾ���ӽڵ�]
	 * @see                                    [�μ�����]
	 * @n<b>����</b>                         :alzq
	 * @n<b>����ʱ��</b>                     : 09-2-18 9:17:13
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-18  [����]
	 ******************************************************************************************************/
	static void DeleteThread(TreeItem_t * treeItem, BOOL delChild, DWORD delayTime = INFINITE);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [ɾ��ָ���߳��Լ������߳�]
	 * @n<b>��������</b>          : DeleteThread
	 * @n@param ThreadInfo * info : [��������]
	 * @param BOOL delChild       : [�Ƿ�ɾ���ӽڵ�]
	 * @see                         [�μ�����]
	 * @n<b>����</b>              :alzq
	 * @n<b>����ʱ��</b>          : 09-2-13 14:26:40
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-13  [����]
	 ******************************************************************************************************/
	static void DeleteThread(ThreadInfo * info, BOOL delChild, DWORD delayTime = INFINITE);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [ɾ�����ô˺������̣߳����߳���Ч]
	 * @n<b>��������</b>      : DeleteThread
	 * @n@param BOOL delChild : [��������]
	 * @see                     [�μ�����]
	 * @n<b>����</b>          :alzq
	 * @n<b>����ʱ��</b>      : 09-2-23 15:28:33
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-23  [����]
	 ******************************************************************************************************/
	static void DeleteThread(BOOL delChild, DWORD delayTime = INFINITE);

protected:
	/**
	 *
	 *	---�Զ�����̺߳���
	 *
	 **/
	static unsigned int WINAPI _CustomThread(LPVOID lpParameter);
	/**
	 *
	 *	---��ʱ�����Ƶ��̹߳����߳�
	 *
	 **/
	static DWORD WINAPI _TimeLimitThreadControlThread(LPVOID lpParameter);
	/**
	 *
	 *	---����ɾ���̵߳Ĺ����߳�,����ÿ���߳������Ŀ���Ȩ������ȡ��ʱ��ɾ��
	 *
	 **/
	static DWORD WINAPI _ThreadDeleteThread(LPVOID lpParameter);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�ͷ��߳������Դ�������߳���ӵ�ɾ��������]
	 * @n<b>��������</b>          : _DisposeThread
	 * @n@param ThreadInfo * info : [��������]
	 * @see                         [�μ�����]
	 * @n<b>����</b>              :alzq
	 * @n<b>����ʱ��</b>          : 09-2-24 14:31:29
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-24  [����]
	 ******************************************************************************************************/
	static void _DisposeThread(ThreadInfo * info);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [���̴߳洢�����в�ѯ]
	 * @n<b>��������</b>            : _FindRunningThread
	 * @n@param HANDLE threadHandle : [��������]
	 * @return                        [����ֵ����]
	 * @see                           [�μ�����]
	 * @n<b>����</b>                :alzq
	 * @n<b>����ʱ��</b>            : 09-2-18 14:24:37
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-18  [����]
	 ******************************************************************************************************/
	static ThreadInfo * _FindRunningThread(DWORD threadId);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [���̴߳洢�����в�ѯ]
	 * @n<b>��������</b>            : _FindRunningThread
	 * @n@param CWinThread * winThread : [��������]
	 * @return                        [����ֵ����]
	 * @see                           [�μ�����]
	 * @n<b>����</b>                :alzq
	 * @n<b>����ʱ��</b>            : 09-2-18 14:24:37
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-18  [����]
	 ******************************************************************************************************/
	//static ThreadInfo * _FindRunningThread(CWinThread * winThread);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [���̴߳洢������ɾ��ָ���ڵ�]
	 * @n<b>��������</b>            : _DelRunningThread
	 * @n@param HANDLE threadHandle : [��������]
	 * @return                        [����ֵ����]
	 * @see                           [�μ�����]
	 * @n<b>����</b>                :alzq
	 * @n<b>����ʱ��</b>            : 09-2-18 14:24:37
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-18  [����]
	 ******************************************************************************************************/
	static void _DelRunningThread(DWORD threadId);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [���ݱ��̴߳洢����ز��������߳�]
	 * @n<b>��������</b>: TSuspendThread
	 * @n@param ThreadInfo * info : [��������]
	 * @see               [�μ�����]
	 * @n<b>����</b>    :alzq
	 * @n<b>����ʱ��</b>: 09-2-16 9:12:42
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-16  [����]
	 ******************************************************************************************************/
	static void _TSuspendThread(ThreadInfo * info);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [���ݱ��̴߳洢����ز����ָ����߳�]
	 * @n<b>��������</b>: TResumeThread
	 * @n@param ThreadInfo * info : [��������]
	 * @see               [�μ�����]
	 * @n<b>����</b>    :alzq
	 * @n<b>����ʱ��</b>: 09-2-16 9:12:42
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-16  [����]
	 ******************************************************************************************************/
	static void _TResumeThread(ThreadInfo * info);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [���ڵ��ɾ������]
	 * @n<b>��������</b>                     : _DisposeTreeItem
	 * @n@param TreeItem_t * treeItem        : [�ڵ��ַ]
	 * @param   bool         terminateEnable : [�Ƿ�����ǿ����ֹ�߳�]
	 * @see                                    [�μ�����]
	 * @n<b>����</b>                         :alzq
	 * @n<b>����ʱ��</b>                     : 09-2-18 9:15:53
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-18  [����]
	 ******************************************************************************************************/
	static void _DisposeTreeItem(TreeItem_t * treeItem, DWORD delayTime = INFINITE);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [ɾ��ĳ���ڵ�������ӽڵ�]
	 * @n<b>��������</b>                     : _DestroyThreadTreeChild
	 * @n@param TreeItem_t * treeItem        : [��������]
	 * @param   bool         terminateEnable : [��������]
	 * @see                                    [�μ�����]
	 * @n<b>����</b>                         :alzq
	 * @n<b>����ʱ��</b>                     : 09-2-18 9:06:35
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-18  [����]
	 ******************************************************************************************************/
	static void _DestroyThreadTreeChild(TreeItem_t * treeItem);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [ɾ��ĳһ���ڵ㼰��֮������нڵ����ӽڵ�]
	 * @n<b>��������</b>              : _DestroyThreadTreeItems
	 * @n@param TreeItem_t * treeItem : [��������]
	 * @see                             [�μ�����]
	 * @n<b>����</b>                  :alzq
	 * @n<b>����ʱ��</b>              : 09-2-13 13:55:00
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-13  [����]
	 ******************************************************************************************************/
	static void _DestroyThreadTreeItems(TreeItem_t * treeItem);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [����һ���߳̿�ʼ����ȡ��������]
	 * @n<b>��������</b>          : SetThreadStart
	 * @n@param ThreadInfo * info : [��������]
	 * @see                         [�μ�����]
	 * @n<b>����</b>              :alzq
	 * @n<b>����ʱ��</b>          : 09-2-13 14:24:55
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-13  [����]
	 ******************************************************************************************************/
	static void _SetThreadStart(ThreadInfo * info);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [����һ���߳�Ϊ���������ý�������]
	 * @n<b>��������</b>          : SetThreadFinish
	 * @n@param ThreadInfo * info : [��������]
	 * @see                         [�μ�����]
	 * @n<b>����</b>              :alzq
	 * @n<b>����ʱ��</b>          : 09-2-13 14:24:55
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-13  [����]
	 ******************************************************************************************************/
	static void _SetThreadFinish(ThreadInfo * info);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [������Ҫɾ�����̶߳��У�������Ҫɾ�����߳̽ڵ�ɾ��]
	 * @n<b>��������</b>: TestAndDeleteThread
	 * @see               [�μ�����]
	 * @n<b>����</b>    :alzq
	 * @n<b>����ʱ��</b>: 09-2-17 9:34:35
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-17  [����]
	 ******************************************************************************************************/
	static void _TestAndDeleteThread(void);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [������������ʱ��ȴ��̶߳���]
	 * @n<b>��������</b>: DealThreadTimeCount
	 * @see               [�μ�����]
	 * @n<b>����</b>    :alzq
	 * @n<b>����ʱ��</b>: 09-2-12 18:46:26
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-12  [����]
	 ******************************************************************************************************/
	static void _DealThreadTimeCount(void);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�������ڵ�]
	 * @n<b>��������</b>                : TCreateMutex
	 * @n@param ThreadInfo *  thread    : [�����ýڵ���߳���Ϣ�ڵ��ַ]
	 * @param   BOOL          initState : [��ʼ����״̬��TRUEΪ���źţ�FALSEΪ���ź�]
	 * @param   const WCHAR * name      : [�����ƣ�Ĭ��Ϊ��]
	 * @return                            [����ֵ����]
	 * @see                               [�μ�����]
	 * @n<b>����</b>                    :alzq
	 * @n<b>����ʱ��</b>                : 09-2-6 9:35:35
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-06  [����]
	 ******************************************************************************************************/
	static SINGLE * _TCreateMutex(ThreadInfo * threadInfo, BOOL initOwner = FALSE, const CTCHAR * name = NULL);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�����źŽڵ�]
	 * @n<b>��������</b>                 : TCreateEvent
	 * @n@param ThreadInfo *  threadInfo : [�����ýڵ���߳���Ϣ�ڵ��ַ]
	 * @param   BOOL          autoReSet  : [�Ƿ��Զ�ת���ź�״̬]
	 * @param   BOOL          initState  : [��ʼ״̬]
	 * @param   const WCHAR * name       : [�ź������ƣ�Ĭ��Ϊ��]
	 * @return                             [����ֵ����]
	 * @see                                [�μ�����]
	 * @n<b>����</b>                     :alzq
	 * @n<b>����ʱ��</b>                 : 09-2-6 9:44:15
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-06  [����]
	 ******************************************************************************************************/
	static SINGLE * _TCreateEvent(BOOL autoReSet = FALSE, BOOL initState = FALSE, const CTCHAR * name = NULL);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [���ȴ��̶߳��в���������]
	 * @n<b>��������</b>: _LockThreadControlMutex
	 * @return            [����ֵ����]
	 * @see               [�μ�����]
	 * @n<b>����</b>    :alzq
	 * @n<b>����ʱ��</b>: 09-2-6 16:03:47
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-06  [����]
	 ******************************************************************************************************/
	static void _LockThreadControlMutex(void);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [���ȴ��̶߳��в���������]
	 * @n<b>��������</b>: _UnLockThreadControlMutex
	 * @return            [����ֵ����]
	 * @see               [�μ�����]
	 * @n<b>����</b>    :alzq
	 * @n<b>����ʱ��</b>: 09-2-6 16:04:04
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-06  [����]
	 ******************************************************************************************************/
	static void _UnLockThreadControlMutex(void);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [��ȡ�ź����������в�����]
	 * @n<b>��������</b>: _LockSingleControlMutex
	 * @return            [����ֵ����]
	 * @see               [�μ�����]
	 * @n<b>����</b>    :alzq
	 * @n<b>����ʱ��</b>: 09-2-17 14:47:59
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-17  [����]
	 ******************************************************************************************************/
	static void _LockSingleControlMutex(void);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�ͷ��ź����������в�����]
	 * @n<b>��������</b>: _UnLockSingleControlMutex
	 * @return            [����ֵ����]
	 * @see               [�μ�����]
	 * @n<b>����</b>    :alzq
	 * @n<b>����ʱ��</b>: 09-2-17 14:48:19
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-17  [����]
	 ******************************************************************************************************/
	static void _UnLockSingleControlMutex(void);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�����̵߳ȴ���ʱ�䣬����ض���������߳̽ڵ㣬�����ݵȴ�ʱ������Ƿ���ӵ�����ʱ��ȴ��̶߳�����]
	 * @n<b>��������</b>              : _AddWaitingThread
	 * @n@param ThreadInfo * lpThread : [��������]
	 * @param   DWORD        millSec  : [��������]
	 * @see                             [�μ�����]
	 * @n<b>����</b>                  :alzq
	 * @n<b>����ʱ��</b>              : 09-2-6 16:42:52
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-06  [����]
	 ******************************************************************************************************/
	static void _AddWaitingThread(ThreadInfo * lpThread, DWORD millSec);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [����ʱ�����Ƶĵȴ��̶߳�����ɾ���ƶ����߳̽ڵ�]
	 * @n<b>��������</b>              : _DelTimeLimitWaitingThread
	 * @n@param ThreadInfo * lpThread : [��������]
	 * @see                             [�μ�����]
	 * @n<b>����</b>                  :alzq
	 * @n<b>����ʱ��</b>              : 09-2-6 16:54:38
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-06  [����]
	 ******************************************************************************************************/
	static void _DelTimeLimitWaitingThread(ThreadInfo * lpThread);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�������ֲ�ѯͬ����]
	 * @n<b>��������</b>            : _FindMutexByName
	 * @n@param const CTCHAR * name : [��������]
	 * @return                        [����ֵ����]
	 * @see                           [�μ�����]
	 * @n<b>����</b>                :alzq
	 * @n<b>����ʱ��</b>            : 09-2-12 13:49:50
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-12  [����]
	 ******************************************************************************************************/
	static SINGLE * _FindMutexByName(const CTCHAR * name);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�������ֲ�ѯͬ���ź���]
	 * @n<b>��������</b>            : _FindEventByName
	 * @n@param const CTCHAR * name : [��������]
	 * @return                        [����ֵ����]
	 * @see                           [�μ�����]
	 * @n<b>����</b>                :alzq
	 * @n<b>����ʱ��</b>            : 09-2-12 14:48:41
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-12  [����]
	 ******************************************************************************************************/
	static SINGLE * _FindEventByName(const CTCHAR * name);

	static void _OutputThread(TreeItem_t * treeItem, fstream * stream, LPSYSTEMTIME systemTime);
	//�����Ȼ�ڻ���ڴ����߳���Ϣ
	static void _OutPutAliveThread(void);
};

#endif
