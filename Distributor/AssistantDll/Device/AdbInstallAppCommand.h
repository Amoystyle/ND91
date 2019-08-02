#pragma once

#include "AdbShellCommand.h"

// ��װ����
#define	ADB_CMD_INSTALL_APP		        "pm install %s %s %s \"%s\""
#define ADB_CMD_INSTALL_PHONE_PATH      "/data/local/tmp/pkg.apk"

namespace ND91Assistant
{

    // ��װAPK�����ֻ��� ��Apk���ϴ����ֻ��У�
    class AdbInstallAppCommand : public AdbShellCommand
    {
    public:
        AdbInstallAppCommand(wstring sn, wstring strPhonePath, bool bLock=true, bool bRecover=true, bool bOnSDCard=true);

		wstring GetInstallFailError();

    private:

        wstring _strPhonePath;  // ��װ��·��
        bool    _bLock;         // �Ƿ����(forward-lock the app)
        bool    _bRecover;      // �Ƿ񸲸�(reinstall the app, keeping its data)
        bool    _bOnSDCard;     // ��װλ�ã�true=SD�� false=�ֻ�

		void ExecuteAgain(bool sdCardOption);

        // ���ش˷��������������ԡ�
        virtual void InternalExecute();

        void GenerateCommand(bool sdCardOption);

        // ������������
        // ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
        virtual bool ParseReturnData();

		string _failerr;
    };
}