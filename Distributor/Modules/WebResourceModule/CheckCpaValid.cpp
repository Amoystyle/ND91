#include "StdAfx.h"
#include "CheckCpaValid.h"

const wstring g_wsCheckCpaUrl = L"http://zj.91.com/controller.ashx?action=checkcpasoftstatus&hashvalues=";
const string g_sCheckCpaInstalledUrl = "http://zj.91.com/controller.ashx?action=checkhasinstalledcpa&imei=";

CCheckLocalCpaValid::CCheckLocalCpaValid(void)
{
}

CCheckLocalCpaValid::~CCheckLocalCpaValid(void)
{
}

void CCheckLocalCpaValid::CheckLocal(const map<string, wstring>& map_CpaInfo)
{
	if (map_CpaInfo.empty())
	{
		return;
	}

	int nNode=0;
	__int64 n64AllSize=0;
	__int64 n64Size=0;
	CFileOperation::GetDirSizeW(CONFIG(_strAndroid_CpaDownPath), nNode, n64Size);
	n64AllSize = n64Size;
	CFileOperation::GetDirSizeW(CONFIG(_strIos_CpaDownPath), nNode, n64Size);
	n64AllSize += n64Size;

	if (n64AllSize>1024*1024*500)
	{
		wstring wsUrl = g_wsCheckCpaUrl;
		for ( map<string, wstring>::const_iterator iter = map_CpaInfo.begin(); iter != map_CpaInfo.end(); ++iter)
		{
			wsUrl = wsUrl + CCodeOperation::GB2312ToUnicode(iter->first) + L",";
		}
		int nPos = wsUrl.find_last_of(L",");
		wsUrl = wsUrl.substr(0, nPos);

		vector<string> vec_Recieve;
		string  strData=CNetOperation::Get(CCodeOperation::WstringToString(wsUrl));
		vec_Recieve = this->ParseCpaJason(strData);

		this->DeleteInvalidCpa(map_CpaInfo, vec_Recieve);
	}
}

vector<string> CCheckLocalCpaValid::ParseCpaJason(string& wsInfo)
{
	vector<string> vec_Temp;
	if (!wsInfo.empty())
	{
		cJSON* pRoot = cJSON_Parse(wsInfo.c_str());

		if(NULL != pRoot)
		{
			int arraySize = cJSON_GetArraySize(pRoot);
			vec_Temp.resize(arraySize);
			for(int i=0; i < arraySize; ++i)
			{
				cJSON * pItem = cJSON_GetArrayItem(pRoot, i);
				if ( pItem && pItem->type == cJSON_String )
				{
					if(NULL!=pItem->valuestring)
					{
						vec_Temp.push_back(pItem->valuestring);
					}
				}
			}
		}
	}

	return vec_Temp;
}

void CCheckLocalCpaValid::DeleteInvalidCpa(const map<string, wstring>& map_CpaInfo, vector<string>& vec_InvalidCpaInfo)
{
	for (vector<string>::iterator iter = vec_InvalidCpaInfo.begin(); iter != vec_InvalidCpaInfo.end(); ++iter)
	{
		const map<string, wstring>::const_iterator Deliter = map_CpaInfo.find(*iter);
		if (Deliter != map_CpaInfo.end())
		{
			CFileOperation::DeleteFile(Deliter->second);
		}
	}
}

bool CCheckLocalCpaValid::ParseServerReturn(string& strData, string& strReturn)
{
	bool bRes = false;
	if (!strData.empty())
	{
		cJSON* pRoot = cJSON_Parse(strData.c_str());

		if(NULL != pRoot)
		{
			int arraySize = cJSON_GetArraySize(pRoot);
			for(int i=0; i < arraySize; ++i)
			{
				cJSON * pItem = cJSON_GetArrayItem(pRoot, i);
				if ( pItem)
				{
					if (pItem->type == cJSON_Number)
					{
						if (1 == pItem->valueint)
						{
							bRes = true;
						}
					}
					if (pItem->type == cJSON_String)
					{
						string strValue = pItem->valuestring;
						strReturn = CCodeOperation::UTF_8ToGB2312(strValue);
					}
				}
			}
		}
	}

	return bRes;
}

bool CCheckLocalCpaValid::Handle(CheckCpaInstalledMsg* pMsg)
{
	if (NULL == pMsg || pMsg->_strHash.empty() || pMsg->_strImei.empty())
	{
		return false;
	}
	string wsUrl = g_sCheckCpaInstalledUrl;
	wsUrl = wsUrl + pMsg->_strImei + "&hashvalue=" + pMsg->_strHash;

	string  strData=CNetOperation::Get(wsUrl);
	string strReturn;
	bool bRes = this->ParseServerReturn(strData, strReturn);
	if (bRes)
	{
		pMsg->_strReturn = strReturn;
	}
	return true;
}

bool CCheckLocalCpaValid::Handle( const DeleteLocalCpa* pMsg )
{
	CheckLocal(*(pMsg->_map));
	pMsg->_map->clear();
	delete pMsg->_map;
	return true;
}
