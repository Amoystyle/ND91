#pragma once
#include <string>
#include <vector>
#include "AssistantDll.h"
using namespace std;
namespace ND91Assistant
{
    // AndroidXMLOperation 分析类
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
        // 获取程序唯一标识的包名
        static wstring GetPackageNameFromApk(wstring strApkPath);
        // 获取程序名称， vector是因为多语言
        static ApkPackageInfo GetPackageInfoFromApk(wstring strApkPath);
        static wstring GetChineseAppName(ApkPackageInfo* pInfo);
    private:
        static wstring GetAppNameFromXML(wstring strXMLData);
    };
}