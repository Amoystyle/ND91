#include "StdAfx.h"
#include "Logic/WebResModel.h"


INSTALLCASE_BEGIN
WebResModel::WebResModel() 
{INTERFUN;
}

WebResModel::~WebResModel()
{INTERFUN;

}

void WebResModel::DoInit()
{INTERFUN;
	_curSelectedPlatform = L"";
}

wstring WebResModel::GetCurPlatformText(wstring id)
{INTERFUN;
	DeviceData* dev=DEVICE_MANAGER->FindDevice(id);
	wstring platform=L"iPhone";
	if(dev)
	{
		if(dev->GetDevInfo()->_deviceType==Ios)
		{
			if(dev->GetDevInfo()->IsPad())
				platform=L"iPad";
			else
				platform =L"iPhone";
		}
		else if(dev->GetDevInfo()->_deviceType==Android)
		{
			platform = L"Android";
		}
	}
	return platform;
}

void WebResModel::ParseDownUrl( wstring url, wstring& appname, wstring& appversion,wstring& appid)
{INTERFUN;
	//if((int)url.find(L"Action=ReadFile")>-1)
	{
		int pos= url.find(L"f_id");
        if (pos <= 0)
            pos = url.find(L"?");
		vector<wstring> splits= CStrOperation::parseStringsW(url.substr(pos),L"&");
		for (size_t i=0;i<splits.size();i++)
		{
			vector<wstring> namevalue= CStrOperation::parseStringsW(splits[i],L"=");
			if(namevalue[0]==L"f_name")
			{
				appname = WebResModel::urldecode(namevalue[1]);
			}
			if(namevalue[0]==L"f_version")
			{
				appversion= namevalue[1];
				break;
			}
			if(namevalue[0]==L"f_id")
			{
				appid=namevalue[1];
			}
		}
	}

}

char hex2char(const char st[]) {                                                                                               
	char res = 0;                                         

	res = (st[0]<='9')?st[0]-'0':(tolower(st[0])-'a')+10; 
	res <<= 4;                                            
	res |= (st[1]<='9')?st[1]-'0':(tolower(st[1])-'a')+10;

	return res;                                           
}


std::wstring WebResModel::urldecode(std::wstring decode)//utf8£¬unicodeÁ½ÖÖ±àÂë
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

  


std::wstring WebResModel::urlencode( std::wstring encode )
{INTERFUN;
	std::string src= CCodeOperation::UnicodeToUTF_8(encode);  
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

std::wstring WebResModel::GetDownLoadUrl( string htmlcontent )
{INTERFUN;
	int pos=htmlcontent.find("<a href=");
	if(pos>-1)
	{
		string response=htmlcontent.substr(pos);
		pos=response.find(">");
		wstring downurl=CCodeOperation::StringToWstring(response.substr(9,pos-10));
		return downurl;
	}
	return L"";
}

INSTALLCASE_END