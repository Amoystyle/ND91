#pragma once

#include "AndroidTcpIO.h"

namespace ND91Assistant
{
    // 安卓USB收发
    class AndroidUsbIO : public AndroidTcpIO
    {
    public:
        AndroidUsbIO(CBaseSocket* pBaseSocket) : AndroidTcpIO(pBaseSocket) {}

        // 发送数据，并接收返回值
        virtual std::string Send(std::string data, bool bReceiveResult = true);

		virtual std::string Receive(UINT aSize);
    };
}
