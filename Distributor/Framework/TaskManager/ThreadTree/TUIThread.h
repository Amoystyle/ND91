/***************************************************************************//**
 *  @file 
 *  @brief [本类为对CWinThread进行封装后的UI线程类，构造函数带入CWND的子类指针
 *  在子类的头文件中必须有“DECLARE_DYNCREATE(class name)”的声明，而在代码文件中
 *  必须加上“IMPLEMENT_DYNCREATE(class name, parent class name)”]
 * 
 *  <b>文件名</b>      : TUIThread.h
 *  @n@n<b>版权所有</b>: 网龙天晴程序部应用软件开发组
 *  @n@n<b>作  者</b>  : alzq
 *  @n@n<b>创建时间</b>: 09-3-2 10:45:46
 *  @n@n<b>文件描述</b>: 
 *  @version  版本        修改者        时间        描述@n
 *  @n        [版本号]    alzq          2009-03-03  [描述]
 * 
*******************************************************************************/
#ifndef __TUITHREAD__
#define __TUITHREAD__

#pragma once
#include "afxwin.h"
#include "ThreadInfo.h"

//创建UI窗口的相关参数
struct CreateUIWndInfo
{
	LPCTSTR lpszClassName;
	LPCTSTR lpszWindowName;
	DWORD dwStyle;
	RECT rect;
	CWnd * lpParentWnd;
	UINT nID;
	CCreateContext * pContext;
	bool bCreateDialogByID;		//是否按照ID生成对话框

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

	CWnd * _m_wnd;						//创建的UI类WND指针
	SINGLE * _m_sLoadFinishEvent;		//创建对象完成后的信号

public:
	TUIThread(void);
	~TUIThread(void);
	
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

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
	virtual CWnd * CustomCreateWin(void);
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
	virtual void PreCreatedUIThread(void);
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
	virtual void AfterCreatedUIThread(void);
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
	virtual void DelWnd(void);

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
	CWnd * GetWindow(void);
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
	bool WaitForWndCreate(void);

protected:
	DECLARE_MESSAGE_MAP()
};

#endif
