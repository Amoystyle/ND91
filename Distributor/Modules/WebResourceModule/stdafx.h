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

#include "Core/DeviceManager.h"
#include "Core/DeviceData.h"
#include "Common/GlobalOption.h"
#include "Core/DeviceInfo.h"
#include "Core/International.h"

#define INTERFUN ;

//Singleton
#include "Singleton.h"
#include "Base/BaseModule.h"
#include "Base/SourceModule.h"
#include "Common/WindowMessageDefine.h"
#include "Message/Message.h"
#include "FileOperationEx.h"

//Cul

#include "Common/GlobalOption.h"
#include "Common/log.h"
#include "Common/FileOperation.h"
#include "Common/StrOperation.h"
#include "Common/CodeOperation.h"

#include "Common/NetOperation.h"
#include "Common/AssistantException.h"

//CommonUI
#include "CommonUIInc.h"

#include "Logic/CSLock.h"

#include "UI/BaseView.h"

#include "Common/TaskCenter/TaskCenterMessageDefine.h"
#include "Common/Device/DeviceInfoDefine.h"
#include "Common/Connection/ConnectionMessage.h"
#include "Logic/WebResourceInnerMsg.h"
#include "WebResourceManager.h"

#include "Common/NetOperation.h"
#include "Common/AssistantException.h"

#include "common/WebResourceModuleConfig.h"

#include "Common/MyFileOperation.h"

#include "common/WebResourceCommonFunc.h"
#include "common/BaseUI.h"
#include "Common/CommonDlg/CommonDlgMessageDefine.h"
#include "Common/WebResource/WebResourceMessageDefine.h"
#include "Common/InstallCase/InstallcaseMessage.h"
#include "Common/UserCenter/UserCenterMsg.h"


#include "Ufslib.h"
#include "StatisticDefine.h"

extern UfslibHandle _pUfsHandle;

#define Statistic_StatFunc(id) LOG->WriteDebug(L"记录统计点");if (_pUfsHandle) FunctionCount(_pUfsHandle, id);
#define Statistic_StatFuncTab(id, tab, ret) LOG->WriteDebug(L"记录统计点");if (_pUfsHandle) ret = FunctionCount(_pUfsHandle, id, NULL, tab);

using namespace ND91Assistant;
