#include "stdafx.h"
#ifdef MODULE_VIDEO

#define ND91PLayer		 L"com.nd.android.player"//"com.nd.android.pandareader";
#define ND91PLayerAPK	 L"com.nd.android.player.apk"//"com.nd.android.pandareader";
#define ND91PLayer_VERSION L"1"
#define ND91PLayerTempPath L"/91 PC Suite/temp/91player/"
#define SUPPORT_VIDEO_FORMAT  L"mp4,m4v,wmv,3gp"

VideoManager::VideoManager(const DeviceData* pDevice)
{
	_pDevice = pDevice;
	_sdcardPath = L"";
	_noSdcard = false;
}

VideoManager::~VideoManager()
{
	Release();
}

void ND91Assistant::VideoManager::Load()
{
    Release();

    LoadAllVideos();

	// 无熊猫影音的视频则无需加载专辑信息
	RETURN_IF(GetSdcardPath().empty() || _p91playerVideoList.size() < 1 );

 	Load91PlayerAlbum();

	Init91PlayerAlbum();
}

void VideoManager::LoadAllVideos()
{
	LoadVideoCommand cmd(_pDevice);
	cmd.Execute();

	const deque<VideoData*> videolist = cmd.GetVideoList();
	for (size_t i=0; i< videolist.size();i++)
	{
		wstring remote = videolist[i]->_remotepath;
		remote = CStrOperation::toLowerW(remote);
		if ((int)remote.find(GetSdcardPath() + L"91player/") == 0)
		{
			InsertOneVideoToList(videolist[i], _p91playerVideoList, false);
		}
		else
			InsertOneVideoToList(videolist[i], _pMyVideoList, false);
	}
}

void VideoManager::Init91PlayerAlbum()
{
	// video放入对应专辑
	for (size_t i = 0; i< _p91playerVideoList.size(); i++)
	{
		if ((int)_p91playerVideoList[i]->_remotepath.find(GetAlbumRootPath()) != 0)
			continue;

		wstring videopath = CFileOperation::GetFileDir(_p91playerVideoList[i]->_remotepath);
		videopath = CStrOperation::trimRightW(videopath, L"/");
		for(size_t j =0 ; j < _playAlbumList.size(); j++)
		{
			int count = _playAlbumList[j]->_playItemList.size();

			wstring albumpath = _playAlbumList[j]->GetAlbumPath();
			if(videopath == albumpath)// 属于此专辑
			{
				_p91playerVideoList[i]->_pOwnAlbum = _playAlbumList[j];
				PlayerAlbumItem* founditem = _playAlbumList[j]->FindItemByPath(_p91playerVideoList[i]->_remotepath);
				if (founditem)
					founditem->_pVideo = _p91playerVideoList[i];
				break;
			}
		}
	}
}
void ND91Assistant::VideoManager::Load91PlayerAlbum()
{
	RETURN_IF(GetSdcardPath().empty());

    DirData*  pDir = new DirData(_pDevice);
	pDir->_name   = CStrOperation::trimRightW(GetAlbumRootPath(),L"/");
	pDir->Refresh();
	Load91PlayerAlbum( pDir);
	SAFE_DELETE(pDir);
}

void ND91Assistant::VideoManager::Load91PlayerAlbum( DirData* pDir )
{
	DownloadFileCommand  downFile(_pDevice);
	for (size_t i = 0; i < pDir->GetChildrens().size(); i++)
	{
		switch(pDir->GetChildrens()[i]->_type)
		{
		case NodeData::DIR:
			{
				PlayAlbumData* playalbum = new PlayAlbumData(_pDevice, pDir->GetChildrens()[i]->GetFullPath());
				if (!playalbum->_playItemList.empty() || !playalbum->_albumImageremotePath.empty())
					_playAlbumList.push_back(playalbum);
			}
			//	Load91PlayerAlbum(dynamic_cast<DirData*>(pDir->GetChildrens()[i]));
			break;
		/*case NodeData::FILE:
			if (_pDevice->GetDevInfo()->_connectType == Android && (int)(pDir->GetChildrens()[i]->_name.find(L".xml")) > -1)
			{
				wstring localpath = _pDevice->GetPath()->GetLocalVideoPath()
                    + pDir->GetChildrens()[i]->_name;

				::DeleteFile(localpath.c_str());

                if (downFile.Download(localpath, pDir->GetChildrens()[i]->GetFullPath()))
				{
					PlayAlbumData* albumdata= AndroidVideoXmlHelper::GetPlayerAlbum(localpath);
					if (albumdata)
					{
						albumdata->_xmlremotePath = pDir->GetChildrens()[i]->GetFullPath();
						_playAlbumList.push_back(albumdata);
					}
					::DeleteFile(localpath.c_str());
				}
			}
			break;*/
		default:
			break;
		}
	}
}

void ND91Assistant::VideoManager::GetVideoImages( const deque<VideoData*>* videoList )
{
	RETURN_IF(!videoList || videoList->size()<1);

    VideoOperationCommand  cmd(_pDevice);
	cmd.GetVideoImages(videoList);
}

void ND91Assistant::VideoManager::Release()
{
	for (size_t i = 0; i< _pMyVideoList.size(); i++)
	{
		SAFE_DELETE(_pMyVideoList[i]);
	}
	_pMyVideoList.clear();

	for(size_t i = 0 ; i< _playAlbumList.size(); i++)
	{
		SAFE_DELETE(_playAlbumList[i]);
	}
	_playAlbumList.clear();

	for (size_t i=0; i< _p91playerVideoList.size();i++)
	{
		SAFE_DELETE(_p91playerVideoList[i]);
	}
	_p91playerVideoList.clear();
}

bool ND91Assistant::VideoManager::IsNeedInstall91Player()
{
#ifdef MODULE_APP	
	GetAppInfoCommand  cmd(_pDevice, ND91PLayer);
	cmd.Execute();
	const AppData* data = cmd.GetAppInfo();
	if(cmd.Success() && !data->_strVersion.empty() )
	{
		wstring versionstr = data->_strVersion;
		if (versionstr.compare(ND91PLayer_VERSION)>-1)
			return false;
	} 
	return true;
#else
    return false;
#endif
}

void ND91Assistant::VideoManager::DeleteVideo( const deque<VideoData*>* videoList )
{
	VideoOperationCommand cmd(_pDevice);

	UploadFileCommand  upfilecmd(_pDevice);

	deque<PlayAlbumData*>  modifyedAlbum;
	for (size_t i = 0; i < videoList->size(); i++)
	{
		bool success = cmd.RemoveVideo(videoList->at(i));
		if (!success)
			continue;
		if(videoList->at(i)->_pOwnAlbum)
		{
			videoList->at(i)->_pOwnAlbum->RemoveItemFromListByPath(videoList->at(i)->_remotepath);
			modifyedAlbum.push_back(videoList->at(i)->_pOwnAlbum);
			RemoveVideoFromList( videoList->at(i), _p91playerVideoList);
		}
		else
		{
			success = RemoveVideoFromList( videoList->at(i), _pMyVideoList);
			if (!success)
				RemoveVideoFromList( videoList->at(i), _p91playerVideoList);
		}
	}
	for (size_t i = 0; i < modifyedAlbum.size(); i++)
	{
		modifyedAlbum[i]->ReWritePlayAlbumXml(_pDevice);
	}
}

bool ND91Assistant::VideoManager::RemoveVideoFromList( VideoData* video, deque<VideoData*>& fromList)
{
	RETURN_FALSE_IF(!video);
	for(deque<VideoData*>::iterator it = fromList.begin(); it != fromList.end(); it++)
	{
		if((*it)->_id == video->_id)
		{
			SAFE_DELETE((*it));
			fromList.erase(it);
			return true;
		}
	}
	return false;
}

void ND91Assistant::VideoManager::AddMyVideos( const deque<wstring> localVideoPaths, ProgressObserver* pObserver )
{
	RETURN_IF(GetSdcardPath().empty());
	wstring remotepath = GetSdcardPath() + L"movie/";

	for(size_t i = 0; i < localVideoPaths.size(); i++)
	{
		AddVideo(localVideoPaths[i], remotepath+CFileOperation::GetFileName(localVideoPaths[i]), _pMyVideoList, pObserver);
	}
}

void ND91Assistant::VideoManager::Add91PlayerVideos( const deque<wstring> localVideoPaths, ProgressObserver* pObserver )
{
	RETURN_IF(GetSdcardPath().empty());
	wstring remotepath = GetSdcardPath() + L"91Player/Video/";

	for(size_t i = 0; i < localVideoPaths.size(); i++)
	{
		AddVideo(localVideoPaths[i], remotepath+CFileOperation::GetFileName(localVideoPaths[i]), _p91playerVideoList, pObserver);
	}
}

bool ND91Assistant::VideoManager::AddVideo( const wstring localVideoPath, wstring remotePath, deque<VideoData*>& toList, ProgressObserver* pObserver )
{
	RETURN_FALSE_IF(localVideoPath.empty() || !CFileOperation::IsFileExist(localVideoPath.c_str()));

	int pos = localVideoPath.find_last_of(L".");
	wstring ext =  pos>-1?localVideoPath.substr(pos+1):L"";
	// 判断不是支持的类型
	RETURN_FALSE_IF(!CStrOperation::ContainsInW(SUPPORT_VIDEO_FORMAT,CStrOperation::toLowerW(ext),L","));

    wstring name = remotePath;
    remotePath = CFileOperation::GetFileDir(remotePath);
    
	if(FileHelper::Upload(_pDevice, localVideoPath, remotePath, pObserver))
	{        
        DirectoryOperationCommand cmd(_pDevice);
        cmd.Rename(remotePath + CFileOperation::GetFileName(localVideoPath), name);
		// 添加
		VideoOperationCommand  VOCmd(_pDevice);
		VideoData* video = VOCmd.AddVideo(localVideoPath, name);
        if (video)
        { 
		    RemoveVideoFromList(video, toList);
		    toList.push_back(new VideoData(video));
		    return true;
        }
	}
	return false;
}

bool ND91Assistant::VideoManager::InsertOneVideoToList( VideoData* video, deque<VideoData*>& ToList, bool del)
{
	RETURN_FALSE_IF(!video);
	bool found = false;
	for(deque<VideoData*>::iterator it = ToList.begin(); it != ToList.end(); it++)
	{
		if(video->_remotepath == (*it)->_remotepath)
		{
			if(del)
			{
				SAFE_DELETE((*it));
				ToList.erase(it);
			}
			found = true;
			break;
		}
	}
	if (del || !found)
		ToList.push_back(new VideoData(video));
	return found;
}

void ND91Assistant::VideoManager::Add91PlayerAlbumVideo( PlayerAlbumItem* pAlbumItem, PlayAlbumData* palbum, ProgressObserver* pObserver)
{
	RETURN_IF(!pAlbumItem || GetSdcardPath().empty());

	// 专辑文件夹路径
	wstring rootpath = GetAlbumRootPath()+pAlbumItem->_albumName+L"/";

	// 找到原有专辑
	PlayAlbumData*  oldALbum = FindPlayAlbumById(pAlbumItem->_albumId);

	// 专辑不存在，返回
	RETURN_IF(!palbum && !oldALbum);

	PlayAlbumData* albumData = NULL;
	if (palbum)
		albumData = new PlayAlbumData(palbum);

	PlayerAlbumItem* albumItem = new PlayerAlbumItem(pAlbumItem);
	albumItem->_filePath = rootpath 
                            + CFileOperation::GetFileNameNoExt(albumItem->_fileDisplayName) 
                            + CStrOperation::IntToWString(albumItem->_fileOrder)
                            + L"."+CStrOperation::trimLeftW(albumItem->_fileType,L".");
	if (albumData)
	{
		if(oldALbum)
		{
			// 旧专辑里的列表项加入到新专辑
			for (size_t i =0 ; i < oldALbum->_playItemList.size(); i++)
			{
				if (albumItem->_fileId != oldALbum->_playItemList[i]->_fileId)
					albumData->_playItemList.push_back(new PlayerAlbumItem(oldALbum->_playItemList[i]));
			}
			// 移除原专辑
			for (deque<PlayAlbumData*>::iterator it=_playAlbumList.begin(); it!=_playAlbumList.end();it++)
			{
				if (oldALbum->_albumId == (*it)->_albumId)
				{
					SAFE_DELETE((*it));
					_playAlbumList.erase(it);
					break;
				}
			}
		}
		// 新加项装入新专辑
		albumData->_playItemList.push_back(albumItem);
		_playAlbumList.push_back(albumData);
		albumData->_xmlremotePath = rootpath + albumData->_albumName+L".xml";
	}
	else if(oldALbum)
	{
		// 新加项加入所属专辑
		oldALbum->RemoveItemFromListByPath(albumItem->_filePath);
		oldALbum->_playItemList.push_back(albumItem);
		albumData = oldALbum;
	}

	// 专辑xml文件写回
	albumData->ReWritePlayAlbumXml(_pDevice);

	UploadFileCommand upfileCmd(_pDevice);
    if (!albumData->_AlbumImagelocalSavePath.empty())
    {
	    upfileCmd.setfilename(albumData->_AlbumImagelocalSavePath, rootpath+albumItem->_albumName+L".jpg");
	    upfileCmd.Execute();
    }

    if (!albumItem->_urlImageLocalPath.empty())
    {
	    upfileCmd.setfilename(albumItem->_urlImageLocalPath, rootpath+albumItem->_albumName+
							L"_"+CFileOperation::GetFileNameNoExt(albumItem->_fileDisplayName)+L".jpg");
	    upfileCmd.Execute();
    }

	// 添加视频
	if(!albumItem->_fileLocalPath.empty() && CFileOperation::IsFileExist(albumItem->_fileLocalPath.c_str()))
	{
		DirectoryOperationCommand dircmd(_pDevice);
		dircmd.DeleteFileLink(albumItem->_filePath);
		bool success = AddVideo(albumItem->_fileLocalPath
            , albumItem->_filePath
            , _p91playerVideoList, pObserver);
		if(success)
		{
			albumItem->_pVideo = _p91playerVideoList[_p91playerVideoList.size()-1];
			_p91playerVideoList[_p91playerVideoList.size()-1]->_pOwnAlbum = albumData;
		}
	}

}

PlayAlbumData* ND91Assistant::VideoManager::FindPlayAlbumById( wstring id )
{
	for (size_t i =0 ; i < _playAlbumList.size(); i++)
	{
		if(_playAlbumList[i]->_albumId == id)
		{
			return _playAlbumList[i];
		}
	}
	return NULL;
}

void ND91Assistant::VideoManager::DownLoadUrlImage( wstring localSavePath, wstring  url, wstring toUploadRemotePath )
{
	RETURN_IF(url.empty());
	::DeleteFile(localSavePath.c_str());
	CFileOperation::MakeLocalDir(CFileOperation::GetFileDir(localSavePath));

	CFileOperation::UrlDownloadToFile(url, localSavePath);

	RETURN_IF(!CFileOperation::IsFileExist(localSavePath.c_str()));

	UploadFileCommand upfileCmd(_pDevice);
	upfileCmd.setfilename(localSavePath, toUploadRemotePath);
	upfileCmd.Execute();
}

std::wstring ND91Assistant::VideoManager::GetSdcardPath()
{
	if(_sdcardPath.empty() && !_noSdcard)
	{
		RootData* rootData = new RootData(_pDevice);
		DirData*  sdcardDir= rootData->GetPath(RootData::SDCARD);
		if (sdcardDir)
			_sdcardPath = sdcardDir->GetFullPath();
		else
			_noSdcard = true;
		SAFE_DELETE(rootData);
	}
	return _sdcardPath; 
}

wstring VideoManager::GetAlbumRootPath()
{
	if(_pDevice->GetDevInfo()->_deviceType == Android)
	{
		return GetSdcardPath()+L"91Player/Vlist/";
	}
	return L"";
}

std::wstring ND91Assistant::VideoManager::GetAlbumImage( PlayAlbumData* pAlbum )
{
	if(!pAlbum)
		return L"";

	if(pAlbum && CFileOperation::IsFileExist(pAlbum->_AlbumImagelocalSavePath.c_str()))
		return pAlbum->_AlbumImagelocalSavePath;

    // 专辑文件夹路径
	wstring remotePath = GetAlbumRootPath()+ pAlbum->_albumName + L"/" + pAlbum->_albumName + L".jpg";

    wstring localPath = _pDevice->GetPath()->GetLocalVideoImagePath()
        + pAlbum->_albumName + L".jpg";

	DownloadFileCommand(_pDevice).Download(localPath, remotePath);

	if(!CFileOperation::IsFileExist(localPath.c_str()))
		pAlbum->_AlbumImagelocalSavePath = L"";
	else
		pAlbum->_AlbumImagelocalSavePath = localPath;

	return pAlbum->_AlbumImagelocalSavePath;
}

PlayAlbumData* ND91Assistant::VideoManager::ParseAlbumPlist( wstring localPlistXml )
{
	// 解析出专辑信息
	PlayAlbumData* albumData = AndroidVideoXmlHelper::ParsePlayerAlbumPlistXML(localPlistXml);

	return albumData;
}

VideoData* ND91Assistant::VideoManager::Find91PlayerVideoByRemotePath( wstring remotePath )
{
	for (size_t i = 0; i < _p91playerVideoList.size(); i++)
	{
		if(_p91playerVideoList[i]->_remotepath == remotePath)
			return _p91playerVideoList[i];
	}
	return NULL;
}

#endif