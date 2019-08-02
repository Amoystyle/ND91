#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "Core/DeviceManager.h"
#include "Module/Application/IosApp/IIosAppOperation.h"
#include "Module/System/IosNotifySBCommand.h"
#include "Device/IosDaemon.h"
#include "Common/Path.h"
#include "Device/AFCFileSysemBase.h"
#include "Device/IosWiFiFileSystem.h"

IIosAppOperation::IIosAppOperation(const DeviceData* pDevice, ProgressSubject& subject)
{
    _pDevData = pDevice;
	_pDevInfo = pDevice->GetDevInfo();

	_pProgressSubject = &subject;
	_pFileStream = NULL;

	if (_pDevInfo->_connectType == USB)
		_pFileStream = new AFCFileSysemBase((DeviceInfo*)_pDevInfo);
	else
		_pFileStream = new IosWiFiFileSystem(_pDevInfo->_ios_wifi_handle);
}

IIosAppOperation::~IIosAppOperation(void)
{
	if (_pFileStream)
		delete _pFileStream;
	_pFileStream = NULL;
}

void IIosAppOperation::NotifySB(int nTimeOut)
{
	IosNotifySBCommand* pNotifyCmd = new IosNotifySBCommand(_pDevData, nTimeOut*1000);
	pNotifyCmd->Execute();
	delete pNotifyCmd;

	//��iPad���豸�Ͻ���PXL�����װ����ͼ���޸������Ժ�
    // com.apple.mobile.installation.plist�ļ�Ȩ�޻���root.600
	//Ŀǰû���ҵ���ɴ������ԭ���ڴ˴�����һ��Ȩ��������
	if (_pDevInfo->_ios_devicebTurnCypher)
	{
		/*SetAttributeCommand cmd(
            _pDevData,
            IPA_APP_INFO_FILE,
			L"644",     // Ȩ��: rw-r--r--
            L"501");    // �û�: mobile
		cmd.Execute();*/
		DirectoryOperationCommand cmd(_pDevData);
		cmd.ChangeMode(IPA_APP_INFO_FILE, "644", L"501");
	}
}

bool IIosAppOperation::InstallAppService(APP_TYPE eType)
{
	if (!_pFileStream)
		return false;

	wstring strCmdFileOnPC = GetInstallCmdFileOnPC(eType);
	wstring strCmdFileOnPhone = GetCmdFileOnPhone(eType);
	wstring strBackFile = GetBackFileOnPhone(eType);
	wstring strFailFile = GetFailFileOnPhone(eType);

	return ExcuteAppService(strCmdFileOnPC, strCmdFileOnPhone, strBackFile, strFailFile);
}

bool IIosAppOperation::UnInstallAppService(APP_TYPE eType, wstring strIdentifier)
{
	if (!_pFileStream)
		return false;

	wstring strCmdFileOnPC = GetUnInstallCmdFileOnPC(eType, strIdentifier);
	wstring strCmdFileOnPhone = GetCmdFileOnPhone(eType);
	wstring strBackFile = GetBackFileOnPhone(eType);
	wstring strFailFile = GetFailFileOnPhone(eType);

	return ExcuteAppService(strCmdFileOnPC, strCmdFileOnPhone, strBackFile, strFailFile);
}

bool IIosAppOperation::ExcuteAppService(wstring strCmdFileOnPC, wstring strCmdFileOnPhone, wstring strBackFile, wstring strFailFile)
{
	bool bRet = false;
	if (_pFileStream->Ios_CopyToPhone(strCmdFileOnPC, strCmdFileOnPhone, *_pProgressSubject) == Transfer_Success)
	{
		unsigned long timeout = 180*1000; //180��ȴ�ʱ��
		unsigned long lFirstTick = GetTickCount();
		unsigned long lNextTick = 0;	

		// ѭ���ȴ�����ķ��ؽ��
		do 
		{
			Sleep(100);

			if (!_pFileStream->Ios_Exists(strBackFile) 
                && !_pFileStream->Ios_Exists(strCmdFileOnPhone) 
                && (strFailFile.length() <= 0 || !_pFileStream->Ios_Exists(strFailFile)))
			{
				bRet = true;
				break;
			}

			lNextTick = GetTickCount();

		} while ((lNextTick-lFirstTick) < timeout);	
	}

	return bRet;
}

wstring IIosAppOperation::GetCmdFileOnPhone(APP_TYPE eType)
{
	wstring strCmdFile = L"";

	IosDaemon daemon(NULL, L"");
	switch (eType)
	{
	case APP_TYPE_IPA:
		{			
			wstring strName = daemon.CreatDaemonCypher(IPA_OPERATION_COMMAND_NAME, _pDevInfo->_ios_devicebTurnCypher, 
				_pDevInfo->_ios_deviceszDaemonCypherBytes);
			strCmdFile = IPA_OPERATION_COMMAND_FOLDER + strName;
		}
		break;
	case APP_TYPE_PXL:
		{
			wstring strName = daemon.CreatDaemonCypher(PXL_OPERATION_COMMAND_NAME, _pDevInfo->_ios_devicebTurnCypher, 
				_pDevInfo->_ios_deviceszDaemonCypherBytes);
			strCmdFile = PXL_OPERATION_COMMAND_FOLDER + strName;
		}
		break;
	case APP_TYPE_DEB:
		{
			wstring strName = daemon.CreatDaemonCypher(DEB_OPERATION_COMMAND_NAME, _pDevInfo->_ios_devicebTurnCypher, 
				_pDevInfo->_ios_deviceszDaemonCypherBytes);
			strCmdFile = DEB_OPERATION_COMMAND_FOLDER + strName;
		}
		break;
    case APP_TYPE_APPSYNC:
        {
            strCmdFile = APPSYNC_OPERATION_COMMAND_FOLDER;
            strCmdFile += DEB_OPERATION_COMMAND_NAME;
        }
        break;
	}

	return strCmdFile;
}

wstring IIosAppOperation::GetBackFileOnPhone(APP_TYPE eType)
{
	wstring strBackFile = L"";

	switch (eType)
	{
	case APP_TYPE_IPA:
		{
			strBackFile = IPA_OPERATION_COMMAND_FOLDER;
			strBackFile += IPA_OPERATION_COMMAND_NAME;
		}
		break;
	case APP_TYPE_PXL:
		{
			strBackFile = PXL_OPERATION_COMMAND_FOLDER;
			strBackFile += PXL_OPERATION_COMMAND_NAME;
		}
		break;
	case APP_TYPE_DEB:
		{
			strBackFile = DEB_OPERATION_COMMAND_FOLDER;
			strBackFile += DEB_OPERATION_COMMAND_NAME;
		}
		break;
    case APP_TYPE_APPSYNC:
        {
            strBackFile = APPSYNC_OPERATION_COMMAND_FOLDER;
            strBackFile += DEB_OPERATION_COMMAND_NAME;
        }
        break;
	}

	return strBackFile;
}

wstring IIosAppOperation::GetFailFileOnPhone(APP_TYPE eType)
{
	wstring strFailFile = L"";

	switch (eType)
	{
	case APP_TYPE_IPA:
		{
			strFailFile = IPA_OPERATION_COMMAND_FOLDER;
			strFailFile += IPA_OPERATION_COMMAND_FAIL_NAME;
		}
		break;
	case APP_TYPE_PXL:
		break;
	case APP_TYPE_DEB:
		{
			strFailFile = DEB_OPERATION_COMMAND_FOLDER;
			strFailFile += DEB_OPERATION_COMMAND_FAIL_NAME;
		}
        break;
    case APP_TYPE_APPSYNC:
        {
            strFailFile = APPSYNC_OPERATION_COMMAND_FOLDER;
            strFailFile += DEB_OPERATION_COMMAND_FAIL_NAME;
        }
        break;
	}

	return strFailFile;
}

wstring IIosAppOperation::GetInstallCmdFileOnPC(APP_TYPE eType)
{
	wstring strCmdFileName = L"";
	wstring strCmdFileContent = L"";

	switch (eType)
	{
	case APP_TYPE_IPA:
		{
			strCmdFileName = IPA_OPERATION_COMMAND_NAME;
		}
		break;
	case APP_TYPE_PXL:
		{
			strCmdFileName = PXL_OPERATION_COMMAND_NAME;
			strCmdFileContent = IosAppXmlHelper::GetPXLInstallCmdFileContent();
		}
		break;
	case APP_TYPE_DEB:
		{
			strCmdFileName = DEB_OPERATION_COMMAND_NAME;
			strCmdFileContent = L"install	ndapp.deb";
		}
		break;
    case APP_TYPE_APPSYNC:
        {
            strCmdFileName = DEB_OPERATION_COMMAND_NAME;
        }
        break;
	default:
		break;
	}

	return CreateCmdFileOnPC(strCmdFileName, strCmdFileContent);
}

wstring IIosAppOperation::GetUnInstallCmdFileOnPC(APP_TYPE eType, wstring strIdentifier)
{
	wstring strCmdFileName = L"";
	wstring strCmdFileContent = L"";

	switch (eType)
	{
	case APP_TYPE_IPA:
		break;
	case APP_TYPE_PXL:
		{
			strCmdFileName = PXL_OPERATION_COMMAND_NAME;
			strCmdFileContent = IosAppXmlHelper::GetPXLUnstallCmdFileContent(strIdentifier);
		}
		break;
	case APP_TYPE_DEB:
		{
			strCmdFileName = DEB_OPERATION_COMMAND_NAME;
			strCmdFileContent = L"remove	" + strIdentifier;
		}
		break;
	default:
		break;
	}

	return CreateCmdFileOnPC(strCmdFileName, strCmdFileContent);
}

wstring IIosAppOperation::CreateCmdFileOnPC(wstring strName, wstring strContent)
{
	wstring strCmdOnPC = L"";

	if (strName.length() <= 0)
		return L"";

	strCmdOnPC = _pDevData->GetPath()->GetLocalAppPath() + strName + CFileOperation::GetGuidW();
    void* hFile = MyFileOperation::MyCreateFile(strCmdOnPC);

	if (strContent.length() > 0)
	{
		string strBuffUTF8 = CCodeOperation::UnicodeToUTF_8(strContent);
		unsigned long nWrite = 0;
        MyFileOperation::MyWriteFile(hFile, strBuffUTF8, nWrite);
	}
    MyFileOperation::MyCloseFile(hFile);

	return strCmdOnPC;
}

void IIosAppOperation::RepairAppIcon()
{
	int nVer = IosPublicFunc::GetVerNum(_pDevInfo->_deviceProductVersion);
	if (nVer >= 300)
		NotifySB(10); //'"���ڵȴ�ͼ�껺������ ..."
	else if (nVer > 200)//'�����װ�ɹ�����ô����ɾ��ͼ�껺���ļ�����ֹ�����װ�ɹ��Ժ�iPhone��ȴ������ʾ���ͼ������⡣��2.0����iPhone����Ҫ��
		_pFileStream->Ios_DeleteFile(IPA_APP_INFO_FILE);
}

bool IIosAppOperation::DownLoadFolder(wstring strSrcOnPhone, wstring strDesOnPc)
{
	if (strSrcOnPhone.length() <= 0 ||
		strDesOnPc.length() <= 0 )
	{
		return false;
	}

	strDesOnPc = CStrOperation::trimRightW(strDesOnPc, WSTRING_PATH_SEPARATER);
	strSrcOnPhone = CStrOperation::trimRightW(strSrcOnPhone, L"/");
	wstring strParentPath = CStrOperation::trimRightW(CFileOperation::GetFileDir(strDesOnPc), WSTRING_PATH_SEPARATER);
	wstring strSrcName = CFileOperation::GetFileName(strSrcOnPhone);

	// ��Ҫ���صĽڵ�
	vector<NodeData*> nodes;
	DirData root(_pDevData);
	NodeData* pDir = FileHelper::GetRealNode(&root, strSrcOnPhone);
	if (!pDir)
		return false;
	nodes.push_back(pDir);
	// ���ص���Ŀ¼	
	//AppProgressObserver pObserver((ProgressSubject*)(_pProgressSubject));
    ProgressObserver* pObserver = NULL;
    if (!_pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(_pProgressSubject->_observers[0]);

	bool ret = (Transfer_Success == FileHelper::Download(&nodes, strParentPath, pObserver));

	if (!ret)
		return false;
		
	//::MoveFile((strParentPath+L"\\"+strSrcName).c_str(), strDesOnPc.c_str());
	CFileOperation::MoveFile((strParentPath+WSTRING_PATH_SEPARATER+strSrcName), strDesOnPc);
	return true;
}
#endif