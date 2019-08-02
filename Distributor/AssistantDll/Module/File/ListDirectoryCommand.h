#pragma once

#include "Module/BaseCommand.h"
#include "Common/greta/RegexprHelper.h"

namespace ND91Assistant
{
    // 读取目录
    class ListDirectoryCommand : public BaseCommand
    {
    public:
        //************************************
        // Method:    构造函数
        // Parameter: const DeviceData* pDevice 设备指针
        // Parameter: NodeData * pNode  若是目录查询其子节点，否则查询单个文件
        //************************************
        ListDirectoryCommand(const DeviceData* pDevice, NodeData* pNode);
        virtual ~ListDirectoryCommand();

        // 改变节点
        // pNode  若是目录查询其子节点，否则查询单个文件信息
        void SetNode(NodeData* pNode);

        // 执行命令
        virtual void Execute();

        // 命令执行是否成功
		bool  DoneSuccess();

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
		bool        _success;
        RegexprHelper _rh;

	private:

        // 解析返回结果
        // 返回 true ：成功；返回false：失败
		bool ParseResult(const string& result);
    };
}
