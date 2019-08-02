#include "stdafx.h"
#include "NetDownloadFile.h"
#include "Path.h"

#pragma comment(lib,"ws2_32.lib")

#define MAX_RECV_LEN           100   // ÿ�ν�������ַ�������.
#define MAX_PENDING_CONNECTS   4     // �ȴ����еĳ���.

CDealSocket dealsocket;

CDealSocket::CDealSocket()
{
	// �׽��ֳ�ʼ��.
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;

	// ��ʼ��WinSock.
	if (WSAStartup(wVersionRequested, &wsaData)!=0)
	{
		TRACE(_T("WSAStartup\n"));
		return;
	}

	// ��� WinSock �汾.
	if (wsaData.wVersion != wVersionRequested)
	{
		TRACE(_T("WinSock version not supported\n"));
		WSACleanup();
		return;
	}
}

//---------------------------------------------------------------------------
CDealSocket::~CDealSocket()
{
	// �ͷ�WinSock.
	WSACleanup();
}

//---------------------------------------------------------------------------
CString CDealSocket::GetResponse(SOCKET hSock)
{
	char szBufferA[MAX_RECV_LEN];  	// ASCII�ַ���. 
	int	iReturn;					// recv�������ص�ֵ.

	CString szError;
	CString strPlus;
	strPlus.Empty();

	while(1)
	{
		// ���׽��ֽ�������.
		iReturn = recv (hSock, szBufferA, MAX_RECV_LEN, 0);
		szBufferA[iReturn]=0;
		strPlus +=szBufferA;

		//	TRACE(szBufferA);

		if (iReturn == SOCKET_ERROR)
		{
			szError.Format(_T("No data is received, recv failed. Error: %d"),
				WSAGetLastError ());
			MessageBox (NULL, szError, TEXT("Client"), MB_OK);
			break;
		}
		else if(iReturn<MAX_RECV_LEN){
			TRACE(_T("Finished receiving data\n"));
			break;
		}
	}

	return strPlus;
}

//---------------------------------------------------------------------------
SOCKET CDealSocket::GetConnect(CString host ,int port)
{
	SOCKET hSocket;
	SOCKADDR_IN saServer;          // �������׽��ֵ�ַ.
	PHOSTENT phostent = NULL;	   // ָ��HOSTENT�ṹָ��.

	// ����һ���󶨵���������TCP/IP�׽���.
	if ((hSocket = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		TRACE(_T("Allocating socket failed. Error: %d\n"),WSAGetLastError ());
		return INVALID_SOCKET;
	}

	// ʹ��TCP/IPЭ��.
	saServer.sin_family = AF_INET;

	// ��ȡ��������ص���Ϣ.
	USES_CONVERSION;
	if ((phostent = gethostbyname (CCodeOperation::UnicodeToUTF_8(host.GetBuffer()).c_str())) == NULL) 
	{
		TRACE(_T("Unable to get the host name. Error: %d\n"),WSAGetLastError ());
		closesocket (hSocket);
		return INVALID_SOCKET;
	}

	// ���׽���IP��ַ��ֵ.
	memcpy ((char *)&(saServer.sin_addr), 
			phostent->h_addr, 
			phostent->h_length);

	// �趨�׽��ֶ˿ں�.
	saServer.sin_port =htons (port); 

	// ���������������׽�������.
	if (connect (hSocket,(PSOCKADDR) &saServer, 
		sizeof (saServer)) == SOCKET_ERROR) 
	{
		TRACE(_T("Connecting to the server failed. Error: %d\n"),WSAGetLastError ());
		closesocket (hSocket);
		return INVALID_SOCKET;
	}

	int nTSize = 0;
	if(setsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nTSize, sizeof(int)) == SOCKET_ERROR)
	{
		closesocket (hSocket);
		return INVALID_SOCKET;
	}

	int timeout= 60000; 
	if(setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
	{
		closesocket (hSocket);
		return INVALID_SOCKET;
	}

	return hSocket;
}

//---------------------------------------------------------------------------
SOCKET CDealSocket::Listening(int port)
{
	SOCKET ListenSocket = INVALID_SOCKET;	// �����׽���.
	SOCKADDR_IN local_sin;				    // �����׽��ֵ�ַ.

	// ����TCP/IP�׽���.
	if ((ListenSocket = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 
	{
		TRACE(_T("Allocating socket failed. Error: %d\n"),WSAGetLastError ());
		return INVALID_SOCKET;
	}

	// ���׽�����Ϣ�ṹ��ֵ.
	local_sin.sin_family = AF_INET;
	local_sin.sin_port = htons (port); 
	local_sin.sin_addr.s_addr = htonl (INADDR_ANY);

	// ���б�����ַ������׽��ְ�.
	if (bind (ListenSocket, 
		(struct sockaddr *) &local_sin, 
		sizeof (local_sin)) == SOCKET_ERROR) 
	{
		TRACE(_T("Binding socket failed. Error: %d\n"),WSAGetLastError ());
		closesocket (ListenSocket);
		return INVALID_SOCKET;
	}

	// �����׽��ֶ��ⲿ���ӵļ���.
	if (listen (ListenSocket, MAX_PENDING_CONNECTS) == SOCKET_ERROR) 
	{
		TRACE(_T("Listening to the client failed. Error: %d\n"),	
			WSAGetLastError ());
		closesocket (ListenSocket);
		return INVALID_SOCKET;
	}

	return ListenSocket;
}







vector<UINT> CNetDownloadFile::vec_DownloadingTask;
CNetDownloadFile::CNetDownloadFile()
{
	_bExit = false;
	m_nFileLength = 0;
	InitializeCriticalSection(&_lock);
	_bReportFinish = false;
}

CNetDownloadFile::~CNetDownloadFile()
{
	DeleteCriticalSection(&_lock);
}

void CNetDownloadFile::SetDownloadInfo(string strUrl, wstring strFile, UINT unTaskIndex)
{
	_strUrl = strUrl;
	_strFile= strFile;
	_unTaskIndex = unTaskIndex;
}


unsigned long CNetDownloadFile::Main()
{
	
	bool bDownloadingTask = false;
	EnterCriticalSection(&_lock);
	for (vector<UINT>::iterator iter = vec_DownloadingTask.begin(); iter != vec_DownloadingTask.end(); ++iter)
	{
		if (_unTaskIndex == *iter)
		{
			bDownloadingTask = true;
		}
	}
	LeaveCriticalSection(&_lock);
//�����������У����������
	if (!bDownloadingTask)
	{
		vec_DownloadingTask.push_back(_unTaskIndex);
		this->DownloadFile(_strUrl, _strFile, _unTaskIndex);
	}

	return 0;
};

bool CNetDownloadFile::DownloadFile(string strUrl, wstring strFile, UINT unTaskIndex)
{
	wstring wsTempDownPath = CFileOperation::GetFileDir(strFile) + CFileOperation::GetFileName(strFile) + L".tmp";
	if (CFileOperation::IsFileExist(wsTempDownPath.c_str()))
	{
		CFileOperation::DeleteFile(wsTempDownPath);
	}
	std::ofstream file(wsTempDownPath.c_str(), std::ios::out|std::ios::ate|std::ios::binary);
	if (file.fail())
		return false;

	_DownloadInfo.unTaskid = unTaskIndex;

	//����ļ���С
    bool bReturn = this->GetFileSize(CCodeOperation::GB2312ToUnicode(strUrl));
	if (!bReturn)
	{
		return false;
	}

	_DownloadInfo.unAllSize = m_nFileLength;

	string strHost="", strPage="";
	if (!ParseUrl(strUrl, strHost, strPage))
	{
		file.close();
		return false;
	}

//�����㱨�����߳�
	CreateThread(0,0,(LPTHREAD_START_ROUTINE)ReportProcess, LPVOID(this), 0, NULL);

	W3Client w3;
	try{
		if(w3.Connect(strHost.c_str()))
		{
			if(w3.Request(strPage.c_str()))
			{
				char buf[1024] = "\0";
				int nSize = 0;
				while((nSize = w3.Response(reinterpret_cast<unsigned char *>(buf), 1024)) > 0)
				{
					for (int i=0; i<nSize; i++)
						file.put(buf[i]);

					_DownloadInfo.unCurDownSize += nSize;
				}
				file.close();
				//������ɸ��Ƶ�ָ��·��
				CopyFile(wsTempDownPath.c_str(), strFile.c_str(), false);
				CFileOperation::DeleteFile(wsTempDownPath);
				this->DeleteTaskID(_unTaskIndex);

				while(!_bReportFinish)
				{
					Sleep(50);//�ȴ��㱨���ȴ�������˳�
				}

				return true;
			}
		}
	}
	catch (...)
	{
	}

	_bExit = true;

	this->DeleteTaskID(_unTaskIndex);

	file.close();
	Sleep(200);
	return false;
}

void CNetDownloadFile::DeleteTaskID(UINT unTask)
{
	EnterCriticalSection(&_lock);
	for (vector<UINT>::iterator iter = vec_DownloadingTask.begin(); iter != vec_DownloadingTask.end();)
	{
		if (unTask == *iter)
		{
			iter = vec_DownloadingTask.erase(iter);
		}
		else
			++iter;
	}
	LeaveCriticalSection(&_lock);
}

bool CNetDownloadFile::ParseUrl( string strUrl, string& strHost, string& strPage )
{
	int nIndex = strUrl.find("://");
	if (nIndex>-1)
	{
		strUrl = strUrl.substr(nIndex + 3);
		nIndex = strUrl.find("/");
		if (nIndex > -1)
		{
			strHost = strUrl.substr(0, nIndex);
			strPage = strUrl.substr(nIndex);
			return true;
		}
	}
	return false;
}

void CNetDownloadFile::ParseHostURL(CString URL,CString &host,CString &path,CString &filename)
{
	URL.TrimLeft();
	URL.TrimRight();
	CString str=URL;
	CString strFind=_T("http://");
	int n=str.Find(strFind);
	if(n!=-1){
		str.Delete(0, n+strFind.GetLength());
	}

	n=str.Find('/');
	host = str.Left(n);
	n=URL.ReverseFind('/');
	path = URL.Left(n+1);
	filename = URL.Right(URL.GetLength()-n-1);
	filename.Replace(_T("//"),_T("/"));
}

//---------------------------------------------------------------------------
SOCKET CNetDownloadFile::ConnectHttpNonProxy(CString strHostAddr,int nPort)
{
	//TRACE(_T("���ڽ�������\n"));
	SOCKET hSocket=dealsocket.GetConnect(strHostAddr,nPort);
	if(hSocket == INVALID_SOCKET)
	return INVALID_SOCKET;

	return hSocket;
}

//---------------------------------------------------------------------------
bool CNetDownloadFile::SocketSend(SOCKET sckDest,CString szHttp)
{
	int iLen=szHttp.GetLength();
	USES_CONVERSION;
	if(send (sckDest,CCodeOperation::UnicodeToUTF_8(szHttp.GetBuffer()).c_str(),iLen,0)==SOCKET_ERROR)
	{
		closesocket(sckDest);
		//	TRACE(_T("��������ʧ��!\n"));
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------
DWORD CNetDownloadFile::GetHttpHeader(SOCKET sckDest,char *str)
{
	BOOL bResponsed=FALSE;
	DWORD nResponseHeaderSize;

	if(!bResponsed)
	{
		char c = 0;
		int nIndex = 0;
		BOOL bEndResponse = FALSE;
		while(!bEndResponse && nIndex < 1024)
		{
			recv(sckDest,&c,1,0);
			str[nIndex++] = c;
			if(nIndex >= 4)
			{
				if( str[nIndex - 4] == '\r' && 
					str[nIndex - 3] == '\n' && 
					str[nIndex - 2] == '\r' && 
					str[nIndex - 1] == '\n')
					bEndResponse = TRUE;
			}
		}

		str[nIndex]=0;
		nResponseHeaderSize = nIndex;
		bResponsed = TRUE;
	}

	return nResponseHeaderSize;
}

DWORD CNetDownloadFile:: GetFileLength(char *httpHeader)
{
	CString strHeader;
	CString strFind=_T("Content-Length:");
	int local;
	strHeader=CString(httpHeader);
	local=strHeader.Find(strFind,0);
	local+=strFind.GetLength();
	strHeader.Delete(0,local);
	local=strHeader.Find(_T("\r\n"));

	if(local!=-1){
		strHeader=strHeader.Left(local);
	}

	USES_CONVERSION;
	return atoi(CCodeOperation::UnicodeToUTF_8(strHeader.GetBuffer()).c_str());
}

// ����: strHostAddr="www.aitenshi.com",
// strHttpAddr="http://www.aitenshi.com/bbs/images/",
// strHttpFilename="pics.jpg".
bool CNetDownloadFile::SendHttpHeader(SOCKET hSocket,CString strHostAddr,
							  CString strHttpAddr,CString strHttpFilename,DWORD nPos)
{
	// ��������. 
	CString sTemp;
	char cTmpBuffer[1025];

	// Line1: �����·��,�汾.
	sTemp.Format(_T("GET %s%s HTTP/1.1\r\n"),strHttpAddr,strHttpFilename);
	if(!SocketSend(hSocket,sTemp))
	return false;

	// Line2:����.
	sTemp.Format(_T("Host: %s\r\n"),strHostAddr);
	if(!SocketSend(hSocket,sTemp)) 
	return false;

	// Line3:���յ���������.
	sTemp.Format(_T("Accept: \r\n"));
	if(!SocketSend(hSocket,sTemp)) 
	return false;

	// Line4:�ο���ַ.
	sTemp.Format(_T("Referer: %s\r\n"),strHttpAddr); 
	if(!SocketSend(hSocket,sTemp)) 
	return false;

	// Line5:���������.
	sTemp.Format(_T("User-Agent: Mozilla/4.0 \
					(compatible; MSIE 5.0; Windows NT; DigExt; DTS Agent;)\r\n"));

	if(!SocketSend(hSocket,sTemp)) 
	return false;

	// ����. Range ��Ҫ���ص����ݷ�Χ������������Ҫ.
	sTemp.Format(_T("Range: bytes=%d-\r\n"),nPos);
	if(!SocketSend(hSocket,sTemp)) 
	return false;

	// LastLine: ����.
	sTemp.Format(_T("\r\n"));
	if(!SocketSend(hSocket,sTemp)) 
	return false;

	// ȡ��httpͷ.
	int i=GetHttpHeader(hSocket,cTmpBuffer);
	if(!i)
	{
		return 0;
	}

	// ���ȡ�õ�httpͷ����404�����������ʾ���ӳ�����.

	string strret=cTmpBuffer;
	int pos=strret.find("\r\n");
	if(pos>-1)
	strret=strret.substr(0,pos);
	if(strret.find("206")==string::npos)
	{
		m_nFileLength=0;
		return false;
	}

	// �õ��������ļ��Ĵ�С.
	m_nFileLength=GetFileLength(cTmpBuffer);

	return true;
}

bool CNetDownloadFile::GetFileSize(wstring wsUrl)
{
	CString strHostAddr;
	CString strHttpAddr;
	CString strHttpFilename;
	int nHostPort=80;

	ParseHostURL(wsUrl.c_str(),strHostAddr,strHttpAddr,strHttpFilename);

	SOCKET hSocket;
	hSocket=ConnectHttpNonProxy(strHostAddr,nHostPort);
	if(hSocket == INVALID_SOCKET) return 1;
	// �����ļ�ͷ�������ļ���С.
	return SendHttpHeader(hSocket,strHostAddr,strHttpAddr,strHttpFilename,0);
}

//�ر�����
UINT CNetDownloadFile::ReportProcess(LPVOID pVoid)
{
	CNetDownloadFile* pNetDownloadFile = (CNetDownloadFile*)pVoid;
	if (NULL == pNetDownloadFile)
	{
		return 0;
	}

	while(true)
	{
		if (pNetDownloadFile->_DownloadInfo.unCurDownSize >= pNetDownloadFile->_DownloadInfo.unAllSize
			 || pNetDownloadFile->_bExit)
		{//������������㱨����
			break;
		}
		ReponseDownloadReportMsg ReportMsg;
		ReportMsg.bDownloadSuccess = true;
		ReportMsg.st_DoenloadInfo = pNetDownloadFile->_DownloadInfo;
		ReportMsg.SendToUI();

		Sleep(500);
	}

	//�ٻᱨһ�ν���
	bool bDownloadSuccess;
	if (pNetDownloadFile->_DownloadInfo.unCurDownSize >= pNetDownloadFile->_DownloadInfo.unAllSize)
	{
		bDownloadSuccess = true;
	}
	else
		bDownloadSuccess = false;

	ReponseDownloadReportMsg ReportMsg;
	ReportMsg.bDownloadSuccess = bDownloadSuccess;
	ReportMsg.st_DoenloadInfo = pNetDownloadFile->_DownloadInfo;
	ReportMsg.SendToUI();

	pNetDownloadFile->_bReportFinish = true;

	return 0;
}