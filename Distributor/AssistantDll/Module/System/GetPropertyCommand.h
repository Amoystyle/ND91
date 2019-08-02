#pragma once

#include "Module/BaseCommand.h"
#include "Core/DeviceInfo.h"

#define ANDROID_SHELL_GETPROPERTY       "getprop"

namespace ND91Assistant
{
    // 取硬件信息命令
    class GetPropertyCommand : public BaseCommand
    {
    public:
        GetPropertyCommand(const DeviceData* pDevice);
        virtual ~GetPropertyCommand();

        // 执行命令
        virtual void Execute();

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

        DeviceInfo* GetDeviceInfo(){return &_DeviceInfo;};
        bool Success(){return _success;};

    private:
        //存储解析后的信息，注意跟构造函数传进来的DeviceInfo指针区分开
        DeviceInfo _DeviceInfo;
        bool _success;
    };
}