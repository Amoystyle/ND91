#include "pch_module.h"

#if defined( MODULE_APP )

#include "NpkData.h"

NpkData::NpkData(string strData)
{
	parseData(CCodeOperation::GB2312ToUnicode(strData));
}

NpkData::NpkData(wstring strData)
{
	parseData(strData);
}

NpkData::~NpkData()
{
	for (vector<NPK_ITEM*>::iterator it = _items.begin(); it != _items.end(); it++)
	{
		NPK_ITEM* pItem = (NPK_ITEM*)(*it);
		for (vector<NPK_CopyData*>::iterator it2 = pItem->datas.begin(); it2 != pItem->datas.end(); it2++)
		{
			NPK_CopyData* pData = (NPK_CopyData*)(*it2);
			SAFE_DELETE(pData);
		}
		pItem->datas.clear();
		SAFE_DELETE(pItem);
	}
	_items.clear();
}

void NpkData::parseData(wstring strData)
{
	xml_document<wchar_t> doc;
	doc.parse<0>(const_cast<wchar_t *>(strData.c_str()));
	xml_node<wchar_t> * root = doc.first_node(NPK_XML_ROOT);
	if (!root)
		return;
	// 多个ITEM
	xml_attribute<wchar_t> * attr;
	xml_node<wchar_t> * node;
	for (xml_node<wchar_t> * itemnode = root->first_node(NPK_XML_ITEM);  itemnode != NULL; itemnode = root->next_sibling())
	{
		NPK_ITEM* pItem = new NPK_ITEM();
		//name
		attr = itemnode->first_attribute(NPK_XML_ITEMNAME); 
		if (attr)
			pItem->Name = attr->value();
		//apk
		node = itemnode->first_node(NPK_XML_APK);
		if (node)
			pItem->apkFileName = node->value();
		//packageName
		node = itemnode->first_node(NPK_XML_PKGNAME);
		if (node)
			pItem->packageName = node->value();

		//versionName
		node = itemnode->first_node(NPK_XML_VERSIONNAME);
		if (node)
			pItem->versionName = node->value();

		// 多个data
		for (xml_node<wchar_t> * datanode = itemnode->first_node(NPK_XML_DATA);  datanode != NULL; datanode = itemnode->next_sibling())
		{
			NPK_CopyData* pData = new NPK_CopyData();

			node = datanode->first_node(NPK_XML_DATA_LOCAL);
			if (node)
				pData->localName = node->value();

			node = datanode->first_node(NPK_XML_DATA_REMOTE);
			if (node)
				pData->remotePath = node->value();

			node = datanode->first_node(NPK_XML_DATA_FOLDER);
			if (node)
				pData->bFolder = node->value() == L"1" ? true : false;

			pItem->datas.push_back(pData);
		}
		_items.push_back(pItem);
	}
}
#endif