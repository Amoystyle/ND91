#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "IosIPAAppOperation.h"
#include "IosIPAOperationWithItunes.h"
#include "IosIPAOperationWithDaemon.h"
#include "IosPXLAppOperation.h"
#include "Module/Application/AppManager.h"
#include "Common/Path.h"

IosIPAAppOperation::IosIPAAppOperation(const DeviceData* pDevice, ProgressSubject& subject)
:IIosAppOperation(pDevice, subject)
{
}

IosIPAAppOperation::~IosIPAAppOperation(void)
{
}

int IosIPAAppOperation::GetAppCount(APP_OWNER appOwner)
{
	if (!_pDevInfo)
		return 0;

	//USB�����£����Ȳ���ITUNES��ȡ����Ϊ���ֹ�ж��һЩ�����,�����ipalist�ļ�û���������
	if (_pDevInfo->_connectType == USB)
	{
		IosIPAOperationWithItunes itunesOp;
		return itunesOp.GetAppCount(_pDevInfo->_ios_devptr, appOwner);
	}
	else
	{
		if ( _pDevInfo->_ios_bJailBreaked && _pDevInfo->_ios_daemonInstalled )
		{
			IosIPAOperationWithDaemon op(_pDevData, *_pProgressSubject);
			return op.GetAppCount(appOwner);
		}
	}

	return 0;
}

bool IosIPAAppOperation::GetAppInfos(vector<AppData*> &vtInfos)
{
	if (!_pDevInfo)
		return false;

	bool bRet = false;

	//USB�����£����Ȳ���ITUNES��ȡ����Ϊ���ֹ�ж��һЩ�����,�����ipalist�ļ�û���������
	if (_pDevInfo->_connectType == USB)
	{
		IosIPAOperationWithItunes itunesOp;
		bRet = itunesOp.GetAppInfos((DeviceInfo*)_pDevInfo, APP_OWNER_USER, vtInfos);

		if (!bRet && _pDevInfo->_ios_bJailBreaked && _pDevInfo->_ios_daemonInstalled)
		{
			if (_pDevData && _pDevData->GetIosDeamonMode() == 0)
				return false;

			IosIPAOperationWithDaemon op(_pDevData, *_pProgressSubject);
			bRet = op.GetAppInfos(vtInfos);
		}
	}
	else
	{
		if ( _pDevInfo->_ios_bJailBreaked && _pDevInfo->_ios_daemonInstalled )
		{
			IosIPAOperationWithDaemon op(_pDevData, *_pProgressSubject);
			bRet = op.GetAppInfos(vtInfos);
		}
	}

	return bRet;
}

bool IosIPAAppOperation::GetAppDiskUsage(map< wstring, vector<int64_t> >& diskUsages)
{
    IosIPAOperationWithItunes itunesOp;
    return itunesOp.GetAppDiskUsage((DeviceInfo*)_pDevInfo, diskUsages);
}

bool IosIPAAppOperation::GetAppIconPhonePath(AppData* pApp)
{
    IosIPAOperationWithItunes itunesOp;
    return itunesOp.GetAppIconPhonePath((DeviceInfo*)_pDevInfo, pApp);
}

bool IosIPAAppOperation::GetAppPurchaser(AppData* pApp)
{
    IosIPAOperationWithItunes itunesOp;
    return itunesOp.GetAppPurchaser((DeviceInfo*)_pDevInfo, pApp);
}

CommandExecuteResult IosIPAAppOperation::InstallApp(wstring strAppFullName, string& errorMsg,App2ExeInfo* exeInfo)
{
	if (!_pDevInfo)
		return COMMAND_EXECUTE_FAIL;

	int nVerNum = IosPublicFunc::GetVerNum(_pDevInfo->_deviceProductVersion);

	//2.x���ϰ汾��֧��
	if (nVerNum < 200)
		THROW_ASSISTANT_EXCEPTION(PRODUCT_VERSION_NOT_AGREE);//return false;

    /*
	bool bInstallAppSync = true; //�Ƿ�װ��APPSYNC��Խ�����ж�
	//�����жϵ�ǰ�����ֻ��Ƿ��Ѿ����ڿ��԰�װAppSync����
	if (nVerNum >= 300 && _pDevInfo->_ios_bJailBreaked)
		bInstallAppSync = IsInstallAppSync(nVerNum);

	//û�а�װ��ֱ����ʧ�ܴ���
	if (!bInstallAppSync)
		THROW_ASSISTANT_EXCEPTION(IOS_UNINSTALL_APPSYNC);
    */

    // ����Ҫ��װAPPSYNC������Ҫ����ָ�����ļ���
    wstring strAppSync = L"/var/mobile/appsync/";
    wstring strTemp = L"/private/var/mobile/tdmtanf";
    if (!_pFileStream->Ios_Exists(strAppSync))
        _pFileStream->Ios_CreateDirectory(strAppSync);
    if (!_pFileStream->Ios_Exists(strTemp))
        _pFileStream->Ios_CreateDirectory(strTemp);

	//��װǰ��ж�ؾɳ���
	//wstring strIdentifier = GetIpaAppID(strAppFullName);// ����Ҫж�أ���ȡstrIdentifier�ǳ���ʱ
	//UnInstallApp(strIdentifier);

	if (_pDevInfo->_connectType == USB && exeInfo == 0)
	{
		LOG->WriteDebug(L"InstallApp IosIPAOperationWithItunes begin " +  strAppFullName);
		IosIPAOperationWithItunes itunesOp;
        return itunesOp.InstallApp(_pDevData, strAppFullName, errorMsg, *_pProgressSubject);
	}
	else if(exeInfo)
	{
		IosIPAOperationWithItunes itunesOp;
		CommandExecuteResult ret = 	itunesOp.InstallAppEx(_pDevData,strAppFullName, errorMsg,*_pProgressSubject,exeInfo);
		if( COMMAND_EXECUTE_FAIL == ret)
		{
			IosIPAOperationWithDaemon op1(_pDevData, *_pProgressSubject);
			return op1.InstallApp(strAppFullName, errorMsg,exeInfo)
                ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
		}	
		return ret;
		
	}
	else if (nVerNum >=300 && _pDevInfo->_ios_bJailBreaked && _pDevInfo->_ios_daemonInstalled )
	{
		IosIPAOperationWithDaemon op(_pDevData, *_pProgressSubject);
		return op.InstallApp(strAppFullName, errorMsg,NULL)
            ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
	}

	return COMMAND_EXECUTE_FAIL;
}

//���Ϊ�ػ���ж��
bool IosIPAAppOperation::UnInstallApp(wstring strIdentifier)
{
	if (!_pDevInfo)
		return false;

	if (strIdentifier.length() <= 0 || 
		strIdentifier == DAEMON_APP_ID_ONE || 
		strIdentifier == DAEMON_APP_ID_TWO)
		return false;

	int nVerNum = IosPublicFunc::GetVerNum(_pDevInfo->_deviceProductVersion);

	if (_pDevInfo->_connectType == USB)
	{
		IosIPAOperationWithItunes itunesOp;
		return itunesOp.UnInstallApp(_pDevInfo, strIdentifier);
	}
	else if (nVerNum >=200 && _pDevInfo->_ios_bJailBreaked && _pDevInfo->_ios_daemonInstalled )
	{
		IosIPAOperationWithDaemon op(_pDevData, *_pProgressSubject);
		return op.UnInstallApp(strIdentifier);
	}

	return false;
}

CommandExecuteResult IosIPAAppOperation::ArchiveApp(wstring strIdentifier, wstring strAppPhonePath, wstring strAppName, wstring strVersion, eArchiveMode eMode, wstring strFileOnPc)
{
	if (!_pDevInfo)
		return COMMAND_EXECUTE_FAIL;

	if (strIdentifier.length() <= 0 ||
		strAppPhonePath.length() <= 0 ||
		strFileOnPc.length() <= 0 )
		return COMMAND_EXECUTE_FAIL;

	if (_pDevInfo->_connectType == USB)
	{
		IosIPAOperationWithItunes itunesOp;
		return itunesOp.ArchiveApp(_pDevInfo, 
			strIdentifier, eMode, strFileOnPc, *_pProgressSubject);
	}
	else
	{
		IosIPAOperationWithDaemon op(_pDevData, *_pProgressSubject);
		return op.ArchiveApp(strIdentifier, strAppPhonePath, strAppName, strVersion, eMode, strFileOnPc)
            ? COMMAND_EXECUTE_SUCCESS : COMMAND_EXECUTE_FAIL;
	}
}

bool IosIPAAppOperation::IsInstallAppSync(int nVerNum)
{
    wstring strAppSync = L"/var/mobile/appsync/";
	if (_pFileStream->Ios_Exists(strAppSync) 
        && _pFileStream->Ios_Exists(L"/usr/bin/patchsync"))
		return true;

    wstring strTemp = L"/private/var/mobile/tdmtanf";
    if (nVerNum >= 400 && _pFileStream->Ios_Exists(strTemp))
        return true;

    if (nVerNum >= 400 && !_pFileStream->Ios_Exists(strAppSync))
        _pFileStream->Ios_CreateDirectory(strAppSync);

	if (nVerNum >= 400 && !_pFileStream->Ios_Exists(strTemp))
	{
        if (_pFileStream->Ios_CreateDirectory(strTemp))
            return true;
	}

	wstring strAppSyncFile = L"/usr/libexec/installd";
	if (_pFileStream->Ios_Exists(strAppSyncFile + L".bak") || _pFileStream->Ios_Exists(strAppSyncFile + L".backup"))
		return true;

	return false;
}

wstring IosIPAAppOperation::GetIpaAppID(wstring strIpaFullName)
{
    if (!_pDevData)
        return L"";
	wstring strDaemonFolderTemp = _pDevData->GetPath()->GetLocalAppPath()
	    + CCodeOperation::GB2312ToUnicode(CFileOperation::GetGuid());

	CFileOperation::UnZipFile(strIpaFullName, strDaemonFolderTemp);

    wstring strID = L"";
	vector<wstring> vt;
	CFileOperation::GetMatchFileFullName(strDaemonFolderTemp, L"Info.plist", vt);
	if (vt.size() > 0)
	{
		wstring strPlist = vt.at(0);	
		if (IosPlistConverter::GetXmlFromPlist(strPlist, strPlist))
		{
			IosPlistHelper xmlHelper(strPlist, eXmlType_UTF8);
			XML_NODE_INDEX index;
			//index.pNext = new XML_NODE_INDEX;
			index.AddChild(0);
			strID = xmlHelper.SearchKeyValue(index, "CFBundleIdentifier");
		}	
	}

	CFileOperation::DeleteDirectory(strDaemonFolderTemp);

	return strID;
}
#endif