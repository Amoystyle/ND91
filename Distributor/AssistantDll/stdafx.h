#pragma once

#pragma warning (disable:4251)

#include <map>
#include <set>
#include <list>
#include <deque>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include <assert.h>

#ifdef WIN32
    #include <io.h>
    #include <windows.h>
	#include <Iphlpapi.h>
    #include <cfgmgr32.h>
    #include <winsock.h>
    #include <WinInet.h>
    #include <GdiPlus.h>
    #include <process.h>
    #include <newdev.h>
    #include <errno.h>
    
    #include "stdint.h"

    #pragma comment(lib, "newdev.lib")
    #pragma comment(lib, "setupapi.lib")
    #pragma comment(lib, "rpcrt4.lib")
    #pragma comment(lib, "Iphlpapi.lib")
    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "shell32.lib")
    #pragma comment(lib, "ole32.lib")
    #pragma comment(lib, "version.lib")
    #pragma comment(lib, "advapi32.lib")
    #pragma comment(lib, "wininet.lib")
    #pragma comment(lib, "newdev.lib")
#else
    #include <string.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <stdint.h>
    #include <uuid/uuid.h>
    #include <dlfcn.h>
    #include <dirent.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <sys/types.h>

    #include "Common/winPort.h"

#endif

#include "AssistantDll.h"
#include "common/AssistantException.h"
#include "common/BaseException.h"
#include "common/SystemException.h"
#include "common/Log.h"
#include "common/DealTime.h"
#include "common/CodeOperation.h"
#include "common/FileOperation.h"
#include "Common/IniFileOperation.h"
#include "common/StrOperation.h"
#include "Common/CSLock.h"
#include "Common/GlobalOption.h"
#include "Common/MyCodeOperation.h"
#include "common/MyFileOperation.h"
#include "common/MyStrOperation.h"

using namespace std;
using namespace Gdiplus;
using namespace ND91Assistant;