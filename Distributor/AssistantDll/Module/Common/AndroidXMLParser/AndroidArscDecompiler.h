#pragma once
#include "Common/BinaryBufferReader.h"
#include "AndroidXMLParser.h"
using namespace std;
namespace ND91Assistant
{
    // 解析Apk包中的Arsc文件
    class CAndroidArscDecompiler : public CAndroidXMLParser
    {
    public:
        CAndroidArscDecompiler(wstring strFile);
        ~CAndroidArscDecompiler();

    protected:
        typedef struct DataHead
        {
            short	nDataType;
            short	nDataLength;
            int		nBufferSize;
            int		nOffset;
        };
        typedef struct DataEntry
        {
            DataEntry(int nID_, int nPackageName_, int nTypeName_, int nSpecName_, int nValueType_, int nValue_)
            {
                nID             = nID_;
                nPackageName    = nPackageName_;
                nTypeName       = nTypeName_;
                nSpecName       = nSpecName_;
                nValueType      = nValueType_;
                nValue          = nValue_;            
            };
            int nID;
            int nPackageName;
            int nTypeName;
            int nSpecName;
            int nValueType;
            int nValue;
        };

    protected:
	    BinaryBufferReader* _pReader;
	    DataHead _dataHead;
        vector<DataEntry*> _datas;

        vector<wstring> _packageNames;
	    vector<wstring> _tableStrings;
	    vector<wstring> _typeNames;
	    vector<wstring> _specNames;

	    int _nPackageID;
	    int _nResID;
        wstring _strPackageName;

        int _nPackageNameIndex;
        int _nTypeNameIndex;    

    protected:        
	    DataHead ParseHead();
	    bool ParsePackage(DataHead dataHead);
	    bool ParseType(DataHead dataHead);
	    bool ParseEntry(DataHead dataHead);
        void ParseValue(int nIDIndex, int nSpecNameIndex);
        vector<DataEntry*> GetDataEntry(int nID);
    public:
        //************************************
        // Summary:   根据ID获取内容
        // Returns:   vector<wstring> 可能会返回多个内容，如某个字符串的不同语言版本
        // Parameter: int nID AndroidManifest中有该值，形如 @7F06000A
        //************************************
        vector<wstring> GetSpecValues(int nID);
        wstring GetSpecValue(int nID);
        // 将 形如 @7F06000A 的ID 转为整型，不符合该格式的，则返回-1
        int GetID(wstring strID);
        // 获取疑是icon
        wstring GetIconFromStrings();
    };
}