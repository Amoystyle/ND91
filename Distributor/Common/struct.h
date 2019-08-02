#pragma once

enum eLoadState
{
    LoadState_Unload,
    LoadState_Loading,
    LoadState_Loaded,
};

enum DeviceConnectState
{
    Device_Connect,
    Device_Disconnect,
    Device_DFUConnect,
    Device_DFUDisconnect,
    Device_Linkin,
};

enum RunUpdateType
{
    NoRunUpdate,
    RunForNormal,
    RunForSilent,
    RunForUpdate,
};

enum ActionIndex
{
    ACTION_INFO_LOADDEVICEINFO,
    ACTION_INFO_NET,
    ACTION_INFO_POWERINFO,
    Action_File_Download,
    Action_BS_Backup,
    Action_BS_Restore,
    Action_Init,
    Action_Check,
    Action_APP_LOAD,
    Action_APP_INSTALL,
    Action_APP_UNINSTALL,
	Action_APP_UP2PARICY,
	Action_APP_UP2LEGAL,
	Action_APP_UPGRADEALL,
    Action_Load_TaskList,
	Action_Delete_Task,
	Action_Start_Task,
	Action_Install_Deamon,
    Action_Driver_Install,
    Action_LoadData,
    Action_LoadWebData,
	Action_GetCustomerInfo,
	Action_SendCustomerInfo,
	Action_FM_LoadRootData,		// 加载节点
    Action_FM_Download,         // 文件管理：下载
    Action_FM_UploadFile,       // 文件管理：上传文件
    Action_FM_UploadDir,        // 文件管理：上传文件夹
    Action_FM_Delete,           // 文件管理：删除
    Action_FM_DragOutFile,      // 文件管理：拖出文件
    Action_FM_Paste,            // 文件管理：粘贴
	Action_FM_Load_App,		    // 文件管理：加载应用程序名
	Action_FM_Ctreat_Dir,	    // 文件管理：预创建dir目录
	Action_DIS_Progress,	    // 驱动安装设置：显示进度
	Action_DIS_Install,		    // 驱动安装设置：下载及安装操作
	Action_DIS_InstallOne,		// 驱动安装设置：单个下载及安装操作
	Action_SearchApp,		    // 软件搜索
	Action_DoMemoryClear,       // 加载网络装机方案
    Action_Device_Clearup,      // 一键清理
	Action_Device_Reboot,	    // 重启
	Action_Device_FixFlashExit,	// 闪退修复
	Action_BR_Export,	        // 导出联系人
	Action_BR_Import,	        // 导入联系人
	Action_Post_StaticData,	    // 上报统计数据(应用启动,渠道信息)
    Action_SpaceInfoChanged,    // 空间信息变更
	
	Action_GetHotAppsInfo,		// 获取24小时热门应用列表
	Action_GetSoftCategorysInfo,// 获取游戏或者软件分类列表
	Action_GetAppDatasInfo,		// 获取软件数据列表,带分页
	Action_GetGameDatasInfo,	// 获取游戏数据列表
	Action_GetOneAppInfo,		// 获取单个软件详细信息
	Action_SearchAppsInfo,		// 搜索软件
	Action_DownLoadApp,			// 下载单个应用
	Action_GetAppsIcon,			// 获取应用的缩略图

	Action_PostDownLoadInfo,	// 上报应用下载信息(装机商后台)
	Action_PostInstallInfo,		// 上报应用安装信息	
	Action_SendDataToService,	// 向服务端发送数据(开始未登录，下载/安装操作后登录)
	Action_PostDataToService,	// 向服务端发送数据(之前未发送成功的启动程序后发送)
	Action_SendDownloadInfo,	// 91后台发送下载完成统计
	Action_SendSuccessInfo,		// 91后台发送安装成功统计
	Action_SendFailInfo,		// 91后台发送安装失败统计

	Action_InstallNetPlan,		//安装网络装机方案
	Action_App_LoadIcon,        //加载AppIcon
};
