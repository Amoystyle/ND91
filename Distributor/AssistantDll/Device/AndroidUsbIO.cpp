
#include "AndroidUsbIO.h"

using namespace ND91Assistant;

std::string AndroidUsbIO::Send(std::string data,  bool bReceiveResult)
{
    // ��������
	_pBaseSocket->sendData(data.c_str(), data.length());

    // ���շ��ؽ��
    if ( bReceiveResult )
    {
        return _pBaseSocket->receiveData(0);
    }
    else
    {
        return "";
    }
}

std::string ND91Assistant::AndroidUsbIO::Receive( UINT aSize )
{
	return _pBaseSocket->receiveData(aSize);
}