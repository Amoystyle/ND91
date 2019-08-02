#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "IosIPAOperationWithDaemon.h"
#include "Module/System/IosPlistChangeCommand.h"
#include "Common/Path.h"
#include "Module/Application/AppManager.h"

IosIPAOperationWithDaemon::IosIPAOperationWithDaemon(const DeviceData* pDevice, ProgressSubject& subject)
:IIosAppOperation(pDevice, subject)
{
}

IosIPAOperationWithDaemon::~IosIPAOperationWithDaemon(void)
{
}

int IosIPAOperationWithDaemon::GetAppCount(APP_OWNER appOwner)
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
	}

	return nCount;
}

bool IosIPAOperationWithDaemon::GetAppInfos(vector<AppData*> &vtInfos)
{
	if (!_pFileStream)
		return false;

	bool bRet = false;
	wstring destinationOnComputer = _pDevData->GetPath()->GetLocalAppPath() + IPA_APP_INFO_FILE_NAME;

	// 判断ipa类型程序文件是否存在，若存在则下载
	if (_pFileStream->Ios_Exists(IPA_APP_INFO_FILE) && 
		_pFileStream->Ios_DownFromPhone(IPA_APP_INFO_FILE, destinationOnComputer) == Transfer_Success)
	{	
		// 将plist文件转换为xml文件并解析
		if (IosPlistConverter::GetXmlFromPlist(destinationOnComputer, destinationOnComputer))
		{
			IosAppXmlHelper::PraseIpaAppInfos(destinationOnComputer, vtInfos, _pFileStream);
			bRet = true;
		}	

        CFileOperation::DeleteFile(destinationOnComputer);
	}

	return bRet;
}

CommandExecuteResult IosIPAOperationWithDaemon::InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo)
{
	if (!_pFileStream || !_pProgressSubject)
		return COMMAND_EXECUTE_FAIL;

	//判断IPA目录是否存在
	bool bHasIpaDir = _pFileStream->Ios_Exists(NDIPAINSTALLPATH);
	if (!bHasIpaDir)
		bHasIpaDir = _pFileStream->Ios_CreateDirectory(NDIPAINSTALLPATH);

	if (!bHasIpaDir)
		return COMMAND_EXECUTE_FAIL;

	//设置权限	
	/*SetAttributeCommand* pSetAttr = new SetAttributeCommand(_pDevData, NDIPAINSTALLPATH, L"777");
	pSetAttr->Execute();
	delete pSetAttr;*/
	DirectoryOperationCommand cmd(_pDevData);
	cmd.ChangeMode(NDIPAINSTALLPATH, "777");

	bool bRet = false;

	_pProgressSubject->SetTaskNumberSum(3);
	_pProgressSubject->SetAllTaskSum(300);

	_pProgressSubject->SetCurrentTaskSum(100);
	_pProgressSubject->SendPrompt(MESSAGE_INSTALL_UPLOAD);
	//上传安装包
	UploadFileCommand uploadCmd(_pDevData);
	uploadCmd.setfilename(strAppFullName, NDIPAINSTALLFULLNAME);
	// 添加进度转换
	//AppProgressObserver pObserver((ProgressSubject*)(_pProgressSubject));
    ProgressObserver* pObserver = NULL;
    if (!_pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_pProgressSubject->_observers[0]);
	uploadCmd.SetParam(exeInfo->OffsetAppInExe,exeInfo->len);
	uploadCmd.Attach(pObserver);
	uploadCmd.SetTaskNumberSum(1);
	uploadCmd.SetAllTaskSum(CFileOperation::GetFileSizeW(strAppFullName));
	uploadCmd.Execute();
	bool success = uploadCmd.GetUploadResult() == Transfer_Success;
	if (!success)//(!_pFileStream->Ios_CopyToPhone(strAppFullName, NDIPAINSTALLFULLNAME))
		return COMMAND_EXECUTE_FAIL;

	_pProgressSubject->SetCurrentTaskSum(100);
	_pProgressSubject->IncreaseTaskNumberComplete();
	_pProgressSubject->SendPrompt(MESSAGE_INSTALL_INSTALL);
	_pProgressSubject->AddTaskProgress(100);
	_pProgressSubject->SendProgress();

    bRet = InstallAppService(APP_TYPE_IPA);
	if (bRet)
	{
		_pProgressSubject->SetCurrentTaskSum(100);
		_pProgressSubject->IncreaseTaskNumberComplete();
        _pProgressSubject->SendProgress();

		_pProgressSubject->SendPrompt(MESSAGE_INSTALL_NOTIFYSB);

		RepairAppIcon();

		_pProgressSubject->AddTaskProgress(100);
		_pProgressSubject->IncreaseTaskNumberComplete();
        _pProgressSubject->SendProgress();
	}
	

	//删除安装包
	_pFileStream->Ios_DeleteFile(NDIPAINSTALLFULLNAME);

    return bRet ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
}

bool IosIPAOperationWithDaemon::UnInstallApp(wstring strIdentifier)
{
	if (!_pFileStream || IosPublicFunc::GetVerNum(_pDevInfo->_deviceProductVersion) < 200)
		return false;

	wstring strIpaListOnPhone = IPA_APP_INFO_FILE;
	wstring strIpaListOnPC = _pDevData->GetPath()->GetLocalAppPath() + IPA_APP_INFO_FILE_NAME;

	// 将plist文件转换为xml文件并解析
	bool bRet = false;
	IosPlistChangeCommand plistCommand(_pDevData, strIpaListOnPhone);
	plistCommand.Execute();
	if (plistCommand.GetIsComplete() &&
		_pFileStream->Ios_Exists(strIpaListOnPhone) &&
		_pFileStream->Ios_DownFromPhone(strIpaListOnPhone, strIpaListOnPC) == Transfer_Success)
	{
		wstring strAppPathOnPhone;
		if (IosAppXmlHelper::DeleteIpaAppInfo(strIpaListOnPC, strIdentifier, strAppPathOnPhone))
		{
			//上传文件
			_pFileStream->Ios_CopyToPhone(strIpaListOnPC, strIpaListOnPhone);	
			FileHelper::Delete(_pDevData, strAppPathOnPhone);			
			bRet = !_pFileStream->Ios_Exists(strAppPathOnPhone);	
		}				

        CFileOperation::DeleteFile(strIpaListOnPC);
	}

	if (bRet)
	{
		if (IosPublicFunc::GetVerNum(_pDevInfo->_deviceProductVersion) >= 300)
			NotifySB(5);
		Sleep(1000);
	}

	return bRet;
}

CommandExecuteResult IosIPAOperationWithDaemon::ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, wstring strVersion, eArchiveMode eMode, wstring strFileOnPc)
{
	if (strIdentifier.length() <= 0 ||
		strFileOnPc.length() <= 0)
		return COMMAND_EXECUTE_FAIL;

	wstring strDocuments = L"\\Documents";
	wstring strLibrary = L"\\Library";
	wstring strTmp = L"\\tmp";

	wstring strAppFolder = _pDevData->GetPath()->GetLocalAppPath() + L"Archive_" + strIdentifier;

	if (CFileOperation::IsDirExist(strAppFolder.c_str()))
		CFileOperation::DeleteDirectory(strAppFolder);
	CFileOperation::MakeLocalDir(strAppFolder);

	wstring strPayloadFolder = strAppFolder + L"\\Payload";
	wstring strContainerFolder = strAppFolder + L"\\Container";
	wstring strAppPhonePathName = CFileOperation::GetFileName(CStrOperation::trimRightW(strAppPhonePath, L"/"));
	CFileOperation::MakeLocalDir(strContainerFolder);
// 	CFileOperation::MakeLocalDir(strContainerFolder + strDocuments);
// 	CFileOperation::MakeLocalDir(strContainerFolder + strLibrary);
// 	CFileOperation::MakeLocalDir(strContainerFolder + strTmp);

	_pProgressSubject->ResetProgressData();
	_pProgressSubject->SetTaskNumberSum(3);
	_pProgressSubject->SetAllTaskSum(300);

	//下载数据
	_pProgressSubject->SetCurrentTaskSum(100);
	_pProgressSubject->SendPrompt(MESSAGE_ARCHIVE_DOWNLOAD);
	// 需要下载的节点
	vector<NodeData*> nodes;
	DirData root(_pDevData);
	NodeData* pDir = FileHelper::GetNode(&root, strAppPhonePath);
	if (!pDir)
		return COMMAND_EXECUTE_FAIL;
	nodes.push_back(pDir);
	// 下载到此目录	
	//AppProgressObserver pObserver((ProgressSubject*)(_pProgressSubject));
    ProgressObserver* pObserver = NULL;
    if (!_pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_pProgressSubject->_observers[0]);
	bool ret = (Transfer_Success == FileHelper::Download(&nodes, strAppFolder, pObserver));

	if (!ret)
		return COMMAND_EXECUTE_FAIL;

	_pProgressSubject->AddTaskProgress(100-_pProgressSubject->GetLastProgress()->_allTaskProgresss);
	_pProgressSubject->SendProgress();
	
	//整理数据
	_pProgressSubject->SetCurrentTaskSum(100);	
	_pProgressSubject->SendPrompt(MESSAGE_ARCHIVE_ARRANGE);
	//::MoveFile((strAppFolder+L"\\"+strAppPhonePathName).c_str(), strPayloadFolder.c_str());
	CFileOperation::MoveFile((strAppFolder+WSTRING_PATH_SEPARATER+strAppPhonePathName), strPayloadFolder);
	vector<wstring> folders;
	CFileOperation::FindAllFolderInDir(strPayloadFolder, folders);
	vector<wstring>::iterator it;
	for (it=folders.begin(); it!=folders.end(); it++)
	{
		wstring strTempFolder = *it;
		if (CStrOperation::endsWithW(strTempFolder, strDocuments) ||
			CStrOperation::endsWithW(strTempFolder, strLibrary) ||
			CStrOperation::endsWithW(strTempFolder, strTmp))
		{
			CFileOperation::CopyDir(strTempFolder, strContainerFolder);
			CFileOperation::DeleteDirectory(strTempFolder);
		}
	}
	folders.clear();

	vector<wstring> files;
	CFileOperation::FindAllFileInDir(strPayloadFolder, files);
	for (it=files.begin(); it!=files.end(); it++)
	{
		wstring strTempFile = *it;
		//::MoveFile(strTempFile.c_str(), (strAppFolder+L"\\"+CFileOperation::GetFileName(strTempFile)).c_str());
		CFileOperation::MoveFile(strTempFile, (strAppFolder+WSTRING_PATH_SEPARATER+CFileOperation::GetFileName(strTempFile)));
	}
	files.clear();

	_pProgressSubject->AddTaskProgress(100-_pProgressSubject->GetLastProgress()->_currentTaskProgress);
	_pProgressSubject->IncreaseTaskNumberComplete();
    _pProgressSubject->SendProgress();

	//打包数据
	_pProgressSubject->SetCurrentTaskSum(100);	
	_pProgressSubject->SendPrompt(MESSAGE_ARCHIVE_PACKAGE);
	ret = CFileOperation::ZipDirectory(strAppFolder, strFileOnPc);

	_pProgressSubject->AddTaskProgress(100-_pProgressSubject->GetLastProgress()->_currentTaskProgress);
	_pProgressSubject->IncreaseTaskNumberComplete();
    _pProgressSubject->SendProgress();

	CFileOperation::DeleteDirectory(strAppFolder);

    return ret ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
}
#endif