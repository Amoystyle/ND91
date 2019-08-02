#include "stdafx.h"
#include "IosPlistHelper.h"

IosPlistHelper::IosPlistHelper(wstring strPi, wstring strDocType, eXmlType eType, XML_ELEMENT root)
{
	_nRootIndex = 0;
	_eType = eType;
	_strData = "";

	switch (_eType)
	{
	case eXmlType_UTF8:
		{			
			if (strPi.length() > 0)
			{
				string strPiUTF8 = CCodeOperation::UnicodeToUTF_8(strPi);
				xml_node<char>* rot = _doc.allocate_node(rapidxml::node_pi, _doc.allocate_string(strPiUTF8.c_str()));
				_doc.append_node(rot);
				_nRootIndex++;
			}

			if (strDocType.length() > 0)
			{
				string strDocTypeUTF8 = CCodeOperation::UnicodeToUTF_8(strDocType);
				xml_node<char>* info = _doc.allocate_node(rapidxml::node_doctype, "", _doc.allocate_string(strDocTypeUTF8.c_str()));
				_doc.append_node(info);
				_nRootIndex++;
			}			

			_doc.append_node(GetElementNode_UTF8(root));
			_nRootIndex++;
		}
		break;
	default:
		break;
	}
}

IosPlistHelper::IosPlistHelper(wstring strXmlFileName, eXmlType eType, bool isSpecial)
{
	_nRootIndex = 0;
	_eType = eType;
	_strData = "";

	switch (_eType)
	{
	case eXmlType_UTF8:
		{
			_strData = CFileOperation::ReadFile(strXmlFileName, false);
            _doc.SetIsSpecial(isSpecial);
            try
            {
                _doc.parse<0>(const_cast<char *>(_strData.c_str()));
            }
            catch (...)
            {	
            }
		}
		break;
	default:
		break;
	}
}

IosPlistHelper::IosPlistHelper(string plistcontent, eXmlType eType)
{
	_nRootIndex = 0;
	_eType = eType;
	_strData = plistcontent;

	switch (_eType)
	{
	case eXmlType_UTF8:
		{
			_doc.parse<0>(const_cast<char *>(_strData.c_str()));
		}
		break;
	default:
		break;
	}
}

IosPlistHelper::~IosPlistHelper(void)
{
	vector<char*>::iterator it;
	for (it=_allocBuf.begin(); it!=_allocBuf.end(); it++)
	{
		if (*it)
			delete (*it);
		*it = NULL;
	}
	_allocBuf.clear();
}

bool IosPlistHelper::AddElement(XML_NODE_INDEX index, XML_ELEMENT element)
{
	switch (_eType)
	{
	case eXmlType_UTF8:
		return AddElement_UTF8(&index, element);
		break;
	default:
		break;
	}

	return false;
}
bool IosPlistHelper::AddElement(rapidxml::xml_node<char> * pNode, XML_ELEMENT element)
{
	switch (_eType)
	{
	case eXmlType_UTF8:
		return AddElement_UTF8(pNode, element);
		break;
	default:
		break;
	}

	return false;
}

bool IosPlistHelper::AddElement_UTF8(XML_NODE_INDEX* index, XML_ELEMENT element)
{
	rapidxml::xml_node<char> * pNode = GetNodeFromIndex(index);

	if (pNode)
	{
		pNode->append_node(GetElementNode_UTF8(element));
		return true;
	}

	return false;
}
bool IosPlistHelper::AddElement_UTF8(rapidxml::xml_node<char> * pNode, XML_ELEMENT element)
{
	//rapidxml::xml_node<char> * pNode = GetNodeFromIndex(index);

	if (pNode)
	{
		pNode->append_node(GetElementNode_UTF8(element));
		return true;
	}

	return false;
}
bool IosPlistHelper::WriteToFile(wstring strFileName)
{
	if (strFileName.length() <=0)
		return false;

	//CFileOperation::DeleteFile(strFileName);
    void* hFile = MyFileOperation::MyCreateFile(strFileName);
	if (!hFile)
		return false;


    MyFileOperation::MyCloseFile(hFile);

	string strFileNameGB2312 = CCodeOperation::UnicodeToGB2312(strFileName);
	std::ofstream out(strFileNameGB2312.c_str());
	switch (_eType)
	{
	case eXmlType_UTF8:
		{
			out<<_doc;
		}
		break;
	default:
		break;
	}		

	return true;
}

wstring IosPlistHelper::GetXmlContent()
{
	switch (_eType)
	{
	case eXmlType_UTF8:
		{
			std::string text;  
			rapidxml::print(std::back_inserter(text), _doc, 0);

			return CCodeOperation::UTF_8ToUnicode(text);
		}
		break;
	default:
		break;
	}	

	return L"";
}

xml_node<char>* IosPlistHelper::GetElementNode_UTF8(XML_ELEMENT element)
{
	string strName = CCodeOperation::UnicodeToUTF_8(element.strName);
	string strValue = CCodeOperation::UnicodeToUTF_8(element.strValue);
	string strAttributeName = CCodeOperation::UnicodeToUTF_8(element.strAttributeName);
	string strAttributeValue = CCodeOperation::UnicodeToUTF_8(element.strAttributeValue);

	xml_node<char>* pNode = _doc.allocate_node(rapidxml::node_element, String2char(strName), String2char(strValue));
	if (strAttributeName.length() > 0)
		pNode->append_attribute(_doc.allocate_attribute(String2char(strAttributeName), String2char(strAttributeValue)));

	return pNode;
}

char* IosPlistHelper::String2char(string str)
{
	char* szResult = NULL;

	int nSize = str.length();
	if (nSize > 0)
	{
		szResult = new char[nSize + 1];
		//ZeroMemory(szResult, nSize+1);
		memset(szResult, 0, nSize+1);
		memcpy(szResult, str.c_str(), nSize);
		_allocBuf.push_back(szResult);
	}

	return szResult;
}

bool IosPlistHelper::IsKeyExist(rapidxml::xml_node<char>* node, string strKey)
{
	if (!node)
		return false;

	bool bExist = false;

	for (rapidxml::xml_node<char> * itemnode = node->first_node();  itemnode != NULL; itemnode = itemnode->next_sibling())
	{
		if (itemnode->value() == strKey)
		{
			bExist = true;
			break;
		}
	}

	return bExist;
}

rapidxml::xml_node<char>* IosPlistHelper::SearchKeyNodeFromXmlNode(rapidxml::xml_node<char>* node, string strKey)
{
	if (!node)
		return NULL;

	rapidxml::xml_node<char> *pValueNode = NULL;

	for (rapidxml::xml_node<char> * itemnode = node->first_node();  itemnode != NULL; itemnode = itemnode->next_sibling())
	{
		if (itemnode->value() == strKey)
		{
			pValueNode = itemnode->next_sibling();
			break;
		}
	}

	return pValueNode;
}

wstring IosPlistHelper::SearchKeyFromXmlNode(rapidxml::xml_node<char>* node, string strKey)
{
	if (!node)
		return L"";

	wstring strKeyValue = L"";
	for (rapidxml::xml_node<char> * itemnode = node->first_node();  itemnode != NULL; itemnode = itemnode->next_sibling())
	{
		if (itemnode->value() == strKey)
		{
			rapidxml::xml_node<char> *pValueNode = itemnode->next_sibling();
			if (pValueNode)
			{
				if (strcmp(pValueNode->name(), "array") == 0)
				{
					rapidxml::xml_node<char> *pValueNodeTemp = pValueNode->first_node();
					if (pValueNodeTemp)
						strKeyValue = CCodeOperation::UTF_8ToUnicode(pValueNodeTemp->value());
				}
				else
				{
					strKeyValue = CCodeOperation::UTF_8ToUnicode(pValueNode->value());
				}
			}
			break;
		}
	}

	return strKeyValue;
}

void IosPlistHelper::GetValuesFromNode(rapidxml::xml_node<char>* node, vector<wstring>& vtValues)
{
	vtValues.clear();

	RETURN_IF(!node);

	for (rapidxml::xml_node<char> * itemnode = node->first_node();  itemnode != NULL; itemnode = itemnode->next_sibling())
	{
		if (strcmp(itemnode->name(), "string") == 0)
		{
			vtValues.push_back(CCodeOperation::UTF_8ToUnicode(string(itemnode->value())));
		}
		else if(strcmp(itemnode->name(),"integer")==0)
		{
			vtValues.push_back(CCodeOperation::UTF_8ToUnicode(string(itemnode->value())));
		}
	}
}

void IosPlistHelper::GetNodesFromNode(rapidxml::xml_node<char>* node, vector<rapidxml::xml_node<char>*>& vtValues, string strNodeType)
{
	vtValues.clear();

	RETURN_IF(!node);

	for (rapidxml::xml_node<char> * itemnode = node->first_node();  itemnode != NULL; itemnode = itemnode->next_sibling())
	{
		if (strNodeType.compare(itemnode->name()) == 0)
		{
			vtValues.push_back(itemnode);
		}
	}
}

rapidxml::xml_node<char>* IosPlistHelper::FindFirstArrayNode()
{
	rapidxml::xml_node<char> * pNode = _doc.first_node();
	int nRootIndex = _nRootIndex-1;
	for (; pNode != NULL && nRootIndex > 0; pNode = pNode->next_sibling(), nRootIndex--);
	RETURN_NULL_IF ( ! pNode );

	for (rapidxml::xml_node<char> * itemnode = pNode->first_node();  itemnode != NULL; itemnode = itemnode->next_sibling())
	{
		if (strcmp(itemnode->name(),"array") == 0)
		{
			return itemnode;
		}
	}

	return NULL;
}

rapidxml::xml_node<char>* IosPlistHelper::GetNodeFromIndex(XML_NODE_INDEX* index)
{
	rapidxml::xml_node<char> * pNode = _doc.first_node();
	int nRootIndex = _nRootIndex-1;
	for (; pNode != NULL && nRootIndex > 0; pNode = pNode->next_sibling(), nRootIndex--);
	RETURN_NULL_IF ( ! pNode );

	// 对每层
	for(size_t i = 0; i < index->size(); ++i)
	{
		pNode = pNode->first_node();

		RETURN_NULL_IF ( ! pNode );

		// 每层的第几个子节点
		int offset = index->GetChildIndex(i);
		for (int j = 0; j < offset; j++)
		{
			pNode = pNode->next_sibling();
			RETURN_NULL_IF ( ! pNode ); // 找不到了
		}
	}

	return pNode;

// 	XML_NODE_INDEX* pIndexTemp = index->pNext;
// 
// 	rapidxml::xml_node<char> * pNode = _doc.first_node();
// 	int nRootIndex = _nRootIndex-1;
// 	for (; pNode != NULL && nRootIndex > 0; pNode = pNode->next_sibling(), nRootIndex--);
// 
// 	while (pIndexTemp && pNode)
// 	{
// 		pNode = pNode->first_node();
// 
// 		int nIndex = pIndexTemp->nIndex;
// 		for (;  pNode != NULL && pNode->next_sibling() != NULL; nIndex--)
// 		{
// 			if (nIndex == 0)
// 				break;		
// 
// 			pNode = pNode->next_sibling();
// 		}
// 
// 		if (nIndex > 0)
// 			return false;
// 
// 		pIndexTemp = pIndexTemp->pNext;			
// 	}
// 
// 	return pNode;
}

rapidxml::xml_node<char>* IosPlistHelper::SearchKeyNode(XML_NODE_INDEX index, string strKey, string strNodeType)
{
	rapidxml::xml_node<char> * pNode = GetNodeFromIndex(&index);

	RETURN_NULL_IF(!pNode);

	rapidxml::xml_node<char> * pSearchNode = NULL;

	for (rapidxml::xml_node<char> * itemnode = pNode->first_node();  itemnode != NULL; itemnode = itemnode->next_sibling())
	{
		if (strKey.compare(itemnode->value()) == 0 && itemnode->next_sibling() && strNodeType.compare(itemnode->next_sibling()->name()) == 0)
		{
			pSearchNode = itemnode->next_sibling();
			break;
		}
	}

	return pSearchNode;
}


wstring IosPlistHelper::SearchKeyValue(XML_NODE_INDEX index, string strKey)
{
	wstring strKeyValue = L"";

	rapidxml::xml_node<char> * pNode = GetNodeFromIndex(&index);

	if (pNode && strcmp(pNode->name(),"dict") == 0)
	{
		for (rapidxml::xml_node<char> * itemnode = pNode->first_node();  itemnode != NULL; itemnode = itemnode->next_sibling())
		{
			if (itemnode->value() == strKey)
			{
				rapidxml::xml_node<char> *pValueNode = itemnode->next_sibling();
				if (pValueNode)
				{
					if (strcmp(pValueNode->name(), "array") == 0)
					{
						rapidxml::xml_node<char> *pValueNodeTemp = pValueNode->first_node();
						if (pValueNodeTemp)
							strKeyValue = CCodeOperation::UTF_8ToUnicode(pValueNodeTemp->value());
					}
					else
					{
						strKeyValue = CCodeOperation::UTF_8ToUnicode(pValueNode->value());
					}
				}
				break;
			}
		}
	}

	return strKeyValue;
}

void IosPlistHelper::DeleteNode(XML_NODE_INDEX index)
{
	rapidxml::xml_node<char> * pNodeDelete = GetNodeFromIndex(&index);
	if (pNodeDelete)
	{
		char lastOffset = index.GetChildIndex(index.size() - 1);
		index.DelLastChild();
		rapidxml::xml_node<char> * pNodeParent = GetNodeFromIndex(&index);
		if (pNodeParent)
			pNodeParent->remove_node(pNodeDelete);
		index.AddChild(lastOffset);
	}

// 	rapidxml::xml_node<char> * pNodeDelete = GetNodeFromIndex(&index);
// 
// 	if (pNodeDelete)
// 	{
// 		XML_NODE_INDEX parentIndex;
// 		parentIndex.nIndex = index.nIndex;
// 		XML_NODE_INDEX* pParentIndexTemp = &parentIndex;
// 		XML_NODE_INDEX* pIndexTemp = &index;
// 		while (pIndexTemp->pNext && pIndexTemp->pNext->pNext)
// 		{
// 			pParentIndexTemp->pNext = new XML_NODE_INDEX;
// 			pParentIndexTemp->pNext->nIndex = pIndexTemp->pNext->nIndex;
// 			pParentIndexTemp = pParentIndexTemp->pNext;
// 
// 			pIndexTemp = pIndexTemp->pNext;
// 		}
// 
// 		rapidxml::xml_node<char> * pNodeParent = GetNodeFromIndex(&parentIndex);
// 		if (pNodeParent)
// 			pNodeParent->remove_node(pNodeDelete);
// 	}
}

void IosPlistHelper::DeleteNode(rapidxml::xml_node<char>* node, string strKey)
{
	if (!node)
		return ;

	for (rapidxml::xml_node<char> * itemnode = node->first_node();  itemnode != NULL; itemnode = itemnode->next_sibling())
	{
		if (itemnode->value() == strKey)
		{
			rapidxml::xml_node<char> *pValueNode = itemnode->next_sibling();
			if (pValueNode)
				node->remove_node(pValueNode);
			node->remove_node(itemnode);
			break;
		}
	}
}

rapidxml::xml_node<char>* IosPlistHelper::GetFirstNode()
{
	return _doc.first_node();
}