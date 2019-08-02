#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // 获取android设备是否已root
    class GetResolutionCommand : public BaseCommand
    {
    public:
        GetResolutionCommand(const DeviceData* pDevice);
        virtual ~GetResolutionCommand();

        // 执行命令
        virtual void Execute();

        // 生成Android命令数据包
        virtual std::string GetAndroidBodyData();

        // 解析Android返回结果数据包
        virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}


        wstring GetResult(){return _strSize;}

    private:
        
		wstring _strSize;
    };
}