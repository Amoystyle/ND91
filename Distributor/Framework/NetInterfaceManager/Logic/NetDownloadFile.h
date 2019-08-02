#pragma once
#include "Common/NetDispatch/NetDispatchMessageDefine.h"

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

//简单的文件下载，有进度汇报类
class CNetDownloadFile : public BaseThread
{

public:

	CNetDownloadFile();

	~CNetDownloadFile();

	void SetDownloadInfo(string strUrl, wstring strFile, UINT unTaskIndex);

	unsigned long Main();

	bool DownloadFile(string strUrl, wstring strFile, UINT unTaskIndex=0);
	
private:

	void DeleteTaskID(UINT unTask);

	bool GetFileSize(wstring wsUrl);

	bool ParseUrl(string strUrl, string& strHost, string& strPage);
	//回报进度
	static UINT ReportProcess(LPVOID pVoid = NULL);

	//以下为建立连接获取文件大小
	void ParseHostURL(CString URL,CString &host,CString &path,CString &filename);

	SOCKET ConnectHttpNonProxy(CString strHostAddr,int nPort);

	bool SocketSend(SOCKET sckDest,CString szHttp);

	DWORD GetHttpHeader(SOCKET sckDest,char *str);

	DWORD GetFileLength(char *httpHeader);

	bool SendHttpHeader(SOCKET hSocket,CString strHostAddr,
		CString strHttpAddr,CString strHttpFilename,DWORD nPos);
private:

	string  _strUrl;
	wstring _strFile;
	UINT _unTaskIndex;
	bool _bExit;

	DownloadInfo _DownloadInfo;
	DWORD  m_nFileLength;

	static vector<UINT> vec_DownloadingTask;

	CRITICAL_SECTION	_lock;

	bool   _bReportFinish;
};