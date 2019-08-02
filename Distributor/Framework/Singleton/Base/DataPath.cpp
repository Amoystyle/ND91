#include "stdafx.h"
#include "DataPath.h"

#define BUFSIZE 512

std::wstring DataPath::_dataPath;
std::wstring DataPath::_tempPath;

std::wstring DataPath::GetDataDir()
{
    if( ! _dataPath.empty())
        return _dataPath;

    TCHAR szPath[MAX_PATH];
    HRESULT ret = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, szPath);
    if ( SUCCEEDED(ret) )
    {
        _dataPath.assign(szPath);
        _dataPath.push_back('\\');
        _dataPath.append(COMPANY_NAME);
        _dataPath.push_back('\\');
        _dataPath.append(PRODUCT_NAME);
        CFileOperation::MakeLocalDir(_dataPath);
    }
    else
    {
        Log::GetInstance()->Write(Log::LOG_ALERT,
            L"SHGetFolderPath return %x.\n", ret);        
        _dataPath.append(L"C:\\");
        _dataPath.append(COMPANY_NAME);
        _dataPath.push_back('\\');
        _dataPath.append(PRODUCT_NAME);
    }

    return _dataPath;
}

std::wstring DataPath::GetTempDir()
{
    if ( ! _tempPath.empty() )
        return _tempPath;

    DWORD bufferSize = BUFSIZE;
    wchar_t buffer[BUFSIZE];
    DWORD ret = GetTempPath(bufferSize, buffer);

    if (ret > bufferSize || (ret == 0))
    {
        Log::GetInstance()->Write(Log::LOG_ALERT,
            L"GetTempDir failed with error %d.\n", GetLastError());
    }
    else
    {
        _tempPath.assign(buffer);
        _tempPath.append(PRODUCT_NAME);
        CFileOperation::MakeLocalDir(_tempPath);
    }

    return _tempPath;
}

void AppendString(wstring& str, unsigned char* pBuffer, int length)
{
    for(int i = 0; i < length; ++i )
        str.push_back(pBuffer[length]);
}

std::wstring DataPath::GetUniqueTempDir()
{
    wstring dir = GetTempDir() + L"\\" + CFileOperation::GetGuidW();
    if ( CFileOperation::MakeLocalDir(dir) )
        return dir;
    else
        return L"";
}
