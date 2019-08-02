#pragma once
#include "BaseIO.h"
#include "ConnectionPool.h"
namespace ND91Assistant
{
    // 安卓 TCP 方式收发
    class AndroidTcpIO : public BaseIO
    {
    public:
        AndroidTcpIO(int nConnectionID, ConnectionPool* pConnectionPool );

        ~AndroidTcpIO();

        // summary  : 发送安卓命令并返回结果
        // return   : std::string         : 结果数据包（二进制）
        // parameter: std::string data    : 命令数据包
        // parameter: bool bReceiveResult : true=接收返回值 false=不接收返回值
        virtual std::string SendAndroidDatagram(std::string data, bool bReceiveResult = true);

        // ios用的方法，不实现
		virtual std::wstring SendIosFile(std::wstring strCmdFileOnPhone, std::wstring strResultFileOnPhone, std::wstring strCmdFileContent, bool bReceiveResult = true);

        // 接收数据
        // 参数 size : 指定接收的字节数，0则接收全部
        virtual std::string Receive(unsigned int size = 0);

        // 检查连接是否有效，如果无效，应终止所有重试操作
        bool IsConnectionAvailable();

    private:
        int _nConnectionID;
        ConnectionPool* _pConnectionPool;
    };
}
