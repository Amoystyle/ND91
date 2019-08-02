#include "pch_module.h"

#include "Module/Music/MusicManager.h"
#include "Module/iPod/VCiPodFileSync.h"
#include "Module/iPod/Ios5iPodManager.h"
#include "Module/iPod/iPodManager.h"

#include <atlconv.h>

#pragma warning(disable: 4244)

template<class T>
void DeleteVector(vector<T*> p)
{
	for(size_t i = 0; i < p.size(); ++i)
		SAFE_DELETE(p[i]);
	p.clear();	
}
MusicPlayListData::MusicPlayListData()
{
}
MusicPlayListData::~MusicPlayListData()
{
	_playlistTrackIDs.clear();
}

MusicManager::MusicManager(DeviceData* pDeviceData)
{
	_pDeviceData = pDeviceData;
	_pAndroidMusicMgr = NULL;
	_iProductVersion =0;
	if(_pDeviceData->GetDevInfo()->_deviceType == Ios)
	{
		_iProductVersion = IosPublicFunc::GetVerNum(_pDeviceData->GetDevInfo()->_deviceProductVersion);
	}
	
	 
}
CommandExecuteResult MusicManager::load()
{
	if(!_pDeviceData)
		return COMMAND_EXECUTE_FAIL;
	Release();
	if(_pDeviceData->GetDevInfo()->_deviceType == Ios)
	{
		_iProductVersion = IosPublicFunc::GetVerNum(_pDeviceData->GetDevInfo()->_deviceProductVersion);
		wstring workpath = CFileOperation::GetCurrWorkingDir();
		workpath+=L"itunes_control";

		vector<NodeData*> nodes;
		DirData root(_pDeviceData);
		NodeData* pDir = FileHelper::GetNode(&root,  L"/private/var/mobile/Media/iTunes_Control/Artwork");
		NodeData* pDir1 = FileHelper::GetNode(&root,  L"/private/var/mobile/Media/iTunes_Control/iTunes");        

		if(pDir1)
			nodes.push_back(pDir1);
		if(pDir)
			nodes.push_back(pDir);

		// 下载到此目录
		CFileOperation::DeleteDirectory(workpath+L"\\iTunes");
		CFileOperation::DeleteDirectory(workpath+L"\\Artwork");
		FileHelper::Download(&nodes, workpath, NULL);

		USES_CONVERSION;
		iPodFileSync::InitFileSync(_pDeviceData,workpath);
		try
		{
			if(_iProductVersion >=500)
			{

				ciPodManager5::Init(workpath,_iProductVersion,0);
			}
			else
			{
				ciPodManager::Init(workpath,_iProductVersion);
			}
		}
		catch(...)
		{
			return COMMAND_EXECUTE_FAIL;
		}
	}
	else
	{
		_iProductVersion = 0;
		if(!_pAndroidMusicMgr)
			_pAndroidMusicMgr = new AndroidMusicManager(_pDeviceData);
		if (!_pAndroidMusicMgr->Load())
            return COMMAND_EXECUTE_FAIL;
	}
    return COMMAND_EXECUTE_SUCCESS;
}

MusicManager::~MusicManager()
{
	Release();
	SAFE_DELETE(_pAndroidMusicMgr);
}

void MusicManager::Release()
{
	DeleteVector(_TrackList);
	DeleteVector(_PlayList_List);
	DeleteVector(_ArtworkList);

	if(_pDeviceData->GetDevInfo()->_deviceType == Ios)
	{
		if(_iProductVersion<500)
			ciPodManager::DestroyInstance();
		else
			ciPodManager5::DestroyInstance();
	}
}

const vector<MusicTrackData*>* MusicManager::GetAllTracks()
{
	DeleteVector(_TrackList);
	 if(!_pDeviceData)
		 return &_TrackList;
	 if(_pDeviceData->GetDevInfo()->_deviceType == Ios)
	 {
		 if(_iProductVersion<500)
		 {	
			 ciPodManager* Manager = ciPodManager::GetInstance();
			 const CIosiPodData* iPodata = Manager->GetPiPodCDBData();	
			 for (size_t i = 0; i < iPodata->GetTrackCount(); i++)
			 {
				 CIosCDBTrackData* pTrack = iPodata->GetTrackData(i);
				MusicTrackData* track = new MusicTrackData();
				track->_playlistTrackID = pTrack->Id();
				if( _pDeviceData->GetDevInfo()->_ios_bJailBreaked)
					track->_MobileFilePath = L"/private/var/mobile/Media/";
				else
					track->_MobileFilePath = L"";
				track->_MobileFilePath +=  CStrOperation::ReplaceW(pTrack->Devicepath(),L":",L"/");
				_TrackList.push_back(track);
			 }
		 }
		 else
		 {
			 ciPodManager5* Manager = ciPodManager5::GetInstance();
			 const CIosiPodData* iPodata = Manager->GetPiPodCDBData();	
			 for (size_t i = 0; i < iPodata->GetTrackCount(); i++)
			 {
				 CIosCDBTrackData* pTrack = iPodata->GetTrackData(i);
				 MusicTrackData* track = new MusicTrackData();
				 track->_playlistTrackID = pTrack->Id();
				if( _pDeviceData->GetDevInfo()->_ios_bJailBreaked)
					track->_MobileFilePath = L"/private/var/mobile/Media/";
				else
					track->_MobileFilePath = L"";
				 track->_MobileFilePath +=  CStrOperation::ReplaceW(pTrack->Devicepath(),L":",L"/");
				 _TrackList.push_back(track);
			 }
		 }
	 }
	 else
	 {
		 const deque<AudioData*>* tracks=_pAndroidMusicMgr->GetAllTracks();
		 for (size_t i =0; i < tracks->size();i++)
		 {
			 AudioData* data=tracks->at(i);
			 MusicTrackData* track = new MusicTrackData();
			 track->_playlistTrackID = data->Id();
			 track->_MobileFilePath = data->GetFullPath();
			 _TrackList.push_back(track);
		 }
	 }
	return &_TrackList;
}
const vector<MusicPlayListData*>* MusicManager::GetAllPlayLists()
{
	DeleteVector(_PlayList_List);
	if(!_pDeviceData)
		return &_PlayList_List;
	if(_pDeviceData->GetDevInfo()->_deviceType == Ios)
	{
		if(_iProductVersion>=500)
		{
			ciPodManager5* Manage = ciPodManager5::GetInstance();		
			const cIPodData5* piPodData = Manage->GetPiPodData();		
			for (size_t i = 0; i <piPodData->GetPlayListCount(); i++)
			{
				CIosCDBPlayList* playlist = piPodData->GetPlayListData(i);
				MusicPlayListData* Musicplaylist = new MusicPlayListData();
				Musicplaylist->_PlayListName = playlist->Listname();
				for(size_t j = 0; j< playlist->TrackList()->size();j++)
					Musicplaylist->_playlistTrackIDs.push_back(playlist->TrackList()->at(j));
				_PlayList_List.push_back(Musicplaylist);
			}
			
		}
		else
		{
			ciPodManager* Manager = ciPodManager::GetInstance();
			const CIosiPodData* piPodData = Manager->GetPiPodCDBData();
			for (size_t i = 0; i <piPodData->GetPlayListDBCount(); i++)
			{
				CIosCDBPlayList* playlist = piPodData->GetPlayListDBData(i);
				MusicPlayListData* Musicplaylist = new MusicPlayListData();
				Musicplaylist->_PlayListName = playlist->Listname();
				for(size_t j = 0; j< playlist->TrackList()->size();j++)
					Musicplaylist->_playlistTrackIDs.push_back(playlist->TrackList()->at(j));
				_PlayList_List.push_back(Musicplaylist);
			}			
		}
	}
	else
	{
		const deque<PlaylistData*>* playlists=_pAndroidMusicMgr->GetAllPlayLists();
		for (size_t i = 0; i <playlists->size(); i++)
		{
			MusicPlayListData* Musicplaylist = new MusicPlayListData();
			Musicplaylist->_PlayListName = playlists->at(i)->Name();
			deque<PlayItemData>* playitems=playlists->at(i)->GetPlaylistItems();
			for(size_t j = 0; j< playitems->size();j++)
				Musicplaylist->_playlistTrackIDs.push_back(playitems->at(j)._trackId);
			_PlayList_List.push_back(Musicplaylist);
		}	
	}
	return &_PlayList_List;
}
long MusicManager::AddTrack(wstring TrackPath,wstring remotePath,Bitmap * CoverData )
{
	long id=0;
	if(!_pDeviceData)
		return 0;
	if(_pDeviceData->GetDevInfo()->_deviceType == Ios)
	{
		if(_iProductVersion>=500)
		{
			ciPodManager5* Manage = ciPodManager5::GetInstance();			
			CIosCDBTrackData* track = Manage->InsertNewTrack(TrackPath);	
			if(track)
				id=track->Id();
		}
		else
		{
			ciPodManager* Manager = ciPodManager::GetInstance();
			CIosCDBTrackData* track = Manager->InsertNewTrack(TrackPath);
			if(track)
				id=track->Id();
		}
	}
	else
	{
		id = _pAndroidMusicMgr->AddTrack(TrackPath,remotePath);
	}
	return id;
}
long MusicManager::AddPlayList(wstring playListName)
{
	long id=0;
	if(!_pDeviceData)
		return 0;
	if(_pDeviceData->GetDevInfo()->_deviceType == Ios)
	{
		CIosCDBPlayList* playlist;
		if(_iProductVersion>=500)
		{
			ciPodManager5* Manage = ciPodManager5::GetInstance();
			Manage->CreatePlayList(playListName);
			const cIPodData5* piPodData = Manage->GetPiPodData();
			 playlist = piPodData->GetPlayListData(piPodData->GetPlayListCount()-1);
			 if(playlist)
				 id = playlist->ID();
		}
		else
		{
			ciPodManager* Manager = ciPodManager::GetInstance();
			const CIosiPodData* piPodData = Manager->GetPiPodCDBData();
			playlist = piPodData->GetPlayListDBData(piPodData->GetPlayListDBCount()-1);
			if(playlist)
				id = playlist->ID();
		}
	}
	else
	{
		PlaylistData* plist=_pAndroidMusicMgr->AddPlayList(playListName);
		if(plist)
			id = plist->Id();
	}
	return id;
}
void MusicManager::AppTrackToPlayList(MusicPlayListData* playList,long TrackID)
{
	if(!_pDeviceData)
		return;
	if(_pDeviceData->GetDevInfo()->_deviceType == Ios)
	{
		if(_iProductVersion<500)
		{
			ciPodManager* Manager = ciPodManager::GetInstance();		
			Manager->AddToPlayList(TrackID,playList->_PlayListName);
		}
		else
		{
			ciPodManager5* Manage = ciPodManager5::GetInstance();				
			vector<__int64> Tracks;
			Tracks.push_back(TrackID);
			Manage->AddToPlayList(playList->_PlayListName,Tracks);
		}
	}
	else
	{
		_pAndroidMusicMgr->AddTrackToPlayList(playList->_PlayListID, TrackID);
	}
}
void MusicManager::ClearAllMusicData()
{
	
}
long MusicManager::GetNewidByPlayListTaskId(const vector<MusicTrackData*>* TaskList,long _playlistTrackID)
{
	for(size_t i=0;i<TaskList->size();i++)
	{
		MusicTrackData* track = TaskList->at(i);
					if(track->_playlistTrackID == _playlistTrackID)
			{
				return track->_NewTrackid;
			}
	}
	return -1;
}