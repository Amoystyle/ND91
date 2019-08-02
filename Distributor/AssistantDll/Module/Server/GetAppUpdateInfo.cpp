#include "stdafx.h"
#include "Module/Server/GetAppUpdateInfo.h"

#include "Core/DeviceData.h"
#include "Module/Server/GetWebInfoBase.h"
#include "Common/NetOperation.h"
#include "Common/rapidxml/rapidxml.hpp"
#include "Common/cJSON.h"
using namespace rapidxml;

CGetAppUpdateInfo::CGetAppUpdateInfo( int deviceType)
{
	_pDevice	= NULL;
	_deviceType = deviceType;
}

CGetAppUpdateInfo::CGetAppUpdateInfo( DeviceData* pDevice )
{
	_pDevice	= pDevice;
	_deviceType = Ios;
}

CGetAppUpdateInfo::~CGetAppUpdateInfo(void)
{
}

bool CGetAppUpdateInfo::GetAppUpdateInfo( vector<AppUpdateInfo*>* pinfo )
{
	wstring appInfos, platform;
	
	if ( !_pDevice )
		platform = _deviceType == 1 ? L"iphone" : L"android";
	else
		platform = _pDevice->GetDevInfo()->_deviceType == Ios ? L"iphone" : L"android"; 

	wstring url = L"http://pcx.sj.91.com/soft/all/controller.ashx?net=mini&app=soft&controller=" + platform + L"&action=getsoftlist&method=post";

	RETURN_FALSE_IF ( !pinfo || pinfo->empty() );

	// 组合URL参数
	for ( int i=0; i<(int)pinfo->size();i++ )
	{
		AppUpdateInfo* info = pinfo->at(i);
		WCHAR szName[16] = {0};
		swprintf(szName, L"&softinfo[%d]", i+1);	
		wstring strName = szName;
		int vcode;
		memcpy(&vcode,info->_verCode.c_str(),4);
		wstring appinfo = strName + L"[name]=" + info->_name + strName + L"[identifier]=" + info->_identifier + strName + L"[version]=" + 
			info->_oldVersion + strName + L"[versionCode]=" + CStrOperation::IntToWString(vcode) + strName + L"[shortVer]=";

		appInfos += appinfo;
	}

	string postData; 
	string lang = "zh-CN";
	string netInfo = CCodeOperation::UnicodeToUTF_8(appInfos);

	if ( !_pDevice )
	{
		CGetWebInfoBase getInfo;
		postData = getInfo.GetSendInfo(_deviceType) + "&updateType=localLib" + netInfo + "&lang=" + lang + "&cv=";	
	}
	else
	{
		CGetWebInfoBase gBase(_pDevice);
		postData = gBase.GetPhoneInfo() + netInfo + "&lang=" + lang + "&cv=";
	}
	
	// 调用Post加密体后获取数据

	wstring data = L"PHONE_INFO=PhoneSystem=&PhoneSystemVersion=&PhoneType=" + platform + L"&Resolution=320x480&Source=" 
						+ platform + L"pcsuite&SourceVersion=3.0.0.1&UniqueID=000000000000000&CV=&FwVersion=iPhone; path=/";
	// 每次post之前先设置正确的Cookies,以保证从服务器获取数据正确 (暂时先这样做)
	BOOL bReturn = InternetSetCookie(L"http://pcx.sj.91.com", NULL, data.c_str());

	string imginfo	= CNetOperation::Post(CCodeOperation::UnicodeToUTF_8(url), postData);

	RETURN_FALSE_IF (imginfo.empty() || "0" == imginfo);

	// 解析数据
	return ParseInfo(imginfo, pinfo);
}

bool CGetAppUpdateInfo::ParseInfo( string imginfo, vector<AppUpdateInfo*>* pinfo )
{
	xml_document<char> doc;
	doc.parse<0>((char*)imginfo.c_str());

	rapidxml::xml_node<char>* root = doc.first_node();            //xml文件的根节点
	if (!root)
		return false;
	for (rapidxml::xml_node<char>* node = root->first_node(); node!=NULL; node = node->next_sibling())
	{
		AppUpdateInfo* info = new AppUpdateInfo;
		string strname	= node->name();
		if ( "SoftInfo" == strname )
		{
			for (rapidxml::xml_node<char>* listnode = node->first_node(); listnode!=NULL; listnode = listnode->next_sibling())
			{
				if (!listnode)
					continue;
				strname			= listnode->name();
				string strVal	= listnode->value();

				if ( "f_id" == strname )
				{
					info->_nfid = atoi(strVal.c_str());
				}
				else if ( "Name" == strname )
				{
					info->_name = CCodeOperation::UTF_8ToUnicode(strVal);
				}
				else if ( "Size" == strname )
				{
					info->_nsize = atoi(strVal.c_str());
				}
				else if ( "Identifier" == strname )
				{
					info->_identifier = CCodeOperation::UTF_8ToUnicode(strVal);
				}
				else if ( "AddTime" == strname )
				{
					info->_addTime = CDealTime::StringToTime(CCodeOperation::UTF_8ToUnicode(strVal));
				}
				else if ( "OldVersion" == strname )
				{
					info->_oldVersion = CCodeOperation::UTF_8ToUnicode(strVal);
				}
				else if ( "NewVersion" == strname )
				{
					info->_newVersion = CCodeOperation::UTF_8ToUnicode(strVal);
				}
				else if ( "VersionCode" == strname )
				{
					info->_verCode = CCodeOperation::UTF_8ToUnicode(strVal);
				}
				else if ( "Update" == strname )
				{
					info->_bUpdate = atoi(strVal.c_str()) == 1 ? true : false;
				}
				else if ( "DownNum" == strname )
				{
					info->_nDownNum = atoi(strVal.c_str());
				}
				else if ( "URL" == strname )
				{
					rapidxml::xml_node<char>* valuenode = listnode->first_node();
					strVal = valuenode->value();
					info->_url = CCodeOperation::UTF_8ToUnicode(strVal);
				}
				else
					continue;
			}

			for ( int i=0;i<(int)pinfo->size();i++ )
			{
				if ( info->_identifier == pinfo->at(i)->_identifier )
				{
					AppUpdateInfo* appData = pinfo->at(i);
					SAFE_DELETE(appData);

					pinfo->at(i) = info;	
					break;
				}
			}
		}
	}
	return true;
}

bool CGetAppUpdateInfo::GetAppUpdateData( vector<AppUpdateInfo*>* pinfo )
{
	wstring appInfos, platform;

	if ( !_pDevice )
		platform = _deviceType == 1 ? L"1" : L"4";
	else
		platform = _pDevice->GetDevInfo()->_deviceType == Ios ? L"iphone" : L"android";; 

	wstring url = L"http://appupdate.sj.91.com/service.ashx?act=405&proj=2900&platform=" + platform + L"&fw=" 
					+ _pDevice->GetDevInfo()->_deviceProductVersion + L"&imei=" + _pDevice->GetId();

	RETURN_FALSE_IF ( !pinfo || pinfo->empty() );

	// 组合URL参数
	for ( int i=0; i<(int)pinfo->size();i++ )
	{
		AppUpdateInfo* info = pinfo->at(i);
		wstring appinfo = L"";
 
		if ( platform == L"1" )
		{
			wstring version = info->_buildVersion.empty() ? info->_oldVersion : info->_buildVersion;
			appinfo = info->_identifier + L", " + version + L", " + info->_oldVersion + L";";
		} 
		else
		{
			appinfo = info->_identifier + L", " + info->_verCode + L";";
		}
		appInfos += appinfo;
	}
	appInfos = L"&installedSofts=" + appInfos;

	wstring sendInfos = appInfos.substr(0, appInfos.length()-1);	
	string postData = CCodeOperation::UnicodeToUTF_8(sendInfos);
	string appDatas	= CNetOperation::Post(CCodeOperation::UnicodeToUTF_8(url), postData);

	RETURN_FALSE_IF (appDatas.empty() || "0" == appDatas);

	return ParseData( appDatas, pinfo );
}

bool CGetAppUpdateInfo::ParseData( string appDatas, vector<AppUpdateInfo*>* pinfo )
{
	std::string strTemp;
	cJSON* pRoot = cJSON_Parse(appDatas.c_str());
	if ( pRoot )
	{
		cJSON *pChild = cJSON_GetObjectItem(pRoot, "Code");
		if ( pChild )
		{
			if (pChild->type == cJSON_String)
			{
				strTemp.assign(pChild->valuestring, strlen(pChild->valuestring));
				RETURN_FALSE_IF ( "0" != strTemp );
			}
			else if ( pChild->type == cJSON_Number )
			{
				RETURN_FALSE_IF ( 0 != pChild->valueint );
			}
		}
		
		pChild = cJSON_GetObjectItem(pRoot, "Result");
		if ( pChild && pChild->type == cJSON_String)
		{
			strTemp.assign(pChild->valuestring, strlen(pChild->valuestring));
			RETURN_FALSE_IF ( strTemp.empty() );
		}
	}

	return ParseInfo( strTemp, pinfo );
}

bool CGetAppUpdateInfo::GetAppsRealUpdateUrls( vector<AppUpdateInfo*>* pinfo )
{
	string appIDs, id, platform;

	RETURN_FALSE_IF ( !pinfo || pinfo->empty() );

	// 组合URL参数
	for ( int i=0; i<(int)pinfo->size(); i++ )
	{
		AppUpdateInfo* info = pinfo->at(i);

		char szID[16] = {0};
		sprintf(szID, ",%d", info->_nfid);

		id = szID;
		appIDs += id;
	}
	appIDs = appIDs.substr(1);

	if ( !_pDevice )
		platform = _deviceType == 1 ? "iphone" : "android";
	
	string url = "http://pcfirm.sj.91.com/controller.ashx?action=batchdownload&platform=" + platform + "&ids=" + appIDs;//批量获取软件升级的真正URL

	string appUrls = CNetOperation::Get(url);

	xml_document<char> doc;
	doc.parse<0>((char*)appUrls.c_str());

	rapidxml::xml_node<char>* root = doc.first_node();            //xml文件的根节点
	RETURN_FALSE_IF (!root);

	for (rapidxml::xml_node<char>* node = root->first_node(); node!=NULL; node = node->next_sibling())
	{
		string strname	= node->name();
		if ( "softinfo" == strname )
		{
			int appID;
			wstring strUrl;
			for (rapidxml::xml_node<char>* listnode = node->first_node(); listnode!=NULL; listnode = listnode->next_sibling())
			{
				if (!listnode)
					continue;
				strname			= listnode->name();
				string strVal	= listnode->value();

				if ( "id" == strname )
				{
					appID = atoi(strVal.c_str());
				}
				else if ( "url" == strname )
				{
					rapidxml::xml_node<char>* valuenode = listnode->first_node();
					strVal = valuenode->value();
					strUrl = CCodeOperation::UTF_8ToUnicode(strVal);
				}
				else
					continue;
			}

			for ( int i=0;i<(int)pinfo->size();i++ )
			{
				AppUpdateInfo* info = pinfo->at(i);
				if ( info->_nfid == appID )
				{
					info->_url = strUrl;	
					break;
				}
			}
		}
	}
	return true;
}

bool CGetAppUpdateInfo::GetAppUpdateDataNew( vector<AppUpdateInfo*>* pinfo, bool bLegal )
{
	wstring appInfos, platform, imei, actType, phoneVersion;

	if ( !_pDevice )
	{
		platform = _deviceType == 1 ? L"1" : L"4";
		imei = L"";
		phoneVersion = L"";
	}
	else
	{
		platform = _pDevice->GetDevInfo()->_deviceType == Ios ? L"iphone" : L"android";
		imei = _pDevice->GetId();
		phoneVersion = _pDevice->GetDevInfo()->_deviceProductVersion;
	}

	actType = bLegal ? L"605" : L"404";
	wstring urlWords = bLegal ? L"http://appupdate2.sj.91.com/" : L"http://appupdate.sj.91.com/";

	wstring url = urlWords + L"service.ashx?act=" + actType + L"&proj=2900&platform=" + platform + L"&fw="
					+ phoneVersion + L"&imei=" + imei;

	RETURN_FALSE_IF ( !pinfo || pinfo->empty() );

	// 组合URL参数
	for ( int i=0; i<(int)pinfo->size();i++ )
	{
		AppUpdateInfo* info = pinfo->at(i);
		wstring appinfo = bLegal ? CStrOperation::IntToWString(info->_nfid) + L", " :  info->_identifier + L", ";
		
		if ( platform == L"1" )
			appinfo += info->_oldVersion + L", " + info->_verCode + L";";	 
		else
			appinfo += info->_verCode + L", " + info->_oldVersion + L";";
		
		appInfos += appinfo;
	}
	appInfos = L"&installedSofts=" + appInfos;

	wstring sendInfos = appInfos.erase(appInfos.length()-1, 1);	
	string postData = CCodeOperation::UnicodeToUTF_8(sendInfos);
	string appDatas	= CNetOperation::Post(CCodeOperation::UnicodeToUTF_8(url), postData);

	RETURN_FALSE_IF (appDatas.empty() || "0" == appDatas);

	return ParseJsonData( appDatas, pinfo, bLegal );
}

bool CGetAppUpdateInfo::ParseJsonData( string appInfos, vector<AppUpdateInfo*>* pinfo, bool bLegal )
{
	std::string strTemp;
	cJSON* pRoot = cJSON_Parse(appInfos.c_str());
	if ( pRoot )
	{
		cJSON *pChild = cJSON_GetObjectItem(pRoot, "Code");
		if ( pChild && pChild->type == cJSON_Number)
		{
			RETURN_FALSE_IF ( 0 != pChild->valueint );
		}

		pChild = cJSON_GetObjectItem(pRoot, "Result");
		if ( pChild && pChild->type == cJSON_Array)
		{
			int size = cJSON_GetArraySize(pChild);

			for ( int i=0;i<size;i++ )
			{
				cJSON* pJson = cJSON_GetArrayItem(pChild, i);
				if ( pJson )
				{
					AppUpdateInfo* info = new AppUpdateInfo;

					info->_bLegal = bLegal;

					cJSON *pChild = cJSON_GetObjectItem(pJson, "f_id");
					if ( pChild && pChild->type == cJSON_String)
					{
						strTemp.assign(pChild->valuestring, strlen(pChild->valuestring));
						info->_nfid = atoi(strTemp.c_str());
					}

					pChild = cJSON_GetObjectItem(pJson, "Name");
					if ( pChild && pChild->type == cJSON_String)
					{
						strTemp.assign(pChild->valuestring, strlen(pChild->valuestring));
						info->_name = CCodeOperation::UTF_8ToUnicode(strTemp);
					}

					pChild = cJSON_GetObjectItem(pJson, "Size");
					if ( pChild && pChild->type == cJSON_String)
					{
						strTemp.assign(pChild->valuestring, strlen(pChild->valuestring));
						info->_nsize = atoi(strTemp.c_str());
					}

					pChild = cJSON_GetObjectItem(pJson, "Identifier");
					if ( pChild && pChild->type == cJSON_String)
					{
						strTemp.assign(pChild->valuestring, strlen(pChild->valuestring));
						info->_identifier = CCodeOperation::UTF_8ToUnicode(strTemp);
					}

					pChild = cJSON_GetObjectItem(pJson, "AddTime");
					if ( pChild && pChild->type == cJSON_String)
					{
						strTemp.assign(pChild->valuestring, strlen(pChild->valuestring));
						info->_addTime = CDealTime::StringToTime(CCodeOperation::UTF_8ToUnicode(strTemp));
					}

					pChild = cJSON_GetObjectItem(pJson, "OldVersion");
					if ( pChild && pChild->type == cJSON_String)
					{
						strTemp.assign(pChild->valuestring, strlen(pChild->valuestring));
						info->_oldVersion = CCodeOperation::UTF_8ToUnicode(strTemp);
					}

					pChild = cJSON_GetObjectItem(pJson, "NewVersion");
					if ( pChild && pChild->type == cJSON_String)
					{
						strTemp.assign(pChild->valuestring, strlen(pChild->valuestring));
						info->_newVersion = CCodeOperation::UTF_8ToUnicode(strTemp);
					}

					pChild = cJSON_GetObjectItem(pJson, "VersionCode");
					if ( pChild && pChild->type == cJSON_String)
					{
						strTemp.assign(pChild->valuestring, strlen(pChild->valuestring));
						info->_verCode = CCodeOperation::UTF_8ToUnicode(strTemp);
					}

					pChild = cJSON_GetObjectItem(pJson, "Update");
					if ( pChild && pChild->type == cJSON_String)
					{
						strTemp.assign(pChild->valuestring, strlen(pChild->valuestring));
						info->_bUpdate = atoi(strTemp.c_str()) == 1 ? true : false;
					}

					pChild = cJSON_GetObjectItem(pJson, "DownNum");
					if ( pChild && pChild->type == cJSON_String)
					{
						strTemp.assign(pChild->valuestring, strlen(pChild->valuestring));
						info->_nDownNum = atoi(strTemp.c_str());
					}

					pChild = cJSON_GetObjectItem(pJson, "URL");
					if ( pChild && pChild->type == cJSON_String)
					{
						strTemp.assign(pChild->valuestring, strlen(pChild->valuestring));
						info->_url = CCodeOperation::UTF_8ToUnicode(strTemp);
					}
				

					for ( int i=0;i<(int)pinfo->size();i++ )
					{
						wstring temp1 = bLegal ? CStrOperation::IntToWString(info->_nfid) : info->_identifier;
						wstring temp2 = bLegal ? CStrOperation::IntToWString(pinfo->at(i)->_nfid) : pinfo->at(i)->_identifier;

						if ( temp1 == temp2 )
						{
							AppUpdateInfo* appData = pinfo->at(i);
							SAFE_DELETE(appData);

							pinfo->at(i) = info;	
							break;
						}
					}	
				}
			}
		}
	}
	
	return true;

}

bool CGetAppUpdateInfo::GetItunesidByIdentify( vector<AppUpdateInfo*>* pinfo )
{
	wstring infos;
	// 组合URL参数
	for ( int i=0; i<(int)pinfo->size();i++ )
	{
		AppUpdateInfo* info = pinfo->at(i);
		if ( !info ) continue;
		
		infos += info->_identifier + L";";
	}
	wstring url = L"http://itunesapp2.sj.91.com/xmlcontroller.ashx?action=getsoftlist";

	std::string postData = CCodeOperation::UnicodeToUTF_8(infos);
	std::string appDatas = CNetOperation::Post(CCodeOperation::UnicodeToUTF_8(url), postData);

	RETURN_FALSE_IF (appDatas.empty() || "0" == appDatas);


	xml_document<char> doc;
	doc.parse<0>((char*)appDatas.c_str());

	rapidxml::xml_node<char>* root = doc.first_node();            //xml文件的根节点
	RETURN_FALSE_IF (!root);
	std::string nodeName = root->name();	
	RETURN_FALSE_IF ( "fileinfo" != nodeName );

	std::string strTemp;
	for (rapidxml::xml_node<char>* node = root->first_node(); node!=NULL; node = node->next_sibling())
	{
		if ( !node ) continue;
				
		std::string strname	= node->name();
		if ( "soft" != strname ) continue;
		strTemp = node->value();
		
		int pos = strTemp.find("|");
		std::string strItunesID, strIdentify;

		if ( pos > 1 )
		{
			strItunesID = strTemp.substr(0,pos);
			strIdentify = strTemp.substr(pos+1);
		}	

		for ( int i=0; i<(int)pinfo->size();i++ )
		{
			AppUpdateInfo* info = pinfo->at(i);
			if ( !info ) continue;

			if ( info->_identifier == CCodeOperation::UTF_8ToUnicode(strIdentify) )
			{
				info->_itunesid = atoi(strItunesID.c_str());
			}							
		}

		
	}
	return true;
}