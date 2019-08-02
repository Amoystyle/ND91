#pragma once
#include "AdbInstallAppCommand.h"

#include <string>
using namespace std;

#define ANDROID_APP_INSTALL_DIR     L"/data/local/tmp/"         //守护文件要上传到手机中的目录
#define ANDROID_APP_INSTALL_PATH    L"/data/local/tmp/pkg.apk"  //守护上传后的完整路径




namespace ND91Assistant
{
    // ADB程序管理
    // 安装程序到手机中 （程序在PC中）
    class AdbInstallCommand
    {
    public:
        AdbInstallCommand(string sn);
        ~AdbInstallCommand();

        //安装APK
        bool InstallApk(wstring strPCPath, wstring strPackageName = L"");

        //安装NPK
        bool InstallNpk(wstring strPCPath);

        //卸载程序
        bool UnInstallApp(wstring strPackageName, bool bKeepData=true);

    private:
        string  _sn;
    };
}