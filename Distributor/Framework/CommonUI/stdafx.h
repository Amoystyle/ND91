// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#define _CRT_SECURE_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <atlstr.h>
#include <ShellAPI.h>
// C 运行时头文件
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <list>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <deque>
#include <time.h>
#include <winver.h>
#include "stdint.h"
#include <shlobj.h>
using namespace std;

#pragma warning(disable:4251)
#pragma warning(disable:4800)
#include "DirectUI.h"
#pragma warning(default:4251)
#pragma warning(default:4800)

// 使用GDI
#include <GdiPlus.h>
#pragma comment (lib, "gdiplus.lib ")
using namespace Gdiplus;

//Cul
#include "Common/GlobalOption.h"
#include "Common/log.h"
#include "Common/FileOperation.h"
#include "Common/StrOperation.h"
#include "Common/CodeOperation.h"

//Single
#include "Singleton.h"
#include "Common/WindowMessageDefine.h"
#include "Message/Message.h"
#include "Manager/ThreadManager.h"

//cur
#include "Common/FileOperation.h"

//Self Include
#include "CommonUIInc.h"

#define INTERFUN