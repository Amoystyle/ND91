///////////////////////////////////////////////////////////////////////////////
//
// @file 
// @brief���ݿ���
//
// <b>�ļ���</b>      : DBManager.h
// @n@n<b>��Ȩ����</b>: �����������Ӧ�����������
// @n@n<b>��  ��</b>  : wengcm
// @n@n<b>����ʱ��</b>: 2010-2-8 16:04:04
// @n@n<b>�ļ�����</b>: 
// @version		�汾	�޸���		  ʱ��		  ����@n
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
	//������
	static HANDLE dealMutex;
	//���ݿ����ӱ�׼
	static bool m_isconnect;
	//���ݿ���
	static CString m_dbName;
	//���ݿ�ʵ��
	static CppSQLite3DB m_db;	 
	//��ʼ�����ݿ�
	static bool InitDB(LPCTSTR lpszNewValue);
	//�ر����ݿ�
	static void CloseDB();
	//���ĵ�ǰ�����ݿ�
	//static bool SetDatabaseName(LPCTSTR lpszNewValue);//�Ƚ�Σ��,����
	//ִ��sql
    static void DoExecSql(LPCSTR SQLBuf);
	static void DoExecSql(LPCWSTR SQLBuf);
	//��ѯ����
	static bool DB_QUERY_DATA(LPCTSTR SQLBuf,CppSQLite3Query& resQ);

	//��ѯ������
	static bool DB_QUERY_TABLEDATA(LPCTSTR SQLBuf,CppSQLite3Table& resQ);

	void ExitDB();

	static CDBManager* GetInstance();
private:	
	static CDBManager* _pDBManager;//���ɸö���ֻʵ�ֹر�

	static bool   _g_bExit;
};
