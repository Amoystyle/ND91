// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#define INTERFUN ;

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#define USERDATA_FOLDER        L"91Distributor"

#include <atlbase.h>
#include <atlstr.h>
#include   <assert.h>
#include "stdint.h"

// ����Ϊ����AssistantDll�õ���ͷ�ļ�
#include <vector>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>

#include <list>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <deque>
#include <time.h>
#include <winver.h>
using namespace std;

//Singleton
#include "Singleton.h"
#include "Common/WindowMessageDefine.h"
#include "Message/Message.h"
#include "FileOperationEx.h"

#define TRACE ATLTRACE
#define  ASSERT assert
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#ifdef _TASK_DLL
#define _DLL_EXPOR __declspec(dllexport)
#else
#define _DLL_EXPOR __declspec(dllimport)
#endif