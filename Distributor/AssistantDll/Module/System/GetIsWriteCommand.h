#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // 获取android设备system/app 目录是否可写
    class GetIsWriteCommand : public BaseCommand
    {
    public:
        GetIsWriteCommand(const DeviceData* pDevice);
        virtual ~GetIsWriteCommand();

        // 执行命令
        virtual void Execute();

        // 生成Android命令数据包
        virtual std::string GetAndroidBodyData();

        // 解析Android返回结果数据包
        virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}


        bool GetResult(){return _bWrite;}

    private:
        
		bool _bWrite;
    };
}