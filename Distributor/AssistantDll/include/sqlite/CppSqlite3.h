/***************************************************************************//**
 *  @file 
 *  @brief [��Ҫ����]sqlite���ݿ������
 * 
 *  <b>�ļ���</b>      : CppSQLite3.h
 *  @n@n<b>��Ȩ����</b>: �����������Ӧ�����������
 *  @n@n<b>��  ��</b>  :  ���鲨
 *  @n@n<b>����ʱ��</b>: 2009-01-04
 *  @n@n<b>�ļ�����</b>: 
 *  @version  �汾        �޸���        ʱ��        ����@n
 *  @n        v1.0.0      ���鲨     2009-01-16     ���ע�͡�����ṹ�����Ӷ��̷߳��ʻ���
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


/*������sqlite3�Ķ��壬Ϊ�˼�������sqlite3.h�ļ�*/
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
#define CPPSQLITE_ERROR    1000 /* �Զ������ */       
/* end-of-error-codes */

#ifdef CPPSQLITE3_EXPORTS
#define CPPSQLITE3_API 
#else
#define CPPSQLITE3_API 
#endif

//=========================SAFE_MAP=======================
//��ȫ��ϣ��
struct SAFE_MAP
{
public:
// ���캯������������
	SAFE_MAP();
	~SAFE_MAP();

// ��Ա����
	/*LPVOID*/void* GetElement(/*DWORD*/unsigned long dwKey);                   //���ݹؼ��ֻ�ȡֵ
	void   PutElement(/*DWORD*/unsigned long dwKey, /*LPVOID*/void* lpValue);   //�����
	void   RemoveAt(/*DWORD*/unsigned long dwKey);                     //ɾ����
	void   RemoveAll();                               //ɾ��������

	bool   GetFirstItem();                            //���º������ڱ�����ϣ��
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
//�쳣����
class  CLASSINDLL_CLASS_DECL CPPSQLITE3_API CppSQLite3Exception
{
public:
// ���캯������������
    CppSQLite3Exception(const	int		nErrCode,
								char*	szErrMess,
								bool	bDeleteMsg = true);
    CppSQLite3Exception(const CppSQLite3Exception& e);
    virtual ~CppSQLite3Exception();

// ��Ա����
    const	int			ErrorCode() { return m_nErrCode; }									// ��������
    const	char*		ErrorMessage() { return m_pszErrMess; }								// ���������Ϣ
    static	const char* ErrorCodeAsString(int nErrCode);									// �����������Ϊ�ַ�����ʽ���

// ����
private:
    int		m_nErrCode;																		// �����
    char*	m_pszErrMess;	
	// ������Ϣ
};


//=========================CppSQLite3Query=======================
// ���ݿ⶯̬��ѯ��
class  CLASSINDLL_CLASS_DECL CPPSQLITE3_API CppSQLite3Query
{
public:
// ���캯������������
    CppSQLite3Query();
    CppSQLite3Query(const CppSQLite3Query& rQuery);
    CppSQLite3Query(sqlite3* pDB,
					sqlite3_stmt* pVM,
					bool bEof,
					bool bOwnVM=true);
	virtual ~CppSQLite3Query();

// ��Ա����
    CppSQLite3Query& operator=(const CppSQLite3Query& rQuery);

    int		    NumFields();                //��ȡ�ֶ���Ŀ
    int		    FieldIndex(const char* szField);                //ͨ���ֶ����ƻ�ȡ�ֶ�����ֵ
    const char* FieldName(int nCol);    //ͨ���ֶ�����ֵ��ȡ�ֶ�����

    const char* FieldDeclType(int nCol);  //��ȡ������ CREATE TABLE ���������������
    int		    FieldDataType(int nCol);    //��ȡ����ֵ����������

	int			FieldDataRealType(int nCol);

    const char* FieldValue(int nField); //��ȡ�ֶ�ֵ
    const char* FieldValue(const char* szField);

    int		    GetIntField(int nField, int nNullValue=0);  //��ȡ�����ֶ�ֵ
    int		    GetIntField(const char* szField, int nNullValue=0);

    double   	GetFloatField(int nField, double fNullValue=0.0);  //��ȡ�����ֶ�ֵ
    double	    GetFloatField(const char* szField, double fNullValue=0.0);

    const char* GetStringField(int nField, const char* szNullValue="");  //��ȡ�ַ����ֶ�ֵ
    const char* GetStringField(const char* szField, const char* szNullValue="");

    const unsigned char* GetBlobField(int nField, int& nLen);  //��ȡ�������ֶ�ֵ
    const unsigned char* GetBlobField(const char* szField, int& nLen);

    bool	   FieldIsNull(int nField);    //�ж��ֶ��Ƿ�Ϊ��
    bool	   FieldIsNull(const char* szField);

    bool       Eof();                      //�Ƿ񵽴��¼��ĩβ
    void	   NextRow();                  //�Ƶ���¼����һ��

	void	   SetPointRow(CppSQLite3Query& query);            //����ǰ�м�1
	int		   GetPointRow();              //��ȡ��ǰ��λ��
    void	   Finalize();                 //�ͷż�¼����Դ

//˽�г�Ա����
private:
    void CheckVM();                     //�ж�sqlite3_stmt�Ƿ�Ϊ��

//����
private:
	sqlite3*		m_pDB;
    sqlite3_stmt*	m_pVM;
    bool			m_bEof;
    int				m_nCols;
    bool			m_bOwnVM;
	int				pointRow;
};


//========================CppSQLite3Table=======================
// ���ݿ⾲̬��ѯ��
class CPPSQLITE3_API CLASSINDLL_CLASS_DECL CppSQLite3Table
{
public:

// ���캯������������
    CppSQLite3Table();               
    CppSQLite3Table(const CppSQLite3Table& rTable);
    CppSQLite3Table(char** paszResults, int nRows, int nCols);
    virtual ~CppSQLite3Table();

// ��Ա����
    CppSQLite3Table& operator=(const CppSQLite3Table& rTable);

    int NumFields();                    //��ȡ�ֶ���Ŀ
    int NumRows();                      //��ȡ����Ŀ
    const char* FieldName(int nCol);    //������Ż�ȡ�ֶ�����

    const char* FieldValue(int nField); //��ȡ�ֶ�ֵ�����ַ�����ʾ��
    const char* FieldValue(const char* szField);

    int GetIntField(int nField, int nNullValue=0);          //��ȡ�����ֶ�ֵ
    int GetIntField(const char* szField, int nNullValue=0);

    double GetFloatField(int nField, double fNullValue=0.0);//��ȡ�����ֶ�ֵ
    double GetFloatField(const char* szField, double fNullValue=0.0);

    const char* GetStringField(int nField, const char* szNullValue="");         //��ȡ�ı��ֶ�ֵ
    const char* GetStringField(const char* szField, const char* szNullValue="");

    bool FieldIsNull(int nField);       //�ж��ֶ��Ƿ�Ϊ��
    bool FieldIsNull(const char* szField);

    void SetRow(int nRow);              //���õ�ǰ��λ��
    void Finalize();                    //�ͷż�¼����Դ

// ˽�г�Ա����
private:
    void CheckResults();

//����
private:
    int m_nCols;
    int m_nRows;
    int m_nCurrentRow;
    char** m_paszResults;               //�����¼������
};


//=======================CppSQLite3Statement=======================
// sql��������
class  CPPSQLITE3_API CLASSINDLL_CLASS_DECL CppSQLite3Statement
{
public:

// ���캯������������
    CppSQLite3Statement();
    CppSQLite3Statement(const CppSQLite3Statement& rStatement);
    CppSQLite3Statement(sqlite3* pDB, sqlite3_stmt* pVM);
    virtual ~CppSQLite3Statement();

// ��Ա����
    CppSQLite3Statement& operator=(const CppSQLite3Statement& rStatement);

    CppSQLite3Query ExecQuery();        //ִ�в�ѯ��䣬�����ؼ�¼��
    int ExecDML();                      //ִ��SQL���
    void Bind(int nParam, const char* szValue);             //����ı�ֵ
    void Bind(int nParam, const int nValue);                //�������ֵ
    void Bind(int nParam, const double dwValue);            //��Ӹ���ֵ
    void Bind(int nParam, const unsigned char* blobValue, int nLen);            //��Ӷ�����ֵ
    void BindNull(int nParam);          //��ӿ�ֵ

    void Reset();                       //��ʼ��SQL��������
    void Finalize();                    //�ͷ�SQL��������

//˽�г�Ա����
private:
    void CheckDB();
    void CheckVM();

//����
private:
    sqlite3* m_pDB;
    sqlite3_stmt* m_pVM;
};

//=======================CppSQLite3DB=======================
// ���ݿ�������
class  CPPSQLITE3_API CLASSINDLL_CLASS_DECL CppSQLite3DB
{
public:

// ���캯������������
    CppSQLite3DB();
    virtual ~CppSQLite3DB();

// ��Ա����
	/*���º������ڹر�/�����ݿ�*/
	bool    Open(const char *szFile);   //�����ݿ�
	bool    Open(const char *szFile, const char *szKey);   //�����ݿ�
	bool	IsOpen();                   //�ж����ݿ��Ƿ��
	void    Close();                    //�ر����ݿ�

	/*���º����������ݿ����/����*/
	bool    SetKey(const char *szKey, int nKey);           //��������
	bool    ResetKey(const char *szKey, int nKey);         //�޸�����
    /*���º�������ִ��sql���*/
    int     ExecDML(const char* szSQL); //ִ��SQL���
    int     ExecScalar(const char* szSQL);                  //ִ�з��ص�����ֵ�Ĳ�ѯ���
	CppSQLite3Query     ExecQuery(const char* szSQL);           //ִ�в�ѯ��䣬���ض�̬��¼��
	CppSQLite3Table     GetTable(const char* szSQL);            //ִ�в�ѯ��䣬���ؾ�̬��¼��
    CppSQLite3Statement CompileStatement(const char* szSQL);//����SQL��䣬���ؽ������SQL����
    long    LastRowId();           //��������Ĳ�����������μ�
    void    Interrupt();       //�ж����ݿ��κ����в���
    void    SetBusyTimeout(int nMillisecs);                 //�������ݿ⺯����ʱʱ��
    static const char* SQLiteVersion() { return SQLITE_VERSION; }               //�������ݿ�汾

	/*���º������ڶ�ȡ���ݿ�*/
	int     Cmd(const char* fmt, ...);  //��ȡ���ݼ�
	int     Cmd(const char* fmt, va_list args);  //��ȡ���ݼ�
	bool    More();                     //�ж��Ƿ񵽴��¼����ĩβ

	long    Exec() { return 1; };        //�ú���Ϊ����������û���κε��ô���������mysql�ӿڵļ���

	double  GetDouble(int nField, double fNullValue=0.0);                              // ��ȡ�����ֶ�ֵ
	double  GetDouble(const char* szField, double fNullValue=0.0);

	const char* Get(int nField, const char* szNullValue="");                          // ��ȡ�ֶ�ֵ�����ַ�����ʾ��
    const char* Get(const char* szField, const char* szNullValue="");

	long    GetLong(int nField, int nNullValue=0);                                       // ��ȡ�����ֶ�ֵ
    long    GetLong(const char* szField, int nNullValue=0);

    const unsigned char* GetBlob(int nField, int& nLen);                              // ��ȡ������ֵ
    const unsigned char* GetBlob(const char* szField, int& nLen);

	int	    NumFields();                //��ȡ�ֶ���Ŀ
    int		FieldIndex(const char* szField);                //ͨ���ֶ����ƻ�ȡ�ֶ�����ֵ
    int		FieldDataType(int nCol);    //��ȡ����ֵ����������
	bool	FieldIsNull(int nField);    //�ж��ֶ��Ƿ�Ϊ��
    bool	FieldIsNull(const char* szField);
    const char* FieldName(int nCol);    //ͨ���ֶ�����ֵ��ȡ�ֶ�����
    const char* FieldDeclType(int nCol);  //��ȡ������ CREATE TABLE ���������������
	void   ClearResult();              //���cmd���ؽ����

    /*���º������ڶ�����ȡblob�ֶ�*/
	int GetBlob(const char* szTable, const char* szField, const char* extra_condition, unsigned char* pData, int& nLen);

	/*���º���������ӻ��޸����ݿ�*/
	void   AddEdit(const char* szSQL);  //��ӻ�༭���ݿ�
	void   Bind(int nParam, const char* szValue);             //����ı�ֵ
    void   Bind(int nParam, const int nValue);                //�������ֵ
    void   Bind(int nParam, const double dwValue);            //��Ӹ���ֵ
    void   Bind(int nParam, const unsigned char* blobValue, int nLen);            //��Ӷ�����ֵ
    void   BindNull(int nParam);          //��ӿ�ֵ
    int    Update();                    //�������ݿ�
	void   ClearStmt();              //���cmd���ؽ����
	void   ResetStmt();              //���cmd���ؽ����
	int    SaveBlob(const char* szSQL, const void* pBlobData, int nDataLen);             //д�������ֵ
	

	/*���º�������������*/
	int    BeginTransaction();                                                           //ִ������
	int    Commit();                                                                  
	int    Rollback(); 
	int    ContractDB();                  //ѹ�����ݿ�

	/*��������*/
	bool    TableExists(const char* szTable);               //�жϱ��Ƿ����

	/*������һЩ���߷���*/
	static std::string  IntToStr( int  num);                //��ֵת�����ַ���
	static std::string  ConvertToFormatTime(time_t &t);                //ʱ������ת�����ַ������ͣ���2005-01-01 01:01:01
    static std::string  ConvertTimeToStr(time_t &t);                   //ʱ��ת�����ַ�������,��3223341
	
    sqlite3* GetSqlite3DB() { return m_pDB; }

//˽�г�Ա����
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
	
//����
private:
    sqlite3* m_pDB;

	SAFE_MAP m_mapRes;                  //����Cmd()������ȡ�ļ�¼����
	SAFE_MAP m_mapStmts;                //����AddEidt()������ȡ��CppSQLite3Statementָ��
	SAFE_MAP m_mapTrans;                //��¼���ݿ�����ڵ�ǰ�߳����Ƿ���������״̬
	
};


//=========================CppSQLite3Buffer=========================
//������������
class  CPPSQLITE3_API CLASSINDLL_CLASS_DECL CppSQLite3Buffer
{
public:
// ���캯������������
    CppSQLite3Buffer();
    ~CppSQLite3Buffer();

// ��Ա����
    const char*		Format(const char* szFormat, ...);										// ��ʽ��
    operator const	char*() { return m_pBuf; }												// ���������������������ת�����ַ���ָ��
    void			Clear();																// �ͷŲ���ջ�����

// ����
private:
    char* m_pBuf;																			// ������ָ��
};

//=========================CppSQLite3Binary=======================
//�����Ʊ��봦����
class  CPPSQLITE3_API CLASSINDLL_CLASS_DECL CppSQLite3Binary
{
public:
// ���캯������������
    CppSQLite3Binary();
    ~CppSQLite3Binary();

// ��Ա����
    void	SetBinary(const unsigned char* pBuf, int nLen);									// ���ö����Ʊ���
    void	SetEncoded(const unsigned char* pBuf);											// ���ñ���
	int		SetBinaryLength();																// ���ö����Ƴ���
	void	Clear();																		// ��ջ�����

    const unsigned	char* GetEncoded();														// ��ȡ����
    const unsigned	char* GetBinary();														// ��ȡ������
    unsigned char* AllocBuffer(int nLen);													// ���仺������С

// ����
private:
    unsigned char* m_pBuf;																	// ������
    int		m_nBinaryLen;																	// �����Ʊ��볤��
    int		m_nBufferLen;																	// ��������С
    int		m_nEncodedLen;																	// ���볤��
    bool	m_bEncoded;																		// �����Ƿ���ȷ
};

#endif
