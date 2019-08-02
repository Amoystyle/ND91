// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

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
#include <iostream>
#include <fstream>
using namespace std;

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
#include "Base/Config.h"
#include "Message/Message.h"
#include "Base/DataPath.h"
#include "Base/BaseThread.h"
#include "Base/BaseCSLock.h"

#include "Common/NetDispatch/NetDispatchMessageDefine.h"

//cur
#include "Common/CodeOperation.h"
#include "Common/FileOperation.h"
#include "Common/StrOperation.h"

//assistant
#include "Common/NetOperation.h"
#include "Common/w3c.h"

using namespace ND91Assistant;
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
