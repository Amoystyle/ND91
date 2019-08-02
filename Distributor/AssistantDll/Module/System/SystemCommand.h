#pragma once

#include "Module/BaseCommand.h"
#include "Common/BinaryBufferWriter.h"

namespace ND91Assistant
{
    //生成和处理心跳包
	class SystemCommand
	{
	public:

		// 生成Android数据体
        // 参数 port : 长连接的本地端口
        static std::string GetAndroidDatagram(short port);

        void setLongConnectSign();

		// 解析返回数据包
        void ParseturnDatagram(std::string data);

		// 生成ios文件。返回文件名，调用者负责删除文件
        std::string GetIosCommandFile() { return ""; }

		// 解析返回文件，解析后删除文件
        void SetIosReturnFile(std::string filename) {}

        // 检查数据包头的格式
        // 返回true表示正确，返回false表示错误
        bool VerifyPackageHead();

        // 将数据内容转为字符串输出
        std::wstring ToString();

        int GetDataLength() { return _nDataLength; }

        int GetCommandId() { return _nCommandID; }
    private:
        char _pHead[5];      // 包头字符串
        int _nFlag;          // 数据包标志
        int _nDataLength;    // 数据包长度
        int _nCommandID;     // 命令类型
	};
}