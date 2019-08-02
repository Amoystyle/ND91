#pragma once

#include "AdbRemoteCommand.h"

// ӳ��˿�
#define	ADB_CMD_FORWARD_TCP		"host-serial:%s:forward:tcp:%d;tcp:%d"	

namespace ND91Assistant
{
    // ӳ��ADB�˿�����
    // ӳ��֮��ADB��ת���������˿ڵ�����
    class AdbForwardTcpCommand : public AdbRemoteCommand
    {
    public:
        // sn: �豸���к�
        // pcPort: pc�� socket�˿�
        // devicePort: �豸��socket�˿�
        AdbForwardTcpCommand(const std::wstring sn,wstring id, int pcPort, int devicePort);

    protected:
		virtual void InternalExecute();

        // ������������
        // ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
        virtual bool ParseReturnData();

    private:
        int _pcPort;
        int _devicePort;
    };

}