#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    class AppData;

    // 获取android设备是否已root
    class UninstallSystemAppCommand : public BaseCommand
    {
    public:
        UninstallSystemAppCommand(const DeviceData* pDevice, const AppData* appData);
        virtual ~UninstallSystemAppCommand();

        // 执行命令
        virtual void Execute();

        // 生成Android命令数据包
        virtual std::string GetAndroidBodyData();

        // 解析Android返回结果数据包
        virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}


        bool GetResult(){return _bSuccess;}

    private:
        
		bool _bSuccess;

		const AppData* _appData;
    };
}