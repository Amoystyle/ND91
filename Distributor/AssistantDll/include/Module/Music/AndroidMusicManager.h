#pragma once

#include "Module/Music/AlbumData.h"
#include "Module/Music/AudioData.h"
#include "Module/Music/PlayItemData.h"
#include "Module/Music/PlayListData.h"

namespace ND91Assistant
{
// ���ֹ�����
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

    // ��������,���������������б��ר��
	bool Load();

    // ֹͣ����
	bool StopPlay();

	// Summary  : ��Ӹ���
	// Returns	: int ʵ����ӵĸ�������
	// Parameter: deque<wstring> localPaths���ظ���·������
	int AddTrack(deque<wstring> localPaths,ProgressObserver* pObserver = NULL);

	// Summary  : ��Ӹ������ظ���id
	// Returns	: int
	// Parameter: wstring localPath
	int AddTrack(wstring localPath, wstring remotePath=L"");

    // Summary  : �Ƴ�����
    // Returns	: bool
    // Parameter: AudioData * audio
    bool RemoveTrack(AudioData* audio);

	// Summary  : ��Ӳ����б�
	// Returns	: bool
	// Parameter: wstring name
	PlaylistData* AddPlayList(wstring name);


	bool AddTrackToPlayList(int playlistId, int trackId);

	// Summary  : ɾ�������б�
	// Returns	: bool
	// Parameter: PlaylistData * playlist
	bool DeletePlaylist(PlaylistData* playlist);

    // �������豸����

	// ��ȡ��������
	const deque<AudioData*>* GetAllTracks(){return &_tracks;}

    // ��ȡ�����б����
	const deque<PlaylistData*>* GetAllPlayLists(){return &_playlists;}

	// Summary  :��ȡר������
	// Returns	: deque<AlbumData*>* ר������
	const deque<AlbumData*>*  GetAllAlbums(){return &_Albums;}

	// Summary  : ͨ������id�ҵ�����
	// Returns	: AudioData*
	// Parameter: int trackId
	AudioData* findAudioById(int trackId);


	// Summary  : ͨ��·���Ҹ���
	// Returns	: AudioData*
	// Parameter: wstring remotePath
	AudioData* findAudioByRemotePath(wstring remotePath);


	// Summary  : ͨ��ר������ȡר��
	// Returns	: AlbumData*
	// Parameter: wstring albumName
	AlbumData* findAlbumByName(wstring albumName);

	// Summary  : �ͷ�
	// Returns	: void
	void Release();

    //��ȡ�������������ڿ��ܶԴ˽����Ż����ʶ�������
    int GetMusicCount();
private:
	// Summary  : ����ר����ר�������в������򴴽��������ط���
	// Returns	: AlbumData* ר������Ӧ��ר��
	// Parameter: wstring albumName ר����
	AlbumData* InsertAlbum(wstring  albumName);

	// Summary  : ���벥���б�����
	// Returns	: void
	// Parameter: PlaylistData * playlist
	void InsertPlaylist(PlaylistData* playlist);

	// Summary  : �������������
	// Returns	: void
	// Parameter: AudioData * audio
	void InsertTrack(AudioData* audio);

	// Summary  : �Ӷ������Ƴ�ר��
	// Returns	: void
	// Parameter: AlbumData * album
	void RemoveAlbumFromlist(AlbumData* album);

	// Summary  : �Ӷ������Ƴ�����
	// Returns	: void
	// Parameter: AudioData * audio
	void RemoveTrackFromlist(AudioData* audio);

	// Summary  : �Ӷ������Ƴ������б�
	// Returns	: void
	// Parameter: PlaylistData * playlist
	void RemovePlaylistFromList(PlaylistData* playlist);

private:

	deque<PlaylistData*>  _playlists;// �����б�

	deque<AlbumData*>     _Albums;   // ר��

	deque<AudioData*>     _tracks;   // ����

	const DeviceData*     _pDevice;
};

}
