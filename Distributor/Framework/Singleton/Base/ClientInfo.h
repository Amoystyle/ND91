#pragma once
#include "BaseCSLock.h"

class VersionCheck;
class SINGLETON_API CClientInfo
{
public:
	static CClientInfo* GetInstance();
	string GetChangleId();
	string GetVersion();
	string GetIniVersion();
	int GetAppId();
	string GetAppKey();
	~CClientInfo(void);
private:
	CClientInfo(void);
	static CClientInfo* _instance;
	static BaseCSLock* _lock;
	VersionCheck* _version;
	int _appId;
	string _appKey;
};
