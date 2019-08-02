#pragma once

#include "AdbShellCommand.h"
#include "Common/CodeOperation.h"
// ��װ����
#define	ADB_CMD_UNINSTALL_APP		"pm uninstall %s \"%s\""

namespace ND91Assistant
{
    // ж���ֻ��еĳ���
    class AdbUninstallAppCommand : public AdbShellCommand
    {
    public:
        AdbUninstallAppCommand(wstring sn, wstring strPackage, bool _bKeepData)
        {
            _sn = CCodeOperation::UnicodeToUTF_8(sn);            
            char buffer[512];
            //sprintf_s(buffer, 512, ADB_CMD_UNINSTALL_APP, _bKeepData ? "-k" : "", CCodeOperation::UnicodeToUTF_8(strPackage).c_str());
            sprintf(buffer, ADB_CMD_UNINSTALL_APP, _bKeepData ? "-k" : "", CCodeOperation::UnicodeToUTF_8(strPackage).c_str());
            _strCommand = buffer;
        };

    protected:
        // ������������
        // ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
        virtual bool ParseReturnData()
        {
			if ( _returnData.find("Failure") == string::npos && _returnData.find("Success") == string::npos )
			{
				_returnData = _pBaseSocket->receiveData();
			}

            _success = _returnData.find("Failure") == string::npos;
            return _success;
        };
    };
}