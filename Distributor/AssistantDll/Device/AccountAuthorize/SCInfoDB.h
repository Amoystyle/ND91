#pragma once

#include "include/sqlite/CppSqlite3.h"

#define AUTHORIZATIONDB  AuthorizationDB::GetInstance()

namespace ND91Assistant
{
    class Authorization;

    // SC�����Ϣ���ݿ�
    class  AuthorizationDB : public CppSQLite3DB
    {
    public:
        // ȡ�õ�������
        static AuthorizationDB* GetInstance();

        // ��ȡָ��UUID�豸�Ѿ���Ȩ���˺�
       // void GetAuthorizedAccounts(wstring uuid, vector<wstring>& authorizedAccounts);

        // ��ȡ������Ȩ�ļ���Ϣ
        void GetAllAuthorizations(vector<Authorization*>& authorizations);

        // ɾ����ǰ�豸������������Ȩ��Ϣ
       // void DeleteAuthorizedAccounts(wstring uuid);

        // ��������Ȩ���˻�
       // bool InsertAuthorizedAccount(wstring account, wstring udid);
        bool InstertAuthorization(Authorization* info);

        // ���ܹؼ�����
        static wstring EncryptData(wstring data);

        // ���ܹؼ�����
        static wstring DecryptData(wstring data);

        static wstring _DBFolder;   // ������ݿⱾ���ļ���

    private:

        // ��ȡ��ǰ���ݿ�汾
        wstring GetDbVersion();

        // ����Criteria��ȡ������Ȩ�ļ���Ϣ(�����߸����ͷ�Authorization*)
        void GetAuthorizationsByCriteria(wstring criteria, vector<Authorization*>& authorizations);

    private:
        AuthorizationDB();
        virtual ~AuthorizationDB();

        static AuthorizationDB* _pInstance;     // ����
        wstring                 _DBPath;        // ������ݿⱾ��·��
        wstring                 _DBVersion;     // ��ǰ���ݿ�汾,��Ҫ�͹�����������ݿ�һ��ά��
    };
}