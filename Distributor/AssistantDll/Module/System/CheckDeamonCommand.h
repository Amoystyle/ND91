///////////////////////////////////////////////////////////
//  CheckDeamonCommand.h
//  Implementation of the Class CheckDeamonCommand
//  Created on:      01-六月-2011 9:42:51
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_2C5E8742_3520_4e6c_9E51_84FF83E6B33B__INCLUDED_)
#define EA_2C5E8742_3520_4e6c_9E51_84FF83E6B33B__INCLUDED_

#include "Module/BaseCommand.h"
namespace ND91Assistant
{
    // 检测守护命令
    class CheckDeamonCommand : public BaseCommand
    {

    public:
        CheckDeamonCommand(const DeviceData* pDevice);
        virtual ~CheckDeamonCommand();

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

    private:
        //守护的版本号
        std::string _strVersion;
        BinaryBufferWriter _BodyWriter;

    public:
        std::string getVersion()
        {
            return _strVersion;
        };

    };
}

#endif // !defined(EA_2C5E8742_3520_4e6c_9E51_84FF83E6B33B__INCLUDED_)
