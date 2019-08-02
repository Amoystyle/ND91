#pragma once

#include "AdbShellCommand.h"
#include "Module/system/ProcessData.h"
#include "Common/Stroperation.h"
#include "Common/CodeOperation.h"

#define ADB_CMD_KILL      "kill "
using namespace std;
namespace ND91Assistant
{
    //获取当前进程列表，可用于检查守护进程是否存在，启动守护是否成功
    class AdbKillProgressCommand : public AdbShellCommand
    {
    public:
        AdbKillProgressCommand(const wstring sn, int nPID)
        {
			_sn = CCodeOperation::UnicodeToUTF_8(sn);
            _strCommand = ADB_CMD_KILL + CStrOperation::Int2string(nPID);
        };
        virtual ~AdbKillProgressCommand()
        {
        };
    protected:
        virtual bool ParseReturnData()
        {
            _success = true;
            return true;
        };
    };
}