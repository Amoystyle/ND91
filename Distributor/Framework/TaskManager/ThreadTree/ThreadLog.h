/***************************************************************************//**
 *  @file 
 *  @brief [为线程管理器进行LOG记录的类]
 * 
 *  <b>文件名</b>      : ThreadLog.h
 *  @n@n<b>版权所有</b>: 网龙天晴程序部应用软件开发组
 *  @n@n<b>作  者</b>  : alzq
 *  @n@n<b>创建时间</b>: 09-2-24 14:29:16
 *  @n@n<b>文件描述</b>: 
 *  @version  版本        修改者        时间        描述@n
 *  @n        [版本号]    alzq          2009-02-24  [描述]
 * 
*******************************************************************************/
#ifndef __THREAD_LOG__
#define __THREAD_LOG__

#include <fstream>
#include "ThreadInfo.h"

using namespace std;

#pragma once

#ifdef _LANG_TRANSLATE                  //                  //
#define LANG_EXPORT __declspec(dllexport)                   //                  //
#else                                   //                  //
#define LANG_EXPORT                     //                  //
#endif  

class LANG_EXPORT ThreadLog
{
public:
	ThreadLog(void);
	~ThreadLog(void);

protected:
	static CTCHAR * _g_csFilePath;							//文件路径

	static CRITICAL_SECTION _g_csLogWritSection;			//LOG记录写入时的锁

	static fstream _g_fsLogStream;

public:
	static void InitThreadLog(const CTCHAR * logFilePath);
	static void CloseThreadLog(void);
	static void AddLog(ThreadInfo * thread, const char * mes);
	static void AddLog(const char * mes);

protected:
	static void _LockLog(void);
	static void _UnLockLog(void);
};

#endif
