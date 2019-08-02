#pragma once
#include "AdbInstallAppCommand.h"

#include <string>
using namespace std;

#define ANDROID_APP_INSTALL_DIR     L"/data/local/tmp/"         //�ػ��ļ�Ҫ�ϴ����ֻ��е�Ŀ¼
#define ANDROID_APP_INSTALL_PATH    L"/data/local/tmp/pkg.apk"  //�ػ��ϴ��������·��




namespace ND91Assistant
{
    // ADB�������
    // ��װ�����ֻ��� ��������PC�У�
    class AdbInstallCommand
    {
    public:
        AdbInstallCommand(string sn);
        ~AdbInstallCommand();

        //��װAPK
        bool InstallApk(wstring strPCPath, wstring strPackageName = L"");

        //��װNPK
        bool InstallNpk(wstring strPCPath);

        //ж�س���
        bool UnInstallApp(wstring strPackageName, bool bKeepData=true);

    private:
        string  _sn;
    };
}