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

	// Summary  : 检查本地助手安装包是否完整，是否需要更新
	// Returns  : bool
	// Parameter: int nDeviceVer
	// Parameter: bool isPad
	virtual bool CheckDeamonFilesPerfect( int nDeviceVer, bool isPad );

	// Summary  : 下载守护
	// Returns  : bool
	virtual bool DownloadDeamon();


	// Summary  : 安装助手
	// Returns  : DEAMON_INSTALLSTATE
	virtual DEAMON_INSTALLSTATE InstallDeamon();

	// Summary  : 检查本地助手安装包是否完整，是否需要更新
	// Returns  : bool
	// Parameter: int nDeviceVer
	// Parameter: bool isPad
	virtual bool CheckAssistFilesPerfect( int nDeviceVer, bool isJailBreaked, bool isPad, bool isCpa=false );

	// Summary  : 下载手机版助手
	// Returns  : bool
	virtual bool DownloadAssistant(bool isJailBreaked, bool isPad, bool isCpa=false);

	// Summary  : 安装守护和助手
	// Returns  : DEAMON_INSTALLSTATE
	virtual DEAMON_INSTALLSTATE InstallAssistant();

	// Summary  : 判断是否需要安装或者更新守护
	// Returns  : bool
	virtual bool NeedDeamonInstall();

	// Summary  : 判断是否需要安装或者更新助手
	// Returns  : bool
	virtual bool NeedAssistInstall();

private:

	// Summary  : 检查本地守护文件是否需要升级替换
	// Returns  : bool
	// Parameter: int nDeviceVer
	// Parameter: bool isPad
	bool CheckDeamonVerFromWeb( int nDeviceVer, bool isPad );


	// Summary  : 检查本地手机版助手是否需升级要替换
	// Returns  : bool
	// Parameter: int nDeviceVer
	// Parameter: bool isPad
	bool CheckAssistantVerFromWeb( int nDeviceVer, bool isJailBreaked, bool isPad );

	// Summary  : 从配置文件获取版本号
	// Returns  : std::wstring
	// Parameter: bool Deamon
	// Parameter: bool isPad
	wstring GetVersionInConfig(bool Deamon, bool isJailBreaked, bool isPad);

	// Summary  : 从配置文件获取文件MD5值
	// Returns  : std::wstring
	// Parameter: bool Deamon
	// Parameter: bool isPad
	wstring GetMD5InConfig(bool Deamon, bool isJailBreaked, bool isPad);

	// Summary  : 设置渠道ID
	// Returns  : void
	void SetChannelID();

	// Summary  : 获取手机版91助手路径
	// Returns  : std::wstring
	// Parameter: wstring appID
	wstring GetAssistPath(wstring appID);

};
