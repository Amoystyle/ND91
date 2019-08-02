#include "atlstr.h"
#include "TaskItem.h"
#define MAX_RECV_LEN           100   // ÿ�ν�������ַ�������.
#define MAX_PENDING_CONNECTS   4     // �ȴ����еĳ���.

struct TaskDealParm;
typedef bool (*CheckReturn)(TaskDealParm *);
class  CHttpSect
{
public:
	CString  szProxyAddr;     // ���������ַ.
	CString  szHostAddr;      // Host��ַ.
	int      nProxyPort;      // �������˿ں�.
	int      nHostPort;       // Host�˿ں�.
	CString  szHttpAddr;      // Http�ļ���ַ.
	CString  szHttpFilename;  // Http�ļ���.
	CString  szDesFilename;   // ���غ���ļ���.
	DWORD    nStart;          // �ָ����ʼλ��.
	DWORD    nEnd;            // �ָ����ʼλ��.
	DWORD    bProxyMode;      // ����ģ̬. 
	int      nRetrytimes;     // ���Դ���
	//CheckReturn checkfunc;
	bool (*checkfunc)(TaskDealParm *);
	TaskDealParm* taskDealParm;
	CTaskItem *taskItem;
};

class  CHttpGet  
{
	friend class TaskControl;
public:
	CHttpGet();
	virtual ~CHttpGet();
	//static unsigned long m_downloaded;

private:
	CHttpSect *sectinfo;
	static int m_nCount;
	static UINT ThreadDownLoad(void* pParam);
	//bool (*CheckCancel)();
	//func CheckCancel;
public:
	static DWORD m_nFileLength;

private:
	static SOCKET ConnectHttpProxy(CString strProxyAddr,int nPort);
	static SOCKET ConnectHttpNonProxy(CString strHostAddr,int nPort);
	static BOOL SendHttpHeader(SOCKET hSocket,CString strHostAddr,
		CString strHttpAddr,CString strHttpFilename,DWORD nPos);
	static DWORD GetHttpHeader(SOCKET sckDest,char *str);
	static DWORD GetFileLength(char *httpHeader);
	static BOOL SocketSend(SOCKET sckDest,CString szHttp);

	BOOL FileCombine(CHttpSect *pInfo, FILE *fpwrite);

public:
	void setCheckCancel(bool (*func)());

	BOOL HttpDownLoadProxy(
		CString strProxyAddr,
		int nProxyPort,
		CString strHostAddr,
		CString strHttpAddr,
		CString strHttpFilename,
		CString strWriteFileName,
		int nSectNum
		,CheckReturn checkCancel
		, TaskDealParm* taskDealParm
		,CTaskItem *taskItem);

	BOOL HttpDownLoadNonProxy(
		CString strHostAddr,
		CString strHttpAddr,
		CString strHttpFilename,
		CString strWriteFileName,
		int nSectNum,
		CheckReturn CheckCancel
		, TaskDealParm* taskDealParm
		,CTaskItem *taskItem);

	BOOL HttpDownLoad(
		CString strProxyAddr,
		int nProxyPort,
		CString strHostAddr,
		int nHostPort,
		CString strHttpAddr,
		CString strHttpFilename,
		CString strWriteFileName,
		int nSectNum,
		BOOL bProxy,CheckReturn CheckCancel
		, TaskDealParm* taskDealParm
		,CTaskItem *taskItem);
};

class CDealSocket  
{
public:
	CDealSocket();
	virtual ~CDealSocket();

public:
	SOCKET GetConnect(CString host ,int port);
	SOCKET Listening(int port);
	CString GetResponse(SOCKET hSock);
};

class CDownloadFile  
{
public:
	CDownloadFile();
	virtual ~CDownloadFile();

public:
	BOOL FileExists(LPCTSTR lpszFileName);
	FILE* GetFilePointer(LPCTSTR lpszFileName);
	DWORD GetFileSizeByName(LPCTSTR lpszFileName);
	CString GetShortFileName(LPCSTR lpszFullPathName);
	static const char * CStringTostring(CString str);
	static const char * UnicodeToAnsi(const WCHAR* pSrc,size_t nLength);
};

extern CDownloadFile downloadfile;

extern CDealSocket dealsocket;

//ʹ�ýӿڣ��������Զ�ʹ��join���ȴ�������ɡ�������ɺ��ֹͣһ��ʱ��
bool fnHttpDownload(wstring strUrl,
				  wstring &strWriteFileName,
				  CTaskItem *taskItem,
				  TaskDealParm* taskDealParm,
				  CheckReturn checkCancel = NULL,
				  CString strProxy = _T(""),
				  int nProxyPort = 8080,
				  int nThread = 1
				  );