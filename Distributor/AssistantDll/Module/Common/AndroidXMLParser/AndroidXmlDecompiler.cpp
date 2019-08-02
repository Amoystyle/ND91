#include "stdafx.h"
#include "AndroidXmlDecompiler.h"

using namespace ND91Assistant;
CAndroidXmlDecompiler::CAndroidXmlDecompiler()
{

}
CAndroidXmlDecompiler::CAndroidXmlDecompiler(wstring strFile)
{
	_strXML = L"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n";
	_nLastNamespaceIndex = 0;
	string strData = CFileOperation::ReadFile(strFile, true);
	_pReader = new BinaryBufferReader(strData.c_str(), strData.length());

	try
	{
		ParseHead();
		ParseStringBuffers(_pReader, &_strings);
		while (ParseBody())
		{
		}
	}
	catch(...)
	{
	}

	delete _pReader;
	_pReader = NULL;
}

CAndroidXmlDecompiler::~CAndroidXmlDecompiler()
{

}

bool CAndroidXmlDecompiler::ParseHead()
{
	if (!CheckInt(_pReader, 0x00080003))
		return false;
	_pReader->AddOffset(4);
	return true;
}

int CAndroidXmlDecompiler::GetNameSpaceIndex(int nValue)
{
	// 获取Namespace值
	int nSpaceIndex = -1;
	for (size_t i=0; i+1<_namespaces.size(); i += 2)
	{
		if (_namespaces.at(i+1) == nValue)
		{
			nSpaceIndex = _namespaces.at(i);
			break;
		}
	}
	return nSpaceIndex;
}

bool CAndroidXmlDecompiler::ParseBody()
{
	int nType = 0;
	_pReader->ReadInt32(nType);
	switch(nType)
	{
	case 0x00080180:
		ParseIDs();
		break;
	case 0x00100100:// 进入命名空间
		{
			_pReader->AddOffset(12);
			_namespaces = ReadIntArray(_pReader, 2);
			_nLastNamespaceIndex = 0;
		}
		break;
	case 0x00100101:// 退出命名空间
		{
			ReadIntArray(_pReader, 5);
		}
		break;
	case 0x00100102:// 进入标签
		_strPret += L"\t";
		ParseElement();		
		break;
	case 0x00100103://退出标签
		{
			vector<int> intList = ReadIntArray(_pReader, 5);
			wstring strNameSpaceName  = intList.at(3) >= 0 ? GetString(&_strings, intList.at(3)) : L"";
			wstring strElementValue = intList.at(4) >= 0 ? GetString(&_strings, intList.at(4)) : L"";
			WriteResult(L"</" + (strNameSpaceName.length() > 0 ? strNameSpaceName + L":" + strElementValue : strElementValue) + L">");
		}
		if (_strPret.length() > 0)
			_strPret = _strPret.substr(0, _strPret.length() - 1);
		break;
	case 0x00100104:// 不知道是什么东西
		{
			ReadIntArray(_pReader, 6);
		}
		break;
	default:
		return false;
		break;
	}
	return true;
}

void CAndroidXmlDecompiler::ParseIDs()
{
	int num;
	_pReader->ReadInt32(num);
	if (num < 8 || num % 4 != 0)
	{
		#ifdef _DEBUG
		throw "Invalid resource ids size";
		#endif
	}
	_resourceIDs = ReadIntArray(_pReader, num / 4 - 2);
}

void CAndroidXmlDecompiler::ParseElement()
{
	vector<int> topList = ReadIntArray(_pReader, 8);
	wstring strElementName = GetString(&_strings, topList.at(4));
	int nAttributeNum = topList.at(6);
	WriteResult(L"<" + strElementName);
	for (size_t i=_nLastNamespaceIndex; i+1<_namespaces.size(); i += 2)
	{
		wstring strNameSpaceName  = L"xmlns:" + GetString(&_strings, _namespaces.at(i));
		wstring strNameSpaceValue = GetString(&_strings, _namespaces.at(i+1));
		WriteResult(strNameSpaceName + L"=\"" + strNameSpaceValue + L"\"");
		_nLastNamespaceIndex = i+2;
	}

	for (int i=0; i<nAttributeNum; i++)
	{
		vector<int> attrList = ReadIntArray(_pReader, 5);
		wstring strNameSpace = GetString(&_strings, GetNameSpaceIndex(attrList.at(0)));
		wstring strName = GetString(&_strings, attrList.at(1));
		// 获取值
		int nType = (attrList.at(3) >> 24);
		int nData = attrList.at(4);
		wstring strValue = GetValue(nType, nData, &_strings);
		WriteResult((strNameSpace.length() > 0 ? strNameSpace + L":" + strName : strName) + L"=\"" + strValue + L"\"");
	}
	WriteResult(L">");
}

void CAndroidXmlDecompiler::WriteResult( wstring strResult )
{
	wstring strPret = _strPret.length() > 0 ? _strPret.substr(0, _strPret.length() - 1) : L"";
	_strXML += strPret + strResult + L"\r\n";
}

void CAndroidXmlDecompiler::Save( wstring strPath )
{
	string str = CCodeOperation::UnicodeToUTF_8(_strXML);
	CFileOperation::WriteBinFile(strPath, str.c_str(), str.length());
}

std::wstring CAndroidXmlDecompiler::GetData()
{
	return _strXML;
}

