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

#include "Common/WindowMessageDefine.h"

//Singleton
#include "Singleton.h"
#include "Base/BaseModule.h"
#include "Base/DeviceModule.h"
#include "Base/SourceModule.h"
#include "Message/Message.h"
#include "Message/GroupMessage.h"

//CommonMsg
#include "Common/Device/DeviceModuleMessage.h"

//Cul
#include "Common/GlobalOption.h"
#include "Common/log.h"
#include "Common/FileOperation.h"
#include "Common/StrOperation.h"
#include "Common/CodeOperation.h"

//Common
#include "Common/Device/DeviceInfoDefine.h"
#include "Common/Connection/ConnectionMessage.h"
#include "Common/Application/ApplicationMessage.h"
#include "Common/Statistics/StatisticsMessageDefine.h"
#include "Common/CommonMessage/ModuleMessage.h"
#include "Common/InstallCase/InstallcaseMessage.h"
//CommonUI
#include "CommonUIInc.h"

//DeviceModules
#include "Modules/ConnectGuideModule/ConnectGuideModule.h"
#include "Modules/DeviceInfoModule/DeviceInfoModule.h"
#include "Modules/ApplicationModule/ApplicationModule.h"
#include "Modules/FileManagerModule/FileManagerModule.h"
#include "Modules/BackUpAndRestoreModule/BackUpAndRestoreModule.h"
#include "Modules/RecentInstallCaseModule/RecentInstallCaseModule.h"

#include "Logic/DeviceInnerMsg.h"
#include "Logic/DeviceModuleType.h"