#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "Device/AccountAuthorize/Authorizor.h"
#include "Module/Application/AppManager.h"

#include "IosIPAOperationWithItunes.h"
#include "IosIPAItunesManager.h"
#include "IPAPackageHelper.h"

#include "Core/DeviceManager.h"
#include "Device/CFConverter.h"
#include "Device/IosUsbConnection.h"
#include "Device/ITunesMobileDevice.h"
#include "Device/IosUsbSync.h"
#include "Common/Path.h"
#include "Common/plist/PlistOperation.h"

IosIPAOperationWithItunes::IosIPAOperationWithItunes()
:_progressId(0)
{
}

IosIPAOperationWithItunes::~IosIPAOperationWithItunes(void)
{
    if ( _progressId )
        IosIPAItunesManager::GetInstance()->ClearProgress(_progressId);
}

int IosIPAOperationWithItunes::GetAppCount(void* pDevPtr, APP_OWNER appOwner)
{
	map<string, void*> mapInfos;

	int nCount = 0;
	if (GetAppInfos(pDevPtr, appOwner, mapInfos))
	{
		nCount = mapInfos.size();
	}

	CCFConverter::ClearMap(mapInfos);

	return nCount;
}

bool IosIPAOperationWithItunes::GetAppInfos(DeviceInfo* devInfo, APP_OWNER eType, vector<AppData*> &vtInfos)
{
	bool bRet = false;
	vtInfos.clear();

	if (!devInfo)
		return false;

	map<string, void*> mapInfos;
    _pDevInfoPtr = devInfo;
	if (GetAppInfos(devInfo->_ios_devptr, eType, mapInfos, false))
	{
		PraseAppInfos(devInfo, mapInfos, vtInfos);
		bRet = true;
	}

	CCFConverter::ClearMap(mapInfos);

	return bRet;
}

CommandExecuteResult IosIPAOperationWithItunes::ArchiveApp(const DeviceInfo* pDevInfo, wstring strIdentifier, eArchiveMode eMode, wstring strFileOnPc, ProgressSubject& subject)
{
    _progressId = IosIPAItunesManager::GetInstance()->SetProgress(&subject);
    if ( -1 == _progressId )
        return COMMAND_THREAD_OVER_LIMIT;   // 不支持更多的并发操作了

    _progressSubject = &subject;

	if (!pDevInfo || strIdentifier.length() <= 0 || strFileOnPc.length() <= 0)
        return COMMAND_EXECUTE_FAIL;

	AFCFileSysemBase fileStream(pDevInfo->_ios_afcHandleApp, pDevInfo);
	wstring strAppArchivesFileOnPhone = L"ApplicationArchives/" + strIdentifier + L".zip";

	_progressSubject->ResetProgressData();
	_progressSubject->SetTaskNumberSum(4);
	_progressSubject->SetAllTaskSum(400);

	//先前备份过了.先删除
	_progressSubject->SetCurrentTaskSum(100);
	_progressSubject->SendPrompt(MESSAGE_ARCHIVE_REMOVE);
	if (fileStream.Ios_Exists(strAppArchivesFileOnPhone))
		RemoveAppArchiveOnPhone(pDevInfo, strIdentifier);

	_progressSubject->AddTaskProgress(100-_progressSubject->GetLastProgress()->_currentTaskProgress);
	_progressSubject->IncreaseTaskNumberComplete();
    _progressSubject->SendProgress();

	//备份到设备上
	_progressSubject->SetCurrentTaskSum(100);
	_progressSubject->SendPrompt(MESSAGE_ARCHIVE_APP_ONPHONE);
	if (!ArchiveAppOnPhone(pDevInfo, strIdentifier, eMode))
		THROW_ASSISTANT_EXCEPTION(ARCHIVE_APP_ARCHIVE_ON_DEVICE_ERROR);//return false;

	_progressSubject->AddTaskProgress(100-_progressSubject->GetLastProgress()->_currentTaskProgress);
	_progressSubject->IncreaseTaskNumberComplete();
    _progressSubject->SendProgress();

	//从设备拷贝至PC
	_progressSubject->SetCurrentTaskSum(100);
	_progressSubject->SendPrompt(MESSAGE_ARCHIVE_DOWNLOAD);
	//AppProgressObserver pObserver((ProgressSubject*)(&subject));
    ProgressObserver* pObserver = NULL;
    if (!subject._observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(subject._observers[0]);
	ProgressSubject progressSubjec;
	progressSubjec.Attach(pObserver);
	progressSubjec.SetTaskNumberSum(1);
	progressSubjec.SetAllTaskSum(fileStream.Ios_FileSize(strAppArchivesFileOnPhone));
	if (fileStream.Ios_DownFromPhone(strAppArchivesFileOnPhone, strFileOnPc, progressSubjec) != Transfer_Success)
		THROW_ASSISTANT_EXCEPTION(ARCHIVE_APP_COPY_TO_PC_ERROR);

	_progressSubject->AddTaskProgress(100-_progressSubject->GetLastProgress()->_currentTaskProgress);
	_progressSubject->IncreaseTaskNumberComplete();
    _progressSubject->SendProgress();

	//备份完后删除
	_progressSubject->SetCurrentTaskSum(100);
	_progressSubject->SendPrompt(MESSAGE_ARCHIVE_REMOVE);
	if (fileStream.Ios_Exists(strAppArchivesFileOnPhone))
		RemoveAppArchiveOnPhone(pDevInfo, strIdentifier);

	_progressSubject->AddTaskProgress(100-_progressSubject->GetLastProgress()->_currentTaskProgress);
	_progressSubject->IncreaseTaskNumberComplete();
    _progressSubject->SendProgress();

	return COMMAND_EXECUTE_SUCCESS;
}

bool IosIPAOperationWithItunes::UnInstallApp(const DeviceInfo* pDevInfo, wstring strIdentifier)
{
    SetCSLock(((DeviceInfo*)pDevInfo)->_csInstallationProxyLock);

	if (strIdentifier.length() <= 0)
		return false;

    IosUsbConnection conn((DeviceInfo*)pDevInfo);
    if (!conn.StartInstallationProxyService())
        return false;

    bool bRet = false;
	if (0 == CITunesMobileDevice::iTunes_AMDeviceUninstallApplication(pDevInfo->_ios_pInstallationProxySocket, 
		CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(strIdentifier)), NULL, NULL, NULL))
		bRet = true;

    conn.StopInstallationProxyService();

	return bRet;
}

int IosIPAOperationWithItunes::PreparatoryCheckAppInstall(AppData* pApp)
{
    if (pApp == NULL)
        THROW_ASSISTANT_EXCEPTION(INSTALAPPFILELOST);

    // ipad软件不适合安装到iphone设备
    if (!pApp->_DeviceFamily.empty())
    {
        if (find(pApp->_DeviceFamily.begin(),pApp->_DeviceFamily.end(), L"1") == pApp->_DeviceFamily.end()
            && (_pDevInfoPtr->IsPhone() || _pDevInfoPtr->IsPod()))
            THROW_ASSISTANT_EXCEPTION(DeviceUnSupported);//return -402653056;//kAMDBundleVerificationFailedError
    }
	LOG->WriteDebug(L"InstallApp IosIPAOperationWithItunes DeviceFamily match " +  pApp->_AppName);

    // 支持的固件
    if (!pApp->_minimumOSVersion.empty())
    {
        if (pApp->_minimumOSVersion.compare(_pDevInfoPtr->_deviceProductVersion) > 0)
            THROW_ASSISTANT_EXCEPTION(PRODUCT_VERSION_NOT_AGREE);//return -402653124;//此软件要求固件{0}以上，不适合当前固件{1}
    }

	LOG->WriteDebug(L"InstallApp IosIPAOperationWithItunes os Version match " +  pApp->_AppName);
    return 0;
}

int IosIPAOperationWithItunes::CheckValideToAuthorize(int iValideToAuthorize)
{
    switch(iValideToAuthorize)
    {
    case -1:
        THROW_ASSISTANT_EXCEPTION(NotValideToAuthorizeByDeviceVersion);break;
    case -2:
        THROW_ASSISTANT_EXCEPTION(NotValideToAuthorizeByNotUSB);break;
    case -3:
        THROW_ASSISTANT_EXCEPTION(NotValideToAuthorizeByItunesVersionTooLow);break;
    case -4:
        THROW_ASSISTANT_EXCEPTION(NotValideToAuthorizeByNotFoundItunes);break;
    case -5:
        THROW_ASSISTANT_EXCEPTION(NotValideToAuthorizeByItunesIsSimple);break;
    }

    return 0;
}

CommandExecuteResult IosIPAOperationWithItunes::InstallAppEx(const DeviceData* pDevice, wstring strAppFullName, 
											 string& errorMsg, ProgressSubject& subject,
											 App2ExeInfo* exeInfo)
{
    _progressId = IosIPAItunesManager::GetInstance()->SetProgress(&subject);
    if ( -1 == _progressId )
        return COMMAND_THREAD_OVER_LIMIT;   // 不支持更多的并发操作了

	_pDevInfoPtr        = pDevice->GetDevInfo();
	_pAfcSocketApp      = pDevice->GetDevInfo()->_ios_afcSocketApp;
	_pAfcHandleApp      = pDevice->GetDevInfo()->_ios_afcHandleApp;
	_progressSubject    = &subject;
	 return OldInstallApplication(strAppFullName, errorMsg,exeInfo->OffsetAppInExe,exeInfo->len)
         ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
}

void IosIPAOperationWithItunes::Upload91IPAInfo2Documents(wstring identifier, wstring acountInfo)
{
    AFCFileSysemBase* pFileStream =  AFCAppFileSystem::StartAPPHouseArrestService(identifier, (DeviceInfo*)_pDevInfoPtr);
    RETURN_IF(NULL == pFileStream);

    wstring iPath = L"/Documents/info.ini";
    if (!acountInfo.empty()
        && !pFileStream->Ios_Exists(iPath))
        pFileStream->OpenWrite(iPath, CCodeOperation::UnicodeToUTF_8(acountInfo));

    pFileStream->Ios_DeleteFile(L"info.ini");
    SAFE_DELETE(pFileStream);
}

CommandExecuteResult IosIPAOperationWithItunes::InstallApp(const DeviceData* pDevice, wstring strAppFullName, string& errorMsg, ProgressSubject& subject)
{
    _progressId = IosIPAItunesManager::GetInstance()->SetProgress(&subject);
    RETURN_VALUE_IF(-1 == _progressId, COMMAND_THREAD_OVER_LIMIT); // 不支持更多的并发操作了

    _pDevInfoPtr        = pDevice->GetDevInfo();
    _pAfcSocketApp      = pDevice->GetDevInfo()->_ios_afcSocketApp;
    _pAfcHandleApp      = pDevice->GetDevInfo()->_ios_afcHandleApp;
    _progressSubject    = &subject;

    wstring tempDir = Path::GlobalTemp()+CFileOperation::GetGuidW()+L"\\";
    AppData* pAppInfo = AppManager::GetAppInfoByFile(strAppFullName, tempDir);// 是否需要释放pAppInfo?
	LOG->WriteDebug(L"InstallApp IosIPAOperationWithItunes GetAppInfoByFile " +  strAppFullName);
    RETURN_VALUE_IF(0 != PreparatoryCheckAppInstall(pAppInfo), COMMAND_EXECUTE_FAIL);

	LOG->WriteDebug(L"InstallApp IosIPAOperationWithItunes PreparatoryCheckAppInstall Done " +  pAppInfo->_AppName);
    _identifier = pAppInfo->_packageName;
    bool res = InstallApplication(strAppFullName, errorMsg);
	LOG->WriteDebug(L"InstallApp InstallApplication Done " +  pAppInfo->_AppName);
    RETURN_VALUE_IF(!res, COMMAND_EXECUTE_FAIL);
    RETURN_VALUE_IF(pAppInfo == NULL, COMMAND_EXECUTE_SUCCESS);
    RETURN_VALUE_IF(!pAppInfo->_is91IPA && !pAppInfo->_isLegalSoft, COMMAND_EXECUTE_SUCCESS);

    _progressSubject->SendPrompt(MESSAGE_INSTALL_AUTHORIZOR);
    Authorizor au(pDevice);    // 授权
    if (pAppInfo->_is91IPA)
    {
		LOG->WriteDebug(L"InstallApp Authorizor begin " +  pAppInfo->_AppName);
        RETURN_VALUE_IF(0 != CheckValideToAuthorize(Authorizor::IsValideToAuthorize(pDevice)), COMMAND_EXECUTE_SUCCESS_AuthorizorFaile);
        bool ret = au.Authorize(pAppInfo->_acountInfo, pAppInfo->_AppName);

		LOG->WriteDebug(L"InstallApp IsArchivedIPA begin " +  pAppInfo->_AppName);
        //上传info.ini文件到Documents目录下
        if (!IPAPackageHelper::IsArchivedIPA(strAppFullName))
            Upload91IPAInfo2Documents(pAppInfo->_packageName, pAppInfo->_acountInfo);

#ifndef ASSISTANT_LITE
        RETURN_VALUE_IF(!ret, COMMAND_EXECUTE_SUCCESS_AuthorizorFaile);
#endif
    }
    else if (pAppInfo->_isLegalSoft)
        au.AttemptFix();

	LOG->WriteDebug(L"InstallApp InstallApp Done " +  pAppInfo->_AppName);
    return COMMAND_EXECUTE_SUCCESS;
}

/*
bool IosIPAOperationWithItunes::InstallApplication(wstring strAppFullName, string& errorMsg)
{
    int ret = -1;
    try
    {
        bool isSecure = false;
        wstring deviceProductType = CStrOperation::toLowerW(_pDevInfoPtr->_deviceProductType);
        //判定优先使用的方法
        if (CFileOperation::GetFileSizeW(strAppFullName) > 1073741824 || //大于1G
            CStrOperation::startsWith(deviceProductType, L"iphone1") ||   //一代iPhone
            CStrOperation::startsWith(deviceProductType, L"iphone2") ||   //三代iPhone
            CStrOperation::startsWith(deviceProductType, L"ipod1")   ||   //一代Touch
            CStrOperation::startsWith(deviceProductType, L"ipod2")   ||   //二代Touch    
            CStrOperation::startsWith(deviceProductType, L"ipod3")        //三代Touch
            )
        {
			LOG->WriteDebug(L"InstallApp ItunesFastInstallApplication begin " +  strAppFullName);
            // 此处优先使用iTunes的快速安装方法
            ret = ItunesFastInstallApplication(strAppFullName);
            isSecure = true;
			
        }
        else
        {
			LOG->WriteDebug(L"InstallApp FastInsallApplication begin " +  strAppFullName);
            // 否则使用我们自己传文件的快速安装方法
            ret = FastInsallApplication(strAppFullName);
            isSecure = false;
        }

        if (ret != 0)
        {

            if (isSecure)
            {
				LOG->WriteDebug(L"InstallApp FastInsallApplication begin " +  strAppFullName);
                // 修改为直接使用快速安装，失败后再调用老的安装方式
                ret = FastInsallApplication(strAppFullName);
            }
            else
            {
				LOG->WriteDebug(L"InstallApp ItunesFastInstallApplication begin " +  strAppFullName);
                // 修改为直接使用快速安装，失败后再调用老的安装方式
                ret = ItunesFastInstallApplication(strAppFullName);
            }

			if (ret == -402620415 || ret == -402620395 || ret == -402636802)//-402636802 设备禁止安装应用.// 未越狱的设备安装破解软件会出现这个错误,直接返回不用重试安装
				THROW_ASSISTANT_EXCEPTION(PRODUCT_PROHIBIT_APPINSTATLL);

            //出错后,用原先旧方式重新安装一遍,以免因为一些ipa解压失败导致安装失败
            if (ret != 0)
                return false;//OldInstallApplication(strAppFullName, errorMsg);
        }
    }
    catch (...)
    { 
		if (ret == -402620415 || ret == -402620395 || ret == -402636802)//-402636802 设备禁止安装应用.// 未越狱的设备安装破解软件会出现这个错误,直接返回不用重试安装
			THROW_ASSISTANT_EXCEPTION(PRODUCT_PROHIBIT_APPINSTATLL);
        return false;//OldInstallApplication(strAppFullName, errorMsg);
    }

    return true;
}
*/

bool IosIPAOperationWithItunes::InstallApplication(wstring strAppFullName, string& errorMsg)
{
    int ret = -1;
    try
    {
        bool isSecure = false;
        bool inSelfUnzipMode = false;//91 自己边解压边上传方法
        wstring deviceProductType = CStrOperation::toLowerW(_pDevInfoPtr->_deviceProductType);

        if (CFileOperation::GetFileSizeW(strAppFullName) < 10485760 && //小于10M比较新的机器用
            !CStrOperation::startsWith(deviceProductType, L"iphone1") &&   //一代iPhone
            !CStrOperation::startsWith(deviceProductType, L"iphone2") &&   //三代iPhone
            !CStrOperation::startsWith(deviceProductType, L"ipod1")   &&   //一代Touch
            !CStrOperation::startsWith(deviceProductType, L"ipod2")   &&   //二代Touch    
            !CStrOperation::startsWith(deviceProductType, L"ipod3")        //三代Touch
            )
        {
            ret = InstallAppInZipMode(strAppFullName, errorMsg);//OLD
        }
        else if (_pDevInfoPtr->_connectType == USB && !IPAPackageHelper::IsArchivedIPA(strAppFullName))
        {
            //InstallAppInUnzipModeWithiTunesInterface 该方法暂时不支持安装有存档的备份.
            //需要另外研究相关参数
            ret = InstallAppInUnzipModeWithiTunesInterface(strAppFullName, errorMsg);//itunesFast
            isSecure = true;
        }
        else
        {
            //否则使用我们自己传文件的快速安装方法
            ret = InstallAppInUnzipMode(strAppFullName, errorMsg);//fast
            inSelfUnzipMode = true;
        }

        if (ret != 0)
        {
            //常见错误不需要重试安装:
            //Note!!! 这几个错误码会随着iTunes版本变化而变化
            //-402636802 设备禁止安装应用.
            // -402653049,IPA包里面的架构配置节点有问题.//kAMDIncorrectArchitectureError

            IPAOperationErrorType errorType = (IPAOperationErrorType)(ret);
            if (inSelfUnzipMode && errorType == ApplicationVerificationFailed)
            {
                // 有些ipa,比如Twitter5.0.2.ipa,助手边解压边上传方法会校验失败.
                // TODO:自己边解压边上传方法待解决.
                if (IPAPackageHelper::CheckPackageCopyRight(strAppFullName))
                    return ( InstallAppInZipMode(strAppFullName, errorMsg) == 0 );
            }

            //未越狱的设备安装破解软件会出现ApplicationVerificationFailed这个错误,直接返回不用重试安装
            if (errorType == IncorrectArchitecture
                || errorType == PackageExtractionFailed
                || errorType == PackageInspectionFailed
                || errorType == ApplicationVerificationFailed
                /*|| errorType == InstallProhibited
                || errorType == APIInternalError*/)
                return false;

            if (ret == -402620415 || ret == -402620395 || ret == -402620393 || ret == -402620391)
                return false;

            if (isSecure)
                ret = InstallAppInUnzipMode(strAppFullName, errorMsg);
            else
                ret = InstallAppInUnzipModeWithiTunesInterface(strAppFullName, errorMsg);
        }
    }
    catch (...)
    {
        return ( InstallAppInZipMode(strAppFullName, errorMsg) == 0 );
    }

    return ( ret == 0 );
}

bool IosIPAOperationWithItunes::OldInstallApplication(wstring strAppFullName, string& errorMsg,unsigned __int64 AppOffsetInExe,unsigned __int64 AppSize)
{
    bool bRet = false;

    AFCFileSysemBase fileStream(_pAfcHandleApp, _pDevInfoPtr);
    fileStream.IsAFCConnectionValid();

    //void* pInstallSocket = NULL;
    IosUsbConnection conn((DeviceInfo*)_pDevInfoPtr);
    if (!conn.StartInstallationProxyService(/*&pInstallSocket*/))
        return false;

    //安装文件使用GUID区分，可避免并行安装时的问题
    wstring strTempName = L"foo_" + CCodeOperation::GB2312ToUnicode(CFileOperation::GetGuid()) + L".ipa";
    wstring strAppInstallFileOnPhone = L"/PublicStaging/" + strTempName;

    // 获取临时目录
    DeviceData* pDevice = DEVICE_MANAGER->FindIosDevice(_pDevInfoPtr->_ios_devptr);
    RETURN_FALSE_IF(!pDevice);
   

    _progressSubject->ResetProgressData();
    _progressSubject->SetTaskNumberSum(2);
    _progressSubject->SetAllTaskSum(2*100);

    _progressSubject->SetCurrentTaskSum(100);
    _progressSubject->SendPrompt(MESSAGE_INSTALL_UPLOAD);

	if(AppOffsetInExe==0)
	{	
		wstring strAppInstallFileOnPc = pDevice->GetPath()->GetLocalIpaPath();
		CFileOperation::MakeLocalDir(strAppInstallFileOnPc);

		strAppInstallFileOnPc += strTempName;
		CFileOperation::CopyFile(strAppFullName, strAppInstallFileOnPc, false);	

		if (!_pAfcSocketApp || 
			strAppFullName.length() <= 0 || 
			!CFileOperation::IsFileExist(strAppFullName.c_str()) ||
			!CStrOperation::endsWithW(CStrOperation::toLowerW(strAppFullName), L".ipa")) //只安装IPA文件
			return false;

		 //使用ITUNES默认APP上传函数，这个函数会让iTunes_AMDeviceInstallApplication函数记住上传后的文件在设备上的位置
        void* cfString = CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(strAppInstallFileOnPc));
        PROGRESS_CALLBACK1 pCallback = IosIPAItunesManager::GetInstance()->GetDataTransferCallBack(_progressId);
		if (0 == CITunesMobileDevice::iTunes_AMDeviceTransferApplication(_pAfcSocketApp, cfString, NULL, pCallback, NULL))
		{
			_progressSubject->IncreaseTaskNumberComplete();
			_progressSubject->SendProgress();

			//安装文件只需传入本地即可
			_progressSubject->SetCurrentTaskSum(100);
			_progressSubject->SendPrompt(MESSAGE_INSTALL_INSTALL);

            clock_t begin, end;
            begin = clock();
            void* cfString = CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(strAppInstallFileOnPc));
            PROGRESS_CALLBACK2 pCallback = IosIPAItunesManager::GetInstance()->GetPackInstallCallBack(_progressId);
			if (0 == CITunesMobileDevice::iTunes_AMDeviceInstallApplication(
                _pDevInfoPtr->_ios_pInstallationProxySocket/*pInstallSocket*/, cfString, NULL, pCallback, NULL))
			{
                end = clock();
                wchar_t temp[128] = {0};
                wsprintf(temp, L"iTunes_AMDeviceInstallApplication use time: %d\n", end-begin);
                OutputDebugStr(temp);

				bRet = true;
				_progressSubject->IncreaseTaskNumberComplete();
				_progressSubject->SendProgress();
			}
		}
		CFileOperation::DeleteFile(strAppInstallFileOnPc);
    }
    int iErr = -1;
    if (!bRet)
    {
        //安装失败的时候,需要判断下afc服务时候down了,
        //未越狱的设备安装破解软件,会导致afcdown掉.
        //RestartAFCServiceIfNeed(conn);
        fileStream.IsAFCConnectionValid();

        _progressSubject->ResetProgressData();
        _progressSubject->SetTaskNumberSum(2);
        _progressSubject->SetAllTaskSum(2*100);

        _progressSubject->SetCurrentTaskSum(100);
        _progressSubject->SendPrompt(MESSAGE_INSTALL_UPLOAD);		
        //用此方法上传的IPA有一定几率无法安装
        // 添加进度转换
        ProgressObserver* pObserver = NULL;
        if (!_progressSubject->_observers.empty())
            pObserver = dynamic_cast<ProgressObserver*>(_progressSubject->_observers[0]);
        ProgressSubject progressSubjec;
        progressSubjec.Attach(pObserver);
        progressSubjec.SetTaskNumberSum(1);
        progressSubjec.SetAllTaskSum(CFileOperation::GetFileSizeW(strAppFullName));
        if (fileStream.Ios_CopyToPhoneEx(strAppFullName, strAppInstallFileOnPhone,AppOffsetInExe,AppSize, progressSubjec) == Transfer_Success)
        {
            _progressSubject->AddTaskProgress(100-_progressSubject->GetLastProgress()->_currentTaskProgress);
            _progressSubject->IncreaseTaskNumberComplete();
            _progressSubject->SendProgress();

            //安装文件需传入设备上的位置
            _progressSubject->SetCurrentTaskSum(100);
            _progressSubject->SendPrompt(MESSAGE_INSTALL_INSTALL);

            void* cfString = CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(strAppInstallFileOnPhone));
            PROGRESS_CALLBACK2 pCallback = IosIPAItunesManager::GetInstance()->GetPackInstallCallBack(_progressId);
            iErr = CITunesMobileDevice::iTunes_AMDeviceInstallApplication(_pDevInfoPtr->_ios_pInstallationProxySocket/*pInstallSocket*/, 
                cfString, NULL, pCallback, NULL);
            if (0 == iErr)
            {
                bRet = true;	
                _progressSubject->IncreaseTaskNumberComplete();
                _progressSubject->SendProgress();
            }
            else
            {
                fileStream.Ios_DeleteFile(strAppInstallFileOnPhone);
               // CFileOperation::DeleteFile(strAppInstallFileOnPc);
                //CITunesMobileDevice::CloseSocket(pInstallSocket);
                conn.StopInstallationProxyService();
                errorMsg = CITunesMobileDevice::iTunes_GetDeviceErrorCodeString(iErr);
                if (iErr == -402653056)
                    THROW_ASSISTANT_EXCEPTION(INSTALAPPFILELOST);
                else if (iErr == -402653183)
                    THROW_ASSISTANT_EXCEPTION(INSTALAPPUNZIPERROR);
            }
        }
    }

    fileStream.Ios_DeleteFile(strAppInstallFileOnPhone);
   
    //CITunesMobileDevice::CloseSocket(pInstallSocket);
    conn.StopInstallationProxyService();

    return bRet;
}

int IosIPAOperationWithItunes::InstallAppInZipMode(wstring strAppFullName, string& errorMsg)
{
    AFCFileSysemBase fileStream(_pAfcHandleApp, _pDevInfoPtr);
    fileStream.IsAFCConnectionValid();

    // 上传文件进度条处理
    _progressSubject->ResetProgressData();
    _progressSubject->SetTaskNumberSum(2);
    _progressSubject->SetAllTaskSum(2*100);
    _progressSubject->SetCurrentTaskSum(100);
    _progressSubject->SendPrompt(MESSAGE_INSTALL_UPLOAD);

    ProgressObserver* pObserver = NULL;
    if (!_progressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_progressSubject->_observers[0]);
    ProgressSubject progressSubjec;
    progressSubjec.Attach(pObserver);
    progressSubjec.SetTaskNumberSum(1);
    progressSubjec.SetAllTaskSum(CFileOperation::GetFileSizeW(strAppFullName));

    //安装文件使用GUID区分，可避免并行安装时的问题
    wstring strTempName = L"foo_" + CCodeOperation::GB2312ToUnicode(CFileOperation::GetGuid()) + L".ipa";
    wstring strAppInstallFileOnPhone = L"/PublicStaging/" + strTempName;
    if (fileStream.Ios_CopyToPhone(strAppFullName, strAppInstallFileOnPhone, progressSubjec) != Transfer_Success)
    {
        fileStream.Ios_DeleteFile(strAppInstallFileOnPhone);
        return -1;
    }

    // 安装进度条处理
    _progressSubject->AddTaskProgress(100-_progressSubject->GetLastProgress()->_currentTaskProgress);
    _progressSubject->IncreaseTaskNumberComplete();
    _progressSubject->SendProgress();
    _progressSubject->SetCurrentTaskSum(100);
    _progressSubject->SendPrompt(MESSAGE_INSTALL_INSTALL);

    clock_t begin, end;
    begin = clock();
    int iErr = InstallAppSubProcedure(strAppInstallFileOnPhone, strAppFullName, errorMsg);
    end = clock();
    wchar_t temp[128] = {0};
    wsprintf(temp, L"InstallAppSubProcedure use time: %d\n", end-begin);
    OutputDebugStr(temp);

    if (0 == iErr)
    {
        _progressSubject->IncreaseTaskNumberComplete();
        _progressSubject->SendProgress();
    }

    return iErr;
}

int IosIPAOperationWithItunes::ItunesFastInstallApplication(wstring strAppFullName)
{
    IosUsbSync usbSync(_pDevInfoPtr);			
    usbSync.StartSync();

    IosUsbConnection usbConn((DeviceInfo*)_pDevInfoPtr);
    //RestartAFCServiceIfNeed(usbConn);
    AFCFileSysemBase fileStream(_pAfcHandleApp, _pDevInfoPtr);
    fileStream.IsAFCConnectionValid();

    _progressSubject->ResetProgressData();
    _progressSubject->SetTaskNumberSum(2);
    _progressSubject->SetAllTaskSum(2*100);

    _progressSubject->SetCurrentTaskSum(100);
    _progressSubject->SendPrompt(MESSAGE_INSTALL_UPLOAD);

    void* cfString = CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(strAppFullName));
    PROGRESS_CALLBACK2 pCallback = IosIPAItunesManager::GetInstance()->GetPackInstallCallBack(_progressId);
    int iErr = CITunesMobileDevice::iTunes_AMDeviceTransferPath(_pDevInfoPtr->_ios_devptr, 
        cfString, NULL, pCallback, NULL);

    if (0 == iErr)
    {
        wstring ipaFolder = L"PublicStaging/" + CFileOperation::GetFileName(strAppFullName);
        void* sockProxy = NULL;
        iErr = usbConn.StartIOSService((char*)"com.apple.mobile.installation_proxy", &sockProxy);
        if (0 == iErr)
        {
            _progressSubject->IncreaseTaskNumberComplete();
            _progressSubject->SendProgress();

            //安装文件只需传入本地即可
            _progressSubject->SetCurrentTaskSum(100);
            _progressSubject->SendPrompt(MESSAGE_INSTALL_INSTALL);

            void* cfString = CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(strAppFullName));
            PROGRESS_CALLBACK2 pCallback = IosIPAItunesManager::GetInstance()->GetPackInstallCallBack(_progressId);
            iErr = CITunesMobileDevice::iTunes_AMDeviceInstallApplication(sockProxy, 
                cfString, NULL, pCallback, NULL);

            if (0 == iErr)
            {
                _progressSubject->IncreaseTaskNumberComplete();
                _progressSubject->SendProgress();
            }

            usbConn.StopIOSService(&sockProxy);
        }
    }
    
    usbSync.StopSync();

    return iErr;
}

int IosIPAOperationWithItunes::InstallAppInUnzipModeWithiTunesInterface(wstring strAppFullName, string& errorMsg)
{
    AFCFileSysemBase fileStream(_pAfcHandleApp, _pDevInfoPtr);
    fileStream.IsAFCConnectionValid();

    _progressSubject->ResetProgressData();
    _progressSubject->SetTaskNumberSum(2);
    _progressSubject->SetAllTaskSum(2*100);

    _progressSubject->SetCurrentTaskSum(100);
    _progressSubject->SendPrompt(MESSAGE_INSTALL_UPLOAD);

    PROGRESS_CALLBACK2 pCallback = IosIPAItunesManager::GetInstance()->GetPackInstallCallBack(_progressId);
    void* cfString = CCFConverter::StringtoCFString(CCodeOperation::UnicodeToUTF_8(strAppFullName), false);
    int iErr = CITunesMobileDevice::iTunes_AMDeviceTransferPath(_pDevInfoPtr->_ios_devptr, 
        cfString, NULL, pCallback, NULL);

    wstring strAppInstallFileOnPhone = L"PublicStaging/" + CFileOperation::GetFileName(strAppFullName);
    if (iErr == 0)
    {
        _progressSubject->IncreaseTaskNumberComplete();
        _progressSubject->SendProgress();
        _progressSubject->SetCurrentTaskSum(100);
        _progressSubject->SendPrompt(MESSAGE_INSTALL_INSTALL);

        iErr = InstallAppSubProcedure(strAppInstallFileOnPhone, strAppFullName, errorMsg);
        if (0 == iErr)
        {
            _progressSubject->IncreaseTaskNumberComplete();
            _progressSubject->SendProgress();
        }
    }
    else
    {
        //这里是获取传输失败消息,安装失败在上面获取
        errorMsg = CITunesMobileDevice::GetAMDErrorText(iErr);
        if (!errorMsg.empty())
            LOG_DEBUG(L"Transfer Ipa to device with Error:"+CCodeOperation::UTF_8ToUnicode(errorMsg));
        errorMsg = GetAMDErrorByDescription(errorMsg, iErr);
        fileStream.Ios_DeleteFile(strAppInstallFileOnPhone);
    }

    return iErr;
}

int IosIPAOperationWithItunes::FastInsallApplication(wstring strAppFullName)
{
    IosUsbConnection usbConn((DeviceInfo*)_pDevInfoPtr);
    //RestartAFCServiceIfNeed(usbConn);

    _progressSubject->ResetProgressData();
    _progressSubject->SetTaskNumberSum(2);
    _progressSubject->SetAllTaskSum(2*100);
    _progressSubject->SetCurrentTaskSum(100);
    _progressSubject->SendPrompt(MESSAGE_INSTALL_UPLOAD);

    // 添加进度转换
    ProgressObserver* pObserver = NULL;
    if (!_progressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_progressSubject->_observers[0]);

    ProgressSubject progressSubjec;
    progressSubjec.Attach(pObserver);
    progressSubjec.SetTaskNumberSum(1);
    //progressSubjec.SetAllTaskSum(CFileOperation::GetFileSizeW(strAppFullName));

    wstring ipaFolder;
    AFCFileSysemBase* fileStream = new AFCFileSysemBase(_pAfcHandleApp, _pDevInfoPtr);
    fileStream->IsAFCConnectionValid();
    IPAPackageHelper* packageHelper = new IPAPackageHelper(_pDevInfoPtr, fileStream, strAppFullName);
    progressSubjec.SetAllTaskSum(packageHelper->GetPackageUncompressedSize());
    if (!packageHelper->TransferIPAPayload(ipaFolder, &progressSubjec))
    {
        fileStream->Ios_DeleteFile(ipaFolder);
        SAFE_DELETE(fileStream);
        SAFE_DELETE(packageHelper);
        return -1;//传输失败反馈-1;
    }

    //_progressSubject->AddTaskProgress(100-_progressSubject->GetLastProgress()->_currentTaskProgress);
    // ? 此次可能有错误，因为总任务数为200，每个子任务数100，这里可能会超出了子任务数的最大值
    _progressSubject->AddTaskProgress(200);
    _progressSubject->IncreaseTaskNumberComplete();
    _progressSubject->SendProgress();
    void* sockProxy = NULL;
    int iErr = usbConn.StartIOSService((char*)"com.apple.mobile.installation_proxy", &sockProxy);
    if (0 == iErr)
    {
        //安装文件只需传入本地即可
        _progressSubject->SetCurrentTaskSum(100);
        _progressSubject->SendPrompt(MESSAGE_INSTALL_INSTALL);

        void* cfString = CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(ipaFolder));
        PROGRESS_CALLBACK2 pCallback = IosIPAItunesManager::GetInstance()->GetPackInstallCallBack(_progressId);
        iErr = CITunesMobileDevice::iTunes_AMDeviceInstallApplication(sockProxy, cfString, NULL, pCallback, NULL);

        if (0 == iErr)
        {
            _progressSubject->IncreaseTaskNumberComplete();
            _progressSubject->SendProgress();
        }
        else
            //安装失败的时候,需要判断下afc服务时候down了,
            //未越狱的设备安装破解软件,会导致afcdown掉.
            //RestartAFCServiceIfNeed(usbConn);
            fileStream->IsAFCConnectionValid();

        //删除掉安装失败的安装文件
        if (0 != iErr && fileStream->Ios_Exists(ipaFolder))
            fileStream->Ios_DeleteFile(ipaFolder);

        //获取错误消息
        if (0 != iErr)
            LOG->WriteError(L"Insatll Ipa "+ipaFolder+ L" Error ");
    }

    SAFE_DELETE(fileStream);
    SAFE_DELETE(packageHelper);
    return iErr;
}

int IosIPAOperationWithItunes::InstallAppInUnzipMode(wstring strAppFullName, string& errorMsg)
{
    AFCFileSysemBase* fileStream = new AFCFileSysemBase(_pAfcHandleApp, _pDevInfoPtr);
    fileStream->IsAFCConnectionValid();

    // 上传文件进度条处理
    _progressSubject->ResetProgressData();
    _progressSubject->SetTaskNumberSum(2);
    _progressSubject->SetAllTaskSum(2*100);
    _progressSubject->SetCurrentTaskSum(100);
    _progressSubject->SendPrompt(MESSAGE_INSTALL_UPLOAD);

    // 添加进度转换
    ProgressObserver* pObserver = NULL;
    if (!_progressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_progressSubject->_observers[0]);

    ProgressSubject progressSubjec;
    progressSubjec.Attach(pObserver);
    progressSubjec.SetTaskNumberSum(1);

    wstring ipaFolder;
    IPAPackageHelper* packageHelper = new IPAPackageHelper(_pDevInfoPtr, fileStream, strAppFullName);
    progressSubjec.SetAllTaskSum(packageHelper->GetPackageUncompressedSize());
    if (!packageHelper->TransferIPAPayload(ipaFolder, &progressSubjec))
    {
        fileStream->Ios_DeleteFile(ipaFolder);
        SAFE_DELETE(fileStream);
        SAFE_DELETE(packageHelper);
        return -1;//传输失败反馈-1;
    }

    _progressSubject->IncreaseTaskNumberComplete();
    _progressSubject->SendProgress(); 
    _progressSubject->SetCurrentTaskSum(100);
    _progressSubject->SendPrompt(MESSAGE_INSTALL_INSTALL);

    int iErr = InstallAppSubProcedure(ipaFolder, strAppFullName, errorMsg);
    if (0 == iErr)
    {
        _progressSubject->IncreaseTaskNumberComplete();
        _progressSubject->SendProgress();
    }

    //删除掉安装失败的安装文件
    if (0 != iErr && fileStream->Ios_Exists(ipaFolder))
        fileStream->Ios_DeleteFile(ipaFolder);

    SAFE_DELETE(fileStream);
    SAFE_DELETE(packageHelper);
    return iErr;
}

int IosIPAOperationWithItunes::InstallAppSubProcedure(wstring strAppInstallFileOnPhone, wstring strAppFullName, string& errorMsg)
{
    bool result = InstallAppOnDeviceWithProtocol(strAppInstallFileOnPhone, NULL, errorMsg);

    IPAOperationErrorType errorType = UnknowError;
    if (result)
    {
        errorType = None;

        // 安装成功,判断下是否需要还原存档
        if (IPAPackageHelper::IsArchivedIPA(strAppFullName))
        {
            //IPAPackageHelper.TransferIPAArchive(pathOnPC, packageInfo, this, AppCallBack, m_installArgs);

            AFCFileSysemBase* pFileStream =  AFCAppFileSystem::StartAPPHouseArrestService(_identifier, (DeviceInfo*)_pDevInfoPtr);
            RETURN_VALUE_IF(NULL == pFileStream, -1);
            IPAPackageHelper* packageHelper = new IPAPackageHelper(_pDevInfoPtr, pFileStream, strAppFullName);
            packageHelper->TransferIPAArchive(_progressSubject);
            SAFE_DELETE(pFileStream);
            SAFE_DELETE(packageHelper);
        }
    }
    else
    {
        //安装失败的时候,需要判断下afc服务是否down了,
        //未越狱的设备安装破解软件,会导致afcdown掉.
        AFCFileSysemBase fileStream(_pAfcHandleApp, _pDevInfoPtr);
        fileStream.IsAFCConnectionValid();

        RETURN_VALUE_IF(errorMsg.empty(), (int)errorType);

        if (errorMsg == "ApplicationVerificationFailed")
            errorType = ApplicationVerificationFailed;
        else if (errorMsg == "IncorrectArchitecture")
            errorType = IncorrectArchitecture;
        else if (errorMsg == "InstallProhibited")
            errorType = InstallProhibited;
        else if (errorMsg == "PackageExtractionFailed")
            errorType = PackageExtractionFailed;
        else if (errorMsg == "PackageInspectionFailed")
            errorType = PackageInspectionFailed;
        else if (errorMsg == "APIInternalError")
            errorType = APIInternalError;
    }

    return (int)errorType;
}

bool IosIPAOperationWithItunes::InstallAppOnDeviceWithProtocol(wstring strAppInstallFileOnPhone, void* clientOptionsDict, string& errorMsg)
{
    std::map<string, void*> msgDict;
    msgDict["Command"] = CCFConverter::StringtoCFString("Install");
    msgDict["PackagePath"] = CCFConverter::StringtoCFString(CCodeOperation::UnicodeToUTF_8(strAppInstallFileOnPhone), false);
    if (NULL != clientOptionsDict)
        msgDict["ClientOptions"] = clientOptionsDict;

    return InstallationProxyPerformOperation("do Install operation ", msgDict, errorMsg);
}

bool IosIPAOperationWithItunes::InstallationProxyPerformOperation(string operation, map<string, void*> msgDict, string& errorMsg)
{
    SetCSLock(((DeviceInfo*)_pDevInfoPtr)->_csInstallationProxyLock);
    bool res = false;
    errorMsg = "";
    IosUsbConnection usbConn((DeviceInfo*)_pDevInfoPtr);

    try
    {
        RETURN_FALSE_IF(!usbConn.StartInstallationProxyService());
        if (!usbConn.SendMessageToService(_pDevInfoPtr->_ios_pInstallationProxySocket, msgDict))
        {
            usbConn.StopInstallationProxyService();
            return false;
        }
		DWORD beginRecive =	GetTickCount();

		LOG->WriteDebug(L"InstallationProxyPerformOperation ReadMssageFromService begin" );
        while (true)
        {
            map<string, string> replyInfo;
            usbConn.ReadMssageFromService(_pDevInfoPtr->_ios_pInstallationProxySocket, replyInfo);
            if (replyInfo.empty())
            {
                errorMsg = "The installationProxyService error";
                break;
            }

            // 判断是否出错了
            if(replyInfo.find("Error") != replyInfo.end())
            {
                errorMsg = replyInfo["Error"];
                break;
            }

            // 判断是否完成
            if(replyInfo.find("Status") != replyInfo.end()
                && replyInfo["Status"] == "Complete")
            {
                res = true;
                break;
            }
			DWORD endRecive =	GetTickCount();
			if(endRecive - beginRecive > 60000)//1分钟超时
			{
				break;
			}
			//LOG->WriteDebug(L"InstallationProxyPerformOperation ReadMssageFromService:" + CCodeOperation::StringToWstring(replyInfo.c));
        }
		LOG->WriteDebug(L"InstallationProxyPerformOperation ReadMssageFromService end" );
    }
    catch (...)
    {
        LOG->WriteError(L"InstallationProxyPerformOperation Error");
        res = false;
    }

    usbConn.StopInstallationProxyService();
    return res;
}

string IosIPAOperationWithItunes::GetAMDErrorByDescription(string errorMsg, int iErrCode)
{
    // 这个方法是有一些问题的.主要是一些错误对应的错误码在不同的版本itunes里面不一样;
    // 比如 kMISInstallProhibited  设备禁止安装 等

    string languageKey;
    string errorMsgDes;

    if (iErrCode == -402620415)
        languageKey = "kMISAdHocSignatureError";
    else if (iErrCode == -402620391)
        languageKey = "kMISAdHocSignatureError";
    //else if (iErrCode == -402636802)//设备禁止安装
    //    languageKey = "kMISInstallProhibited";
    else if (iErrCode == -402653056)
        languageKey = "kMISInstalAppFileLost";  //应用程序包不完整
    else if (iErrCode == -402653183)
        languageKey = "kMISInstalAppUnZipError";  //应用程序包无法解压

    if (languageKey.empty() && CStrOperation::startsWith(errorMsg, "kAM"))
        languageKey = errorMsg;

    if (!languageKey.empty())
        errorMsgDes = languageKey;

    if (errorMsgDes.empty())
        errorMsgDes = errorMsg;

    return errorMsgDes;
}

void IosIPAOperationWithItunes::RestartAFCServiceIfNeed(IosUsbConnection iosConet)
{
    // 先判断AFC服务是否正常
    try
    {
        wstring afcTestFileName = L"netDragon.mobile.device_AFCTester";
        AFCFileSysemBase fileStream(_pAfcHandleApp, _pDevInfoPtr);
        fileStream.OpenWrite(afcTestFileName, "0123456789");
        if (fileStream.Ios_Exists(afcTestFileName))
        { 
            fileStream.Ios_DeleteFile(afcTestFileName);
            return;
        }
    }
    catch (...)
    {
        LOG->WriteError(L"AFC Service Down");
    }

    // 异常的话重启该服务
    void* afcSocket = NULL;
    void* afcHandle = NULL;
    string temp = "com.apple.afc";
    if (0 != iosConet.StartIOSService( (char*)temp.c_str() , &afcSocket))
        return ;

    if (0 != CITunesMobileDevice::iTunes_AFCConnectionOpen(afcSocket, 0, &afcHandle))
    {
        CITunesMobileDevice::CloseSocket(afcSocket);
        afcSocket = NULL;
    }
}

// 返回结果类似如下,以SBMatchingApplicationGenres 为key
//        <dict>
//        <key>CFBundleIdentifier</key>
//        <string>com.apple.mobilemail</string>
//        <key>SBMatchingApplicationGenres</key>
//        <array>
//            <string>Productivity</string>
//            <string>Utilities</string>
//            <string>Business</string>
//        </array>
//        </dict>

void IosIPAOperationWithItunes::SetAppDiskUsageSendInfo(map<string, void*>* srcDict)
{
    vector<string> temp;
    temp.push_back("CFBundleIdentifier");
    temp.push_back("DynamicDiskUsage");
    temp.push_back("StaticDiskUsage");
    temp.push_back("UIFileSharingEnabled");
    temp.push_back("CFBundleDisplayName");
    temp.push_back("CFBundleShortVersionString");
    temp.push_back("CFBundleVersion");
    temp.push_back("Container");
    temp.push_back("CFBundleExecutable");
    temp.push_back("CFBundleName");
    temp.push_back("CFBundleIconFile");
    temp.push_back("CFBundleIconFiles");
    temp.push_back("Icon files");
    temp.push_back("ApplicationDSID");
    //temp.push_back("SBMatchingApplicationGenres");// 获取appType对应已经安装程序所属类别信息
    srcDict->insert(pair<string, void*>("ReturnAttributes", CCFConverter::ConvertStringVector(temp)));
}

bool IosIPAOperationWithItunes::GetAppInfos(void* pDevPtr, APP_OWNER eType, map<std::string, void*> &mapInfos, bool isGetAppDiskUsage)
{
	if (!pDevPtr)
		return false;

	_eType = eType;
	void* pKey = CCFConverter::StringtoCFString("ApplicationType");
	void* pValue = NULL;
	switch (eType)
	{
	case APP_OWNER_USER:
		pValue = CCFConverter::StringtoCFString("User");
		break;
	case APP_OWNER_SYS:
		pValue = CCFConverter::StringtoCFString("System");
		break;
	case APP_OWNER_INTERNAL:
		pValue = CCFConverter::StringtoCFString("Internal");
		break;
	default: //默认user
		pValue = CCFConverter::StringtoCFString("User");
		break;
	}

    map<string, void*> srcDict;
    srcDict.insert(pair<string, void*>("ApplicationType", pValue));

    // 设置获取安装程序的磁盘使用信息的发送信息
    if (isGetAppDiskUsage && IosPublicFunc::GetVerNum(_pDevInfoPtr->_deviceProductVersion) >= 400)
        SetAppDiskUsageSendInfo(&srcDict);

	bool bRet = false;
	//void* pDict = CCFConverter::NDCFDictionaryCreate(CCFConverter::NDkCFAllocatorSystemDefault(), &pKey, &pValue, 1, NULL, NULL);
    void* pDict = CCFConverter::DictionarytoCFDictionary(srcDict);
	SetCSLock(((DeviceInfo*)_pDevInfoPtr)->_csIOSSessionLock);
	if (0 == CITunesMobileDevice::iTunes_AMDeviceConnect(pDevPtr))
	{
		if (0 == CITunesMobileDevice::iTunes_AMDeviceStartSession(pDevPtr))
		{
			void* pApps = NULL;
			if (0 == CITunesMobileDevice::iTunes_AMDeviceLookupApplications(pDevPtr, pDict, &pApps))
			{
				bRet = CCFConverter::GetIosData(pApps, mapInfos);
// 				if (pApps)
// 					CCFConverter::NDCFRelease(pApps);
			}

			CITunesMobileDevice::iTunes_AMDeviceStopSession(pDevPtr);
		}
		CITunesMobileDevice::iTunes_AMDeviceDisconnect(pDevPtr);
	}

	return bRet;
}

bool IosIPAOperationWithItunes::GetAppDiskUsage(DeviceInfo* pDevInfo, map< wstring, vector<int64_t> >& diskUsages)
{
    RETURN_FALSE_IF(!pDevInfo);
    _pDevInfoPtr = pDevInfo;

    map<string, void*> srcDict;
    srcDict.insert(pair<string, void*>("ApplicationType", CCFConverter::StringtoCFString("User")));

    vector<string> temp;
    temp.push_back("CFBundleIdentifier");
    temp.push_back("DynamicDiskUsage");
    temp.push_back("StaticDiskUsage");
    srcDict.insert(pair<string, void*>("ReturnAttributes", CCFConverter::ConvertStringVector(temp)));

    bool bRet = false;
    map<std::string, void*> mapInfos;
    void* pDict = CCFConverter::DictionarytoCFDictionary(srcDict);

    SetCSLock(((DeviceInfo*)_pDevInfoPtr)->_csIOSSessionLock);
    if (0 == CITunesMobileDevice::iTunes_AMDeviceConnect(pDevInfo->_ios_devptr))
    {
        if (0 == CITunesMobileDevice::iTunes_AMDeviceStartSession(pDevInfo->_ios_devptr))
        {
            void* pApps = NULL;
            if (0 == CITunesMobileDevice::iTunes_AMDeviceLookupApplications(pDevInfo->_ios_devptr, pDict, &pApps))
            {
                bRet = CCFConverter::GetIosData(pApps, mapInfos);
// 				if (pApps)
// 					CCFConverter::NDCFRelease(pApps);
            }

            CITunesMobileDevice::iTunes_AMDeviceStopSession(pDevInfo->_ios_devptr);
        }
        CITunesMobileDevice::iTunes_AMDeviceDisconnect(pDevInfo->_ios_devptr);
    }

    map<std::string, void*>::iterator it; 
    for (it = mapInfos.begin(); it != mapInfos.end(); it++)
    {
        map<string, void*> appInfoNormal;
        CCFConverter::GetIosData(it->second, appInfoNormal);

        vector<int64_t> disk;
        disk.push_back(GetInt64Value(appInfoNormal, "DynamicDiskUsage"));
        disk.push_back(GetInt64Value(appInfoNormal, "StaticDiskUsage"));
        diskUsages[GetStringValue(appInfoNormal, "CFBundleIdentifier")] = disk;
    }

    return bRet;
}

wstring IosIPAOperationWithItunes::GetStringValue(map<string, void*> appInfoNormal, string key)
{
    map<string, void*>::const_iterator find;
    find = appInfoNormal.find(key);
    if (find ==  appInfoNormal.end())
        return L"";

    string temp;
    CCFConverter::GetIosData(find->second, temp, false);
    return CCodeOperation::UTF_8ToUnicode(temp);
}

int64_t IosIPAOperationWithItunes::GetInt64Value(map<string, void*> appInfoNormal, string key)
{
    map<string, void*>::const_iterator find;
    find = appInfoNormal.find(key);
    if (find == appInfoNormal.end())
        return 0;

    UINT64 temp = 0;
    CCFConverter::GetIosData(find->second, temp);
    return temp;
}

bool IosIPAOperationWithItunes::GetBoolValue(map<string, void*> appInfoNormal, string key)
{
    map<string, void*>::const_iterator find;
    find = appInfoNormal.find(key);
    if (find ==  appInfoNormal.end())
        return false;

    bool temp;
    CCFConverter::GetIosData(find->second, temp);
    return temp;
}

vector<wstring> IosIPAOperationWithItunes::GetVectorStringValue(map<string, void*> appInfoNormal, string key)
{
    vector<wstring> wTemp;
    map<string, void*>::const_iterator find;
    find = appInfoNormal.find(key);
    if (find ==  appInfoNormal.end())
        return wTemp;

    vector<string> temp;
    CCFConverter::GetIosData(find->second, temp);

    for (size_t i = 0; i < temp.size(); i++)
        wTemp.push_back(CCodeOperation::GB2312ToUnicode(temp.at(i)));

    return wTemp;
}

void IosIPAOperationWithItunes::PraseAppInfos(DeviceInfo* devInfo, map<std::string, void*> mapInfos, vector<AppData*> &vtInfos)
{
    vtInfos.clear();

    map<std::string, void*>::iterator it; 
    for (it = mapInfos.begin(); it != mapInfos.end(); it++)
    {
		if(!devInfo)
			return;
        AppData* pData = new AppData();
        pData->_eAppType = APP_TYPE_IPA;
        pData->_eAppOwner = _eType;

        map<string, void*> appInfoNormal;
        CCFConverter::GetIosData(it->second, appInfoNormal);
        
        pData->_applicationDSID = GetInt64Value(appInfoNormal, "ApplicationDSID");
        pData->_isLegalSoft = (pData->_applicationDSID == 0 ? 0 : 1);
        pData->_dynamicDiskUsage = GetInt64Value(appInfoNormal, "DynamicDiskUsage");
        pData->_staticDiskUsage = GetInt64Value(appInfoNormal, "StaticDiskUsage");
        pData->_bShare = GetBoolValue(appInfoNormal, "UIFileSharingEnabled");
        pData->_AppName = GetStringValue(appInfoNormal, "CFBundleDisplayName");
        pData->_strVersion = GetStringValue(appInfoNormal, "CFBundleShortVersionString");
        if (pData->_strVersion.empty())
            pData->_strVersion = GetStringValue(appInfoNormal, "CFBundleVersion");
        pData->_packageName = GetStringValue(appInfoNormal, "CFBundleIdentifier");
        pData->_strAppPhonePath = GetStringValue(appInfoNormal, "Container");
        pData->_strIconPhonePath = CStrOperation::trimRightW(pData->_strAppPhonePath, L"/") + L"/";

        pData->_strBundleExecutableName = GetStringValue(appInfoNormal, "CFBundleExecutable");
        if (pData->_strBundleExecutableName.empty())
            pData->_strBundleExecutableName = GetStringValue(appInfoNormal, "CFBundleName");
        if (pData->_strBundleExecutableName.length() > 0)
            pData->_strBundleExecutableName += L".app";

        if (pData->_AppName.length() <= 0)
        {
            pData->_AppName = GetStringValue(appInfoNormal, "CFBundleName");
            if (pData->_AppName.length() <= 0)
                pData->_AppName = pData->_strBundleExecutableName;
        }

        pData->_strIconName = GetStringValue(appInfoNormal, "CFBundleIconFile");
        if (pData->_strIconName.length() == 0)
        {
            vector<wstring> strIconNames = GetVectorStringValue(appInfoNormal, "CFBundleIconFiles");
            if (strIconNames.empty())
                strIconNames = GetVectorStringValue(appInfoNormal, "Icon files");

            if (!strIconNames.empty())
                pData->_strIconName = strIconNames.at(0);
        }

        if (pData->_strIconName.length() <= 0 || !CStrOperation::endsWithW(CStrOperation::toLowerW(pData->_strIconName), L".png"))
            pData->_strIconName = L"Icon.png";
 
        //PraseIconPhonePath(devInfo, pData, strBundleExecutableName, strIconName);
        //PrasePurchaser(devInfo, pData);

        //vector<wstring> strApplicationGenres = GetVectorStringValue(appInfoNormal, "SBMatchingApplicationGenres");

        vtInfos.push_back(pData);
    }
}

bool IosIPAOperationWithItunes::GetAppIconPhonePath(DeviceInfo* pDevInfo, AppData* pApp)
{
    RETURN_FALSE_IF(pApp->_eAppType != APP_TYPE_IPA);
    PraseIconPhonePath(pDevInfo, pApp);
    return true;
}

bool IosIPAOperationWithItunes::GetAppPurchaser(DeviceInfo* pDevInfo, AppData* pApp)
{
    RETURN_FALSE_IF(pApp->_eAppType != APP_TYPE_IPA);
    PrasePurchaser(pDevInfo, pApp);
    return true;
}

#define Check_Icon_Path(pFileStream, pData, strIconPath)   if ( pFileStream->Ios_Exists(pData->_strIconPhonePath) )  { SAFE_DELETE(pFileStream); return; }  else { pData->_strIconPhonePath = strIconPath; };

#define CHCEK_ICON_PATH(pFileStream, pData, strIconPath)   if ( pFileStream->Ios_Exists(pData._strIconPhonePath) )  { SAFE_DELETE(pFileStream); return; }  else { pData._strIconPhonePath = strIconPath; };

void IosIPAOperationWithItunes::PraseIconPhonePath(DeviceInfo* devInfo, AppData* pData)
{
	RETURN_IF(!devInfo);

	AFCFileSysemBase* pFileStream = NULL;
	if (devInfo->_ios_bJailBreaked)
	{
		pFileStream = new AFCFileSysemBase(devInfo);
		RETURN_IF(!pFileStream);

		pData->_strIconPhonePath += pData->_strBundleExecutableName + L"/" + pData->_strIconName;
		wstring startDir = CStrOperation::trimRightW(pData->_strAppPhonePath, L"/") + L"/";

		Check_Icon_Path(pFileStream, pData, startDir+pData->_strBundleExecutableName+L"/icon.png");
		if (pFileStream->Ios_Exists(pData->_strIconPhonePath))
		{
			SAFE_DELETE(pFileStream);
			return ;
		}

		// 设备个别的应用目录结构不同
		DeviceData* pDevData = DeviceManager::GetInstance()->FindDevice(devInfo->_ios_deviceIdentifier);
		RETURN_IF(!pDevData);
		DirData* pdir = (DirData*)FileHelper::GetNode(pDevData->GetRootData()->GetDir(), pData->_strAppPhonePath);
		RETURN_IF(pdir == NULL);

		deque<NodeData*> pNodeS = pdir->GetChildrens();
		for (size_t i = 0; i < pNodeS.size(); i++)
		{
			NodeData* pNode = pNodeS.at(i);
			if (!CStrOperation::endsWithW(pNode->_name, L".app"))
				continue;

			pData->_strIconPhonePath = startDir + pNode->_name + L"/" + pData->_strIconName;
			Check_Icon_Path(pFileStream, pData, startDir+pNode->_name+L"/icon_57.png");
			Check_Icon_Path(pFileStream, pData, startDir+pNode->_name+L"/icon57.png");
			Check_Icon_Path(pFileStream, pData, startDir+pNode->_name+L"/icon-57.png");
			Check_Icon_Path(pFileStream, pData, startDir+pNode->_name+L"/icon_72.png");
			Check_Icon_Path(pFileStream, pData, startDir+pNode->_name+L"/icon72.png");
			Check_Icon_Path(pFileStream, pData, startDir+pNode->_name+L"/icon-72.png");
			Check_Icon_Path(pFileStream, pData, startDir+pNode->_name+L"/Icon-72.png");
			Check_Icon_Path(pFileStream, pData, startDir+pNode->_name+L"/icon_114.png");
			Check_Icon_Path(pFileStream, pData, startDir+pNode->_name+L"/icon114.png");
			Check_Icon_Path(pFileStream, pData, startDir+pNode->_name+L"/icon-114.png");
			Check_Icon_Path(pFileStream, pData, startDir+pNode->_name+L"/Default.png");

			break;
		}
		Check_Icon_Path(pFileStream, pData, startDir+L"iTunesArtwork");
	}
	else
	{
		pFileStream =  AFCAppFileSystem::StartAPPHouseArrestService(pData->_packageName, devInfo);
		RETURN_IF(!pFileStream);

		pData->_strIconPhonePath = pData->_strBundleExecutableName+L"/"+pData->_strIconName;
		Check_Icon_Path(pFileStream, pData, pData->_AppName+L".app"+L"/"+pData->_strIconName);
		Check_Icon_Path(pFileStream, pData, pData->_AppName+L".app"+L"/57.png");
		Check_Icon_Path(pFileStream, pData, pData->_AppName+L".app"+L"/72.png");
		Check_Icon_Path(pFileStream, pData, pData->_AppName+L".app"+L"/114.png");
		Check_Icon_Path(pFileStream, pData, L"iTunesArtwork");
	}

	SAFE_DELETE(pFileStream);
}

void IosIPAOperationWithItunes::PrasePurchaser(DeviceInfo* devInfo, AppData* pData)
{
    AFCFileSysemBase* pFileStream = NULL;
    if (devInfo->_ios_bJailBreaked)
    {
        pFileStream = new AFCFileSysemBase(devInfo);
        if (pFileStream)
				{
					wstring strMetadataPlistFile = CStrOperation::trimRightW(pData->_strAppPhonePath, L"/") + L"/" + IPA_APP_METADATA_FILE;
					pData->_strPurchaser = GetPurchaser(pFileStream, strMetadataPlistFile);
					delete pFileStream;
					pFileStream = NULL;
				}
    }
    else
    {
        pFileStream =  AFCAppFileSystem::StartAPPHouseArrestService(pData->_packageName, devInfo);
        if (pFileStream)
				{
        	wstring strMetadataPlistFile = IPA_APP_METADATA_FILE;
        	pData->_strPurchaser = GetPurchaser(pFileStream, strMetadataPlistFile);
        	delete pFileStream;
        	pFileStream = NULL;
				}
    }
}

wstring IosIPAOperationWithItunes::GetPurchaser(AFCFileSysemBase* pFileStream, wstring strMetadataFullName)
{
	if (!pFileStream || 
		strMetadataFullName.length() <= 0 ||
		!pFileStream->Ios_Exists(strMetadataFullName) ||
        pFileStream->Ios_IsDirectory(strMetadataFullName))
		return L"";

	wstring strPurchaser = L"";

    wstring strMetadataOnPc = Path::GlobalTemp() + CFileOperation::GetGuidW()
        + IPA_APP_METADATA_FILE; // 文件存放在本地的全路径

	if (pFileStream->Ios_DownFromPhone(strMetadataFullName, strMetadataOnPc) == Transfer_Success)
	{
		// 将plist文件转换为xml文件并解析
		if (IosPlistConverter::GetXmlFromPlist(strMetadataOnPc, strMetadataOnPc))
		{
			strPurchaser = IosAppXmlHelper::GetPurchaser(strMetadataOnPc);
		}
        CFileOperation::DeleteFile(strMetadataOnPc);
	}

	return strPurchaser;
}

bool IosIPAOperationWithItunes::ArchiveAppOnPhone(const DeviceInfo* pDevInfo, wstring strIdentifier, eArchiveMode eMode, bool bUnInstall)
{
    SetCSLock(((DeviceInfo*)pDevInfo)->_csInstallationProxyLock);

	if (!pDevInfo || strIdentifier.length() <= 0)
		return false;

    IosUsbConnection conn((DeviceInfo*)pDevInfo);
    if (!conn.StartInstallationProxyService())
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
	void* pKey = CCFConverter::StringtoCFString("ArchiveType");
	void* pVaule = CCFConverter::StringtoCFString(strMode);
	CCFConverter::NDCFDictionaryAddValue(pCFDict, pKey, pVaule);
	pKey = CCFConverter::StringtoCFString("SkipUninstall");
	pVaule = CCFConverter::BoolToCFBool(bUnInstall?false:true);
	CCFConverter::NDCFDictionaryAddValue(pCFDict, pKey, pVaule);

    bool bRet = false;
    void* cfString = CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(strIdentifier));
    PROGRESS_CALLBACK2 pCallback = IosIPAItunesManager::GetInstance()->GetArchiveIPACallBack(_progressId);
	if (0 == CITunesMobileDevice::iTunes_AMDeviceArchiveApplication(pDevInfo->_ios_pInstallationProxySocket, cfString, pCFDict, pCallback, NULL))
		bRet = true;

    conn.StopInstallationProxyService();

	return bRet;
}

bool IosIPAOperationWithItunes::RemoveAppArchiveOnPhone(const DeviceInfo* pDevInfo, wstring strIdentifier)
{
    SetCSLock(((DeviceInfo*)pDevInfo)->_csInstallationProxyLock);

	if (!pDevInfo || strIdentifier.length() <= 0)
		return false;

	IosUsbConnection conn((DeviceInfo*)pDevInfo);
	if (!conn.StartInstallationProxyService())
		return false;

    bool bRet = false;
    void* cfString = CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(strIdentifier));
    PROGRESS_CALLBACK2 pCallback = IosIPAItunesManager::GetInstance()->GetRemoveIPAArchiveCallBack(_progressId);
	if (0 == CITunesMobileDevice::iTunes_AMDeviceRemoveApplicationArchive(pDevInfo->_ios_pInstallationProxySocket, cfString, NULL, pCallback, NULL))
		bRet = true;

    conn.StopInstallationProxyService();

	return bRet;
}


#endif