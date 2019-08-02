#pragma once

#include "Common/rapidxml/rapidxml.hpp"
#include "Common/rapidxml/rapidxml_utils.hpp"
#include "Common/rapidxml/rapidxml_print.hpp"
#include "Common/rapidxml/rapidxml_iterators.hpp"
using namespace rapidxml;

#include "Module/Theme/ThemeData.h"

namespace ND91Assistant
{

// �������xml�ļ�����
class AndroidThemeXmlHelper
{
public:
	AndroidThemeXmlHelper(void);
	virtual ~AndroidThemeXmlHelper(void);

	// Summary  : ����ȫ��������Ϣ
	// Returns	: std::wstring ���ص�ǰ����ID
	// Parameter: wstring localpath Ҫ������xml�ļ�
	// Parameter: deque<ThemeData * > * themeList �����б�
	static ThemeData* ResolveAllThemes(wstring localpath, deque<ThemeData*>* themeList);


	static ThemeData* ResolveSmartThemes( wstring localpath, deque<ThemeData*>* themeList );

	// Summary  : ��������������ϸ��Ϣ
	// Returns	: ThemeData*
	// Parameter: wstring localPathҪ������xml�ļ�
	static ThemeData*  ResolveDetailTheme(wstring localPath);

	// Summary  : ���������Ҫ��Ϣ��������ǰ����id������id�б�
	// Returns	: ND91Assistant::ThemeBriefInfo
	// Parameter: wstring localPathҪ������xml�ļ�
	static ThemeBriefInfo ResolveBriefTheme(wstring localPath);

	// Summary  : ����һ������������
	// Returns	: ThemeData*
	// Parameter: wstring localPathҪ������xml�ļ�
	static ThemeData*  ResolveTotalTheme(wstring localPath);

	static int IsNeededAppIcon(wstring  appid);

	static string urldecode(string valuestr);

private:

	// Summary  : �����õ���������
	// Returns	: ThemeData*
	// Parameter: xml_node<wchar_t> * themeNode
	static ThemeData*  ResolveDetailTheme(xml_node<char> * themeNode);
};

}
