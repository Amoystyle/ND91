#pragma once

#include "Module/BaseCommand.h"

namespace ND91Assistant
{
    // 获取android设备 修改system/app权限是否成功 
    class GetIsRemountCommand : public BaseCommand
    {
    public:
        GetIsRemountCommand(const DeviceData* pDevice);
        virtual ~GetIsRemountCommand();

        // 执行命令
        virtual void Execute();

        // 生成Android命令数据包
        virtual std::string GetAndroidBodyData();

        // 解析Android返回结果数据包
        virtual void SetAndroidReturnData(std::string data);

		virtual void SetIosReturnFile(std::wstring filename) {;}


        bool GetResult(){return _bRemount;}

    private:
        
		bool _bRemount;
    };
}