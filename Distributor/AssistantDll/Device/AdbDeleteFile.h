#pragma once

#include "AdbShellCommand.h"
#include "Common/CodeOperation.h"

namespace ND91Assistant
{
    //删除手机中的文件
    class AdbDeleteFile : public AdbShellCommand
    {
    public:
        AdbDeleteFile(const wstring sn, const wstring strPhonePath)
        {
            _sn = CCodeOperation::UnicodeToUTF_8(sn);
			_strCommand = "rm \"" + CCodeOperation::UnicodeToUTF_8(strPhonePath) + "\"";
        };

    protected:
        virtual bool ParseReturnData()
        {
            _success = _returnData.empty();
            return _success;
        };
    };
}