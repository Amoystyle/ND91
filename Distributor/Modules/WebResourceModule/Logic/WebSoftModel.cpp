#include "stdafx.h"
#include "WebSoftModel.h"
#include "WebSoftHelp.h"
#include "Singleton.h"
#include "Manager/ThreadManager.h"


#include "WebSoftThread/GetHotAppsThread.h"
#include "WebSoftThread/GetSoftCategorysThread.h"
#include "WebSoftThread/GetAppDatasThread.h"
#include "WebSoftThread/GetGameDatasThread.h"
#include "WebSoftThread/GetSoftInfoThread.h"
#include "WebSoftThread/SearchAppsThread.h"
#include "WebSoftThread/GetAppIconThread.h"

WebSoftModel::WebSoftModel(PlatFrom platType)
{
	_platType	= platType;
	_platForm	= _platType == IPHONE ? "iphone" : "ipad";
	_total		= 0;	
	InitializeCriticalSection(&_slock);

}

WebSoftModel::~WebSoftModel()
{
	DeleteCriticalSection(&_slock);
}

bool WebSoftModel::Get24HoursHotAppList()
{
	char buf[128] = {0};
	sprintf( buf, "%s%s%s%s", Act1, ProjCode, _platForm.c_str(), _platForm.c_str() );
	std::string strTemp = buf;
	std::string checkCode = CStrOperation::toLower( strTemp ) + MD5Key;
	std::string strMd5	  = CStrOperation::GetMD5( checkCode );

	memset(buf,'\0', 128);
	sprintf( buf, "%s?act=%s&proj=%s&platform=%s&fw=%s&chkcode=", ITUNESAPPURL,
				Act1, ProjCode, _platForm.c_str(), _platForm.c_str() );
	std::string postData  = buf;

	postData  = CStrOperation::toLower( postData ) + strMd5;


	const string threadid = "GetHotAppsThread" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	GetHotAppsThread * appthread= new GetHotAppsThread(threadid);
	appthread->_url = CCodeOperation::UnicodeToUTF_8(CCodeOperation::GB2312ToUnicode(postData));
	appthread->_VerDatas = &_VerDatas;
	appthread->_platType = _platType;
	appthread->Run();


// 	BaseParam parm;
// 	parm.SetStr(CCodeOperation::GB2312ToUnicode(postData));
// 	HANDLE nHandle = StartSimpleActionThread(Action_GetHotAppsInfo, &parm);
	return true;
}

bool WebSoftModel::GetSoftCategoryList( bool isGame )
{
	std::string strGame = isGame ? "true" : "false";

	char buf[128] = {0};
	sprintf( buf, "%s%s%s%s%s", Act2, ProjCode, _platForm.c_str(), _platForm.c_str(), strGame.c_str() );
	std::string strTemp = buf;
	std::string checkCode = CStrOperation::toLower( strTemp ) + MD5Key;
	std::string strMd5	  = CStrOperation::GetMD5( checkCode );

	memset(buf,'\0', 128);
	sprintf( buf, "%s?act=%s&proj=%s&platform=%s&fw=%s&isgame=%s&chkcode=", ITUNESAPPURL,
				Act2, ProjCode, _platForm.c_str(), _platForm.c_str(), strGame.c_str() );
	std::string postData  = buf;

	postData  = CStrOperation::toLower( postData ) + strMd5;


	const string threadid = "GetHotAppsThread" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	GetSoftCategorysThread * appthread= new GetSoftCategorysThread(threadid);
	appthread->_url = CCodeOperation::UnicodeToUTF_8(CCodeOperation::GB2312ToUnicode(postData));
	appthread->_VerCats = &_VerCats;
	appthread->_platType = _platType;
	appthread->Run();


// 	BaseParam parm;
// 	parm.SetStr(CCodeOperation::GB2312ToUnicode(postData));
// 	HANDLE nHandle = StartSimpleActionThread(Action_GetSoftCategorysInfo, &parm);
	return true;
}

bool WebSoftModel::GetAppDataList( int categoryId, int count,int page, std::string sortType )
{
	if(categoryId==6014)
	{
		return GetGameDataList(categoryId,count,page,sortType);
	}
	char buf[256] = {0};
	sprintf( buf, "%s%s%s%s%d%d%d%s", Act3, ProjCode, _platForm.c_str(), _platForm.c_str(), categoryId, count, page, sortType.c_str() );

	std::string strTemp = buf;
	std::string checkCode = CStrOperation::toLower( strTemp ) + MD5Key;
	std::string strMd5	  = CStrOperation::GetMD5( checkCode );

	memset(buf,'\0', 256);
	sprintf( buf, "%s?act=%s&proj=%s&platform=%s&fw=%s&cid=%d&size=%d&page=%d&sort=%s&chkcode=", ITUNESAPPURL,
			Act3, ProjCode, _platForm.c_str(), _platForm.c_str(), categoryId, count, page, sortType.c_str() );
	std::string postData  = buf;

	postData  = CStrOperation::toLower( postData ) + strMd5;

	const string threadid = "GetAppDatasThread" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	GetAppDatasThread * appthread= new GetAppDatasThread(threadid);
	appthread->_url = CCodeOperation::UnicodeToUTF_8(CCodeOperation::GB2312ToUnicode(postData));
	appthread->_VerDatas = &_VerDatas;
	appthread->_platType = _platType;
	appthread->_total = &_total;
	appthread->Run();

// 	BaseParam parm;
// 	parm.SetStr(CCodeOperation::GB2312ToUnicode(postData));
// 	HANDLE nHandle = StartSimpleActionThread(Action_GetAppDatasInfo, &parm);

	return true;
}

bool WebSoftModel::GetGameDataList( int categoryId, int count, int page, std::string sortType )
{
	char buf[256] = {0};
	sprintf( buf, "%s%s%s%s%d%d%d%s", Act4, ProjCode, _platForm.c_str(), _platForm.c_str(), categoryId, count, page, sortType.c_str() );

	std::string strTemp = buf;
	std::string checkCode = CStrOperation::toLower( strTemp ) + MD5Key;
	std::string strMd5	  = CStrOperation::GetMD5( checkCode );

	memset(buf,'\0', 256);
	sprintf( buf, "%s?act=%s&proj=%s&platform=%s&fw=%s&cid=%d&size=%d&page=%d&sort=%s&chkcode=", ITUNESAPPURL, 
				Act4, ProjCode, _platForm.c_str(), _platForm.c_str(), categoryId, count, page, sortType.c_str() );
	std::string postData  = buf;

	postData  = CStrOperation::toLower( postData ) + strMd5;



	const string threadid = "GetGameDatasThread" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	GetGameDatasThread * appthread= new GetGameDatasThread(threadid);
	appthread->_url = CCodeOperation::UnicodeToUTF_8(CCodeOperation::GB2312ToUnicode(postData));
	appthread->_VerDatas = &_VerDatas;
	appthread->_platType = _platType;
	appthread->_total = &_total;
	appthread->Run();

// 	BaseParam parm;
// 	parm.SetStr(CCodeOperation::GB2312ToUnicode(postData));
// 	HANDLE nHandle = StartSimpleActionThread(Action_GetGameDatasInfo, &parm);

	return true;
}

bool WebSoftModel::GetOneAppInfo( int appId )
{
	char buf[128] = {0};
	sprintf( buf, "%s%s%s%s%d", Act5, ProjCode, _platForm.c_str(), _platForm.c_str(), appId );
	std::string strTemp = buf;
	std::string checkCode = CStrOperation::toLower( strTemp ) + MD5Key;
	std::string strMd5	  = CStrOperation::GetMD5( checkCode );

	memset(buf,'\0', 128);
	sprintf( buf, "%s?act=%s&proj=%s&platform=%s&fw=%s&id=%d&chkcode=", ITUNESAPPURL,
		Act5, ProjCode, _platForm.c_str(), _platForm.c_str(), appId );
	std::string postData  = buf;

	postData  = CStrOperation::toLower( postData ) + strMd5;

	const string threadid = "GetSoftInfoThread" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	GetSoftInfoThread * appthread= new GetSoftInfoThread(threadid);
	appthread->_url = CCodeOperation::UnicodeToUTF_8(CCodeOperation::GB2312ToUnicode(postData));
	appthread->_platType = _platType;
	appthread->Run();


// 	BaseParam parm;
// 	parm.SetStr(CCodeOperation::GB2312ToUnicode(postData));
// 	HANDLE nHandle = StartSimpleActionThread(Action_GetOneAppInfo, &parm);

	return true;
}

bool WebSoftModel::SearchAppList( wstring strKeyWord, int count,int page )
{
	char buf[512] = {0};
	std::string keyword = CNetOperation::URLEncode(CCodeOperation::UnicodeToUTF_8(strKeyWord));
	sprintf( buf, "%s%s%s%s%d%d", Act6, ProjCode, _platForm.c_str(), keyword.c_str(), count, page );

	std::string strTemp = buf;
	std::string checkCode = CStrOperation::toLower( strTemp ) + MD5Key;
	std::string strMd5	  = CStrOperation::GetMD5( checkCode );

	memset(buf,'\0', 512);
	sprintf( buf, "%s?act=%s&proj=%s&platform=%s&keyword=%s&size=%d&page=%d&chkcode=", 
			 SEARCHAPPURL, Act6, ProjCode, _platForm.c_str(), keyword.c_str(), count, page );
	std::string postData  = buf;

	postData  = CStrOperation::toLower( postData ) + strMd5;


	const string threadid = "GetSoftInfoThread" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	SearchAppsThread * appthread= new SearchAppsThread(threadid);
	appthread->_url = CCodeOperation::UnicodeToUTF_8(CCodeOperation::GB2312ToUnicode(postData));
	appthread->_platType = _platType;
	appthread->_VerDatas = &_VerDatas;
	appthread->_total = &_total;
	appthread->Run();


// 	BaseParam parm;
// 	parm.SetStr(CCodeOperation::GB2312ToUnicode(postData));
// 	HANDLE nHandle = StartSimpleActionThread(Action_SearchAppsInfo, &parm);
	return true;
}


bool WebSoftModel::GetAppsIconFromWeb(vector<int> fids, vector<wstring> urls)
{
	CFileOperation::MakeLocalDir(WebResourceModuleCONFIG(_strWebLegalSoft));
	CFileOperation::MakeLocalDir(WebResourceModuleCONFIG(_strWebLegalSoft) + L"WebAppIcons" );

	const string threadid = "GetAppIconThread" + CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	GetAppIconThread * appthread= new GetAppIconThread(threadid);
	appthread->_platType = _platType;

	for (size_t i=0;i<fids.size();i++)
	{
		appthread->_fids.push_back(fids[i]);
		appthread->_urls.push_back(urls[i]);
	}

	appthread->Run();

	return true;

}

bool WebSoftModel::GetAppDatas( vector<WebSoftData*>& verDatas, int& total )
{
	total = _total;
	RETURN_FALSE_IF (_VerDatas.empty());
	EnterCriticalSection(&_slock);
	verDatas.insert( verDatas.end(), _VerDatas.begin(), _VerDatas.end() );
	_VerDatas.clear();
	LeaveCriticalSection(&_slock);
	return true;
}

bool WebSoftModel::GetCategoryDatas( vector<WebSoftCategory*>& verCats )
{
	RETURN_FALSE_IF (_VerCats.empty());

	verCats.insert( verCats.end(), _VerCats.begin(), _VerCats.end() );
	_VerCats.clear();

	return true;
}

