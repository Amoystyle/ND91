#pragma once

#include "AssistantDll.h"
#include <string>
using namespace std;

// 网络操作类，包括Url的操作
class CLASSINDLL_CLASS_DECL CNetOperation
{
public:
    static bool ParseUrl(string strUrl, string& strHost, string& strPage);
    static string GetUrlHost(string strUrl);
    static wstring GetUrlHostW(wstring strUrl);
    static bool DownloadFile(string strUrl, wstring strFile);

    static string Post(string strUrl, string postData);
    static string Post(string strHost, string strPage, string postData);
    static string Get(string strUrl,  bool auto_redirect=true);
    static string Get(string strHost, string strPage, bool auto_redirect=true);

    static string URLEncode(string src);
    static string URLEncode(wstring wsrc);

	static string URLDecode(string src);
};