#pragma once

struct AblApplicationItem
{
	// 程序信息
	wstring _packageName;		// 安装包名 ISO表示程序的唯一标识
	int     _nVersionCode;      // android内部版本号，与升级相关                 
	wstring _strVersion;		// 版本描述                   
	wstring _AppName;			// 应用程序名             
	wstring _strAppPhonePath;	// 程序路径 (手机中)              

	wstring _strIconPhonePath;	// 图标路径 (手机中)
	wstring _strIconPCPath;		// 图标路径 (PC中)

	int		_isLegalSoft;		//是否正版

	int64_t _dynamicDiskUsage;	// 文档大小
	int64_t _staticDiskUsage;	// 程序大小
};

struct RequestLoadAppMsg : public BaseMessage<RequestLoadAppMsg>
{
	wstring deviceID;
	wstring searchKey;
};

struct ResponseLoadAppMsg : public GroupMessage<ResponseLoadAppMsg>
{
	AblApplicationItem item;
};

// 获取设备信息缓存
struct RequestInstallAppMsg : public BaseMessage<RequestInstallAppMsg>
{
	wstring deviceID;

	wstring appPath;
};

struct ResponseInstallAppMsg : public BaseMessage<ResponseInstallAppMsg>
{
	wstring deviceID;
};

enum NotifyInstallAppType
{
	NotifyInstallAppType_SelectFiles,
	NotifyInstallAppType_SelectFolder,
	NotifyInstallAppType_InstallFiles,
};

struct NotifyInstallAppMsg : public BaseMessage<NotifyInstallAppMsg>
{
	NotifyInstallAppMsg()
	{
		module = NULL;
		bRes = false;
		vetFiles.clear();
		bFile = true;
	}
	void* module;

	wstring deviceID;

	NotifyInstallAppType nType;
	wstring wsText;
	bool   bFile;
	vector<wstring> vetFiles;
	bool  bRes;
};

struct NotifyRefreshAppMsg : public BaseMessage<NotifyInstallAppMsg>
{
	NotifyRefreshAppMsg()
	{
		module = NULL;
	}
	void* module;
};

//应用线程发过来的消息
struct AppThreadMsg : public BaseMessage<AppThreadMsg>
{
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
};

enum Path_Type
{
	E_TYPE_ICON,
	E_TYPE_IosDownPath,
	E_TYPE_AndroidDownPath,
	E_TYPE_OptionIni,
};
//获取配置指定的路径
struct GetAppointPathMsg : public BaseMessage<GetAppointPathMsg>
{
	Path_Type st_eType;
	wstring   st_wsPath;
};

//刷新主窗口
struct InvalidateMainMsg : BaseMessage<InvalidateMainMsg>
{
	InvalidateMainMsg(){}
};


//线程发给appmodel加载数据
struct LoadAppDataMsg : public BaseMessage<LoadAppDataMsg>
{
};

//线程发给appmodel卸载软件
struct UnInstallAppMsg : public BaseMessage<UnInstallAppMsg>
{
	UnInstallAppMsg()
	{
		_st_pVoidList = NULL;
	}
	vector<void*>* _st_pVoidList;
};

//线程发给appmodel安装软件
struct InstallAppMsg : public BaseMessage<InstallAppMsg>
{
	InstallAppMsg()
	{
		_str = L"";
	}
	vector<wstring> _strList;
	wstring _str;
};


//线程发给appmodel升级盗版软件
struct UpGradeSinglePriacyMsg : public BaseMessage<UpGradeSinglePriacyMsg>
{
	UpGradeSinglePriacyMsg()
	{
		pVoidParam = NULL;
		bislegal = false;
	}
	void* pVoidParam;
	bool  bislegal;
};

//线程发给appmodel升级所有软件
struct UpgradeAllMsg : public BaseMessage<UpgradeAllMsg>
{
	UpgradeAllMsg()
	{
		nUpgradeType = 0;
	}
	int nUpgradeType;
};


//线程发给appmodel加载软件图标
struct AppLoadIconMsg : public BaseMessage<AppLoadIconMsg>
{
	AppLoadIconMsg()
	{
		pVoidParam = NULL;
	}
	void* pVoidParam;
};