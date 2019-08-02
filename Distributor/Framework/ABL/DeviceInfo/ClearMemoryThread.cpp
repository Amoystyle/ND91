#include "stdafx.h"
#include "ClearMemoryThread.h"
#include "Module/System/SystemManager.h"

ClearMemoryThread::ClearMemoryThread( string threadID, wstring deviceID )
						: DeviceBaseThread(threadID, deviceID)
{
    _autoDelete = true;
}

ClearMemoryThread::~ClearMemoryThread()
{
}

unsigned long ClearMemoryThread::Main()
{
	this->ClearMemory();

	return 0;
}

void ClearMemoryThread::ClearMemory()
{
	ResponseClearDeviceMemoryMsg msg;
	msg.deviceID = this->_deviceID;

	EnumPhoneOS osType = ePhoneOS_None;
	INT64 totalSize = 0;
	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_deviceID);
	if(pDevice != NULL)
	{
		osType = pDevice->GetDevInfo()->_deviceType == Ios? ePhoneOS_Ios:ePhoneOS_Android;

		vector<wstring> pathList;
		if(GetPathList(pathList) && pathList.size() > 0)
		{
			totalSize = ClearFiles(pathList);	
		}
	}

	msg.nOsType = osType;
	msg.nSize = totalSize;

	RETURN_IF(CheckExit());

	msg.SendToUI();
}

bool ClearMemoryThread::GetPathList( vector<wstring> &pathList )
{
	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_deviceID);
	if(pDevice == NULL)
		return false;

	if(pDevice->GetDevInfo()->_deviceType == Ios)
	{
		pathList.push_back(L"/var/mobile/Media/PublicStaging");
		pathList.push_back(L"/var/mobile/Media/ApplicationArchives");	// Jailbreaked
		pathList.push_back(L"/PublicStaging");
		pathList.push_back(L"/ApplicationArchives");
		pathList.push_back(L"/var/mobile/Media/Downloads");		//AppStore下载安装未完成的资源 Jailbreaked
		pathList.push_back(L"/Downloads");		//AppStore下载安装未完成的资源 No Jailbreaked

		const vector<AppData*>* pDatas = pDevice->GetAppManager()->GetDatas();
		if(pDatas == NULL || pDatas->size() <= 0)
		{
			pDevice->GetAppManager()->Load();
			pDatas  = pDevice->GetAppManager()->GetDatas(); 
		}

		if(pDatas == NULL)
			return true;

		for (size_t i = 0; i < pDatas->size(); i++)
		{
			AppData* pApp = pDatas->at(i);
			if (pApp->_eAppOwner != APP_OWNER_SYS)
			{
				wstring cmp = CStrOperation::toLowerW(pApp->_packageName);
				if(cmp.compare(L"com.pptv.ipadwebkit") == 0 ||				//PPTV ipad
					cmp.compare(L"com.pptv.iphoneapp") == 0 ||				//PPTV iphone
					cmp.compare(L"com.tudou.tudouipad") == 0 ||				//TuDou ipad
					cmp.compare(L"com.tudou.tudouiphone") == 0 ||			//TuDou iphone
					cmp.compare(L"com.xunlei.xunleikankanipadapp4") == 0 ||	//xunlei ipad
					cmp.compare(L"com.xunlei.xunleikankan.iphone") == 0)	//xunlei iphone
				{
					wstring path = CFileOperation::GetFileDir(pApp->_strAppPhonePath) + L"Library/Caches/";
					pathList.push_back(path);
				}
				else if(cmp.compare(L"com.pps.test") == 0)					//PPS
				{
					wstring path = CFileOperation::GetFileDir(pApp->_strAppPhonePath) + L"Library/Caches/ImageCach";
					pathList.push_back(path);

					path = CFileOperation::GetFileDir(pApp->_strAppPhonePath) + L"Library/Caches/ZipFile/";	
					pathList.push_back(path);

				}
			}
		}
	}
	else
	{
		pathList.push_back(L"/data/local/tmp/");	// 软件安装临时目录
		pathList.push_back(L"/data/data/com.pplive.androidphone/cache/webviewCache/");	// PPTV
		pathList.push_back(L"/mnt/sdcard/tudoucache/list/");	//tudou
		pathList.push_back(L"/data/data/com.xunlei.kankan/cache/");	// xunleikankan
		pathList.push_back(L"/mnt/sdcard/.pps/image_cache/");	// PPS SDCard
		pathList.push_back(L"/data/data/tv.pps.mobile/cache/.pps/list_cache/");	// PPS Phone
	}
	return true;
}

INT64 ClearMemoryThread::ClearFiles(vector<wstring> pathList )
{
	INT64 totalSize = 0;

	DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_deviceID);
	if(pDevice == NULL)
		return totalSize;

	for (vector<wstring>::iterator it = pathList.begin(); it != pathList.end(); it++)
	{
		wstring path = *it;

		DirData* pDir = dynamic_cast<DirData*>(FileHelper::GetNode(pDevice->GetRootData()->GetDir(), path));
		if(pDir == NULL)
			continue;

		deque<NodeData*> subNode = pDir->GetChildrens();
		if(subNode.size() <= 0)
			continue;

		vector<NodeData*> delNodes;
		for (size_t i = 0; i < subNode.size(); i++)
		{
			NodeData* pNode = subNode.at(i);

			totalSize += pNode->_size;

			delNodes.push_back(pNode);
		}
		if(delNodes.size() > 0)
			FileHelper::BatchDelete(&delNodes, NULL);/**/
	}
	return totalSize;
}