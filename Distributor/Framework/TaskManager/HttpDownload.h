#include "atlstr.h"
#include "TaskItem.h"
#define MAX_RECV_LEN           100   // 每次接收最大字符串长度.
#define MAX_PENDING_CONNECTS   4     // 等待队列的长度.

struct TaskDealParm;
typedef bool (*CheckReturn)(TaskDealParm *);
class  CHttpSect
{
public:
	CString  szProxyAddr;     // 理服务器地址.
	CString  szHostAddr;      // Host地址.
	int      nProxyPort;      // 代理服务端口号.
	int      nHostPort;       // Host端口号.
	CString  szHttpAddr;      // Http文件地址.
	CString  szHttpFilename;  // Http文件名.
	CString  szDesFilename;   // 下载后的文件名.
	DWORD    nStart;          // 分割的起始位置.
	DWORD    nEnd;            // 分割的起始位置.
	DWORD    bProxyMode;      // 下载模态. 
	int      nRetrytimes;     // 重试次数
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

//使用接口，函数中自动使用join，等待下载完成。下载完成后会停止一段时间
bool fnHttpDownload(wstring strUrl,
				  wstring &strWriteFileName,
				  CTaskItem *taskItem,
				  TaskDealParm* taskDealParm,
				  CheckReturn checkCancel = NULL,
				  CString strProxy = _T(""),
				  int nProxyPort = 8080,
				  int nThread = 1
				  );