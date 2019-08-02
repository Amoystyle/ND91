#pragma  once

#include "Core/DeviceInfo.h"

#include <string>
namespace ND91Assistant
{

    class CBaseSocket;

    // IO基类
    class BaseIO
    {
    public:
        BaseIO();
        virtual ~BaseIO();

        // summary  : 发送安卓命令并返回结果
        // return   : std::string         : 命令数据包
        // parameter: std::string data    : 结果数据包（二进制）
        // parameter: bool bReceiveResult : true=接收返回值 false=不接收返回值
        virtual std::string SendAndroidDatagram(std::string data, bool bReceiveResult = true) = 0;

        // summary  : 发送ios命令并返回结果
        // return   : std::wstring						: 结果文件名(unicode)
        // parameter: std::wstring strCmdFileOnPhone	: 上传至设备上的命令文件名(全路径，unicode)
		// parameter: std::wstring strResultFileOnPhone	: 下载至PC上的结果文件名(全路径，unicode)
		// parameter: std::wstring strCmdFileContent	: 命令文件内容(unicode)
        // parameter: bool bReceiveResult				: true=接收返回值 false=不接收返回值
		virtual std::wstring SendIosFile(std::wstring strCmdFileOnPhone, std::wstring strResultFileOnPhone, std::wstring strCmdFileContent, bool bReceiveResult = true) = 0;

    protected:
    };
}

