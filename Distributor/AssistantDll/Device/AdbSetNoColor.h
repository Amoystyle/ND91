#pragma once


#include "AdbShellCommand.h"

namespace ND91Assistant
{
    // ���� ls Ϊ����ʾcolor
    // ͬʱ�õ��Ƿ���ʾcolor�Ľ��
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
        //�Ƿ�֧�� color
        bool _support;  
    };
}