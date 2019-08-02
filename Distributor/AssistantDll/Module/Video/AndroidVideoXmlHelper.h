#pragma once

#include "Module/Video/PlayAlbumData.h"

#include "Common/rapidxml/rapidxml.hpp"
#include "Common/rapidxml/rapidxml_utils.hpp"
#include "Common/rapidxml/rapidxml_print.hpp"
#include "Common/rapidxml/rapidxml_iterators.hpp"
#include <string>

using namespace std;
using namespace rapidxml;

namespace ND91Assistant
{

class AndroidVideoXmlHelper
{
public:
	AndroidVideoXmlHelper(void);
	virtual ~AndroidVideoXmlHelper(void);

	// Summary  : 解析专辑信息xml文件
	// Returns	: PlayAlbumData*
	// Parameter: wstring xmlFile
	static void GetPlayerAlbum(wstring xmlFile, PlayAlbumData* albumData);


	// Summary  : 解析下载的plistxml专辑信息
	// Returns	: PlayAlbumData*
	// Parameter: wstring plistxmlfile
	static PlayAlbumData*  ParsePlayerAlbumPlistXML(wstring  plistxmlfile);


	// Summary  : 生成专辑信息xml文件
	// Returns	: void
	// Parameter: PlayAlbumData * album
	// Parameter: wstring destlocalpath
	static void CreatePlayerAlbumXMLInfo(PlayAlbumData* album, wstring destlocalpath);

private:

	static PlayerAlbumItem* ParsePlayerAlbumItem(xml_node<char>* itemNode);

};

}
