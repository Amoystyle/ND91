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

#ifndef tstring
#ifdef UNICODE	
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif
#endif

#include "AssistantInclude.h"

#include "abldefine.h"

//Singleton
#include "Singleton.h"
#include "WindowMessageDefine.h"
#include "Message/Message.h"
#include "Message/GroupMessage.h"
#include "Manager/ThreadManager.h"


//Cul
#include "Common/GlobalOption.h"
#include "Common/log.h"
#include "Common/FileOperation.h"
#include "Common/StrOperation.h"
#include "Common/CodeOperation.h"

#include "Common/Device/DeviceInfoDefine.h"
#include "Common/Connection/ConnectionMessage.h"
#include "Common/Device/AblDeviceInfo.h"
#include "Common/Device/DeviceMessage.h"
#include "Common/Application/ApplicationMessage.h"
#include "Common/Statistics/StatisticsMessageDefine.h"
#include "Common/UserCenter/UserCenterMsg.h"
#include "Common/Device/DeviceMessage.h"

//Assistant
#include "Common/AssistantException.h"

//common ÏûÏ¢

#include "Common/NetDispatch/NetDispatchMessageDefine.h"
#include "Common/InstallCase/InstallsCase/InstallsCaseData.h"
#include "Common/InstallCase/InstallsCase/InstallsAppInfo.h"
