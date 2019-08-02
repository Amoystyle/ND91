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

	// �û��Զ�����
	bool AffirmLogin();

	// �û������˻���Ϣ�ɹ���¼
	// Returns  : std::wstring �ɹ������û�����ʧ�ܷ��ؿ�
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

	// Summary  : ��ȡ�����ļ����û���Ϣ
	// Returns  : void
	// Parameter: wstring & strId
	// Parameter: wstring & strName
	// Parameter: wstring & strCertif
	void ReadFile(wstring& strId, wstring& strName, wstring& strCertif, std::string& channelID, int& bCapUser );

	// Summary  : �������ļ�д�û���Ϣ
	// Returns  : void
	// Parameter: wstring strId
	// Parameter: std::string strName
	// Parameter: wstring strCertif
	void WriteFile(wstring strId, std::string strName, wstring strCertif, std::string channelID, int bCapUser );

	bool		_bLongin;		// �û��Ƿ��ѵ���
	wstring		_userName;		// �û���	
	wstring		_userID;		// �û�ID
	int			_nCpaUser;		// 1Ϊcpa�û���0Ϊ��ͨ�û�
	std::string	_channelID;

};