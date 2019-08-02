#include "stdafx.h"

#include "Device/ConnectionPool.h"
#include "Common/BaseSocket.h"
#include "core/DeviceInfo.h"

ConnectionPool::ConnectionPool(const DeviceInfo* pDeviceInfo)
{
	_hThread = NULL;
	_bThreadQuitSign = false;
	_nThreadNumber = 0;
    _nCurID = 1;
    _nLastFindIndex = 0;
    _strHost = CCodeOperation::UnicodeToGB2312(pDeviceInfo->_strHost);
    _nPort   = pDeviceInfo->_nPort;
	InitializeCriticalSection(&_cs);
}

ConnectionPool::~ConnectionPool()
{
    ReleaseAll();
}

void ND91Assistant::ConnectionPool::ReleaseAll()
{
	//退出线程
	if (_hThread && !_bThreadQuitSign)
	{
		_bThreadQuitSign = true;
		WaitForSingleObject(_hThread, 2000); 
	}
    SetCSLock(_cs);
    for (vector<IOConnection*>::iterator it = _ConnectionList.begin(); it != _ConnectionList.end(); it++)
    {
        IOConnection* pConnection = (IOConnection*)(*it);
        pConnection->CloseSocket();
        // 为了安全，这个版本先不做释放
        //if (!pConnection->IsUsed())
        //    delete pConnection;
    }
    _ConnectionList.clear();
}

#ifdef WIN32
    DWORD WINAPI ConnectionPool::ConnectionPoolRun( LPVOID lpParameter )
#else
    void* ConnectionPoolRun(void* lpParameter)
#endif
{
    ConnectionPool* pThis = (ConnectionPool*)lpParameter;
    while (!pThis->SetQuitSign())
    {
        pThis->CheckOffTime();
        Sleep(200);
    }
	return 0;
}

ConnectionPool::IOConnection* ConnectionPool::CreateConnetion()
{
    IOConnection* pConnection = new IOConnection(_nCurID++, _strHost.c_str(), _nPort);
    _nThreadNumber ++;
    _ConnectionList.push_back(pConnection);
    LOG->WriteDebug(L"ConnectionPool Connection Count = " + CStrOperation::IntToWString(_ConnectionList.size()));
    //pBaseSocket->setConnectionPool(this);
    return pConnection;
}

void ND91Assistant::ConnectionPool::init(int nMinThread/*=1*/, int nMaxThread/*=20*/ )
{
    SetCSLock(_cs);
    _nMinThreadNumber = nMinThread;
    _nMaxThreadNumber = nMaxThread;
    for (int i=_ConnectionList.size(); i<nMinThread; i++)
    {
        CreateConnetion();
    }	

	//如果有初始化过，则先退出线程
	if (_hThread && !_bThreadQuitSign)
	{
		_bThreadQuitSign = true;
		WaitForSingleObject(_hThread,1000); 
	}
	// 启动管理线程
    ::DWORD	nThreadId;
#ifdef WIN32
	_hThread = CreateThread(NULL, 0, ConnectionPool::ConnectionPoolRun, (LPVOID)this, 0, &nThreadId);
#else
	pthread_create(&_hThread, NULL, ConnectionPoolRun, (LPVOID)this);
#endif

}

int ND91Assistant::ConnectionPool::Get()
{
	EnterCriticalSection(&_cs);
    // 查找可用的连接
    IOConnection* pConnection = FindAvail();
    if ( pConnection ) 
    {//找到可用的，直接返回
		LOG->WriteDebug(L"ConnectionPool Get:" + _nCurID );
        goto END;
    }
	else
	{
		LOG->WriteDebug(L"ConnectionPool CreateConnetion:" + _nCurID );
		 pConnection = CreateConnetion();
		 goto END;
	}
//     else if ( IsCanCreate() )
//     {// 无则创建连接
//         pConnection = CreateConnetion();
//         goto END;        
//     }
//     else 
//     {// 实在不行就等待
//         while ( !pConnection )
//         {
// 			LOG->WriteDebug("ConnectionPool wait for free socket!");
//             LeaveCriticalSection(&_cs);
//             Sleep(1000);
//             EnterCriticalSection(&_cs);
//             pConnection = FindAvail();
//         }          
//     }
END:
    if (pConnection) pConnection->SetUsed(true);
	LeaveCriticalSection(&_cs);
    return pConnection ? pConnection->GetID() : -1;
}

void ND91Assistant::ConnectionPool::Putback( int nID )
{
	SetCSLock(_cs);
    for (vector<IOConnection*>::iterator it = _ConnectionList.begin(); it != _ConnectionList.end(); it++)
    {
        IOConnection* pConnection = *it; 
        if ( pConnection->GetID() == nID )
        {
			if(pConnection->GetSocket()->GetSockLastError() !=0 ||
				pConnection->GetSocket()->getLastError()!=0 
				|| !pConnection->GetSocket()->isValid()
				|| !pConnection->GetSocket()->IsSocketAvailable()
				|| pConnection->GetSocket()->HasBuffData())
			{				
				_nThreadNumber--;
				SAFE_DELETE(pConnection);
				_ConnectionList.erase(it);
				return;
			}
            pConnection->SetUsed(false);
            break;
        }
		
    }    
}

void ND91Assistant::ConnectionPool::CheckOffTime()
{
	SetCSLock(_cs);
	for (vector<IOConnection*>::iterator it = _ConnectionList.begin(); it != _ConnectionList.end();)
	{
        IOConnection* pConnection = *it; 
        if (pConnection->GetFreeTime() > CONNECT_OFF_TIME && _nThreadNumber > _nMinThreadNumber)
        {
			_nThreadNumber--;
			SAFE_DELETE(pConnection);
            it = _ConnectionList.erase(it);
            break;
        }
        else
            it ++;
	}
}

ConnectionPool::IOConnection* ND91Assistant::ConnectionPool::FindAvail()
{
    for (vector<IOConnection*>::iterator it = _ConnectionList.begin(); it != _ConnectionList.end(); it++)
    {
        IOConnection* pConnection = *it; 
        if ( !pConnection->IsUsed() && pConnection->GetSocket()->isValid() )
            return pConnection;
    }
    return NULL;

}

bool ND91Assistant::ConnectionPool::IsCanCreate()
{
    return GetConnectCount() <= _nMaxThreadNumber; 
}

int ND91Assistant::ConnectionPool::GetConnectCount()
{
    return _nThreadNumber;
}

void ND91Assistant::ConnectionPool::SetConnectionUsed( int nID, bool bUsed)
{
    IOConnection* pConnection = FindConnection(nID);
    if (pConnection)
        pConnection->SetUsed(bUsed);
}

bool ND91Assistant::ConnectionPool::ResetConnect( int nID )
{
    IOConnection* pConnection = FindConnection(nID);
    if (pConnection)
	{
      return   pConnection->CreateSocket(); //重建连接
	}
	return false;
}

ConnectionPool::IOConnection* ND91Assistant::ConnectionPool::FindConnection( int nID )
{
    SetCSLock(_cs);
    for (vector<IOConnection*>::iterator it = _ConnectionList.begin(); it != _ConnectionList.end(); it++)
    {
        IOConnection* pIOSocket = *it; 
        if ( pIOSocket->GetID() == nID )
            return pIOSocket;
    }
    return NULL;
}

bool ND91Assistant::ConnectionPool::SendData( int nID, const char* szData, UINT nLen )
{
    IOConnection* pConnection = FindConnection(nID);
    if (!pConnection) return false;

	 CBaseSocket* pSocket = pConnection->GetSocket();
	  if (!pSocket) return SOCKET_STATE_NONE;
    return pSocket->sendData(szData, nLen);
}

string ND91Assistant::ConnectionPool::ReceiveData( int nID, unsigned int size )
{
    IOConnection* pConnection = FindConnection(nID);
    if (!pConnection) return "";

	 CBaseSocket* pSocket = pConnection->GetSocket();
	  if (!pSocket) return "";
    return pSocket->receiveData(size);
}

int ND91Assistant::ConnectionPool::GetConnectState( int nID )
{
    IOConnection* pConnection = FindConnection(nID);
    if (!pConnection) return SOCKET_STATE_NONE;

	 CBaseSocket* pSocket = pConnection->GetSocket();
	  if (!pSocket) return SOCKET_STATE_NONE;
    return pSocket->getState();
}

bool ND91Assistant::ConnectionPool::IsConnectionAvailable( int nID )
{
    IOConnection* pConnection = FindConnection(nID);
    if (!pConnection) return SOCKET_STATE_NONE;

    CBaseSocket* pSocket = pConnection->GetSocket();
    if (!pSocket) return SOCKET_STATE_NONE;

	if(pSocket->GetSockLastError() !=0)
		return false;

    return pSocket->IsSocketAvailable();
}

