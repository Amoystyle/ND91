#include "stdafx.h"
#ifdef MODULE_VIDEO

NDVideoManager::NDVideoManager(const DeviceData* pDevice)
{
	_pIosVideoManager = NULL;
	_pAndroidVideoManager = NULL;
	_myVideos = NULL;
	_other91PlayerVideos = NULL;
	_pDevice = pDevice;

	if (pDevice)
	{
		 if (pDevice->GetDevInfo()->_deviceType == Ios)
			 _pIosVideoManager = new IosVideoManager(pDevice);
		 else
			 _pAndroidVideoManager = new AndroidVideoManager(pDevice);

		 _strLocalAlbumPath = pDevice->GetPath()->GetLocalAlbumPath();
		 CFileOperation::MakeLocalDir(_strLocalAlbumPath);
	}
}

NDVideoManager::~NDVideoManager(void)
{
	ClearList();

	SAFE_DELETE(_pIosVideoManager);
	SAFE_DELETE(_pAndroidVideoManager);
	SAFE_DELETE(_myVideos);
	SAFE_DELETE(_other91PlayerVideos);
}

eVideoInitCode NDVideoManager::InitEnvironment()
{
	RETURN_VALUE_IF(!_pIosVideoManager && !_pAndroidVideoManager, eVideoInitCode_UnConnected);

	if (_pIosVideoManager)
		return _pIosVideoManager->InitEnvironment();
	else if (_pAndroidVideoManager)
		return _pAndroidVideoManager->InitEnvironment();

	return eVideoInitCode_Error;
}

wstring NDVideoManager::GetSupportVideoType()
{
	RETURN_VALUE_IF(!_pIosVideoManager && !_pAndroidVideoManager, L"");

	if (_pIosVideoManager)
		return _pIosVideoManager->GetSupportVideoType();
	else if(_pAndroidVideoManager)
		return _pAndroidVideoManager->GetSupportVideoType();

	return L"";
}

void NDVideoManager::Load()
{
	ClearList();
	SAFE_DELETE(_myVideos);
	SAFE_DELETE(_other91PlayerVideos);

	RETURN_IF(!_pIosVideoManager && !_pAndroidVideoManager);

	_myVideos = CreatePlayAlbumData(ePlayAlbumType_NO_91Player);
	_other91PlayerVideos = CreatePlayAlbumData(ePlayAlbumType_91Player_Without_Album);		


	deque<VideoData*> vtNo91PlayerVideoList;
	deque<VideoData*> vt91PlayerWithoutAlbumVideoList;
	deque<PlayAlbumData*> vtPlayAlbumList;

	if (_pIosVideoManager)
	{
		_pIosVideoManager->LoadMyVideoList(vtNo91PlayerVideoList);

		_pIosVideoManager->Load91PlayerVideoWithoutAlbum(vt91PlayerWithoutAlbumVideoList);		

		_pIosVideoManager->Load91PlayerAlumData(vtPlayAlbumList);		
	}
	else if(_pAndroidVideoManager)
	{
		_pAndroidVideoManager->Load(vtNo91PlayerVideoList,
                                    vt91PlayerWithoutAlbumVideoList,
                                    vtPlayAlbumList);
	}

	AddWithoutAlbumInfoVideo(vtNo91PlayerVideoList, ePlayAlbumType_NO_91Player);
	AddWithoutAlbumInfoVideo(vt91PlayerWithoutAlbumVideoList, ePlayAlbumType_91Player_Without_Album);
	AddAlbumDatas(vtPlayAlbumList);
}

bool NDVideoManager::LoadVideoImage(VideoData* pVideoData)
{
	RETURN_FALSE_IF(!pVideoData);
	RETURN_FALSE_IF(_pIosVideoManager || !_pAndroidVideoManager);
	RETURN_TRUE_IF(CFileOperation::IsFileExist(pVideoData->_imageLocalPath.c_str()));

	bool bRet = false;

	if (_pAndroidVideoManager)
		return _pAndroidVideoManager->LoadVideoImage(pVideoData);

	return bRet;
}

bool NDVideoManager::LoadAlbumImage(PlayAlbumData* pPlayAlbumData)
{
	RETURN_FALSE_IF(!pPlayAlbumData);
	RETURN_FALSE_IF(!_pIosVideoManager && !_pAndroidVideoManager);
	RETURN_FALSE_IF(pPlayAlbumData->_ePlayAlbumType != ePlayAlbumType_91Player_With_Album);
	RETURN_TRUE_IF(CFileOperation::IsFileExist(pPlayAlbumData->_AlbumImagelocalSavePath.c_str()));

	wstring strAlbumImageFullName = _strLocalAlbumPath + pPlayAlbumData->_albumId + pPlayAlbumData->_albumName + L".jpg";

	if (_pIosVideoManager)
	{
		if  (!_pIosVideoManager->LoadAlbumImage(strAlbumImageFullName, pPlayAlbumData->_albumName))
			strAlbumImageFullName = L"";
	}
	else if (_pAndroidVideoManager)
		return _pAndroidVideoManager->LoadAlbumImage(pPlayAlbumData);
	
	bool bRet = false;

	if (CFileOperation::IsFileExist(strAlbumImageFullName.c_str()))
	{
		pPlayAlbumData->_AlbumImagelocalSavePath = strAlbumImageFullName;
		bRet = true;
	}

	return bRet;
}

VideoData* NDVideoManager::AddNetVideo(PlayerAlbumItem* pPlayerAlbumItem, PlayAlbumData* pPlayAlbumData, ProgressObserver* pObserver)
{	
	RETURN_NULL_IF(!_pIosVideoManager && !_pAndroidVideoManager);
	RETURN_NULL_IF(!pPlayerAlbumItem || !pPlayAlbumData);
	RETURN_NULL_IF(pPlayAlbumData->_ePlayAlbumType != ePlayAlbumType_91Player_With_Album);
	RETURN_NULL_IF(!CFileOperation::IsFileExist(pPlayerAlbumItem->_fileLocalPath.c_str()));
	RETURN_NULL_IF(pPlayAlbumData->_albumId == L"" || pPlayAlbumData->_albumName == L"");

	wstring strAlbumImageremotePath = L"";
	VideoData* pVideoData = NULL;
	if (_pIosVideoManager)
	{
		wstring strFileOnPhone = _pIosVideoManager->AddPandaVideo(pPlayerAlbumItem->_fileLocalPath, 
			pPlayerAlbumItem->_fileDisplayName+L"."+CStrOperation::trimLeftW(pPlayerAlbumItem->_fileType, L"."), 
			pPlayAlbumData->_AlbumImagelocalSavePath, pPlayAlbumData->_albumName, pObserver);

		if (strFileOnPhone.length() > 0)
		{
			pVideoData = new VideoData(pPlayerAlbumItem->_fileLocalPath, strFileOnPhone, NULL);
			strAlbumImageremotePath = CFileOperation::GetFileDir(strFileOnPhone) + pPlayAlbumData->_albumName + L".jpg";
		}
	}
	else if(_pAndroidVideoManager)
		pVideoData = _pAndroidVideoManager->AddNetVideo(pPlayerAlbumItem, pPlayAlbumData, _playAlbumList, pObserver);

	if (pVideoData)
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
        if (_pAndroidVideoManager)
            pPlayAlbumDataNew->ReWritePlayAlbumXml(_pDevice);
	}

	return pVideoData;
}

VideoData* NDVideoManager::AddMyVideo(wstring strLocalFullName, ProgressObserver* pObserver)
{
	return AddLocalVideo(strLocalFullName, ePlayAlbumType_NO_91Player, pObserver);
}

VideoData* NDVideoManager::AddPandaPlayerVideo(wstring strLocalFullName, ProgressObserver* pObserver)
{
	return AddLocalVideo(strLocalFullName, ePlayAlbumType_91Player_Without_Album, pObserver);
}

VideoData* NDVideoManager::AddLocalVideo(wstring strLocalFullName, ePlayAlbumType eType, ProgressObserver* pObserver)
{
	RETURN_NULL_IF(eType == ePlayAlbumType_91Player_With_Album)
	RETURN_NULL_IF(!_pIosVideoManager && !_pAndroidVideoManager);
	RETURN_NULL_IF(!CFileOperation::IsFileExist(strLocalFullName.c_str()));

	VideoData* pVideoData = NULL;
	if (_pIosVideoManager)
	{
		wstring strFileOnPhone = _pIosVideoManager->AddMyVideo(strLocalFullName, eType, pObserver);
		if (strFileOnPhone.length() > 0)
			pVideoData = new VideoData(strLocalFullName, strFileOnPhone, NULL);
	}
	else if(_pAndroidVideoManager)
    {
        if (eType == ePlayAlbumType_NO_91Player)
		    pVideoData = _pAndroidVideoManager->AddMyVideo(strLocalFullName, pObserver);
        else if (eType == ePlayAlbumType_91Player_Without_Album)
            pVideoData = _pAndroidVideoManager->AddPandaPlayerVideo(strLocalFullName, pObserver);
    }

	if (pVideoData)
	{
		deque<VideoData*> videoList;
		videoList.push_back(pVideoData);
		AddWithoutAlbumInfoVideo(videoList, eType);
	}

	return pVideoData;
}

bool NDVideoManager::DeleteVideo(VideoData* pVideoData)
{
	RETURN_FALSE_IF(!_pIosVideoManager && !_pAndroidVideoManager);
	RETURN_FALSE_IF(!pVideoData || !pVideoData->_pOwnAlbum);

	bool bRet = false;
	if (_pIosVideoManager)
		bRet = _pIosVideoManager->DeleteVideoFile(pVideoData);
	else if (_pAndroidVideoManager)
	{
		if(pVideoData->_id == 0)
			bRet = true;
		else
			bRet = _pAndroidVideoManager->DeleteVideo(pVideoData, _playAlbumList);
		if (bRet)
		{
			DirectoryOperationCommand cmd(_pDevice);
			bRet = cmd.DeleteFileLink(pVideoData->GetFullPath());
		}
	}

	if(bRet)
	{
		PlayAlbumData* pPlayAlbumData = pVideoData->_pOwnAlbum;
		if (pPlayAlbumData)
			pPlayAlbumData->RemoveItemFromListByPath(pVideoData->_remotepath);
		if (_pAndroidVideoManager && pPlayAlbumData) 
			pPlayAlbumData->ReWritePlayAlbumXml(_pDevice);
	}
	return bRet;
}

bool NDVideoManager::RenameVideo(VideoData* pVideoData, wstring strNewName)
{
	RETURN_FALSE_IF(strNewName.length() <= 0);
	RETURN_FALSE_IF(!_pIosVideoManager && !_pAndroidVideoManager);
	RETURN_FALSE_IF(!pVideoData || !pVideoData->_pOwnAlbum || pVideoData->_pOwnAlbum->_ePlayAlbumType == ePlayAlbumType_91Player_With_Album);
	PlayerAlbumItem* pPlayerAlbumItem = pVideoData->_pOwnAlbum->FindItemByPath(pVideoData->_remotepath);
	RETURN_FALSE_IF(!pPlayerAlbumItem);

	bool bRet = false;
	if (_pIosVideoManager)
		bRet = _pIosVideoManager->RenameVideoFile(pVideoData, strNewName);
	else if (_pAndroidVideoManager)
		bRet = _pAndroidVideoManager->RenameVideo(pVideoData, strNewName);

	if (bRet)
	{		
		pVideoData->_name = strNewName + L"." + CFileOperation::GetFileExt(pVideoData->_name);
		pVideoData->_title = strNewName;

		pPlayerAlbumItem->_fileDisplayName = strNewName;
		pPlayerAlbumItem->_filePath = pVideoData->_remotepath;		
	}

	return bRet;
}

wstring NDVideoManager::DownLoadVideo(VideoData* pVideoData, wstring strLocalSaveDir, ProgressObserver* pObserver)
{
	RETURN_VALUE_IF(!pVideoData, L"");
	RETURN_VALUE_IF(!_pIosVideoManager && !_pAndroidVideoManager, L"");	
	CFileOperation::MakeLocalDir(strLocalSaveDir);

	if (_pIosVideoManager)
	{
		wstring strLocalSaveFullName = CStrOperation::trimRightW(strLocalSaveDir, L"\\") + L"\\" + pVideoData->_name;
		strLocalSaveFullName = CFileOperation::RenameFile(strLocalSaveFullName);

		if ( _pIosVideoManager->DownLoadFile(pVideoData->_remotepath, strLocalSaveFullName, pObserver))
			return strLocalSaveFullName;
	}
	else if (_pAndroidVideoManager)
	{
		return _pAndroidVideoManager->DownLoadVideo(pVideoData, strLocalSaveDir, pObserver);
	}

	return L"";
}

bool NDVideoManager::DeletePlayAlbum(PlayAlbumData* pPlayAlbumData)
{
	RETURN_FALSE_IF(!pPlayAlbumData);
	RETURN_FALSE_IF(pPlayAlbumData->_ePlayAlbumType != ePlayAlbumType_91Player_With_Album);
	RETURN_FALSE_IF(!_pIosVideoManager && !_pAndroidVideoManager);

	bool bRet = false;

	if (_pIosVideoManager)
	{
		bRet = _pIosVideoManager->DeleteAlbum(pPlayAlbumData->_albumName);
	}
	else if (_pAndroidVideoManager)
	{
		bRet = _pAndroidVideoManager->DeletePlayAlbum(pPlayAlbumData);
	}

	if (bRet)
	{
		for (deque<PlayAlbumData*>::iterator it=_playAlbumList.begin(); it!=_playAlbumList.end(); it++)
		{
			PlayAlbumData* pTemp = *it;
			if ((_pAndroidVideoManager && pTemp->_albumId == pPlayAlbumData->_albumId) || 
				(_pIosVideoManager && pTemp->_albumName == pPlayAlbumData->_albumName))
			{
				_playAlbumList.erase(it);
				SAFE_DELETE(pTemp);
				break;
			}
		}		
	}

	return bRet;
}

bool NDVideoManager::ParsePlayerAlbumPlist(wstring strPlistFileName, PlayAlbumData* pPlayAlbumData)
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

PlayAlbumData* NDVideoManager::CreatePlayAlbumData(ePlayAlbumType eType)
{
	PlayAlbumData* pNo91Player = new PlayAlbumData();
	pNo91Player->_ePlayAlbumType = eType;

	return pNo91Player;
}

void NDVideoManager::ClearList()
{
	for (size_t i=0; i< _playAlbumList.size();i++)
	{
		SAFE_DELETE(_playAlbumList[i]);
	}
	_playAlbumList.clear();
}

void NDVideoManager::AddWithoutAlbumInfoVideo(deque<VideoData*> videoList, ePlayAlbumType eType)
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

/*PlayAlbumData* NDVideoManager::FindWithoutAlbumInfoPlayAlbumData(ePlayAlbumType eType)
{
	RETURN_NULL_IF(eType == ePlayAlbumType_91Player_With_Album);

	PlayAlbumData* pPlayAlbumData = NULL;

	for (int i=0; i<(int)_playAlbumList.size(); i++)
	{
		PlayAlbumData* pTemp = _playAlbumList.at(i);
		if (pTemp && pTemp->_ePlayAlbumType == eType)
		{
			pPlayAlbumData = pTemp;
			break;
		}		
	}

	if (!pPlayAlbumData)
	{
		pPlayAlbumData = CreatePlayAlbumData(eType);
		_playAlbumList.push_back(pPlayAlbumData);
	}

	return pPlayAlbumData;
}*/

void NDVideoManager::AddAlbumDatas(deque<PlayAlbumData*> vtPlayAlbumList)
{
	_playAlbumList.insert(_playAlbumList.end(), vtPlayAlbumList.begin(), vtPlayAlbumList.end());
}

PlayAlbumData* NDVideoManager::FindWithAlbumInfoAlbumData(PlayAlbumData* pPlayAlbumData)
{
	RETURN_NULL_IF(!pPlayAlbumData);

	PlayAlbumData* find = NULL;
	for (int i=0; i<(int)_playAlbumList.size(); i++)
	{
		PlayAlbumData* pTemp = _playAlbumList.at(i);
		if ((_pAndroidVideoManager && (pTemp->_albumId == pPlayAlbumData->_albumId
                                        || pTemp->_albumId.empty() && pTemp->_albumName == pPlayAlbumData->_albumName)) 
            || (_pIosVideoManager && pTemp->_albumName == pPlayAlbumData->_albumName))
		{
			find = pTemp;
			break;
		}
	}

	return find;
}

PlayAlbumData* NDVideoManager::GetMyVideoAlbumData()
{
	return _myVideos; //FindWithoutAlbumInfoPlayAlbumData(ePlayAlbumType_NO_91Player);
}

PlayAlbumData* NDVideoManager::GetWithoutAlbumPandaVideoAlbumData()
{
	return _other91PlayerVideos; //FindWithoutAlbumInfoPlayAlbumData(ePlayAlbumType_91Player_Without_Album);
}

const deque<PlayAlbumData*>* ND91Assistant::NDVideoManager::GetAlbumList()
{
    return &_playAlbumList;
}

void ND91Assistant::NDVideoManager::RemoveAlbumData( PlayAlbumData* pPlayAlbumData )
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

std::wstring ND91Assistant::NDVideoManager::GetVideoRootPath()
{
    if (_pIosVideoManager)
    {
        return _pIosVideoManager->GetVideoRootPath();
    }
    else if (_pAndroidVideoManager)
    {
        return _pAndroidVideoManager->GetVideoRootPath();
    }
    return L"";
}

#endif