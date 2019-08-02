#pragma once

#include "AdbRemoteCommand.h"

// 映射端口
#define	ADB_CMD_FORWARD_TCP		"host-serial:%s:forward:tcp:%d;tcp:%d"	

namespace ND91Assistant
{
    // 映射ADB端口命令
    // 映射之后，ADB将转发这两个端口的数据
    class AdbForwardTcpCommand : public AdbRemoteCommand
    {
    public:
        // sn: 设备序列号
        // pcPort: pc端 socket端口
        // devicePort: 设备端socket端口
        AdbForwardTcpCommand(const std::wstring sn,wstring id, int pcPort, int devicePort);

    protected:
		virtual void InternalExecute();

        // 解析返回数据
        // 返回值：true 表示成功，false 表示失败
        virtual bool ParseReturnData();

    private:
        int _pcPort;
        int _devicePort;
    };

}