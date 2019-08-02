#include "stdafx.h"

#include "IosPublicFunc.h"
#include "Common/Path.h"
#include "Device/CFConverter.h"
#include "Common/BaseSocket.h"
#include "Module/Common/IosPList/IosPlistConverter.h"
#include "Module/Common/IosPList/IosPlistHelper.h"

#include "Device/IIosFileSystem.h"
#include "Module/File/DirData.h"

IosPublicFunc::IosPublicFunc(void)
{
}

IosPublicFunc::~IosPublicFunc(void)
{
}

wstring IosPublicFunc::CheckPathByVersion(wstring path, int nProductVer, bool bJailBreaked)
{
	wstring tempPath = L"";
	if (nProductVer >= 113)
	{
		//如果 iPhone 版本号大于等于 1.1.3 ，地址要改为 mobile 下
		tempPath = CStrOperation::ReplaceW(path,L"/root/Media", L"/mobile/Media");
		tempPath = CStrOperation::ReplaceW(path,L"/root/Library", L"/mobile/Library");
	}
	else
	{
		//如果 iPhone 版本号小于 1.1.3 ，地址要改为 root 下
		tempPath = CStrOperation::ReplaceW(path,L"/mobile/Media", L"/root/Media");
		tempPath = CStrOperation::ReplaceW(path,L"/mobile/Library", L"/root/Library");
	}

	if (!bJailBreaked)
		tempPath = CStrOperation::ReplaceW(path,L"/private/var/mobile/Media/", L"/");

	return tempPath;
}

wstring IosPublicFunc::GetTypeValue(IIosFileSystem* pFileStream, wstring strFileOnPhone, wstring strType, bool bNeedChangeToXML)
{
	if (!pFileStream || strFileOnPhone.length() <= 0 || strType.length() <= 0)
		return L"";

    wstring strFileOnPC = Path::GlobalTemp() + L"\\" + strType + L"." + CFileOperation::GetGuidW();

	wstring strResult = L"";
	if (pFileStream->Ios_DownFromPhone(strFileOnPhone, strFileOnPC) == Transfer_Success)
	{
		//if (bNeedChangeToXML)
			//IosPlistConverter::GetXmlFromPlist(strFileOnPC, strFileOnPC);

		string strMemo = CFileOperation::GetFileAllContent(CCodeOperation::WstringToString(strFileOnPC));
        CFileOperation::DeleteFile(strFileOnPC);

		if (bNeedChangeToXML)
			strMemo = CCodeOperation::UTF_8ToGB2312(strMemo);

		wstring wstrMemo = CCodeOperation::GB2312ToUnicode(strMemo);
		//LOG->WriteDebug(L"cxy" + wstrMemo);
		int nStart = wstrMemo.find(L"<key>" + strType + L"</key>");
		if (nStart > 0)
		{
			nStart = wstrMemo.find(L"<string>", nStart);
			if (nStart > 0)
			{
				nStart += wcslen(L"<string>");
				int nEnd = wstrMemo.find(L"</string>", nStart);

				if (nEnd > nStart)
				{
					strResult = wstrMemo.substr(nStart, nEnd - nStart);
					strResult = CStrOperation::trimRightW(strResult, L" ");
					strResult = CStrOperation::trimLeftW(strResult, L" ");
				}
			}
		}
	}

	return strResult;
}

int IosPublicFunc::GetVerNum(wstring strProductVersion)
{
	wstring strVer = CStrOperation::ReplaceW(strProductVersion, L".", L"");
	strVer.resize(3, '0');
  return atoi(CCodeOperation::WstringToString(strVer).c_str());
}

bool IosPublicFunc::SendMessageToService(void* pSocket, void* pCFData)
{
	if (pSocket == NULL || pCFData == NULL)
		return false;

	//转换CFDic To CFPropertyList
	void* pMsgAsXML = CCFConverter::NDCFPropertyListCreateData(NULL, pCFData, kCFPropertyListBinaryFormat_v1_0, 0, NULL);
	if (!pMsgAsXML)
	{
		return false;
	}	

	int nXmlLen = CCFConverter::NDCFDataGetLength(pMsgAsXML);
	int32_t msgSize = htonl(nXmlLen);
	char* szSizeBuffer = new char[sizeof(int32_t)];
  memset(szSizeBuffer, 0, sizeof(int32_t));
	memcpy(szSizeBuffer, &msgSize, sizeof(int32_t));

	bool bRet = false;
#ifdef WIN32	
	CBaseSocket baseSocket((SOCKET)pSocket);
#else
	CBaseSocket baseSocket((intptr_t)pSocket);
#endif	

    char* temp = (char*)CCFConverter::NDCFDataGetBytePtr(pMsgAsXML);
	//发送消息头
	if (baseSocket.sendData(szSizeBuffer, sizeof(int32_t)) &&
		baseSocket.sendData(temp, nXmlLen))
	{
#ifdef _DEBUG
//         wchar_t filePath[128] = {0};
//         wsprintf(filePath, L"D://send%d.xml", nXmlLen);
//         wstring filePathStr = filePath;
//         CFileOperation::WriteBinFile(filePathStr, temp, nXmlLen);  //写入.plist文件
//         IosPlistConverter::GetXmlFromPlist(filePathStr, filePathStr);
#endif
		bRet = true;
	}

	if (szSizeBuffer)
	{
		delete []szSizeBuffer;
		szSizeBuffer = NULL;
	}

	CCFConverter::NDCFRelease(pMsgAsXML);

	return bRet;
}

void* IosPublicFunc::ReadMssageFromService(void* pSocket)
{
	void* pReplay = NULL;

	if (pSocket == NULL)
		return pReplay;

#ifdef WIN32	
	CBaseSocket baseSocket((SOCKET)pSocket);
#else
	CBaseSocket baseSocket((intptr_t)pSocket);
#endif	

	char buffer[4] = {0};
	if (!baseSocket.receiveDataLength(buffer, 4))
		return pReplay;

	int32_t ulRecSize = 0;
	memcpy(&ulRecSize, buffer, 4);
	ulRecSize = ntohl(ulRecSize);
	if (ulRecSize <= 0)
		return pReplay;

	bool bRet = false;
	char* szRec = new char[ulRecSize+1];
    memset(szRec, 0, ulRecSize+1);
	if (baseSocket.receiveDataLength(szRec, ulRecSize))
	{
        LOG->WriteDebug(L"ReadMssageFromService rec len:"+CStrOperation::IntToWString(ulRecSize));
        /*wstring logPath;
        OPTION->Get(GlobalOption::STR_LOG_FILE_PATH, logPath);
        logPath+=L"Temp";
        CFileOperation::WriteBinFile(logPath, szRec, ulRecSize);
        IosPlistConverter::GetXmlFromPlist(logPath, logPath);
        LOG->WriteDebug(L"ReadMssageFromService rec:"+CCodeOperation::GB2312ToUnicode(CFileOperation::ReadFile(logPath)));
        CFileOperation::DeleteFile(logPath);*/

		void* pXmlData = CCFConverter::NDCFDataCreateWithBytesNoCopy(NULL, (unsigned char*)szRec, (int)ulRecSize, CCFConverter::NDkCFAllocatorNull()); 
        void* pError = NULL;
		pReplay = CCFConverter::NDCFPropertyListCreateFromXMLData(NULL, pXmlData, kCFPropertyListImmutable, &pError);

#ifdef _DEBUG
//         wchar_t filePath[128] = {0};
//         wsprintf(filePath, L"D://receive%d.xml", ulRecSize);
//         wstring filePathStr = filePath;
//         CFileOperation::WriteBinFile(filePathStr, szRec, ulRecSize);  //写入.plist文件
//         IosPlistConverter::GetXmlFromPlist(filePathStr, filePathStr);
#endif
	}

	if (szRec)
	{
		delete []szRec;
		szRec = NULL;
	}

	return pReplay;
    return NULL;
}

wstring IosPublicFunc::RenameFile(const wstring localName, IIosFileSystem* pFileSteam)
{
	if (!pFileSteam)
		return L"";

	if(!pFileSteam->Ios_Exists(localName))
		return localName;

	wstring name = CFileOperation::GetFileName(localName);
	wstring nameNoExt = CFileOperation::GetFileNameNoExt(localName);
	wstring ext = name.substr(nameNoExt.length(), name.length()-nameNoExt.length());
	wstring path = localName.substr(0, localName.length()-ext.length());
	int i=0;
	do 
	{
		wchar_t buffer[10];
        CStrOperation::MyItows(++i, buffer, 10);
		name = path+L"_"+buffer+ext;
	} while( pFileSteam->Ios_Exists(name));

	return name;
}

wstring IosPublicFunc::RenameDir(const wstring localName, IIosFileSystem* pFileSteam)
{
	if (!pFileSteam)
		return L"";

	if(!pFileSteam->Ios_Exists(localName))
		return localName;

	wstring path = localName + L"_";
	wstring newName = localName;
	wchar_t buffer[10];
	int i=0;
	do
	{
        CStrOperation::MyItows(++i, buffer, 10);
		newName = path + buffer;
	} while( pFileSteam->Ios_Exists(newName) );

	return newName;
}

eLocaleType IosPublicFunc::GetCurrentLang(IIosFileSystem* pFileStream, wstring tempFolder)
{
	eLocaleType eType = eLocaleType_EN;	
	wstring strLangName = L"en";

	if (pFileStream)
	{
		wstring strTempFile = tempFolder + L"GlobalPreferences" + CFileOperation::GetGuidW() + L".plist";
        CFileOperation::DeleteFile(strTempFile);
		if (pFileStream->Ios_DownFromPhone(Ios_Global_Preferences_File, strTempFile) == Transfer_Success &&
			IosPlistConverter::GetXmlFromPlist(strTempFile, strTempFile))
		{
			IosPlistHelper xmlHelper(strTempFile, eXmlType_UTF8);
			XML_NODE_INDEX index;
			index.AddChild(0);
			strLangName = xmlHelper.SearchKeyValue(index, "AppleLocale");
		}

        CFileOperation::DeleteFile(strTempFile);
	}

	strLangName = CStrOperation::toLowerW(strLangName);
	if (CStrOperation::startsWith(strLangName, L"zh"))
	{
		if (strLangName.find(L"hans") >= 0)
			eType = eLocaleType_ZH;
		else if (strLangName.find(L"hant") >= 0)
			eType = eLocaleType_TW;
	}

	return eType;
}

bool IosPublicFunc::DeleteFolder(const wstring strFolder, IIosFileSystem* pFileStream)
{
	RETURN_FALSE_IF(!pFileStream);

	RETURN_TRUE_IF(!pFileStream->Ios_Exists(strFolder));
	DirData resultDir(NULL);	
	resultDir._name = CStrOperation::trimRightW(strFolder, L"/") + L"/";
	if (pFileStream->Ios_GetFileInfos(&resultDir, eFileGetType_All))
	{
		deque<NodeData*> childs = resultDir.GetChildrensNoReload();
		for ( size_t i=0; i < childs.size(); ++i )
		{
			bool bRet = true;
			NodeData* pFile = childs[i];
			if (!pFile)
				continue;

			if (pFile->_type == NodeData::DIR)
				bRet = DeleteFolder(pFile->GetFullPath(), pFileStream);
			else if (pFile->_type == NodeData::FILE)
				bRet = pFileStream->Ios_DeleteFile(pFile->GetFullPath());

			RETURN_FALSE_IF(!bRet);
		}
	}

	pFileStream->Ios_DeleteEmptyDirectory(strFolder);
	return !pFileStream->Ios_Exists(strFolder);
}

wstring IosPublicFunc::GetItunesDllRealPath( wstring strRegPath, wstring strDllName )
{
	//Common Files 目录
	wstring strPath = L"";
	wstring strCommonFiles = MyFileOperation::GetRegValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion", L"CommonFilesDir");//32 位系统的 
	wstring strCommonFilesX86 = CStrOperation::ReplaceW(strCommonFiles, L"Program Files\\Common Files", L"Program Files (x86)\\Common Files");//64 位系统的

	//================================================================================================
	// 9.2以后的CoreFoundation.dll 和 iTunesMobileDevice.dll 分别在 Apple\Apple Application Support
	// 和Apple\Mobile Device Support 目录下
	//
	// 9.2之前的CoreFoundation.dll 和 iTunesMobileDevice.dll 都 在Apple\Mobile Device Support\bin目录下
	// 但在Apple\Apple Application Support目录下放了个假的 CoreFoundation.dll
	//
	// 旧的简版iTunes的 CoreFoundation.dll 和 iTunesMobileDevice.dll 都 在Apple\Mobile Device Support目录下
	//
	// 新的简版iTunes与9.2之后的iTunes一致
	//

	wstring appleFiles[2] = {L"Apple\\Apple Application Support", L"Apple\\Mobile Device Support"};

	if ( strRegPath.empty() )
	{
		for (int i=0;i<2;i++)
		{
			wstring tempPath = strCommonFilesX86 + appleFiles[i];
			if (CFileOperation::IsDirExist(tempPath.c_str()))
			{
				wstring tempFile = tempPath + strDllName;
				if ( CFileOperation::IsFileExist( tempFile.c_str() ) )
					strPath = tempPath;		
			}

			if ( strPath.empty() )
			{
				wstring path = CStrOperation::ReplaceW( tempPath, strCommonFilesX86, strCommonFiles );
				if ( CFileOperation::IsFileExist( path.c_str() ) )
					strPath = path;			
			}

			if ( !strPath.empty() )
				return strPath;
		}
	}
	else
	{
		wstring tempFile = strRegPath + strDllName;	
		if ( CFileOperation::IsFileExist(tempFile.c_str()) )
			return strRegPath;
	}

	return L"";
}