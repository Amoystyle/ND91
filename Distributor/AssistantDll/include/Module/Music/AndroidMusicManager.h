#pragma once

#include "Module/Music/AlbumData.h"
#include "Module/Music/AudioData.h"
#include "Module/Music/PlayItemData.h"
#include "Module/Music/PlayListData.h"

namespace ND91Assistant
{
// 音乐管理类
class PlaylistData;
class AlbumData;
class AudioData;
class DeviceData;
class CLASSINDLL_CLASS_DECL AndroidMusicManager
{
	friend class AudioData;
	friend class LoadTracksCommand;
	friend class LoadPlaylistCommand;
	friend class AddTrackCommand;
public:
	AndroidMusicManager(const DeviceData* pDevic);
	virtual ~AndroidMusicManager(void);

    // 加载音乐,包含歌曲，播放列表和专辑
	bool Load();

    // 停止播放
	bool StopPlay();

	// Summary  : 添加歌曲
	// Returns	: int 实际添加的歌曲数量
	// Parameter: deque<wstring> localPaths本地歌曲路径集合
	int AddTrack(deque<wstring> localPaths,ProgressObserver* pObserver = NULL);

	// Summary  : 添加歌曲返回歌曲id
	// Returns	: int
	// Parameter: wstring localPath
	int AddTrack(wstring localPath, wstring remotePath=L"");

    // Summary  : 移除歌曲
    // Returns	: bool
    // Parameter: AudioData * audio
    bool RemoveTrack(AudioData* audio);

	// Summary  : 添加播放列表
	// Returns	: bool
	// Parameter: wstring name
	PlaylistData* AddPlayList(wstring name);


	bool AddTrackToPlayList(int playlistId, int trackId);

	// Summary  : 删除播放列表
	// Returns	: bool
	// Parameter: PlaylistData * playlist
	bool DeletePlaylist(PlaylistData* playlist);

    // 不操作设备数据

	// 获取歌曲队列
	const deque<AudioData*>* GetAllTracks(){return &_tracks;}

    // 获取播放列表队列
	const deque<PlaylistData*>* GetAllPlayLists(){return &_playlists;}

	// Summary  :获取专辑队列
	// Returns	: deque<AlbumData*>* 专辑队列
	const deque<AlbumData*>*  GetAllAlbums(){return &_Albums;}

	// Summary  : 通过歌曲id找到歌曲
	// Returns	: AudioData*
	// Parameter: int trackId
	AudioData* findAudioById(int trackId);


	// Summary  : 通过路径找歌曲
	// Returns	: AudioData*
	// Parameter: wstring remotePath
	AudioData* findAudioByRemotePath(wstring remotePath);


	// Summary  : 通过专辑名获取专辑
	// Returns	: AlbumData*
	// Parameter: wstring albumName
	AlbumData* findAlbumByName(wstring albumName);

	// Summary  : 释放
	// Returns	: void
	void Release();

    //获取音乐数量，后期可能对此进行优化，故独立出来
    int GetMusicCount();
private:
	// Summary  : 插入专辑，专辑队列中不存在则创建，并下载封面
	// Returns	: AlbumData* 专辑名对应的专辑
	// Parameter: wstring albumName 专辑名
	AlbumData* InsertAlbum(wstring  albumName);

	// Summary  : 插入播放列表到队列
	// Returns	: void
	// Parameter: PlaylistData * playlist
	void InsertPlaylist(PlaylistData* playlist);

	// Summary  : 插入歌曲到队列
	// Returns	: void
	// Parameter: AudioData * audio
	void InsertTrack(AudioData* audio);

	// Summary  : 从队列中移除专辑
	// Returns	: void
	// Parameter: AlbumData * album
	void RemoveAlbumFromlist(AlbumData* album);

	// Summary  : 从队列中移除歌曲
	// Returns	: void
	// Parameter: AudioData * audio
	void RemoveTrackFromlist(AudioData* audio);

	// Summary  : 从队列中移除播放列表
	// Returns	: void
	// Parameter: PlaylistData * playlist
	void RemovePlaylistFromList(PlaylistData* playlist);

private:

	deque<PlaylistData*>  _playlists;// 播放列表

	deque<AlbumData*>     _Albums;   // 专辑

	deque<AudioData*>     _tracks;   // 歌曲

	const DeviceData*     _pDevice;
};

}
