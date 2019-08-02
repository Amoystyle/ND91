#pragma once

namespace ND91Assistant
{
class AndroidMusicManager;
class AudioData;

// 获取所有歌曲命令
class LoadTracksCommand :public TableQueryCommand
{
public:
	LoadTracksCommand(const DeviceData* pDevice, AndroidMusicManager* pmusic);
	virtual ~LoadTracksCommand(void);

	// 执行命令
	virtual void Execute();

	// Summary  : 获取所有歌曲
	// Returns	: const deque<AudioData*>* 歌曲队列指针
	const deque<AudioData*>* GetAudios(){return &_audios;}

	int GetTrackCounts();

protected:
	// 解析返回数据包
	virtual void SetAndroidReturnData(std::string data);

private:

	deque<AudioData*>  _audios;//歌曲队列

	AndroidMusicManager*      _pMusic;

	int		_trackCount;
	bool	_getCount;
};

}
