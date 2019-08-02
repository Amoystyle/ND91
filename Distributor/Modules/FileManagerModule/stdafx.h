#pragma once

#include "targetver.h"


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
using namespace std;

// DirectUI header file
#pragma warning(disable:4251)
#pragma warning(disable:4482)
#include "DirectUI.h"
#pragma warning(default:4482)
#pragma warning(default:4251)

#ifndef tstring
#ifdef UNICODE	
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif
#endif

#include <shlobj.h>
#include <ShellAPI.h>

#include "Common/WindowMessageDefine.h"


//Singleton
#include "Singleton.h"
#include "Base/BaseModule.h"
#include "Base/DeviceModule.h"
#include "Message/Message.h"
#include "Base/DataPath.h"

//CommonMsg
#include "Common/Device/DeviceModuleMessage.h"

//Cul
#include "Common/GlobalOption.h"
#include "Common/log.h"
#include "Common/FileOperation.h"
#include "Common/StrOperation.h"
#include "Common/CodeOperation.h"

//CommonUI
#include "CommonUIInc.h"

//Assistant
#include "Module/System/SystemManager.h"
#include "Module/File/DirData.h"
#include "Module/File/LinkData.h"
#include "Module/File/FileHelper.h"
#include "Core/International.h"


#include "Ufslib.h"
#include "StatisticDefine.h"

extern UfslibHandle _pUfsHandle;

#define Statistic_StatFunc(id) LOG->WriteDebug(L"记录统计点");if (_pUfsHandle) FunctionCount(_pUfsHandle, id);
#define Statistic_StatFuncTab(id, tab, ret) LOG->WriteDebug(L"记录统计点");if (_pUfsHandle) ret = FunctionCount(_pUfsHandle, id, NULL, tab);

using namespace ND91Assistant;

#include "common/ProgressObServer.h"
#include "common/FileManagerMsg.h"
