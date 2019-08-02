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
	//strProductVersion:固件版本 IOS版本的数字表示
	_strSerialNum = pDevice->GetDevInfo()->_strSerialNumber;

	//pAfcHandle:USB模式下连接/断开时候返回的设备句柄
	//bJailBreaked:是否越狱
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

//strDaemonZipFile:守护的zip压缩包
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

    // 依此文件判定是否是新的守护
    _strDaemonFolderTemp = CStrOperation::trimRightW(_strDaemonFolderTemp, L"\\") + L"\\";
	wstring tempFolder = _strDaemonFolderTemp + L"ndDaemon\\TQServerLaunchd";
	bool isNew = CFileOperation::IsFileExist(tempFolder.c_str());

	//RETURN_FALSE_IF(!InstallDaemonByFolder(_strDaemonFolderTemp, isNew));
    //CFileOperation::DeleteDirectory(_strDaemonFolderTemp);
    //return true;

    return (isNew ? InstallDaemo_New() : InstallDaemo_Old());
}

//strDeamonFolder守护zip包解压后的路径
bool IosDaemonHelper::InstallDaemonByFolder(wstring strDeamonFolder, bool isNew)
{
	//清理上传的垃圾文件
	ClearOldDeamonFile();

	//备份系统原始的 /usr/sbin/update
	BackFile(strDeamonFolder, isNew);

	//更新守护文件
	UpdateFile(strDeamonFolder);

    //更新Plist文件
    UpdateInfoPList(strDeamonFolder);

    //开启服务
    return StartIosServer(strDeamonFolder);
}

bool IosDaemonHelper::InstallDaemo_New()
{
    // 安装前清空已安装的守护

    // 清理上传的垃圾文件
    _pFileSteam->Ios_DeleteFile(L"/usr/bin/TQServer");
    _pFileSteam->Ios_DeleteFile(L"/usr/bin/InstallerOpt");
    _pFileSteam->Ios_DeleteFile(L"/usr/bin/CaptureApp");
    FileHelper::Delete(_pDevice, L"/private/var/root/Media/PXL/Dropoff");
    FileHelper::Delete(_pDevice, L"/private/var/root/Media/ndDaemon");
    FileHelper::Delete(_pDevice, L"/var/root/Library/NetDragon/ndDaemon");

    // 上传文件列表
    UploadFile(true);

    //所有数据已经拷贝到手机，则将守护版本号写入到手机
    WriteTQServerVersion();

    return StartDaemon();
}

bool IosDaemonHelper::InstallDaemo_Old()
{
    // 安装前清空已安装的守护

    // 清理上传的垃圾文件

    // 上传文件列表
    UploadFile(false);

    //所有数据已经拷贝到手机，则将守护版本号写入到手机
    WriteTQServerVersion();

    // 检查守护是否已经安装
    bool daemonInstalled = _pDevice->GetDevInfo()->_ios_daemonInstalled;
    bool isRunRestar = false;
    if (daemonInstalled)
    {
        // 设置所有添加文件的权限（如果守护已经在运行）
        DirectoryOperationCommand cmd(_pDevice);
        bool resultd = cmd.ChangeMode(L"/bin/chmod", "775", L"0");
        resultd = cmd.ChangeMode(L"/var/root/Library/NetDragon/ndDaemon/", "775", L"0");
        resultd = cmd.ChangeMode(L"/System/Library/LaunchDaemons/NetDragon.Mobile.iPhone.PandaDaemon.plist", "644", L"0");

        // 文件设置完成，重启守护
        DeviceData* tempDeviceData = (DeviceData*)_pDevice;
        resultd = tempDeviceData->GetAppManager()->KillDeamonProcess(L"TQServer") == COMMAND_EXECUTE_SUCCESS;

        // 等待数秒，检查守护是否运行
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

	//'删除守护相关的文件
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

	//'''删除熊猫空间
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
	//备份/usr/libexec/debug_image_mount
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

	//3.x iPhone系统已经开机已经不会自动启动update程序	
	wstring strFileOnPC = strDeamonFolder + L"update." + _strSerialNum;	
	wstring strUpdateBak = _strRunFile + L".bak";
	//如果 /usr/sbin/update.bak 不存在，那么表示之前没有备份过此文件，那么就开始备份
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
        // 添加/更新IMEI信息
        wstring strIMEI = _pDevice->GetDevInfo()->_deviceIMEI;
        
        // 回写
        _pFileSteam->Ios_CopyToPhone(strFileOnPC, strVerFileOnDev);
        CFileOperation::DeleteFile(strFileOnPC);		
    }
    else
        return false;
    return true;
}

bool ND91Assistant::IosDaemonHelper::StartIosServer(wstring strDeamonFolder)
{
	//如果守护没有正常运行，则替换守护
	//替换手机Debug服务启动项目
	//重启服务
	int ret;
	int iTry = 2;					//服务启动2次，以保证守护安装成功

	wstring phoneRunFile = L"/usr/libexec/debug_image_mount";

	wstring strfile = strDeamonFolder + L"TQServerLoader";	
 	RETURN_FALSE_IF(_pFileSteam->Ios_CopyToPhone(strfile, phoneRunFile) != Transfer_Success);

	do
	{
		void* uSocket = NULL;
		RETURN_FALSE_IF (_pDevice->GetDevInfo()->_connectType != USB)
		
		//再次检测守护是否运行
		IosUsbConnection usbconnect((DeviceInfo*)_pDevice->GetDevInfo());
		ret = usbconnect.StartIOSService((char*)"com.apple.mobile.debug_image_mount", &uSocket);
	
		iTry--;

	} while (ret !=0 && iTry>0);

	return (ret == 0);
}

bool ND91Assistant::IosDaemonHelper::StartDaemon()
{
    //备份/usr/libexec/debug_image_mount
    wstring tmpdebug_image_mount = _strDaemonFolderTemp+L"debug_image_mount."+_strSerialNum;

    //3.x iPhone系统已经开机已经不会自动启动update程序
    wstring strFileOnPhone = L"/usr/libexec/debug_image_mount";
    wstring strUpdateBak = strFileOnPhone + L".bak";

    //如果 /usr/sbin/update.bak 不存在，那么表示之前没有备份过此文件，那么就开始备份
    if (!_pFileSteam->Ios_Exists(strUpdateBak) 
        && _pFileSteam->Ios_DownFromPhone(strFileOnPhone, tmpdebug_image_mount) == Transfer_Success)
        _pFileSteam->Ios_CopyToPhone(tmpdebug_image_mount, strUpdateBak);

    wstring strFile = _strDaemonFolderTemp+L"TQServerLoader";	
    _pFileSteam->Ios_CopyToPhone(strFile, strFileOnPhone);

    //如果守护没有正常运行，则替换守护
    //替换手机Debug服务启动项目
    //重启服务
    bool isRunRestar = false;
    int iTry = 2;					//服务启动2次，以保证守护安装成功

    do
    {
        void* uSocket = NULL;
        RETURN_FALSE_IF (_pDevice->GetDevInfo()->_connectType != USB)

        IosUsbConnection usbconnect((DeviceInfo*)_pDevice->GetDevInfo());
        usbconnect.StartIOSService((char*)"com.apple.mobile.debug_image_mount", &uSocket);

        //再次检测守护是否运行
        isRunRestar = usbconnect.PhoneDaemonIsStillRunning();
        iTry--;

    } while (!isRunRestar && iTry>0);

    // 将下载备份的debug_image_mount重新还原回手机
    if (CFileOperation::IsFileExist(tmpdebug_image_mount.c_str()))
        _pFileSteam->Ios_CopyToPhone(tmpdebug_image_mount, strFileOnPhone);

    RETURN_TRUE_IF(isRunRestar);

    // 如果仍然没有运行,替换系统启动项目

    // 备份系统原始的 /usr/sbin/update 
    strFile = _strDaemonFolderTemp+L"update."+_strSerialNum;
    // 3.x iPhone系统已经开机已经不会自动启动update程序
    strFileOnPhone = _strRunFile;
    strUpdateBak = strFileOnPhone+L".bak";
    // 如果 /usr/sbin/update.bak 不存在，那么表示之前没有备份过此文件，那么就开始备份
    TransferResult res = Transfer_Unknown;
    if (_pFileSteam && !_pFileSteam->Ios_Exists(strUpdateBak) &&
        _pFileSteam->Ios_DownFromPhone(strFileOnPhone, strFile) == Transfer_Success)
        res = _pFileSteam->Ios_CopyToPhone(strFile, strUpdateBak);

    // 为了保证用户iPhone中 com.apple.update.plist 的正确性，直接上传一个正确的 com.apple.update.plist 到用户 iPhone 中
    strFile = _strDaemonFolderTemp+CFileOperation::GetFileName(_strRunPlist);
    strFileOnPhone = _strRunPlist;
    if (CFileOperation::IsFileExist(strFile.c_str()))
        res = _pFileSteam->Ios_CopyToPhone(strFile, strFileOnPhone);

    // 上传 TQServerLoader 为 /usr/sbin/update
    strFile = _strDaemonFolderTemp+L"TQServerLoader";
    strFileOnPhone = _strRunFile;
    res = _pFileSteam->Ios_CopyToPhone(strFile, strFileOnPhone);

    return isRunRestar;
}