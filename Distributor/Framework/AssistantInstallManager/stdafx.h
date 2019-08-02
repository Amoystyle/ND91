#pragma once

#include "targetver.h"

#define _CRT_SECURE_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN

// Windows Header Files:
#include <windows.h>
#include <shlobj.h>
#include <ShellAPI.h>

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

#pragma comment(lib, "version.lib")

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

#include "Singleton.h"
#include "Base/BaseModule.h"
#include "Base/SingletonDefine.h"
// TODO: 在此处引用程序需要的其他头文件
#include "Base/BaseModule.h"
#include "Common/WindowMessageDefine.h"
#include "Message/Message.h"
#include "Message/GroupMessage.h"

#include "Base/BaseThread.h"

//Cul
#include "Common/GlobalOption.h"
#include "Common/log.h"
#include "Common/FileOperation.h"
#include "Common/StrOperation.h"
#include "Common/CodeOperation.h"
#include "Common/NetOperation.h"
#include "Common/MyFileOperation.h"
#include "Common/AssistantException.h"

//CommonUIFunc
#include "CommonUIInc.h"

//Message
#include "Common/Device/DeviceInfoDefine.h"
#include "Common/Device/DeviceMessage.h"
#include "Common/Statistics/StatisticsMessageDefine.h"


//Assistant
#include "Core/DeviceData.h"
#include "Core/DeviceManager.h"
#include "Module/File/DirData.h"
#include "Module/File/NodeData.h"
#include "Module/File/FileHelper.h"
#include "Module/Application/AppManager.h"
using namespace ND91Assistant;