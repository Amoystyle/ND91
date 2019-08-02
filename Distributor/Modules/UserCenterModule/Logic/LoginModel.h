#pragma once

#define USERCENTERURL _T("http://zj.91.com") 
#define THEIEXPLORE	  _T("C:\\Program Files\\Internet Explorer\\iexplore.exe")	 

const unsigned char DES_KEY [24] = { 0x32, 0x73, 0xfe, 0x7c, 0x7f, 0x2f, 0x63, 0x48, 0x93, 0xd1, 0x71, 0x0c, 0x82, 0x41, 0xfa, 0x18, 0xc3, 0x97, 0x58, 0xdc, 0x08, 0xd0, 0x5e, 0x2a };

class LoginModel 
{
public:
    LoginModel();
    ~LoginModel();

	static LoginModel* GetInstance();

	// 用户自动登入
	bool AffirmLogin();

	// 用户输入账户信息成功登录
	// Returns  : std::wstring 成功返回用户名，失败返回空
	wstring LoginSuccess( bool bSave );


	bool  GetBeLogin()	{ return _bLongin; } 
	void  SetBeLogin( bool bLogin )	{ _bLongin = bLogin; } 	

	wstring	GetUserID()	{ return _userID; }
	void  SetUserID( wstring id )	{ _userID = id; }	
	
	wstring	GetUserName()	{ return _userName; }
	void  SetUserName( wstring name )	{ _userName = name; }	

	std::string	GetChannelID()	{ return _channelID; }

	int	GetBeCpaUser()	{ return _nCpaUser; }
	void SetBeCpaUser( int nCpaUser )	{ _nCpaUser = nCpaUser; }
	
	void ReSetData();

private:

	wstring GetGmtTime();

	// Summary  : 读取配置文件的用户信息
	// Returns  : void
	// Parameter: wstring & strId
	// Parameter: wstring & strName
	// Parameter: wstring & strCertif
	void ReadFile(wstring& strId, wstring& strName, wstring& strCertif, std::string& channelID, int& bCapUser );

	// Summary  : 往配置文件写用户信息
	// Returns  : void
	// Parameter: wstring strId
	// Parameter: std::string strName
	// Parameter: wstring strCertif
	void WriteFile(wstring strId, std::string strName, wstring strCertif, std::string channelID, int bCapUser );

	bool		_bLongin;		// 用户是否已登入
	wstring		_userName;		// 用户名	
	wstring		_userID;		// 用户ID
	int			_nCpaUser;		// 1为cpa用户，0为普通用户
	std::string	_channelID;

};