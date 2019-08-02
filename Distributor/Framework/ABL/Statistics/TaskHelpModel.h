#pragma once

#include "UbdgLib.h"
#include "Thread/StaticsticsInnerMsg.h"

struct PostDownloadInfoMsg;
class CTaskStatisticsThread;

// 资源类型常量
enum FeedbackResType
{				
	None = 0,				// 非现有资源(0)						
	Soft = 1,				// 软件资源(1)		[Description("软件")]						
	Theme = 2,				// 主题资源(2)		[Description("主题")]	
	Ring = 3,				// 铃声资源(3)		[Description("铃声")]
	Picture = 4,			// 壁纸资源(4)		[Description("壁纸")]	
	NovelBook = 5,			// 小说(5)			[Description("小说")]	
	EZine = 6,				// 电子杂志(6)		[Description("电子杂志")]
	Sms = 7,				// 短信(7)			[Description("短信")]
	EBook = 8,				// 电子书(8)		[Description("电子书")]
	MobileGame = 9,			// 手机游戏(9)		[Description("手机游戏")]	
	Hardware = 10,			// 硬件站资源(10)	[Description("硬件站资源")]
	Cartoon = 11,			// 漫画(11)			[Description("漫画")]	
	RSS = 12,				// 新闻(12)			[Description("Rss新闻")]		
	Music = 13,				// 音乐(13)			[Description("音乐")]
	Video = 14,				// 视频(14)			[Description("视频")]
	College91 = 15,			// 91商学院资源(15)	[Description("91商学院资源")]
	KT91 = 16,				// 特纳专区(16)		[Description("特纳专区")]		
	ItuneFreeApp,			// 正版下载(17)		[Description("正版下载")]
};


// 反馈类型
enum FeedbackType
{
	Download			= 1,
	Browse				= 2,
	Ratting				= 3,
	DownloadSuccess		= 4,
	SetupSuccess		= 5,
	SetupFail			= 6,
	DownloadFromSub		= 7,						
	DownloadFail		= 8,		// 下载失败统计
	RepeatSetupSuccess	= 9,		// 重复安装成功
};


class TaskHelpModel :
	//public BaseHandler<StaticsticsInnerMsg>,
	public BaseHandler<PostDownloadInfoMsg>,
	public BaseHandler<PostDownloadFinishMsg>,
	public BaseHandler<PostInstallInfoMsg>,
	public BaseHandler<PostInstallResultInfoMsg>,
	public BaseHandler<PostInstallFailInfoMsg>
{
	friend class CTaskStatisticsThread;
public:
    TaskHelpModel();//BaseUI* pUI
    ~TaskHelpModel();

	// 单例
	static TaskHelpModel* GetInstance();

	// Summary  : 向装机商后台发送app下载记录
	// Returns  : void
	// Parameter: InstallsAppInfo* appInfo
	void PostDownLoadInfo( wstring m_appid, wstring m_platform,wstring m_csFileDownUrl,wstring m_strName,wstring m_appVersion,wstring m_csFileSavePath);

	// Summary  : 向装机商后台发送app安装记录
	// Returns  : void
	// Parameter: CTaskItem * item
	// Parameter: wstring deviceId
	void PostInstallInfo( wstring m_appid, wstring m_csFileSavePath,wstring m_strName,wstring m_appVersion ,wstring deviceId );

	// Summary  : 向服务端发送数据(开始未登录，下载/安装操作后登录)
	// Returns  : void
	void SendDataToService( wstring strID );

	// Summary  : 向服务端发送数据(之前未发送成功的启动程序后发送)
	// Returns  : void
	void PostDataToService();


//	---------------------------------------------------------------------------------
//	向91后台服务端发送应用下载，安装信息
//	---------------------------------------------------------------------------------

	// Summary  : 91后台下载完成统计
	// Returns  : void
	// Parameter: CTaskItem * item
	void PostDownloadFinish( unsigned long m_nSpeed,wstring m_platform,int  m_fid, int m_uStatus);

	// Summary  : 91后台安装结果统计
	// Returns  : void
	// Parameter: CTaskItem * item
	// Parameter: wstring deviceId
	void PostInstallResultInfo( wstring m_platform,int  m_fid,wstring m_appid, bool bSuccess);

	// Summary  : 91后台发送安装失败统计
	// Returns  : void
	// Parameter: CTaskItem * item
	// Parameter: wstring deviceId
	void PostInstallFailInfo( wstring filePath, int m_fid, wstring m_appid, wstring m_appVersion,
		unsigned long TotalSize,wstring m_strName,wstring m_installerror,
		wstring deviceId );

private:

	// Summary  : 保存下载app未上报成功数据
	// Returns  : void
	void SaveDownloadData(APPDOWNLOAD_RECORD* pdata);

	// Summary  : 保存安装app未上报成功数据
	// Returns  : void
	void SaveInstallData(APPINSTALL_RECORD* pdata);

	// Summary  : 从配置文件读取下载app未上报成功数据
	// Returns  : APPDOWNLOAD_RECORD * pdata
	APPDOWNLOAD_RECORD* ReadDownloadData();

	// Summary  : 从配置文件读取安装app未上报成功数据
	// Returns  : APPINSTALL_RECORD * pdata
	APPINSTALL_RECORD* ReadInstallData();

	// Summary  : 释放数据
	// Returns  : void
	void ReleaseData();

	virtual bool Action(int nAction, StaticsticsInnerMsg* pParam);  
	bool SendDownLoadInfo(StaticsticsInnerMsg* pParam);
	bool SendInstallInfo(StaticsticsInnerMsg* pParam);
	bool SendInfo(StaticsticsInnerMsg* pParam);
	bool SendData(StaticsticsInnerMsg* pParam);
	bool SendDownloadSms(StaticsticsInnerMsg* pParam);
	bool SendSuccessInfo(StaticsticsInnerMsg* pParam);
	bool SendFailInfo(StaticsticsInnerMsg* pParam);


private:
	
	void SetValues(char* szBuf, std::string str);

	// 获取ini文件的char*键值
	std::string GetIniCharValue( std::string strIniFile, std::string strGroup, std::string strKey );
	// 获取ini文件的int键值
	long GetIniIntValue( std::string strIniFile, std::string strGroup, std::string strKey );

	//virtual bool Handle( const StaticsticsInnerMsg* pMsg );

	virtual bool Handle( const PostDownloadInfoMsg* pMsg );

	virtual bool Handle( const PostDownloadFinishMsg* pMsg );

	virtual bool Handle( const PostInstallInfoMsg* pMsg );

	virtual bool Handle( const PostInstallResultInfoMsg* pMsg );

	virtual bool Handle( const PostInstallFailInfoMsg* pMsg );

	vector<APPDOWNLOAD_RECORD*> _verDownloads;
	vector<APPINSTALL_RECORD*>  _verInstalls;
	int _nDowntCount;
	int _nInstallCount;

	std::string					_nowVersion;

};