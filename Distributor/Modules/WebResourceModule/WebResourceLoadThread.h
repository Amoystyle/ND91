#pragma once
#include "Base/BaseThread.h"
#include <string>
#include "CWebResourceMacros.h"
#include "NetResourceStruct.h"
using namespace std;

struct WebResourceInnserMsg;
class BaseCSLock;
class CWebResourceLoadThread :
	public BaseThread,private BaseCSLock
{
public:
	CWebResourceLoadThread(const string threadId);
	~CWebResourceLoadThread(void);
	PROPERTY(NetEnumAppSort, _appSort, AppSort);
	PROPERTY(ENUM_PLATFORM, _platform, Platform);
private:
	virtual unsigned long Main();
	
	
};
