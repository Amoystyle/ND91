#pragma once

#define CONFIG_HANDLE CConfig::GetInstance()
#define CONFIG(e)	CConfig::GetInstance()->e

struct GetConfigMsg;

// 系统配置类，主要用来配置一些固定参数
class SINGLETON_API CConfig
{
private:
	static CConfig* _pInstance;   // 单例
public:
	// 取得单例对象
	static CConfig* GetInstance();
	// 释放单力对象
	static void Release();

	//设置是否第一次运行
	void SetFirstTimeRun( bool val );

	//获取是否第一次运行
	bool GetFirstTimeRun();

	//获取是否显示网络资源New图标
	bool GetShowWebNew();

	//设置是否显示网络资源New图标
	void SetShowWebNew(bool val);

	//设置下载路径
	void SetDownLoadPath( int platform, wstring downpath );

	//设置是否自动删除已经完成任务
	void SetAutoDeleteTask(bool del);

	static std::wstring GetIniFileValue( wstring strIniFile, wstring strGroup, wstring strKey );

	static bool SetIniFileValue( wstring strIniFile, wstring strGroup, wstring strKey, wstring strValue );

private:
	CConfig();
	virtual ~CConfig();

    void LoadDefault();
    void LoadOption(wstring strIniFilePath);
	void LoadUserSetting();
public:
    wstring _strUserDataPath;   // 用户数据目录
    wstring _strWorkPath;       // 工作目录    
    wstring _strTemp;           // 临时目录
    wstring _strInfoFile;       // 信息文件
    wstring _strDumpPath;       // Dum文件保存目录
    wstring _strDriverPath;     // 驱动文件保存目录
    wstring _strStatisticPath;  // 统计目录
    wstring _strDownTempPath;   // 下载临时目录
    wstring _strBackupPath;     // 备份目录
    wstring _strDownloadPath;   // 下载目录
    wstring _strDevicePhotoPath;// 设备图片目录
    wstring _strSoftwarePath;   // 软件下载目录    
    wstring _strAssistPath;     // 守护下载目录
    wstring _strUpdatePath;     // 更新文件目录
    wstring _strIconPath;       // 图标目录
	wstring _strWebLegalSoft;	// 正版应用目录
    wstring _strOptionIni;      // Option.ini文件路径,即程序的配置
    wstring _strUserSettingIni; // 用户配置文件
	wstring _strCachePath;      // 缓存目录
    wstring _strLogPath;        // Log路径
    wstring _strIosDownPath;      // ios软件默认下载路径
    wstring _strAndroidDownPath;  // android软件默认下载路径
	wstring _strIos_CpaDownPath;      // ios Cpa软件默认下载路径
	wstring _strAndroid_CpaDownPath;  // android Cpa软件默认下载路径
	wstring _strBizDictionaryIni; // 数据字典表，存放非设置的键值对值
	wstring _strSHSHPath;		//SHSH备份路径

	wstring _strDriverInstallIni;	//驱动设置已下载驱动配置文档
	wstring _strDriverInstallXML;	//驱动设置可安装驱动配置文档

	wstring _strTrayIconPath;		//托盘图标对应地址；    

    bool _bEnableAndroid;       // 是否激活Android
    bool _bEnableIos;           // 是否激活Ios
	bool _bFirstTimeRun;		//是否第一次运行
	bool _bShowWebNew;			//是否显示网络资源New图标

	bool _bAppStart;			//开机是否启动
	bool _bAppClose;			//关闭时是关闭还是最小化到托盘
	bool _bDownLoadMsg;			//下载任务开始时是否给予提示
	bool _bSoftInstallMsg;		//软件安装完成时是否给予提示
	bool _bAutoDeleteTask;		//是否自动删除已经完成任务
	bool _bAutoDelUpdatedApp;	//升级后是否自动删除低版本的应用
};

