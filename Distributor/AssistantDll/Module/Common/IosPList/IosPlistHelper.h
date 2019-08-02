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

//XML�ڵ������
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

//�ڵ��������nIndex��0��ʼ����
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
	//  �����±��ʾ������ֵ��ʾĳ��ڼ����ڵ�
	vector<char> _index;

}XML_NODE_INDEX;

//XML�����࣬Ŀǰ��ʵ��UTF8����
class IosPlistHelper
{
public:
	// Summary  : ���캯������Ҫ���ڴ���XML�ļ�
	// Returns  : 
	// Parameter: wstring strPi ��xml�ļ�ͷ
	// Parameter: wstring strDocType ��xml�ļ�����
	// Parameter: eXmlType eType ��XML�ļ���������
	// Parameter: XML_ELEMENT root ��XML�ĸ��ڵ�
	IosPlistHelper(wstring strPi, wstring strDocType, eXmlType eType, XML_ELEMENT root);
	// Summary  : ���캯������Ҫ�����޸ġ���ѯ�ȣ���ʱ��δ�õ�
	// Returns  : 
	// Parameter: wstring strXmlFileName ��xml�ļ�������·����
	// Parameter: eXmlType eType ��XML�ļ���������
	IosPlistHelper(wstring strXmlFileName, eXmlType eType, bool isSpecial = false);

	IosPlistHelper(string plistcontent,  eXmlType eType);

	~IosPlistHelper(void);

	// Summary  : ���XML�ڵ�
	// Returns  : void
	// Parameter: XML_NODE_INDEX �����ڵ��������Ӹ��ڵ㿪ʼ
	// Parameter: XML_ELEMENT element ���ڵ���Ϣ
	bool AddElement(XML_NODE_INDEX index, XML_ELEMENT element);

	// Summary  : ���XML�ڵ�
	// Returns  : void
	// Parameter: pNode �����ڵ�
	// Parameter: XML_ELEMENT element ���ڵ���Ϣ
	bool AddElement(rapidxml::xml_node<char> * pNode, XML_ELEMENT element);

	// Summary  : �����ɵ�XMLд���ļ�
	// Returns  : void
	// Parameter: wstring strFileName ����д����ļ�������·����
	bool WriteToFile(wstring strFileName);

	// Summary  : ��ȡXML�ı�����
	// Returns  : std::wstring ���ı����ݣ�UNICODE��
	wstring GetXmlContent();

	// Summary  : ��XML�л�ȡ��ӦKEY��ֵ����IOS��nodeΪDICT
	// Returns  : std::wstring key��ֵ
	// Parameter: XML_NODE_INDEX index �����ڵ��������Ӹ��ڵ㿪ʼ
	// Parameter: string strKey ��key������
	/************************************************************************/
	/* ��<key>CFBundleIdentifier</key>
		<string>com.91.sj.iphone.91Player</string>         
		strKey = CFBundleIdentifier, ����com.91.sj.iphone.91Player*/
	/************************************************************************/
	wstring SearchKeyValue(XML_NODE_INDEX index, string strKey);

	// Summary  : ��XML�л�ȡ��ӦKEY�Ľڵ㣬��IOS��nodeΪDICT
	// Returns  : std::wstring key�Ľڵ㣬�����ڷ���NULL
	// Parameter: XML_NODE_INDEX index �����ڵ��������Ӹ��ڵ㿪ʼ
	// Parameter: string strKey ��key������
	/* ��<key>user</key>
		<dict>
			<key>CFBundleIdentifier</key>
			<string>com.91.sj.iphone.91Player</string>      
		</dict>
		��strKey = user, ����dict�ڵ�*/
	/************************************************************************/
	rapidxml::xml_node<char>* SearchKeyNode(XML_NODE_INDEX index, string strKey, string strNodeType = "dict"); 

	//��XML�л�ȡ��ӦKEY��ֵ����IOS��nodeΪDICT
	/*�磺
	<dict>
	<key>CFBundleVersion</key>
	<string>1.8</string>
	<key>EnvironmentVariables</key>
	</dict>
	�洢����������Ϣ�Ľڵ�*/
	static wstring SearchKeyFromXmlNode(rapidxml::xml_node<char>* node, string strKey);

	static rapidxml::xml_node<char>* SearchKeyNodeFromXmlNode(rapidxml::xml_node<char>* node, string strKey);

	//�ж�KEY�Ƿ���XML��
	static bool IsKeyExist(rapidxml::xml_node<char>* node, string strKey);

	// Summary  : ɾ���ڵ�
	// Returns  : void
	// Parameter: XML_NODE_INDEX index ����ɾ���ڵ������
	void DeleteNode(XML_NODE_INDEX index);

	// Summary  : ɾ���ڵ���ָ��KEY�Ľڵ�
	// Returns  : void
	// Parameter: rapidxml::xml_node<char> * node ����ɾ���Ľڵ�ĸ��ڵ�
	// Parameter: string strKey ����ɾ���ڵ��KEYֵ
	/* �� <dict>
			<key>user</key>
			<dict>
				<key>CFBundleIdentifier</key>
				<string>com.91.sj.iphone.91Player</string>      
			</dict>
			<key>temp</key>
			<string>tmp</string>
		</dict>
	��nodeΪ������DICT,��strKey=user, ��ɾ����Ϊ
		<dict>
			<key>temp</key>
			<string>tmp</string>
		</dict>*/
	void DeleteNode(rapidxml::xml_node<char>* node, string strKey);

	// Summary  : ��ȡָ�������Ľڵ�
	// Returns  : rapidxml::xml_node<char>* ���ڵ���Ϣ
	// Parameter: XML_NODE_INDEX index ��ָ���ڵ���������Ǹ��ڵ㣩
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
	xml_document<char> _doc; //xml������
	eXmlType _eType; //xml��������

	int _nRootIndex; //���ڵ������
	vector<char*> _allocBuf; //��Ŵ���XML�ڵ������ɵ��ڴ棬�Ա��������ͷ�
	string _strData; //��Ŵ��ļ���ȡ��XML����
};
