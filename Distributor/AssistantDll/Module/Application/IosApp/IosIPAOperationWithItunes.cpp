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
        return COMMAND_THREAD_OVER_LIMIT;   // ��֧�ָ���Ĳ���������

    _progressSubject = &subject;

	if (!pDevInfo || strIdentifier.length() <= 0 || strFileOnPc.length() <= 0)
        return COMMAND_EXECUTE_FAIL;

	AFCFileSysemBase fileStream(pDevInfo->_ios_afcHandleApp, pDevInfo);
	wstring strAppArchivesFileOnPhone = L"ApplicationArchives/" + strIdentifier + L".zip";

	_progressSubject->ResetProgressData();
	_progressSubject->SetTaskNumberSum(4);
	_progressSubject->SetAllTaskSum(400);

	//��ǰ���ݹ���.��ɾ��
	_progressSubject->SetCurrentTaskSum(100);
	_progressSubject->SendPrompt(MESSAGE_ARCHIVE_REMOVE);
	if (fileStream.Ios_Exists(strAppArchivesFileOnPhone))
		RemoveAppArchiveOnPhone(pDevInfo, strIdentifier);

	_progressSubject->AddTaskProgress(100-_progressSubject->GetLastProgress()->_currentTaskProgress);
	_progressSubject->IncreaseTaskNumberComplete();
    _progressSubject->SendProgress();

	//���ݵ��豸��
	_progressSubject->SetCurrentTaskSum(100);
	_progressSubject->SendPrompt(MESSAGE_ARCHIVE_APP_ONPHONE);
	if (!ArchiveAppOnPhone(pDevInfo, strIdentifier, eMode))
		THROW_ASSISTANT_EXCEPTION(ARCHIVE_APP_ARCHIVE_ON_DEVICE_ERROR);//return false;

	_progressSubject->AddTaskProgress(100-_progressSubject->GetLastProgress()->_currentTaskProgress);
	_progressSubject->IncreaseTaskNumberComplete();
    _progressSubject->SendProgress();

	//���豸������PC
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

	//�������ɾ��
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

    // ipad������ʺϰ�װ��iphone�豸
    if (!pApp->_DeviceFamily.empty())
    {
        if (find(pApp->_DeviceFamily.begin(),pApp->_DeviceFamily.end(), L"1") == pApp->_DeviceFamily.end()
            && (_pDevInfoPtr->IsPhone() || _pDevInfoPtr->IsPod()))
            THROW_ASSISTANT_EXCEPTION(DeviceUnSupported);//return -402653056;//kAMDBundleVerificationFailedError
    }
	LOG->WriteDebug(L"InstallApp IosIPAOperationWithItunes DeviceFamily match " +  pApp->_AppName);

    // ֧�ֵĹ̼�
    if (!pApp->_minimumOSVersion.empty())
    {
        if (pApp->_minimumOSVersion.compare(_pDevInfoPtr->_deviceProductVersion) > 0)
            THROW_ASSISTANT_EXCEPTION(PRODUCT_VERSION_NOT_AGREE);//return -402653124;//�����Ҫ��̼�{0}���ϣ����ʺϵ�ǰ�̼�{1}
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
        return COMMAND_THREAD_OVER_LIMIT;   // ��֧�ָ���Ĳ���������

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
    RETURN_VALUE_IF(-1 == _progressId, COMMAND_THREAD_OVER_LIMIT); // ��֧�ָ���Ĳ���������

    _pDevInfoPtr        = pDevice->GetDevInfo();
    _pAfcSocketApp      = pDevice->GetDevInfo()->_ios_afcSocketApp;
    _pAfcHandleApp      = pDevice->GetDevInfo()->_ios_afcHandleApp;
    _progressSubject    = &subject;

    wstring tempDir = Path::GlobalTemp()+CFileOperation::GetGuidW()+L"\\";
    AppData* pAppInfo = AppManager::GetAppInfoByFile(strAppFullName, tempDir);// �Ƿ���Ҫ�ͷ�pAppInfo?
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
    Authorizor au(pDevice);    // ��Ȩ
    if (pAppInfo->_is91IPA)
    {
		LOG->WriteDebug(L"InstallApp Authorizor begin " +  pAppInfo->_AppName);
        RETURN_VALUE_IF(0 != CheckValideToAuthorize(Authorizor::IsValideToAuthorize(pDevice)), COMMAND_EXECUTE_SUCCESS_AuthorizorFaile);
        bool ret = au.Authorize(pAppInfo->_acountInfo, pAppInfo->_AppName);

		LOG->WriteDebug(L"InstallApp IsArchivedIPA begin " +  pAppInfo->_AppName);
        //�ϴ�info.ini�ļ���DocumentsĿ¼��
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
        //�ж�����ʹ�õķ���
        if (CFileOperation::GetFileSizeW(strAppFullName) > 1073741824 || //����1G
            CStrOperation::startsWith(deviceProductType, L"iphone1") ||   //һ��iPhone
            CStrOperation::startsWith(deviceProductType, L"iphone2") ||   //����iPhone
            CStrOperation::startsWith(deviceProductType, L"ipod1")   ||   //һ��Touch
            CStrOperation::startsWith(deviceProductType, L"ipod2")   ||   //����Touch    
            CStrOperation::startsWith(deviceProductType, L"ipod3")        //����Touch
            )
        {
			LOG->WriteDebug(L"InstallApp ItunesFastInstallApplication begin " +  strAppFullName);
            // �˴�����ʹ��iTunes�Ŀ��ٰ�װ����
            ret = ItunesFastInstallApplication(strAppFullName);
            isSecure = true;
			
        }
        else
        {
			LOG->WriteDebug(L"InstallApp FastInsallApplication begin " +  strAppFullName);
            // ����ʹ�������Լ����ļ��Ŀ��ٰ�װ����
            ret = FastInsallApplication(strAppFullName);
            isSecure = false;
        }

        if (ret != 0)
        {

            if (isSecure)
            {
				LOG->WriteDebug(L"InstallApp FastInsallApplication begin " +  strAppFullName);
                // �޸�Ϊֱ��ʹ�ÿ��ٰ�װ��ʧ�ܺ��ٵ����ϵİ�װ��ʽ
                ret = FastInsallApplication(strAppFullName);
            }
            else
            {
				LOG->WriteDebug(L"InstallApp ItunesFastInstallApplication begin " +  strAppFullName);
                // �޸�Ϊֱ��ʹ�ÿ��ٰ�װ��ʧ�ܺ��ٵ����ϵİ�װ��ʽ
                ret = ItunesFastInstallApplication(strAppFullName);
            }

			if (ret == -402620415 || ret == -402620395 || ret == -402636802)//-402636802 �豸��ֹ��װӦ��.// δԽ�����豸��װ�ƽ����������������,ֱ�ӷ��ز������԰�װ
				THROW_ASSISTANT_EXCEPTION(PRODUCT_PROHIBIT_APPINSTATLL);

            //�����,��ԭ�Ⱦɷ�ʽ���°�װһ��,������ΪһЩipa��ѹʧ�ܵ��°�װʧ��
            if (ret != 0)
                return false;//OldInstallApplication(strAppFullName, errorMsg);
        }
    }
    catch (...)
    { 
		if (ret == -402620415 || ret == -402620395 || ret == -402636802)//-402636802 �豸��ֹ��װӦ��.// δԽ�����豸��װ�ƽ����������������,ֱ�ӷ��ز������԰�װ
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
        bool inSelfUnzipMode = false;//91 �Լ��߽�ѹ���ϴ�����
        wstring deviceProductType = CStrOperation::toLowerW(_pDevInfoPtr->_deviceProductType);

        if (CFileOperation::GetFileSizeW(strAppFullName) < 10485760 && //С��10M�Ƚ��µĻ�����
            !CStrOperation::startsWith(deviceProductType, L"iphone1") &&   //һ��iPhone
            !CStrOperation::startsWith(deviceProductType, L"iphone2") &&   //����iPhone
            !CStrOperation::startsWith(deviceProductType, L"ipod1")   &&   //һ��Touch
            !CStrOperation::startsWith(deviceProductType, L"ipod2")   &&   //����Touch    
            !CStrOperation::startsWith(deviceProductType, L"ipod3")        //����Touch
            )
        {
            ret = InstallAppInZipMode(strAppFullName, errorMsg);//OLD
        }
        else if (_pDevInfoPtr->_connectType == USB && !IPAPackageHelper::IsArchivedIPA(strAppFullName))
        {
            //InstallAppInUnzipModeWithiTunesInterface �÷�����ʱ��֧�ְ�װ�д浵�ı���.
            //��Ҫ�����о���ز���
            ret = InstallAppInUnzipModeWithiTunesInterface(strAppFullName, errorMsg);//itunesFast
            isSecure = true;
        }
        else
        {
            //����ʹ�������Լ����ļ��Ŀ��ٰ�װ����
            ret = InstallAppInUnzipMode(strAppFullName, errorMsg);//fast
            inSelfUnzipMode = true;
        }

        if (ret != 0)
        {
            //����������Ҫ���԰�װ:
            //Note!!! �⼸�������������iTunes�汾�仯���仯
            //-402636802 �豸��ֹ��װӦ��.
            // -402653049,IPA������ļܹ����ýڵ�������.//kAMDIncorrectArchitectureError

            IPAOperationErrorType errorType = (IPAOperationErrorType)(ret);
            if (inSelfUnzipMode && errorType == ApplicationVerificationFailed)
            {
                // ��Щipa,����Twitter5.0.2.ipa,���ֱ߽�ѹ���ϴ�������У��ʧ��.
                // TODO:�Լ��߽�ѹ���ϴ����������.
                if (IPAPackageHelper::CheckPackageCopyRight(strAppFullName))
                    return ( InstallAppInZipMode(strAppFullName, errorMsg) == 0 );
            }

            //δԽ�����豸��װ�ƽ���������ApplicationVerificationFailed�������,ֱ�ӷ��ز������԰�װ
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

    //��װ�ļ�ʹ��GUID���֣��ɱ��Ⲣ�а�װʱ������
    wstring strTempName = L"foo_" + CCodeOperation::GB2312ToUnicode(CFileOperation::GetGuid()) + L".ipa";
    wstring strAppInstallFileOnPhone = L"/PublicStaging/" + strTempName;

    // ��ȡ��ʱĿ¼
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
			!CStrOperation::endsWithW(CStrOperation::toLowerW(strAppFullName), L".ipa")) //ֻ��װIPA�ļ�
			return false;

		 //ʹ��ITUNESĬ��APP�ϴ������������������iTunes_AMDeviceInstallApplication������ס�ϴ�����ļ����豸�ϵ�λ��
        void* cfString = CCFConverter::StringtoCFString(CCodeOperation::UnicodeToGB2312(strAppInstallFileOnPc));
        PROGRESS_CALLBACK1 pCallback = IosIPAItunesManager::GetInstance()->GetDataTransferCallBack(_progressId);
		if (0 == CITunesMobileDevice::iTunes_AMDeviceTransferApplication(_pAfcSocketApp, cfString, NULL, pCallback, NULL))
		{
			_progressSubject->IncreaseTaskNumberComplete();
			_progressSubject->SendProgress();

			//��װ�ļ�ֻ�贫�뱾�ؼ���
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
        //��װʧ�ܵ�ʱ��,��Ҫ�ж���afc����ʱ��down��,
        //δԽ�����豸��װ�ƽ����,�ᵼ��afcdown��.
        //RestartAFCServiceIfNeed(conn);
        fileStream.IsAFCConnectionValid();

        _progressSubject->ResetProgressData();
        _progressSubject->SetTaskNumberSum(2);
        _progressSubject->SetAllTaskSum(2*100);

        _progressSubject->SetCurrentTaskSum(100);
        _progressSubject->SendPrompt(MESSAGE_INSTALL_UPLOAD);		
        //�ô˷����ϴ���IPA��һ�������޷���װ
        // ��ӽ���ת��
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

            //��װ�ļ��贫���豸�ϵ�λ��
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

    // �ϴ��ļ�����������
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

    //��װ�ļ�ʹ��GUID���֣��ɱ��Ⲣ�а�װʱ������
    wstring strTempName = L"foo_" + CCodeOperation::GB2312ToUnicode(CFileOperation::GetGuid()) + L".ipa";
    wstring strAppInstallFileOnPhone = L"/PublicStaging/" + strTempName;
    if (fileStream.Ios_CopyToPhone(strAppFullName, strAppInstallFileOnPhone, progressSubjec) != Transfer_Success)
    {
        fileStream.Ios_DeleteFile(strAppInstallFileOnPhone);
        return -1;
    }

    // ��װ����������
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

            //��װ�ļ�ֻ�贫�뱾�ؼ���
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
        //�����ǻ�ȡ����ʧ����Ϣ,��װʧ���������ȡ
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

    // ��ӽ���ת��
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
        return -1;//����ʧ�ܷ���-1;
    }

    //_progressSubject->AddTaskProgress(100-_progressSubject->GetLastProgress()->_currentTaskProgress);
    // ? �˴ο����д�����Ϊ��������Ϊ200��ÿ����������100��������ܻᳬ�����������������ֵ
    _progressSubject->AddTaskProgress(200);
    _progressSubject->IncreaseTaskNumberComplete();
    _progressSubject->SendProgress();
    void* sockProxy = NULL;
    int iErr = usbConn.StartIOSService((char*)"com.apple.mobile.installation_proxy", &sockProxy);
    if (0 == iErr)
    {
        //��װ�ļ�ֻ�贫�뱾�ؼ���
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
            //��װʧ�ܵ�ʱ��,��Ҫ�ж���afc����ʱ��down��,
            //δԽ�����豸��װ�ƽ����,�ᵼ��afcdown��.
            //RestartAFCServiceIfNeed(usbConn);
            fileStream->IsAFCConnectionValid();

        //ɾ������װʧ�ܵİ�װ�ļ�
        if (0 != iErr && fileStream->Ios_Exists(ipaFolder))
            fileStream->Ios_DeleteFile(ipaFolder);

        //��ȡ������Ϣ
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

    // �ϴ��ļ�����������
    _progressSubject->ResetProgressData();
    _progressSubject->SetTaskNumberSum(2);
    _progressSubject->SetAllTaskSum(2*100);
    _progressSubject->SetCurrentTaskSum(100);
    _progressSubject->SendPrompt(MESSAGE_INSTALL_UPLOAD);

    // ��ӽ���ת��
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
        return -1;//����ʧ�ܷ���-1;
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

    //ɾ������װʧ�ܵİ�װ�ļ�
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

        // ��װ�ɹ�,�ж����Ƿ���Ҫ��ԭ�浵
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
        //��װʧ�ܵ�ʱ��,��Ҫ�ж���afc�����Ƿ�down��,
        //δԽ�����豸��װ�ƽ����,�ᵼ��afcdown��.
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

            // �ж��Ƿ������
            if(replyInfo.find("Error") != replyInfo.end())
            {
                errorMsg = replyInfo["Error"];
                break;
            }

            // �ж��Ƿ����
            if(replyInfo.find("Status") != replyInfo.end()
                && replyInfo["Status"] == "Complete")
            {
                res = true;
                break;
            }
			DWORD endRecive =	GetTickCount();
			if(endRecive - beginRecive > 60000)//1���ӳ�ʱ
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
    // �����������һЩ�����.��Ҫ��һЩ�����Ӧ�Ĵ������ڲ�ͬ�İ汾itunes���治һ��;
    // ���� kMISInstallProhibited  �豸��ֹ��װ ��

    string languageKey;
    string errorMsgDes;

    if (iErrCode == -402620415)
        languageKey = "kMISAdHocSignatureError";
    else if (iErrCode == -402620391)
        languageKey = "kMISAdHocSignatureError";
    //else if (iErrCode == -402636802)//�豸��ֹ��װ
    //    languageKey = "kMISInstallProhibited";
    else if (iErrCode == -402653056)
        languageKey = "kMISInstalAppFileLost";  //Ӧ�ó����������
    else if (iErrCode == -402653183)
        languageKey = "kMISInstalAppUnZipError";  //Ӧ�ó�����޷���ѹ

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
    // ���ж�AFC�����Ƿ�����
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

    // �쳣�Ļ������÷���
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

// ���ؽ����������,��SBMatchingApplicationGenres Ϊkey
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
    //temp.push_back("SBMatchingApplicationGenres");// ��ȡappType��Ӧ�Ѿ���װ�������������Ϣ
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
	default: //Ĭ��user
		pValue = CCFConverter::StringtoCFString("User");
		break;
	}

    map<string, void*> srcDict;
    srcDict.insert(pair<string, void*>("ApplicationType", pValue));

    // ���û�ȡ��װ����Ĵ���ʹ����Ϣ�ķ�����Ϣ
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

		// �豸�����Ӧ��Ŀ¼�ṹ��ͬ
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
        + IPA_APP_METADATA_FILE; // �ļ�����ڱ��ص�ȫ·��

	if (pFileStream->Ios_DownFromPhone(strMetadataFullName, strMetadataOnPc) == Transfer_Success)
	{
		// ��plist�ļ�ת��Ϊxml�ļ�������
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