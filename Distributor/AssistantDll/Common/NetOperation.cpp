#include "Common/NetOperation.h"
#include "Common/CodeOperation.h"
#include "Common/w3c.h"
//#include "httpRequest.h"

#include <fstream>

bool CNetOperation::ParseUrl( string strUrl, string& strHost, string& strPage )
{
    int nIndex = strUrl.find("://");
    if (nIndex>-1)
    {
		strUrl = strUrl.substr(nIndex + 3);
        nIndex = strUrl.find("/");
        if (nIndex > -1)
        {
            strHost = strUrl.substr(0, nIndex);
            strPage = strUrl.substr(nIndex);
            return true;
        }
    }
    return false;
}

std::string CNetOperation::Get( string strUrl, bool auto_redirect  )
{
    string strHost="", strPage="";
    if (!ParseUrl(strUrl, strHost, strPage))
        return "";
    return Get(strHost, strPage, auto_redirect);  
}

std::string CNetOperation::Get( string strHost, string strPage, bool auto_redirect )
{
    string strReturn="";

// 	string sHeaderSend;     //定义http头   
// 	string sHeaderReceive;  //返回头   
// 	//string sMessage="";     //返回页面内容
// 
// 	string url("http://");
// 	url+=strHost;
// 	url+=strPage;
// 	Request httprq;
// 	httprq.SendRequest(false,url,sHeaderSend,sHeaderReceive,strReturn);
// 
// 	return strReturn;

    W3Client w3;
	try{
		w3.SetAllowAutoRedirect(auto_redirect);
		if(w3.Connect(strHost.c_str()))
		{
			if(w3.Request(strPage.c_str()))
			{
				char buf[1024] = "\0";
				int nSize = 0;
				while((nSize = w3.Response(reinterpret_cast<unsigned char *>(buf), 1024)) > 0)
				{
					strReturn.append(buf, nSize);
				}
			}
		}
	}
	catch (...)
	{
	}
    return strReturn;
}

bool CNetOperation::DownloadFile( string strUrl, wstring strFile )
{
    std::ofstream file(strFile.c_str(), std::ios::out|std::ios::ate|std::ios::binary);
    if (file.fail())
        return false;

    string strHost="", strPage="";
    if (!ParseUrl(strUrl, strHost, strPage))
	{
		file.close();
        return false;
	}
    W3Client w3;
	try{
		if(w3.Connect(strHost.c_str()))
		{
			if(w3.Request(strPage.c_str()))
			{
				char buf[1024] = "\0";
				int nSize = 0;
				while((nSize = w3.Response(reinterpret_cast<unsigned char *>(buf), 1024)) > 0)
				{
					for (int i=0; i<nSize; i++)
						file.put(buf[i]);
				}
				file.close();
				return true;
			}
		}
		//file.close();
	}
	catch (...)
	{
		//return false;
	}
	file.close();
    return false;

}


string CNetOperation::GetUrlHost( string strUrl )
{
    string strHost = strUrl;
    int nIndex,nIndex2;
    nIndex = strUrl.find("://");
    if (nIndex)
    {
        nIndex2 = strUrl.find("/", nIndex + 4);
        if (nIndex2 > 0)
            strHost = strUrl.substr(nIndex+3, nIndex2-nIndex-3);
    }
    return strHost;
}

std::wstring CNetOperation::GetUrlHostW( wstring strUrl )
{
    wstring strHost = strUrl;
    int nIndex,nIndex2;
    nIndex = strUrl.find(L"://");
    if (nIndex)
    {
        nIndex2 = strUrl.find(L"/", nIndex + 4);
        if (nIndex2 > 0)
            strHost = strUrl.substr(nIndex+3, nIndex2-nIndex-3);
    }
    return strHost;
}

string CNetOperation::Post( string strUrl, string postData )
{
    string strHost="", strPage="";
    if (!ParseUrl(strUrl, strHost, strPage))
        return "";
    return Post(strHost, strPage, postData);
}

string CNetOperation::Post( string strHost, string strPage, string postData )
{

	string strReturn="";
	//string sHeaderReceive;  //返回头   
	////string sMessage="";     //返回页面内容

	//string url("http://");
	//url+=strHost;
	//url+=strPage;
	//Request httprq;
	//httprq.SendRequest(true,url,postData,sHeaderReceive,strReturn);
	//return strReturn;

#ifdef WIN32

    W3Client w3;
	try{
		if(w3.Connect(strHost.c_str()))
		{
			w3.SetPostData(postData);
			if(w3.Request(strPage.c_str(), W3Client::reqPost))
			{
				char buf[1024] = "\0";
				int nSize = 0;
				while((nSize = w3.Response(reinterpret_cast<unsigned char *>(buf), 1024)) > 0)
				{
					strReturn.append(buf, nSize);
				}
			}
		}
	}
	catch (...)
	{
	}

    w3.Close();

#endif	
    return strReturn;
}

string CNetOperation::URLEncode( string src )
{
    char hex[] = "0123456789ABCDEF";
    std::string dst;  
    for (size_t i = 0; i < src.size(); i++)  
    {  
        unsigned char ch = src[i];  
        if (isalnum(ch))  
        {  
            dst += ch;  
        }  
        else  
            if (src[i] == ' ')  
            {  
                dst += '+';  
            }  
            else  
            {  
                unsigned char c = static_cast<unsigned char>(src[i]);  
                dst += '%';  
                dst += hex[c / 16];  
                dst += hex[c % 16];  
            }  
    }  
    return dst; 
}

string CNetOperation::URLEncode( wstring wsrc )
{
    return URLEncode(CCodeOperation::UnicodeToGB2312(wsrc));
}

string CNetOperation::URLDecode( string src )
{
	string result;
    size_t nHigh = src.length();
	for (size_t i=0; i<nHigh;i++)
	{
		switch(src[i])
		{
		case '+':
			result += ' ';
			break;
		case '%':
			if(i+2 < nHigh && isxdigit(src[i + 1]) && isxdigit(src[i + 2]))
			{
				char res = 0;                                         
				res = (src[i + 1]<='9')?src[i + 1]-'0':(tolower(src[i + 1])-'a')+10; 
				res <<= 4;                                            
				res |= (src[i + 2]<='9')?src[i + 2]-'0':(tolower(src[i + 2])-'a')+10;
				i += 2;
				result+=res;
			}
			break;
		default:
			result+=src[i];
			break;
		}
	}
	return result;
}
