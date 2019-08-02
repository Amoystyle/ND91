#pragma once
#include "AndroidMusicManager.h"
using namespace std;
namespace ND91Assistant
{
	class CLASSINDLL_CLASS_DECL MusicTrackData
	{
	public:		
		//音乐文件中手机中的路径
		wstring _MobileFilePath;
		//使用的专辑id
		int _artwork_cache_id;
		//GetAllTracks返回的旧id，跟MusicPlayListData里的_playlistTrackIDs关联
		long _playlistTrackID;
		//AddTrack产生的新id，不需要备份
		long _NewTrackid;
	};
	class CLASSINDLL_CLASS_DECL  MusicPlayListData
	{
	public:
		MusicPlayListData();
		~MusicPlayListData();
		//播放列表名
		wstring _PlayListName;
		//播放列表里面所包含的歌曲playlistTrackID列表
		vector<long> _playlistTrackIDs;
		//AddPlayList产生的新id，不需要备份
		long _PlayListID;

	};
	class CLASSINDLL_CLASS_DECL  MusicArtworkData
	{
	public:
		//使用的专辑id
		int _artwork_cache_id;
		//封面数据
		Bitmap * CoverData;
	};
	//音乐备份还原或者从一台设备转移到另一台设备所使用的接口
	class CLASSINDLL_CLASS_DECL MusicManager
	{
	public:
		MusicManager(DeviceData* pDeviceData);
		~MusicManager();
		// 获取歌曲队列
		const vector<MusicTrackData*>* GetAllTracks();
		// 获取播放列表队列
		const vector<MusicPlayListData*>* GetAllPlayLists();
		//获取封面数据
		const vector<MusicArtworkData*>* GetAllArtworks();
		//添加音乐返回的值是新的id，需要保存到MusicTrackData的_NewAddid用于后面添加播放列表歌曲
		long AddTrack(wstring TrackPath,wstring remotePath=L"",Bitmap * CoverData = NULL);
		//添加播放列表返回值为新的播放列表id，需要保存到MusicPlayListData里的_PlayListID，用于后面添加播放列表歌曲
		long AddPlayList(wstring playListName);
		//将音乐添加到播放列表中,TrackID传入AddTrack返回的id
		void AppTrackToPlayList(MusicPlayListData* playList,long NewTrackID);
		//清除手机上音乐
		void ClearAllMusicData();
		//根据播放列表里保存的PlayListTasckID获得新产生的TrackId
		long GetNewidByPlayListTaskId(const vector<MusicTrackData*>* TaskList,long _playlistTrackID);
		
		CommandExecuteResult load();
	
		AndroidMusicManager* getAndroidMusicManager(){return _pAndroidMusicMgr;}

	private:
		void Release();

	private:
		//设备
		DeviceData* _pDeviceData;
		//歌曲列表
		vector<MusicTrackData*> _TrackList;
		//播放列表列表
		vector<MusicPlayListData*> _PlayList_List;
		//封面
		vector<MusicArtworkData*> _ArtworkList;
		int _iProductVersion;

		AndroidMusicManager*  _pAndroidMusicMgr;
	};
}