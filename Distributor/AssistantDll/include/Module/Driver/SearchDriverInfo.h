#pragma once

#include "Module/Driver/BaseDriverController.h"

namespace ND91Assistant
{   

    // 查询已确定的设备信息
    class CLASSINDLL_CLASS_DECL SearchDriverInfo : public BaseDriverController
    {
    public:
        // hardID 要检测的硬件ID
        SearchDriverInfo(string hardID);
        // 析构函数
        ~SearchDriverInfo();

        // 提交接口
        bool Action();

    private:
        string _hardID; // 要检测的硬件ID
    };
}