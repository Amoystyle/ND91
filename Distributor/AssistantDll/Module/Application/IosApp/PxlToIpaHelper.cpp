#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "Module/Application/IosApp/PxlToIpaHelper.h"
#include "Common/Path.h"

PxlToIpaHelper::PxlToIpaHelper(void)
{
}

PxlToIpaHelper::~PxlToIpaHelper(void)
{
}

bool PxlToIpaHelper::ConverPxlToIpa(wstring strPxlFileName, wstring& strIpaFileName, wstring tempFolder)
{
	if (CFileOperation::IsFileExist(strPxlFileName.c_str()))
	{
        tempFolder += L"temp";
        tempFolder += WSTRING_PATH_SEPARATER;

		wstring strExtractPath = tempFolder + L"pxl2ipa" + CFileOperation::GetGuidW();

		strIpaFileName = strExtractPath + L".ipa";
		if (CFileOperation::IsFileExist(strIpaFileName.c_str()))
			strIpaFileName = CFileOperation::RenameFile(strIpaFileName);

		if (ExtractPxl(strPxlFileName, strExtractPath) &&
			CompressIpa(strExtractPath, strIpaFileName, tempFolder))
        {
			return true;
        }
	}	

	strIpaFileName = L"";
	return false;
}

bool PxlToIpaHelper::ExtractPxl(wstring strPxlFileName, wstring strExtractPath)
{
	if (CFileOperation::IsDirExist(strExtractPath.c_str()))
		CFileOperation::DeleteDirectory(strExtractPath);

	bool bRet = CFileOperation::UnZipFile(strPxlFileName, strExtractPath);

	if (bRet && CFileOperation::IsDirExist(strExtractPath.c_str()))
	{
		vector<wstring> vt;
		CFileOperation::GetMatchFileFullName(strExtractPath, L"info.plist", vt, false);
		if (vt.size() > 0)
		{
			wstring strPlist = vt.at(0);		
			IosPlistConverter::GetXmlFromPlist(strPlist, strPlist);				
		}

		return true;
	}

	CFileOperation::DeleteDirectory(strExtractPath);

	return false;
}

bool PxlToIpaHelper::CompressIpa(wstring strExtractPath, wstring strIpaFileName, wstring tempFolder)
{
	bool bRet = false;
	if (CFileOperation::IsDirExist(strExtractPath.c_str()))
	{
		wstring strIpaTempPath = tempFolder + L"ipatemp" + CFileOperation::GetGuidW();		

		if (CFileOperation::IsDirExist(strIpaTempPath.c_str()))
			CFileOperation::DeleteDirectory(strIpaTempPath);

		//::CreateDirectory(strIpaTempPath.c_str(), NULL);
		CFileOperation::MakeLocalDir(strIpaTempPath);

		wstring strAppFolder = L"";
		CFileOperation::GetMatchDirFullNameByEndName(strExtractPath, L".app", strAppFolder);
		if (CFileOperation::IsDirExist(strAppFolder.c_str()))
		{
			wstring strIpaPlayLoad = strIpaTempPath + WSTRING_PATH_SEPARATER + L"Payload";
			//::CreateDirectory(strIpaPlayLoad.c_str(), NULL);
			CFileOperation::MakeLocalDir(strIpaPlayLoad);
			strIpaPlayLoad += WSTRING_PATH_SEPARATER + CFileOperation::GetFileName(strAppFolder);
			if (CFileOperation::CopyDir(strAppFolder, strIpaPlayLoad))
			{
				wstring strIconFile = strAppFolder + WSTRING_PATH_SEPARATER + L"icon.png";
				wstring strIconFileNew = strIpaTempPath + WSTRING_PATH_SEPARATER + L"iTunesArtwork";
				if (CFileOperation::IsFileExist(strIconFile.c_str()))
					CFileOperation::CopyFile(strIconFile, strIconFileNew, false);					

				if (CFileOperation::IsFileExist(strIpaFileName.c_str()))
                    CFileOperation::DeleteFile(strIpaFileName);

				if (CFileOperation::ZipDirectory(strIpaTempPath, strIpaFileName))
					bRet = true;
			}
		}		

		CFileOperation::DeleteDirectory(strIpaTempPath);
		CFileOperation::DeleteDirectory(strExtractPath);
	}

	return bRet;
}
#endif