///////////////////////////////////////////////////////////
//  ReceiveMessageCommand.h
//  Implementation of the Class ReceiveMessageCommand
//  Created on:      01-六月-2011 9:43:02
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(EA_97D6206D_9831_45c5_A5C3_2726AD0C2494__INCLUDED_)
#define EA_97D6206D_9831_45c5_A5C3_2726AD0C2494__INCLUDED_

#include "Module/BaseCommand.h"

namespace ND91Assistant
{

    class ReceiveMessageCommand : public BaseCommand
    {

    public:
        ReceiveMessageCommand(const DeviceData* pDevice);
        virtual ~ReceiveMessageCommand();

        // 执行命令
        virtual void Execute() {}

        // 生成Android命令数据包
        virtual std::string GetAndroidBodyData() { return ""; }

        // 解析Android返回结果数据包
        virtual void SetAndroidReturnData(std::string data) {}

        // 命令传送到IOS设备上的文件名（全路径）
        virtual std::wstring GetIosCommandFilePath() { return L""; }

        // 从IOS上获取的命令结果文件名（全路径）
        virtual std::wstring GetIosResultFilePath() { return L""; }

        // 生成ios文件。返回文件名，调用者负责删除文件
        virtual std::wstring GetIosCommandFileContent() { return L""; }

        // 解析返回文件，解析后删除文件
        virtual  void SetIosReturnFile(std::wstring filename) { }

    private:
        BinaryBufferWriter _BodyWriter;
    };
}

#endif // !defined(EA_97D6206D_9831_45c5_A5C3_2726AD0C2494__INCLUDED_)
