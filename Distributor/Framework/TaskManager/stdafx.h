// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#define INTERFUN ;

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#define USERDATA_FOLDER        L"91Distributor"

#include <atlbase.h>
#include <atlstr.h>
#include   <assert.h>
#include "stdint.h"

// 以下为所有AssistantDll用到的头文件
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
// TODO: 在此处引用程序需要的其他头文件
#ifdef _TASK_DLL
#define _DLL_EXPOR __declspec(dllexport)
#else
#define _DLL_EXPOR __declspec(dllimport)
#endif