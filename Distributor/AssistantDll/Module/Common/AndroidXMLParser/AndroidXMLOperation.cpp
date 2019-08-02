#include "stdafx.h"
#include "Module/Common/AndroidXMLParser/AndroidXMLOperation.h"
#include "common/rapidxml/rapidxml.hpp"
#include "common/Path.h"
#include "Module/Common/AndroidXMLParser/AndroidXmlDecompiler.h"
#include "Module/Common/AndroidXMLParser/AndroidArscDecompiler.h"

using namespace rapidxml;

AndroidXMLOperation::AndroidXMLOperation(void)
{
}

AndroidXMLOperation::~AndroidXMLOperation(void)
{
}

std::wstring ND91Assistant::AndroidXMLOperation::GetAppNameFromXML(wstring strXMLData)
{
    wstring strPackageName = L"pkg";
    xml_document<wchar_t> doc;
    doc.parse<0>(const_cast<wchar_t *>(strXMLData.c_str()));
    xml_node<wchar_t> * root = doc.first_node(L"manifest");
    if (root)
    {
        xml_attribute<wchar_t> * attr = root->first_attribute(L"package"); 
        if (attr)
            strPackageName = attr->value();
    }
    return strPackageName;
}

std::wstring ND91Assistant::AndroidXMLOperation::GetPackageNameFromApk(wstring strApkPath)
{
    wstring strAppName = L"apk";

    wstring strTempFilePath = Path::GlobalTempFile();
    CFileOperation::UnZipFile(strApkPath, strTempFilePath + L"\\", L"AndroidManifest.xml");
    CAndroidXmlDecompiler xml(strTempFilePath + L"\\" + L"AndroidManifest.xml");
    strAppName = GetAppNameFromXML(xml.GetData());

    return strAppName;
}

ApkPackageInfo ND91Assistant::AndroidXMLOperation::GetPackageInfoFromApk( wstring strApkPath )
{
    ApkPackageInfo packageInfo;
	wstring strTempFilePath = Path::GlobalTempFile()+ L"\\"+ CFileOperation::GetGuidW();// CFileOperation::GetFileNameNoExt(strApkPath);
    try
    {
		

		CFileOperation::UnZipFile(strApkPath, strTempFilePath + L"\\", L"AndroidManifest.xml");
        CFileOperation::UnZipFile(strApkPath, strTempFilePath + L"\\", L"resources.arsc");
        CAndroidXmlDecompiler xml(strTempFilePath + L"\\" + L"AndroidManifest.xml");
        xml.Save(strTempFilePath + L"\\AndroidManifest_parsed.xml");
		
		CAndroidArscDecompiler arsc(strTempFilePath + L"\\resources.arsc");
        xml_document<wchar_t> doc;
		
        wstring strXMLData = xml.GetData();

        doc.parse<0>(const_cast<wchar_t *>(strXMLData.c_str()));

        xml_node<wchar_t> * root = doc.first_node(L"manifest");
        wstring strTemp;
        if (root)
        {
            xml_attribute<wchar_t> * attr = root->first_attribute(L"package"); 
            if (attr)
                packageInfo.packageName = attr->value();

            attr = root->first_attribute(L"android:versionCode");
            if (!attr)
                attr = root->first_attribute(L"versionCode");// 解决个别包XML文件内容命名不规范的问题
            if (attr)
                packageInfo.versionCode = attr->value();

            attr = root->first_attribute(L"android:versionName");
            if (!attr)
                attr = root->first_attribute(L"versionName");
            if (attr)
                packageInfo.versionName = attr->value();

			if(CStrOperation::startsWith(packageInfo.versionName,L"@"))
			{
				int nID = arsc.GetID(packageInfo.versionName);
				if (nID != -1)
					packageInfo.versionName = arsc.GetSpecValue(nID);
			}

            attr = root->first_attribute(L"android:installLocation");
            if (!attr)
                attr = root->first_attribute(L"installLocation");
            if (attr)
                packageInfo.installLocation = attr->value();

            xml_node<wchar_t>* sdk = root->first_node(L"uses-sdk");
            if (sdk)
            {
                wstring minSdkVersion;
                attr = sdk->first_attribute(L"android:minSdkVersion");
                if (!attr)
                    attr = sdk->first_attribute(L"minSdkVersion");
                if (attr)
                    minSdkVersion = attr->value();

                if (minSdkVersion == L"1")
                    packageInfo.minSdkVersion = L"Android 1.0";
                else if (minSdkVersion == L"2")
                    packageInfo.minSdkVersion = L"Android 1.1";
                else if (minSdkVersion == L"3")
                    packageInfo.minSdkVersion = L"Android 1.5";
                else if (minSdkVersion == L"4")
                    packageInfo.minSdkVersion = L"Android 1.6";
                else if (minSdkVersion == L"5")
                    packageInfo.minSdkVersion = L"Android 2.0";
                else if (minSdkVersion == L"6")
                    packageInfo.minSdkVersion = L"Android 2.0.1";
                else if (minSdkVersion == L"7")
                    packageInfo.minSdkVersion = L"Android 2.1";
                else if (minSdkVersion == L"8")
                    packageInfo.minSdkVersion = L"Android 2.2";
				else if (minSdkVersion == L"9")
					packageInfo.minSdkVersion = L"Android 2.3";
				else if (minSdkVersion == L"10")
					packageInfo.minSdkVersion = L"Android 2.3.3";
				else if (minSdkVersion == L"11")
					packageInfo.minSdkVersion = L"Android 3.0";
				else if (minSdkVersion == L"12")
					packageInfo.minSdkVersion = L"Android 3.1";
				else if (minSdkVersion == L"13")
					packageInfo.minSdkVersion = L"Android 3.2";
				else if (minSdkVersion == L"14")
					packageInfo.minSdkVersion = L"Android 4.0";
				else if (minSdkVersion == L"15")
					packageInfo.minSdkVersion = L"Android 4.0.3";
				else if (minSdkVersion == L"16")
					packageInfo.minSdkVersion = L"Android 4.1.2";
				else if (minSdkVersion == L"17")
					packageInfo.minSdkVersion = L"Android 4.2";
				else if (minSdkVersion == L"18")
					packageInfo.minSdkVersion = L"Android 4.2.2";
                else
                    packageInfo.minSdkVersion = minSdkVersion;
            }

            xml_node<wchar_t> * application = root->first_node(L"application");
            if (application)
            {
               
                vector<int> idList;

                attr = application->first_attribute(L"android:label");
                if (!attr)
                    attr = application->first_attribute(L"label");
                if (attr)
                {
                    strTemp = attr->value();
                    int nID = arsc.GetID(strTemp);
                    if (nID != -1)
                        packageInfo.packageLable = arsc.GetSpecValues(nID);
                }

                attr = application->first_attribute(L"android:icon");
                if (!attr)
                    attr = application->first_attribute(L"icon");
                if (attr)
                {
                    strTemp = attr->value();
                    int nID = arsc.GetID(strTemp);
                    if (nID != -1)
                        packageInfo.packageIcon = arsc.GetSpecValue(nID);
                }
                if (packageInfo.packageIcon.length() == 0)
                    packageInfo.packageIcon = arsc.GetIconFromStrings();
            }
        }
    }
    catch (...)    
    {
		DWORD ret = GetLastError();
    }
	CFileOperation::DeleteFile(strTempFilePath + L"\\AndroidManifest.xml");
    return packageInfo;
}

wstring ND91Assistant::AndroidXMLOperation::GetChineseAppName(ApkPackageInfo* pInfo)
{
    if (pInfo->packageLable.size() == 0)
        return L"";
    wstring strName = pInfo->packageLable.at(0);
    for (vector<wstring>::iterator it = pInfo->packageLable.begin(); it != pInfo->packageLable.end(); it++)
    {
        if (CStrOperation::IsChineseString(*it))
        {
            strName = *it;
            break;            
        }
    }
    return strName;
}