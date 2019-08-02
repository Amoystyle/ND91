#include "pch_module.h"

#ifdef MODULE_APP

#include "Module/Application/IosAppXmlHelper.h"
#include "Common/Path.h"
#include "Module/Common/IosPList/IosPlistHelper.h"
#include "Device/IIosFileSystem.h"

IosAppXmlHelper::IosAppXmlHelper(void)
{
}

IosAppXmlHelper::~IosAppXmlHelper(void)
{
}

wstring IosAppXmlHelper::PxlSearchKeyFromXML(IosPlistHelper* plistHelper, string strKey)
{
	if (!plistHelper || strKey.length() <= 0)
		return L"";

	XML_NODE_INDEX index;
	index.AddChild(0);
//	index.pNext = new XML_NODE_INDEX;

	return plistHelper->SearchKeyValue(index, strKey);
}

wstring IosAppXmlHelper::PxlSearchAppPath(IosPlistHelper* plistHelper)
{
	if (!plistHelper)
		return L"";

	wstring strContent = plistHelper->GetXmlContent();
	int nIndexEnd = strContent.find(L"/Info.plist</string>");
	if (nIndexEnd > 0)
	{
		int nIndexStart = strContent.rfind(L"<string>", nIndexEnd)+strlen("<string>");/*_tcslen(L"<string>");*/
		if (nIndexStart > 0 && nIndexStart < nIndexEnd)
			return strContent.substr(nIndexStart, nIndexEnd-nIndexStart);
	}

	return L"";
}

void InitPxlCmdXmlFile(IosPlistHelper* pXmlHelper, XML_NODE_INDEX &index, wstring strCmd)
{
    if (!pXmlHelper)
        return ;

    pXmlHelper->AddElement(index, XML_ELEMENT(L"dict"));

    //index.pNext = new XML_NODE_INDEX;
    index.AddChild(0);
    pXmlHelper->AddElement(index, XML_ELEMENT(L"key", L"commands"));
    pXmlHelper->AddElement(index, XML_ELEMENT(L"array"));

    // 	index.pNext->pNext = new XML_NODE_INDEX;
    // 	index.pNext->pNext->nIndex = 1;
    index.AddChild(1);
    pXmlHelper->AddElement(index, XML_ELEMENT(L"dict"));

    //index.pNext->pNext->pNext = new XML_NODE_INDEX;
    index.AddChild(0);
    pXmlHelper->AddElement(index, XML_ELEMENT(L"key", L"command"));
    pXmlHelper->AddElement(index, XML_ELEMENT(L"string", strCmd));
    pXmlHelper->AddElement(index, XML_ELEMENT(L"key", L"package"));
}

wstring IosAppXmlHelper::GetPXLInstallCmdFileContent()
{
	IosPlistHelper xmlHelper(XML_PI, XML_DOCTYPE, eXmlType_UTF8, XML_ELEMENT(L"plist", L"", L"version", L"1.0"));

	XML_NODE_INDEX index;
	InitPxlCmdXmlFile(&xmlHelper, index, L"install");

	xmlHelper.AddElement(index, XML_ELEMENT(L"string", L"ndapp.pxl"));
	return xmlHelper.GetXmlContent();
}

wstring IosAppXmlHelper::GetPXLUnstallCmdFileContent(wstring strIdentifier)
{
	IosPlistHelper xmlHelper(XML_PI, XML_DOCTYPE, eXmlType_UTF8, XML_ELEMENT(L"plist", L"", L"version", L"1.0"));

	XML_NODE_INDEX index;
	InitPxlCmdXmlFile(&xmlHelper, index, L"remove");

	xmlHelper.AddElement(index, XML_ELEMENT(L"string", strIdentifier));

	return xmlHelper.GetXmlContent();
}

void IosAppXmlHelper::CreatePxlPlistFile(wstring strLocalAppPath, wstring strAppName, 
										  wstring strPostflightName, wstring strPreremoveName, wstring strVersion)
{
	if (strLocalAppPath.length() <= 0 ||
		strAppName.length() <= 0 ||
		strVersion.length() <= 0)
		return ;

	strLocalAppPath = CStrOperation::trimRightW(strLocalAppPath, WSTRING_PATH_SEPARATER);
	wstring strAppFilePath = strLocalAppPath + WSTRING_PATH_SEPARATER + L"Info.plist";
	wstring strAppDirName = CFileOperation::GetFileName(strLocalAppPath);

	if (!CFileOperation::IsFileExist(strAppFilePath.c_str()) || !IosPlistConverter::GetXmlFromPlist(strAppFilePath, strAppFilePath))
		return ;	

	IosPlistHelper plistHelper(strAppFilePath, eXmlType_UTF8);
	wstring strCFBundleIdentifier = IosAppXmlHelper::PxlSearchKeyFromXML(&plistHelper, "CFBundleIdentifier");
	wstring strDestination = L"/Applications/" +  IosAppXmlHelper::PxlSearchKeyFromXML(&plistHelper, "CFBundleName") + L".app";
	wstring strSource = strAppDirName;
	wstring strRDPxlPackageDesc = L"";
	wstring strRDPxlPackageFormat = L"1";
	wstring strRDPxlPackageName = strAppDirName;
	wstring strRDPxlPackageVersion = IosAppXmlHelper::PxlSearchKeyFromXML(&plistHelper, "CFBundleVersion");
	wstring strRDPxlPackageIcon = strAppDirName + L"/icon.png";
	wstring strCFBundleDisplayName = IosAppXmlHelper::PxlSearchKeyFromXML(&plistHelper, "CFBundleDisplayName");
	wstring strRDPxlPackageFireware = L"";

	IosPlistHelper xmlHelper(XML_PI, L"", eXmlType_UTF8, XML_ELEMENT(L"plist", L"", L"version", L"1.0"));
	XML_NODE_INDEX index;
	xmlHelper.AddElement(index, XML_ELEMENT(XML_DICT));

	//index.pNext = new XML_NODE_INDEX;
	index.AddChild(0);
	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"CFBundleIdentifier"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, strCFBundleIdentifier));

	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"RDPxlPackageFiles"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_ARRAY));

	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"RDPxlPackagePostflight"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_ARRAY));

	if (strPreremoveName.length() > 0)
	{
		xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"RDPxlPackagePreremove"));
		xmlHelper.AddElement(index, XML_ELEMENT(XML_ARRAY));
	}

	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"RDPxlPackageDesc"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, strAppName));

	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"RDPxlPackageFormat"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"1"));

	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"RDPxlPackageName"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, strAppName));

	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"RDPxlPackageVersion"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, strVersion));

	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"RDPxlPackageIcon"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, strRDPxlPackageIcon));

// 	index.pNext->pNext = new XML_NODE_INDEX;
// 	index.pNext->pNext->nIndex = 3;
	index.AddChild(3);
	xmlHelper.AddElement(index, XML_ELEMENT(XML_DICT));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_DICT));

	//index.pNext->pNext->nIndex = 5;
	index.SetChildIndex(1, 5);
	int nCountPostflightName = 0;
	if (strPostflightName.length() > 0)
	{
		nCountPostflightName++;
		xmlHelper.AddElement(index, XML_ELEMENT(XML_ARRAY));		
	}
	xmlHelper.AddElement(index, XML_ELEMENT(XML_ARRAY));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_ARRAY));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_ARRAY));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_ARRAY));

	if (strPreremoveName.length() > 0)
	{
/*		index.pNext->pNext->nIndex = 7;*/
		index.SetChildIndex(1, 7);
		xmlHelper.AddElement(index, XML_ELEMENT(XML_ARRAY));
	}

// 	index.pNext->pNext->pNext = new XML_NODE_INDEX;
// 	index.pNext->pNext->nIndex = 3;
// 	index.pNext->pNext->pNext->nIndex = 0;
	index.SetChildIndex(1, 3);
	index.AddChild(0);
	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"destination"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"/private/var/mobile/Documents"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"overwrite"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_FALSE));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"permanent"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_FALSE));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"source"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"Documents"));

/*	index.pNext->pNext->pNext->nIndex = 1;*/
	index.SetChildIndex(2, 1);
	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"destination"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, strDestination));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"overwrite"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_TRUE));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"permanent"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_FALSE));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_KEY, L"source"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, strSource));

// 	index.pNext->pNext->nIndex = 5;
// 	index.pNext->pNext->pNext->nIndex = 0;
	index.SetChildIndex(1, 5);
	index.SetChildIndex(2, 0);
	if (strPostflightName.length() > 0)
	{
		xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"sh"));
		xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"/private/var/root/Media/PXL/DB/" + strPostflightName));
	}
//	index.pNext->pNext->pNext->nIndex += nCountPostflightName;
	index.SetChildIndex(2, index.GetChildIndex(2) + nCountPostflightName);
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"chown"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"-R"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"mobile"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"/private/var/mobile/Documents"));
//	index.pNext->pNext->pNext->nIndex += 1;
	index.SetChildIndex(2, index.GetChildIndex(2) + 1);
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"chmod"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"-R"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"777"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"/private/var/mobile/Documents"));
//	index.pNext->pNext->pNext->nIndex += 1;
	index.SetChildIndex(2, index.GetChildIndex(2) + 1);
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"chown"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"-R"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"mobile"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, strDestination));
//	index.pNext->pNext->pNext->nIndex += 1;
	index.SetChildIndex(2, index.GetChildIndex(2) + 1);
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"chmod"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"-R"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"775"));
	xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, strDestination));

	if (strPreremoveName.length() > 0)
	{
// 		index.pNext->pNext->nIndex = 7;
// 		index.pNext->pNext->pNext->nIndex = 0;
		index.SetChildIndex(1, 7);
		index.SetChildIndex(2, 0);
		xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"sh"));
		xmlHelper.AddElement(index, XML_ELEMENT(XML_STRING, L"/private/var/root/Media/PXL/DB/" + strPreremoveName));
	}

	wstring strTargetPath = CFileOperation::GetFileDir(strLocalAppPath) + L"PxlPkg.plist";
	xmlHelper.WriteToFile(strTargetPath);
}

bool IosAppXmlHelper::DeleteIpaAppInfo(wstring strXmlFile, wstring strIdentifier, wstring &strAppPathOnPhone)
{
	IosPlistHelper plistHelper(strXmlFile, eXmlType_UTF8);
	XML_NODE_INDEX index;
	//index.pNext = new XML_NODE_INDEX;
	index.AddChild(0);
	rapidxml::xml_node<char> * pUserNode = plistHelper.SearchKeyNode(index, "User");
	if (pUserNode)
	{
		string strIdentifierUtf8 = CCodeOperation::UnicodeToUTF_8(strIdentifier);
		strAppPathOnPhone = IosPlistHelper::SearchKeyFromXmlNode(IosPlistHelper::SearchKeyNodeFromXmlNode(pUserNode, strIdentifierUtf8), 
			"Path");
		plistHelper.DeleteNode(pUserNode, strIdentifierUtf8);
		plistHelper.WriteToFile(strXmlFile);
		return true;
	}
	return false;
}

void IosAppXmlHelper::PraseIpaAppInfos(wstring strXmlFile, vector<AppData*> &vtInfos, IIosFileSystem* pFileStream)
{
	IosPlistHelper plistHelper(strXmlFile, eXmlType_UTF8);
	XML_NODE_INDEX index;
	index.AddChild(0);

	if(pFileStream == NULL)
	{
		AppData* pAppData = new AppData();		
		pAppData->_AppName = plistHelper.SearchKeyValue(index, "CFBundleDisplayName");
		pAppData->_strVersion = plistHelper.SearchKeyValue(index, "CFBundleShortVersionString");
		pAppData->_sdkVer = plistHelper.SearchKeyValue(index, "DTPlatformVersion");
		if (pAppData->_strVersion.empty())
			pAppData->_strVersion = plistHelper.SearchKeyValue(index, "CFBundleVersion");
		pAppData->_Description = L"";
		pAppData->_Url = L"";
		pAppData->_strIconPhonePath =  plistHelper.SearchKeyValue(index, "CFBundleIconFile");
		if(pAppData->_strIconPhonePath.length()==0)
			pAppData->_strIconPhonePath =  plistHelper.SearchKeyValue(index, "Icon files");

		if(pAppData->_strIconPhonePath.length()==0)
		{
			pAppData->_strIconPhonePath =  plistHelper.SearchKeyValue(index, "CFBundleIconFiles");
		}
		rapidxml::xml_node<char>* strDeviceFamilysNode = plistHelper.SearchKeyNode(index, "UIDeviceFamily","array");
		if(strDeviceFamilysNode)
			IosPlistHelper::GetValuesFromNode(strDeviceFamilysNode, pAppData->_DeviceFamily);
// 		wstring  strDeviceFamilys  =  plistHelper.SearchKeyValue(index, "UIDeviceFamily");
// 		if(strDeviceFamilys.length()>0)
// 		{
// 			pAppData->_DeviceFamily.push_back(strDeviceFamilys);
// 		}
		pAppData->_packageName =  plistHelper.SearchKeyValue(index, "CFBundleIdentifier");		
		if (pAppData->_strIconPhonePath.length() == 0)
		{
			pAppData->_strIconPhonePath = L"Icon.png";
		}
		if (pAppData->_AppName == L"")
		{
			pAppData->_AppName =  plistHelper.SearchKeyValue(index, "CFBundleName");
		}
		vtInfos.push_back(pAppData);
		return;
	}

	
	//index.pNext = new XML_NODE_INDEX;
	
	rapidxml::xml_node<char> * pUserNode = plistHelper.SearchKeyNode(index, "User");
	if (pUserNode)
	{
		for (rapidxml::xml_node<char> * datanode = pUserNode->first_node();  datanode != NULL; datanode = datanode->next_sibling())
		{
			AppData* pAppData = new AppData();
			pAppData->_packageName = CCodeOperation::UTF_8ToUnicode(datanode->value());
			datanode = datanode->next_sibling();
			PraseIpaInfoFromXmlNode(datanode, pAppData, pFileStream);
			vtInfos.push_back(pAppData);
		}
	}
}

void IosAppXmlHelper::PraseIpaInfoFromXmlNode(rapidxml::xml_node<char>* node, AppData* pAppData, IIosFileSystem* pFileStream)
{
	pAppData->_AppName = IosPlistHelper::SearchKeyFromXmlNode(node, "CFBundleDisplayName");
    pAppData->_strVersion = IosPlistHelper::SearchKeyFromXmlNode(node, "CFBundleShortVersionString");
    if (pAppData->_strVersion.empty())
	    pAppData->_strVersion = IosPlistHelper::SearchKeyFromXmlNode(node, "CFBundleVersion");
	pAppData->_Description = L"";
	pAppData->_Url = L"";
	pAppData->_strAppPhonePath = IosPlistHelper::SearchKeyFromXmlNode(node, "Path");

	pAppData->_strIconPhonePath = pAppData->_strAppPhonePath + L"/Icon.png";

	if(pFileStream)
	{
		if (!pFileStream->Ios_Exists(pAppData->_strIconPhonePath))
			pAppData->_strIconPhonePath = pAppData->_strAppPhonePath + L"/icon.png";
		if (!pFileStream->Ios_Exists(pAppData->_strIconPhonePath))
			pAppData->_strIconPhonePath = pAppData->_strAppPhonePath + IosPlistHelper::SearchKeyFromXmlNode(node, "CFBundleIconFile");
		if (!pFileStream->Ios_Exists(pAppData->_strIconPhonePath))
			pAppData->_strIconPhonePath = pAppData->_strAppPhonePath + IosPlistHelper::SearchKeyFromXmlNode(node, "Icon files");
		if (!pFileStream->Ios_Exists(pAppData->_strIconPhonePath))
			pAppData->_strIconPhonePath = CStrOperation::trimRightW(pAppData->_strAppPhonePath, L"/") + L"/" + L"iTunesArtwork";
	}
	else
	{
		//本地文件
		pAppData->_strIconPhonePath = IosPlistHelper::SearchKeyFromXmlNode(node, "CFBundleIconFile");
		pAppData->_packageName = IosPlistHelper::SearchKeyFromXmlNode(node, "CFBundleIdentifier");
		if (pAppData->_strIconPhonePath.length() == 0)
		{

			rapidxml::xml_node<char>* strIconNames = IosPlistHelper::SearchKeyNodeFromXmlNode(node, "CFBundleIconFiles");
			if (!strIconNames)
				pAppData->_strIconPhonePath  = IosPlistHelper::SearchKeyFromXmlNode(node, "Icon files");
			else
			{
				vector<wstring> wsValues;
				IosPlistHelper::GetValuesFromNode(strIconNames, wsValues);
				if(! wsValues.empty())
					pAppData->_strIconPhonePath = wsValues.at(0);
			}
		}
		
		if (pAppData->_strIconPhonePath.length() == 0)
		{
			pAppData->_strIconPhonePath = L"Icon.png";
		}
	}

	wstring strMetadataPlistFile = CStrOperation::trimRightW(pAppData->_strAppPhonePath, L"/") + L"/" + IPA_APP_METADATA_FILE;

    // 文件存放在本地的全路径
    wstring strMetadataOnPc = Path::GlobalTemp() + CFileOperation::GetGuidW() + IPA_APP_METADATA_FILE;

	if (pFileStream && pFileStream->Ios_DownFromPhone(strMetadataPlistFile, strMetadataOnPc) == Transfer_Success)
	{		
		// 将plist文件转换为xml文件并解析
		if (IosPlistConverter::GetXmlFromPlist(strMetadataOnPc, strMetadataOnPc))
		{
			pAppData->_strPurchaser = GetPurchaser(strMetadataOnPc);
		}

        CFileOperation::DeleteFile(strMetadataOnPc);
	}

	pAppData->_eAppType = APP_TYPE_IPA;

	pAppData->_eAppOwner = APP_OWNER_SYS;
	if (IosPlistHelper::IsKeyExist(node, "User"))//((int)strXML.find(L"<key>User</key>") >= 0)
		pAppData->_eAppOwner = APP_OWNER_USER;

	pAppData->_bShare = (IosPlistHelper::SearchKeyFromXmlNode(node, "UIFileSharingEnabled") == L"true") ? true : false;

	if (pAppData->_AppName == L"")
	{
		pAppData->_AppName = IosPlistHelper::SearchKeyFromXmlNode(node, "CFBundleName");
	}
}


void IosAppXmlHelper::PrasePxlAppInfo(wstring destinationOnComputer, vector<AppData*> &vtInfos, IIosFileSystem* pFileStream)
{
	IosPlistHelper plistHelper(destinationOnComputer, eXmlType_UTF8, true);

	AppData* pAppData = new AppData();
	pAppData->_AppName = IosAppXmlHelper::PxlSearchKeyFromXML(&plistHelper, "RDPxlPackageName");
	pAppData->_strVersion = IosAppXmlHelper::PxlSearchKeyFromXML(&plistHelper, "RDPxlPackageVersion");
	pAppData->_Description = IosAppXmlHelper::PxlSearchKeyFromXML(&plistHelper, "RDPxlPackageDesc");
	pAppData->_Url = IosAppXmlHelper::PxlSearchKeyFromXML(&plistHelper, "RDPxlPackageURL");
	pAppData->_packageName = IosAppXmlHelper::PxlSearchKeyFromXML(&plistHelper, "CFBundleIdentifier");
	pAppData->_strIconPhonePath = L"/private/var/root/Media/PXL/DB/" + pAppData->_packageName + L".png";
	pAppData->_eAppType = APP_TYPE_PXL;
	pAppData->_eAppOwner = APP_OWNER_USER;
	pAppData->_strAppPhonePath = IosAppXmlHelper::PxlSearchAppPath(&plistHelper);

   
	//本地文件
	if(pFileStream == NULL)
	{
		pAppData->_strIconPhonePath = IosAppXmlHelper::PxlSearchKeyFromXML(&plistHelper, "RDPxlPackageIcon");
		if(pAppData->_strIconPhonePath.length()==0)
			pAppData->_strIconPhonePath=  pAppData->_packageName + L".png";
		vtInfos.push_back(pAppData);
		return;
	}
	else
	{/*
		// pxl程序大小和文档大小只能获取所在文件夹的大小,并不是真正程序大小
		pAppData->_staticDiskUsage = pFileStream->Ios_FileSize(pAppData->_strAppPhonePath);
		if (pAppData->_staticDiskUsage == -1)
			pAppData->_staticDiskUsage = 0;
		pAppData->_dynamicDiskUsage = pAppData->_staticDiskUsage;*/

        DirData pxlDir(NULL);
        pxlDir._name = pAppData->_strAppPhonePath;

        // 获取该目录下的所有文件
        RETURN_IF(! pFileStream->Ios_GetFileInfos(&pxlDir, eFileGetType_All))
        deque<NodeData*> childs = pxlDir.GetChildrens();
        for(size_t i = 0; i < childs.size(); i++)
        {
            NodeData* pNode = childs.at(i);
            pAppData->_staticDiskUsage += pFileStream->Ios_FileSize(pNode->GetFullPath());
        }
	}

    vtInfos.push_back(pAppData);
}

wstring IosAppXmlHelper::GetPurchaser(wstring strXmlFileOnPC)
{
	IosPlistHelper plistHelper(strXmlFileOnPC, eXmlType_UTF8);
	
	XML_NODE_INDEX index;
	//index.pNext = new XML_NODE_INDEX;
	index.AddChild(0);
	wstring rootappid = plistHelper.SearchKeyValue(index, "AppleID");
	if (!rootappid.empty())
		return rootappid;

	rapidxml::xml_node<char> * pUserNode = plistHelper.SearchKeyNode(index, "com.apple.iTunesStore.downloadInfo");
	if (pUserNode)
	{
		rapidxml::xml_node<char> * pInfoNode = plistHelper.SearchKeyNodeFromXmlNode(pUserNode, "accountInfo");
		if (pInfoNode)
			rootappid = plistHelper.SearchKeyFromXmlNode(pInfoNode, "AppleID");
	}

    if (rootappid.empty())
        rootappid = plistHelper.SearchKeyValue(index, "appleId");

	return rootappid;
}

#endif