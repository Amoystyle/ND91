#pragma once

#include "AndroidTcpIO.h"

namespace ND91Assistant
{
    // ��׿wifi�շ�
    class AndroidWifiIO : public AndroidTcpIO
    {
    public:
        AndroidWifiIO(CBaseSocket* pBaseSocket) : AndroidTcpIO(pBaseSocket) {}

		virtual std::string Send(std::string  data, bool bReceiveResult = true);
		virtual std::string Receive(UINT aSize);
    };
	
}
