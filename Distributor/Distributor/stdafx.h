/********************************************************************
	created:	2013/01/06 18:37
	filename: 	stdafx.h
	author:		QiuChang
	
	purpose:	公共头文件（非公共的请勿添加在此）
*********************************************************************/
#pragma once

#include "targetver.h"

#define _CRT_SECURE_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN

// Windows 头文件:
#include <windows.h>
#include <atlstr.h>
#include <wininet.h>
#include <ShellAPI.h>
// C 运行时头文件
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// DirectUI header file
#pragma warning(disable:4251)
#pragma warning(disable:4482)
#include "DirectUI.h"
#pragma warning(default:4482)
#pragma warning(default:4251)

#include <list>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <deque>
#include <set>
#include <time.h>
#include <winver.h>
using namespace std;

// 使用GDI
#include <GdiPlus.h>
#pragma comment (lib, "gdiplus.lib ")
using namespace Gdiplus;

#pragma comment(lib, "version.lib")

#include "Common/WindowMessageDefine.h"

//Singleton
#include "Singleton.h"
#include "Base/BaseModule.h"
#include "Base/DeviceModule.h"
#include "Base/SourceModule.h"
#include "Message/Message.h"
#include "Message/GroupMessage.h"
#include "Manager/EventManager.h"


//Cul
#include "Common/GlobalOption.h"
#include "Common/log.h"
#include "Common/FileOperation.h"
#include "Common/StrOperation.h"
#include "Common/CodeOperation.h"
#include "stdint.h"

#include "Common/InstallCase/InstallcaseMessage.h"
#include "Common/DependenceFuncCenter/DependenceFuncCenterMessage.h"
#include "Common/Application/ApplicationMessage.h"
#include "Common/Statistics/StatisticsMessageDefine.h"

//CommonUI
#include "CommonUIInc.h"

#include "UI/MainDlg.h"
#include "Modules/ModuleManager/ModuleManager.h"

#define INTERFUN