#include "stdafx.h"

#include "Device/IosDaemonHelper.h"
#include "Core/DeviceData.h"
#include "Device/IosPublicFunc.h"
#include "Module/File/FileHelper.h"
#include "Module/File/DirectoryOperationCommand.h"
#include "Device/IosUsbConnection.h"
#include "Common/Path.h"
#include "Module/Common/IosPList/IosPlistHelper.h"
#include "Module/Application/AppManager.h"

IosDaemonHelper::IosDaemonHelper(const DeviceData* pDevice)
: _pDevice(pDevice)
{
	//strProductVersion:�̼��汾 IOS�汾�����ֱ�ʾ
	_strSerialNum = pDevice->GetDevInfo()->_strSerialNumber;

	//pAfcHandle:USBģʽ������/�Ͽ�ʱ�򷵻ص��豸���
	//bJailBreaked:�Ƿ�Խ��
	_pFileSteam = NULL;
	if (pDevice->GetDevInfo()->_ios_afcHandle && pDevice->GetDevInfo()->_ios_bJailBreaked)
	{
		_pFileSteam = new AFCFileSysemBase(pDevice->GetDevInfo());
		InitRun(IosPublicFunc::GetVerNum(pDevice->GetDevInfo()->_deviceProductVersion));
	}
}

IosDaemonHelper::~IosDaemonHelper(void)
{
    SAFE_DELETE(_pFileSteam);
    CFileOperation::DeleteDirectory(_strDaemonFolderTemp);
}

//strDaemonZipFile:�ػ���zipѹ����
bool IosDaemonHelper::InstallDaemon(wstring strDaemonZipFile, wstring daemonVersionOnService)
{
    RETURN_FALSE_IF(!_pFileSteam);

    _daemonVersionOnService = daemonVersionOnService;
    _strDaemonFolderTemp = _pDevice->GetPath()->GetLocalDeamonFolder();
	CFileOperation::DeleteDirectory(_strDaemonFolderTemp);
    RETURN_FALSE_IF(!CFileOperation::UnZipFile(strDaemonZipFile, _strDaemonFolderTemp));

    vector<wstring> files;
    CFileOperation::FindAllFileInDir(_strDaemonFolderTemp, files, L"zip");
    for (vector<wstring>::iterator it = files.begin(); it != files.end(); it++)
        CFileOperation::UnZipFile(*it, _strDaemonFolderTemp);

    // �����ļ��ж��Ƿ����µ��ػ�
    _strDaemonFolderTemp = CStrOperation::trimRightW(_strDaemonFolderTemp, L"\\") + L"\\";
	wstring tempFolder = _strDaemonFolderTemp + L"ndDaemon\\TQServerLaunchd";
	bool isNew = CFileOperation::IsFileExist(tempFolder.c_str());

	//RETURN_FALSE_IF(!InstallDaemonByFolder(_strDaemonFolderTemp, isNew));
    //CFileOperation::DeleteDirectory(_strDaemonFolderTemp);
    //return true;

    return (isNew ? InstallDaemo_New() : InstallDaemo_Old());
}

//strDeamonFolder�ػ�zip����ѹ���·��
bool IosDaemonHelper::InstallDaemonByFolder(wstring strDeamonFolder, bool isNew)
{
	//�����ϴ��������ļ�
	ClearOldDeamonFile();

	//����ϵͳԭʼ�� /usr/sbin/update
	BackFile(strDeamonFolder, isNew);

	//�����ػ��ļ�
	UpdateFile(strDeamonFolder);

    //����Plist�ļ�
    UpdateInfoPList(strDeamonFolder);

    //��������
    return StartIosServer(strDeamonFolder);
}

bool IosDaemonHelper::InstallDaemo_New()
{
    // ��װǰ����Ѱ�װ���ػ�

    // �����ϴ��������ļ�
    _pFileSteam->Ios_DeleteFile(L"/usr/bin/TQServer");
    _pFileSteam->Ios_DeleteFile(L"/usr/bin/InstallerOpt");
    _pFileSteam->Ios_DeleteFile(L"/usr/bin/CaptureApp");
    FileHelper::Delete(_pDevice, L"/private/var/root/Media/PXL/Dropoff");
    FileHelper::Delete(_pDevice, L"/private/var/root/Media/ndDaemon");
    FileHelper::Delete(_pDevice, L"/var/root/Library/NetDragon/ndDaemon");

    // �ϴ��ļ��б�
    UploadFile(true);

    //���������Ѿ��������ֻ������ػ��汾��д�뵽�ֻ�
    WriteTQServerVersion();

    return StartDaemon();
}

bool IosDaemonHelper::InstallDaemo_Old()
{
    // ��װǰ����Ѱ�װ���ػ�

    // �����ϴ��������ļ�

    // �ϴ��ļ��б�
    UploadFile(false);

    //���������Ѿ��������ֻ������ػ��汾��д�뵽�ֻ�
    WriteTQServerVersion();

    // ����ػ��Ƿ��Ѿ���װ
    bool daemonInstalled = _pDevice->GetDevInfo()->_ios_daemonInstalled;
    bool isRunRestar = false;
    if (daemonInstalled)
    {
        // ������������ļ���Ȩ�ޣ�����ػ��Ѿ������У�
        DirectoryOperationCommand cmd(_pDevice);
        bool resultd = cmd.ChangeMode(L"/bin/chmod", "775", L"0");
        resultd = cmd.ChangeMode(L"/var/root/Library/NetDragon/ndDaemon/", "775", L"0");
        resultd = cmd.ChangeMode(L"/System/Library/LaunchDaemons/NetDragon.Mobile.iPhone.PandaDaemon.plist", "644", L"0");

        // �ļ�������ɣ������ػ�
        DeviceData* tempDeviceData = (DeviceData*)_pDevice;
        resultd = tempDeviceData->GetAppManager()->KillDeamonProcess(L"TQServer") == COMMAND_EXECUTE_SUCCESS;

        // �ȴ����룬����ػ��Ƿ�����
        Sleep(3000);
        IosUsbConnection usbconnect((DeviceInfo*)_pDevice->GetDevInfo());
        isRunRestar = usbconnect.PhoneDaemonIsStillRunning();
    }

    RETURN_TRUE_IF(isRunRestar);

    return StartDaemon();
}

void IosDaemonHelper::UploadFile(bool isNew)
{
    wstring strFileOnPC = _strDaemonFolderTemp + L"chmod";
    wstring strFileOnPhone = L"/bin/chmod";
    _pFileSteam->Ios_CopyToPhone(strFileOnPC, strFileOnPhone);

    strFileOnPC = _strDaemonFolderTemp + L"ndDaemon";
    strFileOnPhone = L"/var/root/Library/NetDragon";
    FileHelper::Upload( _pDevice, strFileOnPC, strFileOnPhone, NULL);

    strFileOnPC = _strDaemonFolderTemp + L"TQServer";
    strFileOnPhone = L"/usr/bin/TQServer";
    _pFileSteam->Ios_CopyToPhone(strFileOnPC, strFileOnPhone);

    RETURN_IF(isNew);
    strFileOnPC = _strDaemonFolderTemp + L"NetDragon.Mobile.iPhone.PandaDaemon.plist";
    strFileOnPhone = L"/System/Library/LaunchDaemons/NetDragon.Mobile.iPhone.PandaDaemon.plist";
    _pFileSteam->Ios_CopyToPhone(strFileOnPC, strFileOnPhone);
}

bool IosDaemonHelper::WriteTQServerVersion()
{
    IosPlistHelper xmlHelper(XML_PI, XML_DOCTYPE, eXmlType_UTF8, XML_ELEMENT(L"plist", L"", L"version", L"1.0"));

    XML_NODE_INDEX index;
    xmlHelper.AddElement(index, XML_ELEMENT(L"dict"));

    index.AddChild(0);
    xmlHelper.AddElement(index, XML_ELEMENT(L"key", L"Version"));
    xmlHelper.AddElement(index, XML_ELEMENT(L"string", _daemonVersionOnService));
    xmlHelper.AddElement(index, XML_ELEMENT(L"key", L"Serial"));
    xmlHelper.AddElement(index, XML_ELEMENT(L"string", _pDevice->GetDevInfo()->_ios_deviceIdentifier));
    xmlHelper.AddElement(index, XML_ELEMENT(L"key", L"Identifyer"));
    xmlHelper.AddElement(index, XML_ELEMENT(L"string", _pDevice->GetDevInfo()->_ios_deviceIdentifier));
    xmlHelper.AddElement(index, XML_ELEMENT(L"key", L"SerialNumber"));
    xmlHelper.AddElement(index, XML_ELEMENT(L"string", _pDevice->GetDevInfo()->_strSerialNumber));

    xmlHelper.WriteToFile(_strDaemonFolderTemp+L"Info.plist");

    wstring ndDaemonPath = L"/private/var/root/Media/ndDaemon/";
    if (!_pFileSteam->Ios_Exists(ndDaemonPath))
        _pFileSteam->Ios_CreateDirectory(ndDaemonPath);

    return _pFileSteam->Ios_CopyToPhone(_strDaemonFolderTemp+L"Info.plist", ndDaemonPath+L"Info.plist") == Transfer_Success;
}

bool IosDaemonHelper::UnInstallDaemon()
{
	RETURN_FALSE_IF (!_pFileSteam);

	//'ɾ���ػ���ص��ļ�
	_pFileSteam->Ios_DeleteFile(L"/usr/bin/TQServer");
	_pFileSteam->Ios_DeleteFile(L"/usr/bin/InstallerOpt");
	_pFileSteam->Ios_DeleteFile(L"/usr/bin/CaptureApp");
	_pFileSteam->Ios_DeleteFile(L"/private/var/mobile/Library/TQServer/");
	FileHelper::Delete(_pDevice, L"/private/var/root/Media/PXL/Dropoff");
	FileHelper::Delete(_pDevice, L"/private/var/root/Media/ndDaemon");
	_pFileSteam->Ios_DeleteFile(L"/System/Library/LaunchDaemons/NetDragon.Mobile.iPhone.PandaDaemon.plist");
	FileHelper::Delete(_pDevice, L"/Applications/PandaDaemon.app");

	//'ndDaemon 3.x
	FileHelper::Delete(_pDevice, L"var/root/Library/NetDragon/ndDaemon");

	//'''ɾ����è�ռ�
	_pFileSteam->Ios_DeleteFile(L"/System/Library/LaunchDaemons/com.91.sj.iphone.pandaspace.plist");
	FileHelper::Delete(_pDevice, L"var/mobile/Media/PandaSpace");
	FileHelper::Delete(_pDevice, L"var/mobile/PandaSpace");
	FileHelper::Delete(_pDevice, L"/Applications/PandaSpace.app");

	return true;
}

void IosDaemonHelper::UpdateFile(wstring strDeamonFolder)
{

	wstring strFileOnPC = strDeamonFolder + CFileOperation::GetFileName(_strRunPlist);
	wstring strFileOnPhone = _strRunPlist;
	RETURN_IF ( _pFileSteam->Ios_CopyToPhone(strFileOnPC, strFileOnPhone) != Transfer_Success );

	strFileOnPC = strDeamonFolder + L"TQServerLoader";
	strFileOnPhone = _strRunFile;
	RETURN_IF ( _pFileSteam->Ios_CopyToPhone(strFileOnPC, strFileOnPhone) != Transfer_Success);

	strFileOnPC = strDeamonFolder + L"chmod";
	strFileOnPhone = L"/bin/chmod";
	RETURN_IF ( _pFileSteam->Ios_CopyToPhone(strFileOnPC, strFileOnPhone) != Transfer_Success);

	strFileOnPC = strDeamonFolder + L"ndDaemon";
	strFileOnPhone = L"/var/root/Library/NetDragon";
	RETURN_IF ( !FileHelper::Upload( _pDevice, strFileOnPC, strFileOnPhone, NULL));

	strFileOnPC = strDeamonFolder + L"TQServer";
	strFileOnPhone = L"/usr/bin/TQServer";
	RETURN_IF ( _pFileSteam->Ios_CopyToPhone(strFileOnPC, strFileOnPhone) != Transfer_Success);

	strFileOnPC = strDeamonFolder + L"NetDragon.Mobile.iPhone.PandaDaemon.plist";
	strFileOnPhone = L"/System/Library/LaunchDaemons/NetDragon.Mobile.iPhone.PandaDaemon.plist";
	RETURN_IF ( _pFileSteam->Ios_CopyToPhone(strFileOnPC, strFileOnPhone) != Transfer_Success);
}

void IosDaemonHelper::BackFile(wstring strDeamonFolder, bool isNew)
{
	//����/usr/libexec/debug_image_mount
	if ( isNew )
	{
		wstring phoneRunFile = L"/usr/libexec/debug_image_mount";
		wstring UpdateBak = phoneRunFile + L".bak";

		wstring tempMount = strDeamonFolder + L"debug_image_mount." + _strSerialNum;


		if (_pFileSteam && !_pFileSteam->Ios_Exists(UpdateBak) &&
			_pFileSteam->Ios_DownFromPhone(phoneRunFile, tempMount) == Transfer_Success)
		{
			_pFileSteam->Ios_CopyToPhone(tempMount, UpdateBak);
			CFileOperation::DeleteFile(tempMount);		
		}
	}

	//3.x iPhoneϵͳ�Ѿ������Ѿ������Զ�����update����	
	wstring strFileOnPC = strDeamonFolder + L"update." + _strSerialNum;	
	wstring strUpdateBak = _strRunFile + L".bak";
	//��� /usr/sbin/update.bak �����ڣ���ô��ʾ֮ǰû�б��ݹ����ļ�����ô�Ϳ�ʼ����
	if (_pFileSteam && !_pFileSteam->Ios_Exists(strUpdateBak) &&
		_pFileSteam->Ios_DownFromPhone(_strRunFile, strFileOnPC) == Transfer_Success)
	{
		_pFileSteam->Ios_CopyToPhone(strFileOnPC, strUpdateBak);
        CFileOperation::DeleteFile(strFileOnPC);		
	}
}

void IosDaemonHelper::ClearOldDeamonFile()
{
	RETURN_IF(! _pFileSteam);

	_pFileSteam->Ios_DeleteFile(L"/usr/bin/TQServer");
	_pFileSteam->Ios_DeleteFile(L"/usr/bin/InstallerOpt");
	_pFileSteam->Ios_DeleteFile(L"/usr/bin/CaptureApp");
	FileHelper::Delete(_pDevice, L"/private/var/root/Media/PXL/Dropoff");
	FileHelper::Delete(_pDevice, L"/private/var/root/Media/ndDaemon");
	//_pFileSteam->Ios_DeleteFile(L"/System/Library/LaunchDaemons/NetDragon.Mobile.iPhone.PandaDaemon.plist");
	FileHelper::Delete(_pDevice, L"/Applications/PandaDaemon.app");

	//ndDaemon 3.x
	FileHelper::Delete(_pDevice, L"/var/root/Library/NetDragon/ndDaemon");
}

void IosDaemonHelper::InitRun(int nVerNum)
{
	if (nVerNum < 300)
	{
		_strRunFile = L"/usr/sbin/update";
		_strRunPlist = L"/System/Library/LaunchDaemons/com.apple.update.plist";
	}
	else if (nVerNum < 400)
	{
		_strRunFile = L"/usr/sbin/syslogd";
		_strRunPlist = L"/System/Library/LaunchDaemons/com.apple.syslogd.plist";
	}
	else
	{
		_strRunFile = L"/System/Library/CoreServices/DumpPanic";
		_strRunPlist = L"/System/Library/LaunchDaemons/com.apple.DumpPanic.plist";
	}
}

bool ND91Assistant::IosDaemonHelper::UpdateInfoPList(wstring strDeamonFolder)
{
    wstring strFileOnPC = strDeamonFolder + L"ndDaemon.Info.plist";
    wstring strVerFileOnDev = L"/private/var/root/Media/ndDaemon/Info.plist";
    if (_pFileSteam && !_pFileSteam->Ios_Exists(strVerFileOnDev) &&
        _pFileSteam->Ios_DownFromPhone(strVerFileOnDev, strFileOnPC) == Transfer_Success)
    {
        // ���/����IMEI��Ϣ
        wstring strIMEI = _pDevice->GetDevInfo()->_deviceIMEI;
        
        // ��д
        _pFileSteam->Ios_CopyToPhone(strFileOnPC, strVerFileOnDev);
        CFileOperation::DeleteFile(strFileOnPC);		
    }
    else
        return false;
    return true;
}

bool ND91Assistant::IosDaemonHelper::StartIosServer(wstring strDeamonFolder)
{
	//����ػ�û���������У����滻�ػ�
	//�滻�ֻ�Debug����������Ŀ
	//��������
	int ret;
	int iTry = 2;					//��������2�Σ��Ա�֤�ػ���װ�ɹ�

	wstring phoneRunFile = L"/usr/libexec/debug_image_mount";

	wstring strfile = strDeamonFolder + L"TQServerLoader";	
 	RETURN_FALSE_IF(_pFileSteam->Ios_CopyToPhone(strfile, phoneRunFile) != Transfer_Success);

	do
	{
		void* uSocket = NULL;
		RETURN_FALSE_IF (_pDevice->GetDevInfo()->_connectType != USB)
		
		//�ٴμ���ػ��Ƿ�����
		IosUsbConnection usbconnect((DeviceInfo*)_pDevice->GetDevInfo());
		ret = usbconnect.StartIOSService((char*)"com.apple.mobile.debug_image_mount", &uSocket);
	
		iTry--;

	} while (ret !=0 && iTry>0);

	return (ret == 0);
}

bool ND91Assistant::IosDaemonHelper::StartDaemon()
{
    //����/usr/libexec/debug_image_mount
    wstring tmpdebug_image_mount = _strDaemonFolderTemp+L"debug_image_mount."+_strSerialNum;

    //3.x iPhoneϵͳ�Ѿ������Ѿ������Զ�����update����
    wstring strFileOnPhone = L"/usr/libexec/debug_image_mount";
    wstring strUpdateBak = strFileOnPhone + L".bak";

    //��� /usr/sbin/update.bak �����ڣ���ô��ʾ֮ǰû�б��ݹ����ļ�����ô�Ϳ�ʼ����
    if (!_pFileSteam->Ios_Exists(strUpdateBak) 
        && _pFileSteam->Ios_DownFromPhone(strFileOnPhone, tmpdebug_image_mount) == Transfer_Success)
        _pFileSteam->Ios_CopyToPhone(tmpdebug_image_mount, strUpdateBak);

    wstring strFile = _strDaemonFolderTemp+L"TQServerLoader";	
    _pFileSteam->Ios_CopyToPhone(strFile, strFileOnPhone);

    //����ػ�û���������У����滻�ػ�
    //�滻�ֻ�Debug����������Ŀ
    //��������
    bool isRunRestar = false;
    int iTry = 2;					//��������2�Σ��Ա�֤�ػ���װ�ɹ�

    do
    {
        void* uSocket = NULL;
        RETURN_FALSE_IF (_pDevice->GetDevInfo()->_connectType != USB)

        IosUsbConnection usbconnect((DeviceInfo*)_pDevice->GetDevInfo());
        usbconnect.StartIOSService((char*)"com.apple.mobile.debug_image_mount", &uSocket);

        //�ٴμ���ػ��Ƿ�����
        isRunRestar = usbconnect.PhoneDaemonIsStillRunning();
        iTry--;

    } while (!isRunRestar && iTry>0);

    // �����ر��ݵ�debug_image_mount���»�ԭ���ֻ�
    if (CFileOperation::IsFileExist(tmpdebug_image_mount.c_str()))
        _pFileSteam->Ios_CopyToPhone(tmpdebug_image_mount, strFileOnPhone);

    RETURN_TRUE_IF(isRunRestar);

    // �����Ȼû������,�滻ϵͳ������Ŀ

    // ����ϵͳԭʼ�� /usr/sbin/update 
    strFile = _strDaemonFolderTemp+L"update."+_strSerialNum;
    // 3.x iPhoneϵͳ�Ѿ������Ѿ������Զ�����update����
    strFileOnPhone = _strRunFile;
    strUpdateBak = strFileOnPhone+L".bak";
    // ��� /usr/sbin/update.bak �����ڣ���ô��ʾ֮ǰû�б��ݹ����ļ�����ô�Ϳ�ʼ����
    TransferResult res = Transfer_Unknown;
    if (_pFileSteam && !_pFileSteam->Ios_Exists(strUpdateBak) &&
        _pFileSteam->Ios_DownFromPhone(strFileOnPhone, strFile) == Transfer_Success)
        res = _pFileSteam->Ios_CopyToPhone(strFile, strUpdateBak);

    // Ϊ�˱�֤�û�iPhone�� com.apple.update.plist ����ȷ�ԣ�ֱ���ϴ�һ����ȷ�� com.apple.update.plist ���û� iPhone ��
    strFile = _strDaemonFolderTemp+CFileOperation::GetFileName(_strRunPlist);
    strFileOnPhone = _strRunPlist;
    if (CFileOperation::IsFileExist(strFile.c_str()))
        res = _pFileSteam->Ios_CopyToPhone(strFile, strFileOnPhone);

    // �ϴ� TQServerLoader Ϊ /usr/sbin/update
    strFile = _strDaemonFolderTemp+L"TQServerLoader";
    strFileOnPhone = _strRunFile;
    res = _pFileSteam->Ios_CopyToPhone(strFile, strFileOnPhone);

    return isRunRestar;
}