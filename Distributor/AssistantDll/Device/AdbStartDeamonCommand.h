#pragma once

#ifndef WIN32
	#include "Common/winPort.h"
#endif
#include "AdbShellCommand.h"
#include "AdbPSCommand.h"
#include "Module/AndroidDefine.h"
// �����ػ�����
#define ADB_CMD_START_DEMON     "am broadcast -a android.intent.action.START_SOCKET_SERVER"
#define ADB_CMD_START_DEMON2     "am startservice -n \"com.nd.assistance/.server.ServerService\""

namespace ND91Assistant
{
    // �����ֻ��ػ�����
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
        // ������������
        // ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
        virtual bool ParseReturnData()
        {            
			AdbPSCommand cmd(_sn);


            Sleep(20);    //ֹͣһ��ʱ���ý�������

            int  retryCount = 0;
            _success = false;
            while( ! _success )
            {
                cmd.Execute();
                _success =  cmd.isProcessExist(ADB_DEMON_PACKAGE_NAME); //ͨ�������б��ж��Ƿ������ɹ�
				if(!_success)
					Sleep(200);
                if (retryCount++ > 3)   // ÿ��ִ������ͺ�ʱԼ100ms��ִ��3������300ms�����Է�����
                    break;
            }

            return _success;
       };
    };

}