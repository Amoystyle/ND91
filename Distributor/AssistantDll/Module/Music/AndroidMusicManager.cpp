#include "pch_module.h"

#ifdef MODULE_MUSIC

#include "Module/Music/AndroidMusicManager.h"
#include "Module/Music/LoadTracksCommand.h"
#include "Module/Music/LoadPlaylistCommand.h"
#include "Module/Music/MusicPlayCommand.h"
#include "Module/Music/AddTrackCommand.h"
#include "Module/Music/MusicSimpleCommand.h"
#include "Module/Music/PlaylistOperationCommand.h"

#include "Module/Common/Media/RealMedia/MP3Data.h"

#define REMOTE_MUSIC_DIR L"/sdcard/music/"

AndroidMusicManager::AndroidMusicManager(const DeviceData* pDevic)
{
	_pDevice = pDevic;
}

AndroidMusicManager::~AndroidMusicManager(void)
{
	Release();
}


bool ND91Assistant::AndroidMusicManager::Load()
{
	Release();
	LoadTracksCommand cmdLoadTrack(_pDevice, this);
	cmdLoadTrack.Execute();

	LoadPlaylistCommand cmdLoadPlaylist(_pDevice, this);
	cmdLoadPlaylist.Execute();

    return cmdLoadTrack.Result() || cmdLoadPlaylist.Result();
}

bool ND91Assistant::AndroidMusicManager::StopPlay()
{
	MusicPlayCommand cmd(_pDevice);
	return cmd.Stop();
}

int ND91Assistant::AndroidMusicManager::AddTrack(deque<wstring> localPaths,ProgressObserver* pObserver)
{
	if(localPaths.empty())
		return false;

	UploadFileCommand  upFileCmd(_pDevice);
    upFileCmd.Attach(pObserver);

    int sizeSum = 0;
    for (size_t i = 0; i < localPaths.size(); i++)
        sizeSum += CFileOperation::GetFileSizeW(localPaths[i]);
    // 设置所有任务总量
    upFileCmd.SetAllTaskSum(sizeSum);
    // 设置任务总数
    upFileCmd.SetTaskNumberSum(localPaths.size());

    size_t count = 0;
    for(size_t i = 0; i < localPaths.size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)

        wstring localPath = localPaths.at(i);

	    // 仅支持.mp3
	    int pos = localPath.find_last_of(L".");
	    wstring  ext = L"";
	    if(pos > 0)
		    ext = localPath.substr(pos+1);
	    if (CStrOperation::toLowerW(ext) != L"mp3" && !MP3Data::IsValidMp3(localPath))
            continue;

	    upFileCmd.setfilename(localPath, REMOTE_MUSIC_DIR+CFileOperation::GetFileName(localPath));
	    upFileCmd.Execute();
	    switch(upFileCmd.GetUploadResult())
	    {
		    case Transfer_Success:
			    {
				    AddTrackCommand	 addTrackCmd(_pDevice, this);
				    addTrackCmd.SetParam(localPath, upFileCmd.GetDestFilePath());
				    addTrackCmd.Execute();
                    if(addTrackCmd.Success())
                        count++;
			    }
			    break;
		    case Transfer_Cancel:
                return (int)count;
			    break;
		    default:
			    break;
	    }

        upFileCmd.IncreaseTaskNumberComplete();
        upFileCmd.SendProgress();

        upFileCmd.SetCurrentTaskSum(1);
        upFileCmd.SetCurrentTaskName(CFileOperation::GetFileName(localPath));

        upFileCmd.AddTaskProgress(1);
        upFileCmd.SendProgress();
    }
    return (int)count;
}

int ND91Assistant::AndroidMusicManager::AddTrack( wstring localPath, wstring remotePath)
{
	int pos = localPath.find_last_of(L".");
	wstring  ext = L"";
	if(pos > 0)
		ext = localPath.substr(pos+1);
	/*if (CStrOperation::toLowerW(ext) != L"mp3" && !MP3Data::IsValidMp3(localPath))
		return 0;*/

	DirectoryOperationCommand dircmd(_pDevice);

	wstring remote = remotePath;
	if(remote.empty()||!dircmd.AndroidCreateDir(CFileOperation::GetFileDir(remotePath)))
		remote = REMOTE_MUSIC_DIR+CFileOperation::GetFileName(localPath);

	UploadFileCommand  upFileCmd(_pDevice);
	if(!dircmd.Exist(remote))
	{
		upFileCmd.setfilename(localPath, remote);
		upFileCmd.Execute();
		if(upFileCmd.GetUploadResult()!=Transfer_Success)
			return 0;
	}

	AddTrackCommand	 addTrackCmd(_pDevice, this);
	addTrackCmd.SetParam(localPath, upFileCmd.GetDestFilePath());
	addTrackCmd.Execute();
	AudioData* pRet = addTrackCmd.GetReturnAudio();
	if(pRet)
		return pRet->Id();

	return 0;
}

bool ND91Assistant::AndroidMusicManager::RemoveTrack( AudioData* audio )
{

	MusicSimpleCommand cmd(_pDevice);
	bool Success = cmd.RemoveTrack(audio);
	if(Success)
	{
		// 删除文件
		DirectoryOperationCommand dircmd(_pDevice);
		dircmd.DeleteFileLink(audio->GetFullPath());

		// 专辑中移除
		if (audio->GetAlbum())
		{
			audio->GetAlbum()->RemoveTrack(audio->Id());
			if(audio->GetAlbum()->GetTracks()->size()<1)
			{
				RemoveAlbumFromlist(audio->GetAlbum());
			}
		}
		// 播放列表中移除
		for (size_t i=0; i< _playlists.size(); i++)
		{
			_playlists[i]->RemoveItemByTrackId(audio->Id());
		}

		RemoveTrackFromlist(audio);
	}
	return Success;
}


PlaylistData* ND91Assistant::AndroidMusicManager::AddPlayList( wstring name )
{
	RETURN_NULL_IF(name.empty());
	// 与系统的名称相同不添加
	if(CStrOperation::ContainsInW(SYSTEM_PlAYLIST_NAMES, CStrOperation::toLowerW(name), L","))
		return NULL;

	PlaylistOperationCommand cmd(_pDevice);
	PlaylistData* playlist = cmd.AddPlaylist(name);
	PlaylistData* newplaylist=NULL;
	if(playlist)
	{	
		newplaylist = new PlaylistData(playlist);
		InsertPlaylist(newplaylist);
	}
    return newplaylist;
}

bool ND91Assistant::AndroidMusicManager::DeletePlaylist( PlaylistData* playlist )
{
	PlaylistOperationCommand cmd(_pDevice);
	bool Success = cmd.RemovePlaylist(playlist);
	if (Success)
		RemovePlaylistFromList(playlist);
	return Success;
}


void ND91Assistant::AndroidMusicManager::InsertPlaylist( PlaylistData* playlist )
{
	for (size_t i =0; i < _playlists.size(); i++)
	{
		if(_playlists[i]->Id() == playlist->Id())
		{
			SAFE_DELETE(_playlists[i]);
			_playlists[i] = playlist;
			return;
		}
	}
	_playlists.push_back(playlist);
}

AlbumData* ND91Assistant::AndroidMusicManager::InsertAlbum(wstring  albumName)
{
	wstring name =albumName.empty()?L"<unknown>":albumName;
	AlbumData* pAlbum = findAlbumByName(name);
	if (pAlbum)
		return pAlbum;
	pAlbum = new AlbumData();
	pAlbum->_albumName = name;
	_Albums.push_back(pAlbum);
	return pAlbum;
}

void ND91Assistant::AndroidMusicManager::InsertTrack( AudioData* audio)
{
	for (size_t i =0; i < _tracks.size(); i++)
	{
		if(_tracks[i]->Id() ==audio->Id())
		{
			SAFE_DELETE(_tracks[i]);
			_tracks[i] = audio;
			return;
		}
	}
	_tracks.push_back(audio);
}

void ND91Assistant::AndroidMusicManager::RemoveTrackFromlist( AudioData* audio )
{
	for (deque<AudioData*>::iterator it =_tracks.begin(); it != _tracks.end(); ++it)
	{
		if((*it)->Id() ==audio->Id())
		{
			SAFE_DELETE((*it));
			_tracks.erase(it);
			return;
		}
	}
}

void ND91Assistant::AndroidMusicManager::RemoveAlbumFromlist( AlbumData* album )
{
	for (deque<AlbumData*>::iterator it =_Albums.begin(); it != _Albums.end(); ++it)
	{
		if((*it)->_albumName == album->_albumName)
		{
			SAFE_DELETE((*it));
			_Albums.erase(it);
			return;
		}
	}
}

void ND91Assistant::AndroidMusicManager::RemovePlaylistFromList( PlaylistData* playlist )
{
	for (deque<PlaylistData*>::iterator it =_playlists.begin(); it != _playlists.end(); ++it)
	{
		if((*it)->Id() == playlist->Id())
		{
			SAFE_DELETE((*it));
			_playlists.erase(it);
			return;
		}
	}
}

void AndroidMusicManager::Release()
{
	for (size_t i=0; i<_tracks.size(); i++)
	{
		SAFE_DELETE(_tracks[i]);
	}
	_tracks.clear();

	for (size_t j =0; j < _playlists.size(); j++)
	{
		SAFE_DELETE(_playlists[j]);
	}
	_playlists.clear();

	for(size_t k=0; k<_Albums.size(); k++)
	{
		SAFE_DELETE(_Albums[k]);
	}
	_Albums.clear();
}

AlbumData* ND91Assistant::AndroidMusicManager::findAlbumByName( wstring albumName )
{
	for (size_t i =0; i < _Albums.size(); i++)
	{
		if(_Albums[i]->_albumName == albumName)
		{
			return _Albums[i];
		}
	}
	return NULL;
}

AudioData* ND91Assistant::AndroidMusicManager::findAudioById( int trackId )
{
	for (size_t i =0; i < _tracks.size(); i++)
	{
		if(_tracks[i]->Id() == trackId)
		{
			return _tracks[i];
		}
	}
	return NULL;
}

AudioData* ND91Assistant::AndroidMusicManager::findAudioByRemotePath( wstring remotePath )
{
	for (size_t i =0; i < _tracks.size(); i++)
	{
		if(_tracks[i]->GetFullPath() == remotePath)
		{
			return _tracks[i];
		}
	}
	return NULL;
}

int ND91Assistant::AndroidMusicManager::GetMusicCount()
{
    int nCount = 0;
    if (_pDevice)
    {
        if (_pDevice->GetDevInfo()->_deviceType == Ios)
        {

        }
        else if (_pDevice->GetDevInfo()->_deviceType == Android)
        {
            nCount += LoadTracksCommand(_pDevice,NULL).GetTrackCounts();
        }

    }
    return nCount;
}

bool ND91Assistant::AndroidMusicManager::AddTrackToPlayList(int playlistId, int trackId )
{
	for (deque<PlaylistData*>::iterator it =_playlists.begin(); it != _playlists.end(); ++it)
	{
		if((*it)->Id()== playlistId)
		{
			return (*it)->AddTrack(trackId);
		}
	}
	return false;
}



#endif