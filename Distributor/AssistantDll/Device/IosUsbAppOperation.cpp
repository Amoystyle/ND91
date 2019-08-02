#include "IosUsbAppOperation.h"
#include "CFConverter.h"
#include "ITunesMobileDevice.h"
#include "../Common/StrOperation.h"
#include "../Common/CodeOperation.h"
#include "../Common/FileOperation.h"
#include "AFCFileSysemBase.h"
#include "../Core/GlobalOption.h"
#include "IosUsbConnection.h"

using namespace ND91Assistant;

IosUsbAppOperation* IosUsbAppOperation::_pInstance = NULL;

IosUsbAppOperation* IosUsbAppOperation::GetInstance()
{
	if (_pInstance)
		return _pInstance;

	_pInstance = new IosUsbAppOperation();

	return _pInstance;
}

IosUsbAppOperation::IosUsbAppOperation()
{
	_progressSubject = &NullProgressSubject::Instance();
}

IosUsbAppOperation::~IosUsbAppOperation(void)
{
}

bool IosUsbAppOperation::GetAppInfos(void* pDevPtr, eApplicationType eType, vector<AppData*> &vtInfos)
{
	bool bRet = false;
	vtInfos.clear();

	map<string, void*> mapInfos;

	if (GetAppInfos(pDevPtr, eType, mapInfos))
	{
		PraseAppInfos(mapInfos, vtInfos);
		bRet = true;
	}

	mapInfos.clear();

	return bRet;
}

bool IosUsbAppOperation::ArchiveApp(void* pDevPtr, void* pAfcHandleApp, wstring strIdentifier, eArchiveMode eMode, wstring strFileOnPc, ProgressSubject& subject)
{
	_progressSubject = &subject;

	if (!pDevPtr || !pAfcHandleApp ||
		strIdentifier.length() <= 0 ||
		strFileOnPc.length() <= 0)
		return false;

	AFCFileSysemBase fileStream(pAfcHandleApp, false);
	wstring strAppArchivesFileOnPhone = L"ApplicationArchives/" + strIdentifier + L".zip";

	_progressSubject->ResetProgressData();
	_progressSubject->SetCurrentTaskSum(100);
	_progressSubject->SetTaskNumberSum(4);
	_progressSubject->SetAllTaskSum(4*100);
	
	//先前备份过了.先删除
	_progressSubject->SendPrompt(MESSAGE_REMOVE_ARCHIVE_APP);
	if (fileStream.Ios_Exists(strAppArchivesFileOnPhone))
		RemoveAppArchiveOnPhone(pDevPtr, strIdentifier);

	_progressSubject->SetCurrentTaskSum(100);
	_progressSubject->IncreaseTaskNumberComplete();

	//备份到设备上
	_progressSubject->SendPrompt(MESSAGE_ARCHIVE_APP_ONPHONE);
	if (!ArchiveAppOnPhone(pDevPtr, strIdentifier, eMode))
		return false;

	_progressSubject->SetCurrentTaskSum(100);
	_progressSubject->IncreaseTaskNumberComplete();

	//从设备拷贝至PC
	_progressSubject->SendPrompt(MESSAGE_DOWNLOAD_APP);
	if (!fileStream.Ios_DownFromPhone(strAppArchivesFileOnPhone, strFileOnPc, subject))
		return false;

	_progressSubject->SetCurrentTaskSum(100);
	_progressSubject->IncreaseTaskNumberComplete();

	//备份完后删除
	_progressSubject->SendPrompt(MESSAGE_REMOVE_ARCHIVE_APP);
	if (fileStream.Ios_Exists(strAppArchivesFileOnPhone))
		RemoveAppArchiveOnPhone(pDevPtr, strIdentifier);

	_progressSubject->Complete();

	return true;
}

bool IosUsbAppOperation::UnInstallApp(void* pDevPtr, wstring strIdentifier)
{
	bool bRet = false;

	if (strIdentifier.length() <= 0)
		return false;

	void* pInstallSocket = NULL;
	DeviceInfo devInfo;
	devInfo._ios_devptr = pDevPtr;
	IosUsbConnection conn(&devInfo);
	if (!conn.StartInstallService(&pInstallSocket))
		return false;

	if (0 == CITunesMobileDevice::iTunes_AMDeviceUninstallApplication(pInstallSocket, 
		CCFConverter::AnsiStringtoCFString(CCodeOperation::UnicodeToGB2312(strIdentifier)), NULL, NULL, NULL))
		bRet = true;

	CITunesMobileDevice::CloseSocket(pInstallSocket);

	return bRet;
}

bool IosUsbAppOperation::InstallApp(void* pDevPtr, void* pAfcSocketApp, void* pAfcHandleApp, wstring strAppFullName, ProgressSubject& subject)
{
	_progressSubject = &subject;

	bool bRet = false;

	if (!pAfcSocketApp || 
		strAppFullName.length() <= 0 || 
		!CFileOperation::IsFileExist(strAppFullName.c_str()) ||
		!CStrOperation::endsWithW(CStrOperation::toLowerW(strAppFullName), L".ipa")) //只安装IPA文件
		return false;

	void* pInstallSocket = NULL;
	DeviceInfo devInfo;
	devInfo._ios_devptr = pDevPtr;
	IosUsbConnection conn(&devInfo);
	if (!conn.StartInstallService(&pInstallSocket))
		return false;

	AFCFileSysemBase fileStream(pAfcHandleApp, false);

	//安装文件使用GUID区分，可避免并行安装时的问题
	wstring strTempName = L"foo_" + CCodeOperation::GB2312ToUnicode(CFileOperation::GetGuid()) + L".ipa";
	wstring strAppInstallFileOnPhone = L"/PublicStaging/" + strTempName;
	wstring strAppInstallFileOnPc;
	if (!OPTION->Get(GlobalOption::STR_IOS_TMP_FILE_FOLDER, strAppInstallFileOnPc))
		strAppInstallFileOnPc = CCodeOperation::GB2312ToUnicode(CFileOperation::GetTempFolder());
	strAppInstallFileOnPc += L"\\" + strTempName;
	::CopyFile(strAppFullName.c_str(), strAppInstallFileOnPc.c_str(), false);

	_progressSubject->ResetProgressData();
	_progressSubject->SetCurrentTaskSum(100);
	_progressSubject->SetTaskNumberSum(2);
	_progressSubject->SetAllTaskSum(2*100);

	_progressSubject->SendPrompt(MESSAGE_UPLOAD_APP);
	//使用ITUNES默认APP上传函数，这个函数会让iTunes_AMDeviceInstallApplication函数记住上传后的文件在设备上的位置
	if (0 == CITunesMobileDevice::iTunes_AMDeviceTransferApplication(pAfcSocketApp, 
		CCFConverter::AnsiStringtoCFString(CCodeOperation::UnicodeToGB2312(strAppInstallFileOnPc)), NULL, OnPackDataTransferCallBack, NULL))
	{
		_progressSubject->SetCurrentTaskSum(100);
		_progressSubject->IncreaseTaskNumberComplete();

		//安装文件只需传入本地即可
		_progressSubject->SendPrompt(MESSAGE_INSTALL_APP);
		if (0 == CITunesMobileDevice::iTunes_AMDeviceInstallApplication(pInstallSocket, 
			CCFConverter::AnsiStringtoCFString(CCodeOperation::UnicodeToGB2312(strAppInstallFileOnPc)), NULL, OnPackInstallCallBack, NULL))
			bRet = true;
	}

	if (!bRet)
	{		
		_progressSubject->ResetProgressData();
		_progressSubject->SetCurrentTaskSum(100);
		_progressSubject->SetTaskNumberSum(2);
		_progressSubject->SetAllTaskSum(2*100);

		_progressSubject->SendPrompt(MESSAGE_UPLOAD_APP);
		//用此方法上传的IPA有一定几率无法安装
		if (fileStream.Ios_CopyToPhone(strAppFullName, strAppInstallFileOnPhone, subject))
		{
			_progressSubject->SetCurrentTaskSum(100);
			_progressSubject->IncreaseTaskNumberComplete();

			//安装文件需传入设备上的位置
			_progressSubject->SendPrompt(MESSAGE_INSTALL_APP);
			if (0 == CITunesMobileDevice::iTunes_AMDeviceInstallApplication(pInstallSocket, 
				CCFConverter::AnsiStringtoCFString(CCodeOperation::UnicodeToGB2312(strAppInstallFileOnPhone)), NULL, OnPackInstallCallBack, NULL))
				bRet = true;			
		}
	}


	fileStream.Ios_DeleteFile(strAppInstallFileOnPhone);
	::DeleteFile(strAppInstallFileOnPc.c_str());
	CITunesMobileDevice::CloseSocket(pInstallSocket);

	_progressSubject->Complete();

	return bRet;
}

bool IosUsbAppOperation::GetAppInfos(void* pDevPtr, eApplicationType eType, map<std::string, void*> &mapInfos)
{
	if (!pDevPtr)
		return false;

	void* pKey = CCFConverter::AnsiStringtoCFString("ApplicationType");
	void* pValue = NULL;
	switch (eType)
	{
	case eApplicationType_User:
		pValue = CCFConverter::AnsiStringtoCFString("User");
		break;
	case eApplicationType_System:
		pValue = CCFConverter::AnsiStringtoCFString("System");
		break;
	case eApplicationType_Internal:
		pValue = CCFConverter::AnsiStringtoCFString("Internal");
		break;
	default: //默认user
		pValue = CCFConverter::AnsiStringtoCFString("User");
		break;
	}

	bool bRet = false;
	void* pDict = CCFConverter::NDCFDictionaryCreate(CCFConverter::NDkCFAllocatorSystemDefault(), &pKey, &pValue, 1, NULL, NULL);
	if (0 == CITunesMobileDevice::iTunes_AMDeviceConnect(pDevPtr))
	{
		if (0 == CITunesMobileDevice::iTunes_AMDeviceStartSession(pDevPtr))
		{
			void* pApps = NULL;
			if (0 == CITunesMobileDevice::iTunes_AMDeviceLookupApplications(pDevPtr, pDict, &pApps))
			{
				bRet = CCFConverter::GetIosData(pApps, mapInfos);
				if (pApps)
					CCFConverter::NDCFRelease(pApps);
			}

			CITunesMobileDevice::iTunes_AMDeviceStopSession(pDevPtr);
		}
		CITunesMobileDevice::iTunes_AMDeviceDisconnect(pDevPtr);
	}

	return bRet;
}

void IosUsbAppOperation::PraseAppInfos(map<std::string, void*> mapInfos, vector<AppData*> &vtInfos)
{
	vtInfos.clear();

	map<std::string, void*>::iterator it; 
	for (it=mapInfos.begin(); it!=mapInfos.end(); it++)
	{
		map<string, string> appInfoNormal;
		if (!CCFConverter::GetIosData(it->second, appInfoNormal))
			continue;

		AppData* pData = new AppData();

		map<string, string>::const_iterator find;

		find = appInfoNormal.find("CFBundleDisplayName");
		if (find !=  appInfoNormal.end())
			pData->_AppName = CCodeOperation::GB2312ToUnicode(find->second);

		if (pData->_AppName.length() <= 0)
		{
			find = appInfoNormal.find("CFBundleName");
			if (find !=  appInfoNormal.end())
				pData->_AppName = CCodeOperation::GB2312ToUnicode(find->second);
		}

		find = appInfoNormal.find("CFBundleVersion");
		if (find !=  appInfoNormal.end())
			pData->_Version = CCodeOperation::GB2312ToUnicode(find->second);

		find = appInfoNormal.find("CFBundleIdentifier");
		if (find !=  appInfoNormal.end())
			pData->_Identifier = CCodeOperation::GB2312ToUnicode(find->second);

		find = appInfoNormal.find("Container");
		if (find !=  appInfoNormal.end())
			pData->_AppPath = CCodeOperation::GB2312ToUnicode(find->second);

		find = appInfoNormal.find("CFBundleIconFile");
		if (find !=  appInfoNormal.end())
		{
			pData->_AppIcon = CStrOperation::trimRightW(pData->_AppPath, L"/") + L"/";

			if (find->second.find(".") < 0)
				pData->_AppIcon += L"Icon.png"; 
			else
				pData->_AppIcon += CCodeOperation::GB2312ToUnicode(find->second);
		}

		pData->_SoftType = eSoftType_IPA;

		pData->_FileSharingEnabled = false;
		map<string, bool> appInfoOther;
		if (CCFConverter::GetIosData(it->second, appInfoOther))
		{
			map<string, bool>::const_iterator findOther;

			findOther = appInfoOther.find("UIFileSharingEnabled");
			if (findOther !=  appInfoOther.end())
				pData->_FileSharingEnabled = findOther->second;
		}

		vtInfos.push_back(pData);
	}
}

bool IosUsbAppOperation::ArchiveAppOnPhone(void* pDevPtr, wstring strIdentifier, eArchiveMode eMode, bool bUnInstall)
{
	bool bRet = false;

	if (!pDevPtr || strIdentifier.length() <= 0)
		return false;

	void* pInstallSocket = NULL;
	DeviceInfo devInfo;
	devInfo._ios_devptr = pDevPtr;
	IosUsbConnection conn(&devInfo);
	if (!conn.StartInstallService(&pInstallSocket))
		return false;

	string strMode = "All";
	switch (eMode)
	{
	case eArchiveMode_ApplicationOnly:
		strMode = "ApplicationOnly";
		break;
	case eArchiveMode_DocumentsOnly:
		strMode = "DocumentsOnly";
		break;
	}

	void* pCFDict = CCFConverter::NDCFDictionaryCreateMutable(NULL, 2, NULL, NULL);
	void* pKey = CCFConverter::AnsiStringtoCFString("ArchiveType");
	void* pVaule = CCFConverter::AnsiStringtoCFString(strMode);
	CCFConverter::NDCFDictionaryAddValue(pCFDict, pKey, pVaule);
	pKey = CCFConverter::AnsiStringtoCFString("SkipUninstall");
	pVaule = CCFConverter::BoolToCFString(bUnInstall?false:true);
	CCFConverter::NDCFDictionaryAddValue(pCFDict, pKey, pVaule);

	if (0 == CITunesMobileDevice::iTunes_AMDeviceArchiveApplication(pInstallSocket, 
		CCFConverter::AnsiStringtoCFString(CCodeOperation::UnicodeToGB2312(strIdentifier)), pCFDict, OnArchiveIPACallback, NULL))
		bRet = true;

	CITunesMobileDevice::CloseSocket(pInstallSocket);

	return bRet;
}

bool IosUsbAppOperation::RemoveAppArchiveOnPhone(void* pDevPtr, wstring strIdentifier)
{
	bool bRet = false;

	if (!pDevPtr || strIdentifier.length() <= 0)
		return false;

	void* pInstallSocket = NULL;
	DeviceInfo devInfo;
	devInfo._ios_devptr = pDevPtr;
	IosUsbConnection conn(&devInfo);
	if (!conn.StartInstallService(&pInstallSocket))
		return false;

	if (0 == CITunesMobileDevice::iTunes_AMDeviceRemoveApplicationArchive(pInstallSocket, 
		CCFConverter::AnsiStringtoCFString(CCodeOperation::UnicodeToGB2312(strIdentifier)), NULL, OnRemoveIPAArchiveCallback, NULL))
		bRet = true;

	CITunesMobileDevice::CloseSocket(pInstallSocket);

	return bRet;
}

void ND91Assistant::OnPackDataTransferCallBack(void* pEvent)
{
	//IosUsbAppOperation::GetInstance()->GetSubject().SendPrompt();
	PraseProgress(pEvent);
}

void ND91Assistant::OnPackInstallCallBack(void* pSender, void* pEvent)
{
	PraseProgress(pSender);
}

void ND91Assistant::OnRemoveIPAArchiveCallback(void* pSender, void* pEvent)
{
	PraseProgress(pSender);
}

void ND91Assistant::OnArchiveIPACallback(void* pSender, void* pEvent)
{
	PraseProgress(pSender);
}

void ND91Assistant::PraseProgress(void* pCFData)
{
	if (!pCFData)
		return ;

	map<string, void*> mapMsg;
	if (CCFConverter::GetIosData(pCFData, mapMsg))
	{
		map<string, void*>::iterator find;

		//判断是否出错了
		find = mapMsg.find("Error");
		if (find!=mapMsg.end())
		{
			//出错处理
			IosUsbAppOperation::GetInstance()->GetSubject().SendPrompt(MESSAGE_ERROR);
			return ;
		}
		else
		{
			const ProgressData* pLast = IosUsbAppOperation::GetInstance()->GetSubject().GetLastProgress();
			int nValue = 0;

			//判断是否完成
			find = mapMsg.find("Status");
			string strStatus = "";
			if (find!=mapMsg.end() && CCFConverter::GetIosData(find->second, strStatus) && strStatus.find("Complete") >= 0)
			{				
				nValue = 100 - pLast->_currentTaskProgress;
			}
			else
			{
				//反馈进度
				find = mapMsg.find("PercentComplete");
				__int32 nPerssent = 0;
				if (find!=mapMsg.end() && CCFConverter::GetIosData(find->second, nPerssent))
				{
					nValue = nPerssent - pLast->_currentTaskProgress;
				}
			}			

			IosUsbAppOperation::GetInstance()->GetSubject().AddTaskProgress(nValue>0?nValue:0);
			IosUsbAppOperation::GetInstance()->GetSubject().SendProgress();
		}
	}
}