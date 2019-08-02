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
#include "Message/Message.h"
#include "Base/DataPath.h"


#include "Common/TaskCenter/TaskCenterMessageDefine.h"
#include "Common/WebResource/WebResourceMessageDefine.h"
#include "Common/InstallCase/InstallcaseMessage.h"
#include "Common/Statistics/StatisticsMessageDefine.h"

#include "Common/InstallCase/InstallsCase/InstallsAppInfo.h"
#include "Common/InstallCase/InstallsCase/InstallsCaseData.h"
#include "Common/InstallCase/NetInstallsCase/NetInstallsCaseData.h"
#include "Common/Device/DeviceInfoDefine.h"
#include "Common/Connection/ConnectionMessage.h"

//Cul
#include "Common/GlobalOption.h"
#include "Common/log.h"
#include "Common/FileOperation.h"
#include "Common/StrOperation.h"
#include "Common/CodeOperation.h"

//CommonUI
#include "CommonUIInc.h"

//TaskManager
#include "TaskItem.h"

//AssistantDll
#include "Common/MyFileOperation.h"
#include "Core/DeviceManager.h"
#include "Common/NetOperation.h"

//taskmanager
#include "CBase_ACK.h"

using namespace ND91Assistant;

//databaseManager
#include "SQLLiteDataBase/DBManager.h"


//self
#include "Common/RecentCaseSpace.h"
#include "Common/Utils.h"
#include "Common/Device/DeviceMessage.h"

USING_RENCENT_NSTALLCASE_Space;


#define INTERFUN ;