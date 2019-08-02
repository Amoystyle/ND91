#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "IosPandaAppOperation.h"
#include "Common/Path.h"
#include "Module/Application/AppManager.h"

IosPandaAppOperation::IosPandaAppOperation(const DeviceData* pDevice, ProgressSubject& subject)
:IIosAppOperation(pDevice, subject)
{
}

IosPandaAppOperation::~IosPandaAppOperation(void)
{
}

int IosPandaAppOperation::GetAppCount(APP_OWNER appOwner)
{
	return 0;//暂时不支持
}

bool IosPandaAppOperation::GetAppInfos(vector<AppData*> &vtInfos)
{
	return false; //暂时不支持

	if (!_pDevInfo->_ios_bJailBreaked)
		return false;

	if (IosPublicFunc::GetVerNum(_pDevInfo->_deviceProductVersion) >= 300)
		return false;

	wstring destinationOnComputer = _pDevData->GetPath()->GetLocalAppPath() 
        + PANDA_APP_INFO_FILE_NAME;       // 文件存放在本地的全路径

	bool bRet = false;

	// 判断panda类型程序文件是否存在，若存在则下载并解析
	if (_pFileStream->Ios_Exists(PANDA_APP_INFO_FILE) && 
		_pFileStream->Ios_DownFromPhone(PANDA_APP_INFO_FILE, destinationOnComputer) == Transfer_Success)
	{
		AnalysisPandaFile(destinationOnComputer, vtInfos);

		bRet = true;
        CFileOperation::DeleteFile(destinationOnComputer);
	}

	return bRet;
}

CommandExecuteResult IosPandaAppOperation::InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo)
{
	return COMMAND_EXECUTE_FAIL;
}

bool IosPandaAppOperation::UnInstallApp(wstring strIdentifier)
{
	return false;
}

CommandExecuteResult IosPandaAppOperation::ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, wstring strVersion, eArchiveMode eMode, wstring strFileOnPc)
{
	return COMMAND_EXECUTE_FAIL;
}

void IosPandaAppOperation::AnalysisPandaFile(wstring destinationOnComputer, vector<AppData*> &vtInfos)
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
			SetPandaAppData(strPakage, pAppData);
			if (pAppData->_eAppType = APP_TYPE_PANDA)
			{
				vtInfos.push_back(pAppData);
			}
		}
	}
}

void IosPandaAppOperation::SetPandaAppData(wstring strXML, AppData* pAppData)
{
// 	pAppData->_AppName = IosAppXmlHelper::SearchXmlKey(strXML, L"name");
// 	pAppData->_strVersion = IosAppXmlHelper::SearchXmlKey(strXML, L"version");
// 	pAppData->_Description = IosAppXmlHelper::SearchXmlKey(strXML, L"description");
// 	pAppData->_Url = IosAppXmlHelper::SearchXmlKey(strXML, L"file");
// 	pAppData->_packageName = IosAppXmlHelper::SearchXmlKey(strXML, L"identifier");
// 	pAppData->_strAppPhonePath = IosAppXmlHelper::SearchXmlKey(strXML, L"localpath");
// 	pAppData->_strIconPhonePath = pAppData->_strAppPhonePath + L"/icon.png";
// 	wstring softType = IosAppXmlHelper::SearchXmlKey(strXML, L"installtype");
// 	if (CStrOperation::toLowerW(softType) == L"ndinstaller")
// 	{
// 		pAppData->_eAppType = APP_TYPE_PANDA;
// 	}
}
#endif