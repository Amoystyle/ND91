#include "stdafx.h"
#include "IOSDeamonInstall.h"
#include "AssistantDll/Common/rapidxml/rapidxml.hpp"
#include "AssistantDll/Common/rapidxml/rapidxml_print.hpp"
#include "Module/Common/IosPList/IosPlistConverter.h"

using namespace rapidxml;


// const wstring DeamonXmlUrl = L"http://sjupdate.sj.91.com/index.ashx?mt={0}&softid=999&fwversion={1}&version={2}&RequestSource={3}";
// const wstring PandaSpaceXmlUrl = L"http://sjupdate.sj.91.com/user.ashx?act=5&mt={0}&versionName={1}&RequestSource={2}&fwversion={3}&versionType={4}&Divide=0";

CIosDeamonInstall::CIosDeamonInstall(wstring deviceId)
:CDeamonInstall(deviceId)
{

}

CIosDeamonInstall::~CIosDeamonInstall(void)
{
}


// 检测服务器上“守护”的最新版本和URL
bool CIosDeamonInstall::CheckDeamonVerFromWeb( int nDeviceVer, bool isPad )
{
	
	try
	{
		if ( _strServiceVer.empty() )
		{
			wstring xmlUrl = L"http://sjupdate.sj.91.com/index.ashx?mt=";
			xmlUrl += CStrOperation::IntToWString( isPad ? 7 : 1 );
			xmlUrl += L"&softid=999&fwversion=";
			xmlUrl += CStrOperation::IntToWString(nDeviceVer);
			xmlUrl += L"&version=1.0&RequestSource=91assiatant";

			string dataUTF8 = CCodeOperation::UTF_8ToGB2312(CNetOperation::Get(CCodeOperation::WstringToString(xmlUrl),true));
			if ( (int)dataUTF8.find("result")<0 && (int)dataUTF8.find("resultState")<0 && (int)dataUTF8.find("code")<0 )
			{
				dataUTF8 = CCodeOperation::UTF_8ToGB2312(CNetOperation::Get(CCodeOperation::WstringToString(xmlUrl),true));

				if ( (int)dataUTF8.find("result")<0 && (int)dataUTF8.find("resultState")<0 && (int)dataUTF8.find("code")<0 )
				{
					dataUTF8 = CCodeOperation::UTF_8ToGB2312(CNetOperation::Get(CCodeOperation::WstringToString(xmlUrl),true));
				}
			}

			RETURN_FALSE_IF ( (int)dataUTF8.find("result")<0 && (int)dataUTF8.find("resultState")<0 && (int)dataUTF8.find("code")<0 );
			dataUTF8 = "<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n" + dataUTF8;

			xml_document<char> doc;
			doc.parse<0>((char*)dataUTF8.c_str());
			rapidxml::xml_node<char>* root = doc.first_node();            //xml文件的根节点
			if (root)
			{
				rapidxml::xml_node<char>* resultStateNode = root->first_node();
				string codeValue = resultStateNode->first_node()->value();
				if (codeValue == "0")
				{
					rapidxml::xml_node<char>* dataNode = resultStateNode->next_sibling();

					rapidxml::xml_node<char>* dataNode_version = dataNode->first_node("version");
					_strServiceVer = CCodeOperation::StringToWstring(dataNode_version->value());		// 网络上最新版本号
					rapidxml::xml_node<char>* dataNode_filelist = dataNode->first_node("filelist");

					rapidxml::xml_node<char>* dataNode_filelist_file = dataNode_filelist->first_node("file");
					rapidxml::xml_node<char>* dataNode_Url = dataNode_filelist_file->last_node();
					_strServiceUrl = CCodeOperation::StringToWstring(dataNode_Url->value());	// 获取网络的下载url
				}
			}
		}

		RETURN_FALSE_IF ( _strServiceVer.empty() );
		
		wstring LocalVersion = GetVersionInConfig(true,false,false);
		RETURN_FALSE_IF (LocalVersion.empty())

		vector<wstring> LocalVerList =  CStrOperation::parseStringsW(LocalVersion,L".");
		vector<wstring> serviceVerList =  CStrOperation::parseStringsW(_strServiceVer,L".");

		return CompareVersion(LocalVerList,serviceVerList,0);	
		

	}
	catch (...)
	{
		
	}

	return true;
}

// 检测本地的“守护”文件是否完整
bool CIosDeamonInstall::CheckDeamonFilesPerfect( int nDeviceVer, bool isPad )
{
		
	RETURN_FALSE_IF ( ! CheckDeamonVerFromWeb(nDeviceVer,isPad) ); 
	
	RETURN_FALSE_IF ( !CFileOperation::IsFileExist(_strDeamonPath.c_str()) )
	
	wstring hash1 = CCodeOperation::s2ws(CFileOperation::GetFileMD5(_strDeamonPath));//CFileOperationEx::GetMD5(_strDeamonPath)
	wstring hash2 = GetMD5InConfig(true,false,false);
	RETURN_FALSE_IF ( hash1 != hash2 );

	_bDownLoadFinish = true;
	return true;
	
}



//下载守护
bool CIosDeamonInstall::DownloadDeamon()
{
	try
	{		
		bool FilesPerfect = false;

		LOG->WriteDebug(L"======================(" + _strDeamonPath + L")只下载一次，开始下载");

		if (CFileOperation::IsFileExist(_strDeamonPath.c_str()))
			::DeleteFileW(_strDeamonPath.c_str());
		
		wstring downloadPath =  _strDeamonPath + L".tmp~";
		::DeleteFileW(_strDeamonPath.c_str());

		FilesPerfect = CNetOperation::DownloadFile(CCodeOperation::WstringToString(_strServiceUrl),downloadPath);
		
		if (FilesPerfect)
		{
			CFileOperation::MoveFileW(downloadPath,_strDeamonPath);
			wstring fileMD5 = CCodeOperation::s2ws(CFileOperation::GetFileMD5(_strDeamonPath));

			//WritePrivateProfileString(_strDeamonPath.c_str(),L"Path",mDeamonZipFilePath.c_str(),_strIniPath.c_str());
			WritePrivateProfileString(IOSDEAMON, L"Hash", fileMD5.c_str(), _strIniPath.c_str());
			WritePrivateProfileString(IOSDEAMON, L"Ver", _strServiceVer.c_str(), _strIniPath.c_str());
			
			_bDownLoadFinish = true;

		}
		else
		{
			//　守护下载失败
		}

		LOG->WriteDebug(L"======================(" + _strDeamonPath + L")只下载一次，结束下载，下载" + (FilesPerfect ? L"true":L"false"));

		return FilesPerfect;
	
	}
	catch (...)
	{
		return false;
	}
}

// 检测本地“助手手机版”安装文件完整性
bool CIosDeamonInstall::CheckAssistFilesPerfect( int nDeviceVer, bool isJailBreaked, bool isPad, bool isCpa )
{

	RETURN_FALSE_IF ( ! CheckAssistantVerFromWeb( nDeviceVer, isJailBreaked, isPad) );
	
	//以往版本中下载失败，只能解决一部分
	wstring filePath;

	if (!isJailBreaked)
	{
		filePath = _strLegalAssistPath;
	}
	else
	{
		filePath = isPad ? _strIpadAssistantPath : _strIphoneAssistPath;
	}

	if(CFileOperation::GetFileSizeW(filePath) < PandaMinSize)
		::DeleteFileW(filePath.c_str());

	RETURN_FALSE_IF ( ! CFileOperation::IsFileExist(filePath.c_str()) )
	
	wstring md5value1 = CCodeOperation::s2ws(CFileOperation::GetFileMD5(filePath));//CFileOperation::GetFileMD5(CCodeOperation::WstringToString(filePath)));
	wstring md5value2 = GetMD5InConfig(false, isJailBreaked, isPad);
	RETURN_FALSE_IF ( md5value1 != md5value2 );
	
	_bDownLoadFinish = true;
	return true;
}


// 是否需要下载服务器的最新版本
bool CIosDeamonInstall::CheckAssistantVerFromWeb( int nDeviceVer, bool isJailBreaked, bool isPad )
{
	try
	{

		//L"http://sjupdate.sj.91.com/user.ashx?act=5&mt={0}&versionName={1}&RequestSource={2}&fwversion={3}&versionType={4}&Divide=0";
		// 平台类型数值,这个变量是作为一个url的参数（iPhone=1, andriod=4, ipad=7, androidpad=9）

		if ( _strServiceVer.empty() )
		{
			wstring xmlUrl = L"http://sjupdate.sj.91.com/user.ashx?";
			if ( isJailBreaked )
			{
				xmlUrl += L"act=5&mt=" + CStrOperation::IntToWString( isPad ? 7:1 );
				xmlUrl += L"&versionName=1.0&RequestSource=distributorassiatant&fwversion=";
				xmlUrl += CStrOperation::IntToWString( nDeviceVer );
				xmlUrl += L"&versionType=Ipa&Divide=0";
			}
			else
			{
				wstring strVer = !CFileOperation::IsFileExist(_strIniPath.c_str()) ? L"0.0" : MyFileOperation::GetIniFileValue(_strIniPath, IOSLEGALASSAT, L"Ver") ;

				xmlUrl += L"mt=1&versionName=0.0";
				xmlUrl += L"&fwversion=" + CStrOperation::IntToWString( nDeviceVer );
				xmlUrl += L".x&versionType=ipa&format=xml&branch=16&softid=2";
			}


			string dataUTF8 = CCodeOperation::UTF_8ToGB2312(CNetOperation::Get(CCodeOperation::WstringToString(xmlUrl),true));
			if ( (int)dataUTF8.find("result")<0 && (int)dataUTF8.find("resultState")<0 && (int)dataUTF8.find("code")<0 )
			{
				dataUTF8 = CCodeOperation::UTF_8ToGB2312(CNetOperation::Get(CCodeOperation::WstringToString(xmlUrl),true));

				if ( (int)dataUTF8.find("result")<0 && (int)dataUTF8.find("resultState")<0 && (int)dataUTF8.find("code")<0 )
				{
					dataUTF8 = CCodeOperation::UTF_8ToGB2312(CNetOperation::Get(CCodeOperation::WstringToString(xmlUrl),true));
				}
			}

			RETURN_FALSE_IF ( (int)dataUTF8.find("result")<0 && (int)dataUTF8.find("resultState")<0 && (int)dataUTF8.find("code")<0 );
			dataUTF8 = "<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n" + dataUTF8;

			xml_document<char> doc;
			doc.parse<0>((char*)dataUTF8.c_str());
			rapidxml::xml_node<char>* root = doc.first_node();            //xml文件的根节点
			if (root)
			{
				rapidxml::xml_node<char>* resultStateNode = root->first_node();
				string codeValue = resultStateNode->first_node()->value();
				if (codeValue == "0")
				{
					rapidxml::xml_node<char>* dataNode = resultStateNode->next_sibling();

					rapidxml::xml_node<char>* dataNode_version = dataNode->first_node("version");
					_strServiceVer = CCodeOperation::StringToWstring(dataNode_version->value());         // 网络上最新版本号
					rapidxml::xml_node<char>* dataNode_filelist = dataNode->first_node("filelist");

					rapidxml::xml_node<char>* dataNode_filelist_file = dataNode_filelist->first_node("file");
					rapidxml::xml_node<char>* dataNode_Url = dataNode_filelist_file->last_node();
					_strServiceUrl = CCodeOperation::StringToWstring(dataNode_Url->value());	// 获取网络的下载url
				}
			}
		}

		RETURN_FALSE_IF ( _strServiceVer.empty() );
		
		wstring LocalVersion = GetVersionInConfig(false, isJailBreaked, isPad);
		RETURN_FALSE_IF ( LocalVersion.empty() );

		vector<wstring> LocalList =  CStrOperation::parseStringsW(LocalVersion,L".");
		vector<wstring> serviceList =  CStrOperation::parseStringsW(_strServiceVer,L".");
		return CompareVersion(LocalList,serviceList,0); 
		
		
	}
	catch(AssistantException e)
	{
		return false;
	}
}



// 下载手机助手（手机版）
bool CIosDeamonInstall::DownloadAssistant(bool isJailBreaked, bool isPad, bool isCpa)
{

	wstring strSavePath, strGroup;
	if ( !isJailBreaked )
	{
		strSavePath = _strLegalAssistPath;
		strGroup	= IOSLEGALASSAT;
	}
	else
	{
		strSavePath = isPad ? _strIpadAssistantPath : _strIphoneAssistPath;
		strGroup	= isPad ? IPADASSAT : IPHONEASSAT;
	}


	try
	{
		if (CFileOperation::IsFileExist(strSavePath.c_str()))
			::DeleteFileW(strSavePath.c_str());
		
		wstring downloadPath =  strSavePath + L".tmp~";
		::DeleteFileW(downloadPath.c_str());

		bool result = CNetOperation::DownloadFile(CCodeOperation::WstringToString(_strServiceUrl), downloadPath);
		if (result)
		{
			if (CFileOperation::IsFileExist(downloadPath.c_str()) && CFileOperation::GetFileSizeW(downloadPath) > PandaMinSize)
			{
				CFileOperation::MoveFileW(downloadPath, strSavePath);
				wstring fileMd5 = CCodeOperation::s2ws(CFileOperation::GetFileMD5(strSavePath));//CFileOperation::GetFileMD5(CCodeOperation::WstringToString(strSavePath)));

				WritePrivateProfileString(strGroup.c_str(), L"Hash", fileMd5.c_str(),		_strIniPath.c_str());
				WritePrivateProfileString(strGroup.c_str(), L"Ver",  _strServiceVer.c_str(), _strIniPath.c_str());
				
				_bDownLoadFinish = true;
			}
			else
				result = false;
		}

		return result;
	}
	catch (...)
	{
		return false;
	}
}


// 获取配置文件中“守护”或“助手手机版”文件的版本值
wstring CIosDeamonInstall::GetVersionInConfig(bool Deamon, bool isJailBreaked, bool isPad)
{

	if (CFileOperation::IsFileExist(_strIniPath.c_str()))
	{
		if (Deamon)
		{
			return MyFileOperation::GetIniFileValue(_strIniPath, IOSDEAMON, L"Ver");
		}
		else
		{
			wstring strKey;

			if ( !isJailBreaked )
				strKey = IOSLEGALASSAT;
			else
				strKey = isPad ? IPADASSAT : IPHONEASSAT;
			
			

			return MyFileOperation::GetIniFileValue(_strIniPath, strKey, L"Ver");
		}
	}
	return L"";
}


// 获取配置文件中“守护”或“助手手机版”文件Hash的加密MD5值
wstring CIosDeamonInstall::GetMD5InConfig(bool Deamon, bool isJailBreaked, bool isPad)
{

	if (CFileOperation::IsFileExist(_strIniPath.c_str()))
	{
		if (Deamon)
		{
			return MyFileOperation::GetIniFileValue(_strIniPath, IOSDEAMON, L"Hash");
		}
		else
		{
			wstring strKey = !isJailBreaked ? IOSLEGALASSAT : isPad ? IPADASSAT : IPHONEASSAT;

			return MyFileOperation::GetIniFileValue(_strIniPath, strKey, L"Hash");
		}
	}
	return L"";
}

// 检测守护版本及更新
bool CIosDeamonInstall::NeedDeamonInstall()
{
		
	RETURN_FALSE_IF ( !_pDevData );
	RETURN_TRUE_IF ( !_pDevData->GetDevInfo()->_ios_bJailBreaked );
	RETURN_FALSE_IF ( !_pDevData->GetDevInfo()->_ios_daemonInstalled );

	wstring curPhoneVer = _pDevData->GetDevInfo()->_daemonVer;
	
	// “IsDaemonInstalled”只要守护与助手（手机版）中的任一个没安装便是false
	RETURN_FALSE_IF ( curPhoneVer.empty() );

	wstring xmlUrl = L"http://sjupdate.sj.91.com/index.ashx?mt=";
	xmlUrl += CStrOperation::IntToWString( _pDevData->GetDevInfo()->IsPad() ? 7 : 1 );
	xmlUrl += L"&softid=999&fwversion=";
	xmlUrl += CStrOperation::IntToWString(5);
	xmlUrl += L"&version=1.0&RequestSource=91assiatant";

	string dataUTF8 = CCodeOperation::UTF_8ToGB2312(CNetOperation::Get(CCodeOperation::WstringToString(xmlUrl),true));
	if ( (int)dataUTF8.find("result")<0 || (int)dataUTF8.find("resultState")<0 || (int)dataUTF8.find("code")<0 )
	{
		dataUTF8 = CCodeOperation::UTF_8ToGB2312(CNetOperation::Get(CCodeOperation::WstringToString(xmlUrl),true));

		if ( (int)dataUTF8.find("result")<0 || (int)dataUTF8.find("resultState")<0 || (int)dataUTF8.find("code")<0 )
		{
			dataUTF8 = CCodeOperation::UTF_8ToGB2312(CNetOperation::Get(CCodeOperation::WstringToString(xmlUrl),true));
		}
	}

	RETURN_FALSE_IF ( (int)dataUTF8.find("result")<0 || (int)dataUTF8.find("resultState")<0 || (int)dataUTF8.find("code")<0 );
	dataUTF8 = "<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n" + dataUTF8;

	xml_document<char> doc;
	doc.parse<0>((char*)dataUTF8.c_str());
	rapidxml::xml_node<char>* root = doc.first_node();            //xml文件的根节点
	if (root)
	{
		rapidxml::xml_node<char>* resultStateNode = root->first_node();
		string codeValue = resultStateNode->first_node()->value();
		if (codeValue == "0")
		{
			rapidxml::xml_node<char>* dataNode = resultStateNode->next_sibling();

			rapidxml::xml_node<char>* dataNode_version = dataNode->first_node("version");
			_strServiceVer = CCodeOperation::StringToWstring(dataNode_version->value());         // 网络上最新版本号
			rapidxml::xml_node<char>* dataNode_filelist = dataNode->first_node("filelist");
		}
	}
	RETURN_FALSE_IF ( _strServiceVer.empty() );

	vector<wstring> curVer =  CStrOperation::parseStringsW(curPhoneVer,L".");
	vector<wstring> newVer =  CStrOperation::parseStringsW(_strServiceVer,L".");

	return( CompareVersion(curVer,newVer,0) );
				
}


// 安装或更新助手
DEAMON_INSTALLSTATE CIosDeamonInstall::InstallDeamon()
{
	
	RETURN_VALUE_IF ( !_pDevData, INSTALLSTATE_FAIL );
	
	if (!_pDevData->GetDevInfo()->_ios_bJailBreaked)
	{
		return INSTALLSTATE_COMPLETED;
	}

	RETURN_VALUE_IF ( !CFileOperation::IsFileExist(_strDeamonPath.c_str()), INSTALLSTATE_FAIL );


	LOG->WriteDebug(L"======================(" + _pDevData->GetDevInfo()->_deviceIMEI + L")(" + _strDeamonPath + L")只安装一次，开始安装");

	AutoLock;

	if ( _strServiceVer.empty() )
		_strServiceVer = MyFileOperation::GetIniFileValue(_strIniPath, IOSDEAMON, L"Ver");
	
	AppManager* appManager = _pDevData->GetAppManager();
	CommandExecuteResult result = appManager->InstallDaemon(_strDeamonPath,_strServiceVer);
	return result == COMMAND_EXECUTE_SUCCESS ? INSTALLSTATE_COMPLETED : INSTALLSTATE_FAIL ;

	LOG->WriteDebug(L"======================(" + _pDevData->GetDevInfo()->_deviceIMEI + L")(" + _strDeamonPath + L")只安装一次，结束安装，安装" + (_InstallState ? L"true" : L"false"));

}



/// <summary>
/// 安装手机助手（手机版）
/// </summary>
DEAMON_INSTALLSTATE CIosDeamonInstall::InstallAssistant()
{

	RETURN_VALUE_IF ( !_pDevData, INSTALLSTATE_FAIL );
	bool isPad = _pDevData->GetDevInfo()->IsPad();

	LOG->WriteDebug(L"======================(" + _pDevData->GetDevInfo()->_deviceIMEI + L")PandaSpace只安装一次，开始安装");

	wstring localPath = !_pDevData->GetDevInfo()->_ios_bJailBreaked ? _strLegalAssistPath : isPad ? _strIpadAssistantPath : _strIphoneAssistPath;
	RETURN_VALUE_IF ( !CFileOperation::IsFileExist(localPath.c_str()), INSTALLSTATE_FAIL );

	AppManager* pManager = _pDevData->GetAppManager();
	RETURN_VALUE_IF (!pManager, INSTALLSTATE_FAIL );
	CommandExecuteResult result = COMMAND_EXECUTE_FAIL;
	try
	{
		wstring errmsg;
		AppData* app=NULL;
		result = pManager->InstallApp(localPath,errmsg,app);

		// 统计功能

		if ( result == COMMAND_EXECUTE_SUCCESS )
		{
			if ( _bLogin && _bCpaUser)			
				SetChannelID();			

			if ( _pDevData->GetDevInfo()->_ios_bJailBreaked )
			{
				CStaticsticsMessage msg(Statistic_ID_InstallForLegal);
				msg.Send();
			}
			else
			{
				if(isPad)
				{
					CStaticsticsMessage msg(Statistic_ID_InstallForIpad);
					msg.Send();
				}
				else
				{
					CStaticsticsMessage msg(Statistic_ID_InstallForIos);
					msg.Send();
				}
			}
		}


	}
	catch (AssistantException e)
	{
		LOG->WriteDebug(L"======================(" + _pDevData->GetDevInfo()->_deviceIMEI + L")PandaSpace安装异常" + CStrOperation::IntToWString( e.GetErrorCode()));
	}
	return result == COMMAND_EXECUTE_SUCCESS ? INSTALLSTATE_COMPLETED : INSTALLSTATE_FAIL;
	LOG->WriteDebug(L"======================(" + _pDevData->GetDevInfo()->_deviceIMEI + L")PandaSpace只安装一次，结束安装，安装" + (result == COMMAND_EXECUTE_SUCCESS ? L"true" : L"false"));

}

bool CIosDeamonInstall::NeedAssistInstall()
{
	RETURN_FALSE_IF (!_pDevData);

	wstring pandaMenu, pandaAppInfo;
	wstring version, strAssistID;

	if ( !_pDevData->GetDevInfo()->_ios_bJailBreaked )
	{
		strAssistID = LEGALASSATID;	
	}
	else
	{
		if (_pDevData->GetDevInfo()->IsPad())
		{
			pandaMenu = L"/Applications/PandaSpaceHD.app/";
			pandaAppInfo = L"/Applications/PandaSpaceHD.app/Info.plist";
			strAssistID = IPADASSATID;	
		}
		else
		{
			pandaMenu = L"/Applications/PandaSpace.app/";
			pandaAppInfo = L"/Applications/PandaSpace.app/Info.plist";
			strAssistID = IOSASSATID;	
		}
	}


    try
    {
		if ( _pDevData->GetDevInfo()->_ios_bJailBreaked )
		{
			DirData root(_pDevData);
			NodeData* pNode = FileHelper::GetNode(&root, pandaAppInfo,false);
			RETURN_TRUE_IF (pNode);
		}


        // 再次判断是否安装了IPA版本
        vector<AppData*> pDatas; 
	    _pDevData->GetAppManager()->GetDatas(pDatas);
        if(pDatas.empty())
        {
            _pDevData->GetAppManager()->Load();
            _pDevData->GetAppManager()->GetDatas(pDatas);
        }
        for (size_t i = 0; i < pDatas.size(); i++)
        {
            AppData* pApp = pDatas.at(i);
            wstring temp = CStrOperation::toLowerW(pApp->_packageName);
			RETURN_TRUE_IF (temp.find(strAssistID) < temp.length());
			      
        }

        return false;
    }
    catch (...)
    {
        return false;
    }
}

void CIosDeamonInstall::SetChannelID()
{
	RETURN_IF ( !_pDevData );
	RETURN_IF ( !_pDevData->GetDevInfo()->_ios_bJailBreaked );

	RETURN_IF ( _strChannelID.empty() );

	wstring appid	= _pDevData->GetDevInfo()->IsPad() ? IPADASSATID : IOSASSATID;
	wstring name	= _pDevData->GetDevInfo()->IsPad() ? L"/PandaSpaceHD.app/" : L"/PandaSpace.app/";
	
	wstring strDir		= GetAssistPath(appid) + name;	
	wstring	phonePath	= strDir + L"NdChannelId.plist";

	wstring localPath = CONFIG(_strCachePath) + L"NdChannelId.plist";
	xml_document<char> doc;

	xml_node<>* dict = doc.allocate_node(node_element,"dict",NULL);
	xml_node<>* node1 = doc.allocate_node(node_element,"key","chl");
	xml_node<>* node2 = doc.allocate_node(node_element,"string",_strChannelID.c_str());
	doc.append_node(dict);
	dict->append_node(node1);
	dict->append_node(node2);

	std::string text;  
	rapidxml::print(std::back_inserter(text), doc, 0);
	text = XML_PLIST_Base + text + "</plist>";
	RETURN_IF ( !CFileOperation::WriteBinFile(localPath, text.c_str(), text.length()) );

	IosPlistConverter::GetPlistFromXml(phonePath,phonePath);

	FileHelper::Delete(_pDevData, phonePath);
	FileHelper::CreateDir(_pDevData, strDir);
	FileHelper::Upload(_pDevData, localPath, strDir,NULL);
}

std::wstring CIosDeamonInstall::GetAssistPath( wstring appID )
{
	vector<AppData*> verData;
	_pDevData->GetAppManager()->Load(); 
	_pDevData->GetAppManager()->GetDatas(verData);

	for (int i=0;i<(int)verData.size();i++)
	{
		AppData* pdata = verData[i];
		if ( (int)CStrOperation::toLowerW(pdata->_packageName).find( appID ) >= 0 )
		{
			return pdata->_strAppPhonePath;
		}
	}
	return L"";
}