#pragma once
#include "AssistantDll.h"
#include "Common/BinaryBufferReader.h"
#include <vector>
#include <string>
using namespace std;
namespace ND91Assistant
{    
    class CAndroidXMLParser
    {
    public:
        struct StringBufferStruct
        {
            char* pBuffer;
            int nStringBufferSize;
            bool bUTF8;
            int nStringCount;
            vector<int> nOffsetList;
            vector<int> nStyleOffset;
            StringBufferStruct()
            {
                pBuffer = NULL;
                nStringBufferSize = 0;
                nStringCount = 0;
                bUTF8 = false;
                nOffsetList.empty();
                nStyleOffset.empty();
            }
            ~StringBufferStruct()
            {
                delete pBuffer;
                nOffsetList.empty();
                nStyleOffset.empty();
            }
        };

	    CAndroidXMLParser(void);
	    ~CAndroidXMLParser(void);

    protected:
        StringBufferStruct* _pStringBuffer;

        CAndroidXMLParser::StringBufferStruct* GetStringBuffersReader(BinaryBufferReader* pReader);
        bool ParseStringBuffers(CAndroidXMLParser::StringBufferStruct* pStringBuffer, vector<wstring>* pStrings);
        wstring ParseStringBuffer(CAndroidXMLParser::StringBufferStruct* pStringBuffer, int nIndex);
	    bool ParseStringBuffers(BinaryBufferReader* pReader, vector<wstring>* pStrings);
	    bool CheckInt(BinaryBufferReader* pReader, int nValue);
	    vector<int> ReadIntArray(BinaryBufferReader* pReader, int num);
	    wstring GetString(vector<wstring>* pStrings, size_t nIndex);
	    wstring GetValue(int nType, int nData, vector<wstring>* pStrings = NULL);
    };
}