#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // 获取android设备是否已root
    class GetIsRootCommand : public BaseCommand
    {
    public:
        GetIsRootCommand(const DeviceData* pDevice);
        virtual ~GetIsRootCommand();

        // 执行命令
        virtual void Execute();

        // 生成Android命令数据包
        virtual std::string GetAndroidBodyData();

        // 解析Android返回结果数据包
        virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}


        bool GetResult(){return _isRoot;}

    private:
        
		bool _isRoot;
    };
}