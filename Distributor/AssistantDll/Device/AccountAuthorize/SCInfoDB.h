#pragma once

#include "include/sqlite/CppSqlite3.h"

#define AUTHORIZATIONDB  AuthorizationDB::GetInstance()

namespace ND91Assistant
{
    class Authorization;

    // SC相关信息数据库
    class  AuthorizationDB : public CppSQLite3DB
    {
    public:
        // 取得单例对象
        static AuthorizationDB* GetInstance();

        // 获取指定UUID设备已经授权的账号
       // void GetAuthorizedAccounts(wstring uuid, vector<wstring>& authorizedAccounts);

        // 获取所有授权文件信息
        void GetAllAuthorizations(vector<Authorization*>& authorizations);

        // 删除当前设备关联的所有授权信息
       // void DeleteAuthorizedAccounts(wstring uuid);

        // 插入新授权的账户
       // bool InsertAuthorizedAccount(wstring account, wstring udid);
        bool InstertAuthorization(Authorization* info);

        // 加密关键数据
        static wstring EncryptData(wstring data);

        // 解密关键数据
        static wstring DecryptData(wstring data);

        static wstring _DBFolder;   // 存放数据库本地文件夹

    private:

        // 获取当前数据库版本
        wstring GetDbVersion();

        // 根据Criteria获取所有授权文件信息(调用者负责释放Authorization*)
        void GetAuthorizationsByCriteria(wstring criteria, vector<Authorization*>& authorizations);

    private:
        AuthorizationDB();
        virtual ~AuthorizationDB();

        static AuthorizationDB* _pInstance;     // 单例
        wstring                 _DBPath;        // 存放数据库本地路径
        wstring                 _DBVersion;     // 当前数据库版本,需要和工程里面的数据库一起维护
    };
}