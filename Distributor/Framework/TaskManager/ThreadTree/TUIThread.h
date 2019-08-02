/***************************************************************************//**
 *  @file 
 *  @brief [����Ϊ��CWinThread���з�װ���UI�߳��࣬���캯������CWND������ָ��
 *  �������ͷ�ļ��б����С�DECLARE_DYNCREATE(class name)�������������ڴ����ļ���
 *  ������ϡ�IMPLEMENT_DYNCREATE(class name, parent class name)��]
 * 
 *  <b>�ļ���</b>      : TUIThread.h
 *  @n@n<b>��Ȩ����</b>: �����������Ӧ�����������
 *  @n@n<b>��  ��</b>  : alzq
 *  @n@n<b>����ʱ��</b>: 09-3-2 10:45:46
 *  @n@n<b>�ļ�����</b>: 
 *  @version  �汾        �޸���        ʱ��        ����@n
 *  @n        [�汾��]    alzq          2009-03-03  [����]
 * 
*******************************************************************************/
#ifndef __TUITHREAD__
#define __TUITHREAD__

#pragma once
#include "afxwin.h"
#include "ThreadInfo.h"

//����UI���ڵ���ز���
struct CreateUIWndInfo
{
	LPCTSTR lpszClassName;
	LPCTSTR lpszWindowName;
	DWORD dwStyle;
	RECT rect;
	CWnd * lpParentWnd;
	UINT nID;
	CCreateContext * pContext;
	bool bCreateDialogByID;		//�Ƿ���ID���ɶԻ���

	CreateUIWndInfo()
	{
		lpszClassName = NULL;
		lpszWindowName = NULL;
		dwStyle = NULL;
		//RECT rect;
		lpParentWnd = NULL;
		nID = NULL;
		pContext = NULL;
		bCreateDialogByID = false;
	}
};


#ifdef _LANG_TRANSLATE                  //                  //
#define LANG_EXPORT __declspec(dllexport)                   //                  //
#else                                   //                  //
#define LANG_EXPORT                     //                  //
#endif  

class LANG_EXPORT TUIThread :
	public CWinThread
{
DECLARE_DYNCREATE(TUIThread)

friend class ThreadTreeControl;

protected:
	ThreadInfo * _m_tiThreadInfo;
	
	CRuntimeClass * _m_lpUIWndClass;
	CreateUIWndInfo * _m_lpUIWndInfo;

	CWnd * _m_wnd;						//������UI��WNDָ��
	SINGLE * _m_sLoadFinishEvent;		//����������ɺ���ź�

public:
	TUIThread(void);
	~TUIThread(void);
	
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

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
	virtual CWnd * CustomCreateWin(void);
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
	virtual void PreCreatedUIThread(void);
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
	virtual void AfterCreatedUIThread(void);
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
	virtual void DelWnd(void);

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
	CWnd * GetWindow(void);
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
	bool WaitForWndCreate(void);

protected:
	DECLARE_MESSAGE_MAP()
};

#endif
