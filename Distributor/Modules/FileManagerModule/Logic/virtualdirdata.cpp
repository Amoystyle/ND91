#include "stdafx.h"
#include "virtualdirdata.h"

VirtualDirData::VirtualDirData(const DeviceData* pDevice)
: DirData(pDevice)
,RootData(pDevice)
{
	_pDevice = pDevice;
	_childNodes.clear();
}

VirtualDirData::~VirtualDirData()
{
	if (_flash_Diskdata)
		SAFE_DELETE(_flash_Diskdata);
	if (_mydocumentData)
		SAFE_DELETE(_mydocumentData);
	if (_backupData)
		SAFE_DELETE(_backupData);
	if (_userappData)
		SAFE_DELETE(_userappData);
	if (_cydiaappData)
		SAFE_DELETE(_cydiaappData);
	if (_photo_data)
		SAFE_DELETE(_photo_data);
	if (_parent)
		SAFE_DELETE(_parent);
}

bool VirtualDirData::Rename( wstring dest )
{
	//不允许重命名
	return true;
}

deque<NodeData*> VirtualDirData::GetChildrens()
{
	if (_childNodes.size() == 0)
	{
		LOG->WriteDebug(L"准备载入快速入口");
		if (_pDevice->GetDevInfo()->_deviceType == Ios)
			InstallIos();
		else if (_pDevice->GetDevInfo()->_deviceType == Android)
			InstallAndroid();
	}
	else
		LOG->WriteDebug(L"无节点，无法载入快速入口");
		
	return _childNodes;
}

void VirtualDirData::InstallIos()
{
	_parent = new DirData(_pDevice);

	wstring flash_disk_str;
	wstring mydocument_str;
	wstring userapp_str;
	wstring backup_str;
	wstring cydiaapp_str;



	const DeviceInfo *info = _pDevice->GetDevInfo();
	if (info->_ios_bJailBreaked)	//如果越狱
	{
		flash_disk_str = _T("/var/mobile/Media/general_storage");
		mydocument_str = _T("/var/mobile/Media/MyDocuments");
		userapp_str = _T("/var/mobile/Applications");
		backup_str = _T("/var/mobile/Media/Recordings");
		cydiaapp_str = _T("/var/root/Media/Cydia/AutoInstall");
	}
	else
	{
		flash_disk_str = _T("/general_storage");
		mydocument_str = _T("/MyDocuments");
		backup_str = _T("/Recordings");
	}



	NodeData *flash_Disk = FileHelper::GetNode(_parent,flash_disk_str);//我的文档
	if (flash_Disk)
	{
		LOG->WriteDebug(L"移动u盘快速入口创建成功");
		_flash_Diskdata = new VirtualNameDirData((DirData*)flash_Disk);
		_flash_Diskdata->_displayName = CommonUIFunc::LoadString(_T("FM_flash_Disk"));
		_flash_Diskdata->SetLogo(_T("#FileManagerUI_flash_disk"));
		_childNodes.push_back(_flash_Diskdata);
	}

	NodeData *mydocument = FileHelper::GetNode(_parent,mydocument_str);//我的文档
	if (mydocument)
	{
		LOG->WriteDebug(L"我的文件快速入口创建成功");
		_mydocumentData = new VirtualNameDirData((DirData*)mydocument);
		_mydocumentData->_displayName = CommonUIFunc::LoadString(_T("FM_mydocument"));
		_mydocumentData->SetLogo(_T("#FileManagerUI_mydocument"));
		_childNodes.push_back(_mydocumentData);
	}
	
	if (info->_ios_bJailBreaked)	//如果越狱			英文快速入口顺序排列要求，所以语音备忘录分开处理
	{
 		NodeData *userapp = FileHelper::GetNode(_parent,userapp_str);//用户应用程序
		if (userapp)
		{
			LOG->WriteDebug(L"应用程序快速入口创建成功");
			_userappData = new VirtualNameDirData((DirData*)userapp);
			_userappData->_displayName = CommonUIFunc::LoadString(_T("FM_userapp"));
			_userappData->SetLogo(_T("#FileManagerUI_userapp"));
			_childNodes.push_back(_userappData);
		}

		NodeData *backup = FileHelper::GetNode(_parent,backup_str);//语音备忘录
		if (backup)
		{
			LOG->WriteDebug(L"语音备忘录快速入口创建成功");
			_backupData = new VirtualNameDirData((DirData*)backup);
			_backupData->_displayName = CommonUIFunc::LoadString(_T("FM_backup"));
			_backupData->SetLogo(_T("#FileManagerUI_backup"));
			_childNodes.push_back(_backupData);
		}
		
		NodeData *cydiaapp = FileHelper::GetNode(_parent,cydiaapp_str);//cydia软件安装
		if (cydiaapp)
		{
			LOG->WriteDebug(L"cydia快速入口创建成功");
			_cydiaappData = new VirtualNameDirData((DirData*)cydiaapp);
			_cydiaappData->_displayName = CommonUIFunc::LoadString(_T("FM_cydiaapp"));
			_cydiaappData->SetLogo(_T("#FileManagerUI_cydiaapp"));
			_childNodes.push_back(_cydiaappData);
		}
	}
	else
	{
		NodeData *backup = FileHelper::GetNode(_parent,backup_str);//语音备忘录
		if (backup)
		{
			LOG->WriteDebug(L"语音备忘录快速入口创建成功");
			_backupData = new VirtualNameDirData((DirData*)backup);
			_backupData->_displayName = CommonUIFunc::LoadString(_T("FM_backup"));
			_backupData->SetLogo(_T("#FileManagerUI_backup"));
			_childNodes.push_back(_backupData);
		}
	}


}

void VirtualDirData::InstallAndroid()
{
	_parent = new DirData(_pDevice);

	SystemManager manager(_pDevice);
	SDCardState state = manager.GetSDCardState();
	if (state != MOUNTED)
	{
		LOG->WriteDebug(L"存储卡被移除，无法显示快速入口");
		return;
	}

	LOG->WriteDebug(L"准备获取储存卡快速入口节点");
	NodeData *flash_Disk = FileHelper::GetNode(_parent,_T("/mnt/sdcard"));//储存卡
	if (flash_Disk)
	{
		LOG->WriteDebug(L"存储卡快速入口创建成功");
		_flash_Diskdata = new VirtualNameDirData((DirData*)flash_Disk);
		_flash_Diskdata->_displayName = CommonUIFunc::LoadString(_T("FM_SDcard"));
		_flash_Diskdata->SetLogo(_T("#FileManagerUI_SDcard"));
		_childNodes.push_back(_flash_Diskdata);
	}
	else
		LOG->WriteDebug(L"存储卡快速入口不存在");
	

	LOG->WriteDebug(L"准备获取移动U盘快速入口节点");
	NodeData *mydocument = FileHelper::GetNode(_parent,_T("/mnt/sdcard/My Documents"));//移动U盘
	if (mydocument)
	{
		LOG->WriteDebug(L"移动U盘快速入口创建成功");
		_mydocumentData = new VirtualNameDirData((DirData*)mydocument);
		_mydocumentData->_displayName = CommonUIFunc::LoadString(_T("FM_flash_Disk"));
		_mydocumentData->SetLogo(_T("#FileManagerUI_flash_disk"));
		_childNodes.push_back(_mydocumentData);
	}
	else
		LOG->WriteDebug(L"移动U盘快速入口不存在");

	DeviceData* ptemp = const_cast<DeviceData*>(_pDevice);		//照片
	if (ptemp)
	{
		DirData *photo = ptemp->GetRootData()->GetPath(PHOTO);
		if (photo)
		{
			LOG->WriteDebug(L"照片快速入口创建成功");
			_photo_data = new VirtualNameDirData((DirData*)photo);
			_photo_data->_displayName = CommonUIFunc::LoadString(_T("FM_photo"));
			_photo_data->SetLogo(_T("#FileManagerUI_photo"));
			_childNodes.push_back(_photo_data);

		}
		else
			LOG->WriteDebug(L"照片快速入口不存在");
	}

}
