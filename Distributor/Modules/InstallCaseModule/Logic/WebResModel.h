#pragma once

INSTALLCASE_BEGIN
class WebResModel
{
public:
	WebResModel();
    ~WebResModel();

protected:
    virtual void DoInit();

	
public:
	static wstring GetCurPlatformText(wstring id);

	static void ParseDownUrl(wstring url, wstring& appname, wstring& appversion,wstring& appid);

	static std::wstring urldecode(std::wstring decode);

	static std::wstring urlencode(std::wstring encode);

	static std::wstring GetDownLoadUrl(string htmlcontent);

protected:

	wstring		_curSelectedPlatform;
	wstring		_curtabName;
};

INSTALLCASE_END