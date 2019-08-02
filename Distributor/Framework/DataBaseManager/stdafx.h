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
#include <atlstr.h>
using namespace std;

#ifndef tstring
#ifdef UNICODE	
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif
#endif
#include "Common/WindowMessageDefine.h"


#include "Common/CodeOperation.h"
#define INTERFUN ;