#pragma once

#include "Module/Driver/BaseDriverController.h"

// 获取驱动与设备ID,驱动如果存在则返回驱动ID; 设备如果不存在则新增该设备,否则返回设备ID
namespace ND91Assistant
{   
    class CLASSINDLL_CLASS_DECL GetDriverUrlID : public BaseDriverController
    {
    public:
        // pDriver 当前驱动
        GetDriverUrlID(Driver* pDriver);
        ~GetDriverUrlID();

        // 提交数据
        bool Action();

        // 取得获得的设备ID
        string GetDriverID() { return _driverID;};
        // 取得获得的驱动ID
        string GetDeviceID() { return _deviceID;};

    private:        
        string _driverID;  // 得到的设备ID
        string _deviceID;  // 得到的驱动ID
    };
}