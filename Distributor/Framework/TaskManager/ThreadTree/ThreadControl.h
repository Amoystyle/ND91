/***************************************************************************//**
 *  @file 
 *  @brief [�߳̿����࣬�û�ͨ��������ⲿ���߳̽��в�ѯ�����]
 * 
 *  <b>�ļ���</b>      : ThreadControl.h
 *  @n@n<b>��Ȩ����</b>: �����������Ӧ�����������
 *  @n@n<b>��  ��</b>  : alzq
 *  @n@n<b>����ʱ��</b>: 09-2-5 15:45:38
 *  @n@n<b>�ļ�����</b>: 
 *  @version  �汾        �޸���        ʱ��        ����@n
 *  @n        [�汾��]    alzq          2009-02-06  [����]
 * 
*******************************************************************************/
#ifndef __THREAD_CONTROL__
#define __THREAD_CONTROL__


#include "Tree.h"
#include "ThreadInfo.h"

#pragma once


#ifdef _LANG_TRANSLATE                  //                  //
#define LANG_EXPORT __declspec(dllexport)                   //                  //
#else                                   //                  //
#define LANG_EXPORT                     //                  //
#endif  

class LANG_EXPORT ThreadControl
{
	friend class ThreadTreeControl;
	friend class ThreadInfo;
	friend class TCWinThread;

/**
 *
 *	-------------������������������
 *
 **/
public:
	ThreadControl(void);
	~ThreadControl(void);

/**
 *
 *	-------------��Ա����������
 *
 **/
protected:
	TreeItem_t * _m_tiThreadInfo;			//�洢�߳̽ڵ��ַ�����Զ��߳̽��в���

	bool _m_bThreadLogEnable;				//�߳��Ƿ����LOG��¼

/**
 *
 *	-------------��������������
 *
 **/
public:
	int ThreadNum;
	/************************************************************************************
	 * @brief
	 * 
	 * �����Ƿ�Ա��߳����߳���LOG�н��м�¼
	 * @n<b>��������</b>: SetLogEnable
	 * @see              
	 * @n<b>����</b>    :alzq
	 * @n<b>����ʱ��</b>: 09-3-16 15:38:34
	 * @version	�޸���        ʱ��        ����@n
	 * @n		alzq          2009-03-16 
	 ************************************************************************************/
	void SetLogEnable(bool enable);
	/************************************************************************************
	 * @brief
	 * 
	 * ��ȡ�߳��Ƿ���ڵ�״̬
	 * @n<b>��������</b>: ThreadAlive
	 * @return           
	 * @see              
	 * @n<b>����</b>    :alzq
	 * @n<b>����ʱ��</b>: 09-3-16 15:42:34
	 * @version	�޸���        ʱ��        ����@n
	 * @n		alzq          2009-03-16 
	 ************************************************************************************/
	BOOL ThreadAlive(void);

	/**
	 ��ȡ�߳������Ϣ
	 **/
	HANDLE GetThreadHandle(void);
	DWORD GetThreadId(void);
	bool GetThreadIsUIThread(void);
	LPVOID GetLPWinThread(void);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�ر��̣߳������ں���ִ�н������̲߳���һ��������ֹ]
	 * @n<b>��������</b>      : DeleteThread
	 * @n@param BOOL delChild : [�Ƿ�ɾ���ӽڵ��߳�]
	 * @see                     [�μ�����]
	 * @n<b>����</b>          :alzq
	 * @n<b>����ʱ��</b>      : 09-2-19 10:00:19
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-19  [����]
	 ******************************************************************************************************/
	void DeleteThread(BOOL delChild, DWORD delayTime = 0);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�����߳̽ڵ㣬�����ڵ�����������]
	 * @n<b>��������</b>                              : TCreateChildThread
	 * @n@param ThreadControl *       control         : [���Դ洢�߳̽ڵ���Ϣ���߳̿��ƶ���]
	 * @param   ThreadInfo *          parentThread    : [�����̵߳Ľڵ��ַ]
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
	bool TCreateChildThread(ThreadControl * control, LPSECURITY_ATTRIBUTES lpsa, DWORD dwStackSize
		, USER_FUNCTION pfnThreadProc, DELETE_THREAD_FUNCTION exitThread, LPVOID lParam, DWORD dwCreationFlags, DWORD * pdwThreadId);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [�����߳̽ڵ�ļ򵥷�ʽ]
	 * @n<b>��������</b>                      : TCreateChildThread
	 * @n@param ThreadControl * control       : [���Դ洢�߳̽ڵ���Ϣ���߳̿��ƶ���]
	 * @param   TreeItem_t *    parentThread  : [�����̵߳Ľڵ��ַ]
	 * @param   USER_FUNCTION   pfnThreadProc : [������ָ��]
	 * @param   LPVOID          lParam        : [�������]
	 * @see                                     [�μ�����]
	 * @n<b>����</b>                          :alzq
	 * @n<b>����ʱ��</b>                      : 09-2-5 14:58:52
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-05  [����]
	 ******************************************************************************************************/
	bool TCreateChildThread(ThreadControl * control, USER_FUNCTION pfnThreadProc, DELETE_THREAD_FUNCTION exitThread, LPVOID lParam = NULL);

/**
 *
 *	-------------�����ຯ��������
 *
 **/
protected:
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [���ô洢���߳̽ڵ��ַ]
	 * @n<b>��������</b>          : _SetThreadInfo
	 * @n@param ThreadInfo * info : [��������]
	 * @see                         [�μ�����]
	 * @n<b>����</b>              :alzq
	 * @n<b>����ʱ��</b>          : 09-2-5 15:03:02
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-05  [����]
	 ******************************************************************************************************/
	void _SetThreadInfo(ThreadInfo * info);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [���õ�ǰ�߳�Ϊ��Ч�̣߳����߳̽ڵ㱻ɾ��ʱ��������]
	 * @n<b>��������</b>: _SetThreadDisable
	 * @see               [�μ�����]
	 * @n<b>����</b>    :alzq
	 * @n<b>����ʱ��</b>: 09-2-5 15:09:07
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-05  [����]
	 ******************************************************************************************************/
	void _SetThreadDisable(void);
	/*******************************************************************************************************
	 * @brief [������Ҫ����]
	 * 
	 * [���õ�ǰ�߳�Ϊ��Ч�̣߳��ڿ����߳�ʱ��������]
	 * @n<b>��������</b>: _SetThreadEnable
	 * @see               [�μ�����]
	 * @n<b>����</b>    :alzq
	 * @n<b>����ʱ��</b>: 09-2-5 15:10:24
	 * @version    �޸���        ʱ��        ����@n
	 * @n          alzq          2009-02-05  [����]
	 ******************************************************************************************************/
	void _SetThreadEnable(void);

	ThreadInfo * _GetThreadInfo(void);
};

#endif
