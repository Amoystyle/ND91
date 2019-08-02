#pragma once

#ifndef WIN32
	#include "Common/winPort.h"
#endif
#include "AdbShellCommand.h"
#include "AdbPSCommand.h"
#include "Module/AndroidDefine.h"
// 启动守护程序
#define ADB_CMD_START_DEMON     "am broadcast -a android.intent.action.START_SOCKET_SERVER"
#define ADB_CMD_START_DEMON2     "am startservice -n \"com.nd.assistance/.server.ServerService\""

namespace ND91Assistant
{
    // 启动手机守护进程
    class AdbStartDeamonCommand : public AdbShellCommand
    {
    public:
        AdbStartDeamonCommand(const std::wstring sn, int nType=0)
        {
			_sn = CCodeOperation::UnicodeToUTF_8(sn);
            if (nType == 0)
                _strCommand = ADB_CMD_START_DEMON;
            else
                _strCommand = ADB_CMD_START_DEMON2;
        };

    protected:
        // 解析返回数据
        // 返回值：true 表示成功，false 表示失败
        virtual bool ParseReturnData()
        {            
			AdbPSCommand cmd(_sn);


            Sleep(20);    //停止一段时间让进程启动

            int  retryCount = 0;
            _success = false;
            while( ! _success )
            {
                cmd.Execute();
                _success =  cmd.isProcessExist(ADB_DEMON_PACKAGE_NAME); //通过进程列表判断是否启动成功
				if(!_success)
					Sleep(200);
                if (retryCount++ > 3)   // 每次执行命令就耗时约100ms，执行3次已是300ms，可以放弃了
                    break;
            }

            return _success;
       };
    };

}