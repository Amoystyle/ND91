#include "stdafx.h"

#include "BaseSocket.h"
#include "Device/ConnectionPool.h"

#include <sstream>

#ifdef WIN32
	#include <Iphlpapi.h>
	#pragma comment(lib, "Iphlpapi.lib")
	#define PROTOCOL AF_INET
	#pragma warning(disable:4996)
#endif
 
using namespace ND91Assistant;

#define COMMAND_TIME_OUT 60000		//Socket 接收超时时间 - 

CBaseSocket::CBaseSocket(void)
{
	m_nState = SOCKET_STATE_NONE;
	m_hSocket = NULL;
	m_bCreate = false;
	m_nBuffer = 1024;
	_nLastUseTime = GetTickCount();

	_bNeedCloseSocket = true;
    _nTimeout = COMMAND_TIME_OUT;
    _nLastSocketError = -1;
	_timerOut=COMMAND_TIME_OUT;
}

CBaseSocket::CBaseSocket(SOCKET hSocket)
{
	m_hSocket = hSocket;
	m_nState = SOCKET_STATE_NONE;
	m_bCreate = false;
	m_nBuffer = 1024;
	_nLastUseTime = GetTickCount();

	_bNeedCloseSocket = false;
    
	if (m_hSocket)
		m_nState = SOCKET_STATE_CREATE;
    _nLastSocketError = -1;
}

CBaseSocket::~CBaseSocket(void)
{	
	if (_bNeedCloseSocket)
		close();
}

bool CBaseSocket::create(const char* szHost, UINT nPort, bool bNoDelay,int DelayTime)
{
	if (!szHost || nPort == 0 || m_nState != SOCKET_STATE_NONE) return false;       
    strcpy(m_pHost, szHost);
	m_nPort = nPort;
	struct protoent * ppe = getprotobyname("tcp");
    if ( ! ppe ) 
    {
		_nLastSocketError = -1;
        std::wostringstream ss;
        ss << "getprotobyname(tcp) fail, error code = " << WSAGetLastError();
        LOG->WriteError(ss.str());
        THROW_ASSISTANT_EXCEPTION(ADB_SOCKET_ERROR);
    }

	m_hSocket = socket(PROTOCOL, SOCK_STREAM, ppe->p_proto);
	if (m_hSocket == INVALID_SOCKET) return false;
	sockaddr_in addrin;
    addrin.sin_family = AF_INET;
    addrin.sin_addr.s_addr = inet_addr(szHost);
    addrin.sin_port = htons(nPort);
	BOOL bRec = connect( m_hSocket, (SOCKADDR*) &addrin, sizeof(addrin));
    
	if (bRec)
		return false;
	if (bNoDelay)
		setopt(DelayTime);
	_nLastSocketError = 0;
	m_nState = SOCKET_STATE_CREATE;
	return true;
}

bool CBaseSocket::setopt(int DelayTime)
{
    try
    {
	    int nSize= 0;   
        int nError = WSAGetLastError();
        if (nError != 0)
		{
			_nLastSocketError = -1;
            return false;
		}

	    if (::setsockopt(m_hSocket,SOL_SOCKET,SO_RCVBUF,(char *)&nSize,sizeof(int))==SOCKET_ERROR)	
		    return false;	
	    int TimeOut;
		TimeOut=DelayTime>0?DelayTime: _nTimeout;
	    if (::setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
		    return false;
		_timerOut = TimeOut;
    }
    catch(...)
    {
        return false;
    }
	//_nLastSocketError = 0;
	return true;
}

bool CBaseSocket::sendData(const char* szData, UINT nLen)
{
	if (m_hSocket == NULL)
		return false;
	nLen = nLen == 0 ? strlen(szData) : nLen;	
	bool bRec = true;
	int nRetry = 0;
	UINT nSendedByte = 0;
	UINT nRemainSend = nLen;
	UINT nCurSend = 0;	
	m_nState = SOCKET_STATE_SEND_ING;
	while(nRetry <= SOCKET_WRITE_TIME)
	{	
		if (nSendedByte < nLen)
		{
			nCurSend = send(m_hSocket, szData + nSendedByte, nRemainSend, 0);
		}
		if (nCurSend == 0)
		{
			nRetry ++;
			continue;
		}
		else if ((int)nCurSend < 0 && WSAGetLastError() == WSAENOTCONN)
		{
			_nLastSocketError = -1;
			m_nState = SOCKET_STATE_ERROR;
			bRec = false;
			break;
		}
		else
		{
			nSendedByte += nCurSend;
			nRemainSend -= nCurSend;
			if (nSendedByte < nLen)
				continue;
			m_nState = SOCKET_STATE_SEND_ED;
			//_nLastSocketError = 0;
			break;
		}
	}
	if (nRetry > SOCKET_WRITE_TIME) 
		bRec = false;
    if (bRec)
	{
		//_nLastSocketError = 0;
	    m_nState = SOCKET_STATE_SEND_ED;
	}

	return bRec;
}

size_t CBaseSocket::receiveData(char** ppData, UINT &nLen, int nRetryTime)
{
	if (m_hSocket == NULL)
		return 0;
	if (*ppData == NULL && nLen > 0)
		return 0;
	if (*ppData == NULL && nLen == 0)
	{
		*ppData = (char*)malloc(RECEIVE_BUFFER_SIZE);
		memset(*ppData, '\0', RECEIVE_BUFFER_SIZE);
		nLen = RECEIVE_BUFFER_SIZE;
	}
	int nRetry = 0;
	int nReceivedByte = 0;
	int nCurLen = 0;
	m_nState = SOCKET_STATE_SEND_ING;
	
	while(nRetry < nRetryTime)
	{
		try
		{		
			//收的过程中可能手机被拔掉，缓冲被销毁
			if (m_hSocket == NULL|| *ppData == NULL)
				return 0;
			memset(m_pRecBuffer,0,sizeof(m_pRecBuffer));
			nCurLen = recv(m_hSocket, m_pRecBuffer, RECEIVE_BUFFER_SIZE, 0);
			if (nCurLen > 0)
			{			
				if (nReceivedByte + nCurLen > (int)nLen) 
				{//接收空间不足时重新分配空间
					int nNewLen = nLen + nLen;
					while (nReceivedByte + nCurLen > nNewLen)
						nNewLen += nNewLen;
					*ppData = (char*)realloc(*ppData, nNewLen);
					nLen = nNewLen;
				}
				memcpy(*ppData + nReceivedByte, m_pRecBuffer, nCurLen);
				nReceivedByte += nCurLen;
				_nLastSocketError = 0;
			}		
			else
			{
                if (nCurLen == SOCKET_ERROR)
                {
                    int nLastError = WSAGetLastError();
					_nLastSocketError = nLastError;
					int prt = (int)this;
                    LOG->WriteDebug(L"=========================== SOCKET_ERROR: " + CStrOperation::IntToWString(nLastError) +  CStrOperation::IntToWString(prt));
//                     if (nLastError == WSAENOTCONN || nLastError == WSAECONNABORTED)
//                         break;  
//					close();
                }
				else
				{
					LOG->WriteDebug(L"=========================== receiveData1:recivelen=0");
//					close();
				}
				break;
			}
		}
		catch(...)
		{

		}
	}
	m_nState = SOCKET_STATE_RECEIVE_ED;
	return nReceivedByte;
}
bool  CBaseSocket::HasBuffData()
{
 		//if (m_hSocket == NULL)
 		//	return true;
 		//char buff[2];
 		// int TimeOut=10;
 		//if (::setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
 		//	return true;
 		//int nCurLen = recv(m_hSocket, buff,1, 0);
 
 		//if(nCurLen <= 0)
 		//{
 		//	TimeOut = _timerOut;
 		//	if (::setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
 		//		return true;
 		//	else
 		//		return false;
 		//}
 		//else
 		//{
 		//	return true;
 		//}
	bool hasRemained = false;
	UINT oldTimeout = _timerOut;

	fd_set fd;
	FD_ZERO(&fd);
	FD_SET(m_hSocket, &fd);

	struct timeval timet;
	timet.tv_sec = 0;
	timet.tv_usec = 10;

	int err = select((int)m_hSocket + 1, &fd, NULL, NULL, &timet);

	//返回0表示超时，返回-1表示socket错误
	if(err != 0 && err != -1 && FD_ISSET(m_hSocket, &fd))
		hasRemained = true;


	return hasRemained;

}
string CBaseSocket::receiveData(UINT nSize, int nRetryTime)
{
	if (m_hSocket == NULL)
		return "";
	int nRetry = 0;
	int nReceivedByte = 0;
	int nRemindByte = nSize == 0 ? RECEIVE_BUFFER_SIZE : nSize;
	int nCurLen = 0;
	m_nState = SOCKET_STATE_RECEIVE_ING;
	string sReceive="";
    int nTickCount = ::GetTickCount();
	
	while(nRetry < nRetryTime)
	{
		if (m_hSocket == NULL)
			return "";
		memset(m_pRecBuffer,0,sizeof(m_pRecBuffer));
		nCurLen = recv(m_hSocket, m_pRecBuffer, min(RECEIVE_BUFFER_SIZE, nRemindByte), 0);
		if (nCurLen > 0)
		{
			sReceive += string(m_pRecBuffer, nCurLen);
			if (nSize > 0)
			{
				nRemindByte -= nCurLen;
				nReceivedByte += nCurLen;
				if (nReceivedByte >= (int)nSize)
				{
					break;
				}
			}
			else if (sReceive.length() >= 12 && sReceive.find("NDPC") == 0)
			{//对Android包头做特殊处理，以便收到完整数据后马上返回 不再等待
				_nLastSocketError = 0;
				int nSize = 0;
				memcpy(&nSize, sReceive.c_str() + 8, 4);
				if (nSize + 12 == sReceive.length())
				{
					break;
				}
			}
			else if (nCurLen == 9 && memcmp(m_pRecBuffer, "Success", 7) == 0 || nCurLen == 6 && memcmp(m_pRecBuffer, "Fail", 4) == 0)
			{
				_nLastSocketError = 0;
				break;
			}			
		}
		else
		{
            if (nCurLen == SOCKET_ERROR)
            {
                int nLastError = WSAGetLastError();
				_nLastSocketError = nLastError;
				int prt = (int)this;
				LOG->WriteDebug(L"=========================== SOCKET_ERROR: " + CStrOperation::IntToWString(nLastError) +  CStrOperation::IntToWString(prt));
               // if (nLastError == WSAENOTCONN || nLastError == WSAECONNABORTED)
               //     break;
				//close();
				break;
            }
			else
			{
				LOG->WriteDebug(L"=========================== receiveData:recivelen=0");
				//close();
				//Sleep(50);
				//nRetry ++; 
				break;
			}
			//nRetry ++;  			
			
		}
	}
    int nTick = ::GetTickCount() - nTickCount;
    if (nTick >= COMMAND_TIME_OUT)
    {
        LOG->WriteDebug(L"Out Time "  + CStrOperation::IntToWString(nTick));
    }
	if (nRetry >= nRetryTime)
	{
		
	}

	m_nState = SOCKET_STATE_RECEIVE_ED;
	return sReceive;
}

bool CBaseSocket::receiveDataLength(char* pData, UINT nReceiveSize, int nRetryTime)
{
	int nReceivedByte = 0;
	int nRemain = nReceiveSize;
	m_nState = SOCKET_STATE_RECEIVE_ING;
    int nRetry = 0; 
	while (nReceivedByte < (int)nReceiveSize)
	{
		int nCurLen = recv(m_hSocket, pData + nReceivedByte, nRemain, 0);
        if (nCurLen > 0)
        {
            nReceivedByte += nCurLen;
            nRemain -= nCurLen;
		//	_nLastSocketError = 0;
        }
        else
        {
            nRetry ++;
            if (nCurLen == 0)
            {
		//		_nLastSocketError = 0;
				LOG->WriteDebug(L"=========================== receiveDataLength:recivelen=0");
			//	close();
				return false;
            }
            else
            {
                if (nCurLen == SOCKET_ERROR)
                {
                    int nLastError = WSAGetLastError();
					_nLastSocketError = nLastError;
					int prt = (int)this;
					LOG->WriteDebug(L"=========================== SOCKET_ERROR: " + CStrOperation::IntToWString(nLastError) +  CStrOperation::IntToWString(prt));
//                     if (nLastError == WSAENOTCONN || nLastError == WSAECONNABORTED)
//                         break;
			//		close();
					return false;
                }
            }
			break;
        }
	}
	m_nState = SOCKET_STATE_RECEIVE_ED;
	return nReceivedByte == nReceiveSize;
}
bool CBaseSocket::close()
{
	if (m_hSocket != NULL && m_nState != SOCKET_STATE_CLOSE)
	{
		closesocket(m_hSocket);
		m_nState = SOCKET_STATE_CLOSE;
		return true;
	}
	return false;
}

bool CBaseSocket::isValid()
{
	return m_hSocket != NULL;
}

int	 CBaseSocket::getState()
{
	return m_nState;
}

int CBaseSocket::getLastError()
{
	return WSAGetLastError();
}
////////////////////////////////////////////////////////////
short CBaseSocket::getActiveSocketPort()
{
	SOCKET hSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (hSocket==NULL)
	{
		return -1;
	}

	sockaddr_in addr;
	addr.sin_family=AF_INET;
#ifdef WIN32
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
#else
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
#endif

	short nTmpPort = 5900;
	for (size_t i =0;i<40;i++)
	{
		addr.sin_port=htons(nTmpPort);
		int nRet = bind(hSocket,(LPSOCKADDR)&addr,sizeof(addr));
		if(nRet != SOCKET_ERROR)
		{
			break;
		}

		nTmpPort += 2;
	}

	closesocket(hSocket);
	return nTmpPort;
}

bool CBaseSocket::isPortUsed(const UINT nPort)
{
#ifdef WIN32
    // 初始化 PMIB_TCPTABLE 结构
    unsigned long dwSize = sizeof (MIB_TCPTABLE);
    PMIB_TCPTABLE pTcpTable = (MIB_TCPTABLE *) new char[dwSize];
    if (pTcpTable == NULL)
    {
        THROW_SYSTEM_EXCETPION(MEMORY_OUT);
    }

    // Make an initial call to GetTcpTable to get the necessary size into the dwSize variable
    if (ERROR_INSUFFICIENT_BUFFER == GetTcpTable(pTcpTable, &dwSize, false))
    {
        delete[] pTcpTable;
        pTcpTable = (MIB_TCPTABLE *) new char[dwSize];
        if (pTcpTable == NULL)
        {
            THROW_SYSTEM_EXCETPION(MEMORY_OUT);
        }
    }

    // Make a second call to GetTcpTable to get the actual data we require
    if (NO_ERROR != GetTcpTable(pTcpTable, &dwSize, TRUE) )
    {
        delete[] pTcpTable;
        THROW_SYSTEM_EXCETPION(GET_TCP_TABLE_FAIL);
    }

    int count = (dwSize - sizeof(int)) / sizeof(MIB_TCPROW);
    for (size_t i = 0; i < pTcpTable->dwNumEntries; i++)
    {
//         struct in_addr local, remote;
//         local.S_un.S_addr = pTcpTable->table[i].dwLocalAddr;
//         remote.S_un.S_addr = pTcpTable->table[i].dwRemoteAddr;
//         wostringstream oss;
//         oss << i << " : "
//             << " local address =" << inet_ntoa(local)
//             << ", local port =" << ntohs(pTcpTable->table[i].dwLocalPort)
//             << ", remote address =" << inet_ntoa(remote)
//             << ", remote port =" << ntohs(pTcpTable->table[i].dwRemotePort)
//             ;
//         LOG->WriteDebug(oss.str());

        // 判断指定端口是否正在监听
        if ( nPort == ntohs((u_short)pTcpTable->table[i].dwLocalPort)
            && MIB_TCP_STATE_LISTEN == pTcpTable->table[i].dwState)
        {
            delete[] pTcpTable;
            return true;
        }
    }

    delete[] pTcpTable;
    return false;
#else
	return false; // ignore android part temporary in OS X  by qiuchang 12.3.20
#endif
}

UINT CBaseSocket::getLocalPort()
{
    UINT nLocalPort = 0;
    struct sockaddr addr;
    struct sockaddr_in* addr_v4;
#ifdef WIN32
    int addr_len = sizeof(addr);
#else
    socklen_t addr_len = sizeof(addr);
#endif
    //获取local ip and port
    memset(&addr, 0, sizeof(addr));
    if (0 == getsockname(m_hSocket, &addr, &addr_len))
    {
        if (addr.sa_family == AF_INET)
        {
            addr_v4 = (sockaddr_in*)&addr;
            nLocalPort = ntohs(addr_v4->sin_port);
        }
    }
    return nLocalPort;
    /*
    //获取remote ip and port
    UINT nRemotePort = 0;
    ZeroMemory(&addr, sizeof(addr));
    if (0 == getpeername(m_hSocket, &addr, &addr_len))
    {
        if (addr.sa_family == AF_INET)
        {
            addr_v4 = (sockaddr_in*)&addr;
            //cMonitorInf.nDesIP = addr_v4->sin_addr.S_un.S_addr;
            nRemotePort = ntohs(addr_v4->sin_port);
            //cMonitorInf.nDesPort = ntohs(addr_v4->sin_port);
        }
    }
    return nRemotePort;
    */
}

int ND91Assistant::CBaseSocket::GetFreeTime()
{ 
	return GetTickCount() - _nLastUseTime;
}

char* ND91Assistant::CBaseSocket::GetHost()
{
    return m_pHost;
}

int ND91Assistant::CBaseSocket::GetPort()
{
    return m_nPort;
}

void ND91Assistant::CBaseSocket::Release()
{
    m_nState = SOCKET_STATE_NONE;
}

bool ND91Assistant::CBaseSocket::IsSocketAvailable()
{
    if (m_nState == SOCKET_STATE_NONE || m_nState == SOCKET_STATE_CLOSE)
        return false;

    return WSAGetLastError() != WSAENOTCONN;
}

int ND91Assistant::CBaseSocket::GetSockLastError()
{
	return _nLastSocketError;
}