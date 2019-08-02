#include "stdafx.h"
#include "SCInfo.h"

#include "Common/FileOperation.h"
#include "SCInfoDB.h"

Authorization::Authorization()
{
	_folder=L"";
    _isLocalCached = false;
    ::time(&_createDate);
}

Authorization::Authorization(const char* data[7])
{
    SetAccountID(AuthorizationDB::DecryptData(CCodeOperation::UTF_8ToUnicode(data[1])));
    SetFolder(CCodeOperation::UTF_8ToUnicode(data[2]));
    SetITunesVersion(CCodeOperation::UTF_8ToUnicode(data[3]));
    SetData(CCodeOperation::UTF_8ToUnicode(data[4]));
    SetServerParams(CCodeOperation::UTF_8ToUnicode(CStrOperation::FromBase64String(data[5])));
    SetCreateDate(atoi(data[6]));

    RETURN_IF(GetAccountID().find(L",") == wstring::npos)
    SetIDS(GetAccountID());
    SetAccountID(L"");
}

Authorization::~Authorization()
{
}

wstring Authorization::ToString()
{
    wstring id = _accountID;
    if (id.empty())
        id = _IDs;

    return id+L""+_iTunesVersion;
}

wstring Authorization::GetFolder() 
{ 
    return _folder; 
}

void Authorization::SetFolder(wstring folder) 
{
    _folder = folder;

    int nodeSum = 0;
    __int64 sizeSum = 0;
    CFileOperation::GetDirSizeW(_folder, nodeSum, sizeSum);
    if (!_folder.empty() 
        && CFileOperation::IsDirExist(_folder.c_str()) 
        && nodeSum > 0)
        _isLocalCached = true;
    else
        _isLocalCached = false;
}