#pragma once

#include "AdbShellCommand.h"
#include "Common/CodeOperation.h"

namespace ND91Assistant
{
    // 检查路径是否存在
    class AdbCheckDir : public AdbShellCommand
    {
    public:
        AdbCheckDir(const wstring sn, const wstring strPhonePath)
        {
			_sn = CCodeOperation::UnicodeToUTF_8(sn);
			_strCommand = "ls -l -a \"" + CCodeOperation::UnicodeToUTF_8(strPhonePath) + "\"";
        };

    protected:
        virtual bool ParseReturnData()
        {
            if((int)_returnData.find("No such file or directory") >= 0 
//                || (int)_returnData.find("Permission denied") >= 0 
                || (int)_returnData.find("Not a directory") >= 0)
                _success = false;
            else
                _success = true;
            return _success;
        };
    };
}