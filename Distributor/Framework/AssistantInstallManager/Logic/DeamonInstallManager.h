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

	// Summary  : ��װ�ػ�
	// Returns  : bool
	bool InstallDeamon( wstring deviceId, bool bAppSync );


	DEAMON_INSTALLSTATE GetResult() { return _Result ;}

	// Summary  : ��¼�Ѿ���װ�ɹ����豸ID
	// Returns  : void
	// Parameter: wstring strID
	// Parameter: bool bsuccess
	void RecordTask(wstring strID, bool bsuccess);

	// Summary  : �жϸ��豸�Ƿ��Ѿ���װ
	// Returns  : bool
	// Parameter: wstring strID
	bool HasInstall(wstring strID);

	bool _bInstallFinish;			// �ֻ��������Ƿ�װ

private:

	virtual bool Handle(const BeginInstallDeamonMsg* pMsg);


private:
		
	DEAMON_INSTALLSTATE _Result;

	map<wstring, bool> _mapDeviceHasInstall;

};
