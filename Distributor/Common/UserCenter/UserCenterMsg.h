#pragma once

//用户登录成功
struct ResponseUserLoginMsg : public BaseMessage<ResponseUserLoginMsg>
{
	bool		bLongin;		// 用户是否已登入
	wstring		userName;		// 用户名	
	wstring		userID;			// 用户ID
	int			nCpaUser;		// 1为cpa用户，0为普通用户
	std::string	channelID;

	ResponseUserLoginMsg ()
	{
		bLongin		= false;
		userName	= L"";
		userID		= L"";	
		nCpaUser	= 0;
		channelID	= "";		
	}
};

//获取用户信息
struct RequestUserInfoMsg : public BaseMessage<RequestUserInfoMsg>
{
	wstring deviceID;
};

//返回用户信息
struct ResponseUserInfoMsg : public BaseMessage<ResponseUserInfoMsg>
{
	wstring deviceID;	// 用户退出，点击保存用户资料时有特殊意义
	wstring lastTime;
};

//保存用户信息
struct RequestSaveUserInfoMsg : public BaseMessage<RequestSaveUserInfoMsg>
{
	wstring deviceID;
};

//用户登录成功
struct GetUserLoginInfoMsg : public BaseMessage<GetUserLoginInfoMsg>
{
	wstring		userId;
	bool		bLongin;		// 用户是否已登入
	bool		bCpaUser;		// 1为cpa用户，0为普通用户
	std::string	channelID;

	GetUserLoginInfoMsg ()
	{
		userId		= L"";
		bLongin		= false;
		bCpaUser	= false;
		channelID	= "";		
	}
};

//开始下载守护和助手
struct BeginDownLoadDeamonMsg : public BaseMessage<BeginDownLoadDeamonMsg>
{
	
};

//开始安装守护和助手
struct BeginInstallDeamonMsg : public BaseMessage<BeginInstallDeamonMsg>
{
	wstring deviceID;
	bool	bAppSync;

	BeginInstallDeamonMsg ()
	{
		deviceID = L"";
		bAppSync = false;	
	}
};

//守护安装完成返回
struct RequestInstallDeamonOverMsg : public BaseMessage<RequestInstallDeamonOverMsg>
{
	wstring deviceID;           // 设备ID
};
