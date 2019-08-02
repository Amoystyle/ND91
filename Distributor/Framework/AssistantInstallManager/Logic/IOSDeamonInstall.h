#pragma once
#include "DeamonInstall.h"

#define XML_PLIST_Base	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
                         <!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \
                         \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n<plist version=\"1.0\">\n\0"

class CIosDeamonInstall 
	: public CDeamonInstall
	, public BaseCSLock
{
public:
	CIosDeamonInstall(wstring deviceId);
	~CIosDeamonInstall(void);

	// Summary  : ��鱾�����ְ�װ���Ƿ��������Ƿ���Ҫ����
	// Returns  : bool
	// Parameter: int nDeviceVer
	// Parameter: bool isPad
	virtual bool CheckDeamonFilesPerfect( int nDeviceVer, bool isPad );

	// Summary  : �����ػ�
	// Returns  : bool
	virtual bool DownloadDeamon();


	// Summary  : ��װ����
	// Returns  : DEAMON_INSTALLSTATE
	virtual DEAMON_INSTALLSTATE InstallDeamon();

	// Summary  : ��鱾�����ְ�װ���Ƿ��������Ƿ���Ҫ����
	// Returns  : bool
	// Parameter: int nDeviceVer
	// Parameter: bool isPad
	virtual bool CheckAssistFilesPerfect( int nDeviceVer, bool isJailBreaked, bool isPad, bool isCpa=false );

	// Summary  : �����ֻ�������
	// Returns  : bool
	virtual bool DownloadAssistant(bool isJailBreaked, bool isPad, bool isCpa=false);

	// Summary  : ��װ�ػ�������
	// Returns  : DEAMON_INSTALLSTATE
	virtual DEAMON_INSTALLSTATE InstallAssistant();

	// Summary  : �ж��Ƿ���Ҫ��װ���߸����ػ�
	// Returns  : bool
	virtual bool NeedDeamonInstall();

	// Summary  : �ж��Ƿ���Ҫ��װ���߸�������
	// Returns  : bool
	virtual bool NeedAssistInstall();

private:

	// Summary  : ��鱾���ػ��ļ��Ƿ���Ҫ�����滻
	// Returns  : bool
	// Parameter: int nDeviceVer
	// Parameter: bool isPad
	bool CheckDeamonVerFromWeb( int nDeviceVer, bool isPad );


	// Summary  : ��鱾���ֻ��������Ƿ�������Ҫ�滻
	// Returns  : bool
	// Parameter: int nDeviceVer
	// Parameter: bool isPad
	bool CheckAssistantVerFromWeb( int nDeviceVer, bool isJailBreaked, bool isPad );

	// Summary  : �������ļ���ȡ�汾��
	// Returns  : std::wstring
	// Parameter: bool Deamon
	// Parameter: bool isPad
	wstring GetVersionInConfig(bool Deamon, bool isJailBreaked, bool isPad);

	// Summary  : �������ļ���ȡ�ļ�MD5ֵ
	// Returns  : std::wstring
	// Parameter: bool Deamon
	// Parameter: bool isPad
	wstring GetMD5InConfig(bool Deamon, bool isJailBreaked, bool isPad);

	// Summary  : ��������ID
	// Returns  : void
	void SetChannelID();

	// Summary  : ��ȡ�ֻ���91����·��
	// Returns  : std::wstring
	// Parameter: wstring appID
	wstring GetAssistPath(wstring appID);

};
