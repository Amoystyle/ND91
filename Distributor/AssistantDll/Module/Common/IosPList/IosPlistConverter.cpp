#include "stdafx.h"
#include "Module/Common/IosPList/IosPlistConverter.h"

#include "common/plist/PlistOperation.h"
#include "Device/CFConverter.h"


bool IosPlistConverter::GetXmlFromPlist(wstring srcPlistFile, wstring desXmlFile)
{
	 CCFConverter::LoadLib();
    string plistData = CFileOperation::ReadFile(srcPlistFile, true);
    RETURN_FALSE_IF(plistData.empty());

    string xmlData = PlistOperation::ConvertPlist(plistData, false);
    RETURN_FALSE_IF(xmlData.empty());

    return CFileOperation::WriteBinFile(desXmlFile, xmlData.c_str(), xmlData.size());
}

bool IosPlistConverter::GetPlistFromXml(wstring sourceXmlFile, wstring destPlistFile)
{
    string xmlData = CFileOperation::ReadFile(sourceXmlFile, true);
    RETURN_FALSE_IF(xmlData.empty());

    string plistData = PlistOperation::ConvertPlist(xmlData, true);
    RETURN_FALSE_IF(plistData.empty());

    return CFileOperation::WriteBinFile(destPlistFile, plistData.c_str(), plistData.size());
}

