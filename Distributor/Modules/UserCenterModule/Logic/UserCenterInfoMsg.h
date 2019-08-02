#pragma once

//用户登录成功
struct UserLoginInfoMsg : public BaseMessage<UserLoginInfoMsg>
{
	bool		bLongin;		// 用户是否已登入
	wstring		userName;		// 用户名	
	wstring		userID;			// 用户ID
	int			nCpaUser;		// 1为cpa用户，0为普通用户
	std::string	channelID;

	UserLoginInfoMsg ()
	{
		bLongin		= false;
		userName	= L"";
		userID		= L"";	
		nCpaUser	= 0;
		channelID	= "";		
	}
};

//测试连接是否正常信息
struct CheckConnectInfoMsg : public BaseMessage<CheckConnectInfoMsg>
{
	bool bConnectOK;

	CheckConnectInfoMsg ()
	{
		bConnectOK = false;
	}

};

//获取用户信息
struct GetCustomerInfoMsg : public BaseMessage<GetCustomerInfoMsg>
{
	wstring userName;
	wstring phoneNum;
	wstring comment;
	wstring strSaveTime;

	GetCustomerInfoMsg ()
	{
		userName	= L"";
		phoneNum	= L"";
		comment		= L"";
		strSaveTime = L"";
	}
};

//发送用户信息
struct SendCustomerInfoMsg : public BaseMessage<SendCustomerInfoMsg>
{
	wstring strSaveTime;

	SendCustomerInfoMsg ()
	{
		strSaveTime = L"";
	}
};

