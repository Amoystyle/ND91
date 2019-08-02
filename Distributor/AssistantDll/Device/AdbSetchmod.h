#pragma once


#include "AdbShellCommand.h"
#include "Common/CodeOperation.h"

namespace ND91Assistant
{
    // ����Ŀ¼/�ļ� ����Ȩ��
    class AdbSetchmod : public AdbShellCommand
    {
    public:
        AdbSetchmod(const wstring sn, const wstring strPhoenPath)
        {
            _sn = CCodeOperation::UnicodeToUTF_8(sn);
			_strCommand = "chmod 777 " + CCodeOperation::UnicodeToUTF_8(strPhoenPath);
        };

    protected:
        virtual bool ParseReturnData()
        {
            _success = _returnData.empty();
            return _success;
        };
    private:
    };
}