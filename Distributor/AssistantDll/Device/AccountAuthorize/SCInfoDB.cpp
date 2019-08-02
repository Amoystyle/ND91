#include "stdafx.h"
#include "SCInfoDB.h"
#include "Common/MyFileOperation.h"
#include "Authorizor.h"
#include "Common/FileOperation.h"
#include "Device/IosPublicFunc.h"
#include "SCInfo.h"

AuthorizationDB* AuthorizationDB::_pInstance = NULL;
wstring AuthorizationDB::_DBFolder  = L"C:\\Documents and Settings\\All Users\\Application Data\\"+Authorizor::_CacheFolder;

AuthorizationDB::AuthorizationDB()
{
    _DBPath     = _DBFolder + L"FairPlay91";
	_DBVersion  = L"1.0.2";// CFileOperation::GetIniFileValue(CFileOperation::GetCurrWorkingDir()+L"Option.ini", L"SCInfoDB", L"version");

    try
    {
        if (!CFileOperation::IsDirExist(_DBFolder.c_str()))
            CFileOperation::MakeLocalDir(_DBFolder);

        wstring SCInfoDBPath = CFileOperation::GetCurrWorkingDir()+L"SCInfoDB\\Harbor";

        if (!CFileOperation::IsFileExist(_DBPath.c_str()))
        {
            // 解压文件到目录
            CFileOperation::CopyFile(SCInfoDBPath, _DBPath, false);
        }
        else
        {
            // 判断本地数据库版本
            int localVersion = IosPublicFunc::GetVerNum(GetDbVersion());
            int newVersion = IosPublicFunc::GetVerNum(_DBVersion);

            //TODO:是否需要数据迁移
            if (newVersion > localVersion)
                CFileOperation::CopyFile(SCInfoDBPath, _DBPath, false);
        }
    }
    catch(...)
    {
    }
}

AuthorizationDB::~AuthorizationDB()
{
}

AuthorizationDB* AuthorizationDB::GetInstance()
{
    if (!_pInstance)
        _pInstance = new AuthorizationDB();
    return _pInstance;
}

wstring AuthorizationDB::GetDbVersion()
{
    wstring version = L"0.1";
    RETURN_VALUE_IF(!CFileOperation::IsFileExist(_DBPath.c_str()), version);

    CppSQLite3DB::Open(CCodeOperation::UnicodeToUTF_8(_DBPath).c_str());
    CppSQLite3Buffer sql;
    sql.Format("Select value from [_sqlitedatabaseproperties] where key = 'DBVersion';");
    CppSQLite3Query q = ExecQuery(sql);
    while (!q.Eof())
    {
        version = CCodeOperation::UTF_8ToUnicode(q.GetStringField(0));
        q.NextRow();
    }
    
    CppSQLite3DB::Close();
    return version;
}

// void AuthorizationDB::GetAuthorizedAccounts(wstring uuid, vector<wstring>& authorizedAccounts)
// {
//     RETURN_IF(!CFileOperation::IsFileExist(_DBPath.c_str()));
//     CppSQLite3DB::Open(CCodeOperation::UnicodeToUTF_8(_DBPath).c_str());
// 
//     CppSQLite3Buffer sql;
//     sql.Format("Select Account from [Authorizeds] Where UDID ='%s'", MyCodeOperation::UnicodeToUTF_8SQL(uuid).c_str());
//     CppSQLite3Query q = ExecQuery(sql);
//     while (!q.Eof())
//     {
//         authorizedAccounts.push_back(DecryptData(CCodeOperation::UTF_8ToUnicode(q.GetStringField(0))));
//         q.NextRow();
//     }
// 
//     CppSQLite3DB::Close();
// }

void AuthorizationDB::GetAllAuthorizations(vector<Authorization*>& authorizations)
{
    GetAuthorizationsByCriteria(L"", authorizations);
}

void AuthorizationDB::GetAuthorizationsByCriteria(wstring criteria, vector<Authorization*>& authorizations)
{
    RETURN_IF(!CFileOperation::IsFileExist(_DBPath.c_str()));
    CppSQLite3DB::Open(CCodeOperation::UnicodeToUTF_8(_DBPath).c_str());

    CppSQLite3Buffer sql;
    sql.Format("Select * from [Infos] %s", MyCodeOperation::UnicodeToUTF_8SQL(criteria).c_str());
    CppSQLite3Query q = ExecQuery(sql);
    while (!q.Eof())
    {
        const char* data[7] = {0};
        for (int i = 0; i < 7; i++)
            data[i] = q.GetStringField(i);

        authorizations.push_back(new Authorization(data));

        q.NextRow();
    }

    CppSQLite3DB::Close();
}

// void AuthorizationDB::DeleteAuthorizedAccounts(wstring uuid)
// {
//     RETURN_IF(!CFileOperation::IsFileExist(_DBPath.c_str()));
//     CppSQLite3DB::Open(CCodeOperation::UnicodeToUTF_8(_DBPath).c_str());
// 
//     CppSQLite3Buffer sql;
//     sql.Format("Delete From  [Authorizeds] Where UDID = '%s';", MyCodeOperation::UnicodeToUTF_8SQL(uuid).c_str());
//     int row = ExecDML((const char*)sql);
// 
//     CppSQLite3DB::Close();
// }

unsigned char KEY[8] = { 0x6a, 0xf7, 0x7b, 0xb9, 0xc9, 0x6a, 0xdf, 0x30 };
wstring AuthorizationDB::EncryptData(wstring data)
{
    RETURN_VALUE_IF(data.empty(), data);

    string utf8 = CCodeOperation::UnicodeToUTF_8(data);
    string strDes = CStrOperation::EncryptDes(utf8, KEY);
    string str = CStrOperation::ToBase64String(strDes);
    return CCodeOperation::UTF_8ToUnicode(str);
}

wstring AuthorizationDB::DecryptData(wstring data)
{
    RETURN_VALUE_IF(data.empty(), data);

    string utf8 = CCodeOperation::UnicodeToUTF_8(data);
    string str = CStrOperation::FromBase64String(utf8);
    string res = CStrOperation::DecryptDes(str, KEY);
    return CCodeOperation::UTF_8ToUnicode(res);
}

// bool AuthorizationDB::InsertAuthorizedAccount(wstring account, wstring udid)
// {
//     RETURN_FALSE_IF(!CFileOperation::IsFileExist(_DBPath.c_str()));
//     CppSQLite3DB::Open(CCodeOperation::UnicodeToUTF_8(_DBPath).c_str());
// 
//     CppSQLite3Buffer sql;
//     sql.Format("Insert into [Authorizeds](Account,UDID) values('%s','%s')", MyCodeOperation::UnicodeToUTF_8SQL(EncryptData(account)).c_str(), MyCodeOperation::UnicodeToUTF_8SQL(udid).c_str());
//     int res = ExecDML((const char*)sql);
// 
//     CppSQLite3DB::Close();
// 
//     return (res == 1);
// }

bool AuthorizationDB::InstertAuthorization(Authorization* info)
{
    RETURN_FALSE_IF(!CFileOperation::IsFileExist(_DBPath.c_str()));
    CppSQLite3DB::Open(CCodeOperation::UnicodeToUTF_8(_DBPath).c_str());

    wstring accountInfo = info->GetAccountID();
    if (accountInfo.empty())
        accountInfo = info->GetIDS();

    string paramSrc = CCodeOperation::UnicodeToUTF_8(info->GetServerParams());
    string paramDes = CStrOperation::ToBase64String(paramSrc);

    wchar_t createDate[128] = {0};
    wsprintf(createDate, L"%d", info->GetCreateDate());

    CppSQLite3Buffer sql;
    sql.Format("Insert into [Infos](Account,Path,Version,Data,Parameters,CreateDate) values('%s','%s','%s','%s','%s','%s')", 
        MyCodeOperation::UnicodeToUTF_8SQL(EncryptData(accountInfo)).c_str(), 
        MyCodeOperation::UnicodeToUTF_8SQL(info->GetFolder()).c_str(),
        MyCodeOperation::UnicodeToUTF_8SQL(info->GetITunesVersion()).c_str(),
        MyCodeOperation::UnicodeToUTF_8SQL(info->GetData()).c_str(),
        MyCodeOperation::UnicodeToUTF_8SQL(CCodeOperation::UTF_8ToUnicode(paramDes)).c_str(),
        MyCodeOperation::UnicodeToUTF_8SQL(createDate).c_str());
    int res = ExecDML((const char*)sql);

    CppSQLite3DB::Close();

    return (res == 1);
}