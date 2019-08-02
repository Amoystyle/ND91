#pragma once

//装机助手统计功能宏定义


//#define Statistic_APP_ID   100457		//一键越狱
//#define Statistic_APP_KEY  "853449c2396e68ae3667366a486cb56c874e1b3350df5031"

//#define Statistic_APP_ID   109739		//91的  有数据

#define Statistic_APP_ID	100571
#define Statistic_APP_KEY	"3cdbd2244fb7d781cf4c8938a5d3eb5413e35d1b626c787b"	

//  --------------------------------功能ID-------------------------------------

// 一、助手功能点	
#define Statistic_ID_MyDevice			"10000"		//我的设备
#define Statistic_ID_BatchTun			"20000"		//批量装机
#define Statistic_ID_LocalRes			"30000"		//本地资源
#define Statistic_ID_NetRes				"40000"		//网络资源
#define Statistic_ID_TaskCenter    		"50000"		//任务中心
#define Statistic_ID_Login				"60000"		//登录
#define Statistic_ID_CheckUpdate   		"70000"		//主窗体左下角的检查更新
#define Statistic_ID_InstallAssist		"80000"		//安装手机版助手
#define Statistic_ID_InstallForAnd		"80001"		//安装91助手(手机版) for Android
#define Statistic_ID_InstallForIos  	"80002"		//安装91助手(手机版) for iPhone
#define Statistic_ID_InstallForIpad		"80003"		//安装91助手(手机版) for iPad
#define Statistic_ID_InstallForLegal	"80004"		//安装91助手(手机版) for 限免

// 二、我的设备	  
#define Statistic_ID_InstallPathSet		"10001"		//安装路径设置
#define Statistic_ID_AddPlan     		"10002"		//【添加】装机方案
#define Statistic_ID_WhatPlan			"10003"		//什么是装机方案？
#define Statistic_ID_Refurbish     		"10004"		//已连接设备【刷新】按钮
#define Statistic_ID_InstallPlan     	"10005"		//【安装】装机方案-本地
#define Statistic_ID_PlanMore     		"10006"		//【更多…】装机方案
#define Statistic_ID_InstallNetPlan     "10007"		//【安装】装机方案-网络
#define Statistic_ID_InstallIosApp    	"10008"		//常用功能：安装程序-iOS
#define Statistic_ID_InstallAndApp  	"10009"		//常用功能：安装程序-Android
#define Statistic_ID_IosReboot  		"10010"		//常用功能：重新启动-iOS
#define Statistic_ID_AndroidReboot 		"10011"		//常用功能：重新启动-Android
#define Statistic_ID_InRecoveryMode		"10012"		//常用功能：进入恢复模式
#define Statistic_ID_OutRecoveryMode	"10013"		//常用功能：退出恢复模式
#define Statistic_ID_IosFreeMemory     	"10014"		//常用功能：内存释放-iOS
#define Statistic_ID_AndroidFreeMemory  "10015"		//常用功能：内存释放-Android
#define Statistic_ID_APP     			"11001"		//应用程序
#define Statistic_ID_UserAppTab     	"11002"		//【用户程序】Tab标签
#define Statistic_ID_SystemAppTab     	"11003"		//【系统程序】Tab标签
#define Statistic_ID_InstallPcApp   	"11004"		//【安装本地软件】按钮
#define Statistic_ID_TopUnstallBnt    	"11005"		//【卸载】按钮（顶部批量）
#define Statistic_ID_ListUnstallBnt		"11006"		//【卸载】按钮（列表单个）
#define Statistic_ID_AppMoveToSDcard	"11007"		//应用程序：移至SD卡（列表单个）
#define Statistic_ID_AppMoveToPhone    	"11008"		//应用程序：移至手机（列表单个）
#define Statistic_ID_LotAppMoveToSDcard	"11009"		//应用程序：移至SD卡（底部批量）
#define Statistic_ID_LotAppMoveToPhone 	"11010"		//应用程序：移至手机（顶部批量）
#define Statistic_ID_BackupRestore		"12001"		//备份还原
#define Statistic_ID_BackupTab     		"12002"		//【本地备份】Tab标签
#define Statistic_ID_RestoreTab     	"12003"		//【本地还原】Tab标签
#define Statistic_ID_DeriveTovCard		"12004"		//导出联系人vCard文件
#define Statistic_ID_DeriveToOutLook	"12005"		//导出联系人Outlook
#define Statistic_ID_vCardImport     	"12006"		//导入联系人vCard文件
#define Statistic_ID_OutLookImport   	"12007"		//导入联系人Outlook
#define Statistic_ID_AnyBackupFile		"12008"		//【选择其他备份】按钮
#define Statistic_ID_BackupContact		"12009"		//备份勾选【联系人】
#define Statistic_ID_BackupMessage		"12010"		//备份勾选【短信】
#define Statistic_ID_BackupCallReocrd	"12011"		//备份勾选【通话记录】
#define Statistic_ID_BackupMusic     	"12012"		//备份勾选【音乐】
#define Statistic_ID_BackupNote     	"12013"		//备份勾选【备忘录】
#define Statistic_ID_BackupWallPaper	"12014"		//备份勾选【壁纸】
#define Statistic_ID_BackupPicture		"12015"		//备份勾选【图片】
#define Statistic_ID_BackupApp   		"12016"		//备份勾选【应用程序】
#define Statistic_ID_BackupAppRecords	"12017"		//备份勾选【应用程序记录】
#define Statistic_ID_BackupAppForIPA	"12018"		//备份勾选【应用程序（仅备份IPA）】
#define Statistic_ID_BackupiBooks    	"12019"		//备份勾选【电子书】
#define Statistic_ID_BackupTheme     	"12020"		//备份勾选【主题】
#define Statistic_ID_BackupRing     	"12021"		//备份勾选【自定义铃声】
#define Statistic_ID_ChangeBackupPath	"12022"		//备份路径【修改】按钮
#define Statistic_ID_BackupBegin     	"12023"		//【开始备份】按钮
#define Statistic_ID_BackupPass     	"12024"		//备份时【跳过】按钮
#define Statistic_ID_BackupStop   		"12025"		//备份时【终止】按钮
#define Statistic_ID_BackupEnding    	"12026"		//备份完成【关闭】按钮
#define Statistic_ID_BackupFileLook		"12027"		//备份完成【立即查看】按钮
#define Statistic_ID_RestoreContact		"12028"		//还原勾选【联系人】
#define Statistic_ID_RestoreMessage		"12029"		//还原勾选【短信】
#define Statistic_ID_RestoreCallReocrd	"12030"		//还原勾选【通话记录】
#define Statistic_ID_RestoreMusic     	"12031"		//还原勾选【音乐】
#define Statistic_ID_RestoreNote     	"12032"		//还原勾选【备忘录】
#define Statistic_ID_RestoreWallPaper	"12033"		//还原勾选【壁纸】
#define Statistic_ID_RestorePicture		"12034"		//还原勾选【图片】
#define Statistic_ID_RestoreApp   		"12035"		//还原勾选【应用程序】
#define Statistic_ID_RestoreAppRecords	"12036"		//还原勾选【应用程序记录】
#define Statistic_ID_RestoreAppForIPA	"12037"		//还原勾选【应用程序（仅备份IPA）】
#define Statistic_ID_RestoreiBooks    	"12038"		//还原勾选【电子书】
#define Statistic_ID_RestoreTheme     	"12039"		//还原勾选【主题】
#define Statistic_ID_RestoreRing     	"12040"		//还原勾选【自定义铃声】
#define Statistic_ID_InstallSoft		"12041"		//【开始还原】按钮

#define Statistic_ID_FileManager     	"13001"		//文件管理
#define Statistic_ID_CreateDir   		"13002"		//文件管理-【新建文件夹】
#define Statistic_ID_UpLoadFile    		"13003"		//文件管理-【上传文件】
#define Statistic_ID_UpLoadDir			"13004"		//文件管理-【上传目录】
#define Statistic_ID_DownLoadFile		"13005"		//文件管理-【下载】
#define Statistic_ID_DeleteFile			"13006"		//文件管理-【删除】
#define Statistic_ID_UpdateTree			"13007"		//文件管理-【刷新】
#define Statistic_ID_RightBntMenu     	"13008"		//文件管理- 右键菜单
#define Statistic_ID_SearchFilter     	"13009"		//文件管理-搜索过滤
#define Statistic_ID_FileRename			"13010"		//文件管理-F2重命名
#define Statistic_ID_MobileUdisk		"13011"		//快速入口-移动U盘
#define Statistic_ID_MemoryCard   		"13012"		//快速入口-储存卡
#define Statistic_ID_MyDocuments		"13013"		//快速入口-我的文档
#define Statistic_ID_Picture			"13014"		//快速入口-照片
#define Statistic_ID_VoiceMemo    		"13015"		//快速入口-语音备忘录
#define Statistic_ID_UserAppList     	"13016"		//快速入口-用户应用程序
#define Statistic_ID_CydiaAppInstall    "13017"		//快速入口-Cydia软件安装



// 三、批量装机										
#define Statistic_ID_IosSysInstall		"20001"		//点击【iOS系统装机】节点
#define Statistic_ID_AndroidSysInstall	"20002"		//点击【Android系统装机】节点
#define Statistic_ID_OneKeyInstall		"20003"		//点击【一键装机】按钮

// 四、本地资源							  
#define Statistic_ID_LocalResBnt		"30001"		//点击【本地资源】节点										
#define Statistic_ID_IosAppLibrary		"30002"		//点击【iOS软件库】节点										
#define Statistic_ID_AndroidAppLibrary	"30003"		//点击【Android软件库】节点									
#define Statistic_ID_IosLibraryUpDate	"30004"		//点击【iOS软件库】-【可升级的应用】						
#define Statistic_ID_AnLibraryUpDate	"30005"		//点击【Android软件库】-【可升级的应用】					
#define Statistic_ID_InstallPlanBnt		"30006"		//点击【装机方案】节点										
#define Statistic_ID_AutoAddPcAPP		"31001"		//本地资源：点击【自动添加磁盘中的手机应用】按钮			
#define Statistic_ID_StopAutoAddApp		"31002"		//本地资源：点击【自动添加磁盘中的手机应用】-【停止】按钮	
#define Statistic_ID_AddIosLibraryBnt	"31003"		//本地资源：点击【添加】-【iOS软件库】按钮					
#define Statistic_ID_AddAnLibraryBnt	"31004"		//本地资源：点击【添加】-【Android软件库】按钮				
#define Statistic_ID_OpenDirBnt			"31005"		//本地资源：点击【打开目录】按钮							
#define Statistic_ID_MoveToBnt   		"31006"		//本地资源：点击【移除】按钮								
#define Statistic_ID_DeleteBnt    		"31007"		//本地资源：点击【删除】按钮								
#define Statistic_ID_IosAddBnt     		"32001"		//iOS软件库：点击【添加】按钮								
#define Statistic_ID_IosSameCheck		"32002"		//iOS软件库：勾选【相同应用仅显示最新版本】					
#define Statistic_ID_IosBatchDelete		"32003"		//iOS软件库：点击【删除】按钮（批量）						
#define Statistic_ID_IosBatchInstall	"32004"		//iOS软件库：点击【一键装机】按钮（批量）					
#define Statistic_ID_IosSingleInstall	"32005"		//iOS软件库：点击单个软件【安装】按钮						
#define Statistic_ID_IosSingleDelete	"32006"		//iOS软件库：点击单个软件【删除】按钮						
#define Statistic_ID_IosUpSameCheck		"32007"		//iOS软件库-可升级的应用：勾选【相同应用仅显示最新版本】	
#define Statistic_ID_IosUpSingleCheck	"32008"		//iOS软件库-可升级的应用：点击单个软件【升级】按钮			
#define Statistic_ID_IosUpBatchCheck	"32009"		//iOS软件库-可升级的应用：点击【升级】按钮（批量）			
#define Statistic_ID_IosUpBatchOneKey	"32010"		//iOS软件库-可升级的应用：点击【一键装机】按钮（批量）		
#define Statistic_ID_AnAddBnt     		"33001"		//Android软件库：点击【添加】按钮							
#define Statistic_ID_AnSameCheck		"33002"		//Android软件库：勾选【相同应用仅显示最新版本】				
#define Statistic_ID_AnBatchDelete		"33003"		//Android软件库：点击【删除】按钮（批量）					
#define Statistic_ID_AnBatchInstall		"33004"		//Android软件库：点击【一键装机】按钮（批量）				
#define Statistic_ID_AnSingleInstall	"33005"		//Android软件库：点击单个软件【安装】按钮					
#define Statistic_ID_AnSingleDelete		"33006"		//Android软件库：点击单个软件【删除】按钮					
#define Statistic_ID_AnUpSameCheck		"33007"		//Android软件库-可升级的应用：勾选【相同应用仅显示最新版本】
#define Statistic_ID_AnUpSingleCheck	"33008"		//Android软件库-可升级的应用：点击单个软件【升级】按钮		
#define Statistic_ID_AnUpBatchCheck		"33009"		//Android软件库-可升级的应用：点击【升级】按钮（批量）		
#define Statistic_ID_AnUpBatchOneKey	"33010"		//Android软件库-可升级的应用：点击【一键装机】按钮（批量）	
#define Statistic_ID_CreateNewPlan  	"34001"		//装机方案：点击【创建新方案】按钮							
#define Statistic_ID_DeleteAllSelect  	"34002"		//装机方案：点击【删除所选方案】按钮（批量）				
#define Statistic_ID_APlanAddBnt  		"34003"		//装机方案-某个方案：点击【添加】按钮						
#define Statistic_ID_AnAppAddBnt  		"34004"		//装机方案-某个方案：点击单个软件【安装】按钮				
#define Statistic_ID_AnAppMoveBnt  		"34005"		//装机方案-某个方案：点击单个软件【移除】按钮				
#define Statistic_ID_ABatchInstallBnt	"34006"		//装机方案-某个方案：点击【一键装机】按钮（批量）			
#define Statistic_ID_ABatchMoveBnt		"34007"		//装机方案-某个方案：点击【移除】按钮（批量）				

// 五、网络资源					  
#define Statistic_ID_LatelyUpdate		"40001"		//点击【最近更新】节点
#define Statistic_ID_TodayHot			"40002"		//点击【今日热门】节点
#define Statistic_ID_InstallNeed		"40003"		//点击【装机必备】节点
#define Statistic_ID_SoftTreasure		"40004"		//点击【软件宝库】节点
#define Statistic_ID_CompleteOfGame		"40005"		//点击【游戏大全】节点
#define Statistic_ID_SearchBnt			"40006"		//点击【搜索】按钮
#define Statistic_ID_UpdateBnt			"40007"		//点击【刷新】按钮

#define Statistic_ID_DownLoadBnt		"40008"		//点击【下载】按钮
#define Statistic_ID_UnJailbrokenFree   "40009"		//点击【不越狱也免费】节点
#define Statistic_ID_IphonePlatForm     "41001"		//不越狱也免费：iPhone平台
#define Statistic_ID_IpadPlatForm		"41002"		//不越狱也免费：iPad平台
#define Statistic_ID_DownNumOf24Hot		"41003"		//不越狱也免费：24小时热门应用下载量
#define Statistic_ID_PageOf24Hot		"41004"		//不越狱也免费：24小时热门应用翻页
#define Statistic_ID_SearchLegalApp		"41005"		//不越狱也免费：搜索正版软件
#define Statistic_ID_DownBntOfDetail	"41006"		//不越狱也免费：软件详情页中【下载】按钮
#define Statistic_ID_DownLoadMostBnt	"41007"		//不越狱也免费：【下载最多】标签
#define Statistic_ID_PriceHighBnt		"41008"		//不越狱也免费：【价格最高】标签
#define Statistic_ID_LastRelease		"41009"		//不越狱也免费：【最新发布】标签
#define Statistic_ID_PullDownChange		"41010"		//不越狱也免费：分类下拉切换
#define Statistic_ID_FlashAnnealing  	"41011"		//不越狱也免费：闪退修复
#define Statistic_ID_DownLoadTotal 		"41012"		//不越狱也免费：正版免费软件总下载量
#define Statistic_ID_GoToPage  			"41013"		//不越狱也免费：页面底部的指定页面跳转功能
#define Statistic_ID_WebSoft_Category   "41014"		//不越狱也免费：首页分类


// 六、任务中心	  
#define Statistic_ID_DownloadTaskBnt	"50001"		//点击【下载任务】节点
#define Statistic_ID_MessageBoxClose	"50002"		//弹出提示框【关闭】按钮
#define Statistic_ID_MessageBoxLook		"50003"		//弹出提示框【点击查看】按钮
#define Statistic_ID_DownloadBeginBnt	"51001"		//下载任务：点击单个软件【开始】按钮
#define Statistic_ID_DownloadStopBnt	"51002"		//下载任务：点击单个软件【暂停】按钮
#define Statistic_ID_DownloadDeleteBnt	"51003"		//下载任务：点击单个软件【删除】按钮
#define Statistic_ID_DownloadBatBegin	"51004"		//下载任务：点击【开始】按钮（批量）
#define Statistic_ID_DownloadBatDelete	"51005"		//下载任务：点击【删除】按钮（批量）
#define Statistic_ID_DownloadBatAutoDel	"51006"		//下载任务：点击【删除】-【自动删除已完成任务】
#define Statistic_ID_InstallAnAppRetry 	"52001"		//安装卸载：点击单个软件【重试】按钮
#define Statistic_ID_InstallAnAppDel  	"52002"		//安装卸载：点击单个软件【删除】按钮
#define Statistic_ID_InstallBatchRetry  "52003"		//安装卸载：点击【重新安装】按钮（批量）
#define Statistic_ID_InstallBatchDel  	"52004"		//安装卸载：点击【删除】按钮（批量）
#define Statistic_ID_InstallAutoDel  	"52005"		//安装卸载：点击【删除】-【自动删除已完成任务】

#define Statistic_ID_NumOfTaskBegin  	"52006"		//软件开始下载任务数
#define Statistic_ID_NumOfSuccess		"52007"		//软件下载成功数
#define Statistic_ID_NumOfInstallBegin  "52008"		//应用开始安装任务数
#define Statistic_ID_NumOfInstallOK		"52009"		//应用安装成功数


// 七、登录		  
#define Statistic_ID_NextAutoLoginBnt  	"60001"		//勾选【下次自动登录】选项
#define Statistic_ID_CloseBnt  			"60002"		//点击【关闭】按钮
#define Statistic_ID_LoginInBnt  		"60003"		//未登录点击【关闭】登录


//  分类统计功能ID
#define Statistic_ID_ConnectDeviceNum 	"900001"	//装机助手同时最大连接设备数
#define Statistic_ID_InstallPlanNum  	"900002"	//装机方案的总数
#define Statistic_ID_ExecuteDuration	"900003"	//单个装机方案安装时长
#define Statistic_ID_AppNumInAnPlan  	"900004"	//单个装机方案的应用总数
#define Statistic_ID_ZJStartTime  		"900005"	//装机助手启动时段	

struct CStaticsticsMessage :
	BaseMessage<CStaticsticsMessage>
{
	CStaticsticsMessage(string id)
	{	
		_id = id;
	}
	
	string _id;
};

struct CStaticsticsMessageWithTab :
	BaseMessage<CStaticsticsMessageWithTab>
{
	string _id;
	string _tab;
	bool ret;
	CStaticsticsMessageWithTab(string id, string tab)
	{
		_id = id;
		_tab = tab;
		ret = false;
	}
};

struct CStaticsticsDevice : public BaseMessage<CStaticsticsDevice>
{
	void* pAblDeviceInfo;
};

struct PostDownloadInfoMsg : public BaseMessage<PostDownloadInfoMsg>
{
	wstring m_appid;
	wstring m_platform;
	wstring m_csFileDownUrl;
	wstring m_strName;
	wstring m_appVersion;
	wstring m_csFileSavePath;
};

struct PostInstallResultInfoMsg : public BaseMessage<PostInstallResultInfoMsg>
{
	wstring m_platform;
	int  m_fid;
	wstring m_appid;
	bool bSuccess;
};


struct PostDownloadFinishMsg : public BaseMessage<PostDownloadFinishMsg>
{
	unsigned long m_nSpeed;
	wstring m_platform;
	int  m_fid;
	int m_uStatus;
	PostDownloadFinishMsg()
	{
		m_fid = 0;
		m_uStatus = 0;
	}
};

struct PostInstallAppMsg : public BaseMessage<PostInstallAppMsg>
{
	int size;
	int duration;
	PostInstallAppMsg()
	{
		size = 0;
		duration = 0;
	}
};


struct PostInstallInfoMsg : public BaseMessage<PostInstallInfoMsg>
{
	  wstring m_appid;
	  wstring m_csFileSavePath;
	  wstring m_strName;
	  wstring m_appVersion ;
	  wstring deviceId ;
};

struct PostInstallFailInfoMsg : public BaseMessage<PostInstallFailInfoMsg>
{
	wstring filePath;
	int m_fid;
	wstring m_appid;
	wstring m_appVersion;
	unsigned long TotalSize;
	wstring m_strName;
	wstring m_installerror;
	wstring deviceId;
	
	PostInstallFailInfoMsg()
	{
		m_fid = 0;
	}
	
};

//这个消息临时先放这里
struct RequestWindowSystemInfoMsg : public BaseMessage<RequestWindowSystemInfoMsg>
{
	// 判断操作系统是否是64位
	bool bIs64;

	//是否是Vista系统
	bool bIsVista;

	//是否是Windows7系统
	bool bIsWindows7;

	//是否是XP系统
	bool bIsXP;

	//获取系统信息
	wstring SystemInfo;

	//获取电脑的MAC地址
	std::string Mac;

	//获取win系统版本
	std::string Os;

	//是否需要安装itunes
	bool bIsNeedInstalliTunes;

};

struct PostDeviceConnectNumMsg : public BaseMessage<PostDeviceConnectNumMsg>
{

};

struct PostStartTimeMsg : public BaseMessage<PostStartTimeMsg>
{

};

struct PostInstallPlanListMsg : public BaseMessage<PostInstallPlanListMsg>
{
	void * pAppInfo;
};

struct PostZJAppNumOfAnPlanMsg : public BaseMessage<PostZJAppNumOfAnPlanMsg>
{
	int num;
};

struct PostZJInstallDurationMsg : public BaseMessage<PostZJInstallDurationMsg>
{
	int num;
	int allSize;
};

struct PostInstallPlanMsg : public BaseMessage<PostInstallPlanMsg>
{
	int size;
	int num;
};

struct PostZJInstallPlanNumMsg : public BaseMessage<PostZJInstallPlanNumMsg>
{
	void* pInstallsCaseDataCollection;
};