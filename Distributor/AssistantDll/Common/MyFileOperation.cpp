#include "stdafx.h"
#include "Common/MyFileOperation.h"
#include "Common/NDZlib.h"
#include "Common/w3c.h"
#include "Common/md5.h"
#include "Common/Version.h"

#ifdef WIN32
    #include <shlobj.h>
    #include <shellapi.h>
#else
    #include <uuid/uuid.h>
    #include <dlfcn.h>
    #include <dirent.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
#endif

using namespace std; 
#pragma warning (disable:4996)


wstring MyFileOperation::GetRegValue(/*HKEY*/struct HKEY__* hTopkey, /*TCHAR*/wchar_t* szSubKey, /*TCHAR*/wchar_t* szValueName)
{
    /*HKEY*/struct HKEY__* hkey;
    /*LONG*/long ret = 0;

    ret = RegOpenKeyEx(hTopkey, szSubKey, 0, KEY_QUERY_VALUE, &hkey);
    RETURN_VALUE_IF (ERROR_SUCCESS != ret, L"");

    /*TCHAR*/wchar_t szData[MAX_PATH] = {0};
    DWORD dwAidSize = MAX_PATH;
    DWORD type = REG_SZ;
    ret = RegQueryValueEx(hkey, szValueName, NULL, &type, (LPBYTE)szData, &dwAidSize);
    RETURN_VALUE_IF (ERROR_SUCCESS != ret, L"");

    int nData = _tcslen(szData);
    RETURN_VALUE_IF ( nData <= 0, L"");

    /*TCHAR*/wchar_t* pResult = NULL;
    pResult = new /*TCHAR*/wchar_t[nData + 1];
    ZeroMemory(pResult, sizeof(/*TCHAR*/wchar_t)*(nData + 1));
    _tcscpy_s(pResult, nData+1, szData);

    RegCloseKey(hkey);

    wstring s(pResult);
    SAFE_DELETE_ARRAY(pResult);
    return s;
}

std::wstring MyFileOperation::GetIniFileValue( wstring strIniFile, wstring strGroup, wstring strKey )
{
    TCHAR buf[1048];
    GetPrivateProfileStringW(strGroup.c_str(), strKey.c_str(), _T(""), buf, 1048, strIniFile.c_str());
    wstring strValue(buf);
    return strValue;
}

bool MyFileOperation::SetIniFileValue( wstring strIniFile, wstring strGroup, wstring strKey, wstring strValue )
{
    return (WritePrivateProfileStringW(strGroup.c_str(), strKey.c_str(), strValue.c_str(), strIniFile.c_str()) == TRUE);
}

std::wstring MyFileOperation::GetWindowsDir()
{
    wchar_t szPath[MAX_PATH];
    GetWindowsDirectoryW(szPath, MAX_PATH);
    return wstring (szPath);
}

std::wstring MyFileOperation::GetSystemPath( int nFolder )
{
    wchar_t buf[256] = {0};
    LPITEMIDLIST pid;
    ::SHGetSpecialFolderLocation(NULL, nFolder, &pid);
    ::SHGetPathFromIDList(pid, buf);
    return wstring(buf);
}

std::wstring MyFileOperation::GetMyDocumentPath()
{
    return GetSystemPath(CSIDL_PERSONAL);
}

wstring MyFileOperation::QueryValue(LPVOID pInfoBuf, wstring lang_codepage, wstring name, LPVOID pInfoVal)
{
    wchar_t tempVal[512] = {0};
    unsigned int dwInfoValSize = 0;
    wstring strRes;
    wstring ver = L"\\StringFileInfo\\"+lang_codepage+L"\\"+name;
    BOOL bRes = VerQueryValue(pInfoBuf, ver.c_str(), &pInfoVal, &dwInfoValSize);
    if (bRes)
    {
        wsprintf(tempVal, L"%s", pInfoVal);
        strRes = tempVal;
    }
    return strRes;
}

FileVersionInfo* MyFileOperation::GetExeFileInfo(wstring strPath)
{
    //首先获得版本信息资源的长度
    DWORD dwHandle = 0;
    DWORD infoSize = GetFileVersionInfoSize(strPath.c_str(), &dwHandle);

    //将版本信息资源读入缓冲区
    LPVOID pInfoBuf = malloc(infoSize);
    GetFileVersionInfo(strPath.c_str(), 0, infoSize, pInfoBuf);

    //获得生成文件使用的代码页及字符集信息
    LPVOID pInfoVal = malloc(512);
    unsigned int dwInfoValSize = 0;
    VerQueryValue(pInfoBuf, L"\\VarFileInfo\\Translation", &pInfoVal, &dwInfoValSize);

    char* pTempInfoVal = (char*)pInfoVal;
    wstring lang_codepage 
        = CCodeOperation::UTF_8ToUnicode(CStrOperation::Int2Hex(*((unsigned short int *)pTempInfoVal)))
        +CCodeOperation::UTF_8ToUnicode(CStrOperation::Int2Hex(*((unsigned short int *)&pTempInfoVal[2])));

    FileVersionInfo* pVersionInfo = new FileVersionInfo();

    // 获得具体的版本号
    pVersionInfo->_fileVersion 
        = QueryValue(pInfoBuf, lang_codepage, L"FileVersion", pInfoVal);

    // 获得具体的公司名称
    pVersionInfo->_companyName 
        = QueryValue(pInfoBuf, lang_codepage, L"CompanyName", pInfoVal);

    // 获得具体的文件描述
    pVersionInfo->_fileDescription 
        = QueryValue(pInfoBuf, lang_codepage, L"FileDescription", pInfoVal);

    // 获得具体的内部名称
    pVersionInfo->_internalName 
        = QueryValue(pInfoBuf, lang_codepage, L"InternalName", pInfoVal);

    // 获得具体的合法版权
    pVersionInfo->_legalCopyright 
        = QueryValue(pInfoBuf, lang_codepage, L"LegalCopyright", pInfoVal);

    // 获得具体的原始文件名
    pVersionInfo->_originalFilename 
        = QueryValue(pInfoBuf, lang_codepage, L"OriginalFilename", pInfoVal);

    // 获得具体的产品名称
    pVersionInfo->_productName 
        = QueryValue(pInfoBuf, lang_codepage, L"ProductName", pInfoVal);

    // 获得具体的产品版本
    pVersionInfo->_productVersion 
        = QueryValue(pInfoBuf, lang_codepage, L"ProductVersion", pInfoVal);

    // 获得具体的商标名
    pVersionInfo->_legalTrademarks 
        = QueryValue(pInfoBuf, lang_codepage, L"LegalTrademarks", pInfoVal);

    // 获得注释
    pVersionInfo->_comments 
        = QueryValue(pInfoBuf, lang_codepage, L"Comments", pInfoVal);

    // 获得PrivateBuild
    pVersionInfo->_privateBuild 
        = QueryValue(pInfoBuf, lang_codepage, L"PrivateBuild", pInfoVal);

    // 获得SpecialBuild
    pVersionInfo->_specialBuild 
        = QueryValue(pInfoBuf, lang_codepage, L"SpecialBuild", pInfoVal);

    free(pInfoBuf);
    return pVersionInfo;
}

std::wstring MyFileOperation::ConvertMMSItemFolder(int messageId, int groupId)
{
    wstring foldername = CCodeOperation::UTF_8ToUnicode(CStrOperation::Int2Hex(groupId));
    if(foldername.length()<2)
        foldername = L"0"+foldername;
    else
        foldername = foldername.substr(foldername.length()-2,2);
    wstring msgidstr = CStrOperation::IntToWString(messageId%16);
    if(msgidstr.length()<2)
        msgidstr = L"0"+msgidstr;
#ifdef WIN32
    foldername += L"\\"+msgidstr+L"\\";
#else
    foldername += L"/"+msgidstr+L"/";
#endif
    return foldername;
}


void* MyFileOperation::MyCreateFile(wstring dest)
{
#ifdef WIN32
    return (void*)::CreateFile(dest.c_str(), GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
#else
    return (void*)fopen(CCodeOperation::UnicodeToUTF_8(dest).c_str(), "w+b");
#endif
}

void MyFileOperation::MyWriteFile(void* hFile, string buffer, unsigned long nWrite)
{
#ifdef WIN32
    ::WriteFile((HANDLE)hFile, buffer.c_str(), buffer.length(), &nWrite, NULL);
#else
    nWrite = fwrite(buffer.c_str(), buffer.length(), 1, (FILE*)hFile);
    fflush((FILE*)hFile);
#endif
}

void MyFileOperation::MyCloseFile(void* hFile)
{
#ifdef WIN32
    CloseHandle((HANDLE)hFile);	
#else
    fclose((FILE*)hFile);
#endif
}

bool MyFileOperation::IsIosPlist( wstring strPcPath )
{
	char tag[9] = {0};
	std::string strtag; 
	fstream fs(strPcPath.c_str(), ios::in|ios::out|ios::binary);
	fs.read(tag, 8);
	fs.close();
	strtag = tag;

	if ( "bplist00" == strtag )
		return true;
	else
		return false;
}


const char* PHOTO_SUPPORT_EXT[]={ "jpg" , "jpeg" , "png" , "gif" , "bmp", "wbmp" };
const char* PHOTO_SUPPORT_MIME[]={ "image/jpeg" , "image/png", "image/gif" , "image/x-ms-bmp", "image/vnd.wap.wbmp"};

string MyFileOperation::GetImageFileMime( wstring path )
{
    int pos = path.find_last_of(L".");
    string ext = "";
    if(pos>=0)
        ext =CCodeOperation::UnicodeToUTF_8(path.substr(pos+1));
    if(ext.empty())
        return "";
    for(int i=0; i<6; i++)
    {
        if(ext == PHOTO_SUPPORT_EXT[i])
        {
            string result = "";
            if (i==0 || i==1)
                result = PHOTO_SUPPORT_MIME[0];
            else
                result = PHOTO_SUPPORT_MIME[i-1];
            return result;
        }
    }
    return "";
}

bool MyFileOperation::IsSupportImageFile( wstring filepath )
{
    int pos = filepath.find_last_of(L".");
    string ext = "";
    if(pos>=0)
        ext =CCodeOperation::UnicodeToUTF_8(filepath.substr(pos+1));
    if(ext.empty())
        return false;

    // 转小写
    ext = CStrOperation::toLower(ext);
    for(int i=0; i<6; i++)
    {
        if(ext == PHOTO_SUPPORT_EXT[i])
            return true;
    }
    return false;

}
