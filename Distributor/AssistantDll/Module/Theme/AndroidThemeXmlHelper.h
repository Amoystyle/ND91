#pragma once

#include "Common/rapidxml/rapidxml.hpp"
#include "Common/rapidxml/rapidxml_utils.hpp"
#include "Common/rapidxml/rapidxml_print.hpp"
#include "Common/rapidxml/rapidxml_iterators.hpp"
using namespace rapidxml;

#include "Module/Theme/ThemeData.h"

namespace ND91Assistant
{

// 主题相关xml文件解析
class AndroidThemeXmlHelper
{
public:
	AndroidThemeXmlHelper(void);
	virtual ~AndroidThemeXmlHelper(void);

	// Summary  : 解析全部主题信息
	// Returns	: std::wstring 返回当前主题ID
	// Parameter: wstring localpath 要解析的xml文件
	// Parameter: deque<ThemeData * > * themeList 主题列表
	static ThemeData* ResolveAllThemes(wstring localpath, deque<ThemeData*>* themeList);


	static ThemeData* ResolveSmartThemes( wstring localpath, deque<ThemeData*>* themeList );

	// Summary  : 解析单个主题详细信息
	// Returns	: ThemeData*
	// Parameter: wstring localPath要解析的xml文件
	static ThemeData*  ResolveDetailTheme(wstring localPath);

	// Summary  : 解析主题简要信息，包含当前主题id和主题id列表
	// Returns	: ND91Assistant::ThemeBriefInfo
	// Parameter: wstring localPath要解析的xml文件
	static ThemeBriefInfo ResolveBriefTheme(wstring localPath);

	// Summary  : 解析一个完整的主题
	// Returns	: ThemeData*
	// Parameter: wstring localPath要解析的xml文件
	static ThemeData*  ResolveTotalTheme(wstring localPath);

	static int IsNeededAppIcon(wstring  appid);

	static string urldecode(string valuestr);

private:

	// Summary  : 解析得到单个主题
	// Returns	: ThemeData*
	// Parameter: xml_node<wchar_t> * themeNode
	static ThemeData*  ResolveDetailTheme(xml_node<char> * themeNode);
};

}
