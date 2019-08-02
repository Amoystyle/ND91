#pragma once

#include "Module/TableQueryCommand.h"

namespace ND91Assistant
{
// 获取所有播放列表
class AndroidMusicManager;
class PlaylistData;
class LoadPlaylistCommand :public TableQueryCommand
{
public:
	LoadPlaylistCommand(const DeviceData* pDevice, AndroidMusicManager* pmusic);
	virtual ~LoadPlaylistCommand(void);

	// 执行命令
	virtual void Execute();

	// Summary  : 返回播放列表
	// Returns	: deque<PlaylistData*>* 播放列表队列
	const deque<PlaylistData*>* GetPlaylist(){return &_playlists;}
protected:
	// 解析返回数据包
	virtual void SetAndroidReturnData(std::string data);

    deque<PlaylistData*>  _playlists; 

	AndroidMusicManager*         _pMusic;
};

}
