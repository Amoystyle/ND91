#include "stdafx.h"
#include "LoginModel.h"
#include "CommCrypt.hpp"
#include <fstream>
#include "UserCenterThread/AffirmLoginThread.h"

static LoginModel* _pLongInM = NULL;

LoginModel* LoginModel::GetInstance()
{
	if (_pLongInM == NULL)
		_pLongInM = new LoginModel;
	return _pLongInM;
}

LoginModel::LoginModel()
{
	_bLongin	= false;
	_nCpaUser	= 0;
}

LoginModel::~LoginModel()
{
	_bLongin = false;
	SAFE_DELETE(_pLongInM);
}

wstring LoginModel::GetGmtTime()
{
	struct tm *newtime; 
	__int64 ltime; 

	_time64( &ltime ); 

	/*   Obtain   coordinated   universal   time:   */ 
	newtime = _gmtime64(&ltime); 

	string dayTime = asctime( newtime );
	dayTime += "GMT";
	return CCodeOperation::GB2312ToUnicode(dayTime);
}

bool LoginModel::AffirmLogin()
{	
	wstring strCertif;
	wstring iniPath = CONFIG(_strCachePath) + L"ulog";

	if ( !CFileOperation::IsFileExist( iniPath.c_str() ) )
	{
		_bLongin = false;
		return false;
	}

	// 读取配置文件信息
	ReadFile(_userID, _userName, strCertif, _channelID, _nCpaUser);

	if ( !_userID.empty() && !_userName.empty() && !strCertif.empty() )
	{
		const wstring centerUrl = L"http://zj.91.com";  
		wstring strData = L"UserId=" + _userID + L"&UserName=" + _userName + L"&Certificate=" + strCertif
						   + L"&ChannelID=" + CCodeOperation::StringToWstring(_channelID) + L"&HasCpa=" + 
						   CStrOperation::IntToWString(_nCpaUser);

		wstring endData = L"ZJZS_Account=" + strData;

		// Thu, 09-Aug-2012 13:35:36 GMT
		wstring daytime = GetGmtTime();
		int pos = daytime.find(' ');
		daytime.insert( pos, 1, ',' );
		endData += L"; expires=" + daytime + L"; path=/";

		RETURN_VALUE_IF ( TRUE != InternetSetCookie(centerUrl.c_str(), NULL, endData.c_str()), _bLongin = false );
		std::string postData = CCodeOperation::UnicodeToUTF_8(strData);

		AffirmLoginThread *loginThread = new AffirmLoginThread("");
		loginThread->_postData = postData;
		loginThread->Run();

	}

	return true;
}

wstring LoginModel::LoginSuccess( bool bSave )
{
	DWORD len = 1024;
	WCHAR netData[1024] = {0};

	RETURN_VALUE_IF ( TRUE != InternetGetCookie( USERCENTERURL, L"ZJZS_Account", netData, &len), L"" );
	wstring temp = netData;

	int pos1 = temp.find(L"UserId=");
	int pos2 = temp.find(L"&UserName=");
	int pos3 = temp.find(L"&Certificate=");
	int pos4 = temp.find(L"&ChannelID=");
	int pos5 = temp.find(L"&HasCpa=");

	RETURN_VALUE_IF ( temp.empty() || pos1<1 || pos2<6 || pos3<9|| pos4<12 || pos5<15, L"" );
	
	wstring username, strcertif;
	_userID		= temp.substr( pos1+7, pos2-pos1-7 );
	username	= temp.substr(pos2+10, pos3-pos2-10);
	strcertif	= temp.substr(pos3+13, pos4-pos3-13);
	_channelID	= CNetOperation::URLDecode( CCodeOperation::UnicodeToUTF_8( temp.substr(pos4+11, pos5-pos4-11) ) );
	_nCpaUser	= _wtoi( temp.substr(pos5+8).c_str() );

	_bLongin = true;

	std::string name = CNetOperation::URLDecode(CCodeOperation::UnicodeToUTF_8(username));
	username = CCodeOperation::UTF_8ToUnicode(name);
	_userName = username;

	//写文件，以供下次自动登录
	if ( bSave )
		WriteFile(_userID, name, strcertif, _channelID, _nCpaUser);
	
	return username;

}

void LoginModel::ReSetData()
{ 
	_bLongin=false;
	_nCpaUser=0;
	_userID=L""; 
	_userName=L""; 
	_channelID="";

	InternetSetCookie(USERCENTERURL, L"ZJZS_Account", L"");
}

void LoginModel::ReadFile( wstring& strId, wstring& strName, wstring& strCertif, std::string& channelID, int& bCapUser )
{
	int length=0, nLen=0; 
	wstring iniPath = CONFIG(_strCachePath) + L"ulog";

	fstream fs(iniPath.c_str(),ios::in|ios::binary);

	char userid[16] = {'\0'};
	fs.read((char*)&nLen,sizeof(int));
	fs.read((char*)&length,sizeof(int));
	fs.read(userid, length);
	std::string strTemp(userid, length);
	wstring sId = CCodeOperation::UTF_8ToUnicode( DesDecrypt(strTemp, (unsigned char*)DES_KEY) );
	strId = sId.substr(0,nLen);
	
	char userName[64] = {'\0'};
	fs.read((char*)&nLen,sizeof(int));
	fs.read((char*)&length,sizeof(int));
	fs.read(userName, length);
	std::string strTemp1(userName, length);
	wstring sName = CCodeOperation::UTF_8ToUnicode( DesDecrypt(strTemp1, (unsigned char*)DES_KEY) );
	strName = sName.substr(0,nLen);

	char certif[256] = {'\0'};
	fs.read((char*)&nLen,sizeof(int));
	fs.read((char*)&length,sizeof(int));
	fs.read(certif, length);
	std::string strTemp2(certif, length);
	wstring sCertif = CCodeOperation::UTF_8ToUnicode( DesDecrypt(strTemp2, (unsigned char*)DES_KEY) );
	strCertif = sCertif.substr(0, nLen);

	char channel[128] = {'\0'};
	fs.read((char*)&nLen,sizeof(int));
	fs.read((char*)&length,sizeof(int));
	fs.read(channel, length);
	std::string strTemp3(channel, length);
	std::string sChannel = DesDecrypt(strTemp3, (unsigned char*)DES_KEY);
	channelID = sChannel.substr(0,nLen);

	fs.read((char*)&nLen,sizeof(int));
	bCapUser = nLen;

	fs.close();
}

void LoginModel::WriteFile( wstring strId, std::string strName, wstring strCertif, std::string channelID, int bCapUser  )
{
	int len=0;
	std::string strTemp;
	wstring iniPath = CONFIG(_strCachePath) + L"ulog";

	if ( !CFileOperation::IsDirExist( CONFIG(_strCachePath).c_str() ) )
		CFileOperation::MakeLocalDir(CONFIG(_strCachePath));

	fstream fs;
	fs.open(iniPath.c_str(),ios::out|ios::binary);
	if (!fs) return ;

	len = strId.length();
	fs.write((char *)&len,sizeof(int));

	strTemp = DesEncrypt( CCodeOperation::UnicodeToUTF_8(strId), (unsigned char*)DES_KEY );
	len = strTemp.length(); 
	fs.write((char *)&len,sizeof(int));
	fs.write(strTemp.c_str(),len);

	len = strName.length();
	fs.write((char *)&len,sizeof(int));

	strTemp = DesEncrypt( strName, (unsigned char*)DES_KEY );
	len = strTemp.length();
	fs.write((char *)&len,sizeof(int));
	fs.write(strTemp.c_str(),len);

	len = strCertif.length();
	fs.write((char *)&len,sizeof(int));

	strTemp = DesEncrypt( CCodeOperation::UnicodeToUTF_8(strCertif), (unsigned char*)DES_KEY );
	len = strTemp.length();
	fs.write((char *)&len,sizeof(int));
	fs.write(strTemp.c_str(),len);

	if ( channelID.empty() )
		channelID = "testid007";
	
	len = channelID.length();
	fs.write((char *)&len,sizeof(int));

	strTemp = DesEncrypt( channelID, (unsigned char*)DES_KEY );
	len = strTemp.length();
	fs.write((char *)&len,sizeof(int));
	fs.write(strTemp.c_str(),len);

	fs.write((char *)&bCapUser,sizeof(int));

	fs.close();
}