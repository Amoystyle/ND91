#pragma once

enum TaskStatistcsAction
{
	Action_PostDownLoadInfo,	// 上报应用下载信息(装机商后台)
	Action_PostInstallInfo,		// 上报应用安装信息	
	Action_SendDataToService,	// 向服务端发送数据(开始未登录，下载/安装操作后登录)
	Action_PostDataToService,	// 向服务端发送数据(之前未发送成功的启动程序后发送)
	Action_SendDownloadInfo,	// 91后台发送下载完成统计
	Action_SendInstallInfo,		// 91后台发送安装成功统计
	Action_SendFailInfo		    // 91后台发送安装失败统计
};

struct StaticsticsInnerMsg : BaseMessage<StaticsticsInnerMsg>
{
	TaskStatistcsAction _action;
	void* voidParam;

	StaticsticsInnerMsg(TaskStatistcsAction action)
	{
		_action = action;
		voidParam = NULL;
	}


};
