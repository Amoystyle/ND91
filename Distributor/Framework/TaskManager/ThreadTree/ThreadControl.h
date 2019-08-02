/***************************************************************************//**
 *  @file 
 *  @brief [线程控制类，用户通过此类从外部对线程进行查询与控制]
 * 
 *  <b>文件名</b>      : ThreadControl.h
 *  @n@n<b>版权所有</b>: 网龙天晴程序部应用软件开发组
 *  @n@n<b>作  者</b>  : alzq
 *  @n@n<b>创建时间</b>: 09-2-5 15:45:38
 *  @n@n<b>文件描述</b>: 
 *  @version  版本        修改者        时间        描述@n
 *  @n        [版本号]    alzq          2009-02-06  [描述]
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
 *	-------------构造析构函数定义区
 *
 **/
public:
	ThreadControl(void);
	~ThreadControl(void);

/**
 *
 *	-------------成员变量声明区
 *
 **/
protected:
	TreeItem_t * _m_tiThreadInfo;			//存储线程节点地址，用以对线程进行操作

	bool _m_bThreadLogEnable;				//线程是否进行LOG记录

/**
 *
 *	-------------公共函数声明区
 *
 **/
public:
	int ThreadNum;
	/************************************************************************************
	 * @brief
	 * 
	 * 设置是否对本线程在线程树LOG中进行记录
	 * @n<b>函数名称</b>: SetLogEnable
	 * @see              
	 * @n<b>作者</b>    :alzq
	 * @n<b>创建时间</b>: 09-3-16 15:38:34
	 * @version	修改者        时间        描述@n
	 * @n		alzq          2009-03-16 
	 ************************************************************************************/
	void SetLogEnable(bool enable);
	/************************************************************************************
	 * @brief
	 * 
	 * 获取线程是否存在的状态
	 * @n<b>函数名称</b>: ThreadAlive
	 * @return           
	 * @see              
	 * @n<b>作者</b>    :alzq
	 * @n<b>创建时间</b>: 09-3-16 15:42:34
	 * @version	修改者        时间        描述@n
	 * @n		alzq          2009-03-16 
	 ************************************************************************************/
	BOOL ThreadAlive(void);

	/**
	 获取线程相关信息
	 **/
	HANDLE GetThreadHandle(void);
	DWORD GetThreadId(void);
	bool GetThreadIsUIThread(void);
	LPVOID GetLPWinThread(void);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [关闭线程，但是在函数执行结束后本线程并不一定马上终止]
	 * @n<b>函数名称</b>      : DeleteThread
	 * @n@param BOOL delChild : [是否删除子节点线程]
	 * @see                     [参见函数]
	 * @n<b>作者</b>          :alzq
	 * @n<b>创建时间</b>      : 09-2-19 10:00:19
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-19  [描述]
	 ******************************************************************************************************/
	void DeleteThread(BOOL delChild, DWORD delayTime = 0);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [创建线程节点，并将节点与控制类相绑定]
	 * @n<b>函数名称</b>                              : TCreateChildThread
	 * @n@param ThreadControl *       control         : [用以存储线程节点信息的线程控制对象]
	 * @param   ThreadInfo *          parentThread    : [父亲线程的节点地址]
	 * @param   LPSECURITY_ATTRIBUTES lpsa            : [参考API]
	 * @param   DWORD                 dwStackSize     : [参考API]
	 * @param   USER_FUNCTION         pfnThreadProc   : [用户函数体，与API不同，带入参数将是线程节点的控制类]
	 * @param   LPVOID                lParam          : [用户需要带入线程中的参数]
	 * @param   DWORD                 dwCreationFlags : [参考API]
	 * @param   DWORD *               pdwThreadId     : [参考API]
	 * @return                                          [返回值描述]
	 * @see                                             [参见函数]
	 * @n<b>作者</b>                                  :alzq
	 * @n<b>创建时间</b>                              : 09-2-5 14:53:07
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-05  [描述]
	 ******************************************************************************************************/
	bool TCreateChildThread(ThreadControl * control, LPSECURITY_ATTRIBUTES lpsa, DWORD dwStackSize
		, USER_FUNCTION pfnThreadProc, DELETE_THREAD_FUNCTION exitThread, LPVOID lParam, DWORD dwCreationFlags, DWORD * pdwThreadId);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [创建线程节点的简单方式]
	 * @n<b>函数名称</b>                      : TCreateChildThread
	 * @n@param ThreadControl * control       : [用以存储线程节点信息的线程控制对象]
	 * @param   TreeItem_t *    parentThread  : [父亲线程的节点地址]
	 * @param   USER_FUNCTION   pfnThreadProc : [函数体指针]
	 * @param   LPVOID          lParam        : [带入参数]
	 * @see                                     [参见函数]
	 * @n<b>作者</b>                          :alzq
	 * @n<b>创建时间</b>                      : 09-2-5 14:58:52
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-05  [描述]
	 ******************************************************************************************************/
	bool TCreateChildThread(ThreadControl * control, USER_FUNCTION pfnThreadProc, DELETE_THREAD_FUNCTION exitThread, LPVOID lParam = NULL);

/**
 *
 *	-------------保护类函数声明区
 *
 **/
protected:
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [设置存储的线程节点地址]
	 * @n<b>函数名称</b>          : _SetThreadInfo
	 * @n@param ThreadInfo * info : [参数描述]
	 * @see                         [参见函数]
	 * @n<b>作者</b>              :alzq
	 * @n<b>创建时间</b>          : 09-2-5 15:03:02
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-05  [描述]
	 ******************************************************************************************************/
	void _SetThreadInfo(ThreadInfo * info);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [设置当前线程为无效线程，当线程节点被删除时进行设置]
	 * @n<b>函数名称</b>: _SetThreadDisable
	 * @see               [参见函数]
	 * @n<b>作者</b>    :alzq
	 * @n<b>创建时间</b>: 09-2-5 15:09:07
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-05  [描述]
	 ******************************************************************************************************/
	void _SetThreadDisable(void);
	/*******************************************************************************************************
	 * @brief [函数简要描述]
	 * 
	 * [设置当前线程为有效线程，在开启线程时进行设置]
	 * @n<b>函数名称</b>: _SetThreadEnable
	 * @see               [参见函数]
	 * @n<b>作者</b>    :alzq
	 * @n<b>创建时间</b>: 09-2-5 15:10:24
	 * @version    修改者        时间        描述@n
	 * @n          alzq          2009-02-05  [描述]
	 ******************************************************************************************************/
	void _SetThreadEnable(void);

	ThreadInfo * _GetThreadInfo(void);
};

#endif
