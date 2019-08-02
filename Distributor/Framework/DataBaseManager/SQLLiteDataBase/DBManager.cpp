#include "stdafx.h"
#include "DBManager.h"

HANDLE CDBManager::dealMutex = NULL;
bool CDBManager::m_isconnect =false;
CString CDBManager::m_dbName =L"";
CppSQLite3DB CDBManager::m_db;	 
CDBManager* CDBManager::_pDBManager = NULL;
bool CDBManager::_g_bExit = false;

CDBManager::CDBManager()
{INTERFUN;
}

CDBManager::~CDBManager()
{INTERFUN;
	CloseDB();

	CloseHandle(dealMutex);
}

/************************************************************************************
 * @brief初始化数据库
 * 
 * 
 * @n<b>函数名称</b>             : InitDB
 * @n@param LPCTSTR lpszNewValue : 
 * @return                        
 * @see                           
 * @n<b>作者</b>                 :wengcm
 * @n<b>创建时间</b>             : 2010-2-22 10:48:55
 * @version	修改者        时间        描述@n
 * @n		wengcm        2010-02-22 
 ************************************************************************************/
bool CDBManager::InitDB(LPCTSTR DBName)
{INTERFUN;
	int ret=SQLITE_OK;
	m_dbName = DBName;
	if(!m_isconnect)
	{
		try
		{
		//	std::string strdbname = Charset::UnicodeToUTF8(DBName);
			USES_CONVERSION;
			string strdbname;
			strdbname = CCodeOperation::UnicodeToUTF_8(DBName);
			CDBManager::m_db.Open(strdbname.c_str());			
		}
		catch (CppSQLite3Exception* e)
		{
			ret =  e->ErrorCode();
			m_isconnect =FALSE;
		}
		if(ret == SQLITE_OK)
			m_isconnect =TRUE;
		else
		{
			CString sret;
			sret.Format(L"Open DB Fail! ret=[%d]",ret);
			TRACE(sret);
			m_isconnect =FALSE;
		}
	}
	dealMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, LANDBOP_MUTEX_NAME);

	if (dealMutex == NULL)
		dealMutex = CreateMutex(NULL, FALSE, LANDBOP_MUTEX_NAME);

	//CreateTable(DBName);
	return m_isconnect;
}

void CDBManager::CloseDB()
{INTERFUN;
	if(m_isconnect)
	{
		WaitForSingleObject(dealMutex, INFINITE);

		CDBManager::m_db.Close();

		_g_bExit = Singleton::GetEventManager()->SystemExitFlag();

		ReleaseMutex(dealMutex);
		
		m_isconnect = false;
	}
}

void CDBManager::ExitDB()
{
	CDBManager::CloseDB();
}

// bool CDBManager::SetDatabaseName(LPCTSTR lpszNewValue)
// {
// 	
// 	if(m_isconnect)
// 			CloseDB();
// 		
// 	return InitDB(lpszNewValue);
// }




void CDBManager::DoExecSql(LPCSTR SQLBuf)
{INTERFUN;
	if(!m_isconnect)
		InitDB(m_dbName);
	WaitForSingleObject(dealMutex, INFINITE);
	if (_g_bExit)
	{
		return;
	}
	int res = 0;
	try
	{
		res = m_db.ExecDML(SQLBuf);
	}
	catch (CppSQLite3Exception &e)
	{
		CStringA mes;
		mes.Format("do sql result error!! SQL: %s |MES: %s\n", SQLBuf, e.ErrorMessage());
		TRACE(mes);
	}
	catch(...)
	{
		CStringA mes;
		mes.Format("do sql result other error!! SQL: %s\n", SQLBuf);
		TRACE(mes);
	}
	ReleaseMutex(dealMutex);
}

void CDBManager::DoExecSql(LPCWSTR SQLBuf)
{INTERFUN;
    std::string strname = CCodeOperation::UnicodeToUTF_8(SQLBuf);
    return DoExecSql(strname.c_str());
}

bool CDBManager::DB_QUERY_DATA(LPCTSTR SQLBuf,CppSQLite3Query& resQ)
{INTERFUN;
	if(!m_isconnect)
		InitDB(m_dbName);

	bool result = true;
	try
	{
		std::string strname = CCodeOperation::UnicodeToUTF_8(SQLBuf);
		resQ =m_db.ExecQuery(strname.c_str());
	}
	catch (CppSQLite3Exception &e)
	{
		CStringA mes;
		mes.Format("do sql result error!! SQL: %s |MES: %s\n", SQLBuf, e.ErrorMessage());
		TRACE(mes);

		result = false;
	}
	catch(...)
	{
		CStringA mes;
		mes.Format("do sql result other error!! SQL: %s\n", SQLBuf);
		TRACE(mes);

		result = false;
	}

	return result;
}

bool CDBManager::DB_QUERY_TABLEDATA(LPCTSTR SQLBuf,CppSQLite3Table& resQ)
{INTERFUN;
	if(!m_isconnect)
		InitDB(m_dbName);

	bool result = true;
	try
	{
		std::string strname = CCodeOperation::UnicodeToUTF_8(SQLBuf);
		resQ =m_db.GetTable(strname.c_str());
	}
	catch (CppSQLite3Exception &e)
	{
		CStringA mes;
		mes.Format("do sql result error!! SQL: %s |MES: %s\n", SQLBuf, e.ErrorMessage());
		TRACE(mes);

		result = false;
	}
	catch(...)
	{
		CStringA mes;
		mes.Format("do sql result other error!! SQL: %s\n", SQLBuf);
		TRACE(mes);

		result = false;
	}

	return result;
}

CDBManager* CDBManager::GetInstance()
{
	if (NULL == _pDBManager)
	{
		_pDBManager = new CDBManager;
	}

	return _pDBManager;
}