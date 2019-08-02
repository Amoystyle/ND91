#include <stdafx.h>

#include "Version.h"
#include "Winver.h"
#include "Module/AndroidDefine.h"

CVersion::CVersion()
{
	_nVer1 = 0;
	_nVer2 = 0;
	_nVer3 = 0;
	_nVer4 = 0;
}

CVersion::CVersion(string strVer)
{
    _nVer1 = 0;
    _nVer2 = 0;
    _nVer3 = 0;
    _nVer4 = 0;
    vector<string> verList = CStrOperation::parseStrings(strVer.c_str(), strVer.length(), '.');    
    if (verList.size() > 0)
        _nVer1 = atoi(verList[0].c_str());
    if (verList.size() > 1)
        _nVer2 = atoi(verList[1].c_str());
    if (verList.size() > 2)
        _nVer3 = atoi(verList[2].c_str());
    if (verList.size() > 3)
        _nVer4 = atoi(verList[3].c_str());
}
CVersion::CVersion(int maTemp, int mjTemp,int rTemp, int bTemp)
{
	_nVer1 = maTemp;
	_nVer2 = mjTemp;
	_nVer3 = rTemp;
	_nVer4 = bTemp;
}

CVersion::~CVersion(void)
{
}

int CVersion::CompareTo(CVersion* pVer)
{
     int n1 = _nVer1 == pVer->_nVer1 ? 0 : _nVer1 > pVer->_nVer1 ? 1 : -1;
     int n2 = _nVer2 == pVer->_nVer2 ? 0 : _nVer2 > pVer->_nVer2 ? 1 : -1;
     int n3 = _nVer3 == pVer->_nVer3 ? 0 : _nVer3 > pVer->_nVer3 ? 1 : -1;
     int n4 = _nVer4 == pVer->_nVer4 ? 0 : _nVer4 > pVer->_nVer4 ? 1 : -1;

     return n1 * 1000 + n2 * 100 + n3 * 10 + n4;
}

std::wstring CVersion::ToStringW()
{
	wstringstream wss;
	wss << _nVer1 << L"." << _nVer2 << L"." << _nVer3 << L"." << _nVer4;
	return wss.str();
}

CVersion CVersion::GetFileVersion( const wchar_t* szFileFullName )
{
    CVersion ver;
    VS_FIXEDFILEINFO *VInfo;
    UINT i;   
    char *buf;
    i = GetFileVersionInfoSize(szFileFullName, 0);

    buf=   (char*)malloc(i);   

    if (GetFileVersionInfo(szFileFullName, 0, i, buf) != 0)
    { 
        if (VerQueryValue(buf, _T("\\"),(LPVOID*)&VInfo,&i))
        {   
            ver._nVer1 = VInfo->dwFileVersionMS >> 16;
            ver._nVer2 = VInfo->dwFileVersionMS & 0x00ff;
            ver._nVer3  = VInfo->dwFileVersionLS >> 16;
            ver._nVer4  = VInfo->dwFileVersionLS & 0x00ff;
        }   
    }    
    free(buf);
    buf = NULL;
    return ver;
}

bool CVersion::NeedUpdateAndroidDeamon()
{
    //如替换守护，须更新ANDROID_DEAMON_REQUIRE_VERSION，这样就不用解析守护包了
    CVersion requireVersion(ANDROID_DEAMON_REQUIRE_VERSION);

    // 如果大版本号不一样，不管升级降级都要替换
    // 如果大版本号一样，再根据小版本号判断是否需要升级
	//王维要求改成版本号不一样马上替换
    return _nVer1 < requireVersion._nVer1
        || _nVer2 < requireVersion._nVer2
        || _nVer3 < requireVersion._nVer3;
}
