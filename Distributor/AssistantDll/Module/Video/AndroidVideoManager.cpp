#include "pch_module.h"

#ifdef MODULE_VIDEO

#include "AndroidVideoManager.h"
#include "LoadVideoCommand.h"
#include "VideoOperationCommand.h"

#include "Core/DeviceManager.h"
#include "Module/Application/GetAppInfoCommand.h"
#include "Common/Path.h"

#define ND91PLayer		    L"com.nd.android.player"    //"com.nd.android.pandareader";
#define ND91PLayerAPK	    L"com.nd.android.player.apk"//"com.nd.android.pandareader";
#define ND91PLayer_VERSION  L"1"
#define ND91PLayerTempPath  L"/91 PC Suite/temp/91player/"
#define SUPPORT_VIDEO_FORMAT  L"mp4,m4v,wmv,3gp,ndv"

AndroidVideoManager::AndroidVideoManager(const DeviceData* pDevice)
: VideoManager(pDevice)
{
    _pDevice = pDevice;
}

AndroidVideoManager::~AndroidVideoManager()
{
}

ND91Assistant::eVideoInitCode ND91Assistant::AndroidVideoManager::InitEnvironment()
{
#ifdef MODULE_APP
    GetAppInfoCommand  cmd(_pDevice, ND91PLayer);
    cmd.Execute();
    const AppData* data = cmd.GetAppInfo();
    if(cmd.Success() && !data->_strVersion.empty() )
    {
        wstring versionstr = data->_strVersion;
        if (versionstr.compare(ND91PLayer_VERSION)>-1)
            return eVideoInitCode_Success;
    } 
    if (!cmd.ExecuteSuccess())
        return eVideoInitCode_Error;
    return eVideoInitCode_UnInstall_91Player;
#else
    return eVideoInitCode_Error;
#endif
}

void ND91Assistant::AndroidVideoManager::Load()
{
    deque<VideoData*> vtMyVideoList;
    deque<VideoData*> vt91PlayerWithoutAlbumVideoList;
    deque<PlayAlbumData*> vtPlayAlbumList;

    Load91PlayerAlbum(vtPlayAlbumList);

    LoadVideoCommand cmd(_pDevice);
    cmd.Execute();
    const deque<VideoData*> videolist = cmd.GetVideoList();

    wstring str91PlayerPath = CStrOperation::toLowerW(GetSdcardPath() + L"91player/");

    for (deque<VideoData*>::const_iterator it = videolist.begin(); it != videolist.end(); it++)
    {
        VideoData* pVideo = *it;
        wstring remote = pVideo->_remotepath;
        remote = CStrOperation::toLowerW(CFileOperation::GetFileDir(remote));
        if ((int)remote.find(str91PlayerPath) == 0)
        {
            bool bFound = false;
            for (deque<PlayAlbumData*>::iterator it2 = vtPlayAlbumList.begin(); it2 != vtPlayAlbumList.end(); it2++)
            {
                wstring albumpath = CStrOperation::toLowerW((*it2)->GetAlbumPath());
                if (remote == albumpath)
                {
                    pVideo->_pOwnAlbum = *it2;
                    PlayerAlbumItem* founditem = (*it2)->FindItemByPath(pVideo->_remotepath);
                    if (founditem)
                        founditem->_pVideo = new VideoData(pVideo);
                    bFound = true;
                    break;
                }
            }
            if (!bFound)
                InsertOneVideoToList(pVideo, vt91PlayerWithoutAlbumVideoList, false);
        }
        else
            InsertOneVideoToList(pVideo, vtMyVideoList, false);
    }    
    for (deque<PlayAlbumData*>::iterator it2 = vtPlayAlbumList.begin(); it2 != vtPlayAlbumList.end(); it2++)
    {
        for (deque<PlayerAlbumItem*>::iterator it3 = (*it2)->_playItemList.begin(); it3 != (*it2)->_playItemList.end(); it3++)
        {
            PlayerAlbumItem* pItem = *it3;
            if (!pItem->_pVideo)
            {
                DirectoryOperationCommand cmd(_pDevice);
                if (cmd.Exist(pItem->_filePath))
                {
                    pItem->_pVideo = new VideoData();
                    pItem->_pVideo->_remotepath = pItem->_filePath;		// 手机端全路径
                    pItem->_pVideo->_album = (*it2)->_albumName;			// 专辑
                    pItem->_pVideo->_title = CFileOperation::GetFileName(pItem->_filePath);			// 标题
                    pItem->_pVideo->_pOwnAlbum = *it2;	// 所属专辑
                    pItem->_pVideo->LoadVideoFileInfo(_pDevice);
                }                
            }
        }
    }
    AddWithoutAlbumInfoVideo(vtMyVideoList, ePlayAlbumType_NO_91Player);
    AddWithoutAlbumInfoVideo(vt91PlayerWithoutAlbumVideoList, ePlayAlbumType_91Player_Without_Album);
    AddAlbumDatas(vtPlayAlbumList);
}

std::wstring ND91Assistant::AndroidVideoManager::GetSupportVideoType()
{
    return L"*.mp4;*.m4v;*.wmv;*.3gp;*.ndv";
}

bool ND91Assistant::AndroidVideoManager::LoadAlbumImage( PlayAlbumData* pPlayAlbumData )
{
    if(!pPlayAlbumData)
        return false;

    if(pPlayAlbumData && CFileOperation::IsFileExist(pPlayAlbumData->_AlbumImagelocalSavePath.c_str()))
        return true;

    // 专辑文件夹路径
    wstring remotePath = GetAlbumRootPath()+ pPlayAlbumData->_albumName + L"/" + pPlayAlbumData->_albumName + L".jpg";

    wstring localPath = _pDevice->GetPath()->GetLocalVideoImagePath()
        + pPlayAlbumData->_albumName + L".jpg";

    DownloadFileCommand(_pDevice).Download(localPath, remotePath);

    if(!CFileOperation::IsFileExist(localPath.c_str()))
        pPlayAlbumData->_AlbumImagelocalSavePath = L"";
    else
        pPlayAlbumData->_AlbumImagelocalSavePath = localPath;

    return true;
}

bool ND91Assistant::AndroidVideoManager::LoadVideoImage( VideoData* pVideoData )
{
    RETURN_FALSE_IF(!pVideoData);
    VideoOperationCommand  cmd(_pDevice);
    deque<VideoData*> VideoList;
    VideoList.push_back(pVideoData);
    cmd.GetVideoImages(&VideoList); 
    return !pVideoData->_imageLocalPath.empty();
}

VideoData* ND91Assistant::AndroidVideoManager::AddMyVideo( wstring strLocalFullName, ProgressObserver* pObserver)
{
    RETURN_NULL_IF(GetSdcardPath().empty());
    wstring remotepath = GetSdcardPath() + L"movie/" + CFileOperation::GetFileName(strLocalFullName);
    return AddVideo(strLocalFullName, remotepath, pObserver);
}

VideoData* ND91Assistant::AndroidVideoManager::AddPandaPlayerVideo( wstring strLocalFullName, ProgressObserver* pObserver)
{
    RETURN_NULL_IF(GetSdcardPath().empty());
    wstring remotepath = GetSdcardPath() + L"91Player/Video/" + CFileOperation::GetFileName(strLocalFullName);
    return AddVideo(strLocalFullName, remotepath, pObserver);
}












std::wstring ND91Assistant::AndroidVideoManager::GetSdcardPath()
{
    if(_sdcardPath.empty())
    {
        DirData* sdcardDir = DEVICE_MANAGER->GetRootData(_pDevice)->GetPath(RootData::SDCARD);
        if (sdcardDir)
            _sdcardPath = sdcardDir->GetFullPath();
    }
    return _sdcardPath;     
}

bool ND91Assistant::AndroidVideoManager::InsertOneVideoToList( VideoData* video, deque<VideoData*>& vtList, bool del/*=true*/ )
{
    RETURN_FALSE_IF(!video);
    bool found = false;
    for(deque<VideoData*>::iterator it = vtList.begin(); it != vtList.end(); it++)
    {
        if(video->_remotepath == (*it)->_remotepath)
        {
            if(del)
            {
                SAFE_DELETE((*it));
                vtList.erase(it);
            }
            found = true;
            break;
        }
    }
    if (del || !found)
        vtList.push_back(new VideoData(video));
    return found;  
}

void ND91Assistant::AndroidVideoManager::Load91PlayerAlbum(deque<PlayAlbumData*>& vtAlbumList)
{
	RETURN_IF(GetSdcardPath().empty());

    DirData*  pDir = new DirData(_pDevice);
	pDir->_name   = CStrOperation::trimRightW(GetAlbumRootPath(),L"/");
	pDir->Refresh();
	Load91PlayerAlbum(vtAlbumList, pDir);
	SAFE_DELETE(pDir);
}

void ND91Assistant::AndroidVideoManager::Load91PlayerAlbum(deque<PlayAlbumData*>& vtAlbumList, DirData* pDir )
{
	DownloadFileCommand  downFile(_pDevice);
	for (size_t i = 0; i < pDir->GetChildrens().size(); i++)
	{
		switch(pDir->GetChildrens()[i]->_type)
		{
		case NodeData::DIR:
			{
				PlayAlbumData* playalbum = new PlayAlbumData(_pDevice, pDir->GetChildrens()[i]->GetFullPath());
                playalbum->_ePlayAlbumType = ePlayAlbumType_91Player_With_Album;
				if (!playalbum->_playItemList.empty() || !playalbum->_albumImageremotePath.empty() || !playalbum->_albumId.empty())
					vtAlbumList.push_back(playalbum);
				else
					SAFE_DELETE(playalbum);
			}
			break;
		default:
			break;
		}
	}
}

std::wstring ND91Assistant::AndroidVideoManager::GetAlbumRootPath()
{
    return GetSdcardPath()+L"91Player/Vlist/";
}

std::wstring ND91Assistant::AndroidVideoManager::GetVideoRootPath()
{
    return GetSdcardPath()+L"91Player/";
}


VideoData* ND91Assistant::AndroidVideoManager::AddVideo( const wstring localVideoPath, wstring remotePath, ProgressObserver* pObserver)
{
    RETURN_NULL_IF(localVideoPath.empty() || !CFileOperation::IsFileExist(localVideoPath.c_str()));

    int pos = localVideoPath.find_last_of(L".");
    wstring ext =  pos>-1?localVideoPath.substr(pos+1):L"";
    // 判断不是支持的类型
    RETURN_NULL_IF(!CStrOperation::ContainsInW(SUPPORT_VIDEO_FORMAT,CStrOperation::toLowerW(ext),L","));

	UploadFileCommand upfileCmd(_pDevice);
	upfileCmd.Attach(pObserver);
	upfileCmd.SetTaskNumberSum(1);
	upfileCmd.SetAllTaskSum(CFileOperation::GetFileSizeW(localVideoPath));
	upfileCmd.setfilename(localVideoPath, remotePath);
	upfileCmd.Execute();
	upfileCmd.Complete();
	if(upfileCmd.GetUploadResult() == Transfer_Success)
    {        
        // 添加
        VideoOperationCommand  VOCmd(_pDevice); 
        VideoData* video = VOCmd.AddVideo(localVideoPath, upfileCmd.GetDestFilePath());
		if(video)
			return new VideoData(video);
    }
    return NULL;
}

VideoData* ND91Assistant::AndroidVideoManager::AddNetVideo(PlayerAlbumItem* pPlayerAlbumItem, 
                                                           PlayAlbumData* pPlayAlbumData, 
                                                           ProgressObserver* pObserver)
{
    deque<PlayAlbumData*> vtPlayAlbumList = _playAlbumList;

    RETURN_NULL_IF(!pPlayerAlbumItem || !pPlayAlbumData);
    RETURN_NULL_IF(pPlayAlbumData->_ePlayAlbumType != ePlayAlbumType_91Player_With_Album);
    RETURN_NULL_IF(!CFileOperation::IsFileExist(pPlayerAlbumItem->_fileLocalPath.c_str()));
    RETURN_NULL_IF(pPlayAlbumData->_albumId == L"" || pPlayAlbumData->_albumName == L"");

    wstring strAlbumImageremotePath = L"";

	RETURN_NULL_IF(!pPlayerAlbumItem || GetSdcardPath().empty());
    // 专辑文件夹路径
    wstring rootpath = GetAlbumRootPath() + pPlayerAlbumItem->_albumName + L"/";

    pPlayerAlbumItem->_filePath = rootpath 
                                    + CFileOperation::GetFileNameNoExt(pPlayerAlbumItem->_fileDisplayName) 
                                    + CStrOperation::IntToWString(pPlayerAlbumItem->_fileOrder)
                                    + L"."+CStrOperation::trimLeftW(pPlayerAlbumItem->_fileType, L".");

    // 上传专辑封面
	UploadFileCommand upfileCmd(_pDevice);
    if (!pPlayAlbumData->_AlbumImagelocalSavePath.empty() && CFileOperation::IsFileExist(pPlayAlbumData->_AlbumImagelocalSavePath.c_str()))
    {
	    upfileCmd.setfilename(pPlayAlbumData->_AlbumImagelocalSavePath, rootpath + pPlayAlbumData->_albumName + L".jpg");
	    upfileCmd.Execute();
    }
    
    // 上传视频截图
    if (!pPlayerAlbumItem->_urlImageLocalPath.empty() && CFileOperation::IsFileExist(pPlayerAlbumItem->_urlImageLocalPath.c_str()))
    {
	    upfileCmd.setfilename(pPlayerAlbumItem->_urlImageLocalPath, rootpath+pPlayerAlbumItem->_albumName+
							L"_"+CFileOperation::GetFileNameNoExt(pPlayerAlbumItem->_fileDisplayName)+L".jpg");
	    upfileCmd.Execute();
    }

	// 添加视频
    VideoData* pVideoData = NULL;
	if(!pPlayerAlbumItem->_fileLocalPath.empty() && CFileOperation::IsFileExist(pPlayerAlbumItem->_fileLocalPath.c_str()))
	{
		DirectoryOperationCommand dircmd(_pDevice);
		dircmd.DeleteFileLink(pPlayerAlbumItem->_filePath);
		return AddVideo(pPlayerAlbumItem->_fileLocalPath, pPlayerAlbumItem->_filePath, pObserver);
	}

    if (pVideoData)
        CheckNetVideo(pPlayerAlbumItem, pPlayAlbumData, pVideoData, strAlbumImageremotePath);
    return pVideoData;
}


PlayAlbumData* ND91Assistant::AndroidVideoManager::InsertAlbumData( PlayAlbumData* pPlayAlbumData, deque<PlayAlbumData*>& vtPlayAlbumList )
{
    RETURN_NULL_IF(!pPlayAlbumData);
    PlayAlbumData* pData = new PlayAlbumData(pPlayAlbumData);
    PlayAlbumData* pOldPlayAlbumData = FindAlbumData(pData->_albumId, vtPlayAlbumList);
    if (pOldPlayAlbumData)
    {
        // 旧专辑里的列表项加入到新专辑
        for (size_t i =0 ; i < pOldPlayAlbumData->_playItemList.size(); i++)
        {
            pData->_playItemList.push_back(new PlayerAlbumItem(pOldPlayAlbumData->_playItemList[i]));
        }
        // 移除原专辑
        for (deque<PlayAlbumData*>::iterator it = vtPlayAlbumList.begin(); it != vtPlayAlbumList.end(); it++)
        {
            if (pOldPlayAlbumData->_albumId == (*it)->_albumId)
            {
                SAFE_DELETE((*it));
                vtPlayAlbumList.erase(it);
                break;
            }
        }
    }    
    vtPlayAlbumList.push_back(pData);
    return pData;
}

PlayerAlbumItem* ND91Assistant::AndroidVideoManager::InsertAlbumItem( PlayAlbumData* pPlayAlbumData, PlayerAlbumItem* pPlayAlbumItem )
{
    for (deque<PlayerAlbumItem*>::iterator it = pPlayAlbumData->_playItemList.begin(); it != pPlayAlbumData->_playItemList.end(); it++)
    {
        if (pPlayAlbumItem->_fileId != (*it)->_fileId
            || pPlayAlbumItem->_filePath == (*it)->_filePath)
        {
            delete *it;
            it = pPlayAlbumData->_playItemList.erase(it);
            break;
        }
    }
    PlayerAlbumItem* pItem = new PlayerAlbumItem(pPlayAlbumItem);
    pPlayAlbumData->_playItemList.push_back(pItem);
    return pItem;
}

PlayAlbumData* ND91Assistant::AndroidVideoManager::FindAlbumData( wstring strAlbumID, deque<PlayAlbumData*>& vtPlayAlbumList )
{
    PlayAlbumData* pOldPlayAlbumData = NULL;
    for (deque<PlayAlbumData*>::iterator it = vtPlayAlbumList.begin(); it != vtPlayAlbumList.end(); it++)
    {
        if ((*it)->_albumId == strAlbumID)
        {
            return *it;
        }
    }
    return NULL;
}

wstring ND91Assistant::AndroidVideoManager::DownLoadVideo( VideoData* pVideoData, wstring destDir, ProgressObserver* pObserver)
{
	DownloadFileCommand downcmd(_pDevice);
	downcmd.Attach(pObserver);
	downcmd.SetTaskNumberSum(1);
    downcmd.SetAllTaskSum(pVideoData->_size);
	downcmd.Download(destDir+CFileOperation::GetFileName(pVideoData->_remotepath), pVideoData->_remotepath);
    return downcmd.GetFileToPC();
}



/////////////////////////////////////////////////////////////////////////////////////
bool ND91Assistant::AndroidVideoManager::DeletePlayAlbum_Action( PlayAlbumData* pPlayAlbumData )
{
    return FileHelper::Delete(_pDevice, pPlayAlbumData->GetAlbumPath());
}

bool ND91Assistant::AndroidVideoManager::DeleteVideo_Action( VideoData* pVideoData )
{
    RETURN_FALSE_IF(!pVideoData);
    VideoOperationCommand cmd(_pDevice);
    bool bRec = cmd.RemoveVideo(pVideoData);
    if (bRec)
    {
        DirectoryOperationCommand cmd(_pDevice);
        bRec = cmd.DeleteFileLink(pVideoData->GetFullPath());
    }
    return bRec;  
}

bool ND91Assistant::AndroidVideoManager::RenameVideo_Action( VideoData* pVideoData, wstring strNewName )
{
    return false;
}





#endif