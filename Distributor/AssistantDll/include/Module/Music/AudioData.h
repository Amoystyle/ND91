///////////////////////////////////////////////////////////
//  AudioData.h
//  Implementation of the Class AudioData
//  Created on:      01-六月-2011 9:42:49
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_03579EE0_4BC3_4e0d_9DF0_13EA0AF78F35__INCLUDED_)
#define EA_03579EE0_4BC3_4e0d_9DF0_13EA0AF78F35__INCLUDED_

#include "Module/File/FileData.h"

namespace ND91Assistant
{

	class AndroidMusicManager;
	class DeviceData;
	class AlbumData;
	class PlaylistData;
	class BinaryBufferReader;
    class RingData;
	// 歌曲属性及操作
    class CLASSINDLL_CLASS_DECL AudioData : public FileData
    {
		friend class LoadTracksCommand;
		friend class AddTrackCommand;
    public:
        AudioData(const DeviceData* pDevice, AndroidMusicManager* pMusic);
        virtual ~AudioData();

		int Id(){return _id;}//歌曲id

		wstring trackName()const{return _title;}//歌曲名

		wstring Artist()const{return _artist;}//艺术家

		int		Duration() const { return _duration; }//时长

		AlbumData* GetAlbum() const{ return _pOwnAlbum; }//所属专辑

		wstring GetFullPath()const{return _remote;}//歌曲文件路径

		bool    IsMusic(){return _isMusic;}//是否音乐标志,非音乐则不被加入到歌曲队列

		// Summary  : 播放
		// Returns	: bool
		bool Play();

		 // Summary  :修改专辑名和艺术家名
		 // Returns	: void
		 // Parameter: wstring albumName
		 // Parameter: wstring artist
		 void ModifyAlbumArtistName(wstring albumName, wstring artist);

		 // Summary  :修改歌曲名
		 // Returns	: void
		 // Parameter: wstring name
		 void ModifyName(wstring name);

		 // Summary  : 修改专辑封面
		 // Returns	: void
		 // Parameter: wstring localPath本地路径
		 void ModifyAlbumImage(wstring localPath);

		 // Summary  :添加到播放列表
		 // Returns	: bool
		 // Parameter: PlaylistData * playlist
		 bool AddToPlaylist(PlaylistData* playlist);


	private:
		// Summary  : 解析歌曲数据
		// Returns	: bool
		// Parameter: BinaryBufferReader & reader
		bool ParseAndroidData(BinaryBufferReader& reader);

	private:
		 int			_id;
		 bool			_isMusic;	// 是否音乐
		 int			_duration;	// 时长
		 wstring		_remote;	// 路径
		 wstring		_title;		// 标题
		 wstring		_artist;	// 艺术家

		 AlbumData*     _pOwnAlbum; // 所属专辑

		 const DeviceData*  _pDevice;
		 AndroidMusicManager*		_pMusic;

    };
}

#endif // !defined(EA_03579EE0_4BC3_4e0d_9DF0_13EA0AF78F35__INCLUDED_)
