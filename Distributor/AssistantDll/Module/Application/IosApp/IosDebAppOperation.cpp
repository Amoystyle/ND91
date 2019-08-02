#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "Module/Application/IosApp/IosDebAppOperation.h"
#include "Common/Path.h"
#include "Module/Application/AppManager.h"

IosDebAppOperation::IosDebAppOperation(const DeviceData* pDevice, ProgressSubject& subject)
:IIosAppOperation(pDevice, subject)
{
}

IosDebAppOperation::~IosDebAppOperation(void)
{
}

int IosDebAppOperation::GetAppCount(APP_OWNER appOwner)
{
	vector<AppData*> vtInfos;
	if (!GetAppInfos(vtInfos))
		return 0;

	int nCount = 0;
	vector<AppData*>::iterator it;
	for (it=vtInfos.begin(); it!=vtInfos.end(); it++)
	{
		if ((*it)->_eAppOwner == appOwner || appOwner == APP_OWNER_ANY)
			nCount++;
		delete (*it);
		*it = NULL;
	}

	vtInfos.clear();

	return nCount;
}

bool IosDebAppOperation::GetAppInfos(vector<AppData*> &vtInfos)
{
	if (!_pDevInfo || !_pDevInfo->_ios_bJailBreaked)
		return false;

    // 文件存放在本地的全路径
	wstring destinationOnComputer = _pDevData->GetPath()->GetLocalAppPath()
        + DEB_APP_INFO_FILE_NAME;      

	bool bRet = false;
	// 判断deb类型程序文件是否存在，若存在则下载并解析
	if (_pFileStream->Ios_Exists(DEB_APP_INFO_FILE) && 
		_pFileStream->Ios_DownFromPhone(DEB_APP_INFO_FILE, destinationOnComputer) == Transfer_Success)
	{
		AnalysisDebFile(destinationOnComputer, vtInfos);

		bRet = true;
        CFileOperation::DeleteFile(destinationOnComputer);
	}

	return bRet;
}

CommandExecuteResult IosDebAppOperation::InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo)
{
	if (_pDevData && _pDevData->GetIosDeamonMode() == 0)
		return COMMAND_EXECUTE_FAIL;

	if (!_pDevInfo || !_pProgressSubject || !CFileOperation::IsFileExist(strAppFullName.c_str()) ||
		!_pDevInfo->_ios_bJailBreaked || !_pDevInfo->_ios_daemonInstalled)
		return COMMAND_EXECUTE_FAIL;

	//检测固件是否符合要求
	int nVer = IosPublicFunc::GetVerNum(_pDevInfo->_deviceProductVersion);
	if (nVer < 200)
		THROW_ASSISTANT_EXCEPTION(PRODUCT_VERSION_NOT_AGREE);//return false;

	//判断DEB安装目录是否存在
	if (!_pFileStream->Ios_Exists(NDDEBINSTALLPATH))
		_pFileStream->Ios_CreateDirectory(NDDEBINSTALLPATH);

	bool bRet = false;

	_pProgressSubject->SetTaskNumberSum(3);
	_pProgressSubject->SetAllTaskSum(300);

	_pProgressSubject->SetCurrentTaskSum(100);
	_pProgressSubject->SendPrompt(MESSAGE_INSTALL_UPLOAD);
	//上传安装包
	UploadFileCommand uploadCmd(_pDevData);
	uploadCmd.setfilename(strAppFullName, NDDEBINSTALLFULLNAME);
	// 添加进度转换
	//AppProgressObserver pObserver((ProgressSubject*)(_pProgressSubject));
    ProgressObserver* pObserver = NULL;
    if (!_pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_pProgressSubject->_observers[0]);
	uploadCmd.Attach(pObserver);
	uploadCmd.SetParam(exeInfo->OffsetAppInExe,exeInfo->len);
	uploadCmd.SetTaskNumberSum(1);
	uploadCmd.SetAllTaskSum(CFileOperation::GetFileSizeW(strAppFullName));
	uploadCmd.Execute();
	bool success = uploadCmd.GetUploadResult() == Transfer_Success;
	if (!success)
		return COMMAND_EXECUTE_FAIL;

	_pProgressSubject->SetCurrentTaskSum(100);
	_pProgressSubject->IncreaseTaskNumberComplete();
    _pProgressSubject->SendProgress();

	//安装deb
	_pProgressSubject->SendPrompt(MESSAGE_INSTALL_INSTALL);
	bRet = InstallAppService(APP_TYPE_DEB);	
	_pProgressSubject->AddTaskProgress(100);
	_pProgressSubject->SendProgress();

	if (bRet)
	{
		_pProgressSubject->SetCurrentTaskSum(100);
		_pProgressSubject->IncreaseTaskNumberComplete();
        _pProgressSubject->SendProgress();

		//修复图标
		_pProgressSubject->SendPrompt(MESSAGE_INSTALL_NOTIFYSB);
		RepairAppIcon();

		_pProgressSubject->AddTaskProgress(100);
		_pProgressSubject->IncreaseTaskNumberComplete();
        _pProgressSubject->SendProgress();
	}

	//删除安装包
	_pFileStream->Ios_DeleteFile(NDDEBINSTALLFULLNAME);

    return bRet ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
}

bool IosDebAppOperation::UnInstallApp(wstring strIdentifier)
{
	if (_pDevData && _pDevData->GetIosDeamonMode() == 0)
		return false;

	if (!_pDevInfo ||
		!_pDevInfo->_ios_bJailBreaked || !_pDevInfo->_ios_daemonInstalled)
		return false;

	if (strIdentifier.length() <= 0 || 
		strIdentifier == DAEMON_APP_ID_ONE || 
		strIdentifier == DAEMON_APP_ID_TWO)
		return false;

	bool bRet = false;

	if (UnInstallAppService(APP_TYPE_DEB, strIdentifier))
	{
		RepairAppIcon();

		bRet = true;
	}

	return bRet;
}

CommandExecuteResult IosDebAppOperation::ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, wstring strVersion, eArchiveMode eMode, wstring strFileOnPc)
{
	THROW_ASSISTANT_EXCEPTION(NOT_SUPPORT);
	return COMMAND_EXECUTE_FAIL;
}

void IosDebAppOperation::AnalysisDebFile(wstring destinationOnComputer, vector<AppData*> &vtInfos)
{
	string strMemo = CFileOperation::GetFileAllContent(CCodeOperation::UnicodeToGB2312(destinationOnComputer));
	wstring wstrMemo = CCodeOperation::UTF_8ToUnicode(strMemo);

	wstring strStartKey = L"Package:";
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

		if (strPakage == L"")
			return ;

		// 包体内容不为空则解析并插入容器
		AppData* pAppData = new AppData();
		SetDebAppData(strPakage, pAppData);
		if (CStrOperation::toLowerW(pAppData->_strState) == L"install ok installed" || 
			CStrOperation::toLowerW(pAppData->_strState) == L"install ok half-configured")
		{
			vtInfos.push_back(pAppData);
		}
	}
}

void IosDebAppOperation::SetDebAppData(wstring strXML, AppData* pAppData)
{
	pAppData->_AppName = SearchDebKey(strXML, L"Name:");
	pAppData->_strVersion = SearchDebKey(strXML, L"Version:");
	pAppData->_Description = SearchDebKey(strXML, L"Description:");
	pAppData->_Url = SearchDebKey(strXML, L"Homepage:");
	pAppData->_packageName = SearchDebKey(strXML, L"Package:");
	pAppData->_strAppPhonePath = L"";
	pAppData->_strIconPhonePath = SearchDebKey(strXML, L"Icon:");
	if (pAppData->_strIconPhonePath.length() > 0)
		pAppData->_strIconPhonePath = CStrOperation::ReplaceW(pAppData->_strIconPhonePath, L"file://", L"");
	pAppData->_DebEssential = SearchDebKey(strXML, L"Essential:");
	pAppData->_DebDepends =  SearchDebKey(strXML, L"Depends:");
	pAppData->_eAppType = APP_TYPE_DEB;
	pAppData->_eAppOwner = GetApplicationType(pAppData->_packageName, pAppData->_DebEssential);
	pAppData->_strState = SearchDebKey(strXML, L"Status:");

	if (pAppData->_AppName == L"")
	{
		pAppData->_AppName = pAppData->_packageName;
	}

}

wstring IosDebAppOperation::SearchDebKey(wstring strPakage, wstring strKey)
{
	wstring strResult = L"";
	int indexStart = -1;
	int indexEnd = -1;

	indexStart = CStrOperation::toLowerW(strPakage).find(CStrOperation::toLowerW(strKey));
	if (indexStart >= 0)
	{
		indexEnd = CStrOperation::toLowerW(strPakage).find(10, indexStart + 1);
		if (indexEnd > indexStart)
		{
			strResult = strPakage.substr(indexStart + strKey.length(), indexEnd - indexStart - strKey.length());
			strResult = CStrOperation::trimRightW(strResult, L" ");
			strResult = CStrOperation::trimLeftW(strResult, L" ");
		}
	}
	return strResult;
}

APP_OWNER IosDebAppOperation::GetApplicationType(wstring appIdentifier, wstring appEssential)
{
	APP_OWNER AppType = APP_OWNER_USER;

	/*一般用户破解完后安装cydia会自动安装一些deb包，我们默认把这些软件当成系统的
	用户用来安装的deb软件当成用户，好处是用户可以方便的找到自己后来安装的deb软件，想卸载也很容易。*/

	string arrSystemDebIdentifier[] = {"adv-cmds","apt","base","bash","berkeleydb","bigboss","bzip2","coreutils","cydia","cydia-sources",
		"darwintools","diffutils","dpkg","findutils","gettext","gnupg","grep","gzip","inetutils","ispazio.net",
		"less","libgcc","libutil","modmyifone","nano","ncurses","network-cmds","pcre","readline","saurik",
		"sed","shell-cmds","ste","system-cmds","tar","unzip","yellowsn0w.com","zodttd","firmware","apr-lib",
		"apt7-key","apt7-lib","coreutils-bin","essential","lzma","pam","pam-modules","profile.d"};

	for (int i = 0; i < 48; i++)
	{
		wstring temp(arrSystemDebIdentifier[i].begin(),arrSystemDebIdentifier[i].end());
		if (appIdentifier == temp)
		{
			AppType = APP_OWNER_SYS;
			break;
		} 
	}

	if (CStrOperation::toLowerW(appEssential) == L"yes")
	{
		AppType = APP_OWNER_SYS;
	}

	return AppType;
}
#endif