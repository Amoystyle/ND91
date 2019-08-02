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

	// Summary  : ����url���������ػ�������
	// Returns  : bool
	bool DownloadDeamons();

	// Summary  : �����豸�����Լ��ػ������ְ�װ�����ȡ����ָ��
	// Returns  : DeamonInstall*
	// Parameter: bool isDeamon
	// Parameter: bool isIos
	// Parameter: bool isPad
	// Parameter: bool isJailBreaked
	CDeamonInstall* GetTaskType( bool isDeamon, bool isIos, bool isCpa=false, bool isJailBreaked=false, bool isPad=false );


private:

	virtual bool Handle(const BeginDownLoadDeamonMsg* pMsg);

	// Summary  : ��ʼ�����ػ���������
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
