#pragma once
#include "DeamonInstall.h"

class CAndroidDeamonInstall : public CDeamonInstall
{
public:
	CAndroidDeamonInstall(wstring deviceId);
	~CAndroidDeamonInstall(void);


	// Summary  : 检查本地助手安装包是否完整，是否更新
	// Returns  : bool
	// Parameter: int nDeviceVer
	// Parameter: bool isPad
	virtual bool CheckAssistFilesPerfect( int nDeviceVer, bool isJailBreaked, bool isPad, bool isCpa=false );

	// Summary  : 下载助手
	// Returns  : bool
	virtual bool DownloadAssistant(bool isJailBreaked, bool isPad, bool isCpa=false);

	// Summary  : 安装助手
	// Returns  : DEAMON_INSTALLSTATE
	virtual DEAMON_INSTALLSTATE InstallAssistant();

	// Summary  : 检查本地守护安装包是否完整，是否需要更新
	// Returns  : bool
	virtual bool CheckDeamonFilesPerfect( int nDeviceVer, bool isPad ) { return true ;}

	// Summary  : 下载守护
	// Returns  : bool
	virtual bool DownloadDeamon(){ return true ;}

	// Summary  : 助手安装
	// Returns  : DEAMON_PANDASPANCE_INSTALLSTATE
	virtual DEAMON_INSTALLSTATE InstallDeamon(){ return INSTALLSTATE_COMPLETED ;}

	// Summary  : 判断是否需要安装或者更新助手
	// Returns  : bool
	virtual bool NeedAssistInstall();

	// Summary  : 判断是否需要安装或者更新守护
	// Returns  : bool
	virtual bool NeedDeamonInstall(){ return true ;}


private:

	// Summary  : 检查本地守护文件是否需要升级替换
	// Returns  : bool
	// Parameter: int nDeviceVer
	bool CheckAssistVerFromWeb( int nDeviceVer, bool isCpa=false );
	
	// Summary  : 从配置文件获取版本号
	// Returns  : std::wstring
	wstring GetVersionInConfig(bool isCpa=false);

	// Summary  : 从配置文件获取文件MD5值
	// Returns  : std::wstring
	wstring GetMD5InConfig(bool isCpa=false);

	// Summary  : 设置渠道ID
	// Returns  : void
	void SetChannelID();

	// Summary  : 设置渠道ID新方法
	// Returns  : void
	void SetChannelIDNew();


};
