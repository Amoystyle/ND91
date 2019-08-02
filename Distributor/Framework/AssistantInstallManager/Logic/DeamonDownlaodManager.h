#pragma once
#include "Common/UserCenter/UserCenterMsg.h"

class CDeamonInstall;
class DeamonDownloadManager 
	: public BaseHandler<BeginDownLoadDeamonMsg> 
{
public:
	DeamonDownloadManager();
	~DeamonDownloadManager(void);

	static DeamonDownloadManager* GetInstance();

	// Summary  : 根据url下载所有守护和助手
	// Returns  : bool
	bool DownloadDeamons();

	// Summary  : 根据设备类型以及守护和助手安装情况获取任务指针
	// Returns  : DeamonInstall*
	// Parameter: bool isDeamon
	// Parameter: bool isIos
	// Parameter: bool isPad
	// Parameter: bool isJailBreaked
	CDeamonInstall* GetTaskType( bool isDeamon, bool isIos, bool isCpa=false, bool isJailBreaked=false, bool isPad=false );


private:

	virtual bool Handle(const BeginDownLoadDeamonMsg* pMsg);

	// Summary  : 开始下载守护或者助手
	// Returns  : bool
	// Parameter: DeamonInstall * pInstall
	// Parameter: bool isDeamon
	// Parameter: bool isPad
	// Parameter: int nDeviceVer
	bool StartDownload( CDeamonInstall* pInstall, bool isDeamon, bool isCpa=false, bool isJailBreaked=false, bool isPad=false, int nDeviceVer=5 );


private:

	CDeamonInstall* _pIosDeamon;
	CDeamonInstall* _pLegalAssist;
	CDeamonInstall* _pIosAssist;
	CDeamonInstall* _pIpadAssist;

	CDeamonInstall* _pAndAssist;
	CDeamonInstall* _pOfficialAssist;


};
