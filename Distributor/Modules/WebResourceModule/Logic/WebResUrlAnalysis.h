#pragma once

class WebResUrlAnalysis
{
public:
	WebResUrlAnalysis();
    ~WebResUrlAnalysis();


public:

	static void ParseDownUrl(wstring url, wstring& appname, wstring& appversion,wstring& appid);

	static std::wstring urldecode(std::wstring decode);

	static std::wstring urlencode(std::wstring encode);

	static std::wstring GetDownLoadUrl(string htmlcontent);

protected:
	wstring		_curtabName;
};
