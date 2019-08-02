/***************************************************************************//**
 *  @file 
 *  @brief [Ϊ�̹߳���������LOG��¼����]
 * 
 *  <b>�ļ���</b>      : ThreadLog.h
 *  @n@n<b>��Ȩ����</b>: �����������Ӧ�����������
 *  @n@n<b>��  ��</b>  : alzq
 *  @n@n<b>����ʱ��</b>: 09-2-24 14:29:16
 *  @n@n<b>�ļ�����</b>: 
 *  @version  �汾        �޸���        ʱ��        ����@n
 *  @n        [�汾��]    alzq          2009-02-24  [����]
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
	static CTCHAR * _g_csFilePath;							//�ļ�·��

	static CRITICAL_SECTION _g_csLogWritSection;			//LOG��¼д��ʱ����

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
