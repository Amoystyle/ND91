#pragma once
#include "Common/BinaryBufferReader.h"
#include "AndroidXMLParser.h"
using namespace std;
namespace ND91Assistant
{
    class CAndroidXmlDecompiler : public CAndroidXMLParser
    {
    public:
	    CAndroidXmlDecompiler();
	    CAndroidXmlDecompiler(wstring strFile);
	    ~CAndroidXmlDecompiler();

    protected:
	    BinaryBufferReader* _pReader;

	    bool _success;
	    bool _bUTF8;							//�Ƿ���UTF8����
	    vector<wstring>	_strings;				//�ַ����б�
	    vector<int>		_namespaces;			//���ƿռ��б�
	    int				_nLastNamespaceIndex;	//����ʾ��_namespacesֵ
	    vector<int>		_resourceIDs;			//ID�б���֪����ʲô��
	    wstring			_strPret;				//Ϊ�˸�ʽ������ǰ���\t
	    wstring			_strXML;
    protected:
	    //wstring GetValue(int nType, int nData);
	    int GetNameSpaceIndex(int nValue);

	    bool ParseHead();
	    //bool ParseStrings();
	    bool ParseBody();
	    void ParseElement();
	    void ParseIDs();

	    void WriteResult(wstring strResult);

    public:	
	    void Save(wstring strPath);
	    wstring GetData();
    };
}