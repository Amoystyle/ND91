#include "pch_module.h"

#if defined( MODULE_APP )

#include "Module/Application/InstallCommand.h"
#include "Module/Application/AppData.h"
#include "Module/Application/NpkData.h"

#include "IosApp/IosAppOperationFactory.h"
#include "Module/Common/AndroidXMLParser/AndroidXMLOperation.h"
#include "Device/AdbSetchmod.h"
#include "Device/AdbUninstallAppCommand.h"
#include "Device/AdbInstallAppCommand.h"
#include "Common/Path.h"
#include "Module/AndroidDefine.h"
#include "Module/File/GetSDCardStateCommand.h"
#include "Module/Application/AppManager.h"
#include "Core/DeviceManager.h"

#define  ANDROIDPRODUCTVERSIONL      22          // ANDROID�豸�Ͱ汾�͸߰汾�ֽ��

InstallCommand::InstallCommand(const DeviceData* pDevice, const AppData* appData)
: BaseCommand(pDevice)
{
	_bLock = false;	
	_bRecover = true;	
	_bOnSDCard = 3;
	_step = NONE;
	_appData = appData;
	//Android
	_nAndroidChannelID = APPLICATION;
	_nAndroidCommandID = INSTALL_APP;
	_exeInfo = NULL;
}

InstallCommand::~InstallCommand()
{

}

void ND91Assistant::InstallCommand::Execute()
{
	if (!_pDevice || !_pDevice->GetDevInfo())
		return ;
	RETURN_IF(!_appData);

    SendPrompt(MESSAGE_INSTALL_PREPARE);
    SendProgress();
	if (_pDevice->GetDevInfo()->_deviceType == Android)
	{
		if (_appData->_eAppType == APP_TYPE_APK)
			_success = InstallApk(_appData->_strAppPCPath, _appData->_packageName);
		else if (_appData->_eAppType == APP_TYPE_NPK)
			_success = InstallNpk(_appData->_strAppPCPath);
		else if (_appData->_eAppType == APP_TYPE_EXE)
		{
			if(_exeInfo->AppInExeType == L"apk")
				_success = InstallApkInExe(_appData->_strAppPCPath,_exeInfo->OffsetAppInExe,_exeInfo->len);
			//if(_exeInfo->AppInExeType == L"npk")
			//	_success = InstallNpkEx(_appData->_strAppPCPath,_exeInfo->OffsetAppInExe,_exeInfo->len);
		}

        _result = _success ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
	}
	else if (_pDevice->GetDevInfo()->_deviceType == Ios)
	{
		
        // ��װ����ǰ�ж��豸�Ƿ����ý�ֹ��װ����
        if (_pDevice->IsProhibitAppInstall())
            THROW_ASSISTANT_EXCEPTION(PRODUCT_PROHIBIT_APPINSTATLL);

		LOG->WriteDebug(L"InstallApp IsProhibitAppInstall " +  _appData->_AppName);
        // ��Խ���豸���ܰ�װ������Ӧ��
        if (!_pDevice->GetDevInfo()->_ios_bJailBreaked 
            && _appData->_isLegalSoft != 1 && !_appData->_isEnterpriseApp)
            THROW_ASSISTANT_EXCEPTION(NoJailBreakedNoLegalSoft);

		LOG->WriteDebug(L"InstallApp NoJailBreakedNoLegalSoft " +  _appData->_AppName);
        // ��Խ���豸�ݲ��ṩ��װӦ�ù���
//         if (!_pDevice->GetDevInfo()->_ios_bJailBreaked)
//             THROW_ASSISTANT_EXCEPTION(NoJailBreakedUnSupported);

		 IIosAppOperation* pOperation = NULL;
		 //appinexe�İ�װ����
 		if(_appData->_eAppType == APP_TYPE_EXE)
 		{
 			if(_exeInfo->AppInExeType == L"ipa")
				pOperation = IosAppOperationFactory::GetAppOperation(_pDevice, APP_TYPE_IPA, *this);
 			else if(_exeInfo->AppInExeType == L"deb")
				pOperation = IosAppOperationFactory::GetAppOperation(_pDevice, APP_TYPE_DEB, *this);
 			else if(_exeInfo->AppInExeType == L"pxl")
				pOperation = IosAppOperationFactory::GetAppOperation(_pDevice, APP_TYPE_PXL, *this);
 		}
		else
			pOperation = IosAppOperationFactory::GetAppOperation(_pDevice, _appData->_eAppType, *this);

		LOG->WriteDebug(L"InstallApp GetAppOperation " +  _appData->_AppName);
		if (pOperation)
		{
			string errmsg;
			_result = pOperation->InstallApp(_appData->_strAppPCPath, errmsg,_exeInfo); 
			_errorMsg= CCodeOperation::UTF_8ToUnicode(errmsg);

            _success =  (_result == COMMAND_EXECUTE_SUCCESS || _result == COMMAND_EXECUTE_SUCCESS_AuthorizorFaile);
			delete pOperation;
		}
		pOperation = NULL;
	}
	Complete();
}


#pragma region Android
bool ND91Assistant::InstallCommand::InstallApkInExe(wstring strPCPath,unsigned __int64 offsetAppInExe,unsigned __int64 len)
{
	if (IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion) < ANDROIDPRODUCTVERSIONL && _bOnSDCard == 1)
		_bOnSDCard = 2;
	//exe�ļ�û�취ȡ����װ����Ϣֱ����·��������
	wstring strPackageName = CStrOperation::ReplaceW(strPCPath,L"\\",L"."); 
	strPackageName = CStrOperation::ReplaceW(strPackageName,L":",L".");
	if (_pDevice->GetDevInfo()->_connectType == WIFI)
	{
		_appRemotePath = ANDROID_APP_INSTALL_DIR_DEMON;
		_appRemotePath += strPackageName + L".apk";
	}
	else if (_pDevice->GetDevInfo()->_connectType == USB)
	{
		_appRemotePath = ANDROID_APP_INSTALL_PATH;	//��װ����Ҫ�ϴ����ֻ��е�·��
	}
	 FileHelper::Delete(_pDevice, _appRemotePath);
	_step = UPLOAD;

	UploadFileCommand uploadCmd(_pDevice);

	ProgressObserver* pObserver = NULL;
	if (!_observers.empty())
		pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);
	uploadCmd.Attach(pObserver);	
	uploadCmd.SendPrompt(MESSAGE_INSTALL_UPLOAD);
	uploadCmd.SetTaskNumberSum(1);
	uploadCmd.SetAllTaskSum(CFileOperation::GetFileSizeW(strPCPath));
	uploadCmd.setfilename(strPCPath, _appRemotePath);
	uploadCmd.SetParam(offsetAppInExe,len);
	uploadCmd.Execute();
	uploadCmd.IncreaseTaskNumberComplete();
	uploadCmd.SendProgress();
	_success = uploadCmd.GetUploadResult() == Transfer_Success;

	if ( !_success )
		LOG->WriteDebug(L"uploadApk : Fail");

	//Step 2 ִ�а�װ����
	if (_success)
	{
		SendPrompt(MESSAGE_INSTALL_INSTALL);
		SendProgress();
		_success = InstallRemoteApk(_appRemotePath, strPackageName);
	}
	_result = _success ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
	return _success;
}

bool ND91Assistant::InstallCommand::InstallApk(wstring strPCPath, wstring strPackageName)
{
    if (IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion) < ANDROIDPRODUCTVERSIONL && _bOnSDCard == 1)
        _bOnSDCard = 2;

    //ApkPackageInfo info = AndroidXMLOperation::GetPackageInfoFromApk(strPCPath);
	wstring PackageName = (strPackageName.empty()?L"pkg":strPackageName);
	PackageName+=L"."+CFileOperation::GetGuidW();
	bool sdcardexist=true;
	if (_pDevice->GetDevInfo()->_connectType == WIFI)
	{
		_appRemotePath = ANDROID_APP_INSTALL_DIR_DEMON;
        _appRemotePath += strPackageName + L".apk";
	}
	else if (_pDevice->GetDevInfo()->_connectType == USB)
	{
		sdcardexist=GetSDCardStateCommand::IsSDCardExist(_pDevice);
		if(_bOnSDCard==1&&sdcardexist)
			_appRemotePath=L"/sdcard/nd/";
		else
			_appRemotePath = ANDROID_APP_INSTALL_DIR;	//��װ����Ҫ�ϴ����ֻ��е�·��
		//if (strPackageName.empty())
		//	_appRemotePath+=L"pkg.apk";
		//else
			_appRemotePath+=PackageName+L".apk";
	}
	FileHelper::Delete(_pDevice, _appRemotePath);

	_step = UPLOAD;
	//Step 1 �ϴ��ļ����ֻ���
	UploadFileCommand uploadCmd(_pDevice);
	// ��ӽ���ת��
	//AppProgressObserver pObserver((ProgressSubject*)this);
    ProgressObserver* pObserver = NULL;
    if (!_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);
	uploadCmd.Attach(pObserver);
    uploadCmd.SendPrompt(MESSAGE_INSTALL_UPLOAD);
	int count=0;
	while(count<2)
	{
		uploadCmd.SetTaskNumberSum(1);
		uploadCmd.SetAllTaskSum(CFileOperation::GetFileSizeW(strPCPath));
		uploadCmd.setfilename(strPCPath, _appRemotePath);
		uploadCmd.Execute();
		uploadCmd.IncreaseTaskNumberComplete();
		uploadCmd.SendProgress();
		_success = uploadCmd.GetUploadResult() == Transfer_Success;
		if(_success)break;
		if(_appRemotePath.find(ANDROID_APP_INSTALL_DIR)!=wstring::npos&&sdcardexist)
			_appRemotePath=CStrOperation::ReplaceW(_appRemotePath,ANDROID_APP_INSTALL_DIR,L"/sdcard/nd/");
		else if(_appRemotePath.find(L"/sdcard/nd/")!=wstring::npos)
			_appRemotePath=CStrOperation::ReplaceW(_appRemotePath,L"/sdcard/nd/",ANDROID_APP_INSTALL_DIR);
		else
			break;
		FileHelper::Delete(_pDevice, _appRemotePath);
		count++;
	}

	if ( !_success )
	{
		LOG->WriteDebug(L"uploadApk : Fail");
		_errorMsg=L"�ϴ��ļ�ʧ��";
	}

	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_pDevice->GetId());
	if(pDevice == NULL)
	{
		return false;
	}
	
	//Step 2 ִ�а�װ����
	if (_success)
	{
		SendPrompt(MESSAGE_INSTALL_INSTALL);
        SendProgress();
		_success = InstallRemoteApk(_appRemotePath, strPackageName);
	}

	//Step 3 ɾ���ļ�
// 	if (_success)
// 		FileHelper::Delete(_pDevice, _appRemotePath);

	_result = _success ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
    return _success;
}

bool ND91Assistant::InstallCommand::InstallNpk(wstring strPCPath)
{
    bool bRec = true;
	wstring strXMLPath; //NPK��XML·��
	wstring strNPKPath;	//NPK������Ŀ¼

	wstring strTempPath;
	OPTION->Get(GlobalOption::STR_ANDROID_NPK_TEMP_PATH, strTempPath);
	//��ѹNPK
	SendPrompt(MESSAGE_INSTALL_PREPARE); //׼�����ݵ�ʱ�� ��ʱ��û�н�����
    SendProgress();
	strTempPath += CFileOperation::GetFileNameNoExt(strPCPath);    
	strTempPath += WSTRING_PATH_SEPARATER;
	strXMLPath = strTempPath + L"npk.xml";
	CFileOperation::UnZipFile(strPCPath, strTempPath);

	//����xml	
	string str = CCodeOperation::UTF_8ToGB2312(CFileOperation::ReadFile(strXMLPath, false));
	NpkData npkData(str);
// 	// ���������ܽ���ֵ
// 	int nTaskNum = 0;
// 	for (vector<NPK_ITEM*>::iterator it = npkData._items.begin(); it != npkData._items.end(); it++)
// 	{
// 		nTaskNum += 2; //ÿ��װһ��APK���2��
// 		nTaskNum +=	(*it)->datas.size();
// 	}
// 	SetTaskNumberSum(nTaskNum);
// 	SetAllTaskSum(nTaskNum * 100);
//     SendProgress();

	for (vector<NPK_ITEM*>::iterator it = npkData._items.begin(); it != npkData._items.end(); it++)
	{
		NPK_ITEM* pItem = (NPK_ITEM*)(*it);
		//��װAPK
		strNPKPath = strTempPath + pItem->Name + WSTRING_PATH_SEPARATER;
		if (!InstallApk(strNPKPath + pItem->apkFileName, pItem->packageName))
        {
            bRec = false;
			break;
        }
		
		SendPrompt(MESSAGE_INSTALL_COPYDATA);
        SendProgress();
		for (vector<NPK_CopyData*>::iterator it2 = pItem->datas.begin(); it2 != pItem->datas.end(); it2++)
		{
			NPK_CopyData* pData = (NPK_CopyData*)(*it2);

            // ��ȡ�豸�ϵ�Ŀ¼�ڵ�
            DirData destDir(_pDevice);
            destDir._name = pData->remotePath + L"/";

            ProgressObserver* pObserver = NULL;
            if (!_observers.empty())
                pObserver = dynamic_cast<ProgressObserver*>(_observers[0]);

            wstring sourceName = strNPKPath + pData->localName;
			if (pData->bFolder)
			{
                if (! destDir.UploadDir(sourceName, pObserver) != Transfer_Success)
                    break;
			}
			else
			{
                vector<wstring> uploadList;
                uploadList.push_back(sourceName);

                if (! destDir.UploadFile(uploadList, pObserver) != Transfer_Success)
                    break;
			}
		}
	}
	if(npkData._items.empty())
		bRec = false;
	return bRec;
}

bool ND91Assistant::InstallCommand::InstallRemoteApk(wstring strPhonePath, wstring strPkgName)
{
	bool bRec = false;
	if (_pDevice->GetDevInfo()->_connectType == WIFI)
	{
		// wifi ͨ������������ػ���ִ�а�װ
		DefaultExecute();
        if (_step == FAIL && !_isMove)
        {
            _bOnSDCard = !_bOnSDCard;
            // ����
            DefaultExecute();
        }
		bRec = _success;
	}
	else
	{
		// Usb ģʽͨ��Adb Socket ��ִ�а�װ 
		// Usb ģʽҲ������wifi��ʽ����װ��������adb�и��ߵ�Ȩ�ޣ��������Ȳ��ø÷�ʽ
		// ��Ȩ

		wstring sn = _pDevice->GetId();

		DeviceData* pDevice = DEVICE_MANAGER->FindDevice(sn);
		if(pDevice == NULL)
		{
			return false;
		}

		AdbSetchmod setchmodCmd2(sn, strPhonePath);
		setchmodCmd2.Execute();
		bRec = setchmodCmd2.Success();

		//ж��ԭ����
		if (strPkgName.length() > 0 && !_isMove)
		{				
			AdbUninstallAppCommand uninstallCmd(sn, strPkgName, false);
			uninstallCmd.Execute();
			//bRec = uninstallCmd.Success(); //ж��ʧ��
		}

		//��װ����
		//if (bRec)
		//{
			LOG->WriteDebug(L"isABDinstall : yes");
			//EnterCriticalSection(&_pDevice->_csAndroidCommandLock);
			AdbInstallAppCommand installCmd(sn, strPhonePath, _bLock, _bRecover, _bOnSDCard == 1);
			installCmd.Execute();
			bRec = installCmd.Success();
			_errorMsg=installCmd.GetInstallFailError();
		    if (!bRec && !_isMove)
		    {
				LOG->WriteDebug(L"firstInstall : fail");
			    AdbInstallAppCommand installCmd2(sn, strPhonePath, _bLock, _bRecover, _bOnSDCard == 1);
			    installCmd2.Execute();
			    bRec = installCmd2.Success();
				_errorMsg=installCmd2.GetInstallFailError();
				if ( bRec )
					LOG->WriteDebug(L"secondInstall : success");	
				else
				{
					LOG->WriteDebug(L"secondInstall : fail");
					  AdbInstallAppCommand installCmd3(sn, strPhonePath, _bLock, _bRecover, !(_bOnSDCard == 1));
					  installCmd3.Execute();
					  bRec = installCmd3.Success();						
				}
		    }
		//}
		_step = FINISH;
		FileHelper::Delete(_pDevice, strPhonePath);
	} 
	return bRec;
}
// Install App With Wifi
std::string ND91Assistant::InstallCommand::GetAndroidBodyData()
{
	BinaryBufferWriter writer;
    string pkgName = CCodeOperation::UnicodeToUTF_8(CFileOperation::GetFileName(_appRemotePath));

	writer.WriteInt(_nAndroidChannelID);
	writer.WriteChar((char)_nAndroidCommandID);
	writer.WriteString(CCodeOperation::UnicodeToUTF_8(_appRemotePath));
	writer.WriteBool(_bLock);
	writer.WriteBool(_bRecover);
	//writer.WriteInt(_bOnSDCard);
    writer.WriteInt(3);
	writer.WriteInt( pkgName == ADB_DEMON_PACKAGE_NAME ? 2 : 1);

	return writer.GetBuffer();
}

void ND91Assistant::InstallCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead(&reader))
	{
		_step = FAIL;
		return;
	}
	string strResult;
	reader.ReadString(strResult);
	if (strResult == "WaitForConfirm")
		_step = CONFIRM;
	else
		_step = FINISH;

}

bool ND91Assistant::InstallCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
{
	pReader->ReadBuffer(_pReturnHead, 4);
	pReader->ReadShort(_nReturnFlag);
	pReader->ReadShort(_nReturnLongConnectSign);
	pReader->ReadInt32(_nReturnBodySize);
	pReader->ReadInt32(_nReturnChannelID);
	//������ص����ݰ���Ϣ�����ϱ�Command����Ϣ��������
	if (_nReturnChannelID != _nAndroidChannelID) // || _nReturnCommonType != _nAndroidCommandID)
		return false;
	string strOkay;
	pReader->ReadString(strOkay, 4);
	if (strOkay != ANDROID_PACKAGE_OKAY)
		return false;
	return true;
}

bool ND91Assistant::InstallCommand::SimpleInstallApp( const DeviceData* pDevice, wstring strAppPath, wstring strPackageName, APP_TYPE appType)
{
	AppData* pData = new AppData();
	pData->_strAppPCPath = strAppPath;
	pData->_packageName  = strPackageName;
	pData->_eAppType = APP_TYPE_APK;

	InstallCommand installCmd(pDevice, pData);
	installCmd.Execute();

	SAFE_DELETE(pData);

	return installCmd.Success();
}
#pragma endregion
#endif