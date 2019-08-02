#include "pch_module.h"

#ifdef MODULE_VIDEO

#include "IosVideoManager.h"
#include "Module/Application/IosApp/IosIPAAppOperation.h"

IosVideoManager::IosVideoManager(const DeviceData* pDevice)
: VideoManager(pDevice)
{
	_ePlayerType = eNDPlayerType_NONE;
	_pFileStream = NULL;
}

IosVideoManager::~IosVideoManager(void)
{
}

eVideoInitCode IosVideoManager::InitEnvironment()
{
	if (!_pDevice || !_pDevice->GetDevInfo()) 
		return eVideoInitCode_UnConnected;

	if (!_pDevice->GetDevInfo()->_ios_bJailBreaked) 
		return eVideoInitCode_Ios_UnJailBreaked;

	eNDPlayerType ePlayerType = GetPlayerType();

	if (ePlayerType == eNDPlayerType_NONE) 
		return eVideoInitCode_UnInstall_91Player;

	if (ePlayerType == eNDPlayerType_IPA && _pDevice->GetDevInfo()->_connectType == WIFI)
		return eVideoInitCode_Cannot_Use_With_Wifi;

	if (_ePlayerType != ePlayerType || !_pFileStream)
	{
		SAFE_DELETE(_pFileStream);
		_pFileStream = GetFileStream(ePlayerType);
	}

	_ePlayerType = ePlayerType;

	return eVideoInitCode_Success;
}

void ND91Assistant::IosVideoManager::Load()
{
    Release();

    _myVideos = CreatePlayAlbumData(ePlayAlbumType_NO_91Player);
    _other91PlayerVideos = CreatePlayAlbumData(ePlayAlbumType_91Player_Without_Album);		

    deque<VideoData*> vtMyVideoList;
    deque<VideoData*> vt91PlayerWithoutAlbumVideoList;
    deque<PlayAlbumData*> vtPlayAlbumList;

    LoadMyVideoList(vtMyVideoList);
    Load91PlayerVideoWithoutAlbum(vt91PlayerWithoutAlbumVideoList);		
    Load91PlayerAlumData(vtPlayAlbumList);		

    AddWithoutAlbumInfoVideo(vtMyVideoList, ePlayAlbumType_NO_91Player);
    AddWithoutAlbumInfoVideo(vt91PlayerWithoutAlbumVideoList, ePlayAlbumType_91Player_Without_Album);
    AddAlbumDatas(vtPlayAlbumList);
}

std::wstring ND91Assistant::IosVideoManager::GetSupportVideoType()
{
    return L"*.mp4;*.mpv;*.3gp;*.mov;*.ndv";
}

bool IosVideoManager::LoadAlbumImage( PlayAlbumData* pPlayAlbumData )
{
    RETURN_FALSE_IF(!pPlayAlbumData);

    wstring strLocalFullName = _strLocalAlbumPath + pPlayAlbumData->_albumId + pPlayAlbumData->_albumName + L".jpg";
    wstring strAlbumName = pPlayAlbumData->_albumName;

    RETURN_FALSE_IF(!_pFileStream);
    wstring strAlbumImageFullName = GetVideoBookingPath() + L"/" + strAlbumName + L"/" + strAlbumName + L".jpg";
    RETURN_FALSE_IF(!_pFileStream->Ios_Exists(strAlbumImageFullName));

    bool bRec = _pFileStream->Ios_DownFromPhone(strAlbumImageFullName, strLocalFullName) == Transfer_Success;

    if (bRec)
        pPlayAlbumData->_AlbumImagelocalSavePath = strLocalFullName;

    return bRec;
}

bool ND91Assistant::IosVideoManager::LoadVideoImage( VideoData* pVideoData )
{
    return false;
}

VideoData* ND91Assistant::IosVideoManager::AddMyVideo( wstring strLocalFullName, ProgressObserver* pObserver /*= NULL*/ )
{
    VideoData* pVideoData = NULL;
    wstring strFileOnPhone = AddMyVideo(strLocalFullName, ePlayAlbumType_NO_91Player, pObserver);
    if (strFileOnPhone.length() > 0)
        pVideoData = new VideoData(strLocalFullName, strFileOnPhone, NULL);
    return pVideoData;
}

VideoData* ND91Assistant::IosVideoManager::AddPandaPlayerVideo( wstring strLocalFullName, ProgressObserver* pObserver /*= NULL*/ )
{
    VideoData* pVideoData = NULL;
    wstring strFileOnPhone = AddMyVideo(strLocalFullName, ePlayAlbumType_91Player_Without_Album, pObserver);
    if (strFileOnPhone.length() > 0)
        pVideoData = new VideoData(strLocalFullName, strFileOnPhone, NULL);
    return pVideoData;
}

VideoData* ND91Assistant::IosVideoManager::AddNetVideo( PlayerAlbumItem* pPlayerAlbumItem, PlayAlbumData* pPlayAlbumData, ProgressObserver* pObserver /*= NULL*/ )
{
    RETURN_NULL_IF(!pPlayerAlbumItem || !pPlayAlbumData);
    RETURN_NULL_IF(pPlayAlbumData->_ePlayAlbumType != ePlayAlbumType_91Player_With_Album);
    RETURN_NULL_IF(!CFileOperation::IsFileExist(pPlayerAlbumItem->_fileLocalPath.c_str()));
    RETURN_NULL_IF(pPlayAlbumData->_albumId == L"" || pPlayAlbumData->_albumName == L"");

    wstring strAlbumImageremotePath = L"";
    VideoData* pVideoData = NULL;

    wstring strFileOnPhone = AddPandaVideo(pPlayerAlbumItem->_fileLocalPath, 
            pPlayerAlbumItem->_fileDisplayName+L"."+CStrOperation::trimLeftW(pPlayerAlbumItem->_fileType, L"."), 
            pPlayAlbumData->_AlbumImagelocalSavePath, pPlayAlbumData->_albumName, pObserver);

    if (strFileOnPhone.length() > 0)
    {
        pVideoData = new VideoData(pPlayerAlbumItem->_fileLocalPath, strFileOnPhone, NULL);
        strAlbumImageremotePath = CFileOperation::GetFileDir(strFileOnPhone) + pPlayAlbumData->_albumName + L".jpg";
    }

    if (pVideoData)
    {
        CheckNetVideo(pPlayerAlbumItem, pPlayAlbumData, pVideoData, strAlbumImageremotePath);
    }

    return pVideoData;
}

bool ND91Assistant::IosVideoManager::DeletePlayAlbum_Action( PlayAlbumData* pPlayAlbumData )
{
    RETURN_FALSE_IF(!_pFileStream);

    wstring strFolderOnPhone = GetVideoBookingPath() + L"/" + pPlayAlbumData->_albumName;
    bool bRet = IosPublicFunc::DeleteFolder(strFolderOnPhone, _pFileStream);

    return bRet;
}

bool ND91Assistant::IosVideoManager::DeleteVideo_Action( VideoData* pVideoData )
{
    RETURN_FALSE_IF(!_pFileStream);
    RETURN_FALSE_IF(!pVideoData);

    bool bRet = false;

    if (!_pFileStream->Ios_Exists(pVideoData->_remotepath))
        bRet = true;
    else
        bRet = _pFileStream->Ios_DeleteFile(pVideoData->_remotepath);

    return bRet;    
}

bool ND91Assistant::IosVideoManager::RenameVideo_Action( VideoData* pVideoData, wstring strNewName )
{
    RETURN_FALSE_IF(!_pFileStream);
    RETURN_FALSE_IF(!pVideoData);

    RETURN_FALSE_IF(!_pFileStream->Ios_Exists(pVideoData->_remotepath));

    wstring strNewFullName = CFileOperation::GetFileDir(pVideoData->_remotepath) + strNewName + L"." + CFileOperation::GetFileExt(pVideoData->_name);

    RETURN_FALSE_IF(_pFileStream->Ios_Exists(strNewFullName));

    bool bRet = _pFileStream->Ios_Rename(pVideoData->_remotepath, strNewFullName);

    if (bRet)
    {		
        if (pVideoData->_pOwnAlbum)
        {
            PlayerAlbumItem* pPlayerAlbumItem = pVideoData->_pOwnAlbum->FindItemByPath(pVideoData->_remotepath);
            if (pPlayerAlbumItem)
                pPlayerAlbumItem->_fileDisplayName = strNewName;
        }
        pVideoData->_name = CFileOperation::GetFileName(strNewFullName);
        pVideoData->_title = strNewName;
    }

    return bRet;
}

std::wstring ND91Assistant::IosVideoManager::DownLoadVideo( VideoData* pVideoData, wstring strLocalSaveDir, ProgressObserver* pObserver /*= NULL*/ )
{
    RETURN_FALSE_IF(!_pFileStream);

    ProgressSubject subject;
    subject.Attach(pObserver);
    subject.ResetProgressData();
    subject.SetTaskNumberSum(1);

    wstring strLocalSaveFullName = CStrOperation::trimRightW(strLocalSaveDir, L"\\") + L"\\" + pVideoData->_name;
    strLocalSaveFullName = CFileOperation::RenameFile(strLocalSaveFullName);
    TransferResult bRet = _pFileStream->Ios_DownFromPhone(pVideoData->_remotepath, strLocalSaveFullName, subject);
    if (bRet == Transfer_Success)
        return strLocalSaveFullName;
    else 
        return L"";
}



void IosVideoManager::LoadMyVideoList(deque<VideoData*>& vtMyVideoList)
{
	vtMyVideoList.clear();
	
	RETURN_IF(!_pFileStream);

	GetVideosFromPath(vtMyVideoList, GetVideoNoBookingPath());
}

void IosVideoManager::Load91PlayerVideoWithoutAlbum(deque<VideoData*>& vtVideoList)
{
	vtVideoList.clear();

	RETURN_IF(!_pFileStream);

	GetVideosFromPath(vtVideoList, GetVideoBookingPath(), false);
}

void IosVideoManager::Load91PlayerAlumData(deque<PlayAlbumData*>& vtPlayAlbumList)
{
	vtPlayAlbumList.clear();

	RETURN_IF(!_pFileStream);
	RETURN_IF(!_pFileStream->Ios_Exists(GetVideoBookingPath()));

	DirData resultDir(_pDevice);
	resultDir._name = GetVideoBookingPath() + L"/";
	if (_pFileStream->Ios_GetFileInfos(&resultDir, eFileGetType_Dirs))
	{
		deque<NodeData*> childs = resultDir.GetChildrensNoReload();
		for ( size_t i = 0; i < childs.size(); i++ )
		{
			NodeData* pFile = childs[i];
			if (!pFile)
				continue;

			PlayAlbumData* pNewAlbumData = new PlayAlbumData();
			pNewAlbumData->_albumName = pFile->_name;	
			pNewAlbumData->_ePlayAlbumType = ePlayAlbumType_91Player_With_Album;
			pNewAlbumData->_albumImageremotePath = resultDir._name + pFile->_name;
			vtPlayAlbumList.push_back(pNewAlbumData);
		}
	}

	for (size_t i = 0; i < vtPlayAlbumList.size(); i++)
	{
		deque<VideoData*> vtVideoList;
		GetVideosFromPath( vtVideoList, GetVideoBookingPath()+L"/"+vtPlayAlbumList.at(i)->_albumName, false, vtPlayAlbumList.at(i));
		for (size_t j = 0; j < vtVideoList.size(); j++)
		{
			PlayerAlbumItem* pNewPlayerAlbumItem = new PlayerAlbumItem();
			pNewPlayerAlbumItem->_albumName = vtPlayAlbumList.at(i)->_albumName;
			pNewPlayerAlbumItem->_pVideo = vtVideoList.at(j);    
			pNewPlayerAlbumItem->_filePath = vtVideoList.at(j)->_remotepath; 

			vtPlayAlbumList.at(i)->_playItemList.push_back(pNewPlayerAlbumItem);
		}
	}
}

wstring IosVideoManager::AddPandaVideo(wstring strLocalVideoFullName, wstring strVideoNameOnPhone, wstring strLocalAlbumImageFullName, 
									   wstring strAlbumName, ProgressObserver* pObserver)
{
	RETURN_VALUE_IF(!_pFileStream, L"");
	RETURN_VALUE_IF(!CFileOperation::IsFileExist(strLocalVideoFullName.c_str()), L"");
	RETURN_VALUE_IF(!IsValidVideo(strLocalVideoFullName), L"");
	RETURN_VALUE_IF(strAlbumName.length() <= 0, L"");

	wstring strAlbumFullNameOnPhone =  GetVideoBookingPath() + L"/" + strAlbumName;
	if (!_pFileStream->Ios_Exists(strAlbumFullNameOnPhone))
		_pFileStream->Ios_CreateDirectory(strAlbumFullNameOnPhone);

	wstring strAlbumImageFullNameOnPhone = strAlbumFullNameOnPhone + L"/" + strAlbumName + L".jpg";
	if (!_pFileStream->Ios_Exists(strAlbumImageFullNameOnPhone) && CFileOperation::IsFileExist(strLocalAlbumImageFullName.c_str()))
		_pFileStream->Ios_CopyToPhone(strLocalAlbumImageFullName, strAlbumImageFullNameOnPhone);

	ProgressSubject subject;
	subject.Attach(pObserver);
	subject.ResetProgressData();
	subject.SetTaskNumberSum(1);
	subject.SetAllTaskSum(CFileOperation::GetFileSizeW(strLocalVideoFullName));
	wstring strVideoFullNameOnPhone = strAlbumFullNameOnPhone + L"/" + strVideoNameOnPhone;

	TransferResult bRet = _pFileStream->Ios_CopyToPhone(strLocalVideoFullName, strVideoFullNameOnPhone, subject);
	if (bRet == Transfer_Success)
		return strVideoFullNameOnPhone;

	return L"";
}

wstring IosVideoManager::AddMyVideo(wstring strLocalFullName, ePlayAlbumType eType, ProgressObserver* pObserver)
{
	RETURN_VALUE_IF(!_pFileStream, L"");
	RETURN_VALUE_IF(!CFileOperation::IsFileExist(strLocalFullName.c_str()), L"");
	RETURN_VALUE_IF(!IsValidVideo(strLocalFullName), L"");
	RETURN_VALUE_IF(eType == ePlayAlbumType_91Player_With_Album, L"");

	wstring strFileOnPhone = GetVideoNoBookingPath() + L"/" + CFileOperation::GetFileName(strLocalFullName);
	if (eType == ePlayAlbumType_91Player_Without_Album)
		strFileOnPhone = GetVideoBookingPath() + L"/" + CFileOperation::GetFileName(strLocalFullName);
	strFileOnPhone = IosPublicFunc::RenameFile(strFileOnPhone, _pFileStream);

	ProgressSubject subject;
	subject.Attach(pObserver);
	subject.ResetProgressData();
	subject.SetTaskNumberSum(1);
	subject.SetAllTaskSum(CFileOperation::GetFileSizeW(strLocalFullName));

	TransferResult bRet = _pFileStream->Ios_CopyToPhone(strLocalFullName, strFileOnPhone, subject);
	if (bRet == Transfer_Success)
		return strFileOnPhone;

	return L"";
}

eNDPlayerType IosVideoManager::GetPlayerType()
{
	eNDPlayerType eType = eNDPlayerType_NONE;
#ifdef MODULE_APP
	ProgressSubject subject;
	IosIPAAppOperation ipaAppOperation(_pDevice, subject);
	vector<AppData*> vtInfos;
	ipaAppOperation.GetAppInfos(vtInfos);
	for (int i=0; i<(int)vtInfos.size(); i++)
	{
		AppData* pTemp = vtInfos.at(i);
		if (pTemp && pTemp->_eAppOwner == APP_OWNER_USER)
		{
			if (pTemp->_packageName == IOS_IPA_91PLAYER_IDENTIFIER)
			{
				eType = eNDPlayerType_IPA;
				break;
			}
			else if (pTemp->_packageName == IOS_PXL_91PLAYER_IDENTIFIER)
			{
				eType = eNDPlayerType_PXL;
				break;
			}
		}
	}
#endif
	if (eType == eNDPlayerType_NONE)
	{
		IIosFileSystem* pFileStream = NULL;

		if (_pDevice->GetDevInfo()->_connectType == WIFI)
			pFileStream = new IosWiFiFileSystem(_pDevice->GetDevInfo()->_ios_wifi_handle);
		else 
			pFileStream = new AFCFileSysemBase(_pDevice->GetDevInfo());

		if (pFileStream && 
			(pFileStream->Ios_Exists(IOS_PXL_91PLAYER_PATH) ||
			pFileStream->Ios_Exists(IOS_PXL_PANDAPLAYER_PATH) ||
			pFileStream->Ios_Exists(IOS_HD_PXL_91PLAYER_PATH)))
			eType = eNDPlayerType_PXL;

		SAFE_DELETE(pFileStream);
	}

	return eType;
}

IIosFileSystem* IosVideoManager::GetFileStream(eNDPlayerType ePlayerType)
{
	IIosFileSystem* pFileStream = NULL;
	switch (ePlayerType)
	{
	case eNDPlayerType_IPA:
		pFileStream = AFCAppFileSystem::StartAPPHouseArrestService(IOS_IPA_91PLAYER_IDENTIFIER, (DeviceInfo*)_pDevice->GetDevInfo());
		break;
	case eNDPlayerType_PXL:
		{
			if (_pDevice->GetDevInfo()->_connectType == WIFI)
				pFileStream = new IosWiFiFileSystem(_pDevice->GetDevInfo()->_ios_wifi_handle);
			else 
				pFileStream = new AFCFileSysemBase(_pDevice->GetDevInfo()->_ios_afcHandleApp, _pDevice->GetDevInfo());
		}
		break;
	}

	return pFileStream;
}

wstring IosVideoManager::GetVideoRootPath()
{
	wstring strVideoRootPath = L"";

	switch (_ePlayerType)
	{
	case eNDPlayerType_IPA:
		strVideoRootPath = L"/Documents/PandaPlayerHD";
		break;
	case eNDPlayerType_PXL:
		{
			if (_pDevice->GetDevInfo()->_connectType == USB)
				strVideoRootPath = L"/91Player";
			else
				strVideoRootPath = L"/var/mobile/Media/91Player";
		}
		break;
	}

	return strVideoRootPath;
}

wstring IosVideoManager::GetVideoBookingPath()
{
	wstring strVideoBookingPath = GetVideoRootPath();

	if (strVideoBookingPath.length() > 0 )
		strVideoBookingPath += L"/Vlist";

	return strVideoBookingPath;
}

wstring IosVideoManager::GetVideoNoBookingPath()
{
	wstring strVideoBookingPath = GetVideoRootPath();

	if (strVideoBookingPath.length() > 0 )
		strVideoBookingPath += L"/Video";

	return strVideoBookingPath;
}

bool IosVideoManager::IsValidVideo(wstring strName)
{
// 	wstring strEndType[9] = {
// 		L".mp3",
// 		L".mp4",
// 		L".mpv",
// 		L".3gp",
// 		L".mov",
// 		L".m4r",
// 		L".wav",
// 		L".aac",
//      L".ndv"
// 	};

    wstring strEndType[5] = {
        L".mp4",
        L".mpv",
        L".3gp",
        L".mov",
        L".ndv"
    };

	bool bRet = false;
	wstring strNameTemp = CStrOperation::toLowerW(strName);
	for (int i = 0; i < 5; i++)
	{
		if (CStrOperation::endsWithW(strNameTemp, strEndType[i]))
		{
			bRet = true;
			break;
		}
	}

	return bRet;
}

VideoData* IosVideoManager::GetVideoData(NodeData* pFile, PlayAlbumData* pAlbumData)
{
	RETURN_NULL_IF(!pFile);
	RETURN_NULL_IF(pFile->_type == NodeData::DIR);
	RETURN_NULL_IF(!IsValidVideo(pFile->_name));

	VideoData* pVideoData = new VideoData((FileData*)pFile, pAlbumData);

	return pVideoData;
}

void IosVideoManager::GetVideosFromPath(deque<VideoData*>& vtMyVideoList, wstring strPath, bool bIncludeDir, PlayAlbumData* pAlbumData)
{
	RETURN_IF(!_pFileStream);

	RETURN_IF(!_pFileStream->Ios_Exists(strPath));

	DirData resultDir(_pDevice);
	resultDir._name = CStrOperation::trimRightW(strPath, L"/") + L"/";
	if (_pFileStream->Ios_GetFileInfos(&resultDir, eFileGetType_All))
	{
		deque<NodeData*> childs = resultDir.GetChildrensNoReload();
		for ( size_t i=0; i < childs.size(); ++i )
		{
			NodeData* pFile = childs[i];
			if (!pFile)
				continue;

			if (pFile->_type == NodeData::DIR && bIncludeDir)
				GetVideosFromPath(vtMyVideoList, pFile->GetFullPath(), bIncludeDir);
			else if (pFile->_type == NodeData::FILE)
			{
				VideoData* pVideoData = GetVideoData(pFile, pAlbumData);
				if (pVideoData)
					vtMyVideoList.push_back(pVideoData);
			}
		}
	}
}



#endif