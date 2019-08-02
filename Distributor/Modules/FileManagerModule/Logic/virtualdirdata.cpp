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
	//������������
	return true;
}

deque<NodeData*> VirtualDirData::GetChildrens()
{
	if (_childNodes.size() == 0)
	{
		LOG->WriteDebug(L"׼������������");
		if (_pDevice->GetDevInfo()->_deviceType == Ios)
			InstallIos();
		else if (_pDevice->GetDevInfo()->_deviceType == Android)
			InstallAndroid();
	}
	else
		LOG->WriteDebug(L"�޽ڵ㣬�޷�����������");
		
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
	if (info->_ios_bJailBreaked)	//���Խ��
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



	NodeData *flash_Disk = FileHelper::GetNode(_parent,flash_disk_str);//�ҵ��ĵ�
	if (flash_Disk)
	{
		LOG->WriteDebug(L"�ƶ�u�̿�����ڴ����ɹ�");
		_flash_Diskdata = new VirtualNameDirData((DirData*)flash_Disk);
		_flash_Diskdata->_displayName = CommonUIFunc::LoadString(_T("FM_flash_Disk"));
		_flash_Diskdata->SetLogo(_T("#FileManagerUI_flash_disk"));
		_childNodes.push_back(_flash_Diskdata);
	}

	NodeData *mydocument = FileHelper::GetNode(_parent,mydocument_str);//�ҵ��ĵ�
	if (mydocument)
	{
		LOG->WriteDebug(L"�ҵ��ļ�������ڴ����ɹ�");
		_mydocumentData = new VirtualNameDirData((DirData*)mydocument);
		_mydocumentData->_displayName = CommonUIFunc::LoadString(_T("FM_mydocument"));
		_mydocumentData->SetLogo(_T("#FileManagerUI_mydocument"));
		_childNodes.push_back(_mydocumentData);
	}
	
	if (info->_ios_bJailBreaked)	//���Խ��			Ӣ�Ŀ������˳������Ҫ��������������¼�ֿ�����
	{
 		NodeData *userapp = FileHelper::GetNode(_parent,userapp_str);//�û�Ӧ�ó���
		if (userapp)
		{
			LOG->WriteDebug(L"Ӧ�ó��������ڴ����ɹ�");
			_userappData = new VirtualNameDirData((DirData*)userapp);
			_userappData->_displayName = CommonUIFunc::LoadString(_T("FM_userapp"));
			_userappData->SetLogo(_T("#FileManagerUI_userapp"));
			_childNodes.push_back(_userappData);
		}

		NodeData *backup = FileHelper::GetNode(_parent,backup_str);//��������¼
		if (backup)
		{
			LOG->WriteDebug(L"��������¼������ڴ����ɹ�");
			_backupData = new VirtualNameDirData((DirData*)backup);
			_backupData->_displayName = CommonUIFunc::LoadString(_T("FM_backup"));
			_backupData->SetLogo(_T("#FileManagerUI_backup"));
			_childNodes.push_back(_backupData);
		}
		
		NodeData *cydiaapp = FileHelper::GetNode(_parent,cydiaapp_str);//cydia�����װ
		if (cydiaapp)
		{
			LOG->WriteDebug(L"cydia������ڴ����ɹ�");
			_cydiaappData = new VirtualNameDirData((DirData*)cydiaapp);
			_cydiaappData->_displayName = CommonUIFunc::LoadString(_T("FM_cydiaapp"));
			_cydiaappData->SetLogo(_T("#FileManagerUI_cydiaapp"));
			_childNodes.push_back(_cydiaappData);
		}
	}
	else
	{
		NodeData *backup = FileHelper::GetNode(_parent,backup_str);//��������¼
		if (backup)
		{
			LOG->WriteDebug(L"��������¼������ڴ����ɹ�");
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
		LOG->WriteDebug(L"�洢�����Ƴ����޷���ʾ�������");
		return;
	}

	LOG->WriteDebug(L"׼����ȡ���濨������ڽڵ�");
	NodeData *flash_Disk = FileHelper::GetNode(_parent,_T("/mnt/sdcard"));//���濨
	if (flash_Disk)
	{
		LOG->WriteDebug(L"�洢��������ڴ����ɹ�");
		_flash_Diskdata = new VirtualNameDirData((DirData*)flash_Disk);
		_flash_Diskdata->_displayName = CommonUIFunc::LoadString(_T("FM_SDcard"));
		_flash_Diskdata->SetLogo(_T("#FileManagerUI_SDcard"));
		_childNodes.push_back(_flash_Diskdata);
	}
	else
		LOG->WriteDebug(L"�洢��������ڲ�����");
	

	LOG->WriteDebug(L"׼����ȡ�ƶ�U�̿�����ڽڵ�");
	NodeData *mydocument = FileHelper::GetNode(_parent,_T("/mnt/sdcard/My Documents"));//�ƶ�U��
	if (mydocument)
	{
		LOG->WriteDebug(L"�ƶ�U�̿�����ڴ����ɹ�");
		_mydocumentData = new VirtualNameDirData((DirData*)mydocument);
		_mydocumentData->_displayName = CommonUIFunc::LoadString(_T("FM_flash_Disk"));
		_mydocumentData->SetLogo(_T("#FileManagerUI_flash_disk"));
		_childNodes.push_back(_mydocumentData);
	}
	else
		LOG->WriteDebug(L"�ƶ�U�̿�����ڲ�����");

	DeviceData* ptemp = const_cast<DeviceData*>(_pDevice);		//��Ƭ
	if (ptemp)
	{
		DirData *photo = ptemp->GetRootData()->GetPath(PHOTO);
		if (photo)
		{
			LOG->WriteDebug(L"��Ƭ������ڴ����ɹ�");
			_photo_data = new VirtualNameDirData((DirData*)photo);
			_photo_data->_displayName = CommonUIFunc::LoadString(_T("FM_photo"));
			_photo_data->SetLogo(_T("#FileManagerUI_photo"));
			_childNodes.push_back(_photo_data);

		}
		else
			LOG->WriteDebug(L"��Ƭ������ڲ�����");
	}

}
