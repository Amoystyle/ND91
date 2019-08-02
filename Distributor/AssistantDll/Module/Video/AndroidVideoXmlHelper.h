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

	// Summary  : ����ר����Ϣxml�ļ�
	// Returns	: PlayAlbumData*
	// Parameter: wstring xmlFile
	static void GetPlayerAlbum(wstring xmlFile, PlayAlbumData* albumData);


	// Summary  : �������ص�plistxmlר����Ϣ
	// Returns	: PlayAlbumData*
	// Parameter: wstring plistxmlfile
	static PlayAlbumData*  ParsePlayerAlbumPlistXML(wstring  plistxmlfile);


	// Summary  : ����ר����Ϣxml�ļ�
	// Returns	: void
	// Parameter: PlayAlbumData * album
	// Parameter: wstring destlocalpath
	static void CreatePlayerAlbumXMLInfo(PlayAlbumData* album, wstring destlocalpath);

private:

	static PlayerAlbumItem* ParsePlayerAlbumItem(xml_node<char>* itemNode);

};

}
