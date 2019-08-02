#pragma once
#include <string>
#include <vector>
#include "AssistantDll.h"
using namespace std;
namespace ND91Assistant
{
    // AndroidXMLOperation ������
    typedef struct ApkPackageInfo
    {
        wstring versionCode;
        wstring versionName;
        wstring packageName;
        vector<wstring> packageLable;
        wstring packageIcon;
        wstring installLocation;
        wstring minSdkVersion;
    }ApkPackageInfo;
    class CLASSINDLL_CLASS_DECL AndroidXMLOperation
    {
    public:
        AndroidXMLOperation(void);
        ~AndroidXMLOperation(void);

    public:
        // ��ȡ����Ψһ��ʶ�İ���
        static wstring GetPackageNameFromApk(wstring strApkPath);
        // ��ȡ�������ƣ� vector����Ϊ������
        static ApkPackageInfo GetPackageInfoFromApk(wstring strApkPath);
        static wstring GetChineseAppName(ApkPackageInfo* pInfo);
    private:
        static wstring GetAppNameFromXML(wstring strXMLData);
    };
}