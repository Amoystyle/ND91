#pragma once

#include "AdbShellCommand.h"

// 安装程序
#define	ADB_CMD_INSTALL_APP		        "pm install %s %s %s \"%s\""
#define ADB_CMD_INSTALL_PHONE_PATH      "/data/local/tmp/pkg.apk"

namespace ND91Assistant
{

    // 安装APK程序到手机中 （Apk已上传到手机中）
    class AdbInstallAppCommand : public AdbShellCommand
    {
    public:
        AdbInstallAppCommand(wstring sn, wstring strPhonePath, bool bLock=true, bool bRecover=true, bool bOnSDCard=true);

		wstring GetInstallFailError();

    private:

        wstring _strPhonePath;  // 安装包路径
        bool    _bLock;         // 是否加锁(forward-lock the app)
        bool    _bRecover;      // 是否覆盖(reinstall the app, keeping its data)
        bool    _bOnSDCard;     // 安装位置：true=SD卡 false=手机

		void ExecuteAgain(bool sdCardOption);

        // 重载此方法做【错误重试】
        virtual void InternalExecute();

        void GenerateCommand(bool sdCardOption);

        // 解析返回数据
        // 返回值：true 表示成功，false 表示失败
        virtual bool ParseReturnData();

		string _failerr;
    };
}