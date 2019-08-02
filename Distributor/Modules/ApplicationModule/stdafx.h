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
#include <ShlObj.h>
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
#include "Message/GroupMessage.h"
#include "Base/BaseThread.h"
#include "Base/DataPath.h"


//Cul
#include "Common/GlobalOption.h"
#include "Common/log.h"
#include "Common/FileOperation.h"
#include "Common/StrOperation.h"
#include "Common/CodeOperation.h"

//CommonUIFunc
#include "CommonUIInc.h"
#include "UI/BaseView.h"

//Common
#include "Common/Device/AblDeviceInfo.h"
#include "Common/Device/DeviceInfoDefine.h"
#include "Common/Device/DeviceMessage.h"
#include "Common/Device/DeviceModuleMessage.h"
#include "Common/Application/ApplicationMessage.h"
#include "Common/WebResource/WebResourceMessageDefine.h"

//taskmanager
#include "CBase_ACK.h"

//assistantdll
#include "Module/Application/AppData.h"
#include "Logic/AppDataUpgrade.h"
#include "Common/AssistantException.h"
#include "Core/DeviceManager.h"
#include "Common/NetOperation.h"
#include "Common/MyFileOperation.h"
#include "Module/Application/AppManager.h"
#include "Module/System/SystemManager.h"
#include "Monitor/ProgressObserver.h"
#include "CSLock.h"

//Application
#include "Logic/ApplicationInnerMsg.h"
#include "Common/BaseOperation.h"
#include "TaskItem.h"

#include "Common/AppNameSpace.h"

#include "Common/TaskCenter/TaskCenterMessageDefine.h"
#include "Common/MessageType.h"
#include "Common/DeviceNotifyCenterDefine.h"
#include "Common/InstallCase/InstallcaseMessage.h"
#include "Common/Statistics/StatisticsMessageDefine.h"

#include "Common/InstallCase/InstallsCase/InstallsAppInfo.h"
#include "Common/InstallCase/InstallsCase/InstallsCaseData.h"
#include "Common/InstallCase/NetInstallsCase/NetInstallsCaseData.h"


#define INTERFUN

using namespace ND91Assistant;
USING_App_Space
