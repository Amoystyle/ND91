#include "pch_module.h"

#ifdef MODULE_BOOKMARK

#include "IosBookmarkPlistHelper.h"
#include "BookmarkFolder.h"

IosBookmarkPlistHelper::IosBookmarkPlistHelper(void)
{
}

IosBookmarkPlistHelper::~IosBookmarkPlistHelper(void)
{
}

BookmarkFolder* ND91Assistant::IosBookmarkPlistHelper::ParseBookMark( wstring path, const DeviceData* pDevice)
{
	BookmarkFolder* pfolder = new BookmarkFolder();
	
	IosPlistHelper plistHelper(path, eXmlType_UTF8);
	XML_NODE_INDEX index;
	index.AddChild(0);
	rapidxml::xml_node<char> * itemnode = plistHelper.SearchKeyNode(index, "Children", "array");

	ParseArrayNode(itemnode, pfolder, pDevice);

	itemnode = plistHelper.SearchKeyNode(index, "WebBookmarkFileVersion", "integer");
	if (itemnode)
	{
		int version = atoi(itemnode->value());
	}
	wstring type = plistHelper.SearchKeyValue(index, "WebBookmarkType");
	pfolder->_id = CCodeOperation::UnicodeToGB2312(plistHelper.SearchKeyValue(index, "WebBookmarkUUID"));

	if(pfolder->_id.empty())
		pfolder->_id = "Root";
	return pfolder;
}

void ND91Assistant::IosBookmarkPlistHelper::SaveBookMarkToPlist( wstring path, BookmarkFolder* rootFolder )
{
	IosPlistHelper helper(XML_PI, XML_DOCTYPE, eXmlType_UTF8, XML_ELEMENT(L"plist", L"", L"version", L"1.0"));
	XML_NODE_INDEX index;
	helper.AddElement(index, XML_ELEMENT(XML_DICT));
	index.AddChild(0);
	helper.AddElement(index, XML_ELEMENT(XML_KEY, L"Children"));
	helper.AddElement(index, XML_ELEMENT(XML_ARRAY));
	helper.AddElement(index, XML_ELEMENT(XML_KEY, L"WebBookmarkFileVersion"));
	helper.AddElement(index, XML_ELEMENT(XML_INTEGER, L"1"));
	helper.AddElement(index, XML_ELEMENT(XML_KEY, L"WebBookmarkType"));
	helper.AddElement(index, XML_ELEMENT(XML_STRING, L"WebBookmarkTypeList"));
	helper.AddElement(index, XML_ELEMENT(XML_KEY, L"WebBookmarkUUID"));
	helper.AddElement(index, XML_ELEMENT(XML_STRING, L"Root"));
	index.AddChild(1);
	int level = 2;
	CreateChildrenNodes(rootFolder, index, level, helper);
	helper.WriteToFile(path);
}

void IosBookmarkPlistHelper::CreateChildrenNodes(BookmarkFolder* pfolder, XML_NODE_INDEX& index, int level, IosPlistHelper& helper)
{
	for (size_t i = 0; i < pfolder->_pVerBookmarks->size(); i++)
	{
		helper.AddElement(index, XML_ELEMENT(XML_DICT));
	}
	index.AddChild(0);
	for (size_t i = 0; i < pfolder->_pVerBookmarks->size(); i++)
	{
		index.SetChildIndex(level, i);
		BookmarkData* node = pfolder->_pVerBookmarks->at(i);
		if(node->GetType() == TypeBookMarkFolder)
		{
			helper.AddElement(index, XML_ELEMENT(XML_KEY, L"Children"));
			helper.AddElement(index, XML_ELEMENT(XML_ARRAY));
			index.AddChild(1);
			CreateChildrenNodes(dynamic_cast<BookmarkFolder*>(node), index, level+2,  helper);
			index.DelLastChild();
			index.SetChildIndex(level, i);
			helper.AddElement(index, XML_ELEMENT(XML_KEY, L"Title"));
			helper.AddElement(index, XML_ELEMENT(XML_STRING, node->_title));
		}
		else
		{
			helper.AddElement(index, XML_ELEMENT(XML_KEY, L"URIDictionary"));
			helper.AddElement(index, XML_ELEMENT(XML_DICT));
			index.AddChild(1);
			helper.AddElement(index, XML_ELEMENT(XML_KEY, L""));
			helper.AddElement(index, XML_ELEMENT(XML_STRING, node->_url));
			helper.AddElement(index, XML_ELEMENT(XML_KEY, L"title"));
			helper.AddElement(index, XML_ELEMENT(XML_STRING, node->_title));
			index.DelLastChild();
			index.SetChildIndex(level, i);
			helper.AddElement(index, XML_ELEMENT(XML_KEY, L"URLString"));
			helper.AddElement(index, XML_ELEMENT(XML_STRING, node->_url));
		}
		helper.AddElement(index, XML_ELEMENT(XML_KEY, L"WebBookmarkType"));
		helper.AddElement(index, XML_ELEMENT(XML_STRING, node->GetType() == TypeBookMarkFolder?L"WebBookmarkTypeList":L"WebBookmarkTypeLeaf"));
		helper.AddElement(index, XML_ELEMENT(XML_KEY, L"WebBookmarkUUID"));
		helper.AddElement(index, XML_ELEMENT(XML_STRING, CCodeOperation::GB2312ToUnicode(node->_id)));
	}
	index.DelLastChild();
}

void IosBookmarkPlistHelper::ParseArrayNode(rapidxml::xml_node<char>* itemnode, BookmarkFolder* pfolder, const DeviceData* pDevice)
{
	RETURN_IF(!itemnode || !pfolder);
	vector<rapidxml::xml_node<char>*>  vtValues;
	IosPlistHelper::GetNodesFromNode(itemnode, vtValues);
	for (vector<rapidxml::xml_node<char>*>::iterator it=vtValues.begin(); it != vtValues.end(); it++)
	{
		BookmarkData* node = NULL;
		wstring type = IosPlistHelper::SearchKeyFromXmlNode((*it), "WebBookmarkType");
		if(type == L"WebBookmarkTypeList")
		{
			node = new BookmarkFolder();
			rapidxml::xml_node<char>* children = IosPlistHelper::SearchKeyNodeFromXmlNode((*it), "Children");
			if(children)
				ParseArrayNode(children, dynamic_cast<BookmarkFolder*>(node), pDevice);
			node->_title = IosPlistHelper::SearchKeyFromXmlNode((*it), "Title");
		}
		else
		{
			node = new BookmarkData();
			rapidxml::xml_node<char> * dict = IosPlistHelper::SearchKeyNodeFromXmlNode((*it), "URIDictionary");
			if(dict)
				node->_title = IosPlistHelper::SearchKeyFromXmlNode(dict, "title");
			node->_url = IosPlistHelper::SearchKeyFromXmlNode((*it), "URLString");
		}
		pfolder->_id = CCodeOperation::UnicodeToGB2312(IosPlistHelper::SearchKeyFromXmlNode((*it), "WebBookmarkUUID"));
		pfolder->insterData(node);
	}
}

#endif