#include "stdafx.h"

#include "AndroidTcpIO.h"
#include "Common/BaseSocket.h"
#include "ConnectionPool.h"
#include "BaseIO.h"

ND91Assistant::AndroidTcpIO::AndroidTcpIO( int nConnectionID, ConnectionPool* pConnectionPool ) 
: BaseIO()
{
    _nConnectionID = nConnectionID;
    _pConnectionPool = pConnectionPool;
}

ND91Assistant::AndroidTcpIO::~AndroidTcpIO()
{
    if (_pConnectionPool)
        _pConnectionPool->Putback(_nConnectionID);
}

std::string AndroidTcpIO::SendAndroidDatagram(std::string data,  bool bReceiveResult)
{
    if (!_pConnectionPool) return "";
    // 发送命令
    bool bRec = _pConnectionPool->SendData(_nConnectionID, data.c_str(), data.length());
    if (!bRec && _pConnectionPool->GetConnectState(_nConnectionID) == SOCKET_STATE_ERROR && data.length() < 100)
    { 
        // 通讯错误，重新连接
        if(_pConnectionPool->ResetConnect(_nConnectionID))      
			bRec = _pConnectionPool->SendData(_nConnectionID, data.c_str(), data.length());
		else
		{
			LOG->WriteDebug(L"socket create fail");
			return "socket create fail";
		}
    }

    // 接收返回结果
    return bReceiveResult ? Receive() : "";
}

std::string AndroidTcpIO::Receive( /*UINT*/unsigned int size )
{
    if (!_pConnectionPool) return "";
    return _pConnectionPool->ReceiveData(_nConnectionID, size);
}

std::wstring AndroidTcpIO::SendIosFile(std::wstring strCmdFileOnPhone, std::wstring strResultFileOnPhone, std::wstring strCmdFileContent, bool bReceiveResult /*= true*/)
{
    THROW_ASSISTANT_EXCEPTION(PROGRAM_ERROR);
	return L"";
}


bool ND91Assistant::AndroidTcpIO::IsConnectionAvailable()
{
    if (!_pConnectionPool) return false;
    return _pConnectionPool->IsConnectionAvailable(_nConnectionID);
}

