#pragma once
#include "AndroidMusicManager.h"
using namespace std;
namespace ND91Assistant
{
	class CLASSINDLL_CLASS_DECL MusicTrackData
	{
	public:		
		//�����ļ����ֻ��е�·��
		wstring _MobileFilePath;
		//ʹ�õ�ר��id
		int _artwork_cache_id;
		//GetAllTracks���صľ�id����MusicPlayListData���_playlistTrackIDs����
		long _playlistTrackID;
		//AddTrack��������id������Ҫ����
		long _NewTrackid;
	};
	class CLASSINDLL_CLASS_DECL  MusicPlayListData
	{
	public:
		MusicPlayListData();
		~MusicPlayListData();
		//�����б���
		wstring _PlayListName;
		//�����б������������ĸ���playlistTrackID�б�
		vector<long> _playlistTrackIDs;
		//AddPlayList��������id������Ҫ����
		long _PlayListID;

	};
	class CLASSINDLL_CLASS_DECL  MusicArtworkData
	{
	public:
		//ʹ�õ�ר��id
		int _artwork_cache_id;
		//��������
		Bitmap * CoverData;
	};
	//���ֱ��ݻ�ԭ���ߴ�һ̨�豸ת�Ƶ���һ̨�豸��ʹ�õĽӿ�
	class CLASSINDLL_CLASS_DECL MusicManager
	{
	public:
		MusicManager(DeviceData* pDeviceData);
		~MusicManager();
		// ��ȡ��������
		const vector<MusicTrackData*>* GetAllTracks();
		// ��ȡ�����б����
		const vector<MusicPlayListData*>* GetAllPlayLists();
		//��ȡ��������
		const vector<MusicArtworkData*>* GetAllArtworks();
		//������ַ��ص�ֵ���µ�id����Ҫ���浽MusicTrackData��_NewAddid���ں�����Ӳ����б����
		long AddTrack(wstring TrackPath,wstring remotePath=L"",Bitmap * CoverData = NULL);
		//��Ӳ����б���ֵΪ�µĲ����б�id����Ҫ���浽MusicPlayListData���_PlayListID�����ں�����Ӳ����б����
		long AddPlayList(wstring playListName);
		//��������ӵ������б���,TrackID����AddTrack���ص�id
		void AppTrackToPlayList(MusicPlayListData* playList,long NewTrackID);
		//����ֻ�������
		void ClearAllMusicData();
		//���ݲ����б��ﱣ���PlayListTasckID����²�����TrackId
		long GetNewidByPlayListTaskId(const vector<MusicTrackData*>* TaskList,long _playlistTrackID);
		
		CommandExecuteResult load();
	
		AndroidMusicManager* getAndroidMusicManager(){return _pAndroidMusicMgr;}

	private:
		void Release();

	private:
		//�豸
		DeviceData* _pDeviceData;
		//�����б�
		vector<MusicTrackData*> _TrackList;
		//�����б��б�
		vector<MusicPlayListData*> _PlayList_List;
		//����
		vector<MusicArtworkData*> _ArtworkList;
		int _iProductVersion;

		AndroidMusicManager*  _pAndroidMusicMgr;
	};
}