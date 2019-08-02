#pragma once
#include "Module/BaseCommand.h"

namespace ND91Assistant
{

class PlaylistData;
class PlayItemData;
// 播放列表操作相关命令
class PlaylistOperationCommand :public BaseCommand
{
public:
	PlaylistOperationCommand(const DeviceData* pDevice);
	virtual ~PlaylistOperationCommand(void);

	// Summary  : 添加播放列表
	// Returns	: PlaylistData* 添加的播放列表
	// Parameter: wstring name 名称
	PlaylistData* AddPlaylist(wstring  name);

	// Summary  : 移除播放列表
	// Returns	: bool 成功失败
	// Parameter: PlaylistData * playlist 要移除的播放列表
	bool RemovePlaylist(PlaylistData* playlist);

	// Summary  : 重命名播放列表
	// Returns	: bool 成功失败
	// Parameter: PlaylistData * playlist 要重命名的播放列表
	// Parameter: string name 新名称
	bool RenamePlaylist(PlaylistData* playlist, wstring name);

	// Summary  : 添加歌曲到播放列表
	// Returns	: bool 成功失败
	// Parameter: PlaylistData * playlist 要添加到的播放列表
	// Parameter: AudioData * track 要添加的歌曲
	bool AddTrackToPlaylist(PlaylistData* playlist, int trackId);

	// Summary  : 从播放列表移除一项
	// Returns	: bool 成功失败
	// Parameter: PlayItemData * item 要移除的列表项
	bool RemoveFromPlaylist(PlayItemData* item);

	// Summary  : 移动播放列表项
	// Returns	: bool 成功失败
	// Parameter: PlayItemData * item 要移动的列表项
	// Parameter: int newIndex 要移动到的新位置
	bool MovePlaylistItem(PlayItemData* item, int newIndex);


	virtual void Execute();

protected:

#pragma region 这6个方法是Command的主要内容，处理令格式与结果格式

	// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
	virtual std::string GetAndroidBodyData();

	// Summary  : 解析返回数据包
	// Returns	: void
	// Parameter: std::string data
	virtual void SetAndroidReturnData(std::string data);

	// Summary  : 解析返回数据包头
	// Returns	: bool
	// Parameter: BinaryBufferReader * pReader
	bool parseAndroidPackageHead(BinaryBufferReader* pReader);

	// 命令传送到IOS设备上的文件名（全路径, unicode）
	virtual std::wstring GetIosCommandFilePath(){return L"";}

	// 从IOS上获取的命令结果文件名（全路径, unicode）
	virtual std::wstring GetIosResultFilePath() {return L"";}

	// 生成ios文件。返回文件名（全路径, unicode），调用者负责删除文件
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// 解析ios设备的返回文件（全路径, unicode）并删除文件
	virtual void SetIosReturnFile(std::wstring filename) { }

private:

	enum CommandType
	{
		NOTYPE,	
        ADD_PLAYLSIT = 5,			// 添加播放列表
		RENAME_PLAYLIST = 6,		// 重命名播放列表
		REMOVE_PLAYLIST = 7,		// 移除播放列表
		ADD_TRACK_TO_PLAYLIST = 8,  // 添加歌曲到播放列表
		MOVE_PLAYLIST_ITEM = 9,		// 移动播放列表项
		REMOVE_FROM_PLAYLIST = 10,  // 从播放列表中移除播放列表项
	};

	CommandType		_cmdType;         // 命令类型

	PlaylistData*	_pSourcePlaylist; // 参数，源要操作的播放列表

	PlayItemData*	_pSourcePlayItem; // 参数，源要操作的播放列表项

	wstring         _Name;			  // 参数，要修改为的新名称

	int             _newIndex;        // 参数，要修改为的新索引

	int             _trackId;		  // 参数，要添加到播放列表的歌曲id


	PlaylistData*	_pReturnPlaylist; // 生成的播放列表

	bool            _success;		  // 操作成功失败标志

};

}