#include "stdafx.h"
#ifdef MODULE_VIDEO

IosVideoManager::IosVideoManager(const DeviceData* pDevice)
{
	_pDevice = pDevice;
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

bool IosVideoManager::DownLoadFile(wstring strFileOnDev, wstring strFileOnPC, ProgressObserver* pObserver)
{
	RETURN_FALSE_IF(!_pFileStream);
	RETURN_FALSE_IF(!_pFileStream->Ios_Exists(strFileOnDev));

	ProgressSubject subject;
	subject.Attach(pObserver);
	subject.ResetProgressData();
	subject.SetTaskNumberSum(1);

	TransferResult bRet = _pFileStream->Ios_DownFromPhone(strFileOnDev, strFileOnPC, subject);
    if (bRet == Transfer_Success)
        return true;
    else 
        return false;
}

bool IosVideoManager::DeleteAlbum(wstring strAlbumName)
{
	RETURN_FALSE_IF(!_pFileStream);

	wstring strFolderOnPhone = GetVideoBookingPath() + L"/" + strAlbumName;
	bool bRet = IosPublicFunc::DeleteFolder(strFolderOnPhone, _pFileStream);

	return bRet;
}

bool IosVideoManager::DeleteVideoFile(VideoData* pVideoData)
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

bool IosVideoManager::LoadAlbumImage(wstring strLocalFullName, wstring strAlbumName)
{
	RETURN_FALSE_IF(!_pFileStream);
	wstring strAlbumImageFullName = GetVideoBookingPath() + L"/" + strAlbumName + L"/" + strAlbumName + L".jpg";
	RETURN_FALSE_IF(!_pFileStream->Ios_Exists(strAlbumImageFullName));
    return _pFileStream->Ios_DownFromPhone(strAlbumImageFullName, strLocalFullName) == Transfer_Success;
}

bool IosVideoManager::RenameVideoFile(VideoData* pVideoData, wstring strNewName)
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

eNDPlayerType IosVideoManager::GetPlayerType()
{
	eNDPlayerType eType = eNDPlayerType_NONE;
#ifdef MODULE_APP
	ProgressSubject subject;
	IosIPAAppOperation ipaAppOperation(_pDevice->GetDevInfo(), subject);
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
			pFileStream = new AFCFileSysemBase(_pDevice->GetDevInfo()->_ios_afcHandle, 
			_pDevice->GetDevInfo()->_ios_afcHandleApp, _pDevice->GetDevInfo()->_ios_bJailBreaked);

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
				pFileStream = new AFCFileSysemBase(_pDevice->GetDevInfo()->_ios_afcHandleApp, 
				_pDevice->GetDevInfo()->_ios_afcHandleApp, _pDevice->GetDevInfo()->_ios_bJailBreaked);
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