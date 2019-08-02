#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // 获取android设备名字
    class GetDeviceNameCommand : public BaseCommand
    {
    public:
        GetDeviceNameCommand(const DeviceData* pDevice);
        virtual ~GetDeviceNameCommand();

        // 执行命令
        virtual void Execute();

        // 生成Android命令数据包
        virtual std::string GetAndroidBodyData();

        // 解析Android返回结果数据包
        virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}

        wstring GetDeviceName(){ return _strDeviceName; }

    private:
        
		wstring _strDeviceName;
    };
}