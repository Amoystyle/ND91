#pragma once

#include "AdbRemoteCommand.h"

namespace ND91Assistant
{
    // Adb Shell �������
    // ͨ��Socketִ��Shell����
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
        string _strCommand;     //���ɵ�����
        bool _success;          //���ؽ���Ƿ�ɹ�
    };
}