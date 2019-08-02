#include "pch_module.h"

#if defined( MODULE_APP )

#include "ApkData.h"

ApkData::ApkData()
{
}

ApkData::~ApkData()
{
}

ND91Assistant::ApkData::ApkData( wstring strApkPath )
{
	wstring strTempPath;
	OPTION->Get(GlobalOption::STR_ANDROID_NPK_TEMP_PATH, strTempPath);

	CFileOperation::UnZipFile(strApkPath, strTempPath);
	wstring strXMLPath = strTempPath + L"AndroidManifest.xml";

	string str = CFileOperation::ReadFile(strXMLPath, false);

	parseData(str);
}

void ApkData::parseData(string strData)
{
	
}
#endif