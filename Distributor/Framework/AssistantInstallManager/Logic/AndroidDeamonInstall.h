#pragma once
#include "DeamonInstall.h"

class CAndroidDeamonInstall : public CDeamonInstall
{
public:
	CAndroidDeamonInstall(wstring deviceId);
	~CAndroidDeamonInstall(void);


	// Summary  : ��鱾�����ְ�װ���Ƿ��������Ƿ����
	// Returns  : bool
	// Parameter: int nDeviceVer
	// Parameter: bool isPad
	virtual bool CheckAssistFilesPerfect( int nDeviceVer, bool isJailBreaked, bool isPad, bool isCpa=false );

	// Summary  : ��������
	// Returns  : bool
	virtual bool DownloadAssistant(bool isJailBreaked, bool isPad, bool isCpa=false);

	// Summary  : ��װ����
	// Returns  : DEAMON_INSTALLSTATE
	virtual DEAMON_INSTALLSTATE InstallAssistant();

	// Summary  : ��鱾���ػ���װ���Ƿ��������Ƿ���Ҫ����
	// Returns  : bool
	virtual bool CheckDeamonFilesPerfect( int nDeviceVer, bool isPad ) { return true ;}

	// Summary  : �����ػ�
	// Returns  : bool
	virtual bool DownloadDeamon(){ return true ;}

	// Summary  : ���ְ�װ
	// Returns  : DEAMON_PANDASPANCE_INSTALLSTATE
	virtual DEAMON_INSTALLSTATE InstallDeamon(){ return INSTALLSTATE_COMPLETED ;}

	// Summary  : �ж��Ƿ���Ҫ��װ���߸�������
	// Returns  : bool
	virtual bool NeedAssistInstall();

	// Summary  : �ж��Ƿ���Ҫ��װ���߸����ػ�
	// Returns  : bool
	virtual bool NeedDeamonInstall(){ return true ;}


private:

	// Summary  : ��鱾���ػ��ļ��Ƿ���Ҫ�����滻
	// Returns  : bool
	// Parameter: int nDeviceVer
	bool CheckAssistVerFromWeb( int nDeviceVer, bool isCpa=false );
	
	// Summary  : �������ļ���ȡ�汾��
	// Returns  : std::wstring
	wstring GetVersionInConfig(bool isCpa=false);

	// Summary  : �������ļ���ȡ�ļ�MD5ֵ
	// Returns  : std::wstring
	wstring GetMD5InConfig(bool isCpa=false);

	// Summary  : ��������ID
	// Returns  : void
	void SetChannelID();

	// Summary  : ��������ID�·���
	// Returns  : void
	void SetChannelIDNew();


};
