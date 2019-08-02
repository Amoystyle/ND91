#pragma once

#include "Module/Driver/BaseDriverController.h"

// 取得驱动下载地址，或者直接下载驱动
namespace ND91Assistant
{   
    class CLASSINDLL_CLASS_DECL GetDriverUrl : public BaseDriverController
    {
    public:
        // deviceID 设备ID
        // driverID 驱动ID
        // 如果是要直接下载，则保存到该地址
        GetDriverUrl(string deviceID, string driverID, wstring strFilePath=L"");
        ~GetDriverUrl();

        // 访问接口,并将结果保存到_strFilePath
        bool Action();
        // 格式化所需要的地址
        string GetUrl();

    private:
        string _driverUrl;      // 驱动地址
        wstring _strFilePath;   // 如果是要直接下载，则保存到该地址

        string _deviceID;       // 传入的设备ID
        string _driverID;       // 传入的驱动ID
    };
}