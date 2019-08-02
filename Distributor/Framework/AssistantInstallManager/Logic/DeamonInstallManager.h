#pragma once
#include "DeamonInstall.h"
#include "Common/UserCenter/UserCenterMsg.h"

class DeamonInstallManager
	: public BaseCSLock
	, public BaseHandler<BeginInstallDeamonMsg>  
{
public:
	DeamonInstallManager();
	~DeamonInstallManager(void);

	static DeamonInstallManager* GetInstance();

	// Summary  : 安装守护
	// Returns  : bool
	bool InstallDeamon( wstring deviceId, bool bAppSync );


	DEAMON_INSTALLSTATE GetResult() { return _Result ;}

	// Summary  : 记录已经安装成功的设备ID
	// Returns  : void
	// Parameter: wstring strID
	// Parameter: bool bsuccess
	void RecordTask(wstring strID, bool bsuccess);

	// Summary  : 判断该设备是否已经安装
	// Returns  : bool
	// Parameter: wstring strID
	bool HasInstall(wstring strID);

	bool _bInstallFinish;			// 手机版助手是否安装

private:

	virtual bool Handle(const BeginInstallDeamonMsg* pMsg);


private:
		
	DEAMON_INSTALLSTATE _Result;

	map<wstring, bool> _mapDeviceHasInstall;

};
