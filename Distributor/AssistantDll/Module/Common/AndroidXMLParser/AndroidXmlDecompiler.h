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
	    bool _bUTF8;							//是否是UTF8编码
	    vector<wstring>	_strings;				//字符串列表
	    vector<int>		_namespaces;			//名称空间列表
	    int				_nLastNamespaceIndex;	//已显示的_namespaces值
	    vector<int>		_resourceIDs;			//ID列表，不知道做什么用
	    wstring			_strPret;				//为了格式，放在前面的\t
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