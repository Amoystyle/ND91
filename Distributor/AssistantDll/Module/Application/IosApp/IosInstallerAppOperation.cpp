#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "IosInstallerAppOperation.h"
#include "Common/Path.h"

IosInstallerAppOperation::IosInstallerAppOperation(const DeviceData* pDevice, ProgressSubject& subject)
:IIosAppOperation(pDevice, subject)
{
}

IosInstallerAppOperation::~IosInstallerAppOperation(void)
{
}

int IosInstallerAppOperation::GetAppCount(APP_OWNER appOwner)
{
	return 0; //暂时不支持
}

bool IosInstallerAppOperation::GetAppInfos(vector<AppData*> &vtInfos)
{
	return false; //暂时不支持

	if (!_pDevInfo->_ios_bJailBreaked)
		return false;

	if (IosPublicFunc::GetVerNum(_pDevInfo->_deviceProductVersion) >= 300)
		return false;

	wstring InstallerOnIosFolder = L"";    // installer程序文件所在IOS设备的目录
	if (IosPublicFunc::GetVerNum(_pDevInfo->_deviceProductVersion) < 113)
		InstallerOnIosFolder = INSTALLER_APP_INFO_FOLDER_LOW;
	else
		InstallerOnIosFolder = INSTALLER_APP_INFO_FOLDER;

    // 文件存放在IOS设备的全路径
	wstring sourceOnIos = InstallerOnIosFolder + INSTALLER_APP_INFO_FILE_NAME;

    // 文件存放在本地的全路径
    wstring destinationOnComputer = _pDevData->GetPath()->GetLocalAppPath()
        + INSTALLER_APP_INFO_FILE_NAME;

	bool bRet = false;
	// 判断installer类型程序文件是否存在，若存在则下载并解析
	if (_pFileStream->Ios_Exists(sourceOnIos) && 
		_pFileStream->Ios_DownFromPhone(sourceOnIos, destinationOnComputer) == Transfer_Success)
	{
		AnalysisInstallerFile(destinationOnComputer, vtInfos);

		bRet = true;
        CFileOperation::DeleteFile(destinationOnComputer);
	}

	return bRet;
}

CommandExecuteResult IosInstallerAppOperation::InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo)
{
	return COMMAND_EXECUTE_FAIL;
}

bool IosInstallerAppOperation::UnInstallApp(wstring strIdentifier)
{
	return false;
}

CommandExecuteResult IosInstallerAppOperation::ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, wstring strVersion, eArchiveMode eMode, wstring strFileOnPc)
{
	return COMMAND_EXECUTE_FAIL;
}

void IosInstallerAppOperation::AnalysisInstallerFile(wstring destinationOnComputer, vector<AppData*> &vtInfos)
{
	string strMemo = CFileOperation::GetFileAllContent(CCodeOperation::UnicodeToGB2312(destinationOnComputer));
	wstring wstrMemo = CCodeOperation::UTF_8ToUnicode(strMemo);

	wstring strStartKey = L"<key>name</key>";
	wstring strPakage = L"";
	int indexStart = 0;
	int indexEnd = 0;

	indexStart = wstrMemo.find(strStartKey,indexStart);
	while (indexStart >= 0)
	{
		// 截取包体内容
		indexStart = wstrMemo.find(strStartKey,indexStart);
		indexEnd = wstrMemo.find(strStartKey,indexStart+1);

		if (indexEnd > indexStart)
			strPakage = wstrMemo.substr(indexStart, indexEnd - 1 - indexStart);
		else
			strPakage = wstrMemo.substr(indexStart);

		indexStart = indexEnd;

		// 包体内容不为空则解析并插入容器
		if (strPakage != L"")
		{
			AppData* pAppData = new AppData();
			SetInstallerAppData(strPakage, pAppData);
			vtInfos.push_back(pAppData);

		}
	}
}

void IosInstallerAppOperation::SetInstallerAppData(wstring strXML, AppData* pAppData)
{
// 	pAppData->_AppName = IosAppXmlHelper::SearchXmlKey(strXML, L"name");
// 	pAppData->_strVersion = IosAppXmlHelper::SearchXmlKey(strXML, L"version");
// 	pAppData->_Description = IosAppXmlHelper::SearchXmlKey(strXML, L"description");
// 	pAppData->_Url = IosAppXmlHelper::SearchXmlKey(strXML, L"url");
// 	if (pAppData->_Url == L"")
// 	{
// 		pAppData->_Url = IosAppXmlHelper::SearchXmlKey(strXML, L"source");
// 	}
// 	pAppData->_packageName = IosAppXmlHelper::SearchXmlKey(strXML, L"bundleIdentifier");
// 	pAppData->_strAppPhonePath = IosAppXmlHelper::SearchXmlKey(strXML, L"<string>/Applications/");
// 	pAppData->_strIconPhonePath = pAppData->_strAppPhonePath + L"/icon.png";
// 	pAppData->_eAppType = APP_TYPE_INSTALLER;
}
#endif