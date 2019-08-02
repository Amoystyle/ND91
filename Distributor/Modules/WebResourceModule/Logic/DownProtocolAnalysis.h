#ifndef DownProtocolAnalysis_h__
#define DownProtocolAnalysis_h__

typedef struct AnalysisData 
{
	wstring module;			//资源类型
	wstring action;			//操作类型
	wstring identify;		//应用名称标识符
	wstring name;			//应用程序包名
	wstring vername;		//应用版本
	wstring vercode;		//应用版本号
	wstring icon;			//图标路径
	wstring info;			//应用描述
	wstring src;			//资源来源
	wstring url;			//资源下载地址
	wstring resid;			//fid
	bool    islegal;			//是否正版
	AnalysisData()
	{
		module = L"";
		action = L"";
		identify = L"";
		name = L"";
		vername = L"";
		vercode = L"";
		icon = L"";
		info = L"";
		src = L"";
		url = L"";
		resid = L"";
		islegal = false;
	}
}AnalysisData;


class DownProtocolAnalysis 
{
public:
	DownProtocolAnalysis();
	~DownProtocolAnalysis();

public:
	//解析URL数据
	//url：传入需要分解的url
	//key：传入必须存在的参数，
	//返回值：如果key不为空则检测url中是否存在对应值，不存在则返回false 否则返回true1
	static bool AnalysisUrl(AnalysisData &data,wstring url,wstring key=_T(""));

	static bool WebSoftAnalysisUrl(AnalysisData &data,wstring url);

private:
	//url加解密
	static tstring UrlEncode(tstring text);	
	static tstring UrlDecode(wstring decode);

	//传入对应字符，按=解析，判断字符正确性
	static wstring CheckValue(wstring buf,wstring key);

	static char hex2char(const char st[]) ;

	//网络资讯下载解析
	static void Parse(map<string,string> &mapParams, wstring url);

	//获取网络资源关键字所对应的值
	static wstring GetValue(map<string,string> &mapData, string key);
};


#endif 