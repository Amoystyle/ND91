#include "stdafx.h"
#include "AndroidArscDecompiler.h"

CAndroidArscDecompiler::CAndroidArscDecompiler(wstring strFile)
{
	string strData = CFileOperation::ReadFile(strFile, true);
	_pReader = new BinaryBufferReader(strData.c_str(), strData.length());

	int nPackageCount; 

	bool nSign = true;
    int nCount = 0; // 防止乱文件导致的死锁
	while (nSign && nCount < 500)
	{
		_dataHead = ParseHead();
		switch (_dataHead.nDataType)
		{
		case 0x0000:
			break;
		case 0x0002:
			_pReader->ReadInt32(nPackageCount);
			ParseStringBuffers(_pReader, &_tableStrings);            
            nCount ++;
			break;
		case 0x0008:
			break;
		case 0x0200:
		    ParsePackage(_dataHead);
            nCount ++;
			break;
		case 0x0202:
			ParseType(_dataHead);
            nCount ++;
			break;
		case 0x0201:
			ParseEntry(_dataHead);
            nCount ++;
			break;
		default:
			//nSign = false;
			break;
		}
        if (_pReader->GetOffset() >= _pReader->GetSize() - 8)
            break;
	}
    SAFE_DELETE(_pReader);

}

CAndroidArscDecompiler::~CAndroidArscDecompiler()
{
    for (vector<DataEntry*>::iterator it = _datas.begin(); it != _datas.end(); it++)
	{
		SAFE_DELETE(*it);
	}
    _datas.clear();
}

CAndroidArscDecompiler::DataHead CAndroidArscDecompiler::ParseHead()
{
	DataHead dataHead;
	dataHead.nOffset = _pReader->GetOffset();
	_pReader->ReadShort(dataHead.nDataType);
	_pReader->ReadShort(dataHead.nDataLength);
    if (dataHead.nDataType == dataHead.nDataLength && (dataHead.nDataType == (short)0xFFFF || dataHead.nDataType == (short)0x0000))
        return dataHead;
	_pReader->ReadInt32(dataHead.nBufferSize);
	return dataHead;
}

bool CAndroidArscDecompiler::ParsePackage(DataHead dataHead)
{	
	_pReader->ReadInt32(_nPackageID);
	_pReader->ReadWString( _strPackageName, (dataHead.nDataLength - 12 - 16) / 2);
	_pReader->AddOffset(16);

    _packageNames.push_back(_strPackageName);
    _nPackageNameIndex = _packageNames.size() - 1;
	
	ParseStringBuffers(_pReader, &_typeNames);
	ParseStringBuffers(_pReader, &_specNames);

	return true;
}
bool CAndroidArscDecompiler::ParseType(DataHead dataHead)
{
	int nTypeIndex, nResCount;
	_pReader->ReadInt32(nTypeIndex);
	_nResID = (0xFF000000 & (_nPackageID << 24)) | ((0x000000FF & nTypeIndex) << 16);
	_pReader->ReadInt32(nResCount);
	_pReader->AddOffset(dataHead.nBufferSize - 16);
    _nTypeNameIndex = nTypeIndex;

	return true;
}

bool CAndroidArscDecompiler::ParseEntry(DataHead dataHead)
{
	int nEntryCount;	
	_pReader->AddOffset(4);
	_pReader->ReadInt32(nEntryCount);
	_pReader->AddOffset(4);

	// ReadConfig
	int nConfigLength;	
	_pReader->ReadInt32(nConfigLength);
	char *pConfigBuffer = new char[nConfigLength - 4];
	_pReader->ReadBuffer(pConfigBuffer, nConfigLength - 4);
    //LOG->WriteDebug(CCodeOperation::GB2312ToUnicode(CStrOperation::BufferToHex(pConfigBuffer, nConfigLength - 4)));
    SAFE_DELETE(pConfigBuffer);

	// ReadEntry
	vector<int> nEntryOffsets = ReadIntArray(_pReader, nEntryCount);
	int nCurOffset = _pReader->GetOffset();
	for (int i=0; i<nEntryCount; i++)
	{
		int nOffset = nEntryOffsets.at(i);
		if (nOffset == -1)
			continue;
		_pReader->SetOffset(nCurOffset + nOffset);
		short nSpecValueOption;
		int nSpecNameIndex;
		_pReader->AddOffset(2);
		_pReader->ReadShort(nSpecValueOption);
		_pReader->ReadInt32(nSpecNameIndex);
		wstring strSpeName = GetString(&_specNames, nSpecNameIndex);

		if (nSpecValueOption & 0x01)
		{			
            int i1, nValueCount;
			_pReader->ReadInt32(i1);
			_pReader->ReadInt32(nValueCount);
            for (int j=0; j<nValueCount; j++)
            {
                int i3;
                _pReader->ReadInt32(i3);
                ParseValue(i, nSpecNameIndex);
            }
		}
        else
            ParseValue(i, nSpecNameIndex);
	}
	
	return true;
}

void CAndroidArscDecompiler::ParseValue(int nIDIndex, int nSpecNameIndex)
{
    short nValueIs8;
    _pReader->ReadShort(nValueIs8); // 该值恒为8
    // 读取数据类型
    _pReader->AddOffset(1);
    byte nValueType;
    _pReader->ReadChar((char&)nValueType);
    // 读取数据值
    int nValue;
    _pReader->ReadInt32(nValue);
    int nResID = _nResID | nIDIndex;

    _datas.push_back(new DataEntry(nResID, _nPackageNameIndex, _nTypeNameIndex, nSpecNameIndex, nValueType, nValue));
}

vector<wstring> CAndroidArscDecompiler::GetSpecValues( int nID )
{
    vector<wstring> datas;
    if (nID == -1)
        return datas;
    for (vector<DataEntry*>::iterator it = _datas.begin(); it != _datas.end(); it++)
    {
        if (nID == (*it)->nID)
            datas.push_back( GetValue((*it)->nValueType, (*it)->nValue, &_tableStrings) );
    }
    return datas;
}

wstring CAndroidArscDecompiler::GetSpecValue( int nID )
{
    if (nID == -1)
        return L"";
    for (vector<DataEntry*>::iterator it = _datas.begin(); it != _datas.end(); it++)
    {
        if (nID == (*it)->nID)
            return GetValue((*it)->nValueType, (*it)->nValue, &_tableStrings);
    }
    return L"";
}

int CAndroidArscDecompiler::GetID( wstring strID )
{
    if (strID.length() != 9)
        return -1;
    return CStrOperation::Hex2Int(CCodeOperation::UnicodeToGB2312(strID.substr(1, 8)));
}

wstring ND91Assistant::CAndroidArscDecompiler::GetIconFromStrings()
{
    wstring strIcon = L"";
    for (vector<wstring>::iterator it = _tableStrings.begin(); it != _tableStrings.end(); it++)
    {
        wstring str = CStrOperation::toLowerW(*it);
        if ((int)str.find(L"res") == 0 &&  (int)str.find(L"icon") > 0 && CFileOperation::GetFileExt(str) == L"png")
        {
            strIcon = str;
        }
        if ((int)str.find(L"res") == 0 && ( (int)str.find(L"/icon.png") > 0||(int)str.find(L"/ic_launcher.png")>0) && CFileOperation::GetFileExt(str) == L"png")
        {
            strIcon = str;
            break;
        }
    }
    return strIcon;
}
