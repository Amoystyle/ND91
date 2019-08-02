#include "stdafx.h"
#include "WebResourceCommonFunc.h"
#include <sstream>
#include <fstream>

wchar_t* WebResourceCommonFunc::AllocateBuffer( wstring sn )
{    
	wchar_t* pBuffer = new wchar_t[sn.size() + 1];
	wcscpy_s(pBuffer, sn.size()+1, sn.c_str());
	pBuffer[sn.size()] = 0;
	return pBuffer;
}

void WebResourceCommonFunc::Split( const string &s, char delim, vector<string> &elems )
{
	istringstream iss(s);
	string item;
	while(getline(iss,item,delim))
	{
		elems.push_back(item);
	}
}

string WebResourceCommonFunc::GetGuid()
{
	GUID   m_guid; 
	char rTmp[128]; 
	if   (S_OK ==::CoCreateGuid(&m_guid))   
	{         
		sprintf(rTmp,"%08X%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X" ,
			m_guid.Data1,  m_guid.Data2,   m_guid.Data3 ,
			m_guid.Data4[0],   m_guid.Data4[1],
			m_guid.Data4[2],   m_guid.Data4[3],
			m_guid.Data4[4],   m_guid.Data4[5],
			m_guid.Data4[6],   m_guid.Data4[7] );  
	}   
	string r(rTmp);
	return r;
}

string WebResourceCommonFunc::GetUrlActionValue( string url, string action )
{
	string result = "";
	vector<string> vecTmp1;
	Split(url,'?',vecTmp1);
	if (vecTmp1.size() >= 2)
	{
		vector<string> vecParam;
		Split(vecTmp1[1],'&',vecParam);
		for (vector<string>::iterator iter = vecParam.begin(); iter != vecParam.end(); ++iter)
		{
			vector<string> actionAndValue;
			Split((*iter),'=',actionAndValue);
			if (actionAndValue.size()>=2)
			{
				if (actionAndValue[0] == action)
				{
					result = actionAndValue[1];
					break;
				}
			}
		}
	}
	return result;
}


