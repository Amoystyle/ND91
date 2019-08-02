/***************************************************************************//**
 *  @file 
 *  @brief [简要描述]sqlite数据库访问类
 * 
 *  <b>文件名</b>      : CppSQLite3.cpp
 *  @n@n<b>版权所有</b>: 网龙天晴程序部应用软件开发组
 *  @n@n<b>作  者</b>  :  徐情波
 *  @n@n<b>创建时间</b>: 2009-01-04
 *  @n@n<b>文件描述</b>: 
 *  @version  版本        修改者        时间        描述@n
 *  @n        v1.0.0      徐情波     2009-01-16     添加注释、整理结构、增加多线程访问机制
 * 
*******************************************************************************/
#include "CppSQLite3.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <algorithm>
#include <string>
using namespace std;
#pragma warning (disable:4996)
/*表示异常类中是否要释放缓冲区空间*/
static const bool DONT_DELETE_MSG = false;

//=======================================通用函数===============================================
//-----------------------------------------------------------------------------------------------
int sqlite3_encode_binary(const unsigned char* in, int n, unsigned char* out);
int sqlite3_decode_binary(const unsigned char* in, unsigned char* out);


//=======================================SAFE_MAP结构===================================
void SAFE_MAP::CriticalSection_Init()
{
#ifdef WIN32	
	InitializeCriticalSection(&m_cs);
#else
	pthread_mutex_init(&m_cs, NULL);
#endif
} 

void SAFE_MAP::CriticalSection_Delete()
{
#ifdef WIN32	
	DeleteCriticalSection(&m_cs);
#else
	pthread_mutex_destroy(&m_cs);
#endif
}

void SAFE_MAP::CriticalSection_Enter()
{
#ifdef WIN32	
	EnterCriticalSection(&m_cs);
#else
	pthread_mutex_lock(&m_cs);
#endif
}
	
void SAFE_MAP::CriticalSection_Leave()
{
#ifdef WIN32	
	LeaveCriticalSection(&m_cs);
#else
	pthread_mutex_unlock(&m_cs);
#endif
}
    
//-----------------------------------------------------------------------------------------------
// 构造函数与析构函数
SAFE_MAP::SAFE_MAP(void)
{
	CriticalSection_Init();
}

SAFE_MAP::~SAFE_MAP()
{
	CriticalSection_Delete();
}

/**
 * @brief [函数简要描述]根据关键字获取值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>    : GetElement
 * @n@param DWORD dwKey : [参数描述]
 * @return                [返回值描述]
 * @see                   [参见函数]
 * @n<b>作者</b>        :
 * @n<b>创建时间</b>    : 2009-3-4 16:48:10
 * @version    修改者        时间        描述@n
 * @n          [修改者]      2009-03-04  [描述]
 */
/*LPVOID*/void* SAFE_MAP::GetElement(/*DWORD*/unsigned long dwKey)
{
	//多线程处理
	CriticalSection_Enter();

	/*LPVOID*/void* lpValue = NULL;
	map</*DWORD*/unsigned long, /*LPVOID*/void*>::const_iterator iter = m_map.find (dwKey);
	if (iter != m_map.end())
		lpValue = iter->second;

	CriticalSection_Leave();
	return lpValue;
}

/**
 * @brief [函数简要描述]获取第一项值，用户遍历哈希表
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: GetFirstItem
 * @return            [返回值描述]
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 2009-3-4 16:49:31
 * @version    修改者        时间        描述@n
 * @n          [修改者]      2009-03-04  [描述]
 */
bool SAFE_MAP::GetFirstItem()
{
	//多线程处理
	CriticalSection_Enter();

	m_iter = m_map.begin ();
	if (m_iter != m_map.end ())
	{
		CriticalSection_Leave();
		return true;
	}
	else
	{
		CriticalSection_Leave();
		return false;
	}

}

/**
 * @brief [函数简要描述]获取下一项值，用户遍历哈希表
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: NextItem
 * @return            [返回值描述]
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 2009-3-4 16:50:21
 * @version    修改者        时间        描述@n
 * @n          [修改者]      2009-03-04  [描述]
 */
bool SAFE_MAP::NextItem()
{
	//多线程处理
	CriticalSection_Enter();

	m_iter++;
	if (m_iter != m_map.end ())
	{
		CriticalSection_Leave();
		return true;
	}
	else
	{
		CriticalSection_Leave();
		return false;
	}

	CriticalSection_Leave();
}

/**
 * @brief [函数简要描述]获取当前项值，用户遍历哈希表
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: GetItem
 * @return            [返回值描述]
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 2009-3-4 16:50:56
 * @version    修改者        时间        描述@n
 * @n          [修改者]      2009-03-04  [描述]
 */
/*LPVOID*/void* SAFE_MAP::GetItem()
{
	return m_iter->second;
}

/**
 * @brief [函数简要描述]添加项
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>       : PutElement
 * @n@param DWORD  dwKey   : [参数描述]
 * @param   LPVOID lpValue : [参数描述]
 * @see                      [参见函数]
 * @n<b>作者</b>           :
 * @n<b>创建时间</b>       : 2009-3-4 16:51:20
 * @version    修改者        时间        描述@n
 * @n          [修改者]      2009-03-04  [描述]
 */
void SAFE_MAP::PutElement(/*DWORD*/unsigned long dwKey, /*LPVOID*/void* lpValue)
{
	//多线程处理
	CriticalSection_Enter();

	m_map[dwKey] = lpValue;

	CriticalSection_Leave();
}

/**
 * @brief [函数简要描述]删除项
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>    : RemoveAt
 * @n@param DWORD dwKey : [参数描述]
 * @see                   [参见函数]
 * @n<b>作者</b>        :
 * @n<b>创建时间</b>    : 2009-3-4 16:51:53
 * @version    修改者        时间        描述@n
 * @n          [修改者]      2009-03-04  [描述]
 */
void SAFE_MAP::RemoveAt(/*DWORD*/unsigned long dwKey)
{
	//多线程处理
	CriticalSection_Enter();

	map</*DWORD*/unsigned long, /*LPVOID*/void*>::const_iterator CIT = m_map.find (dwKey);
	if (CIT != m_map.end())
		 m_map.erase(dwKey);

	CriticalSection_Leave();
}

/**
 * @brief [函数简要描述]清空哈希表
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: RemoveAll
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 2009-3-4 16:52:13
 * @version    修改者        时间        描述@n
 * @n          [修改者]      2009-03-04  [描述]
 */
void SAFE_MAP::RemoveAll()
{
	//多线程处理
	CriticalSection_Enter();

	m_map.empty ();

	CriticalSection_Leave();
}

//=======================================CppSQLite3Exception类===================================
//-----------------------------------------------------------------------------------------------
// 构造函数与析构函数
CppSQLite3Exception::CppSQLite3Exception(const int nErrCode, char* szErrMess, bool bDeleteMsg/*=true*/) : 
									m_nErrCode(nErrCode)
{
	

	m_pszErrMess = sqlite3_mprintf(	"%s[%d]: %s",
										ErrorCodeAsString(nErrCode),
										nErrCode,
										szErrMess ? szErrMess : "");

	if (bDeleteMsg && szErrMess)
	{
		sqlite3_free(szErrMess);
	}
}
									
CppSQLite3Exception::CppSQLite3Exception(const CppSQLite3Exception& e) : 
									m_nErrCode(e.m_nErrCode)
{
	m_pszErrMess = NULL;
	if (e.m_pszErrMess)
	{
		m_pszErrMess = sqlite3_mprintf("%s", e.m_pszErrMess);
	}
}

CppSQLite3Exception::~CppSQLite3Exception()
{
	if (m_pszErrMess)
	{
		sqlite3_free(m_pszErrMess);
		m_pszErrMess = NULL;
		
	}
}

/**
 * @brief [函数简要描述]将错误代号作为字符串格式输出
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>     : ErrorCodeAsString
 * @n@param int nErrCode : [参数描述]错误号
 * @return                 [返回值描述]格式化后的错误号
 * @see                    [参见函数]
 * @n<b>作者</b>         :
 * @n<b>创建时间</b>     : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    增加注释
 */
const char* CppSQLite3Exception::ErrorCodeAsString(int nErrCode)
{
	// 代码号对应输出字符信息
	switch (nErrCode)
	{
		case SQLITE_OK          : return "SQLITE_OK";
		case SQLITE_ERROR       : return "SQLITE_ERROR";
		case SQLITE_INTERNAL    : return "SQLITE_INTERNAL";
		case SQLITE_PERM        : return "SQLITE_PERM";
		case SQLITE_ABORT       : return "SQLITE_ABORT";
		case SQLITE_BUSY        : return "SQLITE_BUSY";
		case SQLITE_LOCKED      : return "SQLITE_LOCKED";
		case SQLITE_NOMEM       : return "SQLITE_NOMEM";
		case SQLITE_READONLY    : return "SQLITE_READONLY";
		case SQLITE_INTERRUPT   : return "SQLITE_INTERRUPT";
		case SQLITE_IOERR       : return "SQLITE_IOERR";
		case SQLITE_CORRUPT     : return "SQLITE_CORRUPT";
		case SQLITE_NOTFOUND    : return "SQLITE_NOTFOUND";
		case SQLITE_FULL        : return "SQLITE_FULL";
		case SQLITE_CANTOPEN    : return "SQLITE_CANTOPEN";
		case SQLITE_PROTOCOL    : return "SQLITE_PROTOCOL";
		case SQLITE_EMPTY       : return "SQLITE_EMPTY";
		case SQLITE_SCHEMA      : return "SQLITE_SCHEMA";
		case SQLITE_TOOBIG      : return "SQLITE_TOOBIG";
		case SQLITE_CONSTRAINT  : return "SQLITE_CONSTRAINT";
		case SQLITE_MISMATCH    : return "SQLITE_MISMATCH";
		case SQLITE_MISUSE      : return "SQLITE_MISUSE";
		case SQLITE_NOLFS       : return "SQLITE_NOLFS";
		case SQLITE_AUTH        : return "SQLITE_AUTH";
		case SQLITE_FORMAT      : return "SQLITE_FORMAT";
		case SQLITE_RANGE       : return "SQLITE_RANGE";
		case SQLITE_ROW         : return "SQLITE_ROW";
		case SQLITE_DONE        : return "SQLITE_DONE";
		case CPPSQLITE_ERROR    : return "CPPSQLITE_ERROR";
		default: return "UNKNOWN_ERROR";
	}
}

//=======================================CppSQLite3Query类======================================
//-----------------------------------------------------------------------------------------------
// 构造函数与析构函数
CppSQLite3Query::CppSQLite3Query()
{
	m_pVM		= NULL;
	m_bEof		= true;
	m_nCols		= NULL;
	m_bOwnVM	= false;
	pointRow	= NULL;
}

CppSQLite3Query::CppSQLite3Query(const CppSQLite3Query& rQuery)
{
	m_pVM = rQuery.m_pVM;
	// Only one object can own the VM
	const_cast<CppSQLite3Query&>(rQuery).m_pVM = NULL;

	m_bEof		= rQuery.m_bEof;
	m_nCols		= rQuery.m_nCols;
	m_bOwnVM	= rQuery.m_bOwnVM;
	pointRow	= rQuery.pointRow;
	m_pDB	    = rQuery.m_pDB;
}

CppSQLite3Query::CppSQLite3Query(	sqlite3* pDB,
									sqlite3_stmt* pVM,
									bool bEof,
									bool bOwnVM/*=true*/)
{
	m_pDB    = pDB;
	m_pVM    = pVM;
	m_bEof   = bEof;
	m_nCols  = sqlite3_column_count(m_pVM);
	m_bOwnVM = bOwnVM;
	pointRow = NULL;

}

CppSQLite3Query::~CppSQLite3Query()
{
	try
	{
		Finalize();
	}
	catch (...)
	{

	}
}
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// 成员函数
CppSQLite3Query& CppSQLite3Query::operator=(const CppSQLite3Query& rQuery)
{
	try
	{
		Finalize();
	}
	catch (...)
	{
	}

	m_pVM       = rQuery.m_pVM;
	const_cast<CppSQLite3Query&>(rQuery).m_pVM = NULL;
	m_bEof		= rQuery.m_bEof;
	m_nCols		= rQuery.m_nCols;
	m_bOwnVM	= rQuery.m_bOwnVM;
	pointRow	= rQuery.pointRow;
	m_pDB	    = rQuery.m_pDB;
	return *this;
}


/**
 * @brief [函数简要描述]获取字段数目
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: NumFields
 * @return            [返回值描述]字段数目
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
int CppSQLite3Query::NumFields()
{
	CheckVM();
	return m_nCols;
}

/**
 * @brief [函数简要描述]通过字段索引获取字段名称
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>   : FieldValue
 * @n@param int nField : [参数描述]字段索引
 * @return               [返回值描述]字段名称
 * @see                  [参见函数]
 * @n<b>作者</b>       :
 * @n<b>创建时间</b>   : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
const char* CppSQLite3Query::FieldValue(int nField)
{
	CheckVM();

	if (nField < 0 || nField > m_nCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Invalid field index requested",
								DONT_DELETE_MSG);
	}

	return (const char*)sqlite3_column_text(m_pVM, nField);
}

/**
 * @brief [函数简要描述]通过字段名称获取字段索引
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>             : FieldValue
 * @n@param const char * szField : [参数描述]字段名称
 * @return                         [返回值描述]字段索引
 * @see                            [参见函数]
 * @n<b>作者</b>                 :
 * @n<b>创建时间</b>             : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
const char* CppSQLite3Query::FieldValue(const char* szField)
{
	int nField = FieldIndex(szField);
	return (const char*)sqlite3_column_text(m_pVM, nField);
}

/**
 * @brief [函数简要描述]获取整型字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>       : GetIntField
 * @n@param int nField     : [参数描述]字段索引]
 * @param   int nNullValue : [参数描述]空值时返回的默认值
 * @return                   [返回值描述]字段值
 * @see                      [参见函数]
 * @n<b>作者</b>           :
 * @n<b>创建时间</b>       : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
int CppSQLite3Query::GetIntField(int nField, int nNullValue/*=0*/)
{
	if (FieldDataType(nField) == SQLITE_NULL)
	{
		return nNullValue;
	}
	else
	{
		return sqlite3_column_int(m_pVM, nField);
	}
}


/**
 * @brief [函数简要描述]获取整型字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                : GetIntField
 * @n@param const char * szField    : [参数描述]字段名称
 * @param   int          nNullValue : [参数描述]空值时返回的默认值
 * @return                            [返回值描述]字段值
 * @see                               [参见函数]
 * @n<b>作者</b>                    :
 * @n<b>创建时间</b>                : 
 * @version    修改者        时间        描述@n
 * @n          许情波     2009-01-04     添加注释
 */
int CppSQLite3Query::GetIntField(const char* szField, int nNullValue/*=0*/)
{
	int nField = FieldIndex(szField);
	return GetIntField(nField, nNullValue);
}


/**
 * @brief [函数简要描述]获取浮点字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>          : GetFloatField
 * @n@param int    nField     : [参数描述]字段索引
 * @param   double fNullValue : [参数描述]空值时返回的默认值
 * @return                      [返回值描述]字段值
 * @see                         [参见函数]
 * @n<b>作者</b>              :
 * @n<b>创建时间</b>          : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
double CppSQLite3Query::GetFloatField(int nField, double fNullValue/*=0.0*/)
{
	if (FieldDataType(nField) == SQLITE_NULL)
	{
		return fNullValue;
	}
	else
	{
		return sqlite3_column_double(m_pVM, nField);
	}
}


/**
 * @brief [函数简要描述]获取浮点字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                : GetFloatField
 * @n@param const char * szField    : [参数描述]字段名称
 * @param   double       fNullValue : [参数描述]空值时返回的默认值
 * @return                            [返回值描述]字段值
 * @see                               [参见函数]
 * @n<b>作者</b>                    :
 * @n<b>创建时间</b>                : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
double CppSQLite3Query::GetFloatField(const char* szField, double fNullValue/*=0.0*/)
{
	int nField = FieldIndex(szField);
	return GetFloatField(nField, fNullValue);
}


/**
 * @brief [函数简要描述]获取字符串字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                 : GetStringField
 * @n@param int          nField      : [参数描述]字段索引
 * @param   const char * szNullValue : [参数描述]空值时返回的默认值
 * @return                             [返回值描述]字段值
 * @see                                [参见函数]
 * @n<b>作者</b>                     :
 * @n<b>创建时间</b>                 :
 * @version    修改者        时间        描述@n
 * @n          徐情波     2009-01-04     添加注释
 */
const char* CppSQLite3Query::GetStringField(int nField, const char* szNullValue/*=""*/)
{
	if (FieldDataType(nField) == SQLITE_NULL)
	{
		return szNullValue;
	}
	else
	{
		return (const char*)sqlite3_column_text(m_pVM, nField);
	}
}


/**
 * @brief [函数简要描述]获取字符串字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                 : GetStringField
 * @n@param const char * szField     : [参数描述]字段名称
 * @param   const char * szNullValue : [参数描述]
 * @return                             [返回值描述]
 * @see                                [参见函数]
 * @n<b>作者</b>                     :
 * @n<b>创建时间</b>                 : 2009-1-4 14:40:28
 * @version    修改者        时间        描述@n
 * @n          [修改者]      2009-01-04  [描述]
 */
const char* CppSQLite3Query::GetStringField(const char* szField, const char* szNullValue/*=""*/)
{
	int nField = FieldIndex(szField);
	return GetStringField(nField, szNullValue);
}


/**
 * @brief [函数简要描述]获取二进制字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>     : GetBlobField
 * @n@param int   nField : [参数描述]字段索引
 * @param   int & nLen   : [参数描述]返回二进制长度
 * @return                 [返回值描述]字段值
 * @see                    [参见函数]
 * @n<b>作者</b>         :
 * @n<b>创建时间</b>     :
 * @version    修改者        时间        描述@n
 * @n          徐情波     2009-01-04     添加注释
 */
const unsigned char* CppSQLite3Query::GetBlobField(int nField, int& nLen)
{
	CheckVM();
	if (nField < 0 || nField > m_nCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Invalid field index requested",
								DONT_DELETE_MSG);
	}

	nLen = sqlite3_column_bytes(m_pVM, nField);
	return (const unsigned char*)sqlite3_column_blob(m_pVM, nField);
}


/**
 * @brief [函数简要描述]获取二进制字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>             : GetBlobField
 * @n@param const char * szField : [参数描述]字段名称
 * @param   int &        nLen    : [参数描述]返回二进制长度
 * @return                         [返回值描述]字段值
 * @see                            [参见函数]
 * @n<b>作者</b>                 :
 * @n<b>创建时间</b>             : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
const unsigned char* CppSQLite3Query::GetBlobField(const char* szField, int& nLen)
{
	int nField = FieldIndex(szField);
	return GetBlobField(nField, nLen);
}


/**
 * @brief [函数简要描述]判断字段是否为空
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>   : FieldIsNull
 * @n@param int nField : [参数描述]字段索引
 * @return               [返回值描述]空：true,不为空:false
 * @see                  [参见函数]
 * @n<b>作者</b>       :
 * @n<b>创建时间</b>   : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
bool CppSQLite3Query::FieldIsNull(int nField)
{
	return (FieldDataType(nField) == SQLITE_NULL);
}


/**
 * @brief [函数简要描述]判断字段是否为空
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>             : FieldIsNull
 * @n@param const char * szField : [参数描述]字段名称
 * @return                         [返回值描述]空:true,不为空:false
 * @see                            [参见函数]
 * @n<b>作者</b>                 :
 * @n<b>创建时间</b>             :
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
bool CppSQLite3Query::FieldIsNull(const char* szField)
{
	int nField = FieldIndex(szField);
	return (FieldDataType(nField) == SQLITE_NULL);
}


/**
 * @brief [函数简要描述]通过字段名称获取字段索引
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>             : FieldIndex
 * @n@param const char * szField : [参数描述]字段名称
 * @return                         [返回值描述]字段索引
 * @see                            [参见函数]
 * @n<b>作者</b>                 :
 * @n<b>创建时间</b>             : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
int CppSQLite3Query::FieldIndex(const char* szField)
{
	CheckVM();

	if (szField)
	{
		for (int nField = 0; nField < m_nCols; nField++)
		{
			const char* szTemp = sqlite3_column_name(m_pVM, nField);

			//if (_stricmp(szField, szTemp) == 0)
			if (strcmp(szField, szTemp) == 0)
			{
				return nField;
			}
		}
	}

	throw CppSQLite3Exception(CPPSQLITE_ERROR,
							(char*)"Invalid field name requested",
							DONT_DELETE_MSG);
}


/**
 * @brief [函数简要描述]通过字段索引获取字段名称
 * 
 * [函数详细描述]
 * @n<b>函数名称</b> : FieldName
 * @n@param int nCol : [参数描述]字段索引
 * @return             [返回值描述]字段名称
 * @see                [参见函数]
 * @n<b>作者</b>     :
 * @n<b>创建时间</b> : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
const char* CppSQLite3Query::FieldName(int nCol)
{
	CheckVM();

	if (nCol < 0 || nCol > m_nCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Invalid field index requested",
								DONT_DELETE_MSG);
	}

	return sqlite3_column_name(m_pVM, nCol);
}


/**
 * @brief [函数简要描述]获取该列在 CREATE TABLE 语句中声明的类型
 * 
 * [函数详细描述]
 * @n<b>函数名称</b> : FieldDeclType
 * @n@param int nCol : [参数描述]字段索引
 * @return             [返回值描述]类型名称
 * @see                [参见函数]
 * @n<b>作者</b>     :
 * @n<b>创建时间</b> :
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
const char* CppSQLite3Query::FieldDeclType(int nCol)
{
	CheckVM();

	if (nCol < 0 || nCol > m_nCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Invalid field index requested",
								DONT_DELETE_MSG);
	}

	return sqlite3_column_decltype(m_pVM, nCol);
}


/**
 * @brief [函数简要描述]获取字段数据类型
 * 
 * [函数详细描述]
 * @n<b>函数名称</b> : FieldDataType
 * @n@param int nCol : [参数描述]字段索引
 * @return             [返回值描述]类型名称
 * @see                [参见函数]
 * @n<b>作者</b>     :
 * @n<b>创建时间</b> :
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */

const char* fieldTypeNames[]= {
"INTEGER",
"FLOAT",
"TEXT",
"BLOB"
};

int CppSQLite3Query::FieldDataType(int nCol)
{
	CheckVM();

	if (nCol < 0 || nCol > m_nCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Invalid field index requested",
								DONT_DELETE_MSG);
	}
	return sqlite3_column_type(m_pVM, nCol);
}

int CppSQLite3Query::FieldDataRealType(int nCol)
{
	const char* field = sqlite3_column_decltype(m_pVM, nCol);
	int find =-1;
	for(int i =0; i < 4; i++)
	{
		if(strcmp(fieldTypeNames[i],field)==0)
		{
			find = i;
			break;
		}
	}
	if(find > 0)
		return find+1;
	return 5;
}


/**
 * @brief [函数简要描述]判断是否移到记录集的末端
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: Eof
 * @return            [返回值描述]是:true 否:false
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
bool CppSQLite3Query::Eof()
{
	if (m_pVM == NULL)
		return true;
	return m_bEof;
}


void CppSQLite3Query::SetPointRow(CppSQLite3Query& query )
{
	query.pointRow=query.pointRow++;
	
}

int CppSQLite3Query::GetPointRow()
{
	return pointRow;
	
}

/**
 * @brief [函数简要描述]移动到记录集下一行
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: NextRow
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Query::NextRow()
{
	CheckVM();

	int nRet = sqlite3_step(m_pVM);

	if (nRet == SQLITE_DONE)
	{
		// no rows
		this->Finalize();
		m_bEof = true;
	}
	else if (nRet == SQLITE_ROW)
	{
		// more rows, nothing to do
	}
	else
	{
		const char* szError=sqlite3_errmsg(m_pDB);
		sqlite3_finalize(m_pVM);
		m_pVM = NULL;
		throw CppSQLite3Exception(nRet,	(char*)szError, DONT_DELETE_MSG);
	}
}


/**
 * @brief [函数简要描述]释放资源
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: Finalize
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>:
 * @version    修改者        时间        描述@n
 * @n          徐情波     2009-01-04     添加注释
 */
void CppSQLite3Query::Finalize()
{
	if (m_pVM && m_bOwnVM)
	{
		int nRet = sqlite3_finalize(m_pVM);
		m_pVM = NULL;
		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(m_pDB);
			throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
		}
	}
}


/**
 * @brief [函数简要描述]判断m_pVM成员是否为空
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: CheckVM
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Query::CheckVM()
{
	if (m_pVM == NULL)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Null Virtual Machine pointer",
								DONT_DELETE_MSG);
	}
}


//=======================================CppSQLite3Table类======================================
//-----------------------------------------------------------------------------------------------
// 构造函数与析构函数
CppSQLite3Table::CppSQLite3Table()
{
	m_paszResults = NULL;
	m_nRows       = NULL;
	m_nCols       = NULL;
	m_nCurrentRow = NULL;
}


CppSQLite3Table::CppSQLite3Table(const CppSQLite3Table& rTable)
{
	m_paszResults = rTable.m_paszResults;
	const_cast<CppSQLite3Table&>(rTable).m_paszResults = NULL;
	m_nRows       = rTable.m_nRows;
	m_nCols       = rTable.m_nCols;
	m_nCurrentRow = rTable.m_nCurrentRow;
}


CppSQLite3Table::CppSQLite3Table(char** paszResults, int nRows, int nCols)
{
	m_paszResults = paszResults;
	m_nRows       = nRows;
	m_nCols       = nCols;
	m_nCurrentRow = NULL;
}


CppSQLite3Table::~CppSQLite3Table()
{
	try
	{
		Finalize();
	}
	catch (...)
	{
	}
}


CppSQLite3Table& CppSQLite3Table::operator=(const CppSQLite3Table& rTable)
{
	try
	{
		Finalize();
	}
	catch (...)
	{
	}
	m_paszResults = rTable.m_paszResults;
	// Only one object can own the results
	const_cast<CppSQLite3Table&>(rTable).m_paszResults = NULL;
	m_nRows       = rTable.m_nRows;
	m_nCols       = rTable.m_nCols;
	m_nCurrentRow = rTable.m_nCurrentRow;
	return *this;
}


/**
 * @brief [函数简要描述]释放资源
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: Finalize
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Table::Finalize()
{
	if (m_paszResults)
	{
		sqlite3_free_table(m_paszResults);
		m_paszResults = NULL;
	}
}


/**
 * @brief [函数简要描述]获取字段数目
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: NumFields
 * @return            [返回值描述]字段数目
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>:
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
int CppSQLite3Table::NumFields()
{
	CheckResults();
	return m_nCols;
}


/**
 * @brief [函数简要描述]获取行数
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: NumRows
 * @return            [返回值描述]行数
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
int CppSQLite3Table::NumRows()
{
	CheckResults();
	return m_nRows;
}

/**
 * @brief [函数简要描述]获取字段值（字符串表示）
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>   : FieldValue
 * @n@param int nField : [参数描述]字段索引
 * @return               [返回值描述]字段值
 * @see                  [参见函数]
 * @n<b>作者</b>       :
 * @n<b>创建时间</b>   : 
 * @version    修改者        时间        描述@n
 * @n          徐情波     2009-01-04     添加注释
 */
const char* CppSQLite3Table::FieldValue(int nField)
{
	CheckResults();

	if (nField < 0 || nField > m_nCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Invalid field index requested",
								DONT_DELETE_MSG);
	}

	int nIndex = (m_nCurrentRow*m_nCols) + m_nCols + nField;
	return m_paszResults[nIndex];
}


/**
 * @brief [函数简要描述]获取字段值（字符串表示）
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>             : FieldValue
 * @n@param const char * szField : [参数描述]字段名称
 * @return                         [返回值描述]字段值
 * @see                            [参见函数]
 * @n<b>作者</b>                 :
 * @n<b>创建时间</b>             : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
const char* CppSQLite3Table::FieldValue(const char* szField)
{
	CheckResults();

	if (szField)
	{
		for (int nField = 0; nField < m_nCols; nField++)
		{
			//if (_stricmp(szField, m_paszResults[nField]) == 0)
			if (strcmp(szField, m_paszResults[nField]) == 0)
			{
				int nIndex = (m_nCurrentRow*m_nCols) + m_nCols + nField;
				return m_paszResults[nIndex];
			}
		}
	}

	throw CppSQLite3Exception(CPPSQLITE_ERROR,
							(char*)"Invalid field name requested",
							DONT_DELETE_MSG);
}


/**
 * @brief [函数简要描述]获取整型字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>       : GetIntField
 * @n@param int nField     : [参数描述]字段索引
 * @param   int nNullValue : [参数描述]字段为空时返回的默认值
 * @return                   [返回值描述]字段值
 * @see                      [参见函数]
 * @n<b>作者</b>           :
 * @n<b>创建时间</b>       :
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
int CppSQLite3Table::GetIntField(int nField, int nNullValue/*=0*/)
{
	if (FieldIsNull(nField))
	{
		return nNullValue;
	}
	else
	{
		return atoi(FieldValue(nField));
	}
}


/**
 * @brief [函数简要描述]获取整型字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                : GetIntField
 * @n@param const char * szField    : [参数描述]字段名称
 * @param   int          nNullValue : [参数描述]字段为空时返回的默认值
 * @return                            [返回值描述]字段值
 * @see                               [参见函数]
 * @n<b>作者</b>                    :
 * @n<b>创建时间</b>                :
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
int CppSQLite3Table::GetIntField(const char* szField, int nNullValue/*=0*/)
{
	if (FieldIsNull(szField))
	{
		return nNullValue;
	}
	else
	{
		return atoi(FieldValue(szField));
	}
}


/**
 * @brief [函数简要描述]获取浮点字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>          : GetFloatField
 * @n@param int    nField     : [参数描述]字段索引
 * @param   double fNullValue : [参数描述]字段为空时返回的默认值
 * @return                      [返回值描述]字段值
 * @see                         [参见函数]
 * @n<b>作者</b>              :
 * @n<b>创建时间</b>          :
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
double CppSQLite3Table::GetFloatField(int nField, double fNullValue/*=0.0*/)
{
	if (FieldIsNull(nField))
	{
		return fNullValue;
	}
	else
	{
		return atof(FieldValue(nField));
	}
}


/**
 * @brief [函数简要描述]获取浮点字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                : GetFloatField
 * @n@param const char * szField    : [参数描述]字段名称
 * @param   double       fNullValue : [参数描述]字段为空时返回的默认值
 * @return                            [返回值描述]字段值
 * @see                               [参见函数]
 * @n<b>作者</b>                    :
 * @n<b>创建时间</b>                : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
double CppSQLite3Table::GetFloatField(const char* szField, double fNullValue/*=0.0*/)
{
	if (FieldIsNull(szField))
	{
		return fNullValue;
	}
	else
	{
		return atof(FieldValue(szField));
	}
}


/**
 * @brief [函数简要描述]获取字符串字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                 : GetStringField
 * @n@param int          nField      : [参数描述]字段索引
 * @param   const char * szNullValue : [参数描述]字段为空时返回的默认值
 * @return                             [返回值描述]字段值
 * @see                                [参见函数]
 * @n<b>作者</b>                     :
 * @n<b>创建时间</b>                 : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
const char* CppSQLite3Table::GetStringField(int nField, const char* szNullValue/*=""*/)
{
	if (FieldIsNull(nField))
	{
		return szNullValue;
	}
	else
	{
		return FieldValue(nField);
	}
}


/**
 * @brief [函数简要描述]获取字符串字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                 : GetStringField
 * @n@param const char * szField     : [参数描述]字段名称
 * @param   const char * szNullValue : [参数描述]字段为空时返回的默认值
 * @return                             [返回值描述]字段值
 * @see                                [参见函数]
 * @n<b>作者</b>                     :
 * @n<b>创建时间</b>                 : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
const char* CppSQLite3Table::GetStringField(const char* szField, const char* szNullValue/*=""*/)
{
	if (FieldIsNull(szField))
	{
		return szNullValue;
	}
	else
	{
		return FieldValue(szField);
	}
}


/**
 * @brief [函数简要描述]判断字段是否为空
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>   : FieldIsNull
 * @n@param int nField : [参数描述]字段索引
 * @return               [返回值描述]是:true,否:false
 * @see                  [参见函数]
 * @n<b>作者</b>       :
 * @n<b>创建时间</b>   : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
bool CppSQLite3Table::FieldIsNull(int nField)
{
	CheckResults();
	return (FieldValue(nField) == NULL);
}


/**
 * @brief [函数简要描述]判断字段是否为空
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>             : FieldIsNull
 * @n@param const char * szField : [参数描述]字段名称
 * @return                         [返回值描述]是:true,否:false
 * @see                            [参见函数]
 * @n<b>作者</b>                 :
 * @n<b>创建时间</b>             :
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
bool CppSQLite3Table::FieldIsNull(const char* szField)
{
	CheckResults();
	return (FieldValue(szField) == NULL);
}


/**
 * @brief [函数简要描述]通过字段索引返回字段名称
 * 
 * [函数详细描述]
 * @n<b>函数名称</b> : FieldName
 * @n@param int nCol : [参数描述]字段索引
 * @return             [返回值描述]字段名称
 * @see                [参见函数]
 * @n<b>作者</b>     :
 * @n<b>创建时间</b> : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
const char* CppSQLite3Table::FieldName(int nCol)
{
	CheckResults();

	if (nCol < 0 || nCol > m_nCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Invalid field index requested",
								DONT_DELETE_MSG);
	}

	return m_paszResults[nCol];
}


/**
 * @brief [函数简要描述]设置当前行位置
 * 
 * [函数详细描述]
 * @n<b>函数名称</b> : SetRow
 * @n@param int nRow : [参数描述]当前行位置
 * @see                [参见函数]
 * @n<b>作者</b>     :
 * @n<b>创建时间</b> : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Table::SetRow(int nRow)
{
	CheckResults();

	if (nRow < 0 || nRow > m_nRows-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Invalid row index requested",
								DONT_DELETE_MSG);
	}

	m_nCurrentRow = nRow;
}

/**
 * @brief [函数简要描述]检测数据集是否为空
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: CheckResults
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Table::CheckResults()
{
	if (m_paszResults == NULL)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Null Results pointer",
								DONT_DELETE_MSG);
	}
}

//=======================================CppSQLite3Statement类======================================
//-----------------------------------------------------------------------------------------------
// 构造函数与析构函数
CppSQLite3Statement::CppSQLite3Statement()
{
	m_pDB = NULL;
	m_pVM = NULL;
}

CppSQLite3Statement::CppSQLite3Statement(const CppSQLite3Statement& rStatement)
{
	m_pDB = rStatement.m_pDB;
	m_pVM = rStatement.m_pVM;
	// Only one object can own VM
	const_cast<CppSQLite3Statement&>(rStatement).m_pVM = 0;
}

CppSQLite3Statement::CppSQLite3Statement(sqlite3* pDB, sqlite3_stmt* pVM)
{
	m_pDB = pDB;
	m_pVM = pVM;
}

CppSQLite3Statement::~CppSQLite3Statement()
{
	try
	{
		Finalize();
	}
	catch (...)
	{
	}
}

CppSQLite3Statement& CppSQLite3Statement::operator=(const CppSQLite3Statement& rStatement)
{
	m_pDB = rStatement.m_pDB;
	m_pVM = rStatement.m_pVM;
	// Only one object can own VM
	const_cast<CppSQLite3Statement&>(rStatement).m_pVM = 0;
	return *this;
}

/**
 * @brief [函数简要描述]执行SQL语句
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: ExecDML
 * @return            [返回值描述]执行SQL语句后影响的行数
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
int CppSQLite3Statement::ExecDML()
{
	CheckDB();
	CheckVM();
	
	const char* szError = NULL;

	int nRet = sqlite3_step(m_pVM);
	if (nRet == SQLITE_DONE)
	{
		int nRowsChanged = sqlite3_changes(m_pDB);
		nRet = sqlite3_reset(m_pVM);
		if (nRet != SQLITE_OK)
		{
			szError = sqlite3_errmsg(m_pDB);
			throw CppSQLite3Exception(nRet, (char*)szError,DONT_DELETE_MSG);
		}
		return nRowsChanged;
	}
	else
	{
		nRet = sqlite3_reset(m_pVM);
		szError = sqlite3_errmsg(m_pDB);
		throw CppSQLite3Exception(nRet, (char*)szError,DONT_DELETE_MSG);
	}
}

/**
 * @brief [函数简要描述]执行SQL查询语句
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: ExecQuery
 * @return            [返回值描述]执行sql语句后返回的记录集
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
CppSQLite3Query CppSQLite3Statement::ExecQuery()
{
	CheckDB();
	CheckVM();

	int nRet = sqlite3_step(m_pVM);
	if (nRet == SQLITE_DONE)
	{
		return CppSQLite3Query(m_pDB, m_pVM, true/*eof*/, false);
	}
	else if (nRet == SQLITE_ROW)
	{
		return CppSQLite3Query(m_pDB, m_pVM, false/*eof*/, false);
	}
	else
	{
		nRet = sqlite3_reset(m_pVM);
		const char* szError = sqlite3_errmsg(m_pDB);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}
}

/**
 * @brief [函数简要描述]添加字符串字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>             : Bind
 * @n@param int          nParam  : [参数描述]字段在sql语句中的索引
 * @param   const char * szValue : [参数描述]欲添加值
 * @see                            [参见函数]
 * @n<b>作者</b>                 :
 * @n<b>创建时间</b>             : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Statement::Bind(int nParam, const char* szValue)
{
	CheckVM();

	int nRes = sqlite3_bind_text(m_pVM, nParam, szValue, -1, SQLITE_TRANSIENT);
	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,
								(char*)"Error binding string param",
								DONT_DELETE_MSG);
	}
}

/**
 * @brief [函数简要描述]添加整型字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>         : Bind
 * @n@param int       nParam : [参数描述]字段在sql语句中的索引
 * @param   const int nValue : [参数描述]欲添加值
 * @see                        [参见函数]
 * @n<b>作者</b>             :
 * @n<b>创建时间</b>         : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Statement::Bind(int nParam, const int nValue)
{
	CheckVM();

	int nRes = sqlite3_bind_int(m_pVM, nParam, nValue);
	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,
								(char*)"Error binding int param",
								DONT_DELETE_MSG);
	}
}

/**
 * @brief [函数简要描述]添加浮点型字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>             : Bind
 * @n@param int          nParam  : [参数描述]字段在sql语句中的索引
 * @param   const double dwValue : [参数描述]欲添加值
 * @see                            [参见函数]
 * @n<b>作者</b>                 :
 * @n<b>创建时间</b>             :
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Statement::Bind(int nParam, const double dValue)
{
	CheckVM();

	int nRes = sqlite3_bind_double(m_pVM, nParam, dValue);
	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,
								(char*)"Error binding double param",
								DONT_DELETE_MSG);
	}
}

/**
 * @brief [函数简要描述]添加二进制字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                        : Bind
 * @n@param int                   nParam    : [参数描述]字段在sql语句中的索引
 * @param   const unsigned char * blobValue : [参数描述]二进制数据
 * @param   int                   nLen      : [参数描述]二进制数据长度
 * @see                                       [参见函数]
 * @n<b>作者</b>                            :
 * @n<b>创建时间</b>                        : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Statement::Bind(int nParam, const unsigned char* blobValue, int nLen)
{
	CheckVM();

	int nRes = sqlite3_bind_blob(m_pVM, nParam,
								(const void*)blobValue, nLen, SQLITE_TRANSIENT);
	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,
								(char*)"Error binding blob param",
								DONT_DELETE_MSG);
	}
}
	
/**
 * @brief [函数简要描述]添加空值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>   : BindNull
 * @n@param int nParam : [参数描述]
 * @see                  [参见函数]
 * @n<b>作者</b>       :
 * @n<b>创建时间</b>   : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Statement::BindNull(int nParam)
{
	CheckVM();

	int nRes = sqlite3_bind_null(m_pVM, nParam);
	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,
								(char*)"Error binding NULL param",
								DONT_DELETE_MSG);
	}
}

/**
 * @brief [函数简要描述]重新初始化SQL解析对象
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: Reset
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波     2009-01-04     添加注释
 */
void CppSQLite3Statement::Reset()
{
	if (m_pVM)
	{
		int nRet = sqlite3_reset(m_pVM);
		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(m_pDB);
			throw CppSQLite3Exception(nRet, (char*)szError,DONT_DELETE_MSG);
		}
	}
}

/**
 * @brief [函数简要描述]释放资源
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: Finalize
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波     2009-01-04     添加注释
 */
void CppSQLite3Statement::Finalize()
{
	if (m_pVM)
	{
		int nRet = sqlite3_finalize(m_pVM);
		m_pVM = NULL;
		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(m_pDB);
			throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
		}
	}
}

/**
 * @brief [函数简要描述]检测m_pDB数据库对象是否为空
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: CheckDB
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>:
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Statement::CheckDB()
{
	if (m_pDB == NULL)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Database not open",
								DONT_DELETE_MSG);
	}
}

/**
 * @brief [函数简要描述]检测m_pVM对象是否为空
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: CheckVM
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Statement::CheckVM()
{
	if (m_pVM == NULL)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Null Virtual Machine pointer",
								DONT_DELETE_MSG);
	}
}


//-----------------------------------------------------------------------------------------------

//=======================================CppSQLite3Statement类======================================
//-----------------------------------------------------------------------------------------------
// 构造函数与析构函数

CppSQLite3DB::CppSQLite3DB()
{
	m_pDB            = NULL;

}


CppSQLite3DB::CppSQLite3DB(const CppSQLite3DB& db)
{
	m_pDB            = db.m_pDB;

}


CppSQLite3DB::~CppSQLite3DB()
{
	
	Close();

}


CppSQLite3DB& CppSQLite3DB::operator=(const CppSQLite3DB& db)
{
	m_pDB            = db.m_pDB;
	return *this;

}

/**
 * @brief [函数简要描述]打开数据库
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>            : Open
 * @n@param const char * szFile : [参数描述]数据库路径
 * @return                        [返回值描述]成功：true
 * @see                           [参见函数]
 * @n<b>作者</b>                :
 * @n<b>创建时间</b>            : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
bool CppSQLite3DB::Open(const char *szFile )
{

	int nRet = sqlite3_open(szFile , &m_pDB);
	if (nRet != SQLITE_OK)
	{
		const char* szError = sqlite3_errmsg(m_pDB);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}

	//SetBusyTimeout(m_nBusyTimeoutMs);
	return true;
}

bool  CppSQLite3DB::Open(const char *szFile, const char* szKey)
{
	int nRet = sqlite3_open(szFile , &m_pDB);
	if (nRet != SQLITE_OK)
	{
		const char* szError = sqlite3_errmsg(m_pDB);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}

	//SetBusyTimeout(m_nBusyTimeoutMs);

#ifdef SQLITE_HAS_CODEC
	if (szKey != NULL && szKey != "")
	{
		bool bRet = this->SetKey (szKey, strlen(szKey));
		if (!bRet)
		{
			this->Close();
			throw CppSQLite3Exception(CPPSQLITE_ERROR, (char*)"key is not right!", DONT_DELETE_MSG);
		}
		
	}
#endif
	return true;


}

/**
 * @brief [函数简要描述]判断数据库是否打开
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: IsOpen
 * @return            [返回值描述]是:true,否:false
 * @see               [参见函数]
 * @n<b>作者</b>    : 徐情波
 * @n<b>创建时间</b>: 2009-1-4 16:14:01
 * @version    修改者        时间        描述@n
 * @n          
 */
bool CppSQLite3DB::IsOpen()
{
    return m_pDB != NULL;
}

#ifdef SQLITE_HAS_CODEC

/**
 * @brief [函数简要描述]设置密码
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>           : SetKey
 * @n@param const char * szKey : [参数描述]密码
 * @param   int          nKey  : [参数描述]密码长度
 * @return                       [返回值描述]
 * @see                          [参见函数]设置是否成功
 * @n<b>作者</b>               : 徐情波
 * @n<b>创建时间</b>           : 2009-1-19 18:22:55
 * @version    修改者        时间        描述@n
 * @n          [修改者]      2009-01-19  [描述]
 */
bool  CppSQLite3DB::SetKey(const char *szKey, int nKey)         //设置密码
{
	if (!IsOpen())
		return false;
	sqlite3_key(m_pDB, szKey, nKey);
	try
	{
		Cmd("select count(*) from sqlite_temp_master");
		this->ClearResult ();
	}
	catch (CppSQLite3Exception& /*e*/)
	{
		return false;
	}
	return true;
}
#endif

#ifdef SQLITE_HAS_CODEC
/**
 * @brief [函数简要描述]修改密码
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>           : ResetKey
 * @n@param const char * szKey : [参数描述]新密码
 * @param   int          nKey  : [参数描述]新密码长度
 * @return                       [返回值描述]修改是否成功
 * @see                          [参见函数]
 * @n<b>作者</b>               : 徐情波
 * @n<b>创建时间</b>           : 2009-1-19 18:23:33
 * @version    修改者        时间        描述@n
 * @n          [修改者]      2009-01-19  [描述]
 */
bool  CppSQLite3DB::ResetKey(const char *szKey, int nKey)
{
	if (!IsOpen())
		return false;
	
	if( strcmp(szKey,"")==0 )
		sqlite3_rekey(m_pDB, NULL, nKey);
	else
	    sqlite3_rekey(m_pDB, szKey, nKey);
	try
	{
		Cmd("select count(*) from sqlite_temp_master");
	}
	catch (CppSQLite3Exception& /*e*/)
	{
		return false;
	}
	return true;
}
#endif

/**
 * @brief [函数简要描述]关闭数据库
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: Close
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3DB::Close()
{

	if (m_pDB)
	{
		bool bHas = false;

        //清除由cmd()函数产生的所有数据集
		bHas = m_mapRes.GetFirstItem ();
		while (bHas)
		{
			CppSQLite3Query* pResultSet = (CppSQLite3Query*)m_mapRes.GetItem ();
			if (pResultSet)
			{
				pResultSet->Finalize();
				delete pResultSet;
				pResultSet = NULL;

			}
			bHas = m_mapRes.NextItem ();

		}
		m_mapRes.RemoveAll ();

		//清除由AddEidt()函数产生的所有CppSQLite3Statement指针
		bHas = m_mapStmts.GetFirstItem ();
		while (bHas)
		{
			CppSQLite3Statement* pStmt = (CppSQLite3Statement*)m_mapStmts.GetItem ();
			if (pStmt)
			{
				pStmt->Finalize();
				delete pStmt;
				pStmt = NULL;

			}
			bHas = m_mapStmts.NextItem ();

		}
		m_mapStmts.RemoveAll ();
	    
		//清楚事务处理标志空间
		m_mapTrans.RemoveAll ();
        
		//关闭数据库
		sqlite3_close(m_pDB);
		m_pDB = NULL;
		
	}
}

void  CppSQLite3DB::Interrupt()
{
	sqlite3_interrupt(m_pDB);
}      

/**
 * @brief [函数简要描述]判断表是否存在
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>             : TableExists
 * @n@param const char * szTable : [参数描述]表名
 * @return                         [返回值描述]是:true ,否:false
 * @see                            [参见函数]
 * @n<b>作者</b>                 :
 * @n<b>创建时间</b>             : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
bool CppSQLite3DB::TableExists(const char* szTable)
{
	char szSQL[128];

	//sprintf_s(szSQL, 128,
	//		"select count(*) from sqlite_master where type='table' and name='%s'",
	//		szTable);
	sprintf(szSQL,
		"select count(*) from sqlite_master where type='table' and name='%s'",
		szTable);
	int nRet = ExecScalar(szSQL);
	return (nRet > 0);
}

/**
 * @brief [函数简要描述]解析SQL语句
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>           : CompileStatement
 * @n@param const char * szSQL : [参数描述]SQL语句
 * @return                       [返回值描述]返回解析后的SQL对象
 * @see                          [参见函数]
 * @n<b>作者</b>               :
 * @n<b>创建时间</b>           : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释,增加多线程处理
 */
CppSQLite3Statement CppSQLite3DB::CompileStatement(const char* szSQL)
{
	CheckDB();

	sqlite3_stmt* pVM = Compile(szSQL);
	return CppSQLite3Statement(m_pDB, pVM);
}


/**
 * @brief [函数简要描述]执行SQL语句
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>           : ExecDML
 * @n@param const char * szSQL : [参数描述]SQL语句
 * @return                       [返回值描述]执行sql语句后影响的行数
 * @see                          [参见函数]
 * @n<b>作者</b>               :
 * @n<b>创建时间</b>           : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释,增加多线程处理
 */
int CppSQLite3DB::ExecDML(const char* szSQL)
{
	CheckDB();
	char* szError=0;

	int nRet = sqlite3_exec(m_pDB, szSQL, 0, 0, &szError);
	if (nRet == SQLITE_OK)
	{
		int nchanges=sqlite3_changes(m_pDB);
		return nchanges;
	}
	else
	{

		throw CppSQLite3Exception(nRet, szError);
	}

}


/**
 * @brief [函数简要描述]执行SQL查询语句
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>           : ExecQuery
 * @n@param const char * szSQL : [参数描述]sql语句
 * @return                       [返回值描述]查询后记录集
 * @see                          [参见函数]
 * @n<b>作者</b>               :
 * @n<b>创建时间</b>           :
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释,增加多线程处理
 */
CppSQLite3Query CppSQLite3DB::ExecQuery(const char* szSQL)
{
	CheckDB();

	sqlite3_stmt* pVM = Compile(szSQL);
	int nRet = sqlite3_step(pVM);
	if (nRet == SQLITE_DONE)
	{
		// no rows
		return CppSQLite3Query(m_pDB, pVM, true/*eof*/);

	}
	else if (nRet == SQLITE_ROW)
	{
		// at least 1 row
		return CppSQLite3Query(m_pDB, pVM, false/*eof*/);
	}
	else
	{
		nRet = sqlite3_finalize(pVM);
		const char* szError = sqlite3_errmsg(m_pDB);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}
}

/**
 * @brief [函数简要描述]执行返回单个数值的SQL语句
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>           : ExecScalar
 * @n@param const char * szSQL : [参数描述]SQL语句
 * @return                       [返回值描述]执行SQL语句后返回的单个数值
 * @see                          [参见函数]
 * @n<b>作者</b>               :
 * @n<b>创建时间</b>           : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
int CppSQLite3DB::ExecScalar(const char* szSQL)
{
	CppSQLite3Query q = ExecQuery(szSQL);

	if (q.Eof() || q.NumFields() < 1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Invalid scalar query",
								DONT_DELETE_MSG);
	}

	return atoi(q.FieldValue(0));
}

unsigned long CppSQLite3DB::MyGetCurrentThreadId()
{
#ifdef WIN32
	return ::GetCurrentThreadId();
#else
	return (unsigned long)pthread_self();
#endif
}

/**
 * @brief [函数简要描述]执行sql语句
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>         : Cmd
 * @n@param const char * fmt : [参数描述]sql语句（不定长变量）
 * @param   ...              : [参数描述]
 * @see                        [参见函数]
 * @n<b>作者</b>             :
 * @n<b>创建时间</b>         : 
 * @version    修改者        时间        描述@
 * @n          徐情波    2009-01-04      添加注释，并修改其中错误,增加多线程处理
 */
int CppSQLite3DB::Cmd(const char *fmt, ...)
{
	 CheckDB();

    if (fmt == NULL)
    {
		throw CppSQLite3Exception(CPPSQLITE_ERROR, (char*)"SQL Statement is empty", DONT_DELETE_MSG);
	    return 0;
	}
	std::string temp = fmt;
	temp.erase(0, temp.find_first_not_of(" "));
    int first_location=temp.find_first_of(" ");
	std::string head_word=temp.substr(0,first_location);
      
	//--------格式化字符串---------------
	va_list s1;
    va_start(s1, fmt);
	char *pBuff = sqlite3_vmprintf(fmt, s1);
    va_end(s1);

	string str_buf = pBuff;
	sqlite3_free(pBuff);
	//------------------------------------------
    transform (head_word.begin(),head_word.end(), head_word.begin(), ::tolower); //xuqb增加大小写转换

	
	if(head_word == "select")
	{
	   ClearResult ();
	   sqlite3_stmt* pVM = Compile(str_buf.c_str());
	   int nRet = sqlite3_step(pVM);
	   if (nRet == SQLITE_DONE)
	   {
		// no rows
		   m_mapRes.PutElement (MyGetCurrentThreadId(), new CppSQLite3Query(m_pDB, pVM, true/*eof*/));
		 
	   }
    	else if (nRet == SQLITE_ROW)
	   {
		// at least 1 row
		   m_mapRes.PutElement (MyGetCurrentThreadId(), new CppSQLite3Query(m_pDB, pVM, false/*eof*/));

	   }
	   else
	   {
		 sqlite3_finalize(pVM);
		 const char* szError = sqlite3_errmsg(m_pDB);
		 throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	   }
	   return 0;

	}
	else
	{
		
       char* szError = NULL;
	   int nRet = sqlite3_exec(m_pDB, str_buf.c_str(), 0, 0, &szError);
	   if (nRet != SQLITE_OK)
	   {
		   throw CppSQLite3Exception(nRet, szError);
       }
	   return sqlite3_changes(m_pDB);
	}
}

int  CppSQLite3DB::Cmd(const char* fmt, va_list args)
{
	CheckDB();

    if (fmt == NULL)
    {
		throw CppSQLite3Exception(CPPSQLITE_ERROR, (char*)"SQL Statement is empty", DONT_DELETE_MSG);
	    return 0;
	}
	std::string temp = fmt;
	temp.erase(0, temp.find_first_not_of(" "));
    int first_location=temp.find_first_of(" ");
	std::string head_word=temp.substr(0,first_location);
      
	//--------格式化字符串---------------
	char *pBuff = sqlite3_vmprintf(fmt, args);

	string str_buf = pBuff;
	sqlite3_free(pBuff);
	//------------------------------------------
    transform (head_word.begin(),head_word.end(), head_word.begin(), ::tolower); //xuqb增加大小写转换
	
	if(head_word =="select")
	{
	   this->ClearResult ();
	   sqlite3_stmt* pVM = Compile(str_buf.c_str());
	   int nRet = sqlite3_step(pVM);
	   if (nRet == SQLITE_DONE)
	   {
		// no rows
		   m_mapRes.PutElement (MyGetCurrentThreadId(), new CppSQLite3Query(m_pDB, pVM, true/*eof*/));
		 
	   }
    	else if (nRet == SQLITE_ROW)
	   {
		// at least 1 row
		  m_mapRes.PutElement (MyGetCurrentThreadId(), new CppSQLite3Query(m_pDB, pVM, false/*eof*/));

	   }
	   else
	   {
		 sqlite3_finalize(pVM);
		 const char* szError = sqlite3_errmsg(m_pDB);
		 throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	   }
	   return 0;

	}
	else
	{
		
       char* szError = NULL;
	   int nRet = sqlite3_exec(m_pDB, str_buf.c_str(), 0, 0, &szError);
	   if (nRet != SQLITE_OK)
	   {
		   throw CppSQLite3Exception(nRet, szError);
       }
	   return sqlite3_changes(m_pDB);
   }

}
/**
 * @brief [函数简要描述]清除由cmd命令返回的结果集
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: ClearResult
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 2009-1-7 20:24:58
 * @version    修改者        时间        描述@n
 * @n          [修改者]      2009-01-07  [描述]
 */
void  CppSQLite3DB::ClearResult()
{
	CppSQLite3Query* pResultSet = (CppSQLite3Query*)m_mapRes.GetElement(MyGetCurrentThreadId());
	if (pResultSet)
	{
		pResultSet->Finalize();
		delete pResultSet;
		pResultSet = NULL;

	}
	m_mapRes.RemoveAt (MyGetCurrentThreadId());
}

/**
 * @brief [函数简要描述]判断记录集是否还有下一行
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: More
 * @return            [返回值描述]是：true,否：false
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
bool CppSQLite3DB::More()
{
	CppSQLite3Query* pResultSet = CheckResultset();
	if (!pResultSet->Eof())
	{
		int pointRow = pResultSet->GetPointRow();
		if (pointRow == 0)
		{
			pResultSet->SetPointRow(*pResultSet);
			return true;
		}
		else
		{
			pResultSet->SetPointRow(*pResultSet);
			pResultSet->NextRow();
			if (!pResultSet->Eof())
				return true;
			else
                return false;
		}
	}
	else
	{
		return false;
	}
}

/**
 * @brief [函数简要描述]获取整型字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>       : GetLong
 * @n@param int nField     : [参数描述]字段索引
 * @param   int nNullValue : [参数描述]字段为空时返回的默认值
 * @return                   [返回值描述]字段值
 * @see                      [参见函数]
 * @n<b>作者</b>           :
 * @n<b>创建时间</b>       : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
long CppSQLite3DB::GetLong(int nField, int nNullValue/*=0*/)
{
	 CppSQLite3Query* pResultSet = CheckResultset();
	 return pResultSet->GetIntField(nField, nNullValue);
}


/**
 * @brief [函数简要描述]获取整型字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                : GetLong
 * @n@param const char * szField    : [参数描述]字段名称
 * @param   int          nNullValue : [参数描述]字段为空时返回的默认值
 * @return                            [返回值描述]字段值
 * @see                               [参见函数]
 * @n<b>作者</b>                    :
 * @n<b>创建时间</b>                : 
 * @version    修改者        时间        描述@n
 * @n          徐情波     2009-01-04     添加注释
 */
long CppSQLite3DB::GetLong(const char* szField, int nNullValue/*=0*/)
{
	CppSQLite3Query* pResultSet = CheckResultset();
	return pResultSet->GetIntField(szField, nNullValue);
}


/**
 * @brief [函数简要描述]获取浮点型字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>          : GetDouble
 * @n@param int    nField     : [参数描述]字段索引
 * @param   double fNullValue : [参数描述]字段为空时返回的默认值
 * @return                      [返回值描述]字段值
 * @see                         [参见函数]
 * @n<b>作者</b>              :
 * @n<b>创建时间</b>          : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
double CppSQLite3DB::GetDouble(int nField, double fNullValue/*=0.0*/)
{
	CppSQLite3Query* pResultSet = CheckResultset();
	return pResultSet->GetFloatField(nField, fNullValue);
}


/**
 * @brief [函数简要描述]获取浮点型字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                : GetDouble
 * @n@param const char * szField    : [参数描述]字段名称
 * @param   double       fNullValue : [参数描述]字段为空时返回的默认值
 * @return                            [返回值描述]字段值
 * @see                               [参见函数]
 * @n<b>作者</b>                    :
 * @n<b>创建时间</b>                : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
double CppSQLite3DB::GetDouble(const char* szField, double fNullValue/*=0.0*/)
{
	CppSQLite3Query* pResultSet = CheckResultset();
	return pResultSet->GetFloatField(szField, fNullValue);
}


/**
 * @brief [函数简要描述]获取字段值（字符串表示）
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                 : Get
 * @n@param int          nField      : [参数描述]字段索引
 * @param   const char * szNullValue : [参数描述]字段为空时返回的默认值
 * @return                             [返回值描述]字段值
 * @see                                [参见函数]
 * @n<b>作者</b>                     :
 * @n<b>创建时间</b>                 : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
const char* CppSQLite3DB::Get(int nField, const char* szNullValue/*=""*/)
{
	CppSQLite3Query* pResultSet = CheckResultset();
	return pResultSet->GetStringField(nField, szNullValue);
}


/**
 * @brief [函数简要描述]获取字段值（字符串表示）
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                 : Get
 * @n@param const char * szField     : [参数描述]字段名称
 * @param   const char * szNullValue : [参数描述]字段为空时返回的默认值
 * @return                             [返回值描述]字段值
 * @see                                [参见函数]
 * @n<b>作者</b>                     :
 * @n<b>创建时间</b>                 : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
const char* CppSQLite3DB::Get(const char* szField, const char* szNullValue/*=""*/)
{
	CppSQLite3Query* pResultSet = CheckResultset();
	return pResultSet->GetStringField(szField, szNullValue);
}

/**
 * @brief [函数简要描述]获取二进制字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>     : GetBlob
 * @n@param int   nField : [参数描述]字段索引
 * @param   int & nLen   : [参数描述]获取的二进制数据长度
 * @return                 [返回值描述]返回二进制数据
 * @see                    [参见函数]
 * @n<b>作者</b>         :
 * @n<b>创建时间</b>     : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
const unsigned char* CppSQLite3DB::GetBlob(int nField, int& nLen)
{
	CppSQLite3Query* pResultSet = CheckResultset();
	return pResultSet->GetBlobField(nField, nLen);
}
/**
 * @brief [函数简要描述]获取二进制字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>             : GetBlob
 * @n@param const char * szField : [参数描述]字段索引
 * @param   int &        nLen    : [参数描述]获取的二进制数据长度
 * @return                         [返回值描述]返回二进制数据
 * @see                            [参见函数]
 * @n<b>作者</b>                 :
 * @n<b>创建时间</b>             : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
const unsigned char* CppSQLite3DB::GetBlob(const char* szField, int& nLen)
{
	CppSQLite3Query* pResultSet = CheckResultset();
	return pResultSet->GetBlobField(szField, nLen);
}

int CppSQLite3DB::GetBlob(const char* szTable, const char* szField, const char* extra_condition, unsigned char* pData, int& nLen)
{
	if (szTable == NULL || szTable == "")
		return CPPSQLITE_ERROR;

	if (szField == NULL || szField == "")
		return CPPSQLITE_ERROR;

	sqlite3_stmt* pVM = NULL;
	try
	{
		string strSQL = "select  "+string(szField)+"  from "+string(szTable)+" where "+string(extra_condition)+"";
		sqlite3_stmt* pVM = Compile(strSQL.c_str());
	    int nRet = sqlite3_step(pVM);

	    if (nRet ==  SQLITE_ROW)
	    {
		   CppSQLite3Query resultSet =CppSQLite3Query(m_pDB, pVM, false);

		   int nLenTemp = 0;
		   const unsigned char* pszTemp = resultSet.GetBlobField (szField, nLenTemp);
		   
		   if (nLen > nLenTemp)
			   nLen = nLenTemp;
		   memcpy(pData, pszTemp, nLen);

		   resultSet.Finalize ();
	    }
	    else
	    {
		  sqlite3_finalize(pVM);
		  return nRet;
	    }
	}
	catch (...)
	{
		if (pVM != NULL)
			sqlite3_finalize(pVM);

		return CPPSQLITE_ERROR;
	}

	return 0;

}

int	CppSQLite3DB::NumFields()             //获取字段数目
{
	CppSQLite3Query* pResultSet = CheckResultset();
	return pResultSet->NumFields();
}

int	CppSQLite3DB::FieldIndex(const char* szField)                //通过字段名称获取字段索引值
{
	CppSQLite3Query* pResultSet = CheckResultset();
	return pResultSet->FieldIndex(szField);
}

const char* CppSQLite3DB::FieldName(int nCol)   //通过字段索引值获取字段名称
{
	CppSQLite3Query* pResultSet = CheckResultset();
	return pResultSet->FieldName(nCol);
}

const char* CppSQLite3DB::FieldDeclType(int nCol) //获取该列在 CREATE TABLE 语句中声明的类型
{
	CppSQLite3Query* pResultSet = CheckResultset();
	return pResultSet->FieldDeclType(nCol);
}

int	CppSQLite3DB::FieldDataType(int nCol)   //获取该列值的数据类型
{
	CppSQLite3Query* pResultSet = CheckResultset();
	return pResultSet->FieldDataType(nCol);
}
bool CppSQLite3DB::FieldIsNull(int nField)    //判断字段是否为空
{
	CppSQLite3Query* pResultSet = CheckResultset();
	return pResultSet->FieldIsNull(nField);
}

bool CppSQLite3DB::FieldIsNull(const char* szField)
{
	CppSQLite3Query* pResultSet = CheckResultset();
	return pResultSet->FieldIsNull(szField);
}

void CppSQLite3DB::AddEdit(const char* szSQL)  //执行sql语句
{
	CheckDB();
	ClearStmt();
	sqlite3_stmt* pVM = Compile(szSQL);
	m_mapStmts.PutElement (MyGetCurrentThreadId(), new CppSQLite3Statement(m_pDB, pVM));

}

void CppSQLite3DB::Bind(int nParam, const char* szValue)             //添加文本值
{
	CppSQLite3Statement* pStmt = CheckStmt();
	pStmt->Bind(nParam, szValue);
}

void CppSQLite3DB::Bind(int nParam, const int nValue)               //添加整型值
{
	CppSQLite3Statement* pStmt = CheckStmt();
	pStmt->Bind(nParam, nValue);
}

void CppSQLite3DB::Bind(int nParam, const double dwValue)           //添加浮点值
{
	CppSQLite3Statement* pStmt = CheckStmt();
	pStmt->Bind(nParam, dwValue);
}

void CppSQLite3DB::Bind(int nParam, const unsigned char* blobValue, int nLen)           //添加二进制值
{
	CppSQLite3Statement* pStmt = CheckStmt();
	pStmt->Bind(nParam, blobValue, nLen);
}

void CppSQLite3DB::BindNull(int nParam)          //添加空值
{
	CppSQLite3Statement* pStmt = CheckStmt();
	pStmt->BindNull(nParam);

}

int CppSQLite3DB::Update()
{
	CppSQLite3Statement* pStmt = CheckStmt();
	return pStmt->ExecDML();
}
void CppSQLite3DB::ClearStmt()
{  
	CppSQLite3Statement* pStmt = (CppSQLite3Statement*)m_mapStmts.GetElement (MyGetCurrentThreadId());
	if (pStmt)
	{
		pStmt->Finalize ();
		delete pStmt;
		pStmt = NULL;

	}
	m_mapStmts.RemoveAt (MyGetCurrentThreadId());

}
void CppSQLite3DB::ResetStmt()
{
	CppSQLite3Statement* pStmt = (CppSQLite3Statement*)m_mapStmts.GetElement (MyGetCurrentThreadId());
	if (pStmt)
	{
		pStmt->Reset ();
	}

}

/**
 * @brief [函数简要描述]保存二进制字段值
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>               : SaveBlob
 * @n@param const char * szSQL     : [参数描述]SQL语句
 * @param   const void * pBlobData : [参数描述]二进制数据
 * @param   int          nDataLen  : [参数描述]二进制数据长度
 * @return                           [返回值描述]所影响的行数
 * @see                              [参见函数]
 * @n<b>作者</b>                   : 徐情波
 * @n<b>创建时间</b>               : 2009-1-4 17:30:22
 * @version    修改者        时间        描述@n
 * @n          
 */
int CppSQLite3DB::SaveBlob(const char* szSQL, const void* pBlobData, int nDataLen)
{
	CheckDB();

	sqlite3_stmt* stmt = NULL;
	int nRet = 0;
	const char* szError = NULL;

	nRet = sqlite3_prepare(m_pDB, szSQL, -1, &stmt, 0);
	if (nRet != SQLITE_OK) 
	{
		szError=sqlite3_errmsg(m_pDB);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}

	// 绑定到BLOB字段
	nRet = sqlite3_bind_blob(stmt, 1, pBlobData, nDataLen, NULL);
	if (nRet != SQLITE_OK)
	{
		szError=sqlite3_errmsg(m_pDB);
		sqlite3_finalize(stmt);
		throw CppSQLite3Exception(nRet,(char*)szError, DONT_DELETE_MSG);
	}

	nRet = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (nRet == SQLITE_DONE || nRet == SQLITE_ROW || nRet == SQLITE_OK)	
	{
		return sqlite3_changes(m_pDB);
	}
	else
	{
		szError = sqlite3_errmsg(m_pDB);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}


}


/**
 * @brief [函数简要描述]执行SQL语句并获取静态记录集
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>           : GetTable
 * @n@param const char * szSQL : [参数描述]SQL语句
 * @return                       [返回值描述]记录集结果
 * @see                          [参见函数]
 * @n<b>作者</b>               :
 * @n<b>创建时间</b>           : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
CppSQLite3Table CppSQLite3DB::GetTable(const char* szSQL)
{
	CheckDB();

	char* szError      = NULL;
	char** paszResults = NULL;
	int nRet           = 0;
	int nRows          = 0;
	int nCols          = 0;
	int pointRow       = 0;

	nRet = sqlite3_get_table(m_pDB, szSQL, &paszResults, &nRows, &nCols, &szError);
	if (nRet == SQLITE_OK)
	{
		return CppSQLite3Table(paszResults, nRows, nCols);
	}
	else
	{
		throw CppSQLite3Exception(nRet, szError);
	}
}


/**
 * @brief [函数简要描述]返回最近的插入操作的整形键
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: LastRowId
 * @return            [返回值描述]最近的插入操作的整形键
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 2009-1-4 17:11:05
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
long CppSQLite3DB::LastRowId()
{
	CheckDB();
	return (long) sqlite3_last_insert_rowid(m_pDB);
}


/**
 * @brief [函数简要描述]设置数据库函数超时时间
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>       : SetBusyTimeout
 * @n@param int nMillisecs : [参数描述]超时时间（毫秒）
 * @see                      [参见函数]
 * @n<b>作者</b>           :
 * @n<b>创建时间</b>       : 2009-1-4 17:12:22
 * @version    修改者        时间        描述@n
 * @n          徐情波    2009-01-04      添加注释
 */
void CppSQLite3DB::SetBusyTimeout(int nMillisecs)
{
	CheckDB();
	sqlite3_busy_timeout(m_pDB, nMillisecs);
}


/**
 * @brief [函数简要描述]开始事务
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: BeginTransaction
 * @return            [返回值描述]
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释、增加多线程处理
 */
int CppSQLite3DB::BeginTransaction()
{
	CheckDB();

	if (m_mapTrans.GetElement (MyGetCurrentThreadId()) != NULL)
		return CPPSQLITE_ERROR;

	 this->Lock ();

	 int nRet = sqlite3_exec( m_pDB, "begin transaction", 0, 0, NULL );
	 if (nRet != SQLITE_OK)
	 {
		 this->UnLock ();
		  return nRet;
	 }

	 m_mapTrans.PutElement (MyGetCurrentThreadId(), (/*LPVOID*/void*)1);
	 return  SQLITE_OK;
	
}

/**
 * @brief [函数简要描述]执行事务
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: Commit
 * @return            [返回值描述]
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释,增加多线程处理
 */
int CppSQLite3DB::Commit()
{
	CheckDB(); 

	if (m_mapTrans.GetElement (MyGetCurrentThreadId()) == NULL)
		return CPPSQLITE_ERROR;

	int nRet = sqlite3_exec( m_pDB, "commit transaction", 0, 0, NULL );

	if (nRet == SQLITE_OK)
	{
		this->UnLock ();
		m_mapTrans.RemoveAt (MyGetCurrentThreadId());
	}
	
	return nRet;
}

/**
 * @brief [函数简要描述]事务回滚
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: Rollback
 * @return            [返回值描述]
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04     添加注释,增加多线程处理
 */
int CppSQLite3DB::Rollback()
{
	CheckDB();

	if (m_mapTrans.GetElement (MyGetCurrentThreadId()) == NULL)
		return CPPSQLITE_ERROR;

	int nRet = sqlite3_exec( m_pDB, "rollback transaction", 0, 0, NULL);

	this->UnLock ();
	m_mapTrans.RemoveAt (MyGetCurrentThreadId());
	
	return nRet;
}

/**
 * @brief [函数简要描述]压缩数据库
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: ContractDB
 * @return            [返回值描述]
 * @see               [参见函数]
 * @n<b>作者</b>    : 徐情波
 * @n<b>创建时间</b>: 2009-1-4 17:25:00
 * @version    修改者        时间        描述@n
 * @n         
 */
int CppSQLite3DB:: ContractDB()
{
	CheckDB();

	int nRet=sqlite3_exec( m_pDB, "VACUUM", 0, 0, NULL );

	return nRet;
}


void CppSQLite3DB::CheckDB()
{
	if (!m_pDB)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Database not open",
								DONT_DELETE_MSG);
	}
}

CppSQLite3Query* CppSQLite3DB::CheckResultset()
{
	CppSQLite3Query* pResultSet = (CppSQLite3Query*)m_mapRes.GetElement(MyGetCurrentThreadId());
	if (!pResultSet)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"NULL CppSQLite3Query Pointer",
								DONT_DELETE_MSG);
	}
	return pResultSet;
}

CppSQLite3Statement* CppSQLite3DB::CheckStmt()
{
	CppSQLite3Statement* pStmt = (CppSQLite3Statement*)m_mapStmts.GetElement (MyGetCurrentThreadId());
	if (!pStmt)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"NULL CppSQLite3Statement Pointer",
								DONT_DELETE_MSG);
	}
	return pStmt;
}

bool CppSQLite3DB::Lock()
{
	sqlite3_mutex* pmutex = sqlite3_db_mutex(m_pDB);
	if (pmutex != NULL)
	{
		sqlite3_mutex_enter(pmutex);
		return true;
	}
	return false;
	
}
bool CppSQLite3DB::UnLock()
{
	sqlite3_mutex* pmutex = sqlite3_db_mutex(m_pDB);
	if (pmutex != NULL)
	{
		sqlite3_mutex_leave(pmutex);
		return true;
	}
	return false;
}

/**
 * @brief [函数简要描述]解析SQL语句
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>           : Compile
 * @n@param const char * szSQL : [参数描述]SQL语句
 * @return                       [返回值描述]SQL语句解析对象
 * @see                          [参见函数]
 * @n<b>作者</b>               :
 * @n<b>创建时间</b>           : 2009-1-4 17:19:08
 * @version    修改者        时间        描述@n
 * @n          徐情波     2009-01-04     添加注释
 */
sqlite3_stmt* CppSQLite3DB::Compile(const char* szSQL)
{
	CheckDB();

	char* szError      = NULL;
	const char* szTail = NULL;
	sqlite3_stmt* pVM  = NULL;

	int nRet = sqlite3_prepare(m_pDB, szSQL, -1, &pVM, &szTail);
	if (nRet != SQLITE_OK)
	{
		const char* szError = sqlite3_errmsg(m_pDB);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}

	return pVM;
}


std::string  CppSQLite3DB::IntToStr( int  num)
{
	if (num  ==   0 )
       return   " 0 " ;
   
    std::string  str  =   "" ;
    int  num_  =  num  >   0   ?  num :  - 1   *  num;

    while (num_)
    {
      str  =  ( char )(num_  %   10   +   48 )  +  str;
      num_  /=   10 ;
    } 
 
    if (num  <  0 )
       str  =   " - "  +  str;

    return  str;
}

std::string CppSQLite3DB::ConvertToFormatTime(time_t& t)
{
#ifdef WIN32
	struct tm *local = NULL;
  localtime_s(local, &t);
#else
	struct tm *local = localtime(&t);
#endif
  
	string year    =  IntToStr(local->tm_year + 1900);
  string month   =  IntToStr(local->tm_mon + 1);
  string day     =  IntToStr(local->tm_mday);
	string hour    =  IntToStr(local->tm_hour);
	string min     =  IntToStr(local->tm_min);
	string sec     =  IntToStr(local->tm_sec);
	string timeStr = year + "-" + month + "-" + day + " " + hour + ":" + min + ":" + sec;

	return timeStr;
}

std::string  CppSQLite3DB::ConvertTimeToStr(time_t& t)
{
	char buf[128];
  //sprintf_s(buf, 128, "%d", t);
  sprintf(buf, "%ld", t);
	string timeStr = buf;
	return timeStr;
}

//-----------------------------------------------------------------------------------------------

//=======================================CppSQLite3Buffer类======================================
//-----------------------------------------------------------------------------------------------
// 构造函数与析构函数
CppSQLite3Buffer::CppSQLite3Buffer()
{
	m_pBuf = NULL;
}

CppSQLite3Buffer::~CppSQLite3Buffer()
{
	Clear();
}

/**
 * @brief [函数简要描述]释放并清空缓冲区
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: Clear
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Buffer::Clear()
{
	// 如果缓冲区指针不为空，则清空
	if (m_pBuf)
	{
		sqlite3_free(m_pBuf);
		m_pBuf = NULL;
	}
}


/**
 * @brief [函数简要描述]格式化不定长输入参数
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>              : Format
 * @n@param const char * szFormat : [参数描述]欲格式化的字符串
 * @param   ...                   : [参数描述]
 * @return                          [返回值描述]格式化后的字符串
 * @see                             [参见函数]
 * @n<b>作者</b>                  :
 * @n<b>创建时间</b>              : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    增加注释
 */
const char* CppSQLite3Buffer::Format(const char* szFormat, ...)
{
	Clear();
	va_list va;
	va_start(va, szFormat);
	m_pBuf = sqlite3_vmprintf(szFormat, va);
	va_end(va);

	return m_pBuf;
}

//=======================================CppSQLite3Binary类======================================
//-----------------------------------------------------------------------------------------------
// 构造函数与析构函数
CppSQLite3Binary::CppSQLite3Binary()
{
	m_pBuf        = NULL;
	m_nBinaryLen  = NULL;
	m_nBufferLen  = NULL;
	m_nEncodedLen = NULL;
	m_bEncoded    = false;
}

CppSQLite3Binary::~CppSQLite3Binary()
{
	Clear();
}

//-----------------------------------------------------------------------------------------------
// 成员函数

/**
 * @brief [函数简要描述]设置缓冲区
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                   : SetBinary
 * @n@param const unsigned char * pBuf : [参数描述]缓冲区
 * @param   int                   nLen : [参数描述]缓冲区大小
 * @see                                  [参见函数]
 * @n<b>作者</b>                       :
 * @n<b>创建时间</b>                   :
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Binary::SetBinary(const unsigned char* pBuf, int nLen)           
{
	m_pBuf = AllocBuffer(nLen);
	memcpy(m_pBuf, pBuf, nLen);
}

/**
 * @brief [函数简要描述]获取编码
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: GetBinary
 * @return            [返回值描述]编码字符串
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
const unsigned char* CppSQLite3Binary::GetBinary()
{
	if (!m_pBuf)
		return NULL;
	if (m_bEncoded)
	{
		// in/out buffers can be the same
		m_nBinaryLen = sqlite3_decode_binary(m_pBuf, m_pBuf);
		if (m_nBinaryLen == -1)
		{
			throw CppSQLite3Exception(CPPSQLITE_ERROR,
									(char*)"Cannot decode binary",
									DONT_DELETE_MSG);
		}
		m_bEncoded = false;
	}

	return m_pBuf;
}

/**
 * @brief [函数简要描述]获取二进制长度
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: SetBinaryLength
 * @return            [返回值描述]二进制长度
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
int CppSQLite3Binary::SetBinaryLength()
{
	// 获取二进制编码
	GetBinary();
	return m_nBinaryLen;
}

/**
 * @brief [函数简要描述]设置编码
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>                   : SetEncoded
 * @n@param const unsigned char * pBuf : [参数描述]
 * @see                                  [参见函数]
 * @n<b>作者</b>                       :
 * @n<b>创建时间</b>                   : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Binary::SetEncoded(const unsigned char* pBuf)
{
	Clear();

	m_nEncodedLen	= strlen((const char*)pBuf);
	m_nBufferLen	= m_nEncodedLen + 1;				// Allow for NULL terminator

	m_pBuf = (unsigned char*)malloc(m_nBufferLen);

	if( !m_pBuf )
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Cannot allocate memory",
								DONT_DELETE_MSG);
	}

	memcpy(m_pBuf, pBuf, m_nBufferLen);
	m_bEncoded = true;
}

//==============================================================================
/**
 * @brief [函数简要描述]获取编码
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: GetEncoded
 * @return            [返回值描述]
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>: 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
const unsigned char* CppSQLite3Binary::GetEncoded()
{
	if( !m_bEncoded )
	{
		unsigned char* ptmp = (unsigned char*)malloc(m_nBinaryLen);
		memcpy(ptmp, m_pBuf, m_nBinaryLen);
		m_nEncodedLen = sqlite3_encode_binary(ptmp, m_nBinaryLen, m_pBuf);
		free(ptmp);
		m_bEncoded = true;
	}

	return m_pBuf;
}

/**
 * @brief [函数简要描述]分配缓冲区大小
 * 
 * [函数详细描述]
 * @n<b>函数名称</b> : AllocBuffer
 * @n@param int nLen : [参数描述]缓冲区大小
 * @return             [返回值描述]分配的缓冲区
 * @see                [参见函数]
 * @n<b>作者</b>     :
 * @n<b>创建时间</b> : 
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
unsigned char* CppSQLite3Binary::AllocBuffer(int nLen)
{
	Clear();

	// Allow extra space for encoded binary as per comments in
	// SQLite encode.c See bottom of this file for implementation
	// of SQLite functions use 3 instead of 2 just to be sure ;-)
	m_nBinaryLen = nLen;
	m_nBufferLen = 3 + (257*nLen)/254;

	m_pBuf = (unsigned char*)malloc(m_nBufferLen);

	if( !m_pBuf )
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
								(char*)"Cannot allocate memory",
								DONT_DELETE_MSG);
	}

	m_bEncoded = false;

	return m_pBuf;
}

/**
 * @brief [函数简要描述]清空缓冲区
 * 
 * [函数详细描述]
 * @n<b>函数名称</b>: Clear
 * @see               [参见函数]
 * @n<b>作者</b>    :
 * @n<b>创建时间</b>:
 * @version    修改者        时间        描述@n
 * @n          徐情波      2009-01-04    添加注释
 */
void CppSQLite3Binary::Clear()
{
	if( m_pBuf )
	{
		m_nBinaryLen = 0;
		m_nBufferLen = 0;
		free(m_pBuf);
		m_pBuf       = NULL;
	}
}


//=======================================公共函数================================================
//-----------------------------------------------------------------------------------------------
int sqlite3_encode_binary(const unsigned char *in, int n, unsigned char *out)
{
  int i, j, e, m;
  int cnt[256];
  if ( n <= 0 )
  {
    out[0] = 'x';
    out[1] = 0;
    return 1;
  }

  memset(cnt, 0, sizeof(cnt));
  for (i=n-1; i>=0; i--)
  { 
	  cnt[in[i]]++; 
  }

  m = n;
  for (i=1; i<256; i++)
  {
    int sum;
    if ( i== '\'' )
		continue;
    sum = cnt[i] + cnt[(i + 1) & 0xff] + cnt[(i + '\'') & 0xff];
    if ( sum<m )
	{
      m = sum;
      e = i;
      if ( m==0 )
		  break;
    }
  }

  out[0] = e;
  j = 1;
  for (i=0; i<n; i++)
  {
    int c = (in[i] - e) & 0xff;
    if ( c == 0 )
	{
      out[j++] = 1;
      out[j++] = 1;
    }
	else if ( c == 1 )
	{
      out[j++] = 1;
      out[j++] = 2;
    }
	else if ( c == '\'' )
	{
      out[j++] = 1;
      out[j++] = 3;
    }
	else
	{
      out[j++] = c;
    }
  }
  out[j] = 0;

  return j;
}


int sqlite3_decode_binary(const unsigned char *in, unsigned char *out)
{
  int i, c, e;
  e = *(in++);
  i = 0;

  while ((c = *(in++)) != 0 )
  {
    if ( c == 1 )
	{
      c = *(in++);
      if ( c == 1 )
	  {
        c = 0;
      }
	  else if ( c == 2 )
	  {
        c = 1;
      }
	  else if( c == 3 )
	  {
        c = '\'';
      }
	  else
	  {
        return -1;
      }
    }
    out[i++] = (c + e)&0xff;
  }

  return i;
}

