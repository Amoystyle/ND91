#include "stdafx.h"
#include "ScreenshotClient.h"

#include "Core/DeviceData.h"
#include "Device/CFConverter.h"
#include "Device/IosUsbConnection.h"

#include "Module/Common/Synchronizer/DeviceLink.h"
#include "Module/Common/Synchronizer/MobileImageMounterClient.h"

#include "Module/File/DirectoryOperationCommand.h"
#include "Module/File/UploadFileCommand.h"
#include "Module/File/FileHelper.h"
#include "Common/Path.h"

ScreenshotClient::ScreenshotClient(const DeviceData* pDeviceData,IosUsbConnection* device)
:ServiceClientBase(device)
{
	 _pDeviceData = pDeviceData;
	 _serviceName = "com.apple.mobile.screenshotr";
	 _isServiceRunning = false;
	 if(device == NULL)
	 {
		 _device = new IosUsbConnection((DeviceInfo*)(_pDeviceData->GetDevInfo()));
	 }
}

ScreenshotClient::~ScreenshotClient(void)
{
}

bool ND91Assistant::ScreenshotClient::StartScreenshotService()
{
	if(!_isServiceRunning)
	{
		_isServiceRunning = StartService();
		if(_isServiceRunning)
		{
			_dlink = new DeviceLink(this);
			if(!_dlink->VersionExchange(100,0))
			{
				if(_serviceSock!=NULL)
				{
					_dlink->Disconnect("");
					CloseService();
				}
				_isServiceRunning = false;
			}
		}
	}
	return _isServiceRunning;
}

wstring ND91Assistant::ScreenshotClient::TakeScreenshotWithoutConvertTiff2PNG()
{
	if(StartScreenshotService())
	{
		map<string, string> msgdict;
		msgdict["MessageType"]="ScreenShotRequest";
		if(_dlink->Send(msgdict))
		{
			wstring filepath=_pDeviceData->GetPath()->GetLocalScreensnapPath()+L"ios_screensnap."+CFileOperation::GetGuidW();
			if(_dlink->Receive(filepath))
				return filepath;
		}
	}
	return L"";
}

bool ND91Assistant::ScreenshotClient::CheckEnvironment()
{
	
	DirectoryOperationCommand dircmd(_pDeviceData);

	bool result=true;
	if(_pDeviceData->GetDevInfo()->_ios_bJailBreaked)
	{
		string path= "/var/mobile/Media/";
		wstring wpath=CCodeOperation::UTF_8ToUnicode(path) +L"Developer";
		if(!dircmd.Exist(wpath) && !dircmd.MakeDir(wpath))
		{
			return false;
		}
		DirData* dir = new DirData(_pDeviceData);
		dir->_name =wpath;
		deque<NodeData*> childs =dir->GetChildrens();
		vector<NodeData*> nodes;
		for (size_t i = 0 ;i<childs.size();i++)
		{
			nodes.push_back(childs[i]);
		}
		if((int)childs.size()>0 && !FileHelper::BatchDelete(&nodes,NULL))
		{
			result = false;
		}
		SAFE_DELETE(dir);
	}

	RETURN_FALSE_IF(!result);
	string path="/var/mobile/Media/";
	wstring wpath = CCodeOperation::UTF_8ToUnicode(path)+L"PublicStaging";
	if(dircmd.Exist(wpath) || dircmd.MakeDir(wpath))
	{
		return true;
	}
	return false;
}

bool ND91Assistant::ScreenshotClient::InstallPrerequisite( wstring prerequisitePackagePath )
{
	if(CheckEnvironment())
	{
		wstring path = L"/private/var/mobile/Media/PublicStaging/DeveloperDiskImage.dmg";
		CFileOperation::UnZipFile(prerequisitePackagePath,CFileOperation::GetFileDir(prerequisitePackagePath));
		wstring localpath = prerequisitePackagePath.substr(0, prerequisitePackagePath.length()-4);
		wstring localimgpath= localpath + L"\\DeveloperDiskImage.dmg";
		wstring localsignpath= localpath + L"\\DeveloperDiskImage.dmg.signature";
		UploadFileCommand upcmd(_pDeviceData);
		string signdata;
		if(CFileOperation::IsFileExist(localimgpath.c_str()))
		{
			upcmd.Upload(localimgpath, path);
		}
		if(CFileOperation::IsFileExist(localsignpath.c_str()))
		{
			signdata=CFileOperation::ReadFile(localsignpath,true);
		}
		MobileImageMounterClient mounter(_device);
		if(!mounter.MountImage("/private/var/mobile/Media/PublicStaging/DeveloperDiskImage.dmg", signdata))
			DirectoryOperationCommand(_pDeviceData).DeleteFileLink(path);
		
	}
	return true;
}