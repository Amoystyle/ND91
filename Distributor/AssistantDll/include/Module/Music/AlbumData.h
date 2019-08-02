#pragma once
#include "Module/BaseData.h"
#include <deque>

using namespace std;

#pragma warning(disable : 4251)
namespace ND91Assistant
{
class AudioData;
class DeviceData;
//专辑
class CLASSINDLL_CLASS_DECL AlbumData :public BaseData
{
public:
	AlbumData();
	virtual ~AlbumData(void);

	// Summary  : 获取专辑封面图
	// Returns	: wstring 本地存储路径
	// Parameter: const DeviceData * pDevice
	wstring  GetAblumImage(const DeviceData* pDevice);

	// Summary  : 设置专辑封面
	// Returns	: bool
	// Parameter: const DeviceData * pDevice
	// Parameter: const wstring remotepath
	bool  SetAlbumImage(const DeviceData* pDevice, const wstring remotepath);

	// Summary  : 移除专辑封面
	// Returns	: bool
	// Parameter: const DeviceData * pDevice
	bool  RemoveAlbumImage(const DeviceData* pDevice);

	// Summary  : 获取专辑中所有歌曲
	// Returns	: deque<AudioData*>*歌曲队列
	const deque<AudioData*>* GetTracks() {return &_audios;}

	// Summary  : 插入歌曲到专辑
	// Returns	: void
	// Parameter: AudioData * track
	void  InsertTrack(AudioData* track);

	// Summary  : 从专辑中移除一首歌曲
	// Returns	: void
	// Parameter: int trackId歌曲id
	void  RemoveTrack(int trackId);

public:

	wstring		_albumName;       // 专辑名
	wstring		_albumImagePath;  // 专辑封面

private:

	// Summary  : 专辑封面下载到本地的存储路径,供界面显示
	// Returns  : 本地文件名
	// Parameter: wstring localFolder 下载到此目录
	wstring  GetAlbumImageLocalSavePath(wstring localFolder);

private:

	deque<AudioData*>   _audios;  // 包含的歌曲
	bool       _hasLoad;

	wstring  _albumImagelocalPath;
};
}
