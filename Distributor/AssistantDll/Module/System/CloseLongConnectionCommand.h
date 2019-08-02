#pragma once
#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // 关闭长连接(心跳线程)
    // 如果退出前不发送该命令，守护在一定时间后也会自动关闭，
    class CloseLongConnectionCommand : public BaseCommand
    {
    public:
        CloseLongConnectionCommand(const DeviceData* pDevice);

        // 执行命令
        virtual void Execute() {};

        // 生成Android命令数据包
        virtual std::string GetAndroidBodyData();

        // 解析Android返回结果数据包
        virtual void SetAndroidReturnData(std::string data);

        // 命令传送到IOS设备上的文件名（全路径）
        virtual std::wstring GetIosCommandFilePath() { return L""; }

        // 从IOS上获取的命令结果文件名（全路径）
        virtual std::wstring GetIosResultFilePath() { return L""; }

        // 生成ios文件。返回文件名，调用者负责删除文件
        virtual std::wstring GetIosCommandFileContent() { return L""; }

        // 解析返回文件，解析后删除文件
        virtual  void SetIosReturnFile(std::wstring filename) { }

    private:
    };
}