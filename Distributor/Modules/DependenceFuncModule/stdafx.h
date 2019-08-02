#pragma once

#define _CRT_SECURE_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN

// Windows Header Files:
#include <windows.h>

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
#include <shlobj.h>
#include <ShellAPI.h>
using namespace std;

#pragma warning(disable:4251)
#pragma warning(disable:4800)
#include "DirectUI.h"
#pragma warning(default:4251)
#pragma warning(default:4800)

#ifndef tstring
#ifdef UNICODE	
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif
#endif

//  π”√GDI
#include <GdiPlus.h>
#pragma comment (lib, "gdiplus.lib ")
using namespace Gdiplus;

#include "Common/WindowMessageDefine.h"

//Singleton
#include "Singleton.h"
#include "Base/BaseModule.h"
#include "Base/DeviceModule.h"
#include "Base/Config.h"
#include "Message/Message.h"
#include "Base/ClientInfo.h"
#include "Manager/EventManager.h"

//CommonMsg
#include "Common/Device/DeviceModuleMessage.h"
#include "Common/Statistics/StatisticsMessageDefine.h"
#include "Common/DependenceFuncCenter/DependenceFuncCenterMessage.h"
#include "Common/InstallCase/InstallcaseMessage.h"

#include "Common/CodeOperation.h"

//Cul
#include "Common/GlobalOption.h"
#include "Common/log.h"
#include "Common/FileOperation.h"
#include "Common/StrOperation.h"
#include "Common/CodeOperation.h"

//assistant
#include "Common/MyFileOperation.h"

using namespace ND91Assistant;
//CommonUI
#include "CommonUIInc.h"

#define INTERFUN ;