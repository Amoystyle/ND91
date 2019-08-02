
#include "AndroidWifiIO.h"

using namespace ND91Assistant;

std::string AndroidWifiIO::Send(std::string data,  bool bReceiveResult)
{
    // 发送命令
    _pBaseSocket->sendData(data.c_str(), data.length());

    // 接收返回结果
    if ( bReceiveResult )
    {
        return _pBaseSocket->receiveData(0);
    }
    else
    {
        return "";
    }
}

std::string ND91Assistant::AndroidWifiIO::Receive( UINT aSize )
{
	return _pBaseSocket->receiveData(aSize);
}