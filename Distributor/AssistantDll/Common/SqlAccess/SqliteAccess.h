#pragma once

#include <map>
#include <vector>
#include <string>
#include <sstream>

#include "include/sqlite/CppSqlite3.h"

using namespace std;

namespace ND91Assistant
{
    // SELECT�ӿ�
    class ISelect
    {
    public:
        // Summary  : ����һ������
        // Parameter: pValues �ӱ���select����һ������
        virtual void AddRow(const vector<const char*>* pValues) = 0;
    };

    // sqlite������
    class SqliteAccess
    {
    public:
        SqliteAccess();
        ~SqliteAccess();

        // open database ( if current database is open, close it at first
        bool Open(wstring filename);

        // close current opened database
        void Close();
 
        // begin transaction
        bool Begin();

        // commit transcation
        bool Commit();

        // rollback transaction
        bool Rollback();

        void Select(ISelect* pTable, wstring table, wstring condition,
            const vector<wstring>* const pColumns);

        // Summary  : ִ��Update���
        // Returns  : UpdateӰ���¼��
        // Parameter: table ����
        // Parameter: condition ����
        // Parameter: pColumns �ֶ�
        // Parameter: pValues  ֵ
        int Update(wstring table, wstring condition,
            const vector<wstring>* pColumns, const vector<wstring>* pValues);

        // Summary  : ִ��Insert���
        // Returns  : insert ��¼��
        // Parameter: table ����
        // Parameter: pColumns �ֶ�
        // Parameter: pValues  ֵ
        int Insert(wstring table, const vector<wstring>* pColumns, const vector<wstring>* pValues);

        // Summary  : ִ��DELETE���
        // Returns  : delete��¼��
        // Parameter: table ����
        // Parameter: condition ����
        int Delete(wstring table, wstring condition);

		//������������ļ�¼��
		int getCount( wstring table,wstring condition);
		//ȡ���pid
		__int64 SelectMaxPid(wstring table,wstring keyName);

        void CreateReadFunction();

		CppSQLite3DB* GetDB(){return &_db;}

    private:
        int Execute(wstring sql);
        bool ExecuteQuery(ISelect* pTable, wstring sql);

    private:
        string              _filename;
        CppSQLite3DB        _db;
    };
}
