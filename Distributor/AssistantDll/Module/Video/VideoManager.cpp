#include "pch_module.h"

#ifdef MODULE_VIDEO

#include "Module/Video/VideoManager.h"
#include "Common/Path.h"

VideoManager::VideoManager(const DeviceData* pDevice)
{
	_myVideos = NULL;
	_other91PlayerVideos = NULL;
	_pDevice = pDevice;

	if (pDevice)
	{
		 _strLocalAlbumPath = pDevice->GetPath()->GetLocalAlbumPath();
		 CFileOperation::MakeLocalDir(_strLocalAlbumPath);
	}
}

VideoManager::~VideoManager(void)
{
    Release();
}

int ND91Assistant::VideoManager::GetVideoCount()
{
    int count = 0;
    eVideoInitCode code = InitEnvironment();
    Load();

    // 我的视频
    PlayAlbumData* pPlayAlbumData = GetMyVideoAlbumData();
    count = (int)(pPlayAlbumData->_playItemList.size());
    if (code == eVideoInitCode_UnInstall_91Player)
        return count;

    // 所有熊猫影音视频
    const deque<PlayAlbumData*>* pAlbumList = GetAlbumList();
    for (deque<PlayAlbumData*>::const_iterator it = pAlbumList->begin(); it!= pAlbumList->end(); it++)
        if ((*it)->_ePlayAlbumType != ePlayAlbumType_NO_91Player)
            count += (int)((*it)->_playItemList.size());

    //其他视频
    pPlayAlbumData = GetWithoutAlbumPandaVideoAlbumData();
    count += (int)(pPlayAlbumData->_playItemList.size());

    return count;
}

bool VideoManager::DeleteVideo(VideoData* pVideoData)
{
	RETURN_FALSE_IF(!pVideoData || !pVideoData->_pOwnAlbum);

	bool bRet = DeleteVideo_Action(pVideoData);

	if(bRet)
	{
		PlayAlbumData* pPlayAlbumData = pVideoData->_pOwnAlbum;
		if (pPlayAlbumData)
			pPlayAlbumData->RemoveItemFromListByPath(pVideoData->_remotepath);
		if (pPlayAlbumData) 
			pPlayAlbumData->ReWritePlayAlbumXml(_pDevice);
	}
	return bRet;
}

bool VideoManager::RenameVideo(VideoData* pVideoData, wstring strNewName)
{
	RETURN_FALSE_IF(strNewName.length() <= 0);
	RETURN_FALSE_IF(!pVideoData || !pVideoData->_pOwnAlbum || pVideoData->_pOwnAlbum->_ePlayAlbumType == ePlayAlbumType_91Player_With_Album);
	PlayerAlbumItem* pPlayerAlbumItem = pVideoData->_pOwnAlbum->FindItemByPath(pVideoData->_remotepath);
	RETURN_FALSE_IF(!pPlayerAlbumItem);

	bool bRet = RenameVideo_Action(pVideoData, strNewName);

	if (bRet)
	{		
		pVideoData->_name = strNewName + L"." + CFileOperation::GetFileExt(pVideoData->_name);
		pVideoData->_title = strNewName;

		pPlayerAlbumItem->_fileDisplayName = strNewName;
		pPlayerAlbumItem->_filePath = pVideoData->_remotepath;		
	}

	return bRet;
}

bool VideoManager::DeletePlayAlbum(PlayAlbumData* pPlayAlbumData)
{
	RETURN_FALSE_IF(!pPlayAlbumData);
	RETURN_FALSE_IF(pPlayAlbumData->_ePlayAlbumType != ePlayAlbumType_91Player_With_Album);

	bool bRet = DeletePlayAlbum_Action(pPlayAlbumData);

	if (bRet)
	{
		for (deque<PlayAlbumData*>::iterator it=_playAlbumList.begin(); it!=_playAlbumList.end(); it++)
		{
			PlayAlbumData* pTemp = *it;
			if ((pTemp->_albumId == pPlayAlbumData->_albumId) || 
				(pTemp->_albumName == pPlayAlbumData->_albumName))
			{
				_playAlbumList.erase(it);
				SAFE_DELETE(pTemp);
				break;
			}
		}		
	}

	return bRet;
}

bool VideoManager::ParsePlayerAlbumPlist(wstring strPlistFileName, PlayAlbumData* pPlayAlbumData)
{
	RETURN_FALSE_IF(!CFileOperation::IsFileExist(strPlistFileName.c_str()));
	RETURN_FALSE_IF(!pPlayAlbumData)

	IosPlistHelper xmlHelper(strPlistFileName, eXmlType_UTF8);
	XML_NODE_INDEX index;
	index.AddChild(0);
	index.AddChild(0);
	pPlayAlbumData->_count = xmlHelper.SearchKeyValue(index, "kAlbumCount");
	pPlayAlbumData->_updata = xmlHelper.SearchKeyValue(index, "kAlbumUpdate");
	pPlayAlbumData->_albumId = xmlHelper.SearchKeyValue(index, "kAlbumID");
	pPlayAlbumData->_albumName = xmlHelper.SearchKeyValue(index, "kAlbumName");
	pPlayAlbumData->_type = _wtoi(xmlHelper.SearchKeyValue(index, "kAlbumType").c_str());
	pPlayAlbumData->_albumUrl = xmlHelper.SearchKeyValue(index, "kAlbumpurl");
	rapidxml::xml_node<char>* pTipStringNode = xmlHelper.SearchKeyNode(index, "kAlbumTip", "string");
	if (pTipStringNode && pTipStringNode->first_node())
		pPlayAlbumData->_introduce = CCodeOperation::UTF_8ToUnicode(pTipStringNode->first_node()->value());
	pPlayAlbumData->_ePlayAlbumType = ePlayAlbumType_91Player_With_Album;

	return true;
}

PlayAlbumData* VideoManager::CreatePlayAlbumData(ePlayAlbumType eType)
{
	PlayAlbumData* pNo91Player = new PlayAlbumData();
	pNo91Player->_ePlayAlbumType = eType;

	return pNo91Player;
}

void VideoManager::AddWithoutAlbumInfoVideo(deque<VideoData*> videoList, ePlayAlbumType eType)
{
	RETURN_IF(eType == ePlayAlbumType_91Player_With_Album);
	if(!_myVideos)
		_myVideos = CreatePlayAlbumData(ePlayAlbumType_NO_91Player);
	if(!_other91PlayerVideos)
		_other91PlayerVideos = CreatePlayAlbumData(ePlayAlbumType_91Player_Without_Album);

	PlayAlbumData* pPlayAlbumData = eType == ePlayAlbumType_NO_91Player?_myVideos:_other91PlayerVideos;	

	for (int i=0; i<(int)videoList.size(); i++)
	{
		VideoData* pTemp = videoList.at(i);
		if (!pTemp) continue;

		if (pPlayAlbumData->FindItemByPath(pTemp->_remotepath)) continue;

		PlayerAlbumItem* pNewPlayerAlbumItem = new PlayerAlbumItem();
		pNewPlayerAlbumItem->_filePath = pTemp->_remotepath;
		pNewPlayerAlbumItem->_pVideo = videoList.at(i);
		pNewPlayerAlbumItem->_pVideo->_pOwnAlbum = pPlayAlbumData;

		pPlayAlbumData->_playItemList.push_back(pNewPlayerAlbumItem);
	}
}

void VideoManager::AddAlbumDatas(deque<PlayAlbumData*> vtPlayAlbumList)
{
	_playAlbumList.clear();
	_playAlbumList.insert(_playAlbumList.end(), vtPlayAlbumList.begin(), vtPlayAlbumList.end());
}

PlayAlbumData* VideoManager::FindWithAlbumInfoAlbumData(PlayAlbumData* pPlayAlbumData)
{
	RETURN_NULL_IF(!pPlayAlbumData);

	PlayAlbumData* find = NULL;
	for (int i=0; i<(int)_playAlbumList.size(); i++)
	{
		PlayAlbumData* pTemp = _playAlbumList.at(i);
		if (((pTemp->_albumId == pPlayAlbumData->_albumId
                                        || pTemp->_albumId.empty() && pTemp->_albumName == pPlayAlbumData->_albumName)) 
            || (pTemp->_albumName == pPlayAlbumData->_albumName))
		{
			find = pTemp;
			break;
		}
	}

	return find;
}

PlayAlbumData* VideoManager::GetMyVideoAlbumData()
{
	return _myVideos;
}

PlayAlbumData* VideoManager::GetWithoutAlbumPandaVideoAlbumData()
{
	return _other91PlayerVideos;
}

const deque<PlayAlbumData*>* ND91Assistant::VideoManager::GetAlbumList()
{
    return &_playAlbumList;
}

void ND91Assistant::VideoManager::RemoveAlbumData( PlayAlbumData* pPlayAlbumData )
{
    for (deque<PlayAlbumData*>::iterator it = _playAlbumList.begin(); it != _playAlbumList.end(); it++)
    {
        if (*it == pPlayAlbumData)
        {
            SAFE_DELETE(*it);
            _playAlbumList.erase(it);
            break;
        }
    }
}

void ND91Assistant::VideoManager::Release()
{
    for (size_t i=0; i< _playAlbumList.size();i++)
    {
        SAFE_DELETE(_playAlbumList[i]);
    }
    _playAlbumList.clear();

    SAFE_DELETE(_myVideos);
    SAFE_DELETE(_other91PlayerVideos);
}

void ND91Assistant::VideoManager::CheckNetVideo( PlayerAlbumItem* pPlayerAlbumItem, PlayAlbumData* pPlayAlbumData, VideoData* pVideoData, wstring strAlbumImageremotePath )
{
    PlayAlbumData* pPlayAlbumDataNew = FindWithAlbumInfoAlbumData(pPlayAlbumData);
    if (pPlayAlbumDataNew)
        pPlayAlbumDataNew->UpdateInfo(pPlayAlbumData);
    else
    {
        pPlayAlbumDataNew = new PlayAlbumData(pPlayAlbumData);
        _playAlbumList.push_back(pPlayAlbumDataNew);
    }
    if (pPlayAlbumDataNew->_albumImageremotePath.length() <= 0)
        pPlayAlbumDataNew->_albumImageremotePath = strAlbumImageremotePath;

    pVideoData->_pOwnAlbum = pPlayAlbumDataNew;

    PlayerAlbumItem* find = pPlayAlbumDataNew->FindItemByPath(pVideoData->_remotepath);
    if (find == NULL)
    {
        PlayerAlbumItem* pNewPlayerAlbumItem = new PlayerAlbumItem(pPlayerAlbumItem);
        pNewPlayerAlbumItem->_pVideo = pVideoData;		
        pNewPlayerAlbumItem->_filePath = pVideoData->_remotepath;	
        pPlayAlbumDataNew->_playItemList.push_back(pNewPlayerAlbumItem);	
    }
    else
    {
        SAFE_DELETE(find->_pVideo);
        find->_pVideo = pVideoData;
    }
    pPlayAlbumDataNew->ReWritePlayAlbumXml(_pDevice);
}


#endif