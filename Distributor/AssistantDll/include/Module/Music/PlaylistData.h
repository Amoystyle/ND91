#pragma once

#include "PlayItemData.h"

namespace ND91Assistant
{
class DeviceData;
class BinaryBufferReader;

#define SYSTEM_PlAYLIST_NAMES L"my favorite,recent play,recent download"

//  播放列表
class CLASSINDLL_CLASS_DECL PlaylistData : public BaseData
{
	friend class LoadPlaylistCommand;
	friend class PlaylistOperationCommand;
public:
	PlaylistData(const DeviceData* pDevice);
	virtual ~PlaylistData(void);

	PlaylistData(PlaylistData* playlist);

	int  Id(){return _id;}//播放列表id

	wstring Name(){return _name;}//播放列表名

    void SetName(wstring name) { _name = name;};

	// Summary  : 是否系统的
	// Returns	: bool
	bool IsSystem();

	// Summary  : 加载播放列表项
	// Returns	: bool
	bool Load();

	// Summary  : 重命名播放列表
	// Returns	: bool
	// Parameter: wstring name
	bool Rename(wstring name);

	// Summary  : 添加歌曲
	// Returns	: bool
	// Parameter: int trackId
	bool AddTrack(int trackId);

	// Summary  : 移除
	// Returns	: bool
	// Parameter: PlayItemData * item
	bool RemoveItem(PlayItemData* item);

	// Summary  : 清空播放列表
	// Returns	: bool
	bool RemoveAllItems();

	// Summary  : 移动播放列表项
	// Returns	: bool
	// Parameter: PlayItemData * item要移动的项
	// Parameter: int newIndex
	bool MoveItem(PlayItemData* item, int newIndex);

	// Summary  : 获得所有播放列表项
	// Returns	: deque<PlayItemData>*
	deque<PlayItemData>* GetPlaylistItems();


	// Summary  : 歌曲被移除，则从播放列表中移除包含该歌曲id的所有播放列表项
	// Returns	: void
	// Parameter: int trackId歌曲id
	void RemoveItemByTrackId(int trackId);

	// Summary  : 添加播放列表项到队列
	// Returns	: void
	// Parameter: PlayItemData item
	void AddPlayListItem(PlayItemData item);

private:
	// Summary  : 解析数据
	// Returns	: bool
	// Parameter: BinaryBufferReader & reader
	bool ParseAndroidData(BinaryBufferReader& reader);

private:

	int						_id;		// 播放列表id

	wstring					_name;		// 播放列表名称

	deque<PlayItemData>		_PlayItems; //播放列表项

	bool                    _hasLoad;

	const DeviceData*		_pDevice;
};

}
