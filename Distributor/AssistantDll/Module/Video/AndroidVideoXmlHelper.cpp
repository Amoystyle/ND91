#include "pch_module.h"

#ifdef MODULE_VIDEO

#include "Module/Video/AndroidVideoXmlHelper.h"
#include "Module/Video/VideoData.h"

#define XPLIST_KEY			"key"
#define XPLIST_STRING		"string"
#define XPLIST_ARRAY		"array"
#define XPLIST_DICT			"dict"
#define XPLIST_PI			"xml version=\"1.0\" encoding=\"UTF-8\""
#define XPLIST_DOCTYPE		"plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\""

#define KEY_ALBUM_ID        "kalbumid"
#define KEY_ALBUM_NAME      "kalbumname"
#define KEY_ALBUM_COUNT     "kalbumcount"
#define KEY_ALBUM_UPDATE    "kalbumupdate"
#define KEY_ALBUM_TYPE      "kalbumtype"
#define KEY_ALBUM_URL       "kalbumpurl"
#define KEY_ALBUM_INTRODUCE "kalbumtip"


const char* Album_Tags[]=
{
	"albumid",
	"albumname",
	"count",
	"update",
	"type",
	"showingtime",
	"director",
	"actor",
	"introduce",
	"albumurl",
	"item"
};

const char* Album_Item_Tags[]=
{
	"fileid",
	"filename",
	"fileurl",
	"filepath",
	"filesize",
	"fileorder",
	"filetype"
};

AndroidVideoXmlHelper::AndroidVideoXmlHelper(void)
{
}

AndroidVideoXmlHelper::~AndroidVideoXmlHelper(void)
{
}

void ND91Assistant::AndroidVideoXmlHelper::GetPlayerAlbum( wstring xmlFile, PlayAlbumData* albumData)
{
	RETURN_IF(xmlFile.empty() || !CFileOperation::IsFileExist(xmlFile.c_str()));
	string strData = CFileOperation::ReadFile(xmlFile, false);

	xml_document<char> doc;
	doc.parse<0>(const_cast<char *>(strData.c_str()));

	xml_node<char> * albumNode = doc.first_node("album");

	RETURN_IF(!albumNode);

	xml_node<char> * childNode = albumNode->first_node();
	RETURN_IF(!childNode);

	if(!albumData)
		albumData = new PlayAlbumData();
    vector<wstring> fileidList;
	while(childNode)
	{
		string name = childNode->name();
		string nametoLow = CStrOperation::toLower(name);
		int  findIndex = -1;
		for (int i =0; i < 11; i++)
		{
			if(Album_Tags[i]== nametoLow)
			{
				findIndex = i;
				break;
			}
		}
		string svalue = childNode->value();
		if(svalue.empty())// 值不存在，或许在cdata子节点
		{
			xml_node<char> * cdata = childNode->first_node("");
			if (cdata && (cdata->type()== node_cdata || cdata->type()== node_data))
				svalue = cdata->value();
		}
		wstring childvalue = CCodeOperation::UTF_8ToUnicode(svalue);
		switch(findIndex)
		{
			case 0:
				albumData->_albumId =  childvalue;
				break;
			case 1:
				albumData->_albumName = childvalue;
				break;
			case 2:
				albumData->_count     = childvalue;
				break;
			case 3:
				albumData->_updata    = childvalue;
				break;
			case 4:
				albumData->_type      = _wtoi(childvalue.c_str());
				break;
			case 5:
				albumData->_showingTime= childvalue;
				break;
			case 6:
				albumData->_director  = childvalue;
				break;
			case 7:
				albumData->_actor     = childvalue;
				break;
			case 8:
				albumData->_introduce = childvalue;
				break;
			case 9:
				albumData->_albumUrl  = childvalue;
				break;
			case 10:
				{
					PlayerAlbumItem* albumItem=ParsePlayerAlbumItem(childNode);
					if (albumItem)
					{
                        // 检查重复
                        bool bFound = false;
                        for (vector<wstring>::iterator it=fileidList.begin(); it != fileidList.end(); it++ )
                        {
                            if (*it == albumItem->_fileId)
                            {
                                bFound = true;
                                break;
                            }
                        }
                        if (bFound)
                            break;
                        ///////////////
						albumItem->_albumId = albumData->_albumId;
						albumItem->_albumName = albumData->_albumName;
						albumData->_playItemList.push_back(albumItem);
                        fileidList.push_back(albumItem->_fileId);
					}
				}
				break;
			default:
				break;
		}
		childNode = childNode->next_sibling();
	}
}

void ND91Assistant::AndroidVideoXmlHelper::CreatePlayerAlbumXMLInfo( PlayAlbumData* album, wstring destlocalpath )
{
	RETURN_IF(!album);
	CFileOperation::MakeLocalDir(CFileOperation::GetFileDir(destlocalpath));

	xml_document<wchar_t>  doc;

	xml_node<wchar_t>*  rot = doc.allocate_node(node_pi, doc.allocate_string(L"xml version='1.0'"));

	doc.append_node(rot);
    
	xml_node<wchar_t>*  nodeAlbum = doc.allocate_node(node_element, L"album");

	doc.append_node(nodeAlbum);

	nodeAlbum->append_node(doc.allocate_node(node_element,L"albumID",  album->_albumId.c_str()));

	nodeAlbum->append_node(doc.allocate_node(node_element,L"albumName",album->_albumName.c_str()));

	nodeAlbum->append_node(doc.allocate_node(node_element,L"count", album->_count.c_str() ));

	nodeAlbum->append_node(doc.allocate_node(node_element,L"update", album->_updata.c_str()));

	wstring  albumType = CStrOperation::IntToWString(album->_type);
	nodeAlbum->append_node(doc.allocate_node(node_element, L"type", albumType.c_str()));

	nodeAlbum->append_node(doc.allocate_node(node_element, L"showingTime", album->_showingTime.c_str()));

	nodeAlbum->append_node(doc.allocate_node(node_element, L"director", album->_director.c_str()));

	nodeAlbum->append_node(doc.allocate_node(node_element, L"actor",album->_actor.c_str()));

	nodeAlbum->append_node(doc.allocate_node(node_element, L"introduce", album->_introduce.c_str()));

	nodeAlbum->append_node(doc.allocate_node(node_element, L"albumURL", album->_albumUrl.c_str()));

	deque<wstring>  wstringlist;
	for (size_t i =0; i < album->_playItemList.size(); i++)
	{
		PlayerAlbumItem* albumitem = album->_playItemList[i];
		if(albumitem->_fileId.empty())
			continue;
		xml_node<wchar_t>* nodeItem = doc.allocate_node(node_element, L"item");
		nodeAlbum->append_node(nodeItem);

		nodeItem->append_node(doc.allocate_node(node_element, L"fileID", albumitem->_fileId.c_str()));

		nodeItem->append_node(doc.allocate_node(node_element, L"fileName", albumitem->_fileDisplayName.c_str()));

		nodeItem->append_node(doc.allocate_node(node_element, L"fileUrl", albumitem->_fileUrl.c_str()));

		nodeItem->append_node(doc.allocate_node(node_element, L"filePath", albumitem->_filePath.c_str()));

		wstringlist.push_back(CStrOperation::Int64ToWString(albumitem->_fileSize));

		nodeItem->append_node(doc.allocate_node(node_element, L"fileSize", wstringlist[wstringlist.size()-1].c_str()));

		wstringlist.push_back(CStrOperation::IntToWString(albumitem->_fileOrder));
		nodeItem->append_node(doc.allocate_node(node_element, L"fileOrder", wstringlist[wstringlist.size()-1].c_str()));

		nodeItem->append_node(doc.allocate_node(node_element, L"fileType", albumitem->_fileType.c_str()));
	}

	wstring text;
	rapidxml::print(std::back_inserter(text), doc, 0);

	ofstream  outxml(destlocalpath.c_str(), ios_base::out);
	char szBOM[4] = {(char)0xEF, (char)0xBB, (char)0xBF, 0};  
	outxml<<szBOM;
	outxml<<CCodeOperation::UnicodeToUTF_8(text);
}

PlayAlbumData* ND91Assistant::AndroidVideoXmlHelper::ParsePlayerAlbumPlistXML( wstring plistxmlfile )
{
	RETURN_NULL_IF(plistxmlfile.empty() || !CFileOperation::IsFileExist(plistxmlfile.c_str()));
	string strData = CFileOperation::ReadFile(plistxmlfile, false);

	xml_document<char> doc;
	doc.parse<0>(const_cast<char *>(strData.c_str()));

	xml_node<char>*  plistNode = doc.first_node("plist");

	RETURN_NULL_IF(!plistNode);

	xml_node<char>*  node = plistNode->first_node(XPLIST_ARRAY);

	RETURN_NULL_IF(!node);
	xml_node<char>* nodeDic = node->first_node(XPLIST_DICT);
	RETURN_NULL_IF(!nodeDic)

	xml_node<char>* nodeDicItem = nodeDic->first_node();
	RETURN_NULL_IF(!nodeDicItem);

    PlayAlbumData* albumData = new PlayAlbumData();
	while(nodeDicItem)
	{
		string nodeName = CStrOperation::toLower(nodeDicItem->name());
		string keyvalue = CStrOperation::toLower(nodeDicItem->value());

		xml_node<char>* nodeItemValue = nodeDicItem->next_sibling();
		string nodeItemName = CStrOperation::toLower(nodeItemValue->name());
		wstring ItemValue = L"";
		if(nodeItemName == XPLIST_STRING)
		{
			ItemValue = CCodeOperation::UTF_8ToUnicode(nodeItemValue->value());
		}
		if(nodeName == XPLIST_KEY)
		{
			if(keyvalue == KEY_ALBUM_COUNT)
			{
				albumData->_count = ItemValue;
			}
			else if(keyvalue == KEY_ALBUM_UPDATE)
			{
				albumData->_updata = ItemValue;
			}
			else if(keyvalue == KEY_ALBUM_ID)
			{
				albumData->_albumId = ItemValue;
			}
			else if(keyvalue == KEY_ALBUM_NAME)
			{
				albumData->_albumName = ItemValue;
			}
			else if(keyvalue == KEY_ALBUM_TYPE)
			{
				albumData->_type = _wtoi(ItemValue.c_str());
			}
			else if(keyvalue == KEY_ALBUM_URL)
			{
				albumData->_albumUrl = ItemValue;
			}
			else if(keyvalue == KEY_ALBUM_INTRODUCE)
			{
				albumData->_introduce = ItemValue;
			}
		}	
		nodeDicItem = nodeItemValue->next_sibling();
	}

	return albumData;
}


PlayerAlbumItem* AndroidVideoXmlHelper::ParsePlayerAlbumItem(xml_node<char>* itemNode)
{

	RETURN_NULL_IF (!itemNode);
	xml_node<char> * item = itemNode->first_node();
	RETURN_NULL_IF (!item);
	PlayerAlbumItem* albumItem = new PlayerAlbumItem();
	while(item)
	{
		string itemname = item->name();
		string itemnameLower = CStrOperation::toLower(itemname);
		int  itemfindIndex = -1;
		for (int i =0; i < 7; i++)
		{
			if(Album_Item_Tags[i]== itemnameLower)
			{
				itemfindIndex = i;
				break;
			}
		}
		string tempvalue = item->value();
		if(tempvalue.empty())// 值不存在，或许在cdata子节点
		{
			xml_node<char> * itemcdata = item->first_node("");
			if (itemcdata && (itemcdata->type()== node_cdata || itemcdata->type()== node_data))
				tempvalue = itemcdata->value();
		}
		wstring itemvalue = CCodeOperation::UTF_8ToUnicode(tempvalue);
		switch(itemfindIndex)
		{
		case 0:
			albumItem->_fileId = itemvalue;
			break;
		case 1:
			albumItem->_fileDisplayName = itemvalue;
			break;
		case 2:
			albumItem->_fileUrl = itemvalue;
			break;
		case 3:
			albumItem->_filePath = itemvalue;
			break;
		case 4:
			albumItem->_fileSize = _wtoi64(itemvalue.c_str());
			break;
		case 5:
			albumItem->_fileOrder= _wtoi(itemvalue.c_str());
			break;
		case 6:
			albumItem->_fileType = itemvalue;
			break;
		default:
			break;
		}
		item = item->next_sibling();
	}
	return albumItem;
}


#endif