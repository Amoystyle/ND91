#pragma once

#include "AssistantDll.h"
#include "Common/StrOperation.h"
#include "Common/CodeOperation.h"
#include "Common/NetOperation.h"
#include "Common/FileOperation.h"

#include <Windows.h>
#include <tchar.h>
#include <string>

using namespace std;

class CLASSINDLL_CLASS_DECL CGetDeviceHardInfo
{
public:
	CGetDeviceHardInfo(wstring strID);
	~CGetDeviceHardInfo(void);

	bool _success;
	wstring _vid;
	wstring _pid;

private:

	void GetHardInfo();
	wstring _id;
};
