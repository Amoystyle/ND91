#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // 获取MAC地址
    class GetMacAddressCommand : public BaseCommand
    {
    public:
        GetMacAddressCommand(const DeviceData* pDevice);
        virtual ~GetMacAddressCommand();

        // 执行命令
        virtual void Execute();

        // 生成Android命令数据包
        virtual std::string GetAndroidBodyData();

        // 解析Android返回结果数据包
        virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}

        wstring GetMacAddress(){ return _macAddress; }

    private:
        
		wstring _macAddress;
    };
}