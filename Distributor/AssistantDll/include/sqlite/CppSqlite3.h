/***************************************************************************//**
 *  @file 
 *  @brief [简要描述]sqlite数据库访问类
 * 
 *  <b>文件名</b>      : CppSQLite3.h
 *  @n@n<b>版权所有</b>: 网龙天晴程序部应用软件开发组
 *  @n@n<b>作  者</b>  :  徐情波
 *  @n@n<b>创建时间</b>: 2009-01-04
 *  @n@n<b>文件描述</b>: 
 *  @version  版本        修改者        时间        描述@n
 *  @n        v1.0.0      徐情波     2009-01-16     添加注释、整理结构、增加多线程访问机制
 * 
*******************************************************************************/

#ifndef	_TQ_CPPSQLITE3_H_	
#define _TQ_CPPSQLITE3_H_

#pragma warning(disable:4251)

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <map>
#ifdef WIN32
	#include <windows.h>
	#include "sqlite3.h"
#include "AssistantDll.h"
#else
	#include <pthread.h>
	#include <sqlite3.h>
#endif


/*以下是sqlite3的定义，为了减少引用sqlite3.h文件*/
#ifndef SQLITE_VERSION
#define SQLITE_VERSION         "3.7.9"
#endif
struct sqlite3;
struct sqlite3_stmt;
#define SQLITE_OK           0   /* Successful result */
#define SQLITE_ERROR        1   /* SQL error or missing database */
#define SQLITE_INTERNAL     2   /* Internal logic error in SQLite */
#define SQLITE_PERM         3   /* Access permission denied */
#define SQLITE_ABORT        4   /* Callback routine requested an abort */
#define SQLITE_BUSY         5   /* The database file is locked */
#define SQLITE_LOCKED       6   /* A table in the database is locked */
#define SQLITE_NOMEM        7   /* A malloc() failed */
#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
#define SQLITE_NOTFOUND    12   /* NOT USED. Table or record not found */
#define SQLITE_FULL        13   /* Insertion failed because database is full */
#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
#define SQLITE_PROTOCOL    15   /* NOT USED. Database lock protocol error */
#define SQLITE_EMPTY       16   /* Database is empty */
#define SQLITE_SCHEMA      17   /* The database schema changed */
#define SQLITE_TOOBIG      18   /* String or BLOB exceeds size limit */
#define SQLITE_CONSTRAINT  19   /* Abort due to constraint violation */
#define SQLITE_MISMATCH    20   /* Data type mismatch */
#define SQLITE_MISUSE      21   /* Library used incorrectly */
#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
#define SQLITE_AUTH        23   /* Authorization denied */
#define SQLITE_FORMAT      24   /* Auxiliary database format error */
#define SQLITE_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
#define SQLITE_NOTADB      26   /* File opened that is not a database file */
#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */
#define CPPSQLITE_ERROR    1000 /* 自定义错误 */       
/* end-of-error-codes */

#ifdef CPPSQLITE3_EXPORTS
#define CPPSQLITE3_API 
#else
#define CPPSQLITE3_API 
#endif

//=========================SAFE_MAP=======================
//安全哈希表
struct SAFE_MAP
{
public:
// 构造函数与析构函数
	SAFE_MAP();
	~SAFE_MAP();

// 成员函数
	/*LPVOID*/void* GetElement(/*DWORD*/unsigned long dwKey);                   //根据关键字获取值
	void   PutElement(/*DWORD*/unsigned long dwKey, /*LPVOID*/void* lpValue);   //添加项
	void   RemoveAt(/*DWORD*/unsigned long dwKey);                     //删除项
	void   RemoveAll();                               //删除所有项

	bool   GetFirstItem();                            //以下函数用于遍历哈希表
	bool   NextItem();
	/*LPVOID*/void* GetItem();

private:
	std::map</*DWORD*/unsigned long, /*LPVOID*/void*> m_map;
	std::map</*DWORD*/unsigned long, /*LPVOID*/void*>::iterator m_iter;
	void CriticalSection_Init();
	void CriticalSection_Delete();
	void CriticalSection_Enter();
	void CriticalSection_Leave();
#ifdef WIN32
		CRITICAL_SECTION        m_cs;
#else 
		pthread_mutex_t					m_cs;
#endif
};

//=========================CppSQLite3Exception=======================
//异常处理
class  CLASSINDLL_CLASS_DECL CPPSQLITE3_API CppSQLite3Exception
{
public:
// 构造函数与析构函数
    CppSQLite3Exception(const	int		nErrCode,
								char*	szErrMess,
								bool	bDeleteMsg = true);
    CppSQLite3Exception(const CppSQLite3Exception& e);
    virtual ~CppSQLite3Exception();

// 成员函数
    const	int			ErrorCode() { return m_nErrCode; }									// 输出错误号
    const	char*		ErrorMessage() { return m_pszErrMess; }								// 输出错误信息
    static	const char* ErrorCodeAsString(int nErrCode);									// 将错误代号作为字符串格式输出

// 属性
private:
    int		m_nErrCode;																		// 错误号
    char*	m_pszErrMess;	
	// 错误信息
};


//=========================CppSQLite3Query=======================
// 数据库动态查询类
class  CLASSINDLL_CLASS_DECL CPPSQLITE3_API CppSQLite3Query
{
public:
// 构造函数与析构函数
    CppSQLite3Query();
    CppSQLite3Query(const CppSQLite3Query& rQuery);
    CppSQLite3Query(sqlite3* pDB,
					sqlite3_stmt* pVM,
					bool bEof,
					bool bOwnVM=true);
	virtual ~CppSQLite3Query();

// 成员函数
    CppSQLite3Query& operator=(const CppSQLite3Query& rQuery);

    int		    NumFields();                //获取字段数目
    int		    FieldIndex(const char* szField);                //通过字段名称获取字段索引值
    const char* FieldName(int nCol);    //通过字段索引值获取字段名称

    const char* FieldDeclType(int nCol);  //获取该列在 CREATE TABLE 语句中声明的类型
    int		    FieldDataType(int nCol);    //获取该列值的数据类型

	int			FieldDataRealType(int nCol);

    const char* FieldValue(int nField); //获取字段值
    const char* FieldValue(const char* szField);

    int		    GetIntField(int nField, int nNullValue=0);  //获取整型字段值
    int		    GetIntField(const char* szField, int nNullValue=0);

    double   	GetFloatField(int nField, double fNullValue=0.0);  //获取浮点字段值
    double	    GetFloatField(const char* szField, double fNullValue=0.0);

    const char* GetStringField(int nField, const char* szNullValue="");  //获取字符串字段值
    const char* GetStringField(const char* szField, const char* szNullValue="");

    const unsigned char* GetBlobField(int nField, int& nLen);  //获取二进制字段值
    const unsigned char* GetBlobField(const char* szField, int& nLen);

    bool	   FieldIsNull(int nField);    //判断字段是否为空
    bool	   FieldIsNull(const char* szField);

    bool       Eof();                      //是否到达记录集末尾
    void	   NextRow();                  //移到记录集下一行

	void	   SetPointRow(CppSQLite3Query& query);            //将当前行加1
	int		   GetPointRow();              //获取当前行位置
    void	   Finalize();                 //释放记录集资源

//私有成员函数
private:
    void CheckVM();                     //判断sqlite3_stmt是否为空

//属性
private:
	sqlite3*		m_pDB;
    sqlite3_stmt*	m_pVM;
    bool			m_bEof;
    int				m_nCols;
    bool			m_bOwnVM;
	int				pointRow;
};


//========================CppSQLite3Table=======================
// 数据库静态查询类
class CPPSQLITE3_API CLASSINDLL_CLASS_DECL CppSQLite3Table
{
public:

// 构造函数与析构函数
    CppSQLite3Table();               
    CppSQLite3Table(const CppSQLite3Table& rTable);
    CppSQLite3Table(char** paszResults, int nRows, int nCols);
    virtual ~CppSQLite3Table();

// 成员函数
    CppSQLite3Table& operator=(const CppSQLite3Table& rTable);

    int NumFields();                    //获取字段数目
    int NumRows();                      //获取行数目
    const char* FieldName(int nCol);    //根据序号获取字段名称

    const char* FieldValue(int nField); //获取字段值（用字符串表示）
    const char* FieldValue(const char* szField);

    int GetIntField(int nField, int nNullValue=0);          //获取整型字段值
    int GetIntField(const char* szField, int nNullValue=0);

    double GetFloatField(int nField, double fNullValue=0.0);//获取浮点字段值
    double GetFloatField(const char* szField, double fNullValue=0.0);

    const char* GetStringField(int nField, const char* szNullValue="");         //获取文本字段值
    const char* GetStringField(const char* szField, const char* szNullValue="");

    bool FieldIsNull(int nField);       //判断字段是否为空
    bool FieldIsNull(const char* szField);

    void SetRow(int nRow);              //设置当前行位置
    void Finalize();                    //释放记录集资源

// 私有成员函数
private:
    void CheckResults();

//属性
private:
    int m_nCols;
    int m_nRows;
    int m_nCurrentRow;
    char** m_paszResults;               //保存记录集数据
};


//=======================CppSQLite3Statement=======================
// sql语句解析类
class  CPPSQLITE3_API CLASSINDLL_CLASS_DECL CppSQLite3Statement
{
public:

// 构造函数与析构函数
    CppSQLite3Statement();
    CppSQLite3Statement(const CppSQLite3Statement& rStatement);
    CppSQLite3Statement(sqlite3* pDB, sqlite3_stmt* pVM);
    virtual ~CppSQLite3Statement();

// 成员函数
    CppSQLite3Statement& operator=(const CppSQLite3Statement& rStatement);

    CppSQLite3Query ExecQuery();        //执行查询语句，并返回记录集
    int ExecDML();                      //执行SQL语句
    void Bind(int nParam, const char* szValue);             //添加文本值
    void Bind(int nParam, const int nValue);                //添加整型值
    void Bind(int nParam, const double dwValue);            //添加浮点值
    void Bind(int nParam, const unsigned char* blobValue, int nLen);            //添加二进制值
    void BindNull(int nParam);          //添加空值

    void Reset();                       //初始化SQL解析对象
    void Finalize();                    //释放SQL解析对象

//私有成员函数
private:
    void CheckDB();
    void CheckVM();

//属性
private:
    sqlite3* m_pDB;
    sqlite3_stmt* m_pVM;
};

//=======================CppSQLite3DB=======================
// 数据库链接类
class  CPPSQLITE3_API CLASSINDLL_CLASS_DECL CppSQLite3DB
{
public:

// 构造函数与析构函数
    CppSQLite3DB();
    virtual ~CppSQLite3DB();

// 成员函数
	/*以下函数用于关闭/打开数据库*/
	bool    Open(const char *szFile);   //打开数据库
	bool    Open(const char *szFile, const char *szKey);   //打开数据库
	bool	IsOpen();                   //判断数据库是否打开
	void    Close();                    //关闭数据库

	/*以下函数用于数据库加密/解密*/
	bool    SetKey(const char *szKey, int nKey);           //设置密码
	bool    ResetKey(const char *szKey, int nKey);         //修改密码
    /*以下函数用于执行sql语句*/
    int     ExecDML(const char* szSQL); //执行SQL语句
    int     ExecScalar(const char* szSQL);                  //执行返回单个数值的查询语句
	CppSQLite3Query     ExecQuery(const char* szSQL);           //执行查询语句，返回动态记录集
	CppSQLite3Table     GetTable(const char* szSQL);            //执行查询语句，返回静态记录集
    CppSQLite3Statement CompileStatement(const char* szSQL);//解析SQL语句，返回解析后的SQL对象
    long    LastRowId();           //返回最近的插入操作的整形键
    void    Interrupt();       //中断数据库任何运行操作
    void    SetBusyTimeout(int nMillisecs);                 //设置数据库函数超时时间
    static const char* SQLiteVersion() { return SQLITE_VERSION; }               //返回数据库版本

	/*以下函数用于读取数据库*/
	int     Cmd(const char* fmt, ...);  //获取数据集
	int     Cmd(const char* fmt, va_list args);  //获取数据集
	bool    More();                     //判断是否到达记录集的末尾

	long    Exec() { return 1; };        //该函数为保留函数，没有任何的用处可用于与mysql接口的兼容

	double  GetDouble(int nField, double fNullValue=0.0);                              // 获取浮点字段值
	double  GetDouble(const char* szField, double fNullValue=0.0);

	const char* Get(int nField, const char* szNullValue="");                          // 获取字段值（用字符串表示）
    const char* Get(const char* szField, const char* szNullValue="");

	long    GetLong(int nField, int nNullValue=0);                                       // 获取整形字段值
    long    GetLong(const char* szField, int nNullValue=0);

    const unsigned char* GetBlob(int nField, int& nLen);                              // 获取二进制值
    const unsigned char* GetBlob(const char* szField, int& nLen);

	int	    NumFields();                //获取字段数目
    int		FieldIndex(const char* szField);                //通过字段名称获取字段索引值
    int		FieldDataType(int nCol);    //获取该列值的数据类型
	bool	FieldIsNull(int nField);    //判断字段是否为空
    bool	FieldIsNull(const char* szField);
    const char* FieldName(int nCol);    //通过字段索引值获取字段名称
    const char* FieldDeclType(int nCol);  //获取该列在 CREATE TABLE 语句中声明的类型
	void   ClearResult();              //清除cmd返回结果集

    /*以下函数用于独立获取blob字段*/
	int GetBlob(const char* szTable, const char* szField, const char* extra_condition, unsigned char* pData, int& nLen);

	/*以下函数用于添加或修改数据库*/
	void   AddEdit(const char* szSQL);  //添加或编辑数据库
	void   Bind(int nParam, const char* szValue);             //添加文本值
    void   Bind(int nParam, const int nValue);                //添加整型值
    void   Bind(int nParam, const double dwValue);            //添加浮点值
    void   Bind(int nParam, const unsigned char* blobValue, int nLen);            //添加二进制值
    void   BindNull(int nParam);          //添加空值
    int    Update();                    //更新数据库
	void   ClearStmt();              //清除cmd返回结果集
	void   ResetStmt();              //清除cmd返回结果集
	int    SaveBlob(const char* szSQL, const void* pBlobData, int nDataLen);             //写入二进数值
	

	/*以下函数用于事务处理*/
	int    BeginTransaction();                                                           //执行事务
	int    Commit();                                                                  
	int    Rollback(); 
	int    ContractDB();                  //压缩数据库

	/*其它函数*/
	bool    TableExists(const char* szTable);               //判断表是否存在

	/*以下是一些工具方法*/
	static std::string  IntToStr( int  num);                //数值转换成字符串
	static std::string  ConvertToFormatTime(time_t &t);                //时间类型转化成字符串类型，如2005-01-01 01:01:01
    static std::string  ConvertTimeToStr(time_t &t);                   //时间转化成字符串类型,如3223341
	
    sqlite3* GetSqlite3DB() { return m_pDB; }

//私有成员函数
private:
    CppSQLite3DB(const CppSQLite3DB& db);
    CppSQLite3DB& operator=(const CppSQLite3DB& db);
    sqlite3_stmt* Compile(const char* szSQL);
    void CheckDB();
	CppSQLite3Query*     CheckResultset();
	CppSQLite3Statement* CheckStmt();
	bool Lock();
	bool UnLock();
	unsigned long MyGetCurrentThreadId();
	
//属性
private:
    sqlite3* m_pDB;

	SAFE_MAP m_mapRes;                  //保存Cmd()函数获取的记录集合
	SAFE_MAP m_mapStmts;                //保存AddEidt()函数获取的CppSQLite3Statement指针
	SAFE_MAP m_mapTrans;                //记录数据库对象在当前线程下是否处于事务处理状态
	
};


//=========================CppSQLite3Buffer=========================
//缓冲区处理类
class  CPPSQLITE3_API CLASSINDLL_CLASS_DECL CppSQLite3Buffer
{
public:
// 构造函数与析构函数
    CppSQLite3Buffer();
    ~CppSQLite3Buffer();

// 成员函数
    const char*		Format(const char* szFormat, ...);										// 格式化
    operator const	char*() { return m_pBuf; }												// 定义造型运算符，将对象转化成字符串指针
    void			Clear();																// 释放并清空缓冲区

// 属性
private:
    char* m_pBuf;																			// 缓冲区指针
};

//=========================CppSQLite3Binary=======================
//二进制编码处理类
class  CPPSQLITE3_API CLASSINDLL_CLASS_DECL CppSQLite3Binary
{
public:
// 构造函数与析构函数
    CppSQLite3Binary();
    ~CppSQLite3Binary();

// 成员函数
    void	SetBinary(const unsigned char* pBuf, int nLen);									// 设置二进制编码
    void	SetEncoded(const unsigned char* pBuf);											// 设置编码
	int		SetBinaryLength();																// 设置二进制长度
	void	Clear();																		// 清空缓冲区

    const unsigned	char* GetEncoded();														// 获取编码
    const unsigned	char* GetBinary();														// 获取二进制
    unsigned char* AllocBuffer(int nLen);													// 分配缓冲区大小

// 属性
private:
    unsigned char* m_pBuf;																	// 缓冲区
    int		m_nBinaryLen;																	// 二进制编码长度
    int		m_nBufferLen;																	// 缓冲区大小
    int		m_nEncodedLen;																	// 编码长度
    bool	m_bEncoded;																		// 编码是否正确
};

#endif
