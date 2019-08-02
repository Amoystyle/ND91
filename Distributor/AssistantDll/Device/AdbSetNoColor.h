#pragma once


#include "AdbShellCommand.h"

namespace ND91Assistant
{
    // 设置 ls 为不显示color
    // 同时得到是否显示color的结果
    class AdbSetNocolor : public AdbShellCommand
    {
    public:
        AdbSetNocolor(const wstring sn)
        {
            _sn = CCodeOperation::UnicodeToUTF_8(sn);
            _strCommand = "ls --color=never";
        };

        bool Support(){return _support;};
    protected:
        virtual bool ParseReturnData()
        {
            if (_returnData.find("No such file or directory") >= 0
                || _returnData.find("Permission denied") >= 0
                || _returnData.find("unrecognized option") >= 0
                || _returnData.find("Unknown option") >= 0)
            {
                _support = false;
            }
            else
            {
                _support = true;
            }
            _success = _support;
            return _success;
        };
    private:
        //是否支持 color
        bool _support;  
    };
}