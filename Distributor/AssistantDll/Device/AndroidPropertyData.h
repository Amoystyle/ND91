#pragma once

#include "Core/DeviceInfo.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

namespace ND91Assistant
{
    // 手机设备信息解析类
    // 用于解析手机返回的数据的解析类
    class AndroidPropertyData
    {
    public:
        AndroidPropertyData(const char* pData, size_t nSize);
        ~AndroidPropertyData();

        //获取属性值
        wstring GetPropertyValue(string strProp);

    private:
        //格式化值
        string FormatValue(string str);

    private:
        map<string, string> _PropList;
    };
}