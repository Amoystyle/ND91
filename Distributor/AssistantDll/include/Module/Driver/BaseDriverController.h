#pragma once

#include "Module/Driver/Driver.h"

// 网络驱动接口基类，提供系列接口所需要的通用方法和变量
namespace ND91Assistant
{   
    class CLASSINDLL_CLASS_DECL BaseDriverController
    {
    public:
        BaseDriverController() {};
        ~BaseDriverController() {};

        // 获取结果
        bool GetResult() {return _result;};

    protected:
        Driver* _pDriver;   // 当前处理的Driver
        bool _result;       // 命令处理结果

        int GetOSData();    // 获得当前OS的信息
        string GetUrlParam(vector<string>* pNames, vector<string>* pValues);    // 打包要发送的数据
    };
}