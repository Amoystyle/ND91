#include "stdafx.h"
#include "AffirmLoginThread.h"
#include "../UserCenterInfoMsg.h"
#include "../LoginModel.h"
//#include "../WebResourceModule/Logic/WebResourceInnerMsg.h"
#include "Common/WebResource/WebResourceMessageDefine.h"
#include "Common/UserCenter/UserCenterMsg.h"


unsigned long AffirmLoginThread::Main()
{
	RETURN_VALUE_IF (_postData.empty(), -1);
	string recdata = CNetOperation::Post("http://zj.91.com/Login_New.aspx?c=1", _postData);
	RETURN_VALUE_IF (recdata.empty(), -1);

	DWORD len = 1024;
	WCHAR netData[1024] = {0};
	RETURN_VALUE_IF ( TRUE != InternetGetCookie( L"http://zj.91.com", L"ZJZS_Account", netData, &len), -1 );
	wstring temp = netData;

	int pos1 = temp.find(L"UserId=");
	int pos2 = temp.find(L"&UserName=");
	int pos3 = temp.find(L"&Certificate=");
	int pos4 = temp.find(L"&ChannelID=");
	int pos5 = temp.find(L"&HasCpa=");

	RETURN_VALUE_IF ( temp.empty() || pos1<1 || pos2<6 || pos3<9|| pos4<12 || pos5<15, -1 );

	wstring username, userID, strcertif;
	std::string channelID;

	userID		= temp.substr( pos1+7, pos2-pos1-7 );
	username	= temp.substr(pos2+10, pos3-pos2-10);
	strcertif	= temp.substr(pos3+13, pos4-pos3-13);
	channelID	= CNetOperation::URLDecode( CCodeOperation::UnicodeToUTF_8( temp.substr(pos4+11, pos5-pos4-11) ) );
	int nCpaUser = _wtoi( temp.substr(pos5+8).c_str() );

	std::string name = CNetOperation::URLDecode(CCodeOperation::UnicodeToUTF_8(username));
	username = CCodeOperation::UTF_8ToUnicode(name);

	LoginModel::GetInstance()->SetUserID(userID);
	LoginModel::GetInstance()->SetUserName(username);
	LoginModel::GetInstance()->SetBeLogin(true);
	LoginModel::GetInstance()->SetBeCpaUser(nCpaUser);


	ResponseUserLoginMsg useMsg;
	useMsg.userID	= userID;
	useMsg.nCpaUser = nCpaUser == 1;
	useMsg.bLongin	= true;
	useMsg.userName = username;
	useMsg.channelID = channelID;
	useMsg.SendToUI();


// 	WebSoftMsg msg;
// 	msg.message = WM_WebLegalSoft_AppDatasHasLoad;
// 	msg.platType = _platType;
// 	msg.SendToUI();

	return 0;
}

AffirmLoginThread::AffirmLoginThread( const string id )
:BaseThread(id)
{

}

AffirmLoginThread::~AffirmLoginThread()
{

}
