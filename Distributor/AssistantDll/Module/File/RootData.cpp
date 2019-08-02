#include "stdafx.h"

#include "Module/File/RootData.h"
#include "Module/File/DirData.h"
#include "Module/File/FileHelper.h"
#include "GetSpaceInfoCommand.h"
#include "DirectoryOperationCommand.h"
#include "GetSDCardStateCommand.h"
#include "DownloadFileCommand.h"

#include "Core/DeviceData.h"
#include "Device/IosUsbConnection.h"
#include "Device/IosPublicFunc.h"
#include "Module/Common/IosPList/IosPlistConverter.h"

#include "Common/rapidxml/rapidxml.hpp"
#include "Common/rapidxml/rapidxml_utils.hpp"
#include "Common/Path.h"

using namespace rapidxml;

RootData::RootData(const DeviceData* pDevice)
{
    _pDir = new DirData(pDevice);
    _pDir->_name = L"/";
    _pDir->_Parent = NULL;
	_pDeviceData = pDevice;
}

RootData::~RootData()
{
    SAFE_DELETE(_pDir);
}

DirData* RootData::GetPath(ROOT_TYPE nType, DirData* pSDCardDir, bool reLoad)
{
	NodeData* pNode = NULL;
    DirData* pDir = _pDir;
    if (reLoad)
        pDir->SetHasLoad(false);

    if (_pDeviceData->GetDevInfo()->_deviceType == Ios)
    {
        //TODO Ios特殊目录， 因为Ios没有SdCard，所以有较大差别，下面代码有重复的，可以调成公用
		switch (nType)
		{
		case ROOT:
			return pDir;
			break;
		case DOCUMENT: //文档
			{
				pNode = FileHelper::GetNode(pDir, L"var/mobile/Media/MyDocuments");
			}
			break;
		case PHOTO:		//照片目录
			{
				pNode = FileHelper::GetNode(pDir, L"private/var/mobile/Media/DCIM/100APPLE");
			}
			break;
		case WALLPAPER:		//墙纸目录
			{
				pNode = FileHelper::GetNode(pDir, L"Library/Wallpaper");
			}
			break;
		case RINGRONES:		//铃声目录
			{
				pNode = FileHelper::GetNode(pDir, L"Library/Ringtones");
			}
			break;
		case PANDAREADER:	//熊猫看书
			{
				wstring strPath = GetPandaReaderPath();						//	IPA熊猫看书路径
                if (strPath.empty())
                {
			        DirectoryOperationCommand dircmd(_pDeviceData);
			        if ( dircmd.Exist( L"/private/var/mobile/Media/EBooks" ) )	//  PXL熊猫看书路径
				        strPath = L"/private/var/mobile/Media/EBooks";
                }

				pNode = FileHelper::GetNode(pDir, strPath);
			}
			break;
		default:
			return NULL;
		}
    }
    else if (_pDeviceData->GetDevInfo()->_deviceType == Android)
    {	    
		LOG_DEBUG(L"GetPath  GetSDCARD START");
        if (!pSDCardDir && nType != SDCARD)
        {
            pSDCardDir = GetPath(SDCARD);
			
            if (nType != PHONEDATA)
            {
                RETURN_NULL_IF (!pSDCardDir);
            }            
        }
        if (nType == SDCARD && pSDCardDir)
            return pSDCardDir;
		//LOG_DEBUG(L"GetPath  GetSDCARD SUCCESS");

	    switch (nType)
	    {
	    case ROOT:			//根目录
		    return _pDir;
		    break;
	    case PHONEDATA:
		    pNode = pDir->findChildrenbyName(L"data");
		    break;
	    case SDCARD:
		    {
			    GetSDCardStateCommand cmd(_pDir->_pDeviceData);
			    cmd.Execute();
				LOG_DEBUG(cmd.GetSDCardState() == MOUNTED?L"GetPath  GetSDCARD success":L"GetPath  GetSDCARD fail");
			    //if (cmd.GetSDCardState() != MOUNTED)
				//    return NULL;

			    pNode = pDir->findChildrenbyName(L"sdcard");
				//if(!pNode)
				//	pNode = pDir->findChildrenbyName(L"mnt/sdcard");
				 if (cmd.GetSDCardState() != MOUNTED&&!pNode)
					  return NULL;

		    }
		    break;
	    case DOCUMENT:		//文档
		    {
			    pNode = pSDCardDir->findChildrenbyName(L"My Documents");
		    }
		    break;
	    case PHOTO:			//照片目录
		    {
                pSDCardDir->Refresh();
                // 照片  "DCIM/Camera", "DCIM/100MEDIA", "Pictures/Camera" 
                //pNode = pSDCardDir->findChildrenbyName(L"DCIM/Camera");
                pNode = FileHelper::GetNode(pSDCardDir, L"DCIM/Camera");
                if(!pNode)
                    //pNode = pSDCardDir->findChildrenbyName(L"DCIM/100MEDIA");
                    pNode = FileHelper::GetNode(pSDCardDir, L"DCIM/100MEDIA");
                if(!pNode)
                    //pNode = pSDCardDir->findChildrenbyName(L"Pictures/Camera");
                    pNode = FileHelper::GetNode(pSDCardDir, L"Pictures/Camera");
                if(!pNode)
                    //pNode = pSDCardDir->findChildrenbyName(L"DCIM/100ANDRO");
                    pNode = FileHelper::GetNode(pSDCardDir, L"DCIM/100ANDRO");
		    }
		    break;
	    case WALLPAPER:		//墙纸目录
		    {
			    pNode = pSDCardDir->findChildrenbyName(L"wallpaper");
		    }
		    break;
	    case RINGRONES:		//铃声目录
		    {
			    pNode = pSDCardDir->findChildrenbyName(L"ringtones");
		    }
		    break;
	    case MUSIC:			//音乐目录
		    {
			    pNode = pSDCardDir->findChildrenbyName(L"music");
		    }
		    break;
	    case PANDAREADER:	//熊猫看书
		    {
			    pNode = pSDCardDir->findChildrenbyName(L"91PandaReader");
				LOG_DEBUG(pNode?L"GetPath  PANDAREADER SUCCESS":L"GetPath  PANDAREADER fail");
		    }
		    break;
	    default:
		    return NULL;
	    }

	    /*if(pNode && pNode->_type == NodeData::LINK)
	    {
		    LinkData* link = dynamic_cast<LinkData*>(pNode);
		    pNode = link->GetTarget();
	    }*/	   
    }

	DirData* pFoundDir = dynamic_cast<DirData*>(pNode);
	if (pFoundDir)
		pFoundDir->SetRootType(nType);

	return pFoundDir;
}

// 获取指定路径的目录
DirData* RootData::GetPath(wstring path, DirData* pParentDir)
{
    vector<wstring> temp = CStrOperation::parseStringsW(path, L"/");
    RETURN_NULL_IF(temp.empty());
    NodeData* pNode = pParentDir->findChildrenbyName(temp.at(0));
    DirData* pDir = dynamic_cast<DirData*>(pNode);
    DirData* pSubDir = NULL;
    if (pDir)
    {
        wstring strTemp;
        for (size_t j = 1; j < temp.size(); j++)
           strTemp += L"/" + temp.at(j);
        strTemp += L"/";
        pSubDir = GetPath(strTemp, pDir);
    }
    
    if (!pSubDir)
        return pDir;
    else
        return pSubDir;
}

int RootData::GetFolderTotalSize(vector<wstring> paths, wstring ExtName)
{
    int numberSum = 0;
    int sizeSum = 0;
    for (size_t i = 0; i < paths.size(); i++)
    {
        DirData* pDir = GetPath(paths.at(i), _pDir);
        if (pDir)
            pDir->RecursiveLoad(sizeSum, numberSum, ExtName);
    }

    return sizeSum;
}

bool RootData::GetSpaceInfo(ROOT_TYPE nType, SpaceInfo* pInfo)
{
	LOG->WriteDebug(L"GetSpaceInfo begin type="+ CStrOperation::IntToWString(nType));
	pInfo->_path = L"";
	pInfo->_totalsize = 0;
	pInfo->_usedsize = 0;
	pInfo->_Available = 0;
	if (nType != PHONEDATA && nType != SDCARD)
	{
		LOG->WriteDebug(L"GetSpaceInfo OK!");
		return false;
	}

	DirData* pDirData = GetPath(nType);
	if (pDirData)
	{
		pInfo->_path = pDirData->GetFullPath();
		GetSpaceInfoCommand spacecmd(pDirData->_pDeviceData);
		 //有些手机sdcard空间信息获取不到，目前文件管理走的不是守护，这边先写死（这是是照着GetSDCardState的做法做的）。
		if(nType == RootData::SDCARD)
			spacecmd.setPath(L"sdcard");
		else
			spacecmd.setPath(pDirData->GetFullPath());
		spacecmd.Execute();
		deque<SpaceInfo> spaceinfos = spacecmd.GetDirInfos();

		for(size_t i =0; i < spaceinfos.size(); i++)
		{
			pInfo->_totalsize += spaceinfos[i]._totalsize;
			pInfo->_usedsize  += spaceinfos[i]._usedsize;
			pInfo->_Available  += spaceinfos[i]._Available;
		}
		LOG->WriteDebug(L"GetSpaceInfo OK!");
		return true;
	}
	else if (_pDeviceData && _pDeviceData->GetDevInfo()->_deviceType == Ios && _pDeviceData->GetDevInfo()->_connectType == USB && nType == PHONEDATA)
	{
		IosUsbConnection iosUsb((DeviceInfo*)_pDeviceData->GetDevInfo());
		IosDiskMemoInfo diskInfo;
		if (iosUsb.GetDiskMemoInfo(diskInfo))
		{
            pInfo->_totalsize = (diskInfo.nTotalDiskCapacity);			
            pInfo->_Available  = diskInfo.nTotalDataAvailable;
			pInfo->_usedsize  = pInfo->_totalsize - pInfo->_Available;
            pInfo->_applicationSize = diskInfo.nMobileApplicationUsage;

            pInfo->_systemSize = diskInfo.nTotalSystemCapacity;         //系统空间
            pInfo->_systemAvailable = diskInfo.nTotalSystemAvailable;    //系统可用空间
            pInfo->_systemUsed = pInfo->_systemSize - pInfo->_systemAvailable;    //系统已用空间

            pInfo->_nAmountDataAvailable = diskInfo.nAmountDataAvailable;
            pInfo->_nAmountDataReserved = diskInfo.nAmountDataReserved;
			LOG->WriteDebug(L"GetSpaceInfo OK!");
			return true;
		}
	}
	LOG->WriteDebug(L"GetSpaceInfo OK!");
	return false;
}

#define IOSJAIBREAKEDPATH   wstring(L"/private/var/mobile/Media")
#define IOSAUDIOPATH	    L"/iTunes_Control"
#define IOSVIDEOPATH	    L"/iTunes_Control/Music"
#define IOSCAMARAPATH	    L"/DCIM"
#define IOSPHOTOPATH	    L"/Photos"
#define IOSBOOKPATH         L"/Books/Purchases"

int64_t RootData::GetIosAudioUsage()
{
    wstring path;
    if (_pDeviceData->GetDevInfo()->_ios_bJailBreaked)
        path = IOSJAIBREAKEDPATH + IOSAUDIOPATH;
    else
        path = IOSAUDIOPATH;

    vector<wstring> paths;
    paths.push_back(path);
    return GetFolderTotalSize(paths, L".mp3");
}

int64_t RootData::GetIosVideoUsage()
{
    wstring path;
    if (_pDeviceData->GetDevInfo()->_ios_bJailBreaked)
        path = IOSJAIBREAKEDPATH + IOSVIDEOPATH;
    else
        path = IOSVIDEOPATH;

    vector<wstring> paths;
    paths.push_back(path);
    return GetFolderTotalSize(paths, L".mp4");
}

int64_t RootData::GetIosCamaraUsage()
{
    wstring path;
    if (_pDeviceData->GetDevInfo()->_ios_bJailBreaked)
        path = IOSJAIBREAKEDPATH + IOSCAMARAPATH;
    else
        path = IOSCAMARAPATH;

    vector<wstring> paths;
    paths.push_back(path);
    return GetFolderTotalSize(paths);
}

int64_t RootData::GetIosPhotoUsage()
{
    wstring path;
    if (_pDeviceData->GetDevInfo()->_ios_bJailBreaked)
        path = IOSJAIBREAKEDPATH + IOSPHOTOPATH;
    else
        path = IOSPHOTOPATH;

    vector<wstring> paths;
    paths.push_back(path);
    return GetFolderTotalSize(paths);
}

int64_t RootData::GetIosBookUsage()
{
    wstring path;
    if (_pDeviceData->GetDevInfo()->_ios_bJailBreaked)
        path = IOSJAIBREAKEDPATH + IOSBOOKPATH;
    else
        path = IOSBOOKPATH;

    if (IosPublicFunc::GetVerNum(_pDeviceData->GetDevInfo()->_deviceProductVersion) <= 313)
        return 0;

    vector<wstring> paths;
    paths.push_back(path);
    return GetFolderTotalSize(paths);
}

std::wstring RootData::GetPandaReaderPath()
{
	Path gPath(_pDeviceData->GetDevInfo());
	wstring path = gPath.GetWorkFolder() + L"installation.plist";
	DownloadFileCommand cmd(_pDeviceData);
	cmd.Download( path ,L"/private/var/mobile/Library/Caches/com.apple.mobile.installation.plist");

	RETURN_VALUE_IF ( !CFileOperation::IsFileExist(path.c_str()), L"");
	if ( MyFileOperation::IsIosPlist(path) )
		IosPlistConverter::GetXmlFromPlist(path, path);          //.Plist转XML

	file<> fdoc( CCodeOperation::UnicodeToUTF_8(path).c_str() );
	xml_document<char> doc;
	doc.parse<0>(fdoc.data());
	RETURN_FALSE_IF(fdoc.data() == NULL);

	bool bFind = false, bGet = false;
	std::string strName, strValue;
    vector<string> verKeys;
	GetRealKey(_pDeviceData->GetDevInfo(), verKeys);

	rapidxml::xml_node<char> * root = doc.first_node();            //xml文件的根节点
	rapidxml::xml_node<char> * scnode = root->first_node();        //根节点的子节点
	rapidxml::xml_node<char> * chnode = scnode->last_node();       //子节点最后一个节点

	//modify by wengcunmin 添加对6.0的支持
	while( chnode && !bFind  )	
	{
		for ( int i=0;i<(int)verKeys.size(); i++ )
		{
			if ( bFind ) break;

			for (rapidxml::xml_node<char> * node = chnode->first_node(); node!=NULL; node = node->next_sibling())
			{
				if ( bFind ) break;

				strName = node->name();
				strValue = node->value();

				if ( "key" == strName && verKeys[i] == strValue )
				{
					rapidxml::xml_node<char> * nextNode = node->next_sibling("dict");
					for (rapidxml::xml_node<char> * pNode = nextNode->first_node(); pNode!=NULL; pNode = pNode->next_sibling())
					{
						strName = pNode->name();
						strValue = pNode->value();
						if ( "key" == strName && "Container" == strValue )
						{
							bGet = true;
							continue;
						}
						if ( "string" == strName && bGet )
						{
							strValue = pNode->value();
							bFind = true;
							break;
						}
					}
				}
			}
		}		
		chnode = chnode->previous_sibling();
	}
	
    RETURN_VALUE_IF(!bFind, L"");
	return CCodeOperation::GB2312ToUnicode(strValue) + L"/Documents";;
}


void RootData::GetRealKey(  const DeviceInfo* pInfo, vector<string>& verKeys )
{
	RETURN_IF ( pInfo->_deviceType != Ios );

	if ( pInfo->IsPad() )
	{
		if ( pInfo->_ios_bJailBreaked )
		{
			verKeys.push_back("com.91.91PandaReaderHD");				// ipad的非appstore版本
			verKeys.push_back("com.91.91readerHD");						// ipad的appstore版本
			verKeys.push_back("NetDragon.Mobile.iPhone.PandaReader");	// iphone的非appstore版本
			verKeys.push_back("com.nd.pandareader");					// iphone的appstore版本	
		}
		else
		{
			verKeys.push_back("com.91.91readerHD");
			verKeys.push_back("com.nd.pandareader");
		}
	}
	else
	{
		if ( pInfo->_ios_bJailBreaked )
		{
			verKeys.push_back("NetDragon.Mobile.iPhone.PandaReader");
			verKeys.push_back("com.nd.pandareader");		
		}
		else
		{
			verKeys.push_back("com.nd.pandareader");
		}
	}

}

bool RootData::GetAndroidSpaceInfo(wstring path, int64_t& used, int64_t &available)
{
	LOG->WriteDebug(L"GetAndroidSpaceInfo begin!");
    GetSpaceInfoCommand spacecmd(_pDeviceData);
    spacecmd.setPath(path);
    spacecmd.Execute();
    deque<SpaceInfo> spaceinfos = spacecmd.GetDirInfos();

    if (spaceinfos.empty())
    {
        used = 0;
        available = 0;
		LOG->WriteDebug(L"GetAndroidSpaceInfo fail!");
        return false;
    }
    else
    {
        used = spaceinfos[0]._usedsize;
        available = spaceinfos[0]._Available;
		LOG->WriteDebug(L"GetAndroidSpaceInfo sucess!");
        return true;
    }
}
