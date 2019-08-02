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
	return 0; //��ʱ��֧��
}

bool IosInstallerAppOperation::GetAppInfos(vector<AppData*> &vtInfos)
{
	return false; //��ʱ��֧��

	if (!_pDevInfo->_ios_bJailBreaked)
		return false;

	if (IosPublicFunc::GetVerNum(_pDevInfo->_deviceProductVersion) >= 300)
		return false;

	wstring InstallerOnIosFolder = L"";    // installer�����ļ�����IOS�豸��Ŀ¼
	if (IosPublicFunc::GetVerNum(_pDevInfo->_deviceProductVersion) < 113)
		InstallerOnIosFolder = INSTALLER_APP_INFO_FOLDER_LOW;
	else
		InstallerOnIosFolder = INSTALLER_APP_INFO_FOLDER;

    // �ļ������IOS�豸��ȫ·��
	wstring sourceOnIos = InstallerOnIosFolder + INSTALLER_APP_INFO_FILE_NAME;

    // �ļ�����ڱ��ص�ȫ·��
    wstring destinationOnComputer = _pDevData->GetPath()->GetLocalAppPath()
        + INSTALLER_APP_INFO_FILE_NAME;

	bool bRet = false;
	// �ж�installer���ͳ����ļ��Ƿ���ڣ������������ز�����
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
		// ��ȡ��������
		indexStart = wstrMemo.find(strStartKey,indexStart);
		indexEnd = wstrMemo.find(strStartKey,indexStart+1);

		if (indexEnd > indexStart)
			strPakage = wstrMemo.substr(indexStart, indexEnd - 1 - indexStart);
		else
			strPakage = wstrMemo.substr(indexStart);

		indexStart = indexEnd;

		// �������ݲ�Ϊ�����������������
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