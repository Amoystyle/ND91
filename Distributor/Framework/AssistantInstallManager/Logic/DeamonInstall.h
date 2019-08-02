#pragma once


#define IOSDEAMON		L"DEAMON"

#define IPHONEASSAT		L"IPHONE"
#define IPADASSAT		L"IPAD"
#define IOSLEGALASSAT	L"LEGAL"
#define ANDROIDASSAT	L"ANDROID"
#define OFFICIALASSAT	L"OFFICIAL"

#define LEGALASSATID	L"netdragon.mobile.iphone.91pandaspace"
#define IOSASSATID		L"netdragon.mobile.iphone.91space"
#define IPADASSATID		L"netdragon.mobile.ipad.91space"
#define ANDROIDASSATID	L"com.dragon.android.pandaspace"

const int PandaMinSize = 1024 * 1024 * 3;

// 安装状态
enum DEAMON_INSTALLSTATE
{
	INSTALLSTATE_WAITTING,
	INSTALLSTATE_RUNNING,
	INSTALLSTATE_COMPLETED,
	INSTALLSTATE_FAIL,
};

class CDeamonInstall
{
public:

	CDeamonInstall(wstring deviceId = L"");
	~CDeamonInstall();


	// Summary  : 初始化函数
	// Returns  : void
	void Init();

	// Summary  : 设置用户登录信息
	// Returns  : void
	// Parameter: bool bLogin
	// Parameter: bool bCpaUser
	// Parameter: std::string channelID
	void SetUserInfo( bool bLogin, bool bCpaUser, std::string channelID );

	// Summary  : 检查本地助手安装包是否完整，是否需要更新(IOS专用)
	// Returns  : bool
	virtual bool CheckDeamonFilesPerfect(int nDeviceVer = 5, bool isPad = false) = 0;

	// Summary  : 下载守护(IOS专用)
	// Returns  : bool
	virtual bool DownloadDeamon() = 0;

	// Summary  : 安装守护(IOS专用)
	// Returns  : DEAMON_INSTALLSTATE
	virtual DEAMON_INSTALLSTATE InstallDeamon() = 0;


	// Summary  : 检查本地助手安装包是否完整，是否需要更新
	// Returns  : bool
	// Parameter: int nDeviceVer
	// Parameter: bool isPad
	virtual bool CheckAssistFilesPerfect( int nDeviceVer, bool isJailBreaked=false, bool isPad=false, bool isCpa=false ) = 0;

	// Summary  : 下载手机版助手
	// Returns  : bool
	virtual bool DownloadAssistant(bool isJailBreaked=false, bool isPad=false, bool isCpa=false) = 0;

	// Summary  : 安装助手
	// Returns  : DEAMON_INSTALLSTATE
	virtual DEAMON_INSTALLSTATE InstallAssistant() = 0;


	// Summary  : 判断是否需要安装或者更新守护
	// Returns  : bool
	virtual bool NeedDeamonInstall() = 0;

	// Summary  : 判断是否需要安装或者更新助手
	// Returns  : bool
	virtual bool NeedAssistInstall() = 0;


	bool _bDownLoadFinish;					// 任务是否下载完成


protected:

	//检查版本
	bool CompareVersion(vector<wstring> LocalVersionList,vector<wstring> serviceVersionList,size_t pos);



	wstring _strIniPath;

	wstring _strIosDir;
	wstring _strIosAssistantDir;
	wstring _strIosDeamonDir;

	wstring _strDeamonPath;					// ios设备各固件版本路径
	wstring _strIphoneAssistPath;
	wstring _strIpadAssistantPath;

	wstring _strLegalAssistPath;			// 正版手机助手


	wstring _strAndDir;
	wstring _strAndAssistantDir;
	wstring _strAssistantAndPath;			// android设备各固件版本路径
	wstring _strAssOfficialAndPath;			// android设备官方无渠道ID版本

	wstring _strServiceVer;					// Deamon服务端最新版本
	wstring _strServiceUrl;					// Deamon下载地址	


	DEAMON_INSTALLSTATE _InstallState;

	wstring		_deviceId;
	DeviceData* _pDevData;

	bool _bLogin;
	bool _bCpaUser;
	std::string _strChannelID;


private:
	
	bool		_bHasInit;

		

};
