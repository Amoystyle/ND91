#pragma once

#include "AdbRemoteCommand.h"

namespace ND91Assistant
{
    // Adb Shell 命令基类
    // 通过Socket执行Shell命令
    class AdbShellCommand : public AdbRemoteCommand
    {
    public:
        AdbShellCommand()
		:AdbRemoteCommand(L"")
        {
            _isShellCommand = true;
            _bSizeInBody = false;
        };

        virtual ~AdbShellCommand() {}

        virtual void InternalExecute()
        {
            _success = ConnectToDevice(_sn);
            if (!_success)
			{
				_bReturnOK = false;
                return;
			}
            CommonExecute(_strCommand);
        };

        bool Success()
        {
            return _success;
        };
    protected:
        string _strCommand;     //生成的命令
        bool _success;          //返回结果是否成功
    };
}