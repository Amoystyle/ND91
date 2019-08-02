#pragma once

#include "Module/TableQueryCommand.h"

namespace ND91Assistant
{
// ��ȡ���в����б�
class AndroidMusicManager;
class PlaylistData;
class LoadPlaylistCommand :public TableQueryCommand
{
public:
	LoadPlaylistCommand(const DeviceData* pDevice, AndroidMusicManager* pmusic);
	virtual ~LoadPlaylistCommand(void);

	// ִ������
	virtual void Execute();

	// Summary  : ���ز����б�
	// Returns	: deque<PlaylistData*>* �����б����
	const deque<PlaylistData*>* GetPlaylist(){return &_playlists;}
protected:
	// �����������ݰ�
	virtual void SetAndroidReturnData(std::string data);

    deque<PlaylistData*>  _playlists; 

	AndroidMusicManager*         _pMusic;
};

}
