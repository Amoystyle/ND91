///////////////////////////////////////////////////////////////////////////////
//
// @file 
// @brief数据库类
//
// <b>文件名</b>      : DBManager.h
// @n@n<b>版权所有</b>: 网龙天晴程序部应用软件开发组
// @n@n<b>作  者</b>  : wengcm
// @n@n<b>创建时间</b>: 2010-2-8 16:04:04
// @n@n<b>文件描述</b>: 
// @version		版本	修改者		  时间		  描述@n
// @n		        wengcm        2010-02-22 
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#ifdef DATABASEMANAGER_EXPORTS
#define DATABASEMANAGER_API __declspec(dllexport)
#else
#define DATABASEMANAGER_API __declspec(dllimport)
#endif

#include "sqlite/CppSqlite3.h"
#include "Manager/EventManager.h"

#define LANDBOP_MUTEX_NAME _T("xczcsjncksdcnisdh9dsd90909")
			
class DATABASEMANAGER_API CDBManager : public IExitDB
{
public:
	CDBManager();
	~CDBManager();
	//互斥体
	static HANDLE dealMutex;
	//数据库连接标准
	static bool m_isconnect;
	//数据库名
	static CString m_dbName;
	//数据库实例
	static CppSQLite3DB m_db;	 
	//初始化数据库
	static bool InitDB(LPCTSTR lpszNewValue);
	//关闭数据库
	static void CloseDB();
	//更改当前打开数据库
	//static bool SetDatabaseName(LPCTSTR lpszNewValue);//比较危险,屏蔽
	//执行sql
    static void DoExecSql(LPCSTR SQLBuf);
	static void DoExecSql(LPCWSTR SQLBuf);
	//查询数据
	static bool DB_QUERY_DATA(LPCTSTR SQLBuf,CppSQLite3Query& resQ);

	//查询表数据
	static bool DB_QUERY_TABLEDATA(LPCTSTR SQLBuf,CppSQLite3Table& resQ);

	void ExitDB();

	static CDBManager* GetInstance();
private:	
	static CDBManager* _pDBManager;//生成该对象只实现关闭

	static bool   _g_bExit;
};
