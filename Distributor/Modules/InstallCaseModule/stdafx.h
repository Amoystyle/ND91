#pragma once

#include "targetver.h"


#define _CRT_SECURE_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN

// Windows Header Files:
#include <windows.h>
#include <atlstr.h>
#include <ShellAPI.h>
#include <ShlObj.h>
#include <atldef.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>

#include <list>
#include <string>
#include <sstream>
#include <fstream>
#include <iosfwd>
#include <map>
#include <set>
#include <vector>
#include <deque>
#include <algorithm>
#include <time.h>
#include <winver.h>
#include <WinInet.h>
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
#if WIN32

   #pragma comment(lib, "wininet.lib")
#endif

#define INTERFUN

#include "Common/WindowMessageDefine.h"

//Singleton
#include "Singleton.h"
#include "Base/BaseModule.h"
#include "Base/SourceModule.h"
#include "Message/Message.h"
#include "Message/GroupMessage.h"
#include "Base/BaseThread.h"
#include "Base/Config.h"
#include "Base/DataPath.h"
#include "Manager/EventManager.h"


//Cul
#include "Common/GlobalOption.h"
#include "Common/log.h"
#include "Common/FileOperation.h"
#include "Common/StrOperation.h"
#include "Common/CodeOperation.h"
#include "stdint.h"


#include "Common/TaskCenter/TaskCenterMessageDefine.h"
#include "Common/InstallCase/InstallcaseMessage.h"
#include "Common/WebResource/WebResourceMessageDefine.h"
#include "Common/Statistics/StatisticsMessageDefine.h"
#include "Common/UserCenter/UserCenterMsg.h"
#include "Common/MessageType.h"
#include "Common/InstallCase/InstallsCase/InstallsAppInfo.h"
#include "Common/InstallCase/InstallsCase/InstallsCaseData.h"
#include "Common/InstallCase/NetInstallsCase/NetInstallsCaseData.h"
#include "Common/Device/DeviceInfoDefine.h"
#include "Common/Device/DeviceMessage.h"
#include "Common/Connection/ConnectionMessage.h"

//CommonUI
#include "CommonUIInc.h"


//AssistantDll
#include "Core/DeviceManager.h"
#include "Common/MyFileOperation.h"
#include "Common/AssistantException.h"
//lib
#include "Common/FileOperation.h"
#include "Common/NetOperation.h"
#include "Common/StrOperation.h"
#include "Common/SystemInfo.h"

//taskmanager
#include "CBase_ACK.h"

using namespace ND91Assistant;
using namespace std;

//databaseManager
#include "SQLLiteDataBase/DBManager.h"

//self
#include "Common/InstallCaseNameSpace.h"
#include "Common/CSLock.h"
#include "Common/Utils.h"
#include "Common/TickChecker.h"
#include "Logic/InstallsCase/InstallsCaseDB.h"
#include "Logic/InstallsCase/InstallCaseManagerEx.h"
#include "Logic/InstallsCase/InstallsCaseDBEx.h"
#include "TaskItem.h"

#include "Common/Statistics/StatisticsMessageDefine.h"


USING_NSTALLCASE_Space