#pragma once

#include "Module/Call/CallData.h"

namespace ND91Assistant
{
    // 取通话记录命令
    class GetCallDataCommand : public BaseCommand
    {
    public:

        GetCallDataCommand(const DeviceData* pDevice);
        virtual ~GetCallDataCommand();

        // 设置是否只获取数据数量
        void SetOnlySize(bool onlySize);

        // 执行命令
        virtual void Execute();

        // 返回数据
        const vector<CallData*>* GetDataList();

        // 返回数据数量
        int GetSize();

    private:

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

        // 执行IOS命令
        bool IosExecute();

    private:
        vector<CallData*> _dataList;
        
        bool _onlySize;// 是否只获取数量

        int _size;     // 数据数量
    };
}