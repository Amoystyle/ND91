#include "StdAfx.h"
#include "AndroidDeamonInstall.h"
#include "AssistantDll/Common/rapidxml/rapidxml.hpp"
#include "AssistantDll/Common/rapidxml/rapidxml_print.hpp"
#include "AssistantDll/Common/rapidxml/rapidxml_utils.hpp"


using namespace rapidxml;


//const wstring DeamonXmlUrl = L"http://sjupdate.sj.91.com/user.ashx?act=5&mt={0}&versionName={1}&RequestSource={2}&fwversion={3}";


CAndroidDeamonInstall::CAndroidDeamonInstall(wstring deviceId)
:CDeamonInstall(deviceId)
{
	
}

CAndroidDeamonInstall::~CAndroidDeamonInstall(void)
{
	
}


// 检测所下载的文件是否完整
bool CAndroidDeamonInstall::CheckAssistFilesPerfect(int nDeviceVer, bool isJailBreaked, bool isPad, bool isCpa)
{

	RETURN_FALSE_IF ( ! CheckAssistVerFromWeb(nDeviceVer, isCpa) );

	//以往版本中下载失败，只能解决一部分
	wstring strFilePath = isCpa ? _strAssOfficialAndPath : _strAssistantAndPath;
	if(CFileOperation::GetFileSizeW(strFilePath) < PandaMinSize)
		::DeleteFileW(strFilePath.c_str());

	RETURN_FALSE_IF ( ! CFileOperation::IsFileExist(strFilePath.c_str()) )
	
	wstring md5value1 = CCodeOperation::s2ws(CFileOperation::GetFileMD5(_strAssistantAndPath));//CCodeOperation::s2ws(CFileOperationEx::GetMD5(_strAssistantAndPath));//
	wstring md5value2 = GetMD5InConfig(isCpa);
	RETURN_FALSE_IF (md5value1 != md5value2);
	
	_bDownLoadFinish = true;
	return true;
}


/// <summary>
/// 本地文件与服务器上的最新的版本比较
/// </summary>
bool CAndroidDeamonInstall::CheckAssistVerFromWeb( int nDeviceVer, bool isCpa )
{
	try
	{
		// 向服务器请求（只会请求一次，除非重新启动助手）
		if ( _strServiceVer.empty() )
		{
			wstring strSoure = isCpa ? L"distributorassiatantsilent" : L"distributorassiatant";
			// “4”是平台类型数值,这个变量是作为一个url的参数（iPhone=1, andriod=4, ipad=7, androidpad=9）
			// versionName这个参数主要与服务器做对比，传递的值高于服务器版本值，codeValue返回9，或者为
			wstring xmlUrl = L"http://sjupdate.sj.91.com/user.ashx?act=5&mt=";
			xmlUrl += CStrOperation::IntToWString( 4 );
			xmlUrl += L"&versionName=1.0&RequestSource=" + strSoure + L"&fwversion=all";


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

		wstring LocalVersion = GetVersionInConfig(isCpa);
		RETURN_FALSE_IF ( LocalVersion.empty() );

		vector<wstring> LocalVersionList =  CStrOperation::parseStringsW(LocalVersion,L".");
		vector<wstring> serviceVersionList =  CStrOperation::parseStringsW(_strServiceVer,L".");
		return CompareVersion(LocalVersionList,serviceVersionList,0); 

	}
	catch(...)
	{
		return false;
	}
}

bool CAndroidDeamonInstall::DownloadAssistant(bool isJailBreaked, bool isPad, bool isCpa)
{
	try
	{
		wstring strFilePath = isCpa ? _strAssOfficialAndPath : _strAssistantAndPath;

		if (CFileOperation::IsFileExist(strFilePath.c_str()))
			::DeleteFileW(strFilePath.c_str());

		wstring downloadPath =  strFilePath + L".tmp~";
		::DeleteFileW(downloadPath.c_str());

		bool result = CNetOperation::DownloadFile(CCodeOperation::WstringToString(_strServiceUrl),downloadPath);
		if (result)
		{
			wstring strGroup = isCpa ? OFFICIALASSAT : ANDROIDASSAT;

			if (CFileOperation::IsFileExist(downloadPath.c_str()) && CFileOperation::GetFileSizeW(downloadPath) > PandaMinSize)
			{
				CFileOperation::MoveFileW(downloadPath, strFilePath);
				
				wstring fileMd5 = CCodeOperation::s2ws(CFileOperation::GetFileMD5(strFilePath));//CCodeOperation::s2ws(CFileOperationEx::GetMD5(_strAssistantAndPath));//CFileOperation::GetFileMD5(CCodeOperation::WstringToString(_strAssistantAndPath)));

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


// 获取数据库中存储的 手机助手（手机版） 文件版本值
wstring CAndroidDeamonInstall::GetVersionInConfig( bool isCpa )
{

	if (CFileOperation::IsFileExist(_strIniPath.c_str()))
	{
		wstring strGroup = isCpa ? OFFICIALASSAT : ANDROIDASSAT;
		return MyFileOperation::GetIniFileValue(_strIniPath, strGroup, L"Ver");
	}

	return L"";

}

// 获取配置文件 手机版助手 md5值
wstring CAndroidDeamonInstall::GetMD5InConfig( bool isCpa )
{

	if (CFileOperation::IsFileExist(_strIniPath.c_str()))
	{
		wstring strGroup = isCpa ? OFFICIALASSAT : ANDROIDASSAT;
		return MyFileOperation::GetIniFileValue(_strIniPath, strGroup, L"Hash");
	}

	return L"";
}


/// <summary>
/// 安装手机助手（手机版）
/// </summary>
DEAMON_INSTALLSTATE CAndroidDeamonInstall::InstallAssistant()
{

	bool bNeed = _bLogin && _bCpaUser;
	wstring strFilePath = bNeed ? _strAssOfficialAndPath : _strAssistantAndPath;

	RETURN_VALUE_IF ( !_pDevData, INSTALLSTATE_FAIL );
	RETURN_VALUE_IF ( !CFileOperation::IsFileExist(strFilePath.c_str()), INSTALLSTATE_FAIL );


	AppManager* appManager = _pDevData->GetAppManager();
	CommandExecuteResult result = COMMAND_EXECUTE_FAIL;
	try
	{
		// 统计功能
		//Statistic_StatFunc(Statistic_ID_InstallForAnd);

		wstring errmsg;
		AppData* app=NULL;
		result = appManager->InstallApp(strFilePath,errmsg,app);

		if ( result == COMMAND_EXECUTE_SUCCESS )
		{
			if ( bNeed )
				SetChannelIDNew();				
		}
	}
	catch (AssistantException e)
	{
		LOG->WriteDebug(L"======================(" + _pDevData->GetDevInfo()->_deviceIMEI + L")PandaSpace安装异常" + CStrOperation::IntToWString( e.GetErrorCode()));
	}

	return result == COMMAND_EXECUTE_SUCCESS ? INSTALLSTATE_COMPLETED : INSTALLSTATE_FAIL ;

}


// 实时检测助手“手机版”安装状态
bool CAndroidDeamonInstall::NeedAssistInstall()
{
	try
	{
		RETURN_FALSE_IF (!_pDevData);
		return _pDevData->GetAppManager()->IsInstallApp(ANDROIDASSATID);
	}
	catch (...)
	{
		return false;
	}
}

void CAndroidDeamonInstall::SetChannelID()
{
	RETURN_IF ( !_pDevData );
	wstring strDir		= _pDevData->GetDevInfo()->_sdcardpath + L"Android/data/com.dragon.android.pandaspace/";	
	wstring	phonePath	= strDir + L"NdChannelId.xml";

	RETURN_IF ( _strChannelID.empty() );

	wstring localPath = CONFIG(_strCachePath) + L"NdChannelId.xml";
	xml_document<char> doc;
	xml_node<>* root = doc.allocate_node(rapidxml::node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'")); 
	doc.append_node(root);

	xml_node<>* node = doc.allocate_node(node_element,"NdData",NULL);
	xml_node<>* dict = doc.allocate_node(node_element,"chl",_strChannelID.c_str());
	doc.append_node(node);
	node->append_node(dict);  

	std::string text;  
	rapidxml::print(std::back_inserter(text), doc, 0);
	RETURN_IF ( !CFileOperation::WriteBinFile(localPath, text.c_str(), text.length()) );

	FileHelper::Delete(_pDevData, phonePath);
	FileHelper::CreateDir(_pDevData, strDir);
	FileHelper::Upload(_pDevData, localPath, strDir,NULL);
	
}

void CAndroidDeamonInstall::SetChannelIDNew()
{
	RETURN_IF ( !_pDevData );
	wstring strDir		= _pDevData->GetDevInfo()->_sdcardpath + L"com.nd.channel.repository/";	
	wstring	phonePath	= strDir + ANDROIDASSATID;
	wstring localPath = CONFIG(_strCachePath) + ANDROIDASSATID;

	RETURN_IF ( _strChannelID.empty() );

	DirData root(_pDevData);
	DirData* pDir = dynamic_cast<DirData*>( FileHelper::GetRealNode(&root, strDir) );
	if ( pDir ) pDir->GetChildrens();
		
	std::string text;  
	xml_document<char> doc;
	if ( !pDir || !pDir->IsExist( ANDROIDASSATID) )
	{
		xml_node<>* root = doc.allocate_node(rapidxml::node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'")); 
		doc.append_node(root);

		xml_node<>* node = doc.allocate_node(node_element,"NdData",NULL);
		xml_node<>* package  = doc.allocate_node(node_element,"package",NULL);
		xml_node<>* pId = doc.allocate_node(node_element,"id", "com.dragon.android.pandaspace");
		xml_node<>* pCh = doc.allocate_node(node_element,"ch", _strChannelID.c_str());
		doc.append_node(node);
		node->append_node(package);
		package->append_node(pId);
		package->append_node(pCh);

		rapidxml::print(std::back_inserter(text), doc, 0);

	}
	else
	{
		if ( CFileOperation::IsFileExist(localPath.c_str()) )
			DeleteFile(localPath.c_str());

		vector<NodeData*> verNode;
		NodeData* pData = pDir->findChildrenbyName(ANDROIDASSATID);	
		verNode.push_back(pData);
		FileHelper::Download(&verNode, CONFIG(_strCachePath), NULL);

		RETURN_IF ( !CFileOperation::IsFileExist(localPath.c_str()) );

		bool bFind = false;
		std::basic_ifstream<char> stream(localPath.c_str(), ios::binary);
		file<char> fdoc(stream);
		doc.parse<0>(fdoc.data());
		RETURN_IF(fdoc.data() == NULL);

		rapidxml::xml_node<char> * root = doc.first_node();            //xml文件的根节点
		for (rapidxml::xml_node<char> * listnode = root->first_node(); listnode!=NULL; listnode = listnode->next_sibling())
		{
			rapidxml::xml_node<char> * fcnode = listnode->first_node();
			rapidxml::xml_node<char> * shnode = listnode->last_node();

			std::string strname = fcnode->name();
			std::string strVal	= fcnode->value();
			if ( "id" == strname && "com.dragon.android.pandaspace" == strVal)
			{
				bFind = true;
				listnode->remove_last_node();                                   //修改节点数据
				listnode->append_node(doc.allocate_node(node_element,"ch",_strChannelID.c_str()));                                       
				break;								
			}			
		}

		if ( !bFind )
		{
			xml_node<>* package  = doc.allocate_node(node_element,"package",NULL);
			xml_node<>* pId = doc.allocate_node(node_element,"id", "com.dragon.android.pandaspace");
			xml_node<>* pCh = doc.allocate_node(node_element,"ch", _strChannelID.c_str());
			root->append_node(package);
			package->append_node(pId);
			package->append_node(pCh);
		}

		rapidxml::print(std::back_inserter(text), doc, 0);
	}

	RETURN_IF ( !CFileOperation::WriteBinFile(localPath, text.c_str(), text.length()) );

	FileHelper::Delete(_pDevData, phonePath);
	FileHelper::CreateDir(_pDevData, strDir);			
	FileHelper::Upload(_pDevData, localPath, strDir,NULL);


}