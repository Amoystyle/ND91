#include "stdafx.h"
#include "DownProtocolAnalysis.h"
#include "Common/NetOperation.h"
#include "Common/WebResourceCommonFunc.h"

DownProtocolAnalysis::DownProtocolAnalysis()
{

}

DownProtocolAnalysis::~DownProtocolAnalysis()
{

}


bool DownProtocolAnalysis::WebSoftAnalysisUrl( AnalysisData &data,wstring url )
{
	try
	{
		vector<wstring> dataver = CStrOperation::parseStringsW(url,_T("&"));

		if (dataver.size() > 2)
		{
			data.resid = CheckValue(dataver.at(2),_T("f_id"));
			data.name = CheckValue(dataver.at(3),_T("f_name"));
			data.name = UrlDecode(data.name);
			data.vername = CheckValue(dataver.at(4),_T("f_version"));
			data.vercode = CheckValue(dataver.at(4),_T("f_version"));
			data.icon = CheckValue(dataver.at(5),_T("f_img"));


			string content=CNetOperation::Get(CCodeOperation::ws2s(url),false);
			int pos= content.find("href='");
			RETURN_FALSE_IF(pos<0);
			content =content.substr(pos+6);
			int pos1=content.find("'");
			RETURN_FALSE_IF(pos1<0);
			content =content.substr(0,pos1);
			//if((int)content.find("action=readfile")>-1)
			//	WebResModel::ParseDownUrl(CCodeOperation::UTF_8ToUnicode(content),name,version,appid);
			content = CNetOperation::Get(content,false);
			pos = content.find("href=\"");
			RETURN_FALSE_IF(pos<0);
			content =content.substr(pos+6);
			pos1=content.find("\"");
			RETURN_FALSE_IF(pos1<0);
			content=content.substr(0,pos1);
			wstring path = WebResourceModuleCONFIG(_strIosDownPath);
			data.url = CCodeOperation::UTF_8ToUnicode(content);


		}
	}
	catch (...)
	{	//如果服务端给的URL出错导致项丢失，那么dataver访问越界可能造成奔溃
		
	}
	
	
	return true;
}


bool DownProtocolAnalysis::AnalysisUrl(AnalysisData &data, wstring url,wstring key/*=_T("")*/ )
{
	try
	{
		if (key.length() != 0)
		{
			if (url.find(key) == wstring::npos)
			{
				LOG->WriteDebug(_T("分解URL时寻找关键字未找到KEY = "));
				LOG->WriteDebug(key);

				return false;
			}
		}
		//vector<wstring> dataver = CStrOperation::parseStringsW(url,_T("&"));
		map<string,string> mapParams;
		Parse(mapParams,url);
		if (mapParams.size() > 2)
		{
			data.action = GetValue(mapParams, "action");
			data.module = GetValue(mapParams, "module");
			data.src = GetValue(mapParams, "src");
			data.name = GetValue(mapParams, "name");
			data.name = UrlDecode(data.name);
			data.resid = GetValue(mapParams, "resid");
			data.identify = GetValue(mapParams, "identify");
			data.vername = GetValue(mapParams, "vername");
			data.vercode = GetValue(mapParams, "vercode");
			data.icon = GetValue(mapParams, "icon");
			data.icon = UrlDecode(data.icon);
			data.url = GetValue(mapParams,"url");
			data.url = UrlDecode(data.url);
		}
	}
	catch (...)
	{	//如果服务端给的URL出错导致项丢失，那么dataver访问越界可能造成奔溃
		
	}
	
	
	return true;
}

wstring DownProtocolAnalysis::CheckValue(wstring buf,wstring key)
{
	vector<wstring> data = CStrOperation::parseStringsW(buf,_T("="));	
	if (data.size()>1 && data.at(0).find(key) != wstring::npos)
	{	
		return data.at(1);
	}

	return _T("");
}

tstring DownProtocolAnalysis::UrlEncode( tstring text )
{
	std::string src= CCodeOperation::UnicodeToUTF_8(text);  
	wchar_t hex[] = L"0123456789ABCDEF";  
	wstring dst;
	for (size_t i = 0; i < src.size(); ++i)
	{
		unsigned char cc = src[i];
		if (isascii(cc))
		{
			if (cc == ' ')
			{
				dst += L"%20";
			}
			else
			{
				wchar_t w[2];
				mbstowcs(w,src.c_str()+i,1);
				dst += w[0];
			}
		}
		else
		{
			unsigned char c = static_cast<unsigned char>(src[i]);
			dst += L'%';
			dst += hex[c / 16];
			dst += hex[c % 16];
		}
	}
	return dst;  
}

char DownProtocolAnalysis::hex2char(const char st[]) 
{                                                                                               
	char res = 0;                                         

	res = (st[0]<='9')?st[0]-'0':(tolower(st[0])-'a')+10; 
	res <<= 4;                                            
	res |= (st[1]<='9')?st[1]-'0':(tolower(st[1])-'a')+10;

	return res;                                           
}

tstring DownProtocolAnalysis::UrlDecode( wstring decode )
{
	std::wstring result;
	std::string sdecode=CCodeOperation::UnicodeToUTF_8(decode);
	string strret;
	bool IsUnicode=sdecode.find("%u")!=string::npos;
	for(unsigned int i = 0; i< static_cast<unsigned int>(sdecode.length()); i++)
	{
		switch(sdecode[i])
		{
		case '+':
			{
				if(!IsUnicode)
					strret+=' ';
				else
					result += L' ';
			}
			break;
		case '%':
			if(isxdigit(sdecode[i + 1]) && isxdigit(sdecode[i + 2]))
			{
				strret+=hex2char(sdecode.substr(i+1,2).c_str());
				if(IsUnicode)
				{
					result+=CCodeOperation::UTF_8ToUnicode(strret);
					strret="";
				}
				i += 2;
			}
			else if(sdecode[i + 1]=='u')
			{
				char strchar[3];
				strchar[0]=hex2char(sdecode.substr(i+4,2).c_str());
				strchar[1]=hex2char(sdecode.substr(i+2,2).c_str());
				unsigned short hex;
				memcpy((char*)&hex, strchar,2);
				result += (wchar_t)hex;
				i+=5;
			}
			else 
			{
				if(IsUnicode)
					result += L'%';
				else
					strret+=sdecode[i];
			}
			break;
			default:
			if(!IsUnicode)
				strret+=sdecode[i];
			else
				result += decode[i];
			break;
		}
	}
	if(!IsUnicode)
		result = CCodeOperation::UTF_8ToUnicode(strret);
	return result;
}

void DownProtocolAnalysis::Parse( map<string,string> &mapParams, wstring url )
{
	string urlTmp = CCodeOperation::ws2s(url);
	vector<string> urlDataTmp;
	WebResourceCommonFunc::Split(urlTmp,'?',urlDataTmp);
	if (urlDataTmp.size()>=2)
	{
		string strParam = urlDataTmp[1];
		vector<string> params;
		WebResourceCommonFunc::Split(strParam,'&',params);
		for (vector<string>::iterator it = params.begin(); it != params.end(); ++it)
		{
			vector<string> paramTmp;
			string strParamTmp = *it;
			WebResourceCommonFunc::Split(strParamTmp,'=',paramTmp);
			if (paramTmp.size()>=2)
			{
				mapParams[paramTmp[0]] = paramTmp[1];
			}
		}
	}
}

wstring DownProtocolAnalysis::GetValue( map<string,string> &mapData, string key )
{
	wstring result = L"";
	map<string,string>::iterator iter = mapData.find(key);
	if (iter != mapData.end())
	{
		string tmp = iter->second;
		result = CCodeOperation::s2ws(tmp);
	}
	return result;
}
