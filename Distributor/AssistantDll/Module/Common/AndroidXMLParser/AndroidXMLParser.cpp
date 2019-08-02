#include "stdafx.h"
#include "AndroidXMLParser.h"

CAndroidXMLParser::CAndroidXMLParser(void)
{
    _pStringBuffer = NULL;
}

CAndroidXMLParser::~CAndroidXMLParser(void)
{

}

CAndroidXMLParser::StringBufferStruct* CAndroidXMLParser::GetStringBuffersReader(BinaryBufferReader* pReader)
{
    int nBufferSize, nStringCount, nStyleCount, nOption, n, i1, nStringBufferSize;
    pReader->ReadInt32(nBufferSize);		//数据区块大小
    pReader->ReadInt32(nStringCount);	//Offset偏移量列表
    pReader->ReadInt32(nStyleCount);		//Style 属性偏移列表
    pReader->ReadInt32(nOption);
    pReader->ReadInt32(n);
    pReader->ReadInt32(i1);

    nStringBufferSize = (i1 == 0 ? nBufferSize : i1) - n;
    if (nStringBufferSize % 4 != 0)
        return NULL;

    bool bUTF8 = ((nOption & 0x100) != 0);

    vector<int> nOffsetList = ReadIntArray(pReader, nStringCount);
    vector<int> nStyleOffset = ReadIntArray(pReader, nStyleCount);

    // 读取数据区
    char* pBuffer = new char[nStringBufferSize];
    if (!pReader->ReadBuffer(pBuffer, nStringBufferSize))
        return NULL;
    StringBufferStruct* pStringBuffer = new StringBufferStruct();
    pStringBuffer->pBuffer = pBuffer;    
    pStringBuffer->nStringBufferSize = nStringBufferSize;
    pStringBuffer->bUTF8 = bUTF8;
    pStringBuffer->nStringCount = nStringCount;
    pStringBuffer->nOffsetList = nOffsetList;
    pStringBuffer->nStyleOffset = nStyleOffset;
    return pStringBuffer;
}

bool CAndroidXMLParser::ParseStringBuffers(CAndroidXMLParser::StringBufferStruct* pStringBuffer, vector<wstring>* pStrings)
{
    if (!pStringBuffer) return false;
    BinaryBufferReader reader(pStringBuffer->pBuffer, pStringBuffer->nStringBufferSize);
    unsigned short s;
    short sTemp;
    for (int i=0; i<pStringBuffer->nStringCount; i++)
    {        
        reader.SetOffset(pStringBuffer->nOffsetList.at(i));
        reader.ReadShort(sTemp);
        s = sTemp;
        if (s > 0xff && pStringBuffer->bUTF8)
            s = s >> 8;
        wstring wstr;
        if (pStringBuffer->bUTF8)
        {
            string str;
            reader.ReadString(str, s);
            wstr = CCodeOperation::UTF_8ToUnicode(str);
        }
        else
        {
            reader.ReadWString( wstr, s);
        }
        pStrings->push_back(wstr); 
    }
    return true;  
}

std::wstring ND91Assistant::CAndroidXMLParser::ParseStringBuffer( CAndroidXMLParser::StringBufferStruct* pStringBuffer, int nIndex )
{
    if (!pStringBuffer || nIndex >= pStringBuffer->nStringCount || nIndex < 0) return L"";

    BinaryBufferReader reader(pStringBuffer->pBuffer, pStringBuffer->nStringBufferSize);
    unsigned short s;
    short sTemp;
    reader.SetOffset(pStringBuffer->nOffsetList.at(nIndex));
    reader.ReadShort(sTemp);
    s = sTemp;
    if (s > 0xff && pStringBuffer->bUTF8)
        s = s >> 8;
    wstring wstr;
    if (pStringBuffer->bUTF8)
    {
        string str;
        reader.ReadString(str, s);
        wstr = CCodeOperation::UTF_8ToUnicode(str);
    }
    else
    {
        reader.ReadWString( wstr, s);
    }
    return wstr;
}

bool CAndroidXMLParser::ParseStringBuffers( BinaryBufferReader* pReader, vector<wstring>* pStrings)
{
    if (!CheckInt(pReader, 0x001C0001))
        return NULL;
    StringBufferStruct* pStringBuffer = GetStringBuffersReader(pReader);
//     _pStringBuffer = pStringBuffer;
//     return true;
    return ParseStringBuffers(pStringBuffer, pStrings);
}

/*
bool CAndroidXMLParser::ParseStringBuffers( BinaryBufferReader* pReader, vector<wstring>* pStrings)
{
	if (!CheckInt(pReader, 0x001C0001))
		return false;
	int nBufferSize, nStringCount, nStyleCount, nOption, n, i1, nStringBufferSize;
	pReader->ReadInt32(nBufferSize);		//数据区块大小
	pReader->ReadInt32(nStringCount);	//Offset偏移量列表
	pReader->ReadInt32(nStyleCount);		//Style 属性偏移列表
	pReader->ReadInt32(nOption);
	pReader->ReadInt32(n);
	pReader->ReadInt32(i1);

	nStringBufferSize = (i1 == 0 ? nBufferSize : i1) - n;
	if (nStringBufferSize % 4 != 0)
		return false;

	bool bUTF8 = ((nOption & 0x100) != 0);

	vector<int> nOffsetList = ReadIntArray(pReader, nStringCount);
	vector<int> nStyleOffset = ReadIntArray(pReader, nStyleCount);

	// 读取数据区
	char* pBuffer = new char[nStringBufferSize];
	if (!pReader->ReadBuffer(pBuffer, nStringBufferSize))
		return false;
	BinaryBufferReader reader(pBuffer, nStringBufferSize);
	for (int i=0; i<nStringCount; i++)
	{
        wstring str = L"CAndroidXMLParser ";
        LOG->WriteDebug(str + CStrOperation::Int64ToWString(i) + L" - " + CStrOperation::Int64ToWString(nStringCount));
        if (i == 328)
        {
            int a = 1;
            a ++;
        }
		reader.SetOffset(nOffsetList.at(i));
		unsigned short s;
        short sTemp;
		reader.ReadShort(sTemp);
        s = sTemp;
        if (s > 0xff && bUTF8)
            s = s >> 8;
		wstring wstr;
		if (bUTF8)
		{
			string str;
			reader.ReadString(str, s);
			wstr = CCodeOperation::UTF_8ToUnicode(str);
		}
		else
		{
			reader.ReadWString( wstr, s);
		}
		reader.ReadShort(sTemp);	// 去掉一个结尾 00 00
		pStrings->push_back(wstr);
	}
	delete pBuffer;
	return true;
}
*/
bool CAndroidXMLParser::CheckInt( BinaryBufferReader* pReader, int nValue )
{
	int n;
	pReader->ReadInt32(n);
	if (n != nValue)
		return false;
	return true;
}

vector<int> CAndroidXMLParser::ReadIntArray( BinaryBufferReader* pReader, int num)
{
	vector<int> nList;
	int n;
	for (int i=0; i<num; i++)
	{
		pReader->ReadInt32(n);
		nList.push_back(n);
	}
	return nList;
}

std::wstring CAndroidXMLParser::GetString( vector<wstring>* pStrings, size_t nIndex )
{
	if (!pStrings)
		return L"";
    if (pStrings->size() == 0 && _pStringBuffer)
    {
        return ParseStringBuffer(_pStringBuffer, nIndex);
    }
    if (nIndex >= pStrings->size())
        return L"";
	return pStrings->at(nIndex);
}

wstring CAndroidXMLParser::GetValue(int nType, int nData, vector<wstring>* pStrings)
{
	char pBuffer[1024];
	string strPackage = ((nData >> 24) == 1 ? "android" : "");
	wstring strValue;
	string strTemp;
	switch(nType)
	{
	case 0x01:
		//sprintf_s(pBuffer, 1024, "@%s%.8X", strPackage.c_str(), nData);
		sprintf(pBuffer, "@%s%.8X", strPackage.c_str(), nData);
		strValue = CCodeOperation::GB2312ToUnicode(string(pBuffer, strlen(pBuffer)));
		break;
	case 0x02:
		//sprintf_s(pBuffer, 1024, "?%s%.8X", strPackage.c_str(), nData);
		sprintf(pBuffer, "?%s%.8X", strPackage.c_str(), nData);
		strValue = CCodeOperation::GB2312ToUnicode(string(pBuffer, strlen(pBuffer)));
		break;
	case 0x03:
		strValue = GetString(pStrings, nData);
		break;
	case 0x04:
	case 0x10:
		//sprintf_s(pBuffer, 1024, "%d", nData);
		sprintf(pBuffer, "%d", nData);
		strValue = CCodeOperation::GB2312ToUnicode(string(pBuffer, strlen(pBuffer)));
		break;
	case 0x05:
	case 0x06:
		strValue = L"0";
		break;
	case 0x11:
		//sprintf_s(pBuffer, 1024, "0x%.8X", nData);
		sprintf(pBuffer, "0x%.8X", nData);
		strValue = CCodeOperation::GB2312ToUnicode(string(pBuffer, strlen(pBuffer)));
		break;
	case 0x12:
		strValue = nData == 0 ? L"false" : L"true";
		break;
	case 0x1C:
		//sprintf_s(pBuffer, 1024, "#%.8X", nData);
		sprintf(pBuffer, "#%.8X", nData);
		strValue = CCodeOperation::GB2312ToUnicode(string(pBuffer, strlen(pBuffer)));
		break;
	default:
		break;
	}
	return strValue;
}