#include "pch_module.h"

#ifdef MODULE_VIDEO

#include "Module/Video/PlayAlbumData.h"
#include "Module/Video/VideoData.h"
#include "Module/Video/AndroidVideoXmlHelper.h"
#include "Common/Path.h"

PlayAlbumData::PlayAlbumData(void)
{
	InitParams();	
}

PlayAlbumData::~PlayAlbumData(void)
{
	for(size_t i = 0; i < _playItemList.size(); i++)
	{
		SAFE_DELETE(_playItemList[i]);
	}
	_playItemList.clear();
	SAFE_DELETE(_pdir);
}

ND91Assistant::PlayerAlbumItem::PlayerAlbumItem()
{
	InitParams();
}

void ND91Assistant::PlayerAlbumItem::InitParams()
{
	_pVideo = NULL;           // 对应视频

	_fileId = L"";           // 文件id, id为空代表下面数据均无效
	_fileDisplayName = L"";  // 文件在播放列表中显示的名字
	_fileUrl = L"";          // 截图url路径
	_filePath = L"";         // 文件在手机端路径
	_fileSize = 0;         // 文件大小
	_fileOrder = 0;        // 文件排序
	_fileType = L"";         // 文件类型
	_downUrl = L"";          // web地址url路径
	_fileLocalPath = L"";    // 文件下载到端路径

	_urlImageLocalPath = L"";

	_albumId = L"";          // 专辑id,从url获得
	_albumName = L"";        // 专辑名,从url获得
}

ND91Assistant::PlayerAlbumItem::PlayerAlbumItem( const PlayerAlbumItem* albumItem )
{
	InitParams();

	RETURN_IF(!albumItem);

    _pVideo  = albumItem->_pVideo ? new VideoData(albumItem->_pVideo) : NULL;


	_fileId  = albumItem->_fileId;           
	_fileDisplayName = albumItem->_fileDisplayName;  
	_fileUrl = albumItem->_fileUrl;          
	_filePath = albumItem->_filePath;         
	_fileSize = albumItem->_fileSize;         
	_fileOrder = albumItem->_fileOrder;        
	_fileType = albumItem->_fileType;         
	_downUrl = albumItem->_downUrl;         
	_fileLocalPath = albumItem->_fileLocalPath;    

	_albumId = albumItem->_albumId;
	_albumName = albumItem->_albumName;

}

ND91Assistant::PlayerAlbumItem::~PlayerAlbumItem()
{
	SAFE_DELETE(_pVideo);
}

PlayerAlbumItem* ND91Assistant::PlayAlbumData::FindItemByPath( wstring remotepath )
{
	PlayerAlbumItem* item = NULL;
	for(size_t k =0; k< _playItemList.size(); k++)
	{
		if(_playItemList[k]->_filePath == remotepath && !_playItemList[k]->_filePath.empty())
		{
			item = _playItemList[k];
			break;
		}
	}
	return item;
}

void ND91Assistant::PlayAlbumData::RemoveItemFromListByPath( wstring remotepath )
{
	for(deque<PlayerAlbumItem*>::iterator it = _playItemList.begin(); it != _playItemList.end(); it++)
	{
		if((*it)->_filePath == remotepath && !(*it)->_filePath.empty())
		{
			SAFE_DELETE((*it));
			_playItemList.erase(it);
			break;
		}
	}
}

void ND91Assistant::PlayAlbumData::ReWritePlayAlbumXml( const DeviceData* pDevice )
{
    RETURN_IF(pDevice->GetDevInfo()->_deviceType != Android);
    RETURN_IF(_ePlayAlbumType != ePlayAlbumType_91Player_With_Album);
	wstring path = pDevice->GetPath()->GetLocalVideoPath() + CFileOperation::GetFileName(_xmlremotePath);
    CFileOperation::DeleteFile(path);

	AndroidVideoXmlHelper::CreatePlayerAlbumXMLInfo(this, path);
	if(CFileOperation::IsFileExist(path.c_str()))
	{
		DirectoryOperationCommand dirCmd(pDevice);
		dirCmd.DeleteFileLink(_xmlremotePath);

		UploadFileCommand  upfilecmd(pDevice);
		upfilecmd.setfilename(path, _xmlremotePath);
		upfilecmd.Execute();
		if(upfilecmd.GetUploadResult() == Transfer_Success)
		{
            CFileOperation::DeleteFile(path);
		}
	}
}

ND91Assistant::PlayAlbumData::PlayAlbumData( PlayAlbumData* pAlbum )
{
	InitParams();

	_ePlayAlbumType = pAlbum->_ePlayAlbumType;
	_albumId	= pAlbum->_albumId;     // 专辑id
	_albumName	= pAlbum->_albumName;   // 专辑名
	_count		= pAlbum->_count;		// 总共集数
	_updata		= pAlbum->_updata;      // 更新至第几集
	_type		= pAlbum->_type;        // 类型，1电影，2电视剧，3综艺节目，6
	_showingTime = pAlbum->_showingTime; // 上映时间
	_director	= pAlbum->_director;	// 导演
	_actor		= pAlbum->_actor;		// 演员
	_introduce	= pAlbum->_introduce;	// 专辑简介
	_albumUrl	= pAlbum->_albumUrl;    // 专辑图片url

	_xmlremotePath = pAlbum->_xmlremotePath;//专辑xml文件

	_AlbumImagelocalSavePath = pAlbum->_AlbumImagelocalSavePath;

	for (size_t i = 0; i<pAlbum->_playItemList.size(); i++)
	{
		_playItemList.push_back(new PlayerAlbumItem(pAlbum->_playItemList[i]));
	}
}

void ND91Assistant::PlayAlbumData::InitParams()
{
	_ePlayAlbumType = ePlayAlbumType_91Player_With_Album;
	 _albumId = L"";     // 专辑id
	_albumName = L"";   // 专辑名
	_count = L"";		// 总共集数
	_updata = L"";      // 更新至第几集
	_type = 0;        // 类型，1电影，2电视剧，3综艺节目，6
	_showingTime = L""; // 上映时间
	_director = L"";	// 导演
	_actor = L"";		// 演员
	_introduce = L"";	// 专辑简介
	_albumUrl = L"";    // 专辑图片url

	_xmlremotePath = L"";//专辑xml文件

	_AlbumImagelocalSavePath = L"";

	_pdir	 = NULL;
}

ND91Assistant::PlayAlbumData::PlayAlbumData(const DeviceData* pDevice, wstring path)
{
    InitParams();
    if (pDevice->GetDevInfo()->_deviceType == Android)
    {
	    _pdir = new DirData(pDevice);
	    _pdir->_name = CStrOperation::trimRightW(path,L"/");
	    _type    = 0; 
	    _albumName = CFileOperation::GetFileName(_pdir->_name);
	    Init();
    }
}

void ND91Assistant::PlayAlbumData::Init()
{
	_pdir->Refresh();
	deque<NodeData*> childrens = _pdir->GetChildrens();
	for (size_t i = 0; i < childrens.size(); i++)
	{
		if(childrens[i]->_type == NodeData::FILE)
		{
			wstring ext = CFileOperation::GetFileExt(childrens[i]->_name);
			if(ext == L"jpg")
			{
				_albumImageremotePath = childrens[i]->GetFullPath();
			}
			else if(ext == L"xml" && _pdir->_pDeviceData->GetDevInfo()->_deviceType == Android)
			{
				wstring localpath = _pdir->_pDeviceData->GetPath()->GetLocalVideoPath()+childrens[i]->_name;
                CFileOperation::DeleteFile(localpath);

				DownloadFileCommand downfileCmd(_pdir->_pDeviceData);
				if(downfileCmd.Download(localpath, childrens[i]->GetFullPath()))
				{
					AndroidVideoXmlHelper::GetPlayerAlbum(localpath, this);
					_xmlremotePath = childrens[i]->GetFullPath();
                    CFileOperation::DeleteFile(localpath);
				}
			}
			else if(IsValidVideo(childrens[i]->_name))
			{
				bool typeIos = (_pdir->_pDeviceData->GetDevInfo()->_deviceType == Ios);
				if((!typeIos && !FindItemByPath(childrens[i]->GetFullPath()))|| typeIos)
				{
					PlayerAlbumItem* albumitem = new PlayerAlbumItem();
					albumitem->_filePath = childrens[i]->GetFullPath();
					_playItemList.push_back(albumitem);
				}
			}
		}
	}
}

std::wstring ND91Assistant::PlayAlbumData::GetAlbumPath()
{
	if(_pdir)
		return _pdir->_name + L"/";
	else
	{
		if(!_albumImageremotePath.empty())
			return CStrOperation::trimRightW(CFileOperation::GetFileDir(_albumImageremotePath),L"/");
		if((int)_playItemList.size()>0)
		{
			return CStrOperation::trimRightW(CFileOperation::GetFileDir(_playItemList[0]->_filePath),L"/");
		}
	}
	return L"";
}

void ND91Assistant::PlayAlbumData::UpdateInfo( PlayAlbumData* pAlbum )
{
    _albumId	= pAlbum->_albumId;     // 专辑id
    _albumName	= pAlbum->_albumName;   // 专辑名
    _count		= pAlbum->_count;		// 总共集数
    _updata		= pAlbum->_updata;      // 更新至第几集
    _type		= pAlbum->_type;        // 类型，1电影，2电视剧，3综艺节目，6
    _showingTime = pAlbum->_showingTime; // 上映时间
    _director	= pAlbum->_director;	// 导演
    _actor		= pAlbum->_actor;		// 演员
    _introduce	= pAlbum->_introduce;	// 专辑简介
    _albumUrl	= pAlbum->_albumUrl;    // 专辑图片url
    _xmlremotePath = pAlbum->_xmlremotePath;//专辑xml文件
}

bool PlayAlbumData::IsValidVideo(wstring strName)
{
	wstring supportType = L"mp4,m4v,wmv,3gp,ndv";
	if(_pdir->_pDeviceData->GetDevInfo()->_deviceType == Ios)
		//supportType = L"mp4,m4v,wmv,3gp,m4r,wav,aac";
        supportType = L"mp4,mpv,3gp,mov,ndv";

	return CStrOperation::ContainsInW(supportType,CStrOperation::toLowerW(CFileOperation::GetFileExt(strName)),L",");
}


#endif