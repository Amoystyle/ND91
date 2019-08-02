#include "stdafx.h"

#include "SqliteAccess.h"

using namespace ND91Assistant;

SqliteAccess::SqliteAccess()
{
}

SqliteAccess::~SqliteAccess()
{
    if (_db.IsOpen())
        _db.Close();
}

bool SqliteAccess::Begin()
{
    RETURN_FALSE_IF( ! _db.IsOpen() );
    return SQLITE_OK == _db.BeginTransaction();
}

bool SqliteAccess::Commit()
{
    RETURN_FALSE_IF( ! _db.IsOpen() );
    return SQLITE_OK == _db.Commit();
}

bool SqliteAccess::Rollback()
{
    RETURN_FALSE_IF( ! _db.IsOpen() );
    return SQLITE_OK == _db.Rollback();
}

void SqliteAccess::Close()
{
	if(_db.IsOpen())
			 _db.Close();
}

bool SqliteAccess::Open( wstring filename )
{
    RETURN_FALSE_IF( ! CFileOperation::IsFileExist(filename.c_str()) );
    _filename = CCodeOperation::UnicodeToUTF_8(filename);

    if ( _db.IsOpen() )
        _db.Close();

    return _db.Open(_filename.c_str());
}

void SqliteAccess::Select(ISelect* pTable,
                          wstring table,
                          wstring condition,
                          const vector<wstring>* const pColumns)
{
    RETURN_IF( table.empty() );
    RETURN_IF( pColumns->empty() );

    wstring sql = L"SELECT ";
    vector<wstring>::const_iterator it;
    for(it = pColumns->begin(); it != pColumns->end(); ++it )
    {
        sql += *it;
        sql.push_back(',');
    }
    sql.erase(sql.size()-1, 1); // delete last comma;

    sql += L" FROM " + table;
    if ( ! condition.empty() )
        sql += L" WHERE " + condition;
    sql.push_back(';');

    ExecuteQuery(pTable, sql);
}

__int64 SqliteAccess::SelectMaxPid(wstring table,wstring keyName)
{
	int64_t ret = -1;
	wstring sql = L"SELECT Max(";
	sql +=keyName;
	sql +=L") FROM " + table;	
	sql.push_back(';');
	try
	{	
		string s = CCodeOperation::UnicodeToUTF_8(sql);
		CppSQLite3Query q = _db.ExecQuery(s.c_str());

		if(!q.Eof())
		{
			const char* value = q.GetStringField(0);
			ret = atol(value);
		}
		else
			ret = 0;
	}
	catch (CppSQLite3Exception& e)
	{
		wostringstream woss;
		woss << "SQLite ERROR(" << e.ErrorCode() << ") " << e.ErrorMessage();
		woss << " : " << sql;
		LOG_ERROR(woss.str());
	}
	return ret;
}
int SqliteAccess::getCount( wstring table,wstring condition)
{
	int ret = -1;
	wstring sql = L"SELECT count(*) ";
	sql += L" FROM " + table;
	if ( ! condition.empty() )
		sql += L" WHERE " + condition;
	sql.push_back(';');
	try
	{	
		string s = CCodeOperation::UnicodeToUTF_8(sql);
		CppSQLite3Query q = _db.ExecQuery(s.c_str());

		if(!q.Eof())
			ret = q.GetIntField(0);
		else
			ret = 0;
	}
	catch (CppSQLite3Exception& e)
	{
		wostringstream woss;
		woss << "SQLite ERROR(" << e.ErrorCode() << ") " << e.ErrorMessage();
		woss << " : " << sql;
		LOG_ERROR(woss.str());
	}
	return ret;
}

int SqliteAccess::Update(wstring table,
                         wstring condition,
                         const vector<wstring>* pColumns,
                         const vector<wstring>* pValues)
{
    RETURN_VALUE_IF(table.empty(), 0);
    RETURN_VALUE_IF( ! pColumns, 0);
    RETURN_VALUE_IF( ! pValues, 0);
    RETURN_VALUE_IF(pColumns->empty(), 0);
    RETURN_VALUE_IF(pValues->empty(), 0);
    RETURN_VALUE_IF(pValues->size() != pColumns->size(), 0);

    wstring sql = L"UPDATE " + table + L" SET ";
    for(size_t i = 0; i < pValues->size(); ++i)
    {
        sql += pColumns->at(i) + L"=" + pValues->at(i) + L",";
    }
    sql.erase(sql.size()-1, 1); // delete last comma;

    if ( ! condition.empty() )
        sql += L" WHERE " + condition;
    sql.push_back(';');

    return Execute(sql);
}


int SqliteAccess::Insert(wstring table,
                         const vector<wstring>* pColumns,
                         const vector<wstring>* pValues)
{
    RETURN_VALUE_IF(table.empty(), 0);
    RETURN_VALUE_IF( ! pColumns, 0);
    RETURN_VALUE_IF( ! pValues, 0);
    RETURN_VALUE_IF(pColumns->empty(), 0);
    RETURN_VALUE_IF(pValues->empty(), 0);
    RETURN_VALUE_IF(pValues->size() != pColumns->size(), 0);

    wstring columns, values;
    for(size_t i = 0; i < pValues->size(); ++i)
    {
        columns.push_back(',');
        columns += pColumns->at(i);

        values.push_back(',');
        values += pValues->at(i);
    }
    columns.erase(0, 1);   // delete first comma
    values.erase(0, 1); // delete first comma

    wstring sql= L"INSERT INTO " + table + L" (" + columns + L") VALUES (" + values + L");";
    return Execute(sql);
}

int SqliteAccess::Delete(wstring table, wstring condition )
{
    RETURN_VALUE_IF(table.empty(), 0);

    wstring sql = L"DELETE FROM " + table;

    if ( ! condition.empty() )
        sql += L" WHERE " + condition;
    sql.push_back(';');

    return Execute(sql);
}

int ND91Assistant::SqliteAccess::Execute(wstring sql)
{
	string s = CCodeOperation::UnicodeToUTF_8(sql);
    try		
	{  
        return _db.ExecDML(s.c_str());
    }
    catch (CppSQLite3Exception& e)
    {
        wostringstream woss;
        woss << "SQLite ERROR(" << e.ErrorCode() << ") " << e.ErrorMessage();
        woss << " : " << sql.c_str();
        LOG_ERROR(woss.str());
        return false;
    }
    catch (...)
    {
        return false;
    }
}

bool SqliteAccess::ExecuteQuery(ISelect* pTable, wstring sql)
{
    try
    {
        string s = CCodeOperation::UnicodeToUTF_8(sql);
        CppSQLite3Query q = _db.ExecQuery(s.c_str());

        while (!q.Eof())
        {
            vector<const char*> values;
            int filedCount = q.NumFields();

            for (int i = 0; i < filedCount; i++)
                values.push_back(q.GetStringField(i));

            pTable->AddRow(&values);
            q.NextRow();
        }
    }
    catch (CppSQLite3Exception& e)
    {
        wostringstream woss;
        woss << "SQLite ERROR(" << e.ErrorCode() << ") " << e.ErrorMessage();
        woss << " : " << sql;
        LOG_ERROR(woss.str());
        return false;
    }

    return true;
}

static void Read(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    RETURN_IF( 1 != argc );
    RETURN_IF( SQLITE_INTEGER != sqlite3_value_type(argv[0]) );

    /*__int64*/int64_t flag = sqlite3_value_int64(argv[0]);

    sqlite3_result_int64(context, flag & 2);
}

void ND91Assistant::SqliteAccess::CreateReadFunction()
{
    RETURN_IF ( ! _db.IsOpen() );

    sqlite3_create_function(
        _db.GetSqlite3DB(),
        "read",
        1,
        SQLITE_UTF8,
        NULL,
        &Read,
        NULL,
        NULL
        );
}
