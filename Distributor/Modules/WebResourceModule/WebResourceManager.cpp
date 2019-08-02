#include "stdafx.h"
#include "WebResourceManager.h"
#include "rapidxml/rapidxml.hpp"
#include "Base/BaseCSLock.h"
#include "Base/ClientInfo.h"

using namespace rapidxml;

const wstring g_strUrlIos = L"http://pcfirm.sj.91.com/Controller.ashx?Action=getnewclientmenu&module=softandgame&type=xml&version=1&platform=iphone&clientVer=";
const wstring g_strUrlAndroid = L"http://pcfirm.sj.91.com/Controller.ashx?Action=getnewclientmenu&module=softandgame&type=xml&version=1&platform=android&clientVer=";
const wstring strSearchHostUrl = L"http://pcfirm.sj.91.com/search.aspx?keyword=";
const wstring strHomeHostUrl = L"http://pcfirm.sj.91.com/";
CWebResourceManager* CWebResourceManager::_pInstance = NULL;

CWebResourceManager::CWebResourceManager()
{
	InitNetModuleDataIds();
	InitNetNodeValues();
}

CWebResourceManager::~CWebResourceManager(void)
{
}

CWebResourceManager* CWebResourceManager::GetInstance()
{
	if(_pInstance==NULL)
		_pInstance=new CWebResourceManager();
	return _pInstance;
}

bool CWebResourceManager::Get(wstring strURL, NetEnumPhoneOS ePhoneOS, vector<NetModuleData>& stResult)
{
	try
	{   
		if (strURL.empty())
		{
			return false;
		}
		string  strData=CNetOperation::Get(CCodeOperation::WstringToString(strURL));
		wstring wsReturnData=CCodeOperation::UTF_8ToUnicode(strData);
		//NetResultData stTemp = this->ParseGetResult(wsReturnData);
		NetModuleData stTemp;
		if (!LoadNewNodeMenu(wsReturnData, stTemp))
		{
			return false;
		}
		stTemp.platform = ePhoneOS;
		if (stTemp.sites.size()>0)
		{
			stTemp.sites[0].platform = ePhoneOS;
		}
		stTemp.moduleId = L"application";
		stResult.push_back(stTemp);
	}
	catch(ND91Assistant::AssistantException e)
	{
		return false;
	}
	catch (...)
	{
		return false;
	}    

	return true;
}

bool CWebResourceManager::Get()
{
	AutoLock;
	if (_stDatas.size()<=0)
	{
		string clientVer = CClientInfo::GetInstance()->GetVersion();

		wstring strUrlIosTmp = g_strUrlIos + CCodeOperation::s2ws(clientVer);
		wstring strUrlAndroidTmp = g_strUrlAndroid + CCodeOperation::s2ws(clientVer);
		bool bResIos=Get(strUrlIosTmp, ePhoneOS_Ios_Net, _stDatas);
		bool bResAndroid=Get(strUrlAndroidTmp, ePhoneOS_Android_Net, _stDatas);

		return bResAndroid&&bResIos;
	}
	return true;
}

#pragma  region V5解析代码，暂时注释到时候还要用到

//NetResultData CWebResourceManager::ParseGetResult(wstring wsReturnData)
//{
//	string str=CCodeOperation::WstringToString(wsReturnData);
//	NetResultData ret;
//
//	cJSON* pRoot=cJSON_Parse(str.c_str());
//	if(pRoot)
//	{
//		cJSON* pChild=cJSON_GetObjectItem(pRoot,"version");
//		if(pChild&&pChild->type==cJSON_String)
//			ret.version=atoi(pChild->valuestring);
//
//		pChild=cJSON_GetObjectItem(pRoot,"menu");
//		if(pChild&&pChild->type==cJSON_Array)
//		{
//			int moduleSize=cJSON_GetArraySize(pChild);
//			for(int j=0;j<moduleSize;++j)
//			{
//				cJSON* pModule=cJSON_GetArrayItem(pChild,j);
//
//				NetEnumPhoneOS platform;
//				wstring strvalue;
//				cJSON* pModuleData=cJSON_GetObjectItem(pModule,"platform");
//				if(pModuleData&&pModuleData->type==cJSON_String)
//					strvalue=CCodeOperation::StringToWstring(pModuleData->valuestring);
//				if(strvalue.find(_T("android"))!=tstring::npos&&strvalue.find(_T("ios"))!=tstring::npos)
//					platform= ePhoneOS_None;
//				else if(strvalue.find(_T("android"))!=tstring::npos)
//					platform=ePhoneOS_Android;
//				else if(strvalue.find(_T("ios"))!=tstring::npos)
//					platform=ePhoneOS_Ios;
//
//				pModuleData=cJSON_GetObjectItem(pModule,"module");
//				if(pModuleData&&pModuleData->type==cJSON_Array)
//				{
//					int arraySize=cJSON_GetArraySize(pModuleData);
//					for(int i=0;i<arraySize;++i)
//					{
//						NetModuleData moduleData;
//
//						cJSON* pModuleNode=cJSON_GetArrayItem(pModuleData,i);
//						cJSON* pModule=cJSON_GetObjectItem(pModuleNode,"value");
//						if(pModule&&pModule->type==cJSON_String)
//							moduleData.moduleId=CCodeOperation::StringToWstring(pModule->valuestring);
//						moduleData.platform=platform;
//
//						vector<NetSite> netSites;
//						pModule=cJSON_GetObjectItem(pModuleNode,"menu");
//						if(pModule)
//						{
//							if(pModule->type==cJSON_Array)
//							{
//								int menuSize=cJSON_GetArraySize(pModule);
//								for (int k=0;k<menuSize;++k)
//								{
//									cJSON* pSites=cJSON_GetArrayItem(pModule,k);
//									NetSite netSite=ParseGetNetSite(pSites,platform);
//									netSites.push_back(netSite);
//								}
//							}
//							else if(pModule->type==cJSON_Object)
//							{
//								NetSite netSite=ParseGetNetSite(pModule,platform);
//								netSites.push_back(netSite);
//							}
//						}
//						moduleData.sites=netSites;
//						ret.moduledatas.push_back(moduleData);
//					}
//				}
//			}
//		}
//	}
//
//	return ret;
//}
//
//NetSite CWebResourceManager::ParseGetNetSite(cJSON* pSites,NetEnumPhoneOS platform)
//{
//	NetSite netSite;
//	netSite.isJailbreak=true; //默认是已越狱的
//	netSite.platform=platform;
//
//	cJSON* pSite=cJSON_GetObjectItem(pSites,"groupname");
//	if(pSite&&pSite->type==cJSON_String)
//		netSite.siteName=CCodeOperation::StringToWstring(pSite->valuestring);
//	pSite=cJSON_GetObjectItem(pSites,"groupvalue");
//	if(pSite&&pSite->type==cJSON_String)
//		netSite.siteId=CCodeOperation::StringToWstring(pSite->valuestring);
//
//	pSite=cJSON_GetObjectItem(pSites,"jailbreak");
//	if(pSite&&pSite->type==cJSON_String)
//		netSite.isJailbreak=(1==atoi(pSite->valuestring));
//
//	vector<NetNode> netNodes;
//
//	cJSON* pNodes=cJSON_GetObjectItem(pSites,"node");
//	if(pNodes)
//	{
//		if(pNodes->type==cJSON_Array)
//		{
//			int nodesSize=cJSON_GetArraySize(pNodes);
//			for (int j=0;j<nodesSize;++j)
//			{
//				cJSON* pNode=cJSON_GetArrayItem(pNodes,j);
//				NetNode netNode=ParseGetNetNode(pNode);
//				netNodes.push_back(netNode);
//			}
//		}
//		else if(pNodes->type==cJSON_Object)
//		{
//			NetNode netNode=ParseGetNetNode(pNodes);
//			netNodes.push_back(netNode);
//		}
//	}
//
//	netSite.nodes=netNodes;
//
//	return netSite;
//}
//
//NetNode CWebResourceManager::ParseGetNetNode(cJSON* pNode)
//{
//	NetNode netNode;
//	time_t timeNow;
//	time(&timeNow);
//
//	netNode.isshow=true;
//	netNode.isselect=false;
//	netNode.isPartner=true;
//
//	cJSON* pNodeItem=cJSON_GetObjectItem(pNode,"value");
//	if(pNodeItem&&pNodeItem->type==cJSON_String)
//		netNode.nodeId=CCodeOperation::StringToWstring(pNodeItem->valuestring);
//	pNodeItem=cJSON_GetObjectItem(pNode,"name");
//	if(pNodeItem&&pNodeItem->type==cJSON_String)
//		netNode.nodeName=CCodeOperation::StringToWstring(pNodeItem->valuestring);
//	pNodeItem=cJSON_GetObjectItem(pNode,"icon");
//	if(pNodeItem&&pNodeItem->type==cJSON_String)
//		netNode.nodeIcon=CCodeOperation::StringToWstring(pNodeItem->valuestring);
//	pNodeItem=cJSON_GetObjectItem(pNode,"url");
//	if(pNodeItem&&pNodeItem->type==cJSON_String)
//		netNode.nodeUrl=CCodeOperation::StringToWstring(pNodeItem->valuestring);
//	pNodeItem=cJSON_GetObjectItem(pNode,"searchurl");
//	if(pNodeItem&&pNodeItem->type==cJSON_String)
//		netNode.searchUrl=CCodeOperation::StringToWstring(pNodeItem->valuestring);
//	pNodeItem=cJSON_GetObjectItem(pNode,"isdefault");
//	if(pNodeItem&&pNodeItem->type==cJSON_String)
//		netNode.isselect=(1==atoi(pNodeItem->valuestring));
//	pNodeItem=cJSON_GetObjectItem(pNode,"ieid");
//	if(pNodeItem&&pNodeItem->type==cJSON_String)
//		netNode.ieid=CCodeOperation::StringToWstring(pNodeItem->valuestring);
//	pNodeItem=cJSON_GetObjectItem(pNode,"tagicon");
//	if(pNodeItem&&pNodeItem->type==cJSON_String)
//		netNode.tagicon=CCodeOperation::StringToWstring(pNodeItem->valuestring);
//	pNodeItem=cJSON_GetObjectItem(pNode,"ispartner");
//	if(pNodeItem&&pNodeItem->type==cJSON_String)
//		netNode.isPartner=(CStrOperation::CompareUnCase(CCodeOperation::StringToWstring(pNodeItem->valuestring),L"true")==0);
//	pNodeItem=cJSON_GetObjectItem(pNode,"starttime");
//	if(pNodeItem&&pNodeItem->type==cJSON_String)
//	{
//		time_t showtime=convert_string_to_time_t(pNodeItem->valuestring);
//		if(difftime(showtime,timeNow)<=0)
//			netNode.isshow=true;
//	}
//	pNodeItem=cJSON_GetObjectItem(pNode,"endtime");
//	if(pNodeItem&&pNodeItem->type==cJSON_String)
//	{
//		tstring strvalue=CCodeOperation::StringToWstring(pNodeItem->valuestring);
//		if(!strvalue.empty())
//		{
//			time_t hidetime=convert_string_to_time_t(pNodeItem->valuestring);
//			if(difftime(hidetime,timeNow)<=0)
//				netNode.isshow=false;
//		}
//	}
//
//	return netNode;
//}
//
//time_t CWebResourceManager::convert_string_to_time_t( const std::string & time_string )
//{
//	struct tm tm1;  
//	time_t time1;  
//	int i = sscanf(time_string.c_str(), "%d/%d/%d %d:%d:%d" ,       
//		&(tm1.tm_year),   
//		&(tm1.tm_mon),   
//		&(tm1.tm_mday),  
//		&(tm1.tm_hour),  
//		&(tm1.tm_min),  
//		&(tm1.tm_sec),  
//		&(tm1.tm_wday),  
//		&(tm1.tm_yday));  
//
//	tm1.tm_year -= 1900;  
//	tm1.tm_mon --;  
//	tm1.tm_isdst=-1;  
//	time1 = mktime(&tm1);  
//
//	return time1; 
//}#pragma region和#pragma endregion
#pragma  endregion


wstring CWebResourceManager::GetModuleDataId( NetEnumAppSort appSort )
{
	map<NetEnumAppSort,wstring>::iterator iter = this->_mapNetModuleDataIds.find(appSort);
	if (iter != this->_mapNetModuleDataIds.end())
	{
		return (*iter).second;
	}
	return L"";
}

void CWebResourceManager::InitNetModuleDataIds()
{
	_mapNetModuleDataIds[Etype_Application] = L"application";
}

vector<NetModuleData> CWebResourceManager::GetNetModuleDatas()
{
	return _stDatas;
}



/// <summary>
/// 加载Web节点，若有更新则读取后写入数据库，若无更新则无操作，前台直接读取数据库即可
bool CWebResourceManager::LoadNewNodeMenu(wstring moduleValue, NetModuleData& stData)
{
	xml_document<wchar_t> doc;
	NetSite stNetSite;

	doc.parse<0>((wchar_t*)moduleValue.c_str());
	rapidxml::xml_node<wchar_t>* root = doc.first_node();            //xml文件的根节点
	RETURN_FALSE_IF (!root);
	xml_attribute<wchar_t> *attribute = root->first_attribute();
	RETURN_FALSE_IF (!attribute);

	wstring str =attribute->name();
	if (L"Name"==str)
	{
		stNetSite.siteName = attribute->value();
	}

	for (rapidxml::xml_node<wchar_t>* node = root->first_node(); node!=NULL; node = node->next_sibling())
	{
		if ( !node )
			continue;
		NetNode stNetNode;
		for (xml_attribute<wchar_t> *attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute())
		{
			if ( !attribute )
				continue;
			wstring str=attribute->name();
			if (L"Name"==str)
			{
				stNetNode.nodeName = attribute->value();
			}
			else if (L"SearchUrl"==str)
			{
				stNetNode.searchUrl = attribute->value();
			} 
			else if (L"Value" == str)
			{
				stNetNode.strValue = attribute->value();;
			}
		}
		
		for (rapidxml::xml_node<wchar_t>* listnode = node->first_node(); listnode!=NULL; listnode = listnode->next_sibling())
		{
			if (!listnode)
			{
				continue;
			}
			stNetNode.initUrl = listnode->value();
			stNetNode.nodeUrl = listnode->value();
		}

		stNetSite.nodes.push_back(stNetNode);
	}

	stData.sites.push_back(stNetSite);
	return true;
}

void CWebResourceManager::ChangeNetNodeUrl( NetSite &site, ENUM_PLATFORM platform )
{
	for (vector<NetNode>::iterator iter = site.nodes.begin(); iter != site.nodes.end(); iter++)
	{
		if(PLATFORM_Android == platform)
		{
			iter->nodeUrl += L"?platform=Android";
			if (iter->searchUrl.empty())
			{
				continue;
			}
			iter->searchUrl += L"?platform=Android&key=";
		}
		else if(PLATFORM_Iphone == platform)
		{
			iter->nodeUrl += L"?platform=iphone&fw=iPhone";
			if (iter->searchUrl.empty())
			{
				continue;
			}
			iter->searchUrl += L"?platform=iphone&fw=iPhone&key=";
		}
		else
		{
			iter->nodeUrl += L"?platform=iphone&fw=iPad";
			if (iter->searchUrl.empty())
			{
				continue;
			}
			iter->searchUrl += L"?platform=iphone&fw=iPad&key=";
		}

	}
}

ENUM_PLATFORM CWebResourceManager::GetPlatformFromUrl( string url )
{
	ENUM_PLATFORM result = PLATFORM_Android;
	vector<string> vecOne;
	WebResourceCommonFunc::Split(url,'?',vecOne);
	vector<string> vecTwo;
	if (vecOne.size() > 1)
	{
		vector<string> vecTwo;
		WebResourceCommonFunc::Split(vecOne[1],'&',vecTwo);
		if (vecTwo.size() > 1)
		{
			vector<string> vecThree;
			WebResourceCommonFunc::Split(vecTwo[1],'=',vecThree);
			if (vecThree.size() > 1)
			{
				if ("Android" == vecThree[1])
				{
					result = PLATFORM_Android;
				}
				else if ("iPhone" == vecThree[1])
				{
					result = PLATFORM_Iphone;
				}
				else if ("iPad" == vecThree[1])
				{
					result = PLATFORM_Ipad;
				}
			}
		}
	}
	
	return result;
}

string CWebResourceManager::ClearSearchKey( string &url )
{
	string key = "&key=";
	string::size_type pos = url.find(key);
	string keyValue = "";
	if (string::npos != pos)
	{
		keyValue = url.substr(pos+key.size());
		url = url.erase(pos+key.size());
	}
	return keyValue;
}

tstring CWebResourceManager::GetNetNodeValue( NetNodeType type )
{
	map<NetNodeType,tstring> ::iterator iter = _mapNetNodeValues.find(type);
	if (iter != _mapNetNodeValues.end())
	{
		return iter->second;
	}
	return L"";
}

void CWebResourceManager::InitNetNodeValues()
{
	_mapNetNodeValues[ItunesApp2] = L"itunesapp2";//不越狱免费装
	_mapNetNodeValues[Index] = L"index";		  //装机必备
	_mapNetNodeValues[Search] = L"search";		  //搜索	
	_mapNetNodeValues[News] = L"news";			  //新闻资讯
	_mapNetNodeValues[Cpa] = L"cpa";              //Cpa
}
