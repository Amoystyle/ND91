#pragma once

#include "AndroidTcpIO.h"

namespace ND91Assistant
{
    // ��׿USB�շ�
    class AndroidUsbIO : public AndroidTcpIO
    {
    public:
        AndroidUsbIO(CBaseSocket* pBaseSocket) : AndroidTcpIO(pBaseSocket) {}

        // �������ݣ������շ���ֵ
        virtual std::string Send(std::string data, bool bReceiveResult = true);

		virtual std::string Receive(UINT aSize);
    };
}
