#pragma once

#include "common/rapidxml/rapidxml.hpp"
#include "common/rapidxml/rapidxml_utils.hpp"
#include "common/rapidxml/rapidxml_print.hpp"
#include "common/rapidxml/rapidxml_iterators.hpp"
#include <string>

using namespace std;
using namespace rapidxml;

#define XML_PI			L"xml version=\"1.0\" encoding=\"UTF-8\""
#define XML_DOCTYPE		L"plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\""
#define XML_KEY			L"key"
#define XML_STRING		L"string"
#define XML_ARRAY		L"array"
#define XML_DICT		L"dict"
#define XML_FALSE		L"false"
#define XML_TRUE		L"true"
#define XML_INTEGER     L"integer"
#define XML_PLIST_Base	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
                                 <!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \
                                 \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n\0"

typedef enum eXmlType
{
	eXmlType_UTF8,
	eXmlType_GB2312,
	eXmlType_UNICODE,
	eXmlType_UNKNOW
};

//XML节点的数据
typedef struct XML_ELEMENT 
{
	wstring strName;
	wstring strValue;
	wstring strAttributeName;
	wstring strAttributeValue;

	XML_ELEMENT(wstring name = L"", wstring value = L"", wstring attributeName = L"", wstring attributeValue = L"")
	{
		strName = name;
		strValue = value;                                                                                                                                                                                                                                                                                                                                                                          
		strAttributeName = attributeName;
		strAttributeValue = attributeValue;
	}
}XML_ELEMENT;

//节点的索引，nIndex从0开始计数
typedef struct XML_NODE_INDEX
{
public:
	size_t size() { return _index.size(); }

	void AddChild(char i) { _index.push_back(i); }

	char GetChildIndex(int level)
	{
		if ( level >= (int)_index.size() )
			return 0;

		return _index[level];
	}

	void DelLastChild() 
	{
		if (size() > 0)
			_index.pop_back();
	}

	void SetChildIndex(int level, char offset)
	{
		if ( level >= (int)_index.size() )
			return;

		_index[level] = offset;
	}

private:
	//  数组下标表示层数，值表示某层第几个节点
	vector<char> _index;

}XML_NODE_INDEX;

//XML操作类，目前仅实现UTF8编码
class IosPlistHelper
{
public:
	// Summary  : 构造函数，主要用于创建XML文件
	// Returns  : 
	// Parameter: wstring strPi ：xml文件头
	// Parameter: wstring strDocType ：xml文件描述
	// Parameter: eXmlType eType ：XML文件编码类型
	// Parameter: XML_ELEMENT root ：XML的跟节点
	IosPlistHelper(wstring strPi, wstring strDocType, eXmlType eType, XML_ELEMENT root);
	// Summary  : 构造函数，主要用于修改、查询等，暂时还未用到
	// Returns  : 
	// Parameter: wstring strXmlFileName ：xml文件名（含路径）
	// Parameter: eXmlType eType ：XML文件编码类型
	IosPlistHelper(wstring strXmlFileName, eXmlType eType, bool isSpecial = false);

	IosPlistHelper(string plistcontent,  eXmlType eType);

	~IosPlistHelper(void);

	// Summary  : 添加XML节点
	// Returns  : void
	// Parameter: XML_NODE_INDEX ：父节点索引，从根节点开始
	// Parameter: XML_ELEMENT element ：节点信息
	bool AddElement(XML_NODE_INDEX index, XML_ELEMENT element);

	// Summary  : 添加XML节点
	// Returns  : void
	// Parameter: pNode ：父节点
	// Parameter: XML_ELEMENT element ：节点信息
	bool AddElement(rapidxml::xml_node<char> * pNode, XML_ELEMENT element);

	// Summary  : 将生成的XML写入文件
	// Returns  : void
	// Parameter: wstring strFileName ：欲写入的文件名（含路径）
	bool WriteToFile(wstring strFileName);

	// Summary  : 获取XML文本内容
	// Returns  : std::wstring ：文本内容（UNICODE）
	wstring GetXmlContent();

	// Summary  : 从XML中获取对应KEY的值，在IOS中node为DICT
	// Returns  : std::wstring key的值
	// Parameter: XML_NODE_INDEX index ：父节点索引，从根节点开始
	// Parameter: string strKey ：key的名称
	/************************************************************************/
	/* 如<key>CFBundleIdentifier</key>
		<string>com.91.sj.iphone.91Player</string>         
		strKey = CFBundleIdentifier, 返回com.91.sj.iphone.91Player*/
	/************************************************************************/
	wstring SearchKeyValue(XML_NODE_INDEX index, string strKey);

	// Summary  : 从XML中获取对应KEY的节点，在IOS中node为DICT
	// Returns  : std::wstring key的节点，不存在返回NULL
	// Parameter: XML_NODE_INDEX index ：父节点索引，从根节点开始
	// Parameter: string strKey ：key的名称
	/* 如<key>user</key>
		<dict>
			<key>CFBundleIdentifier</key>
			<string>com.91.sj.iphone.91Player</string>      
		</dict>
		则strKey = user, 返回dict节点*/
	/************************************************************************/
	rapidxml::xml_node<char>* SearchKeyNode(XML_NODE_INDEX index, string strKey, string strNodeType = "dict"); 

	//从XML中获取对应KEY的值，在IOS中node为DICT
	/*如：
	<dict>
	<key>CFBundleVersion</key>
	<string>1.8</string>
	<key>EnvironmentVariables</key>
	</dict>
	存储单个程序信息的节点*/
	static wstring SearchKeyFromXmlNode(rapidxml::xml_node<char>* node, string strKey);

	static rapidxml::xml_node<char>* SearchKeyNodeFromXmlNode(rapidxml::xml_node<char>* node, string strKey);

	//判断KEY是否在XML中
	static bool IsKeyExist(rapidxml::xml_node<char>* node, string strKey);

	// Summary  : 删除节点
	// Returns  : void
	// Parameter: XML_NODE_INDEX index ：欲删除节点的索引
	void DeleteNode(XML_NODE_INDEX index);

	// Summary  : 删除节点中指定KEY的节点
	// Returns  : void
	// Parameter: rapidxml::xml_node<char> * node ：欲删除的节点的父节点
	// Parameter: string strKey ：欲删除节点的KEY值
	/* 如 <dict>
			<key>user</key>
			<dict>
				<key>CFBundleIdentifier</key>
				<string>com.91.sj.iphone.91Player</string>      
			</dict>
			<key>temp</key>
			<string>tmp</string>
		</dict>
	当node为最外层的DICT,且strKey=user, 则删除后为
		<dict>
			<key>temp</key>
			<string>tmp</string>
		</dict>*/
	void DeleteNode(rapidxml::xml_node<char>* node, string strKey);

	// Summary  : 获取指定索引的节点
	// Returns  : rapidxml::xml_node<char>* ：节点信息
	// Parameter: XML_NODE_INDEX index ：指定节点的索引（非父节点）
	rapidxml::xml_node<char>* GetNodeFromIndex(XML_NODE_INDEX* index);


	rapidxml::xml_node<char>* GetFirstNode();

	rapidxml::xml_node<char>* FindFirstArrayNode();

	static void GetValuesFromNode(rapidxml::xml_node<char>* node, vector<wstring>& vtValues);

	static void GetNodesFromNode(rapidxml::xml_node<char>* node, vector<rapidxml::xml_node<char>*>& vtValues, string strNodeType = "dict");

private:
	bool AddElement_UTF8(rapidxml::xml_node<char> * pNode, XML_ELEMENT element);
	bool AddElement_UTF8(XML_NODE_INDEX* index, XML_ELEMENT element);
	xml_node<char>* GetElementNode_UTF8(XML_ELEMENT element);
	char* String2char(string str);	

private:
	xml_document<char> _doc; //xml操作类
	eXmlType _eType; //xml编码类型

	int _nRootIndex; //根节点的索引
	vector<char*> _allocBuf; //存放创建XML节点中生成的内存，以便析构中释放
	string _strData; //存放从文件读取的XML数据
};
