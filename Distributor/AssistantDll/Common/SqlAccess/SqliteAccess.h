#pragma once

#include <map>
#include <vector>
#include <string>
#include <sstream>

#include "include/sqlite/CppSqlite3.h"

using namespace std;

namespace ND91Assistant
{
    // SELECT接口
    class ISelect
    {
    public:
        // Summary  : 处理一行数据
        // Parameter: pValues 从表中select来的一行数据
        virtual void AddRow(const vector<const char*>* pValues) = 0;
    };

    // sqlite访问类
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

        // Summary  : 执行Update语句
        // Returns  : Update影响记录数
        // Parameter: table 表名
        // Parameter: condition 条件
        // Parameter: pColumns 字段
        // Parameter: pValues  值
        int Update(wstring table, wstring condition,
            const vector<wstring>* pColumns, const vector<wstring>* pValues);

        // Summary  : 执行Insert语句
        // Returns  : insert 记录数
        // Parameter: table 表名
        // Parameter: pColumns 字段
        // Parameter: pValues  值
        int Insert(wstring table, const vector<wstring>* pColumns, const vector<wstring>* pValues);

        // Summary  : 执行DELETE语句
        // Returns  : delete记录数
        // Parameter: table 表名
        // Parameter: condition 条件
        int Delete(wstring table, wstring condition);

		//获得满足条件的记录数
		int getCount( wstring table,wstring condition);
		//取最大pid
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
